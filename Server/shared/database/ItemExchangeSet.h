#pragma once

class CItemExchangeSet : public OdbcRecordset
{
public:
	CItemExchangeSet(OdbcConnection * dbConnection, ItemExchangeArray *pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("ITEM_EXCHANGE"); }
	virtual tstring GetColumns() 
	{
		return _T("nIndex, nNpcNum, bRandomFlag, "
			"nOriginItemNum1, nOriginItemCount1, nOriginItemNum2, nOriginItemCount2, "
			"nOriginItemNum3, nOriginItemCount3, nOriginItemNum4, nOriginItemCount4, "
			"nOriginItemNum5, nOriginItemCount5, " //nOriginItemNum6, nOriginItemCount6, "
			//"nOriginItemNum7, nOriginItemCount7, nOriginItemNum8, nOriginItemCount8, "
			//"nOriginItemNum9, nOriginItemCount9, nOriginItemNum10, nOriginItemCount10, nOriginItemNum11, nOriginItemCount11, "
			"nExchangeItemNum1, nExchangeItemCount1, nExchangeItemNum2, nExchangeItemCount2, nExchangeItemNum3, nExchangeItemCount3, "
			"nExchangeItemNum4, nExchangeItemCount4, nExchangeItemNum5, nExchangeItemCount5");
	}

	virtual bool Fetch()
	{
		_ITEM_EXCHANGE *pData = new _ITEM_EXCHANGE;

		int i = 1;
		_dbCommand->FetchUInt32(i++, pData->nIndex);
		_dbCommand->FetchUInt16(i++, pData->sNpcNum);
		_dbCommand->FetchByte(i++, pData->bRandomFlag);

		for (int x = 0; x < ITEMS_IN_ORIGIN_GROUP; x++)
		{
			_dbCommand->FetchUInt32(i++, pData->nOriginItemNum[x]);
			_dbCommand->FetchUInt32(i++, pData->sOriginItemCount[x]);
		}

		for (int j = 0; j < ITEMS_IN_EXCHANGE_GROUP; j++)
		{
			_dbCommand->FetchUInt32(i++, pData->nExchangeItemNum[j]);
			_dbCommand->FetchUInt32(i++, pData->sExchangeItemCount[j]);
		}

		if (!m_pMap->PutData(pData->nIndex, pData))
			delete pData;

		return true;
	}

	ItemExchangeArray * m_pMap;
};