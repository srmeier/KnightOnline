#pragma once

#pragma comment(lib, "odbc32.lib")

#include <sqlext.h>
#include <set>

// MARS settings from sqlncli.h
#ifndef SQL_COPT_SS_MARS_ENABLED
#        define SQL_COPT_SS_MARS_ENABLED 1224
#endif

#ifndef SQL_MARS_ENABLED_YES
#        define SQL_MARS_ENABLED_YES (SQLPOINTER)1
#endif

#include "../tstring.h"

struct OdbcError
{
	tstring	Source;
	tstring	ErrorMessage;
	tstring	ExtendedErrorMessage;
};

#include "OdbcCommand.h"

class std::recursive_mutex;
class OdbcConnection
{
	friend class OdbcCommand;

public:
	OdbcConnection();

	bool isConnected();
	bool isError();

	INLINE HDBC GetConnectionHandle() { return m_connHandle; }
	INLINE bool isMarsEnabled() { return m_bMarsEnabled; }

	bool Connect(tstring & szDSN, tstring & szUser, tstring & szPass, bool bMarsEnabled = false);
	bool Connect();

	OdbcCommand *CreateCommand();
	static tstring GetSQLError(SQLSMALLINT handleType, SQLHANDLE handle);

	OdbcError *GetError();
	void ResetErrors();

	void Disconnect();
	~OdbcConnection();

private:
	void AddCommand(OdbcCommand *dbCommand);
	void RemoveCommand(OdbcCommand *dbCommand);
	tstring ReportSQLError(SQLSMALLINT handleType, SQLHANDLE handle, const TCHAR *szSource, const TCHAR *szError, ...);

	void Close();
	void ResetHandles();

private:
	tstring m_szDSN, m_szUser, m_szPass;

	HENV m_envHandle;
	HDBC m_connHandle;

	std::recursive_mutex * m_lock;

	std::vector<OdbcError   *> m_odbcErrors;
	std::set   <OdbcCommand *> m_commandSet;

	 bool m_bMarsEnabled;
};