#include "stdafx.h"
#include <sstream>
#include "../shared/Ini.h"
#include "../shared/DateTime.h"

extern bool g_bRunning;
std::vector<Thread *> g_timerThreads;

LoginServer::LoginServer()
	: m_sLastVersion(__VERSION),
	m_fpLoginServer(nullptr)
{
}

bool LoginServer::Startup()
{
	GetInfoFromIni();

	DateTime time;

	CreateDirectory("Logs", NULL);

	m_fpLoginServer = fopen("./Logs/LoginServer.log", "a");
	if (m_fpLoginServer == nullptr)
	{
		printf("ERROR: Unable to open log file.\n");
		return false;
	}

	m_fpUser = fopen(string_format("./Logs/Login_%d_%d_%d.log", time.GetDay(), time.GetMonth(), time.GetYear()).c_str(), "a");
	if (m_fpUser == nullptr)
	{
		printf("ERROR: Unable to open user log file.\n");
		return false;
	}

	if (!m_DBProcess.Connect(m_ODBCName, m_ODBCLogin, m_ODBCPwd))
	{
		printf("ERROR: Unable to connect to the database using the details configured.\n");
		return false;
	}

	printf("Connected to database server.\n");

	if (!m_DBProcess.LoadAccountMap())
	{
		printf("ERROR: Unable to load the account table (TB_USER).\n");
		return false;
	}

	printf(
		"Accounts loaded: %u\n",
		(uint32_t) m_DBProcess.GetRegisteredUserCount());

	if (!m_DBProcess.LoadVersionList())
	{
		printf("ERROR: Unable to load the version list (VERSION).\n");
		return false;
	}

	printf("Latest version in database: %d\n", GetVersion());

	InitPacketHandlers();

	if (!m_socketMgr.Listen(m_LoginServerPort, MAX_USER))
	{
		printf("ERROR: Failed to listen on server port.\n");
		return false;
	}

	m_socketMgr.RunServer();
	g_timerThreads.push_back(new Thread(Timer_UpdateUserCount));
	g_timerThreads.push_back(new Thread(Timer_UpdateAccountMap));
	return true;
}

uint32_t LoginServer::Timer_UpdateUserCount(
	void* lpParam)
{
	while (g_bRunning)
	{
		g_pMain->UpdateServerList();
		sleep(60 * SECOND);
	}
	return 0;
}

uint32_t LoginServer::Timer_UpdateAccountMap(
	void* lpParam)
{
	while (g_bRunning)
	{
		g_pMain->UpdateAccountMap();
		sleep(60 * SECOND);
	}

	return 0;
}

void LoginServer::GetServerList(
	Packet& result)
{
	Guard lock(m_serverListLock);
	result.append(
		m_serverListPacket.contents(),
		m_serverListPacket.size());
}

void LoginServer::UpdateServerList()
{
	// Update the user counts first
	m_DBProcess.LoadUserCountList();

	Guard lock(m_serverListLock);
	Packet& result = m_serverListPacket;

	result.clear();
	result << uint8_t(m_ServerList.size());
	for (const _SERVER_INFO* pServer : m_ServerList)
	{
		result << pServer->strServerIP;
		result << pServer->strServerName;

		if (pServer->sUserCount <= pServer->sPlayerCap)
			result << pServer->sUserCount;
		else
			result << int16_t(-1);
	}
}

void LoginServer::UpdateAccountMap()
{
	m_DBProcess.LoadAccountMap();
}

