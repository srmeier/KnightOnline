#pragma once

#include "LuaEngine.h"
#include "../shared/KOSocket.h"

#include "Unit.h"
#include "ChatHandler.h"

struct _KNIGHTS_USER;
struct _EXCHANGE_ITEM;
struct _USER_SEAL_ITEM;

typedef std::map<uint64_t, _USER_SEAL_ITEM*>	UserItemSealMap;
typedef	std::list<_EXCHANGE_ITEM*>			ItemList;
typedef	std::map<uint32_t, time_t>			SkillCooldownList;
typedef	std::map<uint8_t, time_t>				MagicTypeCooldownList;
typedef	std::map<uint16_t, time_t>			RHitRepeatList; 
typedef	std::map<uint32_t, time_t>			UserSavedMagicMap;

// Time (in seconds) between each save request (3 min).
#define PLAYER_SAVE_INTERVAL			(3 * 60)
// Time (in seconds) between each skill request (-1 sec).
#define PLAYER_SKILL_REQUEST_INTERVAL	0.7f
// Time (in seconds) between each r hit request (-1 sec).
#define PLAYER_R_HIT_REQUEST_INTERVAL	1.0f
// Time (in minute) for daily operations
#define DAILY_OPERATIONS_MINUTE			1440
// Time (in seconds) for nation monuments
#define NATION_MONUMENT_REWARD_SECOND	60

enum GameState
{
	GAME_STATE_CONNECTED,
	GAME_STATE_INGAME
};

enum MerchantState
{
	MERCHANT_STATE_NONE		= -1,
	MERCHANT_STATE_SELLING	= 0,
	MERCHANT_STATE_BUYING	= 1
};

enum ClassType
{
	ClassWarrior		= 1,
	ClassRogue			= 2,
	ClassMage			= 3,
	ClassPriest			= 4,
	ClassWarriorNovice	= 5,
	ClassWarriorMaster	= 6,
	ClassRogueNovice	= 7,
	ClassRogueMaster	= 8,
	ClassMageNovice		= 9,
	ClassMageMaster		= 10,
	ClassPriestNovice	= 11,
	ClassPriestMaster	= 12
};

enum WarpListResponse
{
	WarpListGenericError		= 0,
	WarpListSuccess				= 1,  // "You've arrived at."
	WarpListMinLevel			= 2,  // "You need to be at least level <level>."
	WarpListNotDuringCSW		= 3,  // "You cannot enter during the Castle Siege War."
	WarpListNotDuringWar		= 4,  // "You cannot enter during the Lunar War."
	WarpListNeedNP				= 5,  // "You cannot enter when you have 0 national points."
	WarpListWrongLevelDLW		= 6,  // "Only characters with level 30~50 can enter." (dialog) 
	WarpListDoNotQualify		= 7,  // "You can not enter because you do not qualify." (dialog) 
	WarpListRecentlyTraded		= 8,  // "You can't teleport for 2 minutes after trading." (dialog) 
	WarpListArenaFull			= 9,  // "Arena Server is full to capacity. Please try again later." (dialog) 
	WarpListFinished7KeysQuest	= 10, // "You can't enter because you completed Guardian of 7 Keys quest." (dialog) 
};

enum TransformationType
{
	TransformationNone,
	TransformationMonster,
	TransformationNPC,
	TransformationSiege
};


enum TeamColour
{
	TeamColourNone = 0,
	TeamColourBlue,
	TeamColourRed
};

#define ARROW_EXPIRATION_TIME (5) // seconds

struct Arrow
{
	uint32_t nSkillID;
	time_t tFlyingTime;

	Arrow(uint32_t nSkillID, time_t tFlyingTime) 
	{
		this->nSkillID = nSkillID;
		this->tFlyingTime = tFlyingTime;
	}
};

typedef std::vector<Arrow> ArrowList;

#include "GameDefine.h"

class CGameServerDlg;
class CUser : public Unit, public KOSocket
{
public:
	virtual uint16_t GetID() { return GetSocketID(); }

	std::string & GetAccountName() { return m_strAccountID; }
	virtual std::string & GetName() { return m_strUserID; }

	bool m_bHasCheckedClientVersion;

	std::string	m_strAccountID, m_strUserID;

	uint8_t	m_bRace;
	uint16_t	m_sClass;

	uint8_t	m_nHair;

	uint8_t	m_bRank;
	uint8_t	m_bTitle;
	int64_t	m_iExp;	
	uint32_t	m_iLoyalty, m_iLoyaltyMonthly;
	uint32_t	m_iMannerPoint;
	uint8_t	m_bFace;
	uint8_t	m_bCity;
	int16_t	m_bKnights;	
	uint8_t	m_bFame;
	int16_t	m_sHp, m_sMp, m_sSp;
	uint8_t	m_bStats[STAT_COUNT];
	uint8_t	m_bAuthority;
	int16_t	m_sPoints; // this is just to shut the compiler up
	uint32_t	m_iGold, m_iBank;
	int16_t	m_sBind;

	uint8_t    m_bstrSkill[10];	
	_ITEM_DATA m_sItemArray[INVENTORY_TOTAL];
	_ITEM_DATA m_sWarehouseArray[WAREHOUSE_MAX];

	uint8_t	m_bLogout;
	uint32_t	m_dwTime;
	time_t	m_lastSaveTime;

	uint8_t	m_bAccountStatus;
	uint8_t	m_bPremiumType;
	uint16_t	m_sPremiumTime;
	uint32_t  m_nKnightCash;

	bool	m_bSelectedCharacter;
	bool	m_bStoreOpen;

	int8_t	m_bMerchantState;
	int16_t	m_sMerchantsSocketID;
	std::list<uint16_t>	m_arMerchantLookers;
	_MERCH_DATA	m_arMerchantItems[MAX_MERCH_ITEMS]; //What is this person selling? Stored in "_MERCH_DATA" structure.
	bool	m_bPremiumMerchant;
	UserItemSealMap m_sealedItemMap;

	uint8_t	m_bRequestingChallenge, // opcode of challenge request being sent by challenger
		m_bChallengeRequested;  // opcode of challenge request received by challengee
	int16_t	m_sChallengeUser;

	// Rival system
	int16_t	m_sRivalID;			// rival's session ID
	time_t	m_tRivalExpiryTime;	// time when the rivalry ends

	// Anger gauge system 
	uint8_t	m_byAngerGauge; // values range from 0-5

	// Magic System Cooldown checks
	SkillCooldownList	m_CoolDownList;

	// Magic System Same time magic type checks
	MagicTypeCooldownList	m_MagicTypeCooldownList;

	// Attack System Cooldown checks
	RHitRepeatList	m_RHitRepeatList;

	ArrowList m_flyingArrows;
	std::recursive_mutex m_arrowLock;

	TransformationType m_transformationType;
	uint16_t	m_sTransformID;
	time_t	m_tTransformationStartTime;
	uint16_t	m_sTransformationDuration;

	bool	m_bIsChicken; // Is the character taking the beginner/chicken quest?
	bool	m_bIsHidingHelmet;

	bool	m_bMining;
	time_t	m_tLastMiningAttempt;

	int8_t	m_bPersonalRank;
	int8_t	m_bKnightsRank;

	float	m_oldx, m_oldy, m_oldz;
	int16_t	m_sDirection;

	int64_t	m_iMaxExp;

	uint16_t	m_sMaxWeight;
	uint16_t	m_sMaxWeightBonus;
	uint16_t CUser::MaxWeight (uint16_t MaxWeight);
	int16_t   m_sSpeed;

	uint8_t	m_bPlayerAttackAmount;
	uint8_t	m_bAddWeaponDamage;
	uint16_t	m_sAddArmourAc; 
	uint8_t	m_bPctArmourAc;

	int16_t	m_sItemMaxHp;
	int16_t	m_sItemMaxMp;
	uint16_t	m_sItemWeight;
	int16_t	m_sItemAc;
	int16_t	m_sItemHitrate;
	int16_t	m_sItemEvasionrate;

