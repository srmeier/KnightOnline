#pragma once
	
class CMagicType6Set : public OdbcRecordset
{
public:
	CMagicType6Set(OdbcConnection * dbConnection, Magictype6Array * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAGIC_TYPE6"); }
	virtual tstring GetColumns() {
		return _T(
			"iNum, "\
			"Size, "\
			"TransformID, "\
			"Duration, "\
			"MaxHp, "\
			"MaxMp, "\
			"Speed, "\
			"AttackSpeed, "\
			"TotalHit, "\
			"TotalAc, "\
			"TotalHitRate, "\
			"TotalEvasionRate, "\
			"TotalFireR, "\
			"TotalColdR, "\
			"TotalLightningR, "\
			"TotalMagicR, "\
			"TotalDiseaseR, "\
			"TotalPoisonR, "\
			"Class, "\
			"UserSkillUse, "\
			"NeedItem, "\
			"SkillSuccessRate, "\
			"MonsterFriendly " //,
			//"Nation"
		);
	}

	virtual bool Fetch()
	{
		_MAGIC_TYPE6 *pData = new _MAGIC_TYPE6;

		_dbCommand->FetchUInt32(1, pData->iNum);
		_dbCommand->FetchUInt16(2, pData->sSize);
		_dbCommand->FetchUInt16(3, pData->sTransformID);
		_dbCommand->FetchUInt16(4, pData->sDuration);
		_dbCommand->FetchUInt16(5, pData->sMaxHp);
		_dbCommand->FetchUInt16(6, pData->sMaxMp);
		_dbCommand->FetchByte(7, pData->bSpeed);
		_dbCommand->FetchUInt16(8, pData->sAttackSpeed);
		_dbCommand->FetchUInt16(9, pData->sTotalHit);
		_dbCommand->FetchUInt16(10, pData->sTotalAc);
		_dbCommand->FetchUInt16(11, pData->sTotalHitRate);
		_dbCommand->FetchUInt16(12, pData->sTotalEvasionRate);
		_dbCommand->FetchUInt16(13, pData->sTotalFireR);
		_dbCommand->FetchUInt16(14, pData->sTotalColdR);
		_dbCommand->FetchUInt16(15, pData->sTotalLightningR);
		_dbCommand->FetchUInt16(16, pData->sTotalMagicR);
		_dbCommand->FetchUInt16(17, pData->sTotalDiseaseR);
		_dbCommand->FetchUInt16(18, pData->sTotalPoisonR);
		_dbCommand->FetchUInt16(19, pData->sClass);
		_dbCommand->FetchByte(20, pData->bUserSkillUse);
		_dbCommand->FetchByte(21, pData->bNeedItem);
		_dbCommand->FetchByte(22, pData->bSkillSuccessRate);
		_dbCommand->FetchByte(23, pData->bMonsterFriendly);
		//_dbCommand->FetchByte(24, pData->bNation);

		if (!m_pMap->PutData(pData->iNum, pData))
			delete pData;

		return true;
	}

	Magictype6Array *m_pMap;
};