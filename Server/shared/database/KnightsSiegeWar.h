 #pragma once

class CKnightsSiegeWarfare : public OdbcRecordset
{
public:
	CKnightsSiegeWarfare(OdbcConnection * dbConnection, KnightsSiegeWarfareArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("KNIGHTS_SIEGE_WARFARE"); }
	virtual tstring GetColumns() { return _T("sCastleIndex, sMasterKnights, bySiegeType, byWarDay, byWarTime, byWarMinute, sChallengeList_1, sChallengeList_2, sChallengeList_3, sChallengeList_4, sChallengeList_5, sChallengeList_6, sChallengeList_7, sChallengeList_8, sChallengeList_9, sChallengeList_10, byWarRequestDay, byWarRequestTime, byWarRequestMinute, byGuerrillaWarDay, byGuerrillaWarTime, byGuerrillaWarMinute, strChallengeList, sMoradonTariff, sDellosTariff, nDungeonCharge, nMoradonTax, nDellosTax, sRequestList_1, sRequestList_2, sRequestList_3, sRequestList_4, sRequestList_5, sRequestList_6, sRequestList_7, sRequestList_8, sRequestList_9, sRequestList_10"); }

	virtual bool Fetch()
	{
		_KNIGHTS_SIEGE_WARFARE * pData = new _KNIGHTS_SIEGE_WARFARE;
		int i = 1;

		_dbCommand->FetchUInt16(i++, pData->sCastleIndex);
		_dbCommand->FetchUInt16(i++, pData->sMasterKnights);
		_dbCommand->FetchByte(i++, pData->bySiegeType);
		_dbCommand->FetchByte(i++, pData->byWarDay);
		_dbCommand->FetchByte(i++, pData->byWarTime);
		_dbCommand->FetchByte(i++, pData->byWarMinute);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_1);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_2);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_3);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_4);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_5);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_6);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_7);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_8);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_9);
		_dbCommand->FetchUInt16(i++, pData->sChallengeList_10);
		_dbCommand->FetchByte(i++, pData->byWarRequestDay);
		_dbCommand->FetchByte(i++, pData->byWarRequestTime);
		_dbCommand->FetchByte(i++, pData->byWarRequestMinute);
		_dbCommand->FetchByte(i++, pData->byGuerrillaWarDay);
		_dbCommand->FetchByte(i++, pData->byGuerrillaWarTime);
		_dbCommand->FetchByte(i++, pData->byGuerrillaWarMinute);
		_dbCommand->FetchString(i++, pData->strChallengeList);
		_dbCommand->FetchUInt16(i++, pData->sMoradonTariff);
		_dbCommand->FetchUInt16(i++, pData->sDellosTariff);
		_dbCommand->FetchInt32(i++, pData->nDungeonCharge);
		_dbCommand->FetchInt32(i++, pData->nMoradonTax);
		_dbCommand->FetchInt32(i++, pData->nDellosTax);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_1);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_2);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_3);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_4);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_5);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_6);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_7);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_8);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_9);
		_dbCommand->FetchUInt16(i++, pData->sRequestList_10);


		if (!m_pMap->PutData(pData->sCastleIndex, pData))
			delete pData;

		return true;
	}

	KnightsSiegeWarfareArray *m_pMap;
};