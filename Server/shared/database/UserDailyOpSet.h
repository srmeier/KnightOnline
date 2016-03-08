#pragma once

class CUserDailyOpSet : public OdbcRecordset
{
public:
	CUserDailyOpSet(OdbcConnection * dbConnection, UserDailyOpMap * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("USER_DAILY_OP"); }
	virtual tstring GetColumns() { return _T("strUserId, ChaosMapTime, UserRankRewardTime, PersonalRankRewardTime, KingWingTime, WarderKillerTime1, WarderKillerTime2, KeeperKillerTime, UserLoyaltyWingRewardTime"); }

	virtual bool Fetch()
	{
		_USER_DAILY_OP * pData = new _USER_DAILY_OP;

		int i = 1;
		_dbCommand->FetchString(i++, pData->strUserId);
		_dbCommand->FetchInt32(i++, pData->ChaosMapTime);
		_dbCommand->FetchInt32(i++, pData->UserRankRewardTime);
		_dbCommand->FetchInt32(i++, pData->PersonalRankRewardTime);
		_dbCommand->FetchInt32(i++, pData->KingWingTime);
		_dbCommand->FetchInt32(i++, pData->WarderKillerTime1);
		_dbCommand->FetchInt32(i++, pData->WarderKillerTime2);
		_dbCommand->FetchInt32(i++, pData->KeeperKillerTime);
		_dbCommand->FetchInt32(i++, pData->UserLoyaltyWingRewardTime);

		if (pData->strUserId.empty())
		{
			delete pData;
			return true;
		}

		InsertToMap(pData);
		return true;
	}

	virtual void InsertToMap(_USER_DAILY_OP * pData) 
	{
		if (!pData->strUserId.empty())
			m_pMap->insert(make_pair(pData->strUserId, pData));
	}

	UserDailyOpMap *m_pMap;
};