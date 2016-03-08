#include "stdafx.h"
#include <cstdarg>
#include "OdbcConnection.h"

OdbcConnection::OdbcConnection()
	: m_connHandle(nullptr), m_envHandle(nullptr), m_bMarsEnabled(false), m_lock(new std::recursive_mutex())
{
}

bool OdbcConnection::isConnected() 
{
	Guard lock(m_lock);
	return (m_connHandle != nullptr);
}

bool OdbcConnection::isError() 
{
	Guard lock(m_lock);
	return (!m_odbcErrors.empty());
}

bool OdbcConnection::Connect(tstring & szDSN, tstring & szUser, tstring & szPass, bool bMarsEnabled)
{
	m_szDSN = szDSN;
	m_szUser = szUser;
	m_szPass = szPass;

	m_bMarsEnabled = bMarsEnabled;

	return Connect();
}

bool OdbcConnection::Connect()
{
	if (m_szDSN.empty())
		return false;

	Guard lock(m_lock);

	tstring szConn = _T("DSN=") + m_szDSN + _T(";");
	// Reconnect if we need to.
	if (isConnected())
		Disconnect();

	// Allocate enviroment handle
	if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_envHandle)))
	{
		ReportSQLError(SQL_HANDLE_ENV, m_envHandle, _T("SQLAllocHandle"), _T("Unable to allocate environment handle."));
		goto error_handler;
	}

	// Request ODBC3 support
	if (!SQL_SUCCEEDED(SQLSetEnvAttr(m_envHandle, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0)))
	{
		ReportSQLError(SQL_HANDLE_ENV, m_envHandle, _T("SQLSetEnvAttr"), _T("Unable to set environment attribute (SQL_ATTR_ODBC_VERSION)."));
		goto error_handler;
	}

	// Allocate the connection handle
	if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_DBC, m_envHandle, &m_connHandle)))
	{
		ReportSQLError(SQL_HANDLE_ENV, m_envHandle, _T("SQLAllocHandle"), _T("Unable to allocate connection handle."));
		goto error_handler;
	}

	if (m_szUser.length())
	{
		szConn += _T("UID=") + m_szUser + _T(";");
		if (m_szPass.length())
			szConn += _T("PWD=") + m_szPass + _T(";");
	}

	if (m_bMarsEnabled)
	{
		if (!SQL_SUCCEEDED(SQLSetConnectAttr(m_connHandle, SQL_COPT_SS_MARS_ENABLED, SQL_MARS_ENABLED_YES, SQL_IS_UINTEGER)))
		{
			printf("** WARNING **\n\n");
			printf("Attempted to used MARS (Multiple Active Result Sets), but this\n");
			printf("feature is not supported by your ODBC driver or SQL Server version.\n\n");
			printf("To benefit from MARS, you need to be using at least SQL Server 2005, and at\n");
			printf("least the 'SQL Native Client' ODBC driver (as opposed to the vastly outdated\n'SQL Server' driver).\n\n");
			printf("Continuing to connect without MARS.\n\n");
			m_bMarsEnabled = false;
		}
	}

	if (!SQL_SUCCEEDED(SQLDriverConnect(m_connHandle, SQL_NULL_HANDLE, (SQLTCHAR *)szConn.c_str(), SQL_NTS, 0, 0, 0, 0)))
	{
		ReportSQLError(SQL_HANDLE_DBC, m_connHandle, _T("SQLDriverConnect"), _T("Unable to establish connection."));
		goto error_handler;
	}

	for (auto itr = m_commandSet.begin(); itr != m_commandSet.end(); itr++)
		(*itr)->SetConnectionHandle(m_connHandle);

	return true;

error_handler:
	ResetHandles();
	return false;
}

OdbcCommand *OdbcConnection::CreateCommand()
{
	if (!isConnected()
		&& !Connect())
		return nullptr;

	return new OdbcCommand(this);
}

void OdbcConnection::AddCommand(OdbcCommand *dbCommand)
{
	Guard lock(m_lock);
	m_commandSet.insert(dbCommand);
}

void OdbcConnection::RemoveCommand(OdbcCommand *dbCommand)
{
	Guard lock(m_lock);
	m_commandSet.erase(dbCommand);
}

// Used to internally reset handles. Should ONLY be used in special cases, otherwise we'll break the state of the connection.
void OdbcConnection::ResetHandles()
{
	// Free the connection handle if it's allocated
	if (m_connHandle != nullptr)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_connHandle);
		m_connHandle = nullptr;
	}

	// Free the environment handle if it's allocated
	if (m_envHandle != nullptr)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, m_envHandle);
		m_envHandle = nullptr;
	}
}

tstring OdbcConnection::ReportSQLError(SQLSMALLINT handleType, SQLHANDLE handle,
									   const TCHAR *szSource, const TCHAR *szError, ...)
{
	Guard lock(m_lock);
	TCHAR szErrorBuffer[256];
	OdbcError *error = new OdbcError();

	va_list args;
	va_start(args, szError);
	_vsntprintf(szErrorBuffer, sizeof(szErrorBuffer), szError, args);
	va_end(args);

	error->Source = szSource;
	error->ErrorMessage = szErrorBuffer;

	m_odbcErrors.push_back(error);

	if (handle != nullptr)
	{
		error->ExtendedErrorMessage = GetSQLError(handleType, handle);
		if (!error->ExtendedErrorMessage.empty())
			return error->ExtendedErrorMessage;
	}

	return szErrorBuffer;
}

tstring OdbcConnection::GetSQLError(SQLSMALLINT handleType, SQLHANDLE handle)
{
	tstring result;
	SQLTCHAR SqlState[256], SqlMessage[256];
	SQLINTEGER NativeError;
	SQLSMALLINT TextLength;

	if (!SQL_SUCCEEDED(SQLGetDiagRec(handleType, handle, 1, (SQLTCHAR *)&SqlState, &NativeError, (SQLTCHAR *)&SqlMessage, sizeof(SqlMessage), &TextLength)))
		return result;

	result = (TCHAR *)SqlMessage;
	return result;
}

OdbcError *OdbcConnection::GetError()
{
	Guard lock(m_lock);
	if (m_odbcErrors.empty())
		return nullptr;

	OdbcError *pError = m_odbcErrors.back();
	m_odbcErrors.pop_back();
	return pError;
}

void OdbcConnection::ResetErrors()
{
	if (!isError())
		return;

	Guard lock(m_lock);
	OdbcError *pError;
	while ((pError = GetError()) != nullptr)
		delete pError;
}

void OdbcConnection::Disconnect()
{
	// Make sure our handles are open. If not, there's nothing to do.
	if (!isConnected())
		return;

	Guard lock(m_lock);
	// Kill off open statements
	if (m_commandSet.size())
	{
		for (auto itr = m_commandSet.begin(); itr != m_commandSet.end(); itr++)
		{
			// Detach from the connection first so we don't try to remove it from the set (while we're using it!)
			(*itr)->Detach();

			// Now free it.
			delete (*itr);
		}

		m_commandSet.clear();
	}

	// Close the connection to the server & reset our handles
	Close();
}

void OdbcConnection::Close()
{
	// Disconnect from server.
	SQLDisconnect(m_connHandle);

	// Reset handles
	ResetHandles();
}

OdbcConnection::~OdbcConnection()
{
	Disconnect();
	ResetErrors();
	delete m_lock;
}
