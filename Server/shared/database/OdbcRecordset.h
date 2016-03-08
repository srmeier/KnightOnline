#pragma once

#include "../tstring.h"
class OdbcConnection;
class OdbcCommand;

class OdbcRecordset
{
public:
	OdbcRecordset(OdbcConnection * dbConnection);

	virtual tstring GetTableName() = 0;
	virtual tstring GetColumns() = 0;
	virtual tstring GetWhereClause() { return _T(""); }

	TCHAR * Read(bool bAllowEmptyTable = false);
	virtual bool Fetch() = 0;

	virtual ~OdbcRecordset();

protected:
	OdbcConnection * _dbConnection;
	OdbcCommand * _dbCommand;
};

#define _LOAD_TABLE(Set, DB, Array, AllowEmptyTable, IsSlient) \
	Set _ ## Set(DB, Array); \
	if (!IsSlient) \
	_tprintf(_T("Loaded table [%s]\n"), _ ## Set .GetTableName().c_str()); \
	TCHAR * _szError ## Set = _ ## Set.Read(AllowEmptyTable);

#define LOAD_TABLE(Set, DB, Array, AllowEmptyTable, IsSlient) \
	_LOAD_TABLE(Set, DB, Array, AllowEmptyTable, IsSlient); \
	_HANDLE_DB_ERROR(_szError ## Set)

#define LOAD_TABLE_ERROR_ONLY(Set, DB, Array, AllowEmptyTable, IsSlient) \
	_LOAD_TABLE(Set, DB, Array, AllowEmptyTable, IsSlient); \
	_HANDLE_DB_ERROR_ONLY(_szError ## Set)

#define _DISPLAY_DB_ERROR(err) \
	_tprintf(_T("ERROR: Could not load table, the error received was:\n%s\n"), err); \

#define _HANDLE_DB_ERROR(err) \
	if (err != nullptr) \
	_DISPLAY_DB_ERROR(err); \
	return (err == nullptr)

#define _HANDLE_DB_ERROR_ONLY(err) \
	if (err != nullptr) { \
	_DISPLAY_DB_ERROR(err); \
	return false; \
	}