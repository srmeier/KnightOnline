#pragma once

class CUserKnightsRankSet : public CUserPersonalRankSet
{
public:
	CUserKnightsRankSet(OdbcConnection * dbConnection, UserNameRankMap * pMap) 
		: CUserPersonalRankSet(dbConnection, pMap) {}

	virtual tstring GetTableName() { return _T("USER_KNIGHTS_RANK"); }
	virtual tstring GetColumns() { return _T("shIndex, nMoney, strElmoUserID, strKarusUserID, nElmoLoyalty, nKarusLoyalty"); }

	virtual void InsertToMap(std::string & strKarusUserID, std::string & strElmoUserID, _USER_RANK * pData) 
	{
		if (!strElmoUserID.empty())
		{
			m_pMap->insert(make_pair(strElmoUserID, pData));
			g_pMain->m_playerRankings[ELMORAD_ARRAY].insert(std::make_pair((uint8)pData->nRank, pData));
		}

		if (!strKarusUserID.empty())
		{
			m_pMap->insert(make_pair(strKarusUserID, pData));
			g_pMain->m_playerRankings[KARUS_ARRAY].insert(std::make_pair((uint8)pData->nRank, pData));
		}
	}
};