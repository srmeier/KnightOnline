#pragma once

class OdbcParameter
{
public:
	OdbcParameter(SQLSMALLINT parameterType, SQLSMALLINT dataType, SQLPOINTER parameterAddress, SQLLEN maxLength = 1);

	INLINE SQLSMALLINT GetParameterType() { return m_parameterType; };
	INLINE SQLSMALLINT GetDataType() { return m_dataType; };
	INLINE SQLSMALLINT GetCDataType() { return m_cDataType; };
	INLINE SQLPOINTER GetAddress() { return m_parameterAddress; };
	INLINE SQLLEN GetDataTypeSize() { return m_dataTypeLength; };
	INLINE SQLLEN *GetCBValue() { return &m_pCBValue; };

private:
	SQLSMALLINT m_parameterType, m_dataType, m_cDataType;
	SQLPOINTER m_parameterAddress;
	SQLLEN m_dataTypeLength, m_pCBValue;
};