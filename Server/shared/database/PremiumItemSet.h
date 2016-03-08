#pragma once

class CPremiumItemSet : public OdbcRecordset
{
public:
	CPremiumItemSet(OdbcConnection * dbConnection, PremiumItemArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("PREMIUM_ITEM"); }
	virtual tstring GetColumns() { return _T("Type, ExpRestorePercent, NoahPercent, DropPercent, BonusLoyalty, RepairDiscountPercent, ItemSellPercent"); }

	virtual bool Fetch()
	{
		_PREMIUM_ITEM * pData = new _PREMIUM_ITEM;

		_dbCommand->FetchByte(1, pData->Type);
		_dbCommand->FetchUInt16(2, pData->ExpRestorePercent);
		_dbCommand->FetchUInt16(3, pData->NoahPercent);
		_dbCommand->FetchUInt16(4, pData->DropPercent);
		_dbCommand->FetchUInt32(5, pData->BonusLoyalty);
		_dbCommand->FetchUInt16(6, pData->RepairDiscountPercent);
		_dbCommand->FetchUInt16(7, pData->ItemSellPercent);

		if (!m_pMap->PutData(pData->Type, pData))
			delete pData;

		return true;
	}

	PremiumItemArray *m_pMap;
};