	uint8_t	m_byAPBonusAmount;
	uint8_t	m_byAPClassBonusAmount[4]; // one for each of the 4 class types
	uint8_t	m_byAcClassBonusAmount[4]; // one for each of the 4 class types

	int16_t	m_sStatItemBonuses[STAT_COUNT];
	int8_t	m_bStatBuffs[STAT_COUNT];

	uint16_t	m_sExpGainAmount;
	uint8_t	m_bItemExpGainAmount;
	uint8_t	m_bNPGainAmount, m_bItemNPBonus, m_bSkillNPBonus;
	uint8_t	m_bNoahGainAmount, m_bItemNoahGainAmount;
	uint8_t	m_bMaxWeightAmount; 

	int16_t	m_iMaxHp, m_iMaxMp;

	uint8_t	m_bResHpType;
	bool	m_bWarp;
	uint8_t	m_bNeedParty;

	uint16_t	m_sPartyIndex;
	bool	m_bInParty;
	bool	m_bPartyLeader;

	bool	m_bCanSeeStealth;
	uint8_t	m_bInvisibilityType;

	int16_t	m_sExchangeUser;
	uint8_t	m_bExchangeOK;

	ItemList	m_ExchangeItemList;

	bool	m_bBlockPrivateChat;
	int16_t	m_sPrivateChatUser;

	time_t	m_tHPLastTimeNormal;					// For Automatic HP recovery. 
	time_t	m_tHPStartTimeNormal;
	int16_t	m_bHPAmountNormal;
	uint8_t	m_bHPDurationNormal;
	uint8_t	m_bHPIntervalNormal;

	time_t m_tGameStartTimeSavedMagic;

	uint32_t	m_fSpeedHackClientTime, m_fSpeedHackServerTime;
	uint8_t	m_bSpeedHackCheck;

	time_t	m_tBlinkExpiryTime;			// When you should stop blinking.

	uint32_t	m_bAbnormalType;			// Is the player normal, a giant, or a dwarf?
	uint32_t	m_nOldAbnormalType;

	int16_t	m_sWhoKilledMe;				// ID of the unit that killed you.
	int64_t	m_iLostExp;					// Experience points that were lost when you died.

	time_t	m_tLastTrapAreaTime;		// The last moment you were in the trap area.

	bool	m_bZoneChangeFlag;

	uint8_t	m_bRegeneType;				// Did you die and go home or did you type '/town'?

	time_t	m_tLastRegeneTime;			// The last moment you got resurrected.

	bool	m_bZoneChangeSameZone;		// Did the server change when you warped?

	int		m_iSelMsgEvent[MAX_MESSAGE_EVENT];
	int16_t	m_sEventNid, m_sEventSid;
	uint32_t	m_nQuestHelperID;

	bool	m_bWeaponsDisabled;

	TeamColour	m_teamColour;
	uint32_t		m_iLoyaltyDaily;
	uint16_t		m_iLoyaltyPremiumBonus;
	uint16_t		m_KillCount;
	uint16_t		m_DeathCount;

	float		m_LastX;
	float		m_LastZ;

public:
	INLINE bool isBanned() { return GetAuthority() == AUTHORITY_BANNED; }
	INLINE bool isMuted() { return GetAuthority() == AUTHORITY_MUTED; }
	INLINE bool isAttackDisabled() { return GetAuthority() == AUTHORITY_ATTACK_DISABLED; }
	INLINE bool isGM() { return GetAuthority() == AUTHORITY_GAME_MASTER; }
	INLINE bool isLimitedGM() { return GetAuthority() == AUTHORITY_LIMITED_GAME_MASTER; }

	virtual bool isDead() { return m_bResHpType == USER_DEAD || m_sHp <= 0; }
	virtual bool isBlinking() { return m_bAbnormalType == ABNORMAL_BLINKING; }

	INLINE bool isInGame() { return GetState() == GAME_STATE_INGAME; }
	INLINE bool isInParty() { return m_bInParty; }
	INLINE bool isInClan() { return GetClanID() > 0; }

	INLINE bool isKing() { return m_bRank == 1; }
	INLINE bool isClanLeader() { return GetFame() == CHIEF; }
	INLINE bool isClanAssistant() { return GetFame() == VICECHIEF; }
	INLINE bool isPartyLeader() { return isInParty() && m_bPartyLeader; }

	INLINE bool isWarrior() { return JobGroupCheck(ClassWarrior); }
	INLINE bool isRogue() { return JobGroupCheck(ClassRogue); }
	INLINE bool isMage() { return JobGroupCheck(ClassMage); }
	INLINE bool isPriest() { return JobGroupCheck(ClassPriest); }

	INLINE bool isBeginner() 
	{
		uint16_t sClass = GetClassType();
		return (sClass <= ClassPriest);
	}

	INLINE bool isBeginnerWarrior() { return GetClassType() == ClassWarrior; }
	INLINE bool isBeginnerRogue()   { return GetClassType() == ClassRogue; }
	INLINE bool isBeginnerMage()    { return GetClassType() == ClassMage; }
	INLINE bool isBeginnerPriest()  { return GetClassType() == ClassPriest; }

	INLINE bool isNovice() 
	{
		uint16_t sClass = GetClassType();
		return (sClass == ClassWarriorNovice || sClass == ClassRogueNovice
			|| sClass == ClassMageNovice || sClass == ClassPriestNovice); 
	}

	INLINE bool isNoviceWarrior() { return GetClassType() == ClassWarriorNovice; }
	INLINE bool isNoviceRogue()   { return GetClassType() == ClassRogueNovice; }
	INLINE bool isNoviceMage()    { return GetClassType() == ClassMageNovice; }
	INLINE bool isNovicePriest()  { return GetClassType() == ClassPriestNovice; }

	INLINE bool isMastered() 
	{
		uint16_t sClass = GetClassType();
		return (sClass == ClassWarriorMaster || sClass == ClassRogueMaster 
			|| sClass == ClassMageMaster || sClass == ClassPriestMaster); 
	}

	INLINE bool isMasteredWarrior() { return GetClassType() == ClassWarriorMaster; }
	INLINE bool isMasteredRogue()   { return GetClassType() == ClassRogueMaster; }
	INLINE bool isMasteredMage()    { return GetClassType() == ClassMageMaster; }
	INLINE bool isMasteredPriest()  { return GetClassType() == ClassPriestMaster; }

	INLINE bool isTrading() { return m_sExchangeUser != -1; }
	INLINE bool isStoreOpen() { return m_bStoreOpen; }
	INLINE bool isMerchanting() { return GetMerchantState() != MERCHANT_STATE_NONE; }
	INLINE bool isSellingMerchant() { return GetMerchantState() == MERCHANT_STATE_SELLING; }
	INLINE bool isBuyingMerchant() { return GetMerchantState() == MERCHANT_STATE_BUYING; }
	INLINE bool isMining() { return m_bMining; }

	INLINE bool isBlockingPrivateChat() { return m_bBlockPrivateChat; }

	INLINE bool isTransformed() { return m_transformationType != TransformationNone; }
	INLINE bool isNPCTransformation() { return m_transformationType == TransformationNPC; }
	INLINE bool isMonsterTransformation() { return m_transformationType == TransformationMonster; }
	INLINE bool isSiegeTransformation() { return m_transformationType == TransformationSiege; }

	INLINE bool isWeaponsDisabled() { return m_bWeaponsDisabled; }

	INLINE bool isInPKZone() {  return GetZoneID() == ZONE_ARDREAM || GetZoneID() == ZONE_RONARK_LAND || GetZoneID() == ZONE_RONARK_LAND_BASE; }

	INLINE int8_t GetMerchantState() { return m_bMerchantState; }

	INLINE uint8_t GetAuthority() { return m_bAuthority; }
	INLINE uint8_t GetFame() { return m_bFame; }

