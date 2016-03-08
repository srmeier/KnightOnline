#pragma once

class CMakeWeaponTableSet : public OdbcRecordset
{
public:
	CMakeWeaponTableSet(OdbcConnection * dbConnection, MakeWeaponItemTableArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAKE_WEAPON"); }
	virtual tstring GetColumns() { return _T("byLevel, sClass_1, sClass_2, sClass_3, sClass_4, sClass_5, sClass_6, sClass_7, sClass_8, sClass_9, sClass_10, sClass_11, sClass_12"); }

	virtual bool Fetch()
	{
		_MAKE_WEAPON *pData = new _MAKE_WEAPON;

		_dbCommand->FetchByte(1, pData->byIndex);

		for (int i = 1; i <= 12; i++)
			_dbCommand->FetchUInt16(i+1, pData->sClass[i-1]);

		if (!m_pMap->PutData(pData->byIndex, pData))
			delete pData;

		return true;
	}

	MakeWeaponItemTableArray *m_pMap;
};