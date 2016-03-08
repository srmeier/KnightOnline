#pragma once

class CMonsterRespawnListInformationSet : public OdbcRecordset
{
public:
	CMonsterRespawnListInformationSet(OdbcConnection * dbConnection, MonsterRespawnListInformationArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MONSTER_RESPAWN_LIST_INFORMATION"); }
	virtual tstring GetColumns() { return _T("sIndex, sSid, ZoneID, X, Y, Z, sCount, bRadius"); }

	virtual bool Fetch()
	{
		_MONSTER_RESPAWN_LIST_INFORMATION * pData = new _MONSTER_RESPAWN_LIST_INFORMATION;

		int i = 1;
		_dbCommand->FetchUInt16(i++, pData->sIndex);
		_dbCommand->FetchUInt16(i++, pData->sSid);
		_dbCommand->FetchByte(i++, pData->ZoneID);
		_dbCommand->FetchUInt16(i++, pData->X);
		_dbCommand->FetchUInt16(i++, pData->Y);
		_dbCommand->FetchUInt16(i++, pData->Z);
		_dbCommand->FetchUInt16(i++, pData->sCount);
		_dbCommand->FetchByte(i++, pData->bRadius);

		if (!m_pMap->PutData(pData->sIndex, pData))
			delete pData;

		return true;
	}

	MonsterRespawnListInformationArray *m_pMap;
};