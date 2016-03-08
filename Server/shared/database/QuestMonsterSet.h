#pragma once

class CQuestMonsterSet : public OdbcRecordset
{
public:
	CQuestMonsterSet(OdbcConnection * dbConnection, QuestMonsterArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("QUEST_MONSTER"); }
	virtual tstring GetColumns() { return _T("sQuestNum, sNum1a, sNum1b, sNum1c, sNum1d, sCount1, sNum2a, sNum2b, sNum2c, sNum2d, sCount2, sNum3a, sNum3b, sNum3c, sNum3d, sCount3, sNum4a, sNum4b, sNum4c, sNum4d, sCount4"); }

	virtual bool Fetch()
	{
		_QUEST_MONSTER *pData = new _QUEST_MONSTER();
		uint32 col = 1;

		_dbCommand->FetchUInt16(col++, pData->sQuestNum);
		for (uint32 group = 0; group < QUEST_MOB_GROUPS; group++)
		{
			for (uint32 i = 0; i < QUEST_MOBS_PER_GROUP; i++)
				_dbCommand->FetchUInt16(col++, pData->sNum[group][i]);
			_dbCommand->FetchUInt16(col++, pData->sCount[group]);
		}

		if (!m_pMap->PutData(pData->sQuestNum, pData))
			delete pData;

		return true;
	}

	QuestMonsterArray *m_pMap;
};