	INLINE uint16_t GetClass() { return m_sClass; }
	INLINE uint8_t GetPremium() { return m_bPremiumType; }
	INLINE bool isLockableScroll(uint8_t buffType) { return (buffType == BUFF_TYPE_HP_MP || buffType == BUFF_TYPE_AC || buffType == BUFF_TYPE_DAMAGE || buffType == BUFF_TYPE_SPEED || buffType == BUFF_TYPE_STATS); }
	INLINE uint8_t GetRace() { return m_bRace; }

	/**
	* @brief	Gets the player's base class type, independent of nation.
	*
	* @return	The class type.
	*/
	INLINE ClassType GetBaseClassType()
	{
		static const ClassType classTypes[] = 
		{
			ClassWarrior, ClassRogue, ClassMage, ClassPriest, 
			ClassWarrior, ClassWarrior,	// job changed / mastered
			ClassRogue, ClassRogue,		// job changed / mastered
			ClassMage, ClassMage,		// job changed / mastered
			ClassPriest, ClassPriest	// job changed / mastered
		}; 

		uint8_t classType = GetClassType();
		ASSERT(classType >= 1 && classType <= 12);
		return classTypes[classType - 1];
	}

	/**
	* @brief	Gets class type, independent of nation.
	*
	* @return	The class type.
	*/
	INLINE uint8_t GetClassType()
	{
		return GetClass() % 100;
	}

	INLINE uint16_t GetPartyID() { return m_sPartyIndex; }

	INLINE int16_t GetClanID() { return m_bKnights; }
	INLINE void SetClanID(int16_t val) { m_bKnights = val; }

	INLINE uint32_t GetCoins() { return m_iGold; }
	INLINE uint32_t GetInnCoins() { return m_iBank; }
	INLINE uint32_t GetLoyalty() { return m_iLoyalty; }
	INLINE uint32_t GetMonthlyLoyalty() { return m_iLoyaltyMonthly; }
	INLINE uint32_t GetManner() { return m_iMannerPoint; }

	virtual int32_t GetHealth() { return m_sHp; }
	virtual int32_t GetMaxHealth() { return m_iMaxHp; }
	virtual int32_t GetMana() { return m_sMp; }
	virtual int32_t GetMaxMana() { return m_iMaxMp; }

	// Shortcuts for lazy people
	INLINE bool hasCoins(uint32_t amount) { return (GetCoins() >= amount); }
	INLINE bool hasInnCoins(uint32_t amount) { return (GetInnCoins() >= amount); }
	INLINE bool hasLoyalty(uint32_t amount) { return (GetLoyalty() >= amount); }
	INLINE bool hasMonthlyLoyalty(uint32_t amount) { return (GetMonthlyLoyalty() >= amount); }
	INLINE bool hasManner(uint32_t amount) { return (GetManner() >= amount); }

	INLINE uint8_t GetAngerGauge() { return m_byAngerGauge; }
	INLINE bool hasFullAngerGauge() { return GetAngerGauge() >= MAX_ANGER_GAUGE; }

	INLINE bool hasRival() { return GetRivalID() >= 0; }
	INLINE bool hasRivalryExpired() { return UNIXTIME >= m_tRivalExpiryTime; }
	INLINE int16_t GetRivalID() { return m_sRivalID; }

	INLINE GameState GetState() { return m_state; }

	INLINE uint16_t GetActiveQuestID() { return m_sEventDataIndex; }

	uint8_t GetClanGrade();
	uint8_t GetClanRank();
	uint32_t GetClanPoint();
	void SendClanPointChange(int32_t nChangeAmount = 0);

	uint8_t GetBeefRoastVictory();
	uint8_t GetRankReward(bool isMonthly);
	uint8_t GetWarVictory();

	uint8_t CheckMiddleStatueCapture();
	void MoveMiddleStatue();	

	uint8_t GetPVPMonumentNation();

	uint8_t GetMonsterChallengeTime();
	uint8_t GetMonsterChallengeUserCount();

	INLINE uint8_t GetStat(StatType type)
	{
		if (type >= STAT_COUNT)
			return 0;

		return m_bStats[type];
	}

	INLINE void SetStat(StatType type, uint8_t val)
	{
		ASSERT(type < STAT_COUNT);
		m_bStats[type] = val;
	}

	INLINE int32_t GetStatTotal() // NOTE: Shares name with another, but lack-of args should be self-explanatory
	{
		int32_t total = 0; // NOTE: this loop should be unrolled by the compiler
		foreach_array (i, m_bStats)
			total += m_bStats[i];
		return total;
	}

	INLINE int16_t GetStatItemBonus(StatType type)
	{
		ASSERT(type < STAT_COUNT);
		return m_sStatItemBonuses[type];
	}

	INLINE int16_t GetStatWithItemBonus(StatType type)
	{
		return GetStat(type) + GetStatItemBonus(type);
	}

	INLINE int32_t GetStatItemBonusTotal()
	{
		int32_t total = 0; // NOTE: this loop should be unrolled by the compiler
		foreach_array (i, m_sStatItemBonuses)
			total += m_sStatItemBonuses[i];
		return total;
	}

	INLINE uint16_t GetStatBonusTotal(StatType type)
	{
		return GetStatBuff(type) + GetStatItemBonus(type);
	}

	INLINE uint8_t GetStatBuff(StatType type)
	{
		ASSERT(type < STAT_COUNT);
		return m_bStatBuffs[type];
	}

	INLINE void SetStatBuff(StatType type, uint8_t val)
	{
		ASSERT(type < STAT_COUNT);
		m_bStatBuffs[type] = val;
	}

	INLINE uint32_t GetStatBuffTotal()
	{
		uint32_t total = 0; // NOTE: this loop should be unrolled by the compiler
		foreach_array (i, m_bStatBuffs)
			total += m_bStatBuffs[i];
		return total;
	}

	INLINE uint16_t getStatTotal(StatType type)
	{
		return GetStat(type) + GetStatItemBonus(type) + GetStatBuff(type);
	}

	INLINE uint16_t GetTotalSkillPoints()
	{
		return m_bstrSkill[SkillPointFree] + m_bstrSkill[SkillPointCat1] 
		+ m_bstrSkill[SkillPointCat2] + m_bstrSkill[SkillPointCat3] 
		+ m_bstrSkill[SkillPointMaster];
	}

	INLINE uint8_t GetSkillPoints(SkillPointCategory category)
	{
		if (category < SkillPointFree || category > SkillPointMaster)
			return 0;

		return m_bstrSkill[category];
	}

	INLINE _ITEM_DATA * GetItem(uint8_t pos) 
	{
		ASSERT(pos < INVENTORY_TOTAL);
		return &m_sItemArray[pos]; 
	}

	INLINE _ITEM_TABLE * GetItemPrototype(uint8_t pos) 
	{
		_ITEM_DATA * pItem;
		ASSERT(pos < INVENTORY_TOTAL);
		return GetItemPrototype(pos, pItem);
	}

	_ITEM_TABLE * GetItemPrototype(uint8_t pos, _ITEM_DATA *& pItem);

	INLINE KOMap * GetMap() { return m_pMap; }

	CUser(uint16_t socketID, SocketMgr *mgr); 

	virtual void OnConnect();
	virtual void OnDisconnect();
	virtual bool HandlePacket(Packet & pkt);

	void Update();

	virtual void AddToRegion(int16_t new_region_x, int16_t new_region_z);

	void SetRival(CUser * pRival);
	void RemoveRival();
	void SendLoyaltyChange(int32_t nChangeAmount = 0, bool bIsKillReward = false, bool bIsBonusTime = false, bool bIsAddLoyaltyMonthly = true);

	void NativeZoneReturn();
	void KickOutZoneUser(bool home = false, uint8_t nZoneID = 21);
	void TrapProcess();
	bool JobGroupCheck(int16_t jobgroupid);
	void SendSay(int32_t nTextID[10]);
	void SelectMsg(uint8_t bFlag, int32_t nQuestID, int32_t menuHeaderText, 
		int32_t menuButtonText[MAX_MESSAGE_EVENT], int32_t menuButtonEvents[MAX_MESSAGE_EVENT]);

