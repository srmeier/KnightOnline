#pragma once

#include "../shared/KOSocketMgr.h"

typedef std::map <std::string, _VERSION_INFO *> VersionInfoList;
typedef std::vector<_SERVER_INFO *>	ServerInfoList;

class LoginSession;
class LoginServer
{
	friend class CDBProcess;
public:
	LoginServer();

	INLINE short GetVersion() { return m_sLastVersion; };
	INLINE std::string & GetFTPUrl() { return m_strFtpUrl; };
	INLINE std::string & GetFTPPath() { return m_strFilePath; };

	INLINE News * GetNews() { return &m_news; };

	INLINE VersionInfoList* GetPatchList() { return &m_VersionList; };

	bool Startup();

	static uint32 THREADCALL Timer_UpdateUserCount(void * lpParam);
	void GetServerList(Packet & result);

	~LoginServer();

	KOSocketMgr<LoginSession> m_socketMgr;

private:
	void UpdateServerList();
	void GetInfoFromIni();
	void WriteLogFile(std::string & logMessage);
	void ReportSQLError(OdbcError *pError);

	std::string m_strFtpUrl, m_strFilePath;
	std::string m_ODBCName, m_ODBCLogin, m_ODBCPwd;
	short	m_sLastVersion;

	uint32 m_LoginServerPort;

	VersionInfoList		m_VersionList;
	ServerInfoList		m_ServerList;

	News m_news;

	RWLock m_patchListLock;
	Packet m_serverListPacket;
	std::recursive_mutex m_lock, m_serverListLock;

	FILE *m_fpLoginServer;
public:
	CDBProcess	m_DBProcess;
	void WriteUserLogFile(std::string & logMessage);

	FILE *m_fpUser;
};

extern LoginServer * g_pMain;
