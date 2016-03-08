#pragma once

class CKnightsCapeSet : public OdbcRecordset
{
public:
	CKnightsCapeSet(OdbcConnection * dbConnection, KnightsCapeArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("KNIGHTS_CAPE"); }
	virtual tstring GetColumns() {
		return _T(
			"sCapeIndex, "\
			"nBuyPrice, "\
			"byGrade" //,
			/*"nBuyLoyalty, "\*/
			/*"byRanking"*/
		);
	}

	virtual bool Fetch()
	{
		_KNIGHTS_CAPE *pData = new _KNIGHTS_CAPE;

		_dbCommand->FetchUInt16(1, pData->sCapeIndex);
		_dbCommand->FetchUInt32(2, pData->nReqCoins);
		_dbCommand->FetchByte(3, pData->byGrade);
		pData->nReqClanPoints = 0; //_dbCommand->FetchUInt32(4, pData->nReqClanPoints);
		pData->byRanking = 0; //_dbCommand->FetchByte(5, pData->byRanking);

#if 0	// https://github.com/twostars/snoxd-koserver/issues/168#issuecomment-19630966
		// This assumes the price is in NP, not clan points... 
		// The above comment disagrees with this behaviour, so disabling temporarily.
		// Need to find out for sure what the price is in.

		// Convert this from NP to clan points
		pData->nReqClanPoints /= MAX_CLAN_USERS;
#endif

		if (!m_pMap->PutData(pData->sCapeIndex, pData))
			delete pData;

		return true;
	}

	KnightsCapeArray *m_pMap;
};