	// NOTE(srmeier): testing this debug string functionality
	void SendDebugString(const char* pString);
	int NumEmptySlots(void);

	bool CheckClass(int16_t class1, int16_t class2 = -1, int16_t class3 = -1, int16_t class4 = -1, int16_t class5 = -1, int16_t class6 = -1);
	bool GiveItem(uint32_t nItemID, uint16_t sCount = 1, bool send_packet = true, uint32_t Time = 0);
	bool RobItem(uint32_t nItemID, uint32_t sCount = 1);
	bool RobItem(uint8_t bPos, _ITEM_TABLE * pTable, uint32_t sCount = 1);
	bool RobAllItemParty(uint32_t nItemID, uint16_t sCount = 1);
	bool CheckExistItem(int itemid, int16_t count = 1);
	bool CheckExistItemAnd(int32_t nItemID1, int32_t sCount1, int32_t nItemID2, int32_t sCount2,
		int32_t nItemID3, int32_t sCount3, int32_t nItemID4, int32_t sCount4, int32_t nItemID5, int32_t sCount5,
		int32_t nItemID6, int32_t sCount6, int32_t nItemID7, int32_t sCount7, int32_t nItemID8, int32_t sCount8,
		int32_t nItemID9, int32_t sCount9, int32_t nItemID10, int32_t sCount10, int32_t nItemID11, int32_t sCount11);
	uint32_t GetItemCount(uint32_t nItemID);
	bool CheckWeight(uint32_t nItemID, uint32_t sCount);
	bool CheckWeight(_ITEM_TABLE * pTable, uint32_t nItemID, uint32_t sCount);
	bool CheckSkillPoint(uint8_t skillnum, uint8_t min, uint8_t max);
	bool GoldLose(uint32_t gold, bool bSendPacket = true);
	void GoldGain(uint32_t gold, bool bSendPacket = true, bool bApplyBonus = false);
	void SendItemWeight();
	void UpdateVisibility(InvisibilityType bNewType);
	void ResetGMVisibility();
	void BlinkStart();
	void BlinkTimeCheck();
	void GoldChange(int16_t tid, int gold);
	CUser * GetItemRoutingUser(uint32_t nItemID, uint32_t sCount);
	bool GetStartPosition(int16_t & x, int16_t & y, uint8_t bZone = 0);
	bool GetStartPositionRandom(int16_t & x, int16_t & z, uint8_t bZone = 0);
	int FindSlotForItem(uint32_t nItemID, uint16_t sCount = 1);
	int GetEmptySlot();
	void SendAllKnightsID();
	void SendStackChange(uint32_t nItemID, uint32_t nCount /* needs to be 4 bytes, not a bug */, uint16_t sDurability, uint8_t bPos, bool bNewItem = false, uint32_t Time = 0);

	void Type4Duration();
	void HPTimeChange();
	void HPTimeChangeType3();

	int16_t GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	void OnAttack(Unit * pTarget, AttackType attackType);
	void OnDefend(Unit * pAttacker, AttackType attackType);
	bool TriggerProcItem(uint8_t bSlot, Unit * pTarget, ItemTriggerType triggerType);

