#pragma once

#ifdef GAMESERVER

#include "resource.h"
#include "LuaEngine.h"

#include "Define.h"
#include "ChatHandler.h"

class C3DMap;
class CUser;

#include "LoadServerData.h"

#include "User.h"
#include "AISocket.h"
#include "../shared/ClientSocketMgr.h"

typedef std::map<std::string, CUser *>	NameMap;
typedef	std::map<uint16_t, uint16_t>					ForgettenTempleMonsterList;

class CGameServerDlg
{
public:	
	CGameServerDlg();
	bool Startup();

	void GetTimeFromIni();

	bool LoadItemTable();
	bool LoadItemExchangeTable();
	bool LoadItemUpgradeTable();
	bool LoadServerResourceTable();
	bool LoadMagicTable();
	bool LoadMagicType1();
	bool LoadMagicType2();
	bool LoadMagicType3();
	bool LoadMagicType4();
	bool LoadMagicType5();
	bool LoadMagicType6();
	bool LoadMagicType7();
	bool LoadMagicType8();
	bool LoadMagicType9();
	bool LoadRentalList();
	bool LoadCoefficientTable();
	bool LoadLevelUpTable();
	bool LoadAllKnights(bool bIsslient = false);
	bool LoadAllKnightsUserData(bool bIsslient = false);
	bool LoadKnightsAllianceTable(bool bIsslient = false);
	bool LoadKnightsSiegeWarsTable();
	bool LoadUserRankings();
	void CleanupUserRankings();
	bool LoadKnightsCapeTable();
	bool LoadKnightsRankTable(bool bWarTime = false, bool bIsslient = false);
	bool LoadStartPositionTable();
	bool LoadBattleTable();
	bool LoadKingSystem();
	bool LoadMonsterSummonListTable();
	bool LoadEventTriggerTable();
	bool LoadMonsterChallengeTable();
	bool LoadMonsterChallengeSummonListTable();

	bool MapFileLoad();
	bool LoadNoticeData();

	void AIServerConnect();
	
	static uint32_t THREADCALL Timer_CheckGameEvents(void * lpParam);
	static uint32_t THREADCALL Timer_BifrostTime(void * lpParam);
	static uint32_t THREADCALL Timer_UpdateGameTime(void * lpParam);
	static uint32_t THREADCALL Timer_UpdateSessions(void * lpParam);
	static uint32_t THREADCALL Timer_UpdateConcurrent(void * lpParam);

	void ReqUpdateConcurrent();

	void SendFlyingSantaOrAngel();
	void BattleZoneCurrentUsers();
	void GetCaptainUserPtr();
	void Send_CommandChat(Packet *pkt, int nation, CUser* pExceptUser = nullptr);
	void KickOutZoneUsers(uint8_t ZoneID, uint8_t TargetZoneID = 0, uint8_t bNation = 0);
	void SendItemZoneUsers(uint8_t ZoneID, uint32_t nItemID, uint16_t sCount = 1);
	uint64_t GenerateItemSerial();
	int KickOutAllUsers();
	void CheckAliveUser();
	int GetKnightsGrade(uint32_t nPoints);
	void WritePacketLog();
	uint16_t GetKnightsAllMembers(uint16_t sClanID, Packet & result, uint16_t & pktSize, bool bClanLeader);
	void GetUserRank(CUser *pUser);
	void Announcement(uint16_t type, int nation=0, int chat_type=8, CUser* pExceptUser = nullptr, CNpc *pExpectNpc = nullptr);
	void ResetBattleZone();
	void BanishLosers();
	void BattleZoneVictoryCheck();
	void BattleZoneOpenTimer();
	void BattleZoneOpen(int nType, uint8_t bZone = 0);
	void CastleSiegeWarZoneOpen(int nType, uint8_t bZone = 0);
	void BattleZoneClose();
	void BattleZoneResult(uint8_t nation, CUser* pUser = nullptr);
	void BattleWinnerResult(BattleWinnerTypes winnertype);
	void AliveUserCheck();
	void Send_PartyMember(int party, Packet *result);
	void Send_KnightsMember(int index, Packet *pkt);
	void Send_KnightsAlliance(uint16_t sAllianceID, Packet *pkt);
	void SetGameTime();
	void ResetPlayerRankings(uint8_t ZoneID = 0);
	void UpdateWeather();
	void UpdateGameTime();
	void ResetLoyaltyMonthly();
	void SendAllUserInfo();
	void DeleteAllNpcList(int flag = 0);
	CNpc*  FindNpcInZone(uint16_t sPid, uint8_t byZone);
	void ForgettenTempleEventTimer();
	uint8_t GetMonsterChallengeTime();
	std::string GetBattleAndNationMonumentName(int16_t TrapNumber = -1, uint8_t ZoneID = 0);
	void CheckNationMonumentRewards();

