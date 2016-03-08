#pragma once

class CMakeItemGroupSet : public OdbcRecordset
{
public:
	CMakeItemGroupSet(OdbcConnection * dbConnection, MakeItemGroupArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAKE_ITEM_GROUP"); }
	virtual tstring GetColumns() { return _T("iItemGroupNum, iItem_1, iItem_2, iItem_3, iItem_4, iItem_5, iItem_6, iItem_7, iItem_8, iItem_9, iItem_10, iItem_11, iItem_12, iItem_13, iItem_14, iItem_15, iItem_16, iItem_17, iItem_18, iItem_19, iItem_20, iItem_21, iItem_22, iItem_23, iItem_24, iItem_25, iItem_26, iItem_27, iItem_28, iItem_29, iItem_30"); }

	virtual bool Fetch()
	{
		_MAKE_ITEM_GROUP *pData = new _MAKE_ITEM_GROUP;

		_dbCommand->FetchUInt32(1, pData->iItemGroupNum);
		for (int i = 1; i <= 30; i++)
		{
			uint32 iItem;
			_dbCommand->FetchUInt32(i + 1, iItem);

			// Insert regardless of whether it's set or not. This is official behaviour.
			pData->iItems.push_back(iItem);
		}

		if (!m_pMap->PutData(pData->iItemGroupNum, pData))
			delete pData;

		return true;
	}

	MakeItemGroupArray *m_pMap;
};