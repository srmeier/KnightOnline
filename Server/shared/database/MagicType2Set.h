#pragma once

class CMagicType2Set : public OdbcRecordset
{
public:
	CMagicType2Set(OdbcConnection * dbConnection, Magictype2Array * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAGIC_TYPE2"); }
	virtual tstring GetColumns() { return _T("iNum, HitType, HitRate, AddDamage, AddRange, NeedArrow"); }

	virtual bool Fetch()
	{
		_MAGIC_TYPE2 *pData = new _MAGIC_TYPE2;

		_dbCommand->FetchUInt32(1, pData->iNum);
		_dbCommand->FetchByte(2, pData->bHitType);
		_dbCommand->FetchUInt16(3, pData->sHitRate);
		_dbCommand->FetchUInt16(4, pData->sAddDamage);
		_dbCommand->FetchUInt16(5, pData->sAddRange);
		_dbCommand->FetchByte(6, pData->bNeedArrow);

		if (!m_pMap->PutData(pData->iNum, pData))
			delete pData;

		return true;
	}

	Magictype2Array *m_pMap;
};