	void ReloadKnightAndUserRanks();
	
	bool CastleSiegeWarAttack(CUser *pUser, CUser *pTargetUser);
	void UpdateSiege(int16_t m_sCastleIndex, int16_t m_sMasterKnights, int16_t m_bySiegeType, int16_t m_byWarDay, int16_t m_byWarTime, int16_t m_byWarMinute);
	void UpdateSiegeTax(uint8_t Zone, int16_t ZoneTarrif);

	bool IsDuplicateItem(uint32_t nItemID, uint64_t nItemSerial);
	void AddUserItem(uint32_t nItemID, uint64_t nItemSerial);

	void AddDatabaseRequest(Packet & pkt, CUser *pUser = nullptr);

	// Get info for NPCs in regions around user (WIZ_REQ_NPCIN)
	void NpcInOutForMe(CUser* pSendUser);

	// Get info for NPCs in region
	void GetRegionNpcIn(C3DMap* pMap, uint16_t region_x, uint16_t region_z, Packet& pkt, uint16_t& t_count, CUser* pSendUser);

	// Get list of NPC IDs in region
	void GetRegionNpcList(C3DMap* pMap, uint16_t region_x, uint16_t region_z, Packet& pkt, uint16_t& t_count);

	// Get list of NPCs for regions around user (WIZ_NPC_REGION)
	void RegionNpcInfoForMe(CUser* pSendUser);

	// Get raw list of all units in regions surrounding pOwner.
	void GetUnitListFromSurroundingRegions(Unit* pOwner, std::vector<uint16_t>* pList);

	// Get info for users in regions around user (WIZ_REQ_USERIN)
	void UserInOutForMe(CUser* pSendUser);

	// Get info for users in region
	void GetRegionUserIn(C3DMap* pMap, uint16_t region_x, uint16_t region_z, Packet& pkt, uint16_t& t_count);

	// Get list of user IDs in region
	void GetRegionUserList(C3DMap* pMap, uint16_t region_x, uint16_t region_z, Packet& pkt, uint16_t& t_count);

	// Get list of users for regions around user (WIZ_REGIONCHANGE)
	void RegionUserInOutForMe(CUser* pSendUser);

	// Get info for merchants in regions around user
	void MerchantUserInOutForMe(CUser* pSendUser);

	// Get war status
	INLINE bool isWarOpen() { return m_byBattleOpen != NO_BATTLE;} 

	// Get list of merchants in region
	void GetRegionMerchantUserIn(C3DMap* pMap, uint16_t region_x, uint16_t region_z, Packet & pkt, uint16_t & t_count);

	void SendHelpDescription(CUser *pUser, std::string sHelpMessage);

	INLINE bool isPermanentMessageSet() { return m_bPermanentChatMode; }
	void SetPermanentMessage(const char * format, ...);

	void HandleConsoleCommand(const char * msg);

	template <ChatType chatType>
	INLINE void SendChat(const char * msg, uint8_t byNation = Nation::ALL, bool bFormatNotice = false)
	{
		Packet result;
		std::string buffer;

		if (bFormatNotice)
			GetServerResource(IDP_ANNOUNCEMENT, &buffer, msg);
		else
			buffer = msg;

		ChatPacket::Construct(&result, (uint8_t) chatType, &buffer);
		Send_All(&result, nullptr, byNation);
	}

	template <ChatType chatType>
	INLINE void SendChatToZone(const char * msg, uint8_t ZoneID, uint8_t byNation = Nation::ALL, bool bFormatNotice = false)
	{
		Packet result;
		std::string buffer;

		if (bFormatNotice)
			GetServerResource(IDP_ANNOUNCEMENT, &buffer, msg);
		else
			buffer = msg;

		ChatPacket::Construct(&result, (uint8_t) chatType, &buffer);
		Send_Zone(&result, ZoneID, nullptr, byNation);
	}

	template <ChatType chatType>
	INLINE void SendFormattedChat(const char * msg, uint8_t byNation = Nation::ALL, bool bFormatNotice = false, va_list args = nullptr)
	{
		char buffer[512];
		vsnprintf(buffer, sizeof(buffer), msg, args);
		SendChat<chatType>(buffer, byNation, bFormatNotice);
		va_end(args);
	}

