#pragma once

class CRentalItemSet : public OdbcRecordset
{
public:
	CRentalItemSet(OdbcConnection * dbConnection, RentalItemArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("RENTAL_ITEM"); }
	virtual tstring GetColumns() { return _T("nRentalIndex, nItemIndex, sDurability, nSerialNumber, byRegType, byItemType, byClass, sRentalTime, nRentalMoney, strLenderCharID, strBorrowerCharID"); }

	virtual bool Fetch()
	{
		_RENTAL_ITEM *pData = new _RENTAL_ITEM;

		_dbCommand->FetchUInt32(1, pData->nRentalIndex);
		_dbCommand->FetchUInt32(2, pData->nItemID);
		_dbCommand->FetchUInt16(3, pData->sDurability);
		_dbCommand->FetchUInt64(4, pData->nSerialNum);
		_dbCommand->FetchByte(5, pData->byRegType);
		_dbCommand->FetchByte(6, pData->byItemType);
		_dbCommand->FetchByte(7, pData->byClass);
		_dbCommand->FetchUInt16(8, pData->sRentalTime);
		_dbCommand->FetchUInt32(9, pData->nRentalMoney);
		_dbCommand->FetchString(10, pData->strLenderCharID);
		_dbCommand->FetchString(11, pData->strBorrowerCharID);

		if (!m_pMap->PutData(pData->nRentalIndex, pData))
			delete pData;

		return true;
	}

	RentalItemArray *m_pMap;
};