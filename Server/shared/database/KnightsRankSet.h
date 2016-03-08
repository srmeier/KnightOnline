#pragma once

class CKnightsRankSet : public OdbcRecordset
{
public:
	CKnightsRankSet(OdbcConnection * dbConnection, void * dummy) 
		: OdbcRecordset(dbConnection), nKarusCount(0), nElmoCount(0) 
	{
		memset(&strKarusCaptain, 0, sizeof(strKarusCaptain));
		memset(&strElmoCaptain, 0, sizeof(strElmoCaptain));
	}

	virtual tstring GetTableName() { return _T("KNIGHTS_RATING"); }
	virtual tstring GetColumns() { return _T("nRank, shIndex, nPoints"); }

	virtual bool Fetch()
	{
		_KNIGHTS_RATING * pData = new _KNIGHTS_RATING;

		_dbCommand->FetchUInt32(1, pData->nRank);
		_dbCommand->FetchUInt16(2, pData->sClanID);
		_dbCommand->FetchUInt32(3, pData->nPoints);

		CKnights *pKnights = g_pMain->GetClanPtr(pData->sClanID);
		if (pKnights == nullptr 
			|| (pKnights->m_byNation != KARUS && pKnights->m_byNation != ELMORAD)
			|| !g_pMain->m_KnightsRatingArray[pKnights->m_byNation - 1].PutData(pData->nRank, pData))
		{
			delete pData;
			return true;
		}

		if (pKnights->m_byNation == KARUS)
		{
			if (nKarusCount == 5)
				return true;

			CUser *pUser = g_pMain->GetUserPtr(pKnights->m_strChief, TYPE_CHARACTER);
			if (pUser == nullptr || !pUser->GetMap()->isWarZone())
				return true;

			if (pUser->GetClanID() == pData->sClanID)
			{
				_snprintf(strKarusCaptain[nKarusCount++], 50, "[%s][%s]", pKnights->m_strName.c_str(), pUser->GetName().c_str());
				pUser->ChangeFame(COMMAND_CAPTAIN);
			}
		}
		else if (pKnights->m_byNation == ELMORAD)
		{
			if (nElmoCount == 5)
				return true;

			CUser *pUser = g_pMain->GetUserPtr(pKnights->m_strChief, TYPE_CHARACTER);
			if (pUser == nullptr || !pUser->GetMap()->isWarZone())
				return true;

			if (pUser->GetClanID() == pData->sClanID)
			{
				_snprintf(strElmoCaptain[nElmoCount++], 50, "[%s][%s]", pKnights->m_strName.c_str(), pUser->GetName().c_str());
				pUser->ChangeFame(COMMAND_CAPTAIN);
			}
		}

		return true;
	}

	char strKarusCaptain[5][50], strElmoCaptain[5][50];	
	uint32 nKarusCount, nElmoCount;
};