#pragma once

class CItemOpSet : public OdbcRecordset
{
public:
	CItemOpSet(OdbcConnection * dbConnection, ItemOpArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("ITEM_OP"); }
	virtual tstring GetColumns() { return _T("nItemID, bTriggerType, nSkillID, bTriggerRate"); }

	virtual bool Fetch()
	{
		_ITEM_OP * pData = new _ITEM_OP;

		_dbCommand->FetchUInt32(1, pData->nItemID);
		_dbCommand->FetchByte(2, pData->bTriggerType);
		_dbCommand->FetchUInt32(3, pData->nSkillID);
		_dbCommand->FetchByte(4, pData->bTriggerRate);

		if (!m_pMap->PutData(pData->nItemID, pData))
			delete pData;

		return true;
	}

	ItemOpArray *m_pMap;
};