	template <ChatType chatType>
	void SendFormattedChat(const char * msg, uint8_t byNation = Nation::ALL, bool bFormatNotice = false, ...)
	{
		va_list ap;
		va_start(ap, byNation);
		SendFormattedChat<chatType>(msg, byNation, bFormatNotice, ap);
		va_end(ap);
	}

	/* The following are simply wrappers for more readable SendChat() calls */

	INLINE void SendNotice(const char *msg, uint8_t byNation = Nation::ALL) 
	{
		SendChat<PUBLIC_CHAT>(msg, byNation, true);
	}

	template <ChatType chatType>
	INLINE void SendNotice(const char *msg, uint8_t ZoneID, uint8_t byNation = Nation::ALL, bool bFormatNotice = false) 
	{
		SendChatToZone<chatType>(msg, ZoneID, byNation, bFormatNotice);
	}

	void SendFormattedNotice(const char *msg, uint8_t byNation = Nation::ALL, ...)
	{
		va_list ap;
		va_start(ap, byNation);
		SendFormattedChat<PUBLIC_CHAT>(msg, byNation, true, ap);
		va_end(ap);
	}

	INLINE void SendAnnouncement(const char *msg, uint8_t byNation = Nation::ALL)
	{
		SendChat<WAR_SYSTEM_CHAT>(msg, byNation, true);
	}

	void SendFormattedAnnouncement(const char *msg, uint8_t byNation = Nation::ALL, ...)
	{
		va_list ap;
		va_start(ap, byNation);
		SendFormattedChat<WAR_SYSTEM_CHAT>(msg, byNation, true, ap);
		va_end(ap);
	}

	void SendFormattedResource(uint32_t nResourceID, uint8_t byNation = Nation::ALL, bool bIsNotice = true, ...);

	void Send_Region(Packet* pkt, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr);
	void Send_UnitRegion(Packet* pkt, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr);
	void Send_OldRegions(Packet* pkt, int old_x, int old_z, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr);
	void Send_NewRegions(Packet* pkt, int new_x, int new_z, C3DMap* pMap, int x, int z, CUser* pExceptUser = nullptr);

	void Send_NearRegion(Packet* pkt, C3DMap* pMap, int region_x, int region_z, float curx, float curz, CUser* pExceptUser = nullptr);
	void Send_FilterUnitRegion(Packet* pkt, C3DMap* pMap, int x, int z, float ref_x, float ref_z, CUser* pExceptUser = nullptr);

	void Send_Zone_Matched_Class(Packet* pkt, uint8_t bZoneID, CUser* pExceptUser, uint8_t nation, uint8_t seekingPartyOptions);
	void Send_Zone(Packet* pkt, uint8_t bZoneID, CUser* pExceptUser = nullptr, uint8_t nation = 0, float fRange = 0.0f);

	void Send_All(Packet* pkt, CUser* pExceptUser = nullptr, uint8_t nation = 0, uint8_t ZoneID = 0);
	void Send_AIServer(Packet *pkt);

	void GetServerResource(int nResourceID, std::string * result, ...);
	_START_POSITION *GetStartPosition(int nZoneID);

	int64_t GetExpByLevel(int nLevel);
	C3DMap * GetZoneByID(int zoneID);

	CUser * GetUserPtr(std::string findName, NameType type);
	CUser * GetUserPtr(uint16_t sUserId);
	CNpc  * GetNpcPtr(uint16_t sNpcId);

	Unit * GetUnitPtr(uint16_t id);

	// Spawns an event NPC/monster
	void SpawnEventNpc(uint16_t sSid, bool bIsMonster, uint8_t byZone, float fX, float fY, float fZ, uint16_t sCount = 1, uint16_t sRadius = 0, uint16_t sDuration = 0, uint8_t nation = 0, int16_t socketID = -1, uint16_t nEventRoom = 0);

	// Kill a Npc/Monster
	void KillNpc(uint16_t sNid);

	// Change NPC/Monster properties for Respawn
	void NpcUpdate(uint16_t sSid, bool bIsMonster, uint8_t byGroup = 0, uint16_t sPid = 0);	

	// Adds the account name & session to a hashmap (on login)
	void AddAccountName(CUser *pSession);

