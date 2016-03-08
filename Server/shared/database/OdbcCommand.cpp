#include "stdafx.h"
#include "OdbcConnection.h"

OdbcCommand::OdbcCommand(HDBC conn)
	: m_odbcConnection(nullptr), m_connHandle(conn), m_hStmt(nullptr), m_resultCode(-1)
{
}

OdbcCommand::OdbcCommand(OdbcConnection * conn)
	: m_odbcConnection(conn), m_hStmt(nullptr)
{
	m_connHandle = conn->GetConnectionHandle();
	m_odbcConnection->AddCommand(this);
}

bool OdbcCommand::BindParameters()
{
	for (auto itr = m_params.begin(); itr != m_params.end(); itr++)
	{
		auto param = itr->second;

		if (!SQL_SUCCEEDED(SQLBindParameter(m_hStmt, itr->first + 1, param->GetParameterType(), 
			param->GetCDataType(), param->GetDataType(), param->GetDataTypeSize(), 0, 
			param->GetAddress(), param->GetDataTypeSize(), param->GetCBValue())))
		{
			if (m_odbcConnection != nullptr)
				m_szError = m_odbcConnection->ReportSQLError(SQL_HANDLE_STMT, m_hStmt, _T("SQLBindParameter"), _T("Failed to bind parameter."));
			else
				m_szError = OdbcConnection::GetSQLError(SQL_HANDLE_STMT, m_hStmt);

			Close();
			return false;
		}
	}
	return true;
}

bool OdbcCommand::Open(bool bRetry /*= false*/)
{
	if (isOpen())
		Close();

	if (!SQL_SUCCEEDED(SQLAllocHandle(SQL_HANDLE_STMT, m_connHandle, &m_hStmt)))
	{
		if (m_odbcConnection != nullptr)
			m_szError = m_odbcConnection->ReportSQLError(SQL_HANDLE_DBC, m_connHandle, _T("SQLAllocHandle"), _T("Failed to allocate statement handle."));
		else
			m_szError = OdbcConnection::GetSQLError(SQL_HANDLE_DBC, m_connHandle);

		// Attempt full SQL reconnection once.
		if (m_odbcConnection == nullptr || bRetry)
			return false;

		// Perform soft disconnect, preserving existing commands
		m_odbcConnection->Close();

		// Reconnect
		m_odbcConnection->Connect();

		// Now try running the statement once more time.
		return Open(true); 
	}

	SQLExecDirect(m_hStmt, (SQLTCHAR *)_T("SET NOCOUNT ON"), SQL_NTS);
	return true;
}

bool OdbcCommand::Execute(const tstring & szSQL)
{
	if (!Open())
		return false;

#ifdef USE_SQL_TRACE
	TRACE((szSQL + _T("\n")).c_str());
#endif

	if (!BindParameters())
		return false;

	SQLRETURN result = SQLExecDirect(m_hStmt, (SQLTCHAR *)szSQL.c_str(), szSQL.length());
	if (!(result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO || result == SQL_NO_DATA))
	{
		if (m_odbcConnection != nullptr)
			m_szError = m_odbcConnection->ReportSQLError(SQL_HANDLE_STMT, m_hStmt, (TCHAR *)szSQL.c_str(), _T("Failed to execute statement."));
		else
			m_szError = OdbcConnection::GetSQLError(SQL_HANDLE_STMT, m_hStmt);

		Close();
		return false;
	}

	if (!MoveNext())
		MoveNextSet();

	return true;
}

bool OdbcCommand::Prepare(const tstring & szSQL)
{
	if (!Open())
		return false;

#ifdef USE_SQL_TRACE
	TRACE((szSQL + _T("\n")).c_str());
#endif

	if (!SQL_SUCCEEDED(SQLPrepare(m_hStmt, (SQLTCHAR *)szSQL.c_str(), szSQL.length())))
	{
		if (m_odbcConnection != nullptr)
			m_szError = m_odbcConnection->ReportSQLError(SQL_HANDLE_STMT, m_hStmt, _T("SQLPrepare"), _T("Failed to prepare statement."));
		else
			m_szError = OdbcConnection::GetSQLError(SQL_HANDLE_STMT, m_hStmt);

		Close();
		return false;
	}

	if (!BindParameters())
		return false;

	SQLRETURN result = SQLExecute(m_hStmt);
	if (!(result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO || result == SQL_NO_DATA))
	{
		if (m_odbcConnection != nullptr)
			m_szError = m_odbcConnection->ReportSQLError(SQL_HANDLE_STMT, m_hStmt, (TCHAR *)szSQL.c_str(), _T("Failed to execute prepared statement."));
		else
			m_szError = OdbcConnection::GetSQLError(SQL_HANDLE_STMT, m_hStmt);

		Close();
		return false;
	}

	// If there's no rows to move through, skip to the next result set.
	if (!MoveNext())
		MoveNextSet();

	ClearParameters();
	return true;
}

