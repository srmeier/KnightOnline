#pragma once

class CMakeLareItemTableSet : public OdbcRecordset
{
public:
	CMakeLareItemTableSet(OdbcConnection * dbConnection, MakeLareItemTableArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAKE_ITEM_LARECODE"); }
	virtual tstring GetColumns() { return _T("byLevelGrade, sLareItem, sMagicItem, sGereralItem"); }

	virtual bool Fetch()
	{
		_MAKE_ITEM_LARE_CODE *pData = new _MAKE_ITEM_LARE_CODE;

		_dbCommand->FetchByte(1, pData->byItemLevel);
		_dbCommand->FetchUInt16(2, pData->sLareItem);
		_dbCommand->FetchUInt16(3, pData->sMagicItem);
		_dbCommand->FetchUInt16(4, pData->sGeneralItem);

		if (!m_pMap->PutData(pData->byItemLevel, pData))
			delete pData;

		return true;
	}

	MakeLareItemTableArray *m_pMap;
};