	// Adds the character name & session to a hashmap (when in-game)
	void AddCharacterName(CUser *pSession);

	// Removes an existing character name/session from the hashmap, replaces the character's name 
	// and reinserts the session with the new name into the hashmap.
	void ReplaceCharacterName(CUser *pSession, std::string & strNewUserID);

	// Removes the account name & character names from the hashmaps (on logout)
	void RemoveSessionNames(CUser *pSession);

	// Send to Zone NPC Effect
	void ShowNpcEffect(uint16_t sNpcID, uint32_t nEffectID, uint8_t ZoneID);

	_PARTY_GROUP* GetPartyPtr(uint16_t sPartyID);
	CKnights* GetClanPtr(uint16_t sClanID);
	_KNIGHTS_ALLIANCE* GetAlliancePtr(uint16_t sAllianceID);
	_ITEM_TABLE* GetItemPtr(uint32_t nItemID);
	_KNIGHTS_SIEGE_WARFARE* GetSiegeMasterKnightsPtr(uint16_t sMasterKnights);

	_PARTY_GROUP* CreateParty(CUser* pLeader);
	void DeleteParty(uint16_t sIndex);

	int32_t GetInitialEventIDForNpc(CNpc* pNpc) const;
	int32_t GetEventTrigger(uint8_t byNpcType, uint16_t sTrapNum) const;

	~CGameServerDlg();

	char	m_ppNotice[20][128];
	std::string	m_AIServerIP;
	uint32_t		m_AIServerPort;

	NpcArray							m_arNpcArray;
	ZoneArray							m_ZoneArray;
	ItemtableArray						m_ItemtableArray;
	MagictableArray						m_MagictableArray;
	Magictype1Array						m_Magictype1Array;
	Magictype2Array						m_Magictype2Array;
	Magictype3Array						m_Magictype3Array;
	Magictype4Array						m_Magictype4Array;
	Magictype5Array						m_Magictype5Array;
	Magictype6Array						m_Magictype6Array;
	Magictype7Array						m_Magictype7Array;
	Magictype8Array						m_Magictype8Array;
	Magictype9Array						m_Magictype9Array;
	CoefficientArray					m_CoefficientArray;
	LevelUpArray						m_LevelUpArray;
	PartyArray							m_PartyArray;
	KnightsArray						m_KnightsArray;
	KnightsRatingArray					m_KnightsRatingArray[2]; // one for both nations
	KnightsAllianceArray				m_KnightsAllianceArray;
	KnightsSiegeWarfareArray			m_KnightsSiegeWarfareArray;
	KnightsCapeArray					m_KnightsCapeArray;
	UserNameRankMap						m_UserPersonalRankMap;
	UserNameRankMap						m_UserKnightsRankMap;
	UserRankMap							m_playerRankings[2]; // one for both nations
	std::recursive_mutex				m_userRankingsLock;
	StartPositionArray					m_StartPositionArray;
	ServerResourceArray					m_ServerResourceArray;
	RentalItemArray						m_RentalItemArray;
	ItemExchangeArray					m_ItemExchangeArray;
	ItemUpgradeArray					m_ItemUpgradeArray;
	KingSystemArray						m_KingSystemArray;
	EventTriggerArray					m_EventTriggerArray;
	MonsterChallengeArray				m_MonsterChallengeArray;
	MonsterChallengeSummonListArray		m_MonsterChallengeSummonListArray;
	MonsterSummonListArray				m_MonsterSummonList;
	UserRankingArray					m_UserRankingArray[2];
	NationMonumentInformationArray		m_NationMonumentInformationArray;
	UserItemArray						m_UserItemArray;
	ObjectEventArray					m_ObjectEventArray;

	Atomic<uint16_t>				m_sPartyIndex;
	int16_t	m_sZoneCount;							// AI Server �����ӽ� ����

	bool	m_bFirstServerFlag;		// ������ ó�������� �� ���Ӽ����� ���� ���쿡�� 1, ���� ���� ���� 0
	bool	m_bServerCheckFlag;
	bool	m_bPointCheckFlag;		// AI������ �������� NPC������ �������� (true:������ ����, false:������ ���� ����)
	int16_t   m_sErrorSocketCount;  // �̻����� ���ÿ�

	uint16_t m_sYear, m_sMonth, m_sDate, m_sHour, m_sMin;
	uint8_t m_byWeather;
	uint16_t m_sWeatherAmount;
	int m_nCastleCapture;
	uint8_t m_ReloadKnightAndUserRanksMinute;

