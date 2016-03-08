#pragma once

class CUserPersonalRankSet : public OdbcRecordset
{
public:
	CUserPersonalRankSet(OdbcConnection * dbConnection, UserNameRankMap * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("USER_PERSONAL_RANK"); }
	virtual tstring GetColumns() { return _T("nRank, nSalary, strElmoUserID, strKarusUserID, nElmoLoyaltyMonthly, nKarusLoyaltyMonthly"); }

	virtual bool Fetch()
	{
		_USER_RANK *pData = new _USER_RANK;
		std::string strElmoUserID, strKarusUserID;

		_dbCommand->FetchUInt16(1, pData->nRank);
		_dbCommand->FetchUInt32(2, pData->nSalary);
		_dbCommand->FetchString(3, strElmoUserID);
		_dbCommand->FetchString(4, strKarusUserID);
		_dbCommand->FetchUInt32(5, pData->nLoyalty[ELMORAD_ARRAY]); // monthly NP for USER_PERSONAL_RANK
		_dbCommand->FetchUInt32(6, pData->nLoyalty[KARUS_ARRAY]);

		// Copy strings into struct
		pData->strUserID[ELMORAD_ARRAY] = strElmoUserID;
		pData->strUserID[KARUS_ARRAY] = strKarusUserID;

		// Convert keys to uppercase for case insensitive lookups
		STRTOUPPER(strElmoUserID);
		STRTOUPPER(strKarusUserID);

		// We're not going to insert either of them, so ignore this row and avoid a mem leak.
		if (strElmoUserID.empty() && strKarusUserID.empty())
		{
			delete pData;
			return true; // this is normal.
		}

		InsertToMap(strKarusUserID, strElmoUserID, pData);
		return true;
	}

	virtual void InsertToMap(std::string & strKarusUserID, std::string & strElmoUserID, _USER_RANK * pData) 
	{
		if (!strElmoUserID.empty())
			m_pMap->insert(make_pair(strElmoUserID, pData));

		if (!strKarusUserID.empty())
			m_pMap->insert(make_pair(strKarusUserID, pData));
	}

	UserNameRankMap *m_pMap;
};