#pragma once

class CItemUpgradeSet : public OdbcRecordset
{
public:
	CItemUpgradeSet(OdbcConnection * dbConnection, ItemUpgradeArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("ITEM_UPGRADE"); }
	virtual tstring GetColumns() { return _T("nIndex, nNPCNum, nOriginType, nOriginItem, "
		"nReqItem1, nReqItem2, nReqItem3, nReqItem4, nReqItem5, nReqItem6, nReqItem7, nReqItem8, "
		"nReqNoah, bRateType, nGenRate, nGiveItem"); }

	virtual bool Fetch()
	{
		_ITEM_UPGRADE *pData = new _ITEM_UPGRADE;

		int i = 1;
		_dbCommand->FetchUInt32(i++, pData->nIndex);
		_dbCommand->FetchUInt16(i++, pData->sNpcNum);
		_dbCommand->FetchSByte(i++, pData->bOriginType);
		_dbCommand->FetchUInt16(i++, pData->sOriginItem);
		for (int x = 0; x < MAX_ITEMS_REQ_FOR_UPGRADE; x++)
			_dbCommand->FetchUInt32(i++, pData->nReqItem[x]);
		_dbCommand->FetchUInt32(i++, pData->nReqNoah);
		_dbCommand->FetchByte(i++, pData->bRateType);
		_dbCommand->FetchUInt16(i++, pData->sGenRate);
		_dbCommand->FetchInt32(i++, pData->nGiveItem);

		if (!m_pMap->PutData(pData->nIndex, pData))
			delete pData;

		return true;
	}

	ItemUpgradeArray *m_pMap;
};