	void SendDurability(uint8_t slot, uint16_t durability);
	void SendItemMove(uint8_t subcommand);
	void ItemWoreOut( int type, int damage );
	void Dead();
	void GetUserInfoForAI(Packet & result);
	bool ItemEquipAvailable( _ITEM_TABLE* pTable );
	virtual void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true);
	virtual void MSpChange(int amount);
	void SendPartyHPUpdate();
	void ShowEffect(uint32_t nSkillID);
	void ShowNpcEffect(uint32_t nEffectID, bool bSendToRegion = false);
	void SendAnvilRequest(uint16_t sNpcID, uint8_t bType = ITEM_UPGRADE_REQ);
	void RecastSavedMagic(uint8_t buffType = 0);
	void RecastLockableScrolls(uint8_t buffType);

	// packet handlers start here
	void VersionCheck(Packet & pkt);
	void LoginProcess(Packet & pkt);
	bool WordGuardSystem(std::string Word, uint8_t WordStr);

	void SelNationToAgent(Packet & pkt);
	void AllCharInfoToAgent();
	void ChangeHair(Packet & pkt);
	void NewCharToAgent(Packet & pkt);
	void DelCharToAgent(Packet & pkt);
	void SelCharToAgent(Packet & pkt);
	void SelectCharacter(uint8_t bResult, uint8_t bInit); // from the database
	void SetLogInInfoToDB(uint8_t bInit);
	void RecvLoginInfo(Packet & pkt); // from the database

	void SpeedHackTime(Packet & pkt);
	void TempleProcess(Packet & pkt );
	void TempleOperations(uint8_t bType = 0);

	void MonsterStoneProcess(); 

	void GameStart(Packet & pkt);
	void RentalSystem(Packet & pkt);
	void SkillDataProcess(Packet & pkt);
	void SkillDataSave(Packet & pkt);
	void SkillDataLoad();
	void MoveProcess(Packet & pkt);
	void Rotate(Packet & pkt);
	void Attack(Packet & pkt);

	static void InitChatCommands();
	static void CleanupChatCommands();

	void Chat(Packet & pkt);
	void ChatTargetSelect(Packet & pkt);
	void SendDeathNotice(Unit * pKiller, DeathNoticeType noticeType); 

	bool ProcessChatCommand(std::string & message);

	uint8_t GetUserDailyOp(uint8_t type = 0);
	void SetUserDailyOp(uint8_t type = 0, bool isInsert = false);

	uint32_t GetEventTrigger();

	void RemoveStealth();

	void GivePremium(uint8_t bPremiumType, uint16_t sPremiumTime);
	void RobChaosSkillItems();

	// Nation Transfer, Gender Change and Job Change (in game)
	uint8_t NationChange();
	uint8_t GetNewRace();
	bool GenderChange(uint8_t nRace = 0);
	uint8_t JobChange(uint8_t NewJob = 0);

	COMMAND_HANDLER(HandleTestCommand);
	COMMAND_HANDLER(HandleGiveItemCommand);
	COMMAND_HANDLER(HandleZoneChangeCommand);
	COMMAND_HANDLER(HandleMonsterSummonCommand);
	COMMAND_HANDLER(HandleNPCSummonCommand);
	COMMAND_HANDLER(HandleMonKillCommand);
	COMMAND_HANDLER(HandleWar1OpenCommand);
	COMMAND_HANDLER(HandleWar2OpenCommand);
	COMMAND_HANDLER(HandleWar3OpenCommand);
	COMMAND_HANDLER(HandleWar4OpenCommand);
	COMMAND_HANDLER(HandleWar5OpenCommand);
	COMMAND_HANDLER(HandleWar6OpenCommand);
	COMMAND_HANDLER(HandleCaptainCommand);
	COMMAND_HANDLER(HandleSnowWarOpenCommand);
	COMMAND_HANDLER(HandleSiegeWarOpenCommand);
	COMMAND_HANDLER(HandleWarCloseCommand);
	COMMAND_HANDLER(HandleLoyaltyChangeCommand);
	COMMAND_HANDLER(HandleExpChangeCommand);
	COMMAND_HANDLER(HandleGoldChangeCommand);
	COMMAND_HANDLER(HandleLoyaltyAddCommand); /* for the server NP event */
	COMMAND_HANDLER(HandleExpAddCommand); /* for the server XP event */
	COMMAND_HANDLER(HandleMoneyAddCommand); /* for the server coin event */
	COMMAND_HANDLER(HandlePermitConnectCommand);
	COMMAND_HANDLER(HandleTeleportAllCommand);
	COMMAND_HANDLER(HandleKnightsSummonCommand);
	COMMAND_HANDLER(HandleWarResultCommand);
	COMMAND_HANDLER(HandleResetPlayerRankingCommand);

	void Regene(uint8_t regene_type, uint32_t magicid = 0);
	void RequestUserIn(Packet & pkt);
	void RequestNpcIn(Packet & pkt);
	void RecvWarp(Packet & pkt);
	void Warp(uint16_t sPosX, uint16_t sPosZ);
	void ItemMove(Packet & pkt);
	void NpcEvent(Packet & pkt);

	void ItemTrade(Packet & pkt);

	void BundleOpenReq(Packet & pkt);
	void ItemGet(Packet & pkt);
	CUser * GetLootUser(_LOOT_BUNDLE * pBundle, _LOOT_ITEM * pItem);

	void RecvZoneChange(Packet & pkt);
	void PointChange(Packet & pkt);

	void StateChange(Packet & pkt);
	virtual void StateChangeServerDirect(uint8_t bType, uint32_t nBuff);

	void PartyProcess(Packet & pkt);
	void PartyDelete();
	void PartyPromote(uint16_t sMemberID);
	void PartyRemove( int memberid );
	void PartyInsert();
	void PartyCancel();
	void PartyRequest( int memberid, bool bCreate );

	// Trade system
	void ExchangeProcess(Packet & pkt);
	void ExchangeReq(Packet & pkt);
	void ExchangeAgree(Packet & pkt);
	void ExchangeAdd(Packet & pkt);
	void ExchangeDecide();
	void ExchangeCancel(bool bIsOnDeath = false);
	void ExchangeFinish();

	bool CheckExchange();
	bool ExecuteExchange();

	// Merchant system (both types)
	void MerchantProcess(Packet & pkt);
	void GiveMerchantItems();

	// regular merchants
	void MerchantOpen();
	void MerchantClose();
	void MerchantItemAdd(Packet & pkt);
	void MerchantItemCancel(Packet & pkt);
	void MerchantItemList(Packet & pkt);
	void MerchantItemBuy(Packet & pkt);
	void MerchantInsert(Packet & pkt);
	void CancelMerchant();

	// buying merchants
	void BuyingMerchantOpen(Packet & pkt);
	void BuyingMerchantClose();
	void BuyingMerchantInsert(Packet & pkt);
	void BuyingMerchantInsertRegion();
	void BuyingMerchantList(Packet & pkt);
	void BuyingMerchantBuy(Packet & pkt);

	void RemoveFromMerchantLookers();

	void SkillPointChange(Packet & pkt);

	void ObjectEvent(Packet & pkt);
	bool BindObjectEvent(_OBJECT_EVENT *pEvent);
	bool GateLeverObjectEvent(_OBJECT_EVENT *pEvent, int nid);
	bool FlagObjectEvent(_OBJECT_EVENT *pEvent, int nid);
	bool WarpListObjectEvent(_OBJECT_EVENT *pEvent);

	void UpdateGameWeather(Packet & pkt);

	void ClassChange(Packet & pkt, bool bFromClient = true);
	void ClassChangeReq();
	void SendStatSkillDistribute();
	void AllPointChange(bool bIsFree = false);
	void AllSkillPointChange(bool bIsFree = false);

	void CountConcurrentUser();
	void UserDataSaveToAgent();

	void ItemRepair(Packet & pkt);
	void ItemRemove(Packet & pkt);
	void OperatorCommand(Packet & pkt);
	void WarehouseProcess(Packet & pkt);
	void Home();

	void FriendProcess(Packet & pkt);
	void FriendRequest();
	void FriendModify(Packet & pkt, uint8_t opcode);
	void RecvFriendModify(Packet & pkt, uint8_t opcode);
	void FriendReport(Packet & pkt);
	uint8_t GetFriendStatus(std::string & charName, int16_t & sid);

	void SelectWarpList(Packet & pkt);
	bool GetWarpList( int warp_group );

	void ServerChangeOk(Packet & pkt);

	void PartyBBS(Packet & pkt);
	void PartyBBSRegister(Packet & pkt);
	void PartyBBSDelete(Packet & pkt);
	void PartyBBSNeeded(Packet & pkt, uint8_t type);
	void PartyBBSWanted(Packet & pkt);
	uint8_t GetPartyMemberAmount(_PARTY_GROUP *pParty = nullptr);

	void SendPartyBBSNeeded(uint16_t page_index, uint8_t bType);
	

	void ClientEvent(uint16_t sNpcID);
	void KissUser();

	void RecvSelectMsg(Packet & pkt);
	bool AttemptSelectMsg(uint8_t bMenuID, int8_t bySelectedReward);

	// from the client
	void ItemUpgradeProcess(Packet & pkt);
	void ItemUpgrade(Packet & pkt, uint8_t nUpgradeType = ITEM_UPGRADE);
	void ItemUpgradeNotice(_ITEM_TABLE * pItem, uint8_t UpgradeResult);
	void ItemUpgradeAccessories(Packet & pkt);
	void BifrostPieceProcess(Packet & pkt); // originally named BeefRoastPieceProcess() -- that's not happening.
	void SpecialItemExchange(Packet & pkt);
	void ItemUpgradeRebirth(Packet & pkt);
	void ItemSealProcess(Packet & pkt);
	void SealItem(uint8_t bSealType, uint8_t bSrcPos);
	void CharacterSealProcess(Packet & pkt);

	void ShoppingMall(Packet & pkt);
	void HandleStoreOpen(Packet & pkt);
	void HandleStoreClose();
	void LetterSystem(Packet & pkt);

	void ReqLetterSystem(Packet & pkt);
	void ReqLetterUnread();
	void ReqLetterList(bool bNewLettersOnly = true);
	void ReqLetterRead(Packet & pkt);
	void ReqLetterSend(Packet & pkt);
	void ReqLetterGetItem(Packet & pkt);
	void ReqLetterDelete(Packet & pkt);

	void HandleNameChange(Packet & pkt);
	void HandlePlayerNameChange(Packet & pkt);
	void SendNameChange(NameChangeOpcode opcode = NameChangeShowDialog);

	void HandleHelmet(Packet & pkt);
	void HandleCapeChange(Packet & pkt);

	void HandleChallenge(Packet & pkt);
	void HandleChallengeRequestPVP(Packet & pkt);
	void HandleChallengeRequestCVC(Packet & pkt);
	void HandleChallengeAcceptPVP(Packet & pkt);
	void HandleChallengeAcceptCVC(Packet & pkt);
	void HandleChallengeCancelled(uint8_t opcode);
	void HandleChallengeRejected(uint8_t opcode);

	void HandlePlayerRankings(Packet & pkt);
	uint16_t GetPlayerRank(uint8_t nRankType);

	void HandleMiningSystem(Packet & pkt);
	void HandleMiningStart(Packet & pkt);
	void HandleMiningAttempt(Packet & pkt);
	void HandleMiningStop(Packet & pkt);

	void HandleSoccer(Packet & pkt);

	void SendNotice();
	void AppendNoticeEntry(Packet & pkt, uint8_t & elementCount, const char * message, const char * title);
	void AppendExtraNoticeData(Packet & pkt, uint8_t & elementCount);
	void UserLookChange( int pos, int itemid, int durability );
	void SpeedHackUser();
	void LoyaltyChange(int16_t tid, uint16_t bonusNP = 0);
	void LoyaltyDivide(int16_t tid, uint16_t bonusNP = 0);
	int16_t GetLoyaltyDivideSource(uint8_t totalmember = 0);
	int16_t GetLoyaltyDivideTarget();
	void GrantChickenManner();
	void SendMannerChange(int32_t iMannerPoints);

	bool CanLevelQualify(uint8_t sLevel);
	bool CanChangeZone(C3DMap * pTargetMap, WarpListResponse & errorReason);
	void CheckWaiting(uint8_t sNewZone, uint16_t Time);
	void ZoneChange(uint16_t sNewZone, float x, float z);
	void ZoneChangeParty(uint16_t sNewZone, float x, float z);
	void ZoneChangeClan(uint16_t sNewZone, float x, float z);
	void PlayerRankingProcess(uint16_t ZoneID, bool RemoveInZone = false);
	void AddPlayerRank(uint16_t ZoneID);
	void RemovePlayerRank();
	void UpdatePlayerRank();

	bool isEventUser();

	void SendTargetHP( uint8_t echo, int tid, int damage = 0 );
	bool IsValidSlotPos( _ITEM_TABLE* pTable, int destpos );
	void SetUserAbility(bool bSendPacket = true);
	void LevelChange(uint8_t level, bool bLevelUp = true);
	void SetSlotItemValue();
	void ApplySetItemBonuses(_SET_ITEM * pItem);
	void SendTime();
	void SendWeather();
	void SendPremiumInfo();
	void SetZoneAbilityChange(uint16_t sNewZone);
	void SetMaxMp();
	void SetMaxHp(int iFlag = 0);
	void RecvUserExp(Packet & pkt);
	void ExpChange(int64_t iExp, bool bIsBonusReward = false);
	void LogOut();
	void SendMyInfo();
	void SendServerChange(std::string & ip, uint8_t bInit);
	void Send2AI_UserUpdateInfo(bool initialInfo = false);
	uint16_t GetPremiumProperty(PremiumPropertyOpCodes type);
	void BifrostProcess(CUser * pUser);
	void CastleSiegeWarProcess(CUser * pUser);
	void SiegeWarFareNpc(Packet & pkt);
	void LogosShout(Packet & pkt);

	virtual void GetInOut(Packet & result, uint8_t bType);
	void UserInOut(uint8_t bType);

	void GetUserInfo(Packet & pkt);
	void SendUserStatusUpdate(UserStatus type, UserStatusBehaviour status);
	virtual void Initialize();

	void ChangeFame(uint8_t bFame);
	void SendServerIndex();

	void SendToRegion(Packet *pkt, CUser *pExceptUser = nullptr, uint16_t nEventRoom = 0);
	void SendToZone(Packet *pkt, CUser *pExceptUser = nullptr, uint16_t nEventRoom = 0, float fRange = 0.0f);

	virtual void OnDeath(Unit *pKiller);
	void UpdateAngerGauge(uint8_t byAngerGauge);
	void InitializeStealth();

	// Exchange system
	bool CheckExchange(int nExchangeID);
	bool RunExchange(int nExchangeID, uint32_t count = 0 /* No random flag */);
	bool RunSelectExchange(int nExchangeID);
	uint32_t GetMaxExchange(int nExchangeID);	

	// Clan system
	void SendClanUserStatusUpdate(bool bToRegion = true);

	void SendPartyStatusUpdate(uint8_t bStatus, uint8_t bResult = 0);

	bool CanUseItem(uint32_t nItemID, uint32_t sCount = 1);

	void CheckSavedMagic();
	virtual void InsertSavedMagic(uint32_t nSkillID, uint16_t sDuration);
	virtual void RemoveSavedMagic(uint32_t nSkillID);
	virtual bool HasSavedMagic(uint32_t nSkillID);
	virtual int16_t GetSavedMagicDuration(uint32_t nSkillID);

	void SaveEvent(uint16_t sQuestID, uint8_t bQuestState);
	void DeleteEvent(uint16_t sQuestID);
	bool CheckExistEvent(uint16_t sQuestID, uint8_t bQuestState);	

	void QuestV2MonsterCountAdd(uint16_t sNpcID);
	uint8_t QuestV2CheckMonsterCount(uint16_t sQuestID);
	void QuestV2MonsterDataDeleteAll();

	// Sends the quest completion statuses
	void QuestDataRequest();

	// Handles new quest packets
	void QuestV2PacketProcess(Packet & pkt);
	void QuestV2MonsterDataRequest();
	void QuestV2ExecuteHelper(_QUEST_HELPER * pQuestHelper);
	void QuestV2CheckFulfill(_QUEST_HELPER * pQuestHelper);
	bool QuestV2RunEvent(_QUEST_HELPER * pQuestHelper, uint32_t nEventID, int8_t bSelectedReward = 0);

	void QuestV2SaveEvent(uint16_t sEventDataIndex, int8_t bEventStatus);//uint16_t sQuestID);
	void QuestV2SendNpcMsg(uint32_t nQuestID, uint16_t sNpcID);
	void QuestV2ShowGiveItem(uint32_t nUnk1, uint32_t sUnk1, 
		uint32_t nUnk2, uint32_t sUnk2,
		uint32_t nUnk3, uint32_t sUnk3,
		uint32_t nUnk4, uint32_t sUnk4);
	uint16_t QuestV2SearchEligibleQuest(uint16_t sEventDataIndex);//, uint16_t sNpcID);
	void QuestV2ShowMap(uint32_t nQuestHelperID);
	uint8_t CheckMonsterCount(uint8_t bGroup);

	bool PromoteUserNovice();
	bool PromoteUser();
	void PromoteClan(ClanTypeFlag byFlag);

	// Attack/zone checks
	bool isHostileTo(Unit * pTarget);
	bool isInArena();
	bool isInPVPZone();
	bool isInSafetyArea();

	void ResetWindows();

	void CloseProcess();
	virtual ~CUser() {}

	/* Database requests */
	void ReqAccountLogIn(Packet & pkt);
	void ReqSelectNation(Packet & pkt);
	void ReqAllCharInfo(Packet & pkt);
	void ReqChangeHair(Packet & pkt);
	void ReqCreateNewChar(Packet & pkt);
	void ReqDeleteChar(Packet & pkt);
	void ReqSelectCharacter(Packet & pkt);
	void ReqSaveCharacter();
	void ReqUserLogOut();
	void ReqRegisterClanSymbol(Packet & pkt);
	void ReqSetLogInInfo(Packet & pkt);
	void ReqUserKickOut(Packet & pkt);
	void BattleEventResult(Packet & pkt);
	void ReqShoppingMall(Packet & pkt);
	void ReqLoadWebItemMall();
	void ReqSkillDataProcess(Packet & pkt);
	void ReqSkillDataSave(Packet & pkt);
	void ReqSkillDataLoad(Packet & pkt);
	void ReqFriendProcess(Packet & pkt);
	void ReqRequestFriendList(Packet & pkt);
	void ReqAddFriend(Packet & pkt);
	void ReqRemoveFriend(Packet & pkt);
	void ReqChangeName(Packet & pkt);
	void ReqChangeCape(Packet & pkt);
	void ReqSealItem(Packet & pkt);
	void InsertTaxUpEvent(uint8_t Nation, uint32_t TerritoryTax);

	//private:
	static ChatCommandTable s_commandTable;
	GameState m_state;

	// quest ID | quest state (need to replace with enum)
	typedef std::map<uint16_t, uint8_t> QuestMap;
	QuestMap m_questMap;

	uint8_t m_bKillCounts[QUEST_MOB_GROUPS];
	uint16_t m_sEventDataIndex;

	UserSavedMagicMap m_savedMagicMap;
	std::recursive_mutex m_savedMagicLock;

	_KNIGHTS_USER * m_pKnightsUser;

