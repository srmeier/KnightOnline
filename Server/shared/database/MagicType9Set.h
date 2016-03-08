#pragma once

class CMagicType9Set : public OdbcRecordset
{
public:
	CMagicType9Set(OdbcConnection * dbConnection, Magictype9Array * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAGIC_TYPE9"); }
	virtual tstring GetColumns() { return _T("iNum, ValidGroup, NationChange, MonsterNum, TargetChange, StateChange, Radius, Hitrate, Duration, AddDamage, Vision, NeedItem"); }

	virtual bool Fetch()
	{
		_MAGIC_TYPE9 *pData = new _MAGIC_TYPE9;

		_dbCommand->FetchUInt32(1, pData->iNum);
		_dbCommand->FetchByte(2, pData->bValidGroup);
		_dbCommand->FetchByte(3, pData->bNationChange);
		_dbCommand->FetchUInt16(4, pData->sMonsterNum);
		_dbCommand->FetchByte(5, pData->bTargetChange);
		_dbCommand->FetchByte(6, pData->bStateChange);
		_dbCommand->FetchUInt16(7, pData->sRadius);
		_dbCommand->FetchUInt16(8, pData->sHitRate);
		_dbCommand->FetchUInt16(9, pData->sDuration);
		_dbCommand->FetchUInt16(10, pData->sDamage);
		_dbCommand->FetchUInt16(11, pData->sVision);
		_dbCommand->FetchUInt32(12, pData->nNeedItem);

		if (!m_pMap->PutData(pData->iNum, pData))
			delete pData;

		return true;
	}

	Magictype9Array *m_pMap;
};