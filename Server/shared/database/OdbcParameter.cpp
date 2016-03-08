#include "stdafx.h"
#include "OdbcConnection.h"

OdbcParameter::OdbcParameter(SQLSMALLINT parameterType, SQLSMALLINT dataType, SQLPOINTER parameterAddress, SQLLEN maxLength /*= 1*/)
	: m_parameterType(parameterType), m_cDataType(dataType), m_parameterAddress(parameterAddress), 
	m_dataTypeLength(0), m_pCBValue(SQL_NTS)
{
	switch (m_cDataType)
	{
	case SQL_CHAR:
	case SQL_VARCHAR:
	case SQL_BINARY:
		m_dataTypeLength = maxLength;
		m_dataType = m_cDataType;
		if (m_cDataType == SQL_BINARY || m_cDataType == SQL_CHAR)
			m_pCBValue = m_dataTypeLength;
		break;

	case SQL_C_STINYINT:
	case SQL_C_UTINYINT:
		m_dataType = SQL_TINYINT;
		break;

	case SQL_C_SSHORT:
	case SQL_C_USHORT:
		m_dataType = SQL_SMALLINT;
		break;

	case SQL_C_SLONG:
	case SQL_C_ULONG:
		m_dataType = SQL_INTEGER;
		break;

	case SQL_FLOAT:
		m_cDataType = m_dataType;
		break;

	case SQL_DOUBLE:
		m_cDataType = m_dataType;
		break;

	default: // unknown, default to integer
		m_dataType = SQL_INTEGER;
		m_cDataType = m_dataType + SQL_SIGNED_OFFSET;
	}
}