void LoginServer::GetInfoFromIni()
{
	CIni ini(CONF_LOGIN_SERVER);

	ini.GetString("DOWNLOAD", "URL", "ftp.yoursite.net", m_strFtpUrl, false);
	ini.GetString("DOWNLOAD", "PATH", "/", m_strFilePath, false);

	ini.GetString("ODBC", "DSN", "KN_online", m_ODBCName, false);
	ini.GetString("ODBC", "UID", "", m_ODBCLogin, false);
	ini.GetString("ODBC", "PWD", "", m_ODBCPwd, false);

	m_LoginServerPort = ini.GetInt("SETTINGS","PORT", 15100);

	int nServerCount = ini.GetInt("SERVER_LIST", "COUNT", 1);
	if (nServerCount <= 0) 
		nServerCount = 1;

	char key[20]; 
	_SERVER_INFO* pInfo = nullptr;

	m_ServerList.reserve(nServerCount);

	// TODO: Replace this nonsense with something a little more versatile
	for (int i = 0; i < nServerCount; i++)
	{
		pInfo = new _SERVER_INFO;

		_snprintf(key, sizeof(key), "SERVER_%02d", i);
		ini.GetString("SERVER_LIST", key, "127.0.0.1", pInfo->strServerIP, false);

		_snprintf(key, sizeof(key), "NAME_%02d", i);
		ini.GetString("SERVER_LIST", key, "TEST|Server 1", pInfo->strServerName, false);

		_snprintf(key, sizeof(key), "ID_%02d", i);
		pInfo->sServerID = ini.GetInt("SERVER_LIST", key, 1);

		// TODO: Remove this entirely. There's only one limit in 1.298.
		int16_t sPremiumLimit = 0, sFreeLimit = 0;

		_snprintf(key, sizeof(key), "PREMLIMIT_%02d", i);
		sPremiumLimit = ini.GetInt("SERVER_LIST", key, MAX_USER);

		_snprintf(key, sizeof(key), "FREELIMIT_%02d", i);
		sFreeLimit = ini.GetInt("SERVER_LIST", key, MAX_USER);

		pInfo->sPlayerCap = std::max(sFreeLimit, sPremiumLimit);

		m_ServerList.push_back(pInfo);
	}

	// Read news from INI (max 3 blocks)
#define BOX_START '#' << uint8_t(0) << '\n'
#define LINE_ENDING uint8_t(0) << '\n'
#define BOX_END BOX_START << LINE_ENDING

	m_news.Size = 0;
	std::stringstream ss;
	for (int i = 0; i < 3; i++)
	{
		std::string title, message;

		_snprintf(key, sizeof(key), "TITLE_%02d", i);
		ini.GetString("NEWS", key, "", title);
		if (title.empty())
			continue;

		_snprintf(key, sizeof(key), "MESSAGE_%02d", i);
		ini.GetString("NEWS", key, "", message);
		if (message.empty())
			continue;

		size_t oldPos = 0, pos = 0;
		ss << title << BOX_START;

		// potentially support multiline by making | act as linebreaks (same as the TBL afaik, so at least we're conformant).
		//replace(messages[i].begin(), messages[i].end(), '|', '\n');
		//while ((pos = message.find('\r', pos)) != string::npos)
		//	message.erase(pos, 1);
		//Remove \n for now, perhaps re-implement later
		//while ((pos = message.find('\n', pos)) != string::npos)
		//	message.erase(pos, 1);

		ss << message << LINE_ENDING << BOX_END;
	}

	m_news.Size = ss.str().size();
	if (m_news.Size)
		memcpy(&m_news.Content, ss.str().c_str(), m_news.Size);
}

void LoginServer::WriteLogFile(
	const std::string& logMessage)
{
	Guard lock(m_lock);
	fwrite(logMessage.c_str(), logMessage.length(), 1, m_fpLoginServer);
	fflush(m_fpLoginServer);
}

void LoginServer::WriteUserLogFile(
	const std::string& logMessage)
{
	Guard lock(m_lock);
	fwrite(logMessage.c_str(), logMessage.length(), 1, m_fpUser);
	fflush(m_fpUser);
}

void LoginServer::ReportSQLError(
	OdbcError* pError)
{
	if (pError == nullptr)
		return;

	// This is *very* temporary.
	std:: string errorMessage = string_format(
		_T("ODBC error occurred.\r\nSource: %s\r\nError: %s\r\nDescription: %s\n"),
		pError->Source.c_str(),
		pError->ExtendedErrorMessage.c_str(),
		pError->ErrorMessage.c_str());

	TRACE("%s", errorMessage.c_str());
	WriteLogFile(errorMessage);
	delete pError;
}

LoginServer::~LoginServer() 
{
	printf("Waiting for timer threads to exit...");
	foreach (itr, g_timerThreads)
	{
		(*itr)->waitForExit();
		delete (*itr);
	}
	printf(" exited.\n");

	foreach (itr, m_ServerList)
		delete *itr;
	m_ServerList.clear();

	foreach (itr, m_VersionList)
		delete itr->second;
	m_VersionList.clear();

	if (m_fpLoginServer != nullptr)
		fclose(m_fpLoginServer);

	if (m_fpUser != nullptr)
		fclose(m_fpUser);

	printf("Shutting down socket system...");
	m_socketMgr.Shutdown();
	printf(" done.\n");
}
