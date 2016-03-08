 #pragma once

class CEventTriggerSet : public OdbcRecordset
{
public:
	CEventTriggerSet(OdbcConnection * dbConnection, EventTriggerArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("EVENT_TRIGGER"); }
	virtual tstring GetColumns() { return _T("nIndex, bNpcType, sNpcID, nTriggerNum"); }

	virtual bool Fetch()
	{
		_EVENT_TRIGGER * pData = new _EVENT_TRIGGER;

		_dbCommand->FetchUInt32(1, pData->nIndex);
		_dbCommand->FetchUInt16(2, pData->bNpcType);
		_dbCommand->FetchUInt32(3, pData->sNpcID);
		_dbCommand->FetchUInt32(4, pData->nTriggerNum);

		if (!m_pMap->PutData(pData->nIndex, pData))
			delete pData;

		return true;
	}

	EventTriggerArray *m_pMap;
};