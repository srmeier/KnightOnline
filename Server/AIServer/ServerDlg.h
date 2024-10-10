#pragma once

#include "Define.h"
#include "../shared/KOSocketMgr.h"
#include "../shared/database/OdbcConnection.h"

#include "GameSocket.h"

#include "MAP.h"
#include "PathFind.h"

#include "../shared/STLMap.h"

class CNpcThread;
class CNpcTable;
class Unit;

typedef std::map <uint8_t, CNpcThread*>		NpcThreadArray;
typedef CSTLMap <CNpcTable>					NpcTableArray;
typedef CSTLMap <CNpc>						NpcArray;
typedef CSTLMap <_MAGIC_TABLE>				MagictableArray;
typedef CSTLMap <_MAGIC_TYPE1>				Magictype1Array;
typedef CSTLMap <_MAGIC_TYPE2>				Magictype2Array;
typedef CSTLMap <_MAGIC_TYPE4>				Magictype4Array;
typedef CSTLMap <_PARTY_GROUP>				PartyArray;
typedef CSTLMap <_MAKE_WEAPON>				MakeWeaponItemTableArray;
typedef CSTLMap <_MAKE_ITEM_GRADE_CODE>		MakeGradeItemTableArray;
typedef CSTLMap <_MAKE_ITEM_LARE_CODE>		MakeLareItemTableArray;
typedef std::list <int>						ZoneNpcInfoList;
typedef CSTLMap <MAP>						ZoneArray;
typedef CSTLMap <_K_MONSTER_ITEM>			NpcItemArray;
typedef CSTLMap <_MAKE_ITEM_GROUP>			MakeItemGroupArray;
typedef CSTLMap <_SERVER_RESOURCE>			ServerResourceArray;
typedef CSTLMap <_NPC_LIVE_TIME>			NpcLiveTimeArray;
typedef CSTLMap <_OBJECT_EVENT>				ObjectEventArray;

typedef std::map<uint16_t, CUser *>			UserSessionMap;

class CServerDlg
{
private:
	void ResumeAI();
	bool CreateNpcThread();
	bool GetMagicTableData();
	bool GetMagicType1Data();
	bool GetMagicType2Data();
	bool GetMagicType4Data();
	bool GetNpcTableData(bool bNpcData = true);
	bool GetNpcItemTable();
	bool GetMakeItemGroupTable();
	bool GetMakeWeaponItemTableData();
	bool GetMakeDefensiveItemTableData();
	bool GetMakeGradeItemTableData();
	bool GetMakeLareItemTableData();
	bool GetServerResourceTable();
	bool MapFileLoad();
	void GetServerInfoIni();
	bool GetObjectPostTableData();

public:
	CServerDlg();
	bool Startup();

	bool LoadSpawnCallback(OdbcCommand *dbCommand);
	void GameServerAcceptThread();
	void GetServerResource(int nResourceID, std::string * result, ...);
	bool AddObjectEventNpc(_OBJECT_EVENT* pEvent, MAP * pMap);
	CNpc * SpawnEventNpc(uint16_t sSid, bool bIsMonster, uint8_t byZone, float fX, float fY, float fZ, uint16_t sDuration = 0, uint8_t nation = 0, int16_t socketID = -1,uint16_t nEventRoom = 0);
	void NpcUpdate(uint16_t sSid, bool bIsMonster, uint8_t byGroup = 0, uint16_t sPid = 0);

	void RemoveEventNPC(CNpc * pNpc);
	void AllNpcInfo();

	Unit * GetUnitPtr(uint16_t id);
	CNpc * GetNpcPtr(uint16_t npcId);

	CUser* GetUserPtr(uint16_t sessionId);
	bool SetUserPtr(uint16_t sessionId, CUser * pUser);
	void DeleteUserPtr(uint16_t sessionId);

	MAP * GetZoneByID(int zonenumber);

	static uint32_t THREADCALL Timer_CheckAliveTest(void * lpParam);
	static uint32_t THREADCALL Timer_CheckLiveTimes(void * lpParam);
	
	void CheckAliveTest();
	void CheckLiveTimes();
	void DeleteAllUserList(CGameSocket *pSock = nullptr);
	void Send(Packet * pkt);
	void SendSystemMsg(std::string & pMsg, int type=0);
	void ResetBattleZone();

	~CServerDlg();

public:
	NpcArray			m_arNpc;
	NpcTableArray		m_arMonTable;
	NpcTableArray		m_arNpcTable;
	NpcThreadArray		m_arNpcThread;
	NpcThreadArray		m_arEventNpcThread;
	PartyArray			m_arParty;
	ZoneNpcInfoList		m_ZoneNpcList;
	MagictableArray		m_MagictableArray;
	Magictype1Array		m_Magictype1Array;
	Magictype2Array		m_Magictype2Array;
	Magictype4Array		m_Magictype4Array;
	MakeWeaponItemTableArray	m_MakeWeaponItemArray;
	MakeWeaponItemTableArray	m_MakeDefensiveItemArray;
	MakeGradeItemTableArray	m_MakeGradeItemArray;
	MakeLareItemTableArray	m_MakeLareItemArray;
	ZoneArray				g_arZone;
	NpcItemArray			m_NpcItemArray;
	MakeItemGroupArray		m_MakeItemGroupArray;
	ServerResourceArray		m_ServerResourceArray;
	NpcLiveTimeArray		m_NpcLiveTimeArray;
	ObjectEventArray		m_ObjectEventArray;

	Thread m_zoneEventThread;

	std::string m_strGameDSN, m_strGameUID, m_strGamePWD;
	OdbcConnection m_GameDB;

	uint32_t m_AIServerPort;

	UserSessionMap m_pUser;

	Atomic<uint16_t>	m_TotalNPC;			// DB에있는 총 수
	Atomic<uint16_t>	m_CurrentNPC;
	int16_t			m_sTotalMap;		// Zone 수 
	int16_t			m_sMapEventNpc;		// Map에서 읽어들이는 event npc 수

	bool			m_bFirstServerFlag;		// 서버가 처음시작한 후 게임서버가 붙은 경우에는 1, 붙지 않은 경우 0
	uint8_t  m_byBattleEvent;				   // 전쟁 이벤트 관련 플래그( 1:전쟁중이 아님, 0:전쟁중)
	int16_t m_sKillKarusNpc, m_sKillElmoNpc; // 전쟁동안에 죽은 npc숫자

	uint16_t	m_iYear, m_iMonth, m_iDate, m_iHour, m_iMin, m_iAmount;
	uint8_t m_iWeather;
	bool m_bIsNight;

	std::recursive_mutex m_userLock, m_npcThreadLock, m_eventThreadLock;

	KOSocketMgr<CGameSocket> m_socketMgr;
};

extern CServerDlg * g_pMain;