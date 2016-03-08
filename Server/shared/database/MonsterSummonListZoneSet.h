 #pragma once

class CMonsterSummonListZoneSet : public OdbcRecordset
{
public:
	CMonsterSummonListZoneSet(OdbcConnection * dbConnection, MonsterSummonListZoneArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MONSTER_SUMMON_LIST_ZONE"); }
	virtual tstring GetColumns() { return _T("nIndex, ZoneID, sSid, byFamily"); }

	virtual bool Fetch()
	{
		_MONSTER_SUMMON_LIST_ZONE * pData = new _MONSTER_SUMMON_LIST_ZONE;

		_dbCommand->FetchUInt32(1, pData->nIndex);
		_dbCommand->FetchUInt16(2, pData->ZoneID);
		_dbCommand->FetchUInt16(3, pData->sSid);
		_dbCommand->FetchByte(4, pData->byFamily);

		pData->nIndex = m_pMap->GetSize();

		if (!m_pMap->PutData(pData->nIndex, pData))
			delete pData;

		return true;
	}

	MonsterSummonListZoneArray *m_pMap;
};