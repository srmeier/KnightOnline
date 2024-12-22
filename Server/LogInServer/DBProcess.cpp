#include "stdafx.h"

bool CDBProcess::Connect(
	const std::string& szDSN,
	const std::string& szUser,
	const std::string& szPass)
{
	if (!m_dbConnection.Connect(szDSN, szUser, szPass))
	{
		g_pMain->ReportSQLError(m_dbConnection.GetError());
		return false;
	}

	return true;
}

bool CDBProcess::LoadVersionList()
{
	std::unique_ptr<OdbcCommand> dbCommand(m_dbConnection.CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("SELECT sVersion, sHistoryVersion, strFilename FROM VERSION")))
	{
		g_pMain->ReportSQLError(m_dbConnection.GetError());
		return false;
	}

	if (dbCommand->hasData())
	{
		g_pMain->m_sLastVersion = 0;
		do
		{
			_VERSION_INFO *pVersion = new _VERSION_INFO;

			dbCommand->FetchUInt16(1, pVersion->sVersion);
			dbCommand->FetchUInt16(2, pVersion->sHistoryVersion);
			dbCommand->FetchString(3, pVersion->strFilename);

			g_pMain->m_VersionList.insert(make_pair(pVersion->strFilename, pVersion));

			if (g_pMain->m_sLastVersion < pVersion->sVersion)
				g_pMain->m_sLastVersion = pVersion->sVersion;

		} while (dbCommand->MoveNext());
	}

	return true;
}

bool CDBProcess::LoadUserCountList()
{
	std::unique_ptr<OdbcCommand> dbCommand(m_dbConnection.CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(_T("SELECT serverid, zone1_count, zone2_count, zone3_count FROM CONCURRENT")))
	{
		g_pMain->ReportSQLError(m_dbConnection.GetError());
		return false;
	}

	if (dbCommand->hasData())
	{
		do
		{
			uint16_t zone_1 = 0, zone_2 = 0, zone_3 = 0; uint8_t serverID;
			dbCommand->FetchByte(1, serverID);
			dbCommand->FetchUInt16(2, zone_1);
			dbCommand->FetchUInt16(3, zone_2);
			dbCommand->FetchUInt16(4, zone_3);

			if ((uint8_t)(serverID - 1) < g_pMain->m_ServerList.size())
				g_pMain->m_ServerList[serverID - 1]->sUserCount = zone_1 + zone_2 + zone_3;
		} while (dbCommand->MoveNext());
	}

	return true;
}

uint16_t CDBProcess::AccountLogin(
	const std::string& strAccountID,
	const std::string& strPasswd)
{
	uint16_t result = AUTH_NOT_FOUND;
	std::unique_ptr<OdbcCommand> dbCommand(m_dbConnection.CreateCommand());
	if (dbCommand.get() == nullptr)
		return AUTH_ERROR;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strPasswd.c_str(), strPasswd.length());
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &result);

	if (!dbCommand->Execute(_T("{CALL ACCOUNT_LOGIN(?, ?, ?)}")))
		g_pMain->ReportSQLError(m_dbConnection.GetError());

	// NOTE: for 1298 I'm adding back the code to check if the user is online
	// will also need to convert from @nRet nation return to the flag that the client is
	// expecting. I am pertty sure it is like this because the login server and game server
	// share the same SQL Procedure for logging in users
	if (result == 1
		|| result == 2
		|| result == 3)
	{
		// NOTE: 1298 returns 1 for "no nation selected" or "no characters on a nation"
		// returns 2 if the user have characters on Karus and returns 3 if use has
		// characters on elmorad. But we will ignore this bit and just access the fact
		// that they are able to login.
		result = AUTH_SUCCESS;
	}
	else
	{
	 // NOTE: else the user doesn't have an account or they are banned or they have
	 // a nation other than the ones we are looking for
		result = AUTH_NOT_FOUND;
	}

	return result;
}

bool CDBProcess::IsAccountLoggedIn(
	const std::string& strAccountID,
	uint16_t* sServerPortNo,
	std::string* szServerIP)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_dbConnection.CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	if (!dbCommand->Execute(_T("SELECT nServerNo, strServerIP FROM CURRENTUSER WHERE strAccountID=?")))
	{
		g_pMain->ReportSQLError(m_dbConnection.GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	char strServerIP[MAX_IP_SIZE + 1] = {};
	dbCommand->FetchUInt16(1, *sServerPortNo);
	dbCommand->FetchString(2, strServerIP, sizeof(strServerIP) - 1);

	*szServerIP = strServerIP;
	return true;
}

int16_t CDBProcess::AccountPremium(
	const std::string& strAccountID)
{
	int16_t sHours = -1;
	return sHours;

	// NOTE: not sure what this would correspond to on 1298 server
	std::unique_ptr<OdbcCommand> dbCommand(m_dbConnection.CreateCommand());
	if (dbCommand.get() == nullptr)
		return sHours;
	
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sHours);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());

	if (!dbCommand->Execute(_T("{? = CALL ACCOUNT_PREMIUM(?)}")))
		g_pMain->ReportSQLError(m_dbConnection.GetError());

	return sHours;
}
