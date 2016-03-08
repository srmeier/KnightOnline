#pragma once

#include "OdbcParameter.h"
#include <map>

class OdbcConnection;
class OdbcCommand
{
public:
	typedef std::map<int, OdbcParameter *> OdbcParameterCollection;

	OdbcCommand(HDBC conn);
	OdbcCommand(OdbcConnection * conn);

	INLINE bool isOpen() { return m_hStmt != nullptr; };
	INLINE TCHAR * GetError() { return (TCHAR *)m_szError.c_str();  };
	INLINE bool hasData() { return m_resultCode != SQL_NO_DATA && SQL_SUCCEEDED(m_resultCode); };
	INLINE void SetConnectionHandle(HDBC handle) { m_connHandle = handle; };

private:
	bool BindParameters();

public:
	bool Execute(const tstring & szSQL);
	bool Prepare(const tstring & szSQL);

	bool MoveNext();
	bool MoveNextSet();


#define ADD_ODBC_PARAMETER(name, type, sqlType) \
	void AddParameter(SQLSMALLINT paramType, type *value, SQLLEN maxLength = sizeof(type)); \
	type Fetch ## name(int pos); \
	void Fetch ## name(int pos, type & value);

	ADD_ODBC_PARAMETER(Byte, uint8, SQL_C_TINYINT)
		ADD_ODBC_PARAMETER(SByte, int8, SQL_C_STINYINT)
		ADD_ODBC_PARAMETER(UInt16, uint16, SQL_C_USHORT)
		ADD_ODBC_PARAMETER(Int16, int16, SQL_C_SSHORT)
		ADD_ODBC_PARAMETER(UInt32, uint32, SQL_C_ULONG)
		ADD_ODBC_PARAMETER(Int32, int32, SQL_C_LONG)
		ADD_ODBC_PARAMETER(Single, float, SQL_C_FLOAT)
		ADD_ODBC_PARAMETER(Double, double, SQL_C_DOUBLE)
		ADD_ODBC_PARAMETER(UInt64, uint64, SQL_C_UBIGINT)
		ADD_ODBC_PARAMETER(Int64, int64, SQL_C_SBIGINT)

		void AddParameter(SQLSMALLINT paramType, const char *value, SQLLEN maxLength, SQLSMALLINT sqlDatatype = SQL_CHAR);

	INLINE bool FetchString(int pos, char *outBuffer, SQLLEN maxLength)
	{
		SQLINTEGER bufferSize = 0;
		return FetchString(pos, outBuffer, maxLength, &bufferSize);
	};

	bool FetchString(int pos, char *charArray, SQLLEN maxLength, SQLLEN *bufferSize);
	bool FetchString(int pos, std::string & value);

	INLINE bool FetchBinary(int pos, char *outBuffer, SQLLEN maxLength)
	{
		SQLINTEGER bufferSize = 0;
		return FetchBinary(pos, outBuffer, maxLength, &bufferSize);
	};
	bool FetchBinary(int pos, char *charArray, SQLLEN maxLength, SQLLEN *bufferSize);


#undef ADD_ODBC_PARAMETER

	void Detach();
	~OdbcCommand();

private:
	bool Open(bool bRetry = false);
	void ClearParameters();
public:	void Close();
private:
	HDBC m_connHandle;
	OdbcConnection *m_odbcConnection;
	HSTMT m_hStmt;

	OdbcParameterCollection m_params;

	tstring m_szError;
	SQLRETURN m_resultCode;
};