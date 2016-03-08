#pragma once

class CCoefficientSet : public OdbcRecordset
{
public:
	CCoefficientSet(OdbcConnection * dbConnection, CoefficientArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("COEFFICIENT"); }
	virtual tstring GetColumns() { return _T("sClass, ShortSword, Sword, Axe, Club, Spear, Pole, Staff, Bow, Hp, Mp, Sp, Ac, Hitrate, Evasionrate"); }

	virtual bool Fetch()
	{
		_CLASS_COEFFICIENT *pData = new _CLASS_COEFFICIENT;

		_dbCommand->FetchUInt16(1, pData->sClassNum);
		_dbCommand->FetchSingle(2, pData->ShortSword);
		_dbCommand->FetchSingle(3, pData->Sword);
		_dbCommand->FetchSingle(4, pData->Axe);
		_dbCommand->FetchSingle(5, pData->Club);
		_dbCommand->FetchSingle(6, pData->Spear);
		_dbCommand->FetchSingle(7, pData->Pole);
		_dbCommand->FetchSingle(8, pData->Staff);
		_dbCommand->FetchSingle(9, pData->Bow);
		_dbCommand->FetchSingle(10, pData->HP);
		_dbCommand->FetchSingle(11, pData->MP);
		_dbCommand->FetchSingle(12, pData->SP);
		_dbCommand->FetchSingle(13, pData->AC);
		_dbCommand->FetchSingle(14, pData->Hitrate);
		_dbCommand->FetchSingle(15, pData->Evasionrate);

		if (!m_pMap->PutData(pData->sClassNum, pData))
			delete pData;

		return true;
	}

	CoefficientArray *m_pMap;
};