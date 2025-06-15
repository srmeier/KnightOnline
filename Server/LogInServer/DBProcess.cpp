#include "stdafx.h"

CDBProcess::CDBProcess()
	: m_bUseShortFormVersionTable(false)
{
}

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

bool CDBProcess::LoadVersionList(
	bool bSuppressErrors /*= false*/)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_dbConnection.CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	std::string szSQL;

	if (m_bUseShortFormVersionTable)
		szSQL = _T("SELECT version, hisversion, filename FROM VERSION");
	else
		szSQL = _T("SELECT sVersion, sHistoryVersion, strFilename FROM VERSION");

	if (!dbCommand->Execute(szSQL))
	{
		if (!bSuppressErrors)
			g_pMain->ReportSQLError(m_dbConnection.GetError());

		return false;
	}

	if (dbCommand->hasData())
	{
		g_pMain->m_sLastVersion = 0;
		do
		{
			_VERSION_INFO* pVersion = new _VERSION_INFO;

			dbCommand->FetchUInt16(1, pVersion->sVersion);
			dbCommand->FetchUInt16(2, pVersion->sHistoryVersion);
			dbCommand->FetchString(3, pVersion->strFilename);

			g_pMain->m_VersionList.insert(make_pair(pVersion->strFilename, pVersion));

			if (g_pMain->m_sLastVersion < pVersion->sVersion)
				g_pMain->m_sLastVersion = pVersion->sVersion;

		}
		while (dbCommand->MoveNext());
	}

	return true;
}

bool CDBProcess::LoadUserCountList()
{
	std::lock_guard<std::recursive_mutex> lock(m_dbConnection.GetLock());
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
			uint16_t zone_1 = 0, zone_2 = 0, zone_3 = 0;
			uint8_t serverID;

			dbCommand->FetchByte(1, serverID);
			dbCommand->FetchUInt16(2, zone_1);
			dbCommand->FetchUInt16(3, zone_2);
			dbCommand->FetchUInt16(4, zone_3);

			if ((uint8_t) (serverID - 1) < g_pMain->m_ServerList.size())
				g_pMain->m_ServerList[serverID - 1]->sUserCount = zone_1 + zone_2 + zone_3;
		}
		while (dbCommand->MoveNext());
	}

	return true;
}

bool CDBProcess::LoadAccountMap()
{
	std::map<std::string, _TB_USER> accountMap;

	{
		std::lock_guard<std::recursive_mutex> lock(m_dbConnection.GetLock());

		std::unique_ptr<OdbcCommand> dbCommand(m_dbConnection.CreateCommand());
		if (dbCommand.get() == nullptr)
			return false;

		if (!dbCommand->Execute(_T("SELECT strAccountID, strPasswd, strAuthority FROM TB_USER")))
		{
			g_pMain->ReportSQLError(m_dbConnection.GetError());
			return false;
		}

		if (dbCommand->hasData())
		{
			std::string key;
			do
			{
				char strAccountID[MAX_ID_SIZE + 1] = {}, strPasswd[MAX_PW_SIZE + 1] = {};
				_TB_USER user = {};

				dbCommand->FetchString(1, strAccountID, sizeof(strAccountID) - 1);
				dbCommand->FetchString(2, strPasswd, sizeof(strPasswd) - 1);
				dbCommand->FetchByte(3, user.byAuthority);

				user.strAccountID = strAccountID;
				user.strPasswd = strPasswd;

				key = user.strAccountID;

				// NOTE: By default we use case-insensitive matching for account names.
				// As such, we'll transform this to uppercase for the key.
				STRTOUPPER(key);

				accountMap.insert(
					std::make_pair(key, std::move(user)));
			}
			while (dbCommand->MoveNext());
		}
	}

	{
		std::lock_guard<std::recursive_mutex> lock(m_accountMapLock);
		m_accountMap.swap(accountMap);
	}

	return true;
}

uint16_t CDBProcess::AccountLogin(
	const std::string& strAccountID,
	const std::string& strPasswd)
	const
{
	std::string key = strAccountID;

	// NOTE: By default we use case-insensitive matching for account names.
	// As such, we'll transform this to uppercase for the key.
	STRTOUPPER(key);

	std::lock_guard<std::recursive_mutex> lock(m_accountMapLock);
	auto itr = m_accountMap.find(key);
	if (itr == m_accountMap.end())
		return AUTH_NOT_FOUND;

	const auto& account = itr->second;

	// NOTE: Ideally this should be vague and just return a generic
	// "account not found or invalid password" error.
	if (account.strPasswd != strPasswd)
		return AUTH_INVALID_PW;

	if (account.byAuthority == AUTHORITY_BANNED)
		return AUTH_BANNED;

	return AUTH_SUCCESS;
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