	uint8_t   m_byBattleOpen, m_byOldBattleOpen;
	uint8_t	m_byBattleZone, m_byBattleZoneType, m_bVictory, m_byOldVictory, m_bResultDelayVictory, m_bKarusFlag, m_bElmoradFlag, m_bMiddleStatueNation;
	int32_t	m_byBattleOpenedTime;
	int32_t	m_byBattleTime;
	int32_t	m_byBattleRemainingTime;
	int32_t	m_sBattleTimeDelay;
	int32_t	m_sBattleResultDelay;
	int32_t	m_byBattleSiegeWarStartTime;
	uint8_t	m_SiegeWarNoticeTime;
	std::string	m_SiegeWarWinKnightsNotice;
	bool	m_byBattleSiegeWarOpen;
	bool	m_byBattleSiegeWarMomument;
	bool	m_byBattleSiegeWarTeleport;
	uint8_t	m_sKilledKarusNpc, m_sKilledElmoNpc;
	uint8_t	m_sKarusMonuments, m_sElmoMonuments;
	uint16_t	m_sKarusMonumentPoint, m_sElmoMonumentPoint;
	bool    m_byKarusOpenFlag, m_byElmoradOpenFlag, m_byBanishFlag, m_byBattleSave, m_bResultDelay;
	int16_t   m_sDiscount;
	int16_t	m_sKarusDead, m_sElmoradDead, m_sBanishDelay, m_sKarusCount, m_sElmoradCount;
	std::string m_sBattleZoneOpenDays;
	uint8_t	m_nBattleZoneOpenHourStart[WAR_TIME_COUNT], m_nBattlezoneOpenWarZone[WAR_ZONE_COUNT];

	std::string m_strKarusCaptain, m_strElmoradCaptain;

	uint8_t   m_nBorderDefenseWarTime[BORDER_DEFENSE_WAR_EVENT_COUNT], m_nChaosTime[CHAOS_EVENT_COUNT];
	uint8_t	m_nPVPMonumentNation[MAX_ZONE_ID];

	uint32_t	m_GameServerPort;
	int32_t	m_nGameMasterRHitDamage;
	uint8_t	m_nPlayerRankingResetTime;
	std::string	m_sPlayerRankingsRewardZones;
	uint32_t	m_nPlayerRankingKnightCashReward;
	uint32_t  m_nPlayerRankingLoyaltyReward;

	uint16_t	m_nTempleEventRemainSeconds;

	uint8_t	m_bMaxRegenePoint;

	bool	m_bPermanentChatMode;
	std::string	m_strPermanentChat;

	uint8_t	m_bSantaOrAngel;
	uint8_t	m_sRankResetHour;

	// National Points Settings
	int m_Loyalty_Ardream_Source;
	int m_Loyalty_Ardream_Target;
	int m_Loyalty_Ronark_Land_Base_Source;
	int m_Loyalty_Ronark_Land_Base_Target;
	int m_Loyalty_Ronark_Land_Source;
	int m_Loyalty_Ronark_Land_Target;
	int m_Loyalty_Other_Zone_Source;
	int m_Loyalty_Other_Zone_Target;

	// Bifrost
	uint8_t   m_nBifrostTime[BIFROST_EVENT_COUNT];

	uint8_t m_BifrostVictory;
	uint16_t m_sBifrostRemainingTime;
	uint16_t m_sBifrostTime;
	bool m_sBifrostWarStart;
	uint8_t m_sBifrostVictoryAll;
	uint8_t m_sBifrostVictoryNoticeAll;

	uint16_t m_xBifrostRemainingTime;
	uint16_t m_xJoinOtherNationBifrostTime;
	uint16_t m_xBifrostTime;
	uint16_t m_xBifrostMonumentAttackTime;
	bool m_bAttackBifrostMonument;

	void SendEventRemainingTime(bool bSendAll = false, CUser *pUser = nullptr, uint8_t ZoneID = 0);

	bool m_IsMagicTableInUpdateProcess;
	bool m_IsPlayerRankingUpdateProcess;

	std::vector<int64_t> m_HardwareIDArray;