public:
	DECLARE_LUA_CLASS(CUser);

	// Standard getters
	DECLARE_LUA_GETTER(GetName)
	DECLARE_LUA_GETTER(GetAccountName)
	DECLARE_LUA_GETTER(GetZoneID)
	DECLARE_LUA_GETTER(GetX)
	DECLARE_LUA_GETTER(GetY)
	DECLARE_LUA_GETTER(GetZ)
	DECLARE_LUA_GETTER(GetNation)
	DECLARE_LUA_GETTER(GetLevel)
	DECLARE_LUA_GETTER(GetClass)
	DECLARE_LUA_GETTER(GetCoins)
	DECLARE_LUA_GETTER(GetInnCoins)
	DECLARE_LUA_GETTER(GetLoyalty)
	DECLARE_LUA_GETTER(GetMonthlyLoyalty)
	DECLARE_LUA_GETTER(GetManner)
	DECLARE_LUA_GETTER(GetActiveQuestID)
	DECLARE_LUA_GETTER(GetClanGrade)
	DECLARE_LUA_GETTER(GetClanPoint)
	DECLARE_LUA_GETTER(GetClanRank)
	DECLARE_LUA_GETTER(isWarrior)
	DECLARE_LUA_GETTER(isRogue)
	DECLARE_LUA_GETTER(isMage)
	DECLARE_LUA_GETTER(isPriest)
	DECLARE_LUA_GETTER(isBeginner)
	DECLARE_LUA_GETTER(isBeginnerWarrior)
	DECLARE_LUA_GETTER(isBeginnerRogue)
	DECLARE_LUA_GETTER(isBeginnerMage)
	DECLARE_LUA_GETTER(isBeginnerPriest)
	DECLARE_LUA_GETTER(isNovice)
	DECLARE_LUA_GETTER(isNoviceWarrior)
	DECLARE_LUA_GETTER(isNoviceRogue)
	DECLARE_LUA_GETTER(isNoviceMage)
	DECLARE_LUA_GETTER(isNovicePriest)
	DECLARE_LUA_GETTER(isMastered)
	DECLARE_LUA_GETTER(isMasteredWarrior)
	DECLARE_LUA_GETTER(isMasteredRogue)
	DECLARE_LUA_GETTER(isMasteredMage)
	DECLARE_LUA_GETTER(isMasteredPriest)
	DECLARE_LUA_GETTER(isInClan)
	DECLARE_LUA_GETTER(isClanLeader)
	DECLARE_LUA_GETTER(isInParty)
	DECLARE_LUA_GETTER(isPartyLeader)
	DECLARE_LUA_GETTER(isKing)
	DECLARE_LUA_GETTER(GetBeefRoastVictory)
	DECLARE_LUA_GETTER(GetPartyMemberAmount)
	DECLARE_LUA_GETTER(GetPremium)
	DECLARE_LUA_GETTER(GetWarVictory)
	DECLARE_LUA_GETTER(GetMonsterChallengeTime)
	DECLARE_LUA_GETTER(GetMonsterChallengeUserCount)
	DECLARE_LUA_GETTER(GetRace)

	// Shortcuts for lazy people
	DECLARE_LUA_FUNCTION(hasCoins)  {
		LUA_RETURN(LUA_GET_INSTANCE()->hasCoins(LUA_ARG(uint32_t, 2))); 
	}

	DECLARE_LUA_FUNCTION(hasInnCoins) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasInnCoins(LUA_ARG(uint32_t, 2))); 
	}

	DECLARE_LUA_FUNCTION(hasLoyalty) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasLoyalty(LUA_ARG(uint32_t, 2))); 
	}

	DECLARE_LUA_FUNCTION(hasMonthlyLoyalty) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasMonthlyLoyalty(LUA_ARG(uint32_t, 2)));
	}

	DECLARE_LUA_FUNCTION(hasManner) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasManner(LUA_ARG(uint32_t, 2))); 
	}

	// The useful method wrappers

	// NOTE(srmeier): testing this debug string functionality
	DECLARE_LUA_FUNCTION(SendDebugString) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendDebugString(
			LUA_ARG(const char*, 2)));
	}

	// NOTE(srmeier): for EVT compatibility. Also don't forget to make the method in lua_bindings.cpp!!
	DECLARE_LUA_FUNCTION(HowMuchItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetItemCount(
			LUA_ARG(uint32_t, 2)));
	}

	// NOTE(srmeier): the global CheckClass doesn't seem to match the Evt procedure I need
	DECLARE_LUA_FUNCTION(CheckClass) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckClass(
			LUA_ARG(uint16_t, 2),
			LUA_ARG_OPTIONAL(uint16_t, -1, 3),
			LUA_ARG_OPTIONAL(uint16_t, -1, 4),
			LUA_ARG_OPTIONAL(uint16_t, -1, 5),
			LUA_ARG_OPTIONAL(uint16_t, -1, 6),
			LUA_ARG_OPTIONAL(uint16_t, -1, 7)
		));
	}

	// NOTE(srmeier): returns the number of empty slots in the inventory
	DECLARE_LUA_FUNCTION(EmptySlotCount) {
		LUA_RETURN(LUA_GET_INSTANCE()->NumEmptySlots());
	}

	// NOTE(srmeier): adding a function to change the player's state (EVT flag -> STATE_CHANGE)
	DECLARE_LUA_FUNCTION(StateChange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->StateChangeServerDirect(
			LUA_ARG(uint8_t, 2),
			LUA_ARG(uint32_t, 3)
		));
	}

	//-------------------------------------------------------------------------

	DECLARE_LUA_FUNCTION(GiveItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->GiveItem(
			LUA_ARG(uint32_t, 2), 
			LUA_ARG_OPTIONAL(uint16_t, 1, 3), 
			true,
			LUA_ARG_OPTIONAL(uint32_t, 0, 4)));
	}

	DECLARE_LUA_FUNCTION(RobItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->RobItem(
			LUA_ARG(uint32_t, 2), 
			LUA_ARG_OPTIONAL(uint16_t, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(RobAllItemParty) {
		LUA_RETURN(LUA_GET_INSTANCE()->RobAllItemParty(
			LUA_ARG(uint32_t, 2), 
			LUA_ARG_OPTIONAL(uint16_t, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(CheckExistItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckExistItem(
			LUA_ARG(uint32_t, 2), 
			LUA_ARG_OPTIONAL(uint16_t, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(CheckExistEvent) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckExistEvent(
			LUA_ARG(uint16_t, 2),
			LUA_ARG(uint8_t, 3)));
	}

	DECLARE_LUA_FUNCTION(GoldGain) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->GoldGain(LUA_ARG(int32_t, 2)));	
	}

	DECLARE_LUA_FUNCTION(GoldLose) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->GoldLose(LUA_ARG(uint32_t, 2)));	
	}

	DECLARE_LUA_FUNCTION(ExpChange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ExpChange(LUA_ARG(int32_t, 2)));	
	}

	DECLARE_LUA_FUNCTION(SaveEvent) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->QuestV2SaveEvent(
			//LUA_ARG(uint16_t, 2)));  // quest ID
			LUA_ARG(uint16_t, 2), // sEventDataIndex
			LUA_ARG(char, 3) // bEventStatus
		));  // quest ID
	}

	DECLARE_LUA_FUNCTION(SearchQuest) {
		CUser * pUser = LUA_GET_INSTANCE();
		LUA_RETURN(pUser->QuestV2SearchEligibleQuest(
			LUA_ARG(uint16_t, 2)
			//LUA_ARG_OPTIONAL(uint16_t, pUser->m_sEventSid, 3)
		)); // NPC ID
	}

	DECLARE_LUA_FUNCTION(ShowMap) {
		CUser * pUser = LUA_GET_INSTANCE();
		LUA_NO_RETURN(pUser->QuestV2ShowMap(LUA_ARG_OPTIONAL(uint32_t, pUser->m_nQuestHelperID, 2))); // quest helper ID
	}

	DECLARE_LUA_FUNCTION(CountMonsterQuestSub) {
		LUA_RETURN(LUA_GET_INSTANCE()->QuestV2CheckMonsterCount((LUA_ARG(uint16_t, 2))));
	}

	DECLARE_LUA_FUNCTION(CountMonsterQuestMain) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->QuestV2MonsterCountAdd((LUA_ARG(uint16_t, 2))));
	} 

	DECLARE_LUA_FUNCTION(NpcSay) {
		CUser * pUser = LUA_GET_INSTANCE();
		uint32_t arg = 2; // start from after the user instance.
		int32_t nTextID[10]; 

		foreach_array(i, nTextID)
			nTextID[i] = LUA_ARG_OPTIONAL(int32_t, -1, arg++);

		LUA_NO_RETURN(pUser->SendSay(nTextID));
	}

	// This is probably going to be cleaned up, as the methodology behind these menus is kind of awful.
	// For now, we'll just copy existing behaviour: that is, pass along a set of text IDs & button IDs.
	DECLARE_LUA_FUNCTION(SelectMsg) {
		CUser * pUser = LUA_GET_INSTANCE();
		uint32_t arg = 2; // start from after the user instance.
		int32_t menuButtonText[MAX_MESSAGE_EVENT], 
			menuButtonEvents[MAX_MESSAGE_EVENT];
		uint8_t bFlag = 0;//LUA_ARG(uint8_t, arg++);
		int32_t nQuestID = 0;//LUA_ARG_OPTIONAL(int32_t, -1, arg++);
		int32_t menuHeaderText = LUA_ARG(int32_t, arg++);

		foreach_array(i, menuButtonText)
		{
			menuButtonText[i] = LUA_ARG_OPTIONAL(int32_t, -1, arg++);
			menuButtonEvents[i] = LUA_ARG_OPTIONAL(int32_t, -1, arg++);
		}

		LUA_NO_RETURN(pUser->SelectMsg(bFlag, nQuestID, menuHeaderText, menuButtonText, menuButtonEvents));
	}

	DECLARE_LUA_FUNCTION(NpcMsg) {
		CUser * pUser = LUA_GET_INSTANCE();
		LUA_NO_RETURN(pUser->QuestV2SendNpcMsg(
			LUA_ARG(uint32_t, 2),
			LUA_ARG_OPTIONAL(uint16_t, pUser->m_sEventSid, 3)));
	}

	DECLARE_LUA_FUNCTION(CheckWeight) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckWeight(
			LUA_ARG(uint32_t, 2),		// item ID
			LUA_ARG_OPTIONAL(uint16_t, 1, 3)));	// stack size
	}

	DECLARE_LUA_FUNCTION(CheckSkillPoint) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckSkillPoint(
			LUA_ARG(uint8_t, 2),		// skill point category
			LUA_ARG(uint8_t, 3),		// min
			LUA_ARG(uint8_t, 4)));	// max
	}

	DECLARE_LUA_FUNCTION(isRoomForItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->FindSlotForItem(
			LUA_ARG(uint32_t, 2),					// item ID
			LUA_ARG_OPTIONAL(uint16_t, 1, 3)));	// stack size
	}

	DECLARE_LUA_FUNCTION(CheckExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckExchange(LUA_ARG(uint32_t, 2)));	// exchange ID
	}

	DECLARE_LUA_FUNCTION(RunExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->RunExchange(LUA_ARG(uint32_t, 2)));	// exchange ID
	}

	DECLARE_LUA_FUNCTION(RunSelectExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->RunSelectExchange(LUA_ARG(uint32_t, 2)));	// exchange ID
	}

	DECLARE_LUA_FUNCTION(KissUser) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->KissUser());
	}

	DECLARE_LUA_FUNCTION(PromoteUserNovice) {
		LUA_RETURN(LUA_GET_INSTANCE()->PromoteUserNovice());
	}

	DECLARE_LUA_FUNCTION(PromoteUser) {
		LUA_RETURN(LUA_GET_INSTANCE()->PromoteUser());
	}

	DECLARE_LUA_FUNCTION(ShowEffect) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ShowEffect(LUA_ARG(uint32_t, 2))); // effect ID
	}

	DECLARE_LUA_FUNCTION(ShowNpcEffect) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ShowNpcEffect(LUA_ARG(uint32_t, 2))); // effect ID
	}

	DECLARE_LUA_FUNCTION(CheckWaiting) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->CheckWaiting(
			LUA_ARG(uint8_t, 2),		// zone ID
			LUA_ARG(uint16_t, 3)));	// Time
	}

	DECLARE_LUA_FUNCTION(ZoneChange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ZoneChange(
			LUA_ARG(uint16_t, 2),		// zone ID
			LUA_ARG(float, 3),		// x
			LUA_ARG(float, 4)));	// z
	}

	DECLARE_LUA_FUNCTION(ZoneChangeParty) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ZoneChangeParty(
			LUA_ARG(uint16_t, 2),		// zone ID
			LUA_ARG(float, 3),		// x
			LUA_ARG(float, 4)));	// z
	}

	DECLARE_LUA_FUNCTION(ZoneChangeClan) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ZoneChangeClan(
			LUA_ARG(uint16_t, 2),		// zone ID
			LUA_ARG(float, 3),		// x
			LUA_ARG(float, 4)));	// z
	}

	DECLARE_LUA_FUNCTION(SendNameChange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendNameChange());
	}

	DECLARE_LUA_FUNCTION(SendStatSkillDistribute) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendStatSkillDistribute());
	}

	DECLARE_LUA_FUNCTION(ResetStatPoints) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->AllPointChange());
	}

	DECLARE_LUA_FUNCTION(ResetSkillPoints) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->AllSkillPointChange());
	}

	DECLARE_LUA_FUNCTION(GiveLoyalty) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendLoyaltyChange(LUA_ARG(int32_t, 2)));	
	}

	DECLARE_LUA_FUNCTION(RobLoyalty) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendLoyaltyChange(-(LUA_ARG(int32_t, 2))));	
	}

	DECLARE_LUA_FUNCTION(ChangeManner) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendMannerChange(LUA_ARG(int32_t, 2)));	
	}

	DECLARE_LUA_FUNCTION(PromoteClan) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->PromoteClan((ClanTypeFlag) LUA_ARG_OPTIONAL(uint8_t, ClanTypePromoted, 2)));	
	}

	DECLARE_LUA_FUNCTION(GetStat) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetStat((StatType)(LUA_ARG(uint8_t, 2) + 1)));	
	}

	DECLARE_LUA_FUNCTION(RobClanPoint) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendClanPointChange(-(LUA_ARG(int32_t, 2))));	
	}

	DECLARE_LUA_FUNCTION(RequestPersonalRankReward) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetRankReward(true));	
	}

	DECLARE_LUA_FUNCTION(RequestReward) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetRankReward(false));	
	}

	DECLARE_LUA_FUNCTION(RunCountExchange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->RunExchange(
			LUA_ARG(int, 2),		
			LUA_ARG(uint32_t, 3)));
	}

	DECLARE_LUA_FUNCTION(GetMaxExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetMaxExchange((LUA_ARG(int, 2))));
	}

	DECLARE_LUA_FUNCTION(GetUserDailyOp) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetUserDailyOp((LUA_ARG(uint8_t, 2))));
	}

	DECLARE_LUA_FUNCTION(GetEventTrigger) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetEventTrigger());
	}

	DECLARE_LUA_FUNCTION(CheckMiddleStatueCapture) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckMiddleStatueCapture());
	}

	DECLARE_LUA_FUNCTION(MoveMiddleStatue) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->MoveMiddleStatue());
	}

	DECLARE_LUA_FUNCTION(LevelChange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->LevelChange(LUA_ARG(uint8_t, 2), false));
	}

	DECLARE_LUA_FUNCTION(GivePremium) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->GivePremium(LUA_ARG(uint8_t, 2), LUA_ARG_OPTIONAL(uint8_t, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(GetPVPMonumentNation) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetPVPMonumentNation());
	}

	DECLARE_LUA_FUNCTION(NationChange) {
		LUA_RETURN(LUA_GET_INSTANCE()->NationChange());
	}

	DECLARE_LUA_FUNCTION(GenderChange) {
		LUA_RETURN(LUA_GET_INSTANCE()->GenderChange((LUA_ARG(uint8_t, 2))));
	}

	DECLARE_LUA_FUNCTION(JobChange) {
		LUA_RETURN(LUA_GET_INSTANCE()->JobChange((LUA_ARG(uint8_t, 2))));
	}
};
