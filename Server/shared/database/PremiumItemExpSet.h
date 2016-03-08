#pragma once

class CPremiumItemExpSet : public OdbcRecordset
{
public:
	CPremiumItemExpSet(OdbcConnection * dbConnection, PremiumItemExpArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("PREMIUM_ITEM_EXP"); }
	virtual tstring GetColumns() { return _T("nIndex, Type, MinLevel, MaxLevel, sPercent"); }

	virtual bool Fetch()
	{
		_PREMIUM_ITEM_EXP * pData = new _PREMIUM_ITEM_EXP;

		_dbCommand->FetchUInt16(1, pData->nIndex);
		_dbCommand->FetchByte(2, pData->Type);
		_dbCommand->FetchByte(3, pData->MinLevel);
		_dbCommand->FetchByte(4, pData->MaxLevel);
		_dbCommand->FetchUInt16(5, pData->sPercent);

		if (!m_pMap->PutData(pData->nIndex, pData))
			delete pData;

		return true;
	}

	PremiumItemExpArray *m_pMap;
};