	// Forgetten Temple
	std::vector<uint16_t>				m_nForgettenTempleUsers;
	bool							m_bForgettenTempleIsActive;
	int8_t							m_nForgettenTempleStartHour;
	int8_t							m_nForgettenTempleLevelMin;
	int8_t							m_nForgettenTempleLevelMax;
	int32_t							m_nForgettenTempleStartTime;
	uint8_t							m_nForgettenTempleChallengeTime;
	bool							m_bForgettenTempleSummonMonsters;
	uint8_t							m_nForgettenTempleCurrentStage;
	uint8_t							m_nForgettenTempleLastStage;
	ForgettenTempleMonsterList		m_ForgettenTempleMonsterList;
	uint32_t							m_nForgettenTempleLastSummonTime;
	bool							m_nForgettenTempleBanishFlag;
	uint8_t							m_nForgettenTempleBanishTime;

	int32_t				m_iEventNumber;

	// zone server info
	int					m_nServerNo, m_nServerGroupNo;
	int					m_nServerGroup;	// server�� ��ȣ(0:�������� ����, 1:����1��, 2:����2��)
	ServerArray			m_ServerArray;
	ServerArray			m_ServerGroupArray;

	NameMap		m_accountNameMap,
		m_characterNameMap;

	std::recursive_mutex	m_accountNameLock,
		m_characterNameLock,
		m_questNpcLock;

	// Controlled weather events set by Kings
	uint8_t m_byKingWeatherEvent;
	uint8_t m_byKingWeatherEvent_Day;
	uint8_t m_byKingWeatherEvent_Hour;
	uint8_t m_byKingWeatherEvent_Minute;

	// XP/coin/NP events
	uint8_t m_byNPEventAmount, m_byExpEventAmount, m_byCoinEventAmount;

	INLINE CLuaEngine* GetLuaEngine() { return &m_luaEngine; }

	KOSocketMgr<CUser> m_socketMgr;
	ClientSocketMgr<CAISocket> m_aiSocketMgr;

	FILE* m_fpDeathUser;
	FILE* m_fpDeathNpc;
	FILE* m_fpChat;
	FILE* m_fpCheat;

	void WriteDeathUserLogFile(const std::string& logMessage);
	void WriteDeathNpcLogFile(const std::string& logMessage);
	void WriteChatLogFile(const std::string& logMessage);
	void WriteCheatLogFile(const std::string& logMessage);

private:
	CLuaEngine	m_luaEngine;

	std::string m_strGameDSN, m_strAccountDSN;
	std::string m_strGameUID, m_strAccountUID;
	std::string m_strGamePWD, m_strAccountPWD;
	bool m_bMarsEnabled;

	bool ProcessServerCommand(std::string & command);

public:
	void InitServerCommands();
	void CleanupServerCommands();

	static ServerCommandTable s_commandTable;

	COMMAND_HANDLER(HandleNoticeCommand);
	COMMAND_HANDLER(HandleKillUserCommand);
	COMMAND_HANDLER(HandleWar1OpenCommand);
	COMMAND_HANDLER(HandleWar2OpenCommand);
	COMMAND_HANDLER(HandleWar3OpenCommand);
	COMMAND_HANDLER(HandleWar4OpenCommand);
	COMMAND_HANDLER(HandleWar5OpenCommand);
	COMMAND_HANDLER(HandleWar6OpenCommand);
	COMMAND_HANDLER(HandleSnowWarOpenCommand);
	COMMAND_HANDLER(HandleSiegeWarOpenCommand);
	COMMAND_HANDLER(HandleWarCloseCommand);
	COMMAND_HANDLER(HandleShutdownCommand);
	COMMAND_HANDLER(HandleDiscountCommand);
	COMMAND_HANDLER(HandleGlobalDiscountCommand);
	COMMAND_HANDLER(HandleDiscountOffCommand);
	COMMAND_HANDLER(HandleCaptainCommand);
	COMMAND_HANDLER(HandleSantaCommand);
	COMMAND_HANDLER(HandleSantaOffCommand);
	COMMAND_HANDLER(HandleAngelCommand);
	COMMAND_HANDLER(HandlePermanentChatCommand);
	COMMAND_HANDLER(HandlePermanentChatOffCommand);
	COMMAND_HANDLER(HandleReloadNoticeCommand);
	COMMAND_HANDLER(HandleReloadTablesCommand);
	COMMAND_HANDLER(HandleReloadMagicsCommand);
	COMMAND_HANDLER(HandleReloadRanksCommand);
	COMMAND_HANDLER(HandleCountCommand);
	COMMAND_HANDLER(HandlePermitConnectCommand);
	COMMAND_HANDLER(HandleWarResultCommand);
};

extern CGameServerDlg * g_pMain;

#endif
