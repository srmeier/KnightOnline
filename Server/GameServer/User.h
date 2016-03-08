#pragma once

#include "LuaEngine.h"
#include "../shared/KOSocket.h"

#include "Unit.h"
#include "ChatHandler.h"

struct _KNIGHTS_USER;
struct _EXCHANGE_ITEM;
struct _USER_SEAL_ITEM;

typedef std::map<uint64, _USER_SEAL_ITEM*>	UserItemSealMap;
typedef	std::list<_EXCHANGE_ITEM*>			ItemList;
typedef	std::map<uint32, time_t>			SkillCooldownList;
typedef	std::map<uint8, time_t>				MagicTypeCooldownList;
typedef	std::map<uint16, time_t>			RHitRepeatList; 
typedef	std::map<uint32, time_t>			UserSavedMagicMap;

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
	uint32 nSkillID;
	time_t tFlyingTime;

	Arrow(uint32 nSkillID, time_t tFlyingTime) 
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
	virtual uint16 GetID() { return GetSocketID(); }

	std::string & GetAccountName() { return m_strAccountID; }
	virtual std::string & GetName() { return m_strUserID; }

	std::string	m_strAccountID, m_strUserID;

	uint8	m_bRace;
	uint16	m_sClass;

	uint8	m_nHair;

	uint8	m_bRank;
	uint8	m_bTitle;
	int64	m_iExp;	
	uint32	m_iLoyalty, m_iLoyaltyMonthly;
	uint32	m_iMannerPoint;
	uint8	m_bFace;
	uint8	m_bCity;
	int16	m_bKnights;	
	uint8	m_bFame;
	int16	m_sHp, m_sMp, m_sSp;
	uint8	m_bStats[STAT_COUNT];
	uint8	m_bAuthority;
	int16	m_sPoints; // this is just to shut the compiler up
	uint32	m_iGold, m_iBank;
	int16	m_sBind;

	uint8    m_bstrSkill[10];	
	_ITEM_DATA m_sItemArray[INVENTORY_TOTAL];
	_ITEM_DATA m_sWarehouseArray[WAREHOUSE_MAX];

	uint8	m_bLogout;
	uint32	m_dwTime;
	time_t	m_lastSaveTime;

	uint8	m_bAccountStatus;
	uint8	m_bPremiumType;
	uint16	m_sPremiumTime;
	uint32  m_nKnightCash;

	bool	m_bSelectedCharacter;
	bool	m_bStoreOpen;

	int8	m_bMerchantState;
	int16	m_sMerchantsSocketID;
	std::list<uint16>	m_arMerchantLookers;
	_MERCH_DATA	m_arMerchantItems[MAX_MERCH_ITEMS]; //What is this person selling? Stored in "_MERCH_DATA" structure.
	bool	m_bPremiumMerchant;
	UserItemSealMap m_sealedItemMap;

	uint8	m_bRequestingChallenge, // opcode of challenge request being sent by challenger
		m_bChallengeRequested;  // opcode of challenge request received by challengee
	int16	m_sChallengeUser;

	// Rival system
	int16	m_sRivalID;			// rival's session ID
	time_t	m_tRivalExpiryTime;	// time when the rivalry ends

	// Anger gauge system 
	uint8	m_byAngerGauge; // values range from 0-5

	// Magic System Cooldown checks
	SkillCooldownList	m_CoolDownList;

	// Magic System Same time magic type checks
	MagicTypeCooldownList	m_MagicTypeCooldownList;

	// Attack System Cooldown checks
	RHitRepeatList	m_RHitRepeatList;

	ArrowList m_flyingArrows;
	std::recursive_mutex m_arrowLock;

	TransformationType m_transformationType;
	uint16	m_sTransformID;
	time_t	m_tTransformationStartTime;
	uint16	m_sTransformationDuration;

	bool	m_bIsChicken; // Is the character taking the beginner/chicken quest?
	bool	m_bIsHidingHelmet;

	bool	m_bMining;
	time_t	m_tLastMiningAttempt;

	int8	m_bPersonalRank;
	int8	m_bKnightsRank;

	float	m_oldx, m_oldy, m_oldz;
	int16	m_sDirection;

	int64	m_iMaxExp;

	uint16	m_sMaxWeight;
	uint16	m_sMaxWeightBonus;
	uint16 CUser::MaxWeight (uint16 MaxWeight);
	int16   m_sSpeed;

	uint8	m_bPlayerAttackAmount;
	uint8	m_bAddWeaponDamage;
	uint16	m_sAddArmourAc; 
	uint8	m_bPctArmourAc;

	int16	m_sItemMaxHp;
	int16	m_sItemMaxMp;
	uint16	m_sItemWeight;
	short	m_sItemAc;
	short	m_sItemHitrate;
	short	m_sItemEvasionrate;

	uint8	m_byAPBonusAmount;
	uint8	m_byAPClassBonusAmount[4]; // one for each of the 4 class types
	uint8	m_byAcClassBonusAmount[4]; // one for each of the 4 class types

	int16	m_sStatItemBonuses[STAT_COUNT];
	int8	m_bStatBuffs[STAT_COUNT];

	uint16	m_sExpGainAmount;
	uint8	m_bItemExpGainAmount;
	uint8	m_bNPGainAmount, m_bItemNPBonus, m_bSkillNPBonus;
	uint8	m_bNoahGainAmount, m_bItemNoahGainAmount;
	uint8	m_bMaxWeightAmount; 

	short	m_iMaxHp, m_iMaxMp;

	uint8	m_bResHpType;
	bool	m_bWarp;
	uint8	m_bNeedParty;

	uint16	m_sPartyIndex;
	bool	m_bInParty;
	bool	m_bPartyLeader;

	bool	m_bCanSeeStealth;
	uint8	m_bInvisibilityType;

	short	m_sExchangeUser;
	uint8	m_bExchangeOK;

	ItemList	m_ExchangeItemList;

	bool	m_bBlockPrivateChat;
	short	m_sPrivateChatUser;

	time_t	m_tHPLastTimeNormal;					// For Automatic HP recovery. 
	time_t	m_tHPStartTimeNormal;
	short	m_bHPAmountNormal;
	uint8	m_bHPDurationNormal;
	uint8	m_bHPIntervalNormal;

	time_t m_tGameStartTimeSavedMagic;

	uint32	m_fSpeedHackClientTime, m_fSpeedHackServerTime;
	uint8	m_bSpeedHackCheck;

	time_t	m_tBlinkExpiryTime;			// When you should stop blinking.

	uint32	m_bAbnormalType;			// Is the player normal, a giant, or a dwarf?
	uint32	m_nOldAbnormalType;

	int16	m_sWhoKilledMe;				// ID of the unit that killed you.
	int64	m_iLostExp;					// Experience points that were lost when you died.

	time_t	m_tLastTrapAreaTime;		// The last moment you were in the trap area.

	bool	m_bZoneChangeFlag;

	uint8	m_bRegeneType;				// Did you die and go home or did you type '/town'?

	time_t	m_tLastRegeneTime;			// The last moment you got resurrected.

	bool	m_bZoneChangeSameZone;		// Did the server change when you warped?

	int		m_iSelMsgEvent[MAX_MESSAGE_EVENT];
	short	m_sEventNid, m_sEventSid;
	uint32	m_nQuestHelperID;

	bool	m_bWeaponsDisabled;

	TeamColour	m_teamColour;
	uint32		m_iLoyaltyDaily;
	uint16		m_iLoyaltyPremiumBonus;
	uint16		m_KillCount;
	uint16		m_DeathCount;

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
		uint16 sClass = GetClassType();
		return (sClass <= ClassPriest);
	}

	INLINE bool isBeginnerWarrior() { return GetClassType() == ClassWarrior; }
	INLINE bool isBeginnerRogue()   { return GetClassType() == ClassRogue; }
	INLINE bool isBeginnerMage()    { return GetClassType() == ClassMage; }
	INLINE bool isBeginnerPriest()  { return GetClassType() == ClassPriest; }

	INLINE bool isNovice() 
	{
		uint16 sClass = GetClassType();
		return (sClass == ClassWarriorNovice || sClass == ClassRogueNovice
			|| sClass == ClassMageNovice || sClass == ClassPriestNovice); 
	}

	INLINE bool isNoviceWarrior() { return GetClassType() == ClassWarriorNovice; }
	INLINE bool isNoviceRogue()   { return GetClassType() == ClassRogueNovice; }
	INLINE bool isNoviceMage()    { return GetClassType() == ClassMageNovice; }
	INLINE bool isNovicePriest()  { return GetClassType() == ClassPriestNovice; }

	INLINE bool isMastered() 
	{
		uint16 sClass = GetClassType();
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

	INLINE int8 GetMerchantState() { return m_bMerchantState; }

	INLINE uint8 GetAuthority() { return m_bAuthority; }
	INLINE uint8 GetFame() { return m_bFame; }

	INLINE uint16 GetClass() { return m_sClass; }
	INLINE uint8 GetPremium() { return m_bPremiumType; }
	INLINE bool isLockableScroll(uint8 buffType) { return (buffType == BUFF_TYPE_HP_MP || buffType == BUFF_TYPE_AC || buffType == BUFF_TYPE_DAMAGE || buffType == BUFF_TYPE_SPEED || buffType == BUFF_TYPE_STATS); }
	INLINE uint8 GetRace() { return m_bRace; }

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

		uint8 classType = GetClassType();
		ASSERT(classType >= 1 && classType <= 12);
		return classTypes[classType - 1];
	}

	/**
	* @brief	Gets class type, independent of nation.
	*
	* @return	The class type.
	*/
	INLINE uint8 GetClassType()
	{
		return GetClass() % 100;
	}

	INLINE uint16 GetPartyID() { return m_sPartyIndex; }

	INLINE int16 GetClanID() { return m_bKnights; }
	INLINE void SetClanID(int16 val) { m_bKnights = val; }

	INLINE uint32 GetCoins() { return m_iGold; }
	INLINE uint32 GetInnCoins() { return m_iBank; }
	INLINE uint32 GetLoyalty() { return m_iLoyalty; }
	INLINE uint32 GetMonthlyLoyalty() { return m_iLoyaltyMonthly; }
	INLINE uint32 GetManner() { return m_iMannerPoint; }

	virtual int32 GetHealth() { return m_sHp; }
	virtual int32 GetMaxHealth() { return m_iMaxHp; }
	virtual int32 GetMana() { return m_sMp; }
	virtual int32 GetMaxMana() { return m_iMaxMp; }

	// Shortcuts for lazy people
	INLINE bool hasCoins(uint32 amount) { return (GetCoins() >= amount); }
	INLINE bool hasInnCoins(uint32 amount) { return (GetInnCoins() >= amount); }
	INLINE bool hasLoyalty(uint32 amount) { return (GetLoyalty() >= amount); }
	INLINE bool hasMonthlyLoyalty(uint32 amount) { return (GetMonthlyLoyalty() >= amount); }
	INLINE bool hasManner(uint32 amount) { return (GetManner() >= amount); }

	INLINE uint8 GetAngerGauge() { return m_byAngerGauge; }
	INLINE bool hasFullAngerGauge() { return GetAngerGauge() >= MAX_ANGER_GAUGE; }

	INLINE bool hasRival() { return GetRivalID() >= 0; }
	INLINE bool hasRivalryExpired() { return UNIXTIME >= m_tRivalExpiryTime; }
	INLINE int16 GetRivalID() { return m_sRivalID; }

	INLINE GameState GetState() { return m_state; }

	INLINE uint16 GetActiveQuestID() { return m_sEventDataIndex; }

	uint8 GetClanGrade();
	uint8 GetClanRank();
	uint32 GetClanPoint();
	void SendClanPointChange(int32 nChangeAmount = 0);

	uint8 GetBeefRoastVictory();
	uint8 GetRankReward(bool isMonthly);
	uint8 GetWarVictory();

	uint8 CheckMiddleStatueCapture();
	void MoveMiddleStatue();	

	uint8 GetPVPMonumentNation();

	uint8 GetMonsterChallengeTime();
	uint8 GetMonsterChallengeUserCount();

	INLINE uint8 GetStat(StatType type)
	{
		if (type >= STAT_COUNT)
			return 0;

		return m_bStats[type];
	}

	INLINE void SetStat(StatType type, uint8 val)
	{
		ASSERT(type < STAT_COUNT);
		m_bStats[type] = val;
	}

	INLINE int32 GetStatTotal() // NOTE: Shares name with another, but lack-of args should be self-explanatory
	{
		int32 total = 0; // NOTE: this loop should be unrolled by the compiler
		foreach_array (i, m_bStats)
			total += m_bStats[i];
		return total;
	}

	INLINE int16 GetStatItemBonus(StatType type)
	{
		ASSERT(type < STAT_COUNT);
		return m_sStatItemBonuses[type];
	}

	INLINE int16 GetStatWithItemBonus(StatType type)
	{
		return GetStat(type) + GetStatItemBonus(type);
	}

	INLINE int32 GetStatItemBonusTotal()
	{
		int32 total = 0; // NOTE: this loop should be unrolled by the compiler
		foreach_array (i, m_sStatItemBonuses)
			total += m_sStatItemBonuses[i];
		return total;
	}

	INLINE uint16 GetStatBonusTotal(StatType type)
	{
		return GetStatBuff(type) + GetStatItemBonus(type);
	}

	INLINE uint8 GetStatBuff(StatType type)
	{
		ASSERT(type < STAT_COUNT);
		return m_bStatBuffs[type];
	}

	INLINE void SetStatBuff(StatType type, uint8 val)
	{
		ASSERT(type < STAT_COUNT);
		m_bStatBuffs[type] = val;
	}

	INLINE uint32 GetStatBuffTotal()
	{
		uint32 total = 0; // NOTE: this loop should be unrolled by the compiler
		foreach_array (i, m_bStatBuffs)
			total += m_bStatBuffs[i];
		return total;
	}

	INLINE uint16 getStatTotal(StatType type)
	{
		return GetStat(type) + GetStatItemBonus(type) + GetStatBuff(type);
	}

	INLINE uint16 GetTotalSkillPoints()
	{
		return m_bstrSkill[SkillPointFree] + m_bstrSkill[SkillPointCat1] 
		+ m_bstrSkill[SkillPointCat2] + m_bstrSkill[SkillPointCat3] 
		+ m_bstrSkill[SkillPointMaster];
	}

	INLINE uint8 GetSkillPoints(SkillPointCategory category)
	{
		if (category < SkillPointFree || category > SkillPointMaster)
			return 0;

		return m_bstrSkill[category];
	}

	INLINE _ITEM_DATA * GetItem(uint8 pos) 
	{
		ASSERT(pos < INVENTORY_TOTAL);
		return &m_sItemArray[pos]; 
	}

	INLINE _ITEM_TABLE * GetItemPrototype(uint8 pos) 
	{
		_ITEM_DATA * pItem;
		ASSERT(pos < INVENTORY_TOTAL);
		return GetItemPrototype(pos, pItem);
	}

	_ITEM_TABLE * GetItemPrototype(uint8 pos, _ITEM_DATA *& pItem);

	INLINE KOMap * GetMap() { return m_pMap; }

	CUser(uint16 socketID, SocketMgr *mgr); 

	virtual void OnConnect();
	virtual void OnDisconnect();
	virtual bool HandlePacket(Packet & pkt);

	void Update();

	virtual void AddToRegion(int16 new_region_x, int16 new_region_z);

	void SetRival(CUser * pRival);
	void RemoveRival();
	void SendLoyaltyChange(int32 nChangeAmount = 0, bool bIsKillReward = false, bool bIsBonusTime = false, bool bIsAddLoyaltyMonthly = true);

	void NativeZoneReturn();
	void KickOutZoneUser(bool home = false, uint8 nZoneID = 21);
	void TrapProcess();
	bool JobGroupCheck(short jobgroupid);
	void SendSay(int32 nTextID[8]);
	void SelectMsg(uint8 bFlag, int32 nQuestID, int32 menuHeaderText, 
		int32 menuButtonText[MAX_MESSAGE_EVENT], int32 menuButtonEvents[MAX_MESSAGE_EVENT]);
	bool CheckClass(short class1, short class2 = -1, short class3 = -1, short class4 = -1, short class5 = -1, short class6 = -1);
	bool GiveItem(uint32 nItemID, uint16 sCount = 1, bool send_packet = true, uint32 Time = 0);
	bool RobItem(uint32 nItemID, uint32 sCount = 1);
	bool RobItem(uint8 bPos, _ITEM_TABLE * pTable, uint32 sCount = 1);
	bool RobAllItemParty(uint32 nItemID, uint16 sCount = 1);
	bool CheckExistItem(int itemid, short count = 1);
	bool CheckExistItemAnd(int32 nItemID1, int32 sCount1, int32 nItemID2, int32 sCount2,
		int32 nItemID3, int32 sCount3, int32 nItemID4, int32 sCount4, int32 nItemID5, int32 sCount5,
		int32 nItemID6, int32 sCount6, int32 nItemID7, int32 sCount7, int32 nItemID8, int32 sCount8,
		int32 nItemID9, int32 sCount9, int32 nItemID10, int32 sCount10, int32 nItemID11, int32 sCount11);
	uint32 GetItemCount(uint32 nItemID);
	bool CheckWeight(uint32 nItemID, uint32 sCount);
	bool CheckWeight(_ITEM_TABLE * pTable, uint32 nItemID, uint32 sCount);
	bool CheckSkillPoint(uint8 skillnum, uint8 min, uint8 max);
	bool GoldLose(uint32 gold, bool bSendPacket = true);
	void GoldGain(uint32 gold, bool bSendPacket = true, bool bApplyBonus = false);
	void SendItemWeight();
	void UpdateVisibility(InvisibilityType bNewType);
	void ResetGMVisibility();
	void BlinkStart();
	void BlinkTimeCheck();
	void GoldChange(short tid, int gold);
	CUser * GetItemRoutingUser(uint32 nItemID, uint32 sCount);
	bool GetStartPosition(short & x, short & y, uint8 bZone = 0);
	bool GetStartPositionRandom(short & x, short & z, uint8 bZone = 0);
	int FindSlotForItem(uint32 nItemID, uint16 sCount = 1);
	int GetEmptySlot();
	void SendAllKnightsID();
	void SendStackChange(uint32 nItemID, uint32 nCount /* needs to be 4 bytes, not a bug */, uint16 sDurability, uint8 bPos, bool bNewItem = false, uint32 Time = 0);

	void Type4Duration();
	void HPTimeChange();
	void HPTimeChangeType3();

	short GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	void OnAttack(Unit * pTarget, AttackType attackType);
	void OnDefend(Unit * pAttacker, AttackType attackType);
	bool TriggerProcItem(uint8 bSlot, Unit * pTarget, ItemTriggerType triggerType);

	void SendDurability(uint8 slot, uint16 durability);
	void SendItemMove(uint8 subcommand);
	void ItemWoreOut( int type, int damage );
	void Dead();
	void GetUserInfoForAI(Packet & result);
	bool ItemEquipAvailable( _ITEM_TABLE* pTable );
	virtual void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToAI = true);
	virtual void MSpChange(int amount);
	void SendPartyHPUpdate();
	void ShowEffect(uint32 nSkillID);
	void ShowNpcEffect(uint32 nEffectID, bool bSendToRegion = false);
	void SendAnvilRequest(uint16 sNpcID, uint8 bType = ITEM_UPGRADE_REQ);
	void RecastSavedMagic(uint8 buffType = 0);
	void RecastLockableScrolls(uint8 buffType);

	// packet handlers start here
	void VersionCheck(Packet & pkt);
	void LoginProcess(Packet & pkt);
	bool WordGuardSystem(std::string Word, uint8 WordStr);

	void SelNationToAgent(Packet & pkt);
	void AllCharInfoToAgent();
	void ChangeHair(Packet & pkt);
	void NewCharToAgent(Packet & pkt);
	void DelCharToAgent(Packet & pkt);
	void SelCharToAgent(Packet & pkt);
	void SelectCharacter(Packet & pkt); // from the database
	void SetLogInInfoToDB(uint8 bInit);
	void RecvLoginInfo(Packet & pkt); // from the database

	void SpeedHackTime(Packet & pkt);
	void TempleProcess(Packet & pkt );
	void TempleOperations(uint8 bType = 0);

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

	uint8 GetUserDailyOp(uint8 type = 0);
	void SetUserDailyOp(uint8 type = 0, bool isInsert = false);

	uint32 GetEventTrigger();

	void RemoveStealth();

	void GivePremium(uint8 bPremiumType, uint16 sPremiumTime);
	void RobChaosSkillItems();

	// Nation Transfer, Gender Change and Job Change (in game)
	uint8 NationChange();
	uint8 GetNewRace();
	bool GenderChange(uint8 nRace = 0);
	uint8 JobChange(uint8 NewJob = 0);

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

	void Regene(uint8 regene_type, uint32 magicid = 0);
	void RequestUserIn(Packet & pkt);
	void RequestNpcIn(Packet & pkt);
	void RecvWarp(Packet & pkt);
	void Warp(uint16 sPosX, uint16 sPosZ);
	void ItemMove(Packet & pkt);
	void NpcEvent(Packet & pkt);

	void ItemTrade(Packet & pkt);

	void BundleOpenReq(Packet & pkt);
	void ItemGet(Packet & pkt);
	CUser * GetLootUser(_LOOT_BUNDLE * pBundle, _LOOT_ITEM * pItem);

	void RecvZoneChange(Packet & pkt);
	void PointChange(Packet & pkt);

	void StateChange(Packet & pkt);
	virtual void StateChangeServerDirect(uint8 bType, uint32 nBuff);

	void PartyProcess(Packet & pkt);
	void PartyDelete();
	void PartyPromote(uint16 sMemberID);
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
	void FriendModify(Packet & pkt, uint8 opcode);
	void RecvFriendModify(Packet & pkt, uint8 opcode);
	void FriendReport(Packet & pkt);
	uint8 GetFriendStatus(std::string & charName, int16 & sid);

	void SelectWarpList(Packet & pkt);
	bool GetWarpList( int warp_group );

	void ServerChangeOk(Packet & pkt);

	void PartyBBS(Packet & pkt);
	void PartyBBSRegister(Packet & pkt);
	void PartyBBSDelete(Packet & pkt);
	void PartyBBSNeeded(Packet & pkt, uint8 type);
	void PartyBBSWanted(Packet & pkt);
	uint8 GetPartyMemberAmount(_PARTY_GROUP *pParty = nullptr);

	void SendPartyBBSNeeded(uint16 page_index, uint8 bType);
	

	void ClientEvent(uint16 sNpcID);
	void KissUser();

	void RecvSelectMsg(Packet & pkt);
	bool AttemptSelectMsg(uint8 bMenuID, int8 bySelectedReward);

	// from the client
	void ItemUpgradeProcess(Packet & pkt);
	void ItemUpgrade(Packet & pkt, uint8 nUpgradeType = ITEM_UPGRADE);
	void ItemUpgradeNotice(_ITEM_TABLE * pItem, uint8 UpgradeResult);
	void ItemUpgradeAccessories(Packet & pkt);
	void BifrostPieceProcess(Packet & pkt); // originally named BeefRoastPieceProcess() -- that's not happening.
	void SpecialItemExchange(Packet & pkt);
	void ItemUpgradeRebirth(Packet & pkt);
	void ItemSealProcess(Packet & pkt);
	void SealItem(uint8 bSealType, uint8 bSrcPos);
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
	void HandleChallengeCancelled(uint8 opcode);
	void HandleChallengeRejected(uint8 opcode);

	void HandlePlayerRankings(Packet & pkt);
	uint16 GetPlayerRank(uint8 nRankType);

	void HandleMiningSystem(Packet & pkt);
	void HandleMiningStart(Packet & pkt);
	void HandleMiningAttempt(Packet & pkt);
	void HandleMiningStop(Packet & pkt);

	void HandleSoccer(Packet & pkt);

	void SendNotice();
	void AppendNoticeEntry(Packet & pkt, uint8 & elementCount, const char * message, const char * title);
	void AppendExtraNoticeData(Packet & pkt, uint8 & elementCount);
	void UserLookChange( int pos, int itemid, int durability );
	void SpeedHackUser();
	void LoyaltyChange(int16 tid, uint16 bonusNP = 0);
	void LoyaltyDivide(int16 tid, uint16 bonusNP = 0);
	int16 GetLoyaltyDivideSource(uint8 totalmember = 0);
	int16 GetLoyaltyDivideTarget();
	void GrantChickenManner();
	void SendMannerChange(int32 iMannerPoints);

	bool CanLevelQualify(uint8 sLevel);
	bool CanChangeZone(C3DMap * pTargetMap, WarpListResponse & errorReason);
	void CheckWaiting(uint8 sNewZone, uint16 Time);
	void ZoneChange(uint16 sNewZone, float x, float z);
	void ZoneChangeParty(uint16 sNewZone, float x, float z);
	void ZoneChangeClan(uint16 sNewZone, float x, float z);
	void PlayerRankingProcess(uint16 ZoneID, bool RemoveInZone = false);
	void AddPlayerRank(uint16 ZoneID);
	void RemovePlayerRank();
	void UpdatePlayerRank();

	bool isEventUser();

	void SendTargetHP( uint8 echo, int tid, int damage = 0 );
	bool IsValidSlotPos( _ITEM_TABLE* pTable, int destpos );
	void SetUserAbility(bool bSendPacket = true);
	void LevelChange(uint8 level, bool bLevelUp = true);
	void SetSlotItemValue();
	void ApplySetItemBonuses(_SET_ITEM * pItem);
	void SendTime();
	void SendWeather();
	void SendPremiumInfo();
	void SetZoneAbilityChange(uint16 sNewZone);
	void SetMaxMp();
	void SetMaxHp(int iFlag = 0);
	void RecvUserExp(Packet & pkt);
	void ExpChange(int64 iExp, bool bIsBonusReward = false);
	void LogOut();
	void SendMyInfo();
	void SendServerChange(std::string & ip, uint8 bInit);
	void Send2AI_UserUpdateInfo(bool initialInfo = false);
	uint16 GetPremiumProperty(PremiumPropertyOpCodes type);
	void BifrostProcess(CUser * pUser);
	void CastleSiegeWarProcess(CUser * pUser);
	void SiegeWarFareNpc(Packet & pkt);
	void LogosShout(Packet & pkt);

	virtual void GetInOut(Packet & result, uint8 bType);
	void UserInOut(uint8 bType);

	void GetUserInfo(Packet & pkt);
	void SendUserStatusUpdate(UserStatus type, UserStatusBehaviour status);
	virtual void Initialize();

	void ChangeFame(uint8 bFame);
	void SendServerIndex();

	void SendToRegion(Packet *pkt, CUser *pExceptUser = nullptr, uint16 nEventRoom = 0);
	void SendToZone(Packet *pkt, CUser *pExceptUser = nullptr, uint16 nEventRoom = 0, float fRange = 0.0f);

	virtual void OnDeath(Unit *pKiller);
	void UpdateAngerGauge(uint8 byAngerGauge);
	void InitializeStealth();

	// Exchange system
	bool CheckExchange(int nExchangeID);
	bool RunExchange(int nExchangeID, uint32 count = 0 /* No random flag */);
	bool RunSelectExchange(int nExchangeID);
	uint32 GetMaxExchange(int nExchangeID);	

	// Clan system
	void SendClanUserStatusUpdate(bool bToRegion = true);

	void SendPartyStatusUpdate(uint8 bStatus, uint8 bResult = 0);

	bool CanUseItem(uint32 nItemID, uint32 sCount = 1);

	void CheckSavedMagic();
	virtual void InsertSavedMagic(uint32 nSkillID, uint16 sDuration);
	virtual void RemoveSavedMagic(uint32 nSkillID);
	virtual bool HasSavedMagic(uint32 nSkillID);
	virtual int16 GetSavedMagicDuration(uint32 nSkillID);

	void SaveEvent(uint16 sQuestID, uint8 bQuestState);
	void DeleteEvent(uint16 sQuestID);
	bool CheckExistEvent(uint16 sQuestID, uint8 bQuestState);

	void QuestV2MonsterCountAdd(uint16 sNpcID);
	uint8 QuestV2CheckMonsterCount(uint16 sQuestID);
	void QuestV2MonsterDataDeleteAll();

	// Sends the quest completion statuses
	void QuestDataRequest();

	// Handles new quest packets
	void QuestV2PacketProcess(Packet & pkt);
	void QuestV2MonsterDataRequest();
	void QuestV2ExecuteHelper(_QUEST_HELPER * pQuestHelper);
	void QuestV2CheckFulfill(_QUEST_HELPER * pQuestHelper);
	bool QuestV2RunEvent(_QUEST_HELPER * pQuestHelper, uint32 nEventID, int8 bSelectedReward = 0);

	void QuestV2SaveEvent(uint16 sQuestID);
	void QuestV2SendNpcMsg(uint32 nQuestID, uint16 sNpcID);
	void QuestV2ShowGiveItem(uint32 nUnk1, uint32 sUnk1, 
		uint32 nUnk2, uint32 sUnk2,
		uint32 nUnk3, uint32 sUnk3,
		uint32 nUnk4, uint32 sUnk4);
	uint16 QuestV2SearchEligibleQuest(uint16 sNpcID);
	void QuestV2ShowMap(uint32 nQuestHelperID);
	uint8 CheckMonsterCount(uint8 bGroup);

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
	void InsertTaxUpEvent(uint8 Nation, uint32 TerritoryTax);

	//private:
	static ChatCommandTable s_commandTable;
	GameState m_state;

	// quest ID | quest state (need to replace with enum)
	typedef std::map<uint16, uint8> QuestMap;
	QuestMap m_questMap;

	uint8 m_bKillCounts[QUEST_MOB_GROUPS];
	uint16 m_sEventDataIndex;

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
		LUA_RETURN(LUA_GET_INSTANCE()->hasCoins(LUA_ARG(uint32, 2))); 
	}

	DECLARE_LUA_FUNCTION(hasInnCoins) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasInnCoins(LUA_ARG(uint32, 2))); 
	}

	DECLARE_LUA_FUNCTION(hasLoyalty) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasLoyalty(LUA_ARG(uint32, 2))); 
	}

	DECLARE_LUA_FUNCTION(hasMonthlyLoyalty) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasMonthlyLoyalty(LUA_ARG(uint32, 2)));
	}

	DECLARE_LUA_FUNCTION(hasManner) {
		LUA_RETURN(LUA_GET_INSTANCE()->hasManner(LUA_ARG(uint32, 2))); 
	}

	// The useful method wrappers
	DECLARE_LUA_FUNCTION(GiveItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->GiveItem(
			LUA_ARG(uint32, 2), 
			LUA_ARG_OPTIONAL(uint16, 1, 3), 
			true,
			LUA_ARG_OPTIONAL(uint32, 0, 4)));
	}

	DECLARE_LUA_FUNCTION(RobItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->RobItem(
			LUA_ARG(uint32, 2), 
			LUA_ARG_OPTIONAL(uint16, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(RobAllItemParty) {
		LUA_RETURN(LUA_GET_INSTANCE()->RobAllItemParty(
			LUA_ARG(uint32, 2), 
			LUA_ARG_OPTIONAL(uint16, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(CheckExistItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckExistItem(
			LUA_ARG(uint32, 2), 
			LUA_ARG_OPTIONAL(uint16, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(GoldGain) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->GoldGain(LUA_ARG(int32, 2)));	
	}

	DECLARE_LUA_FUNCTION(GoldLose) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->GoldLose(LUA_ARG(uint32, 2)));	
	}

	DECLARE_LUA_FUNCTION(ExpChange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ExpChange(LUA_ARG(int32, 2)));	
	}

	DECLARE_LUA_FUNCTION(SaveEvent) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->QuestV2SaveEvent(
			LUA_ARG(uint16, 2)));  // quest ID
	}

	DECLARE_LUA_FUNCTION(SearchQuest) {
		CUser * pUser = LUA_GET_INSTANCE();
		LUA_RETURN(pUser->QuestV2SearchEligibleQuest(LUA_ARG_OPTIONAL(uint16, pUser->m_sEventSid, 2))); // NPC ID
	}

	DECLARE_LUA_FUNCTION(ShowMap) {
		CUser * pUser = LUA_GET_INSTANCE();
		LUA_NO_RETURN(pUser->QuestV2ShowMap(LUA_ARG_OPTIONAL(uint32, pUser->m_nQuestHelperID, 2))); // quest helper ID
	}

	DECLARE_LUA_FUNCTION(CountMonsterQuestSub) {
		LUA_RETURN(LUA_GET_INSTANCE()->QuestV2CheckMonsterCount((LUA_ARG(uint16, 2))));
	}

	DECLARE_LUA_FUNCTION(CountMonsterQuestMain) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->QuestV2MonsterCountAdd((LUA_ARG(uint16, 2))));
	} 

	DECLARE_LUA_FUNCTION(NpcSay) {
		CUser * pUser = LUA_GET_INSTANCE();
		uint32 arg = 2; // start from after the user instance.
		int32 nTextID[8]; 

		foreach_array(i, nTextID)
			nTextID[i] = LUA_ARG_OPTIONAL(int32, -1, arg++);

		LUA_NO_RETURN(pUser->SendSay(nTextID));
	}

	// This is probably going to be cleaned up, as the methodology behind these menus is kind of awful.
	// For now, we'll just copy existing behaviour: that is, pass along a set of text IDs & button IDs.
	DECLARE_LUA_FUNCTION(SelectMsg) {
		CUser * pUser = LUA_GET_INSTANCE();
		uint32 arg = 2; // start from after the user instance.
		int32 menuButtonText[MAX_MESSAGE_EVENT], 
			menuButtonEvents[MAX_MESSAGE_EVENT];
		uint8 bFlag = LUA_ARG(uint8, arg++);
		int32 nQuestID = LUA_ARG_OPTIONAL(int32, -1, arg++);
		int32 menuHeaderText = LUA_ARG(int32, arg++);

		foreach_array(i, menuButtonText)
		{
			menuButtonText[i] = LUA_ARG_OPTIONAL(int32, -1, arg++);
			menuButtonEvents[i] = LUA_ARG_OPTIONAL(int32, -1, arg++);
		}

		LUA_NO_RETURN(pUser->SelectMsg(bFlag, nQuestID, menuHeaderText, menuButtonText, menuButtonEvents));
	}

	DECLARE_LUA_FUNCTION(NpcMsg) {
		CUser * pUser = LUA_GET_INSTANCE();
		LUA_NO_RETURN(pUser->QuestV2SendNpcMsg(
			LUA_ARG(uint32, 2),
			LUA_ARG_OPTIONAL(uint16, pUser->m_sEventSid, 3)));
	}

	DECLARE_LUA_FUNCTION(CheckWeight) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckWeight(
			LUA_ARG(uint32, 2),		// item ID
			LUA_ARG_OPTIONAL(uint16, 1, 3)));	// stack size
	}

	DECLARE_LUA_FUNCTION(CheckSkillPoint) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckSkillPoint(
			LUA_ARG(uint8, 2),		// skill point category
			LUA_ARG(uint8, 3),		// min
			LUA_ARG(uint8, 4)));	// max
	}

	DECLARE_LUA_FUNCTION(isRoomForItem) {
		LUA_RETURN(LUA_GET_INSTANCE()->FindSlotForItem(
			LUA_ARG(uint32, 2),					// item ID
			LUA_ARG_OPTIONAL(uint16, 1, 3)));	// stack size
	}

	DECLARE_LUA_FUNCTION(CheckExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->CheckExchange(LUA_ARG(uint32, 2)));	// exchange ID
	}

	DECLARE_LUA_FUNCTION(RunExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->RunExchange(LUA_ARG(uint32, 2)));	// exchange ID
	}

	DECLARE_LUA_FUNCTION(RunSelectExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->RunSelectExchange(LUA_ARG(uint32, 2)));	// exchange ID
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
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ShowEffect(LUA_ARG(uint32, 2))); // effect ID
	}

	DECLARE_LUA_FUNCTION(ShowNpcEffect) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ShowNpcEffect(LUA_ARG(uint32, 2))); // effect ID
	}

	DECLARE_LUA_FUNCTION(CheckWaiting) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->CheckWaiting(
			LUA_ARG(uint8, 2),		// zone ID
			LUA_ARG(uint16, 3)));	// Time
	}

	DECLARE_LUA_FUNCTION(ZoneChange) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ZoneChange(
			LUA_ARG(uint16, 2),		// zone ID
			LUA_ARG(float, 3),		// x
			LUA_ARG(float, 4)));	// z
	}

	DECLARE_LUA_FUNCTION(ZoneChangeParty) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ZoneChangeParty(
			LUA_ARG(uint16, 2),		// zone ID
			LUA_ARG(float, 3),		// x
			LUA_ARG(float, 4)));	// z
	}

	DECLARE_LUA_FUNCTION(ZoneChangeClan) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->ZoneChangeClan(
			LUA_ARG(uint16, 2),		// zone ID
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
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendLoyaltyChange(LUA_ARG(int32, 2)));	
	}

	DECLARE_LUA_FUNCTION(RobLoyalty) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendLoyaltyChange(-(LUA_ARG(int32, 2))));	
	}

	DECLARE_LUA_FUNCTION(ChangeManner) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendMannerChange(LUA_ARG(int32, 2)));	
	}

	DECLARE_LUA_FUNCTION(PromoteClan) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->PromoteClan((ClanTypeFlag) LUA_ARG_OPTIONAL(uint8, ClanTypePromoted, 2)));	
	}

	DECLARE_LUA_FUNCTION(GetStat) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetStat((StatType)(LUA_ARG(uint8, 2) + 1)));	
	}

	DECLARE_LUA_FUNCTION(RobClanPoint) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->SendClanPointChange(-(LUA_ARG(int32, 2))));	
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
			LUA_ARG(uint32, 3)));
	}

	DECLARE_LUA_FUNCTION(GetMaxExchange) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetMaxExchange((LUA_ARG(int, 2))));
	}

	DECLARE_LUA_FUNCTION(GetUserDailyOp) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetUserDailyOp((LUA_ARG(uint8, 2))));
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
		LUA_NO_RETURN(LUA_GET_INSTANCE()->LevelChange(LUA_ARG(uint8, 2), false));
	}

	DECLARE_LUA_FUNCTION(GivePremium) {
		LUA_NO_RETURN(LUA_GET_INSTANCE()->GivePremium(LUA_ARG(uint8, 2), LUA_ARG_OPTIONAL(uint8, 1, 3)));
	}

	DECLARE_LUA_FUNCTION(GetPVPMonumentNation) {
		LUA_RETURN(LUA_GET_INSTANCE()->GetPVPMonumentNation());
	}

	DECLARE_LUA_FUNCTION(NationChange) {
		LUA_RETURN(LUA_GET_INSTANCE()->NationChange());
	}

	DECLARE_LUA_FUNCTION(GenderChange) {
		LUA_RETURN(LUA_GET_INSTANCE()->GenderChange((LUA_ARG(uint8, 2))));
	}

	DECLARE_LUA_FUNCTION(JobChange) {
		LUA_RETURN(LUA_GET_INSTANCE()->JobChange((LUA_ARG(uint8, 2))));
	}
};