bool OdbcCommand::MoveNext()
{
	if (!isOpen())
		return false;

	return SQL_SUCCEEDED(m_resultCode = SQLFetch(m_hStmt));
}

bool OdbcCommand::MoveNextSet()
{
	if (!isOpen())
		return false;

	return SQL_SUCCEEDED(m_resultCode = SQLMoreResults(m_hStmt));
}

#define ADD_ODBC_PARAMETER(name, type, sqlType) void OdbcCommand::AddParameter(SQLSMALLINT paramType, type *value, SQLLEN maxLength) { m_params.insert(std::make_pair(m_params.size(), new OdbcParameter(paramType, sqlType, (SQLPOINTER)value, maxLength))); } \
	type OdbcCommand::Fetch ## name(int pos) { type value; SQLINTEGER cb = SQL_NTS; SQLGetData(m_hStmt, pos, sqlType, &value, 0, &cb); return value; } \
	void OdbcCommand::Fetch ## name(int pos, type & value) { SQLINTEGER cb = SQL_NTS; SQLGetData(m_hStmt, pos, sqlType, &value, 0, &cb); }
ADD_ODBC_PARAMETER(Byte, uint8, SQL_C_UTINYINT)
	ADD_ODBC_PARAMETER(SByte, int8, SQL_C_STINYINT)
	ADD_ODBC_PARAMETER(UInt16, uint16, SQL_C_USHORT)
	ADD_ODBC_PARAMETER(Int16, int16, SQL_C_SSHORT)
	ADD_ODBC_PARAMETER(UInt32, uint32, SQL_C_ULONG)
	ADD_ODBC_PARAMETER(Int32, int32, SQL_C_SLONG)
	ADD_ODBC_PARAMETER(Single, float, SQL_C_FLOAT)
	ADD_ODBC_PARAMETER(Double, double, SQL_C_DOUBLE)
	ADD_ODBC_PARAMETER(UInt64, uint64, SQL_C_UBIGINT)
	ADD_ODBC_PARAMETER(Int64, int64, SQL_C_SBIGINT)
#undef ADD_ODBC_PARAMETER

	void OdbcCommand::AddParameter(SQLSMALLINT paramType, const char *value, SQLLEN maxLength = 1, SQLSMALLINT sqlDataType /*= SQL_CHAR*/)
{
	m_params.insert(std::make_pair(m_params.size(), new OdbcParameter(paramType, sqlDataType, (SQLPOINTER)value, maxLength))); 
}

bool OdbcCommand::FetchString(int pos, char *charArray, SQLLEN maxLength, SQLLEN *bufferSize)
{
	memset(charArray, 0x00, maxLength);
	return SQL_SUCCEEDED(SQLGetData(m_hStmt, pos, SQL_CHAR, charArray, maxLength, bufferSize));
}

bool OdbcCommand::FetchBinary(int pos, char *charArray, SQLLEN maxLength, SQLLEN *bufferSize)
{
	return SQL_SUCCEEDED(SQLGetData(m_hStmt, pos, SQL_BINARY, charArray, maxLength, bufferSize));
}

bool OdbcCommand::FetchString(int pos, std::string & value)
{
	SQLINTEGER bufferSize = 0;
	char buffer[256] = "";

	// Attempt to fetch "small" string of 256 bytes at most (should fit everything we'll need)
	if (!FetchString(pos, buffer, sizeof(buffer), &bufferSize))
	{
		// Error fetching string, nothing we can do.
		if (bufferSize <= 0)
			return false;

		// Allocate a buffer large enough for the string's actual length
		std::unique_ptr<char> varBuffer(new char[bufferSize + 1]);

		// If the string still couldn't be fetched, nothing we can do.
		if (!FetchString(pos, varBuffer.get(), bufferSize + 1, &bufferSize))
			return false;

		value = varBuffer.get();
	}
	// String could be fetched, copy it over to the output var.
	else
	{
		value = buffer;
	}

	// This line's necessary for SQL_CHAR type columns to trim the padding.
	// NOTE: If we need the padding, we should be using the OTHER FetchString() method for it (to fetch it straight into a char array)
	rtrim(value);
	return true;
}

void OdbcCommand::ClearParameters()
{
	if (m_params.empty())
		return;

	for (auto itr = m_params.begin(); itr != m_params.end(); itr++)
		delete itr->second;

	m_params.clear();
}

void OdbcCommand::Close()
{
	if (m_hStmt != nullptr)
	{
		SQLCloseCursor(m_hStmt); // free results, if any
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = nullptr;
	}
}

void OdbcCommand::Detach()
{
	m_odbcConnection = nullptr;
}

OdbcCommand::~OdbcCommand()
{
	Close();
	ClearParameters();

	if (m_odbcConnection != nullptr)
		m_odbcConnection->RemoveCommand(this);
}