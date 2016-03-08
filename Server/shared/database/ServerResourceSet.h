#pragma once

class CServerResourceSet : public OdbcRecordset
{
public:
	CServerResourceSet(OdbcConnection * dbConnection, ServerResourceArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("SERVER_RESOURCE"); }
	virtual tstring GetColumns() { return _T("nResourceID, strResource"); }

	virtual bool Fetch()
	{
		_SERVER_RESOURCE *pData = new _SERVER_RESOURCE;

		_dbCommand->FetchUInt32(1, pData->nResourceID);
		_dbCommand->FetchString(2, pData->strResource);

		if (!m_pMap->PutData(pData->nResourceID, pData))
			delete pData;

		return true;
	}

	ServerResourceArray *m_pMap;
};