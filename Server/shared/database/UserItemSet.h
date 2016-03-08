#pragma once

class CUserItemSet : public OdbcRecordset
{
public:
	CUserItemSet(OdbcConnection * dbConnection, UserItemArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("USER_ITEMS"); }
	virtual tstring GetColumns() { return _T("nItemID, nItemSerial"); }

	virtual bool Fetch()
	{
		uint32 nItemID = _dbCommand->FetchUInt32(1);
		uint64 nItemSerial = _dbCommand->FetchUInt64(2);

		_USER_ITEM * pData = m_pMap->GetData(nItemID);

		if (pData == nullptr)
		{
			pData = new _USER_ITEM;
			pData->nItemID = nItemID;
			pData->nItemSerial.push_back(nItemSerial);

			if (!m_pMap->PutData(pData->nItemID,pData))
				delete pData;
		}
		else
			pData->nItemSerial.push_back(nItemSerial);

		return true;
	}

	UserItemArray *m_pMap;
};