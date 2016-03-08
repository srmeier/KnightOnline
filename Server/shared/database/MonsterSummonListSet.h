#pragma once

class CMonsterSummonListSet : public OdbcRecordset
{
public:
	CMonsterSummonListSet(OdbcConnection * dbConnection, MonsterSummonListArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MONSTER_SUMMON_LIST"); }
	virtual tstring GetColumns() { return _T("sSid, sLevel, sProbability, bType"); }

	virtual bool Fetch()
	{
		_MONSTER_SUMMON_LIST pData;

		_dbCommand->FetchUInt16(1, pData.sSid);
		_dbCommand->FetchUInt16(2, pData.sLevel);
		_dbCommand->FetchUInt16(3, pData.sProbability);
		_dbCommand->FetchByte(4, pData.bType);

		auto itr = m_pMap->GetData(pData.bType);
		if (itr == nullptr)
		{
			itr = new MonsterSummonList();
			m_pMap->PutData(pData.bType, itr);
		}

		itr->push_back(pData);
		return true;
	}

	MonsterSummonListArray *m_pMap;
};