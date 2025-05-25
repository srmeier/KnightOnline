#pragma once

// Classes
#define KARUWARRIOR			101	// Beginner Karus Warrior
#define KARUROGUE			102	// Beginner Karus Rogue
#define KARUWIZARD			103	// Beginner Karus Magician
#define KARUPRIEST			104	// Beginner Karus Priest
#define BERSERKER			105	// Skilled (after first job change) Karus Warrior
#define GUARDIAN			106	// Mastered Karus Warrior
#define HUNTER				107	// Skilled (after first job change) Karus Rogue
#define PENETRATOR			108	// Mastered Karus Rogue
#define SORSERER			109	// Skilled (after first job change) Karus Magician
#define NECROMANCER			110	// Mastered Karus Magician
#define SHAMAN				111	// Skilled (after first job change) Karus Priest
#define DARKPRIEST			112	// Mastered Karus Priest

#define ELMORWARRRIOR		201	// Beginner El Morad Warrior
#define ELMOROGUE			202	// Beginner El Morad Rogue
#define ELMOWIZARD			203	// Beginner El Morad Magician
#define ELMOPRIEST			204	// Beginner El Morad Priest
#define BLADE				205	// Skilled (after first job change) El Morad Warrior
#define PROTECTOR			206	// Mastered El Morad Warrior
#define RANGER				207	// Skilled (after first job change) El Morad Rogue
#define ASSASSIN			208	// Mastered El Morad Rogue
#define MAGE				209	// Skilled (after first job change) El Morad Magician
#define ENCHANTER			210	// Mastered El Morad Magician
#define CLERIC				211	// Skilled (after first job change) El Morad Priest
#define DRUID				212	// Mastered El Morad Priest

// Races
#define KARUS_BIG			1	// Arch Tuarek (Karus Warriors - only!)
#define KARUS_MIDDLE		2	// Tuarek (Karus Rogues & Priests)
#define KARUS_SMALL			3	// Wrinkle Tuarek (Karus Magicians)
#define KARUS_WOMAN			4	// Puri Tuarek (Karus Priests)
#define BABARIAN			11	// Barbarian (El Morad Warriors - only!)
#define ELMORAD_MAN			12	// El Morad Male (El Morad - ALL CLASSES)
#define ELMORAD_WOMAN		13	// El Morad Female (El Morad - ALL CLASSES)

// Ÿ�ݺ� ����� //
#define GREAT_SUCCESS			0X01		// �뼺��
#define SUCCESS					0X02		// ����
#define NORMAL					0X03		// ����
#define	FAIL					0X04		// ���� 

enum e_QuestState
{
	QUEST_STATE_NOT_STARTED	= 0,
	QUEST_STATE_IN_PROGRESS	= 1,
	QUEST_STATE_COMPLETED	= 2
};

enum e_QuestID
{
	QUEST_ID_MASTERY_WARRIOR	= 51,
	QUEST_ID_MASTERY_ROGUE		= 52,
	QUEST_ID_MASTERY_MAGE		= 53,
	QUEST_ID_MASTERY_PRIEST		= 54
};

enum ItemMovementType
{
	ITEM_INVEN_SLOT			= 1,
	ITEM_SLOT_INVEN			= 2,
	ITEM_INVEN_INVEN		= 3,
	ITEM_SLOT_SLOT			= 4,
	ITEM_INVEN_ZONE			= 5,
	ITEM_ZONE_INVEN			= 6
};

enum ItemSlotType
{
	ItemSlot1HEitherHand		= 0,
	ItemSlot1HRightHand			= 1,
	ItemSlot1HLeftHand			= 2,
	ItemSlot2HRightHand			= 3,
	ItemSlot2HLeftHand			= 4,
	ItemSlotPauldron			= 5,
	ItemSlotPads				= 6,
	ItemSlotHelmet				= 7,
	ItemSlotGloves				= 8,
	ItemSlotBoots				= 9,
	ItemSlotEarring				= 10,
	ItemSlotNecklace			= 11,
	ItemSlotRing				= 12,
	ItemSlotShoulder			= 13,
	ItemSlotBelt				= 14,
	ItemSlotBag					= 25,
	ItemSlotCospreGloves		= 100,
	ItemSlotCosprePauldron		= 105,
	ItemSlotCospreHelmet		= 107,
	ItemSlotCospreWings			= 110
};

// Item Weapon Type Define
#define WEAPON_DAGGER			1
#define WEAPON_SWORD			2
#define WEAPON_2H_SWORD			22 // Kind field as-is
#define WEAPON_AXE				3
#define WEAPON_2H_AXE			32 // Kind field as-is
#define WEAPON_MACE				4
#define WEAPON_2H_MACE			42 // Kind field as-is
#define WEAPON_SPEAR			5
#define WEAPON_2H_SPEAR			52 // Kind field as-is
#define WEAPON_SHIELD			6
#define WEAPON_BOW				7
#define WEAPON_LONGBOW			8
#define WEAPON_LAUNCHER			10
#define WEAPON_STAFF			11
#define WEAPON_ARROW			12	// ��ų ���
#define WEAPON_JAVELIN			13	// ��ų ���
#define WEAPON_MACE2			18
#define WEAPON_WORRIOR_AC		21	// ��ų ���
#define WEAPON_LOG_AC			22	// ��ų ���
#define WEAPON_WIZARD_AC		23	// ��ų ���
#define WEAPON_PRIEST_AC		24	// ��ų ���
#define WEAPON_PICKAXE			61	// Unlike the others, this is just the Kind field as-is (not / 10).

#define ACCESSORY_EARRING		91
#define ACCESSORY_NECKLACE		92
#define ACCESSORY_RING			93
#define ACCESSORY_BELT			94

#define ITEM_KIND_COSPRE		252

////////////////////////////////////////////////////////////
// User Status //
#define USER_STANDING			0X01		// �� �ִ�.
#define USER_SITDOWN			0X02		// �ɾ� �ִ�.
#define USER_DEAD				0x03		// ��Ŷ�
//#define USER_BLINKING			0x04		// ��� ��Ƴ���!!!

////////////////////////////////////////////////////////////
// Durability Type
#define ATTACK				0x01
#define DEFENCE				0x02
#define REPAIR_ALL			0x03
#define ACID_ALL			0x04
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Knights Authority Type
/*
#define CHIEF				0x06
#define VICECHIEF			0x05*/
#define OFFICER				0x04
#define KNIGHT				0x03
//#define TRAINEE				0x02
#define PUNISH				0x01	

#define CHIEF				0x01	// ����
#define VICECHIEF			0x02	// �δ���
#define TRAINEE				0x05	// ���
#define COMMAND_CAPTAIN		100		// ���ֱ���
////////////////////////////////////////////////////////////

#define CLAN_COIN_REQUIREMENT	500000
#define CLAN_LEVEL_REQUIREMENT	20

#define ITEM_GOLD			900000000	// �� ������ ��ȣ...
#define ITEM_NO_TRADE		900000001	// Cannot be traded, sold or stored.

////////////////////////////////////////////////////////////
// EVENT MISCELLANOUS DATA DEFINE
#define ZONE_TRAP_INTERVAL	   1		// Interval is one second (in seconds) right now.
#define ZONE_TRAP_DAMAGE	   400		// HP Damage is 10 for now :)

////////////////////////////////////////////////////////////
#define PVP_MONUMENT_NP_BONUS	(5)	// 5 additional NP on kill

////////////////////////////////////////////////////////////
// SKILL POINT DEFINE
#define ORDER_SKILL			0x01
#define MANNER_SKILL		0X02
#define LANGUAGE_SKILL		0x03
#define BATTLE_SKILL		0x04
#define PRO_SKILL1			0x05
#define PRO_SKILL2			0x06
#define PRO_SKILL3			0x07
#define PRO_SKILL4			0x08

enum e_SkillPointCategory
{
	SKILL_POINT_CAT_FREE	= 0,
	SKILL_POINT_CAT_1		= 5,
	SKILL_POINT_CAT_2		= 6,
	SKILL_POINT_CAT_3		= 7,
	SKILL_POINT_CAT_MASTER	= 8
};

/////////////////////////////////////////////////////////////
// ITEM TYPE DEFINE
#define ITEM_TYPE_FIRE				0x01
#define ITEM_TYPE_COLD				0x02
#define ITEM_TYPE_LIGHTNING			0x03
#define ITEM_TYPE_POISON			0x04
#define ITEM_TYPE_HP_DRAIN			0x05
#define ITEM_TYPE_MP_DAMAGE			0x06
#define ITEM_TYPE_MP_DRAIN			0x07
#define ITEM_TYPE_MIRROR_DAMAGE		0x08

/////////////////////////////////////////////////////////////
// JOB GROUP TYPES
#define GROUP_WARRIOR				1
#define GROUP_ROGUE					2
#define GROUP_MAGE					3
#define GROUP_CLERIC				4
#define GROUP_ATTACK_WARRIOR		5
#define GROUP_DEFENSE_WARRIOR		6
#define GROUP_ARCHERER				7
#define GROUP_ASSASSIN				8
#define GROUP_ATTACK_MAGE			9
#define GROUP_PET_MAGE				10
#define GROUP_HEAL_CLERIC			11
#define GROUP_CURSE_CLERIC			12

//////////////////////////////////////////////////////////////
// USER ABNORMAL STATUS TYPES
enum AbnormalType
{
	ABNORMAL_INVISIBLE		= 0,	// Hides character completely (for GM visibility only).
	ABNORMAL_NORMAL			= 1,	// Shows character. This is the default for players.
	ABNORMAL_GIANT			= 2,	// Enlarges character.
	ABNORMAL_DWARF			= 3,	// Shrinks character.
	ABNORMAL_BLINKING		= 4,	// Forces character to start blinking.
	ABNORMAL_GIANT_TARGET	= 6		// Enlarges character and shows "Hit!" effect.
};

//////////////////////////////////////////////////////////////
// Object Type
#define NORMAL_OBJECT		0
#define SPECIAL_OBJECT		1

//////////////////////////////////////////////////////////////
// REGENE TYPES
#define REGENE_NORMAL		0
#define REGENE_MAGIC		1
#define REGENE_ZONECHANGE	2

struct _CLASS_COEFFICIENT
{
	uint16_t	sClassNum;
	float	ShortSword;
	float	Sword;
	float	Axe;
	float	Club;
	float	Spear;
	float	Pole;
	float	Staff;
	float	Bow;
	float	HP;
	float	MP;
	float	SP;
	float	AC;
	float	Hitrate;
	float	Evasionrate;
};

// Dropped loot/chest.
#define LOOT_ITEMS	6
struct _LOOT_ITEM
{
	uint32_t nItemID;
	uint16_t sCount;

	_LOOT_ITEM(uint32_t nItemID, uint32_t sCount)
	{
		this->nItemID = nItemID;
		this->sCount = sCount;
	}
};

struct _LOOT_BUNDLE
{
	uint32_t nBundleID;
	std::vector<_LOOT_ITEM> Items;
	float x, z, y;
	time_t tDropTime;
};

struct	_EXCHANGE_ITEM
{
	uint32_t	nItemID;
	uint32_t	nCount;
	uint16_t	sDurability;
	uint8_t	bSrcPos;
	uint8_t	bDstPos;
	uint64_t	nSerialNum;
};

enum ItemRace
{
	RACE_TRADEABLE_IN_48HR	= 19, // These items can't be traded until 48 hours from the time of creation
	RACE_UNTRADEABLE		= 20  // Cannot be traded or sold.
};

enum SellType
{
	SellTypeNormal		= 0, // sell price is 1/4 of the purchase price
	SellTypeFullPrice	= 1, // sell price is the same as the purchase price
	SellTypeNoRepairs	= 2  // sell price is 1/4 of the purchase price, item cannot be repaired.
};

struct _ITEM_TABLE
{
	uint32_t	m_iNum;
	std::string	m_sName;
	uint8_t	m_bKind;
	uint8_t	m_bSlot;
	uint8_t	m_bRace;
	uint8_t	m_bClass;
	uint16_t	m_sDamage;
	uint16_t	m_sDelay;
	uint16_t	m_sRange;
	uint16_t	m_sWeight;
	uint16_t	m_sDuration;
	uint32_t	m_iBuyPrice;
	uint32_t	m_iSellPrice;
	int16_t	m_sAc;
	uint8_t	m_bCountable;
	uint32_t	m_iEffect1;
	uint32_t	m_iEffect2;
	uint8_t	m_bReqLevel;
	uint8_t	m_bReqLevelMax;
	uint8_t	m_bReqRank;
	uint8_t	m_bReqTitle;
	uint8_t	m_bReqStr;
	uint8_t	m_bReqSta;
	uint8_t	m_bReqDex;
	uint8_t	m_bReqIntel;
	uint8_t	m_bReqCha;
	uint8_t	m_bSellingGroup;
	uint8_t	m_ItemType;
	uint16_t	m_sHitrate;
	uint16_t	m_sEvarate;
	uint16_t	m_sDaggerAc;
	uint16_t	m_sSwordAc;
	uint16_t	m_sMaceAc;
	uint16_t	m_sAxeAc;
	uint16_t	m_sSpearAc;
	uint16_t	m_sBowAc;
	uint8_t	m_bFireDamage;
	uint8_t	m_bIceDamage;
	uint8_t	m_bLightningDamage;
	uint8_t	m_bPoisonDamage;
	uint8_t	m_bHPDrain;
	uint8_t	m_bMPDamage;
	uint8_t	m_bMPDrain;
	uint8_t	m_bMirrorDamage;
	int16_t	m_sStrB;
	int16_t	m_sStaB;
	int16_t	m_sDexB;
	int16_t	m_sIntelB;
	int16_t	m_sChaB;
	int16_t	m_MaxHpB;
	int16_t	m_MaxMpB;
	int16_t	m_bFireR;
	int16_t	m_bColdR;
	int16_t	m_bLightningR;
	int16_t	m_bMagicR;
	int16_t	m_bPoisonR;
	int16_t	m_bCurseR;
	//int16_t	ItemClass;
	//int16_t	ItemExt;

	INLINE bool isStackable() { return m_bCountable != 0; }

	INLINE uint8_t GetKind() { return m_bKind; }
	INLINE uint8_t GetItemGroup() { return uint8_t(m_bKind / 10); }

	INLINE bool isDagger() { return GetItemGroup() == WEAPON_DAGGER; }
	INLINE bool isSword() { return GetItemGroup() == WEAPON_SWORD; }
	INLINE bool is2HSword() { return GetKind() == WEAPON_2H_SWORD; }
	INLINE bool isAxe() { return GetItemGroup() == WEAPON_AXE; }
	INLINE bool is2HAxe() { return GetKind() == WEAPON_2H_AXE; }
	INLINE bool isMace() { return GetItemGroup() == WEAPON_MACE || GetItemGroup() == WEAPON_MACE2; }
	INLINE bool is2HMace() { return GetKind() == WEAPON_2H_MACE || GetItemGroup() == WEAPON_MACE2; }
	INLINE bool isSpear() { return GetItemGroup() == WEAPON_SPEAR; }
	INLINE bool is2HSpear() { return GetKind() == WEAPON_2H_SPEAR; }
	INLINE bool isShield() { return GetItemGroup() == WEAPON_SHIELD; }
	INLINE bool isStaff() { return GetItemGroup() == WEAPON_STAFF; }
	INLINE bool isBow() { return GetItemGroup() == WEAPON_BOW || GetItemGroup() == WEAPON_LONGBOW; }
	INLINE bool isPickaxe() { return GetKind() == WEAPON_PICKAXE; }

	INLINE bool isAccessory() { return GetKind() == ACCESSORY_EARRING || GetKind() == ACCESSORY_NECKLACE || GetKind() == ACCESSORY_RING || GetKind() == ACCESSORY_BELT; }
	INLINE bool isEarring() { return GetKind() == ACCESSORY_EARRING; }
	INLINE bool isNecklace() { return GetKind() == ACCESSORY_NECKLACE; }
	INLINE bool isRing() { return GetKind() == ACCESSORY_RING; }
	INLINE bool isBelt() { return GetKind() == ACCESSORY_BELT; }
	
	INLINE bool is2Handed() { return m_bSlot == ItemSlot2HLeftHand || m_bSlot == ItemSlot2HRightHand; }
};

struct _ZONE_SERVERINFO
{
	int16_t		sServerNo;
	std::string	strServerIP;
};

struct _KNIGHTS_CAPE
{
	uint16_t	sCapeIndex;
	uint32_t	nReqCoins;
	uint8_t		byGrade;		// clan grade requirement
};

struct _KNIGHTS_ALLIANCE
{
	uint16_t	sMainAllianceKnights;
	uint16_t	sSubAllianceKnights;
	uint16_t	sMercenaryClan_1;
	uint16_t	sMercenaryClan_2;
};

struct _START_POSITION
{
	uint16_t	ZoneID;
	uint16_t	sKarusX;
	uint16_t	sKarusZ;
	uint16_t	sElmoradX;
	uint16_t	sElmoradZ;
	uint16_t	sKarusGateX;
	uint16_t	sKarusGateZ;
	uint16_t	sElmoradGateX;
	uint16_t	sElmoradGateZ;
	uint8_t	bRangeX;
	uint8_t	bRangeZ;
};

struct _KNIGHTS_RATING
{
	uint32_t nRank;
	uint16_t sClanID;
	uint32_t nPoints;
};

struct _USER_RANK
{
	uint16_t	nRank;  // shIndex for USER_KNIGHTS_RANK
	std::string strUserID[2];
	uint32_t	nSalary; // nMoney for USER_KNIGHTS_RANK
	uint32_t	nLoyalty[2]; // nKarusLoyaltyMonthly/nElmoLoyaltyMonthly for USER_PERSONAL_RANK
};

// TODO: Rewrite this system to be less script dependent for exchange logic.
// Coin requirements should be in the database, and exchanges should be grouped.
#define ITEMS_IN_ORIGIN_GROUP 5
#define ITEMS_IN_EXCHANGE_GROUP 5

struct _ITEM_EXCHANGE
{
	uint32_t	nIndex;
	uint16_t	sNpcNum;
	uint8_t	bRandomFlag;

	uint32_t	nOriginItemNum[ITEMS_IN_ORIGIN_GROUP];
	uint32_t	sOriginItemCount[ITEMS_IN_ORIGIN_GROUP];

	uint32_t	nExchangeItemNum[ITEMS_IN_EXCHANGE_GROUP];
	uint32_t	sExchangeItemCount[ITEMS_IN_EXCHANGE_GROUP];
};

#define MAX_ITEMS_REQ_FOR_UPGRADE 8
struct _ITEM_UPGRADE
{
	uint32_t	nIndex;
	uint16_t	sNpcNum;
	int8_t	bOriginType;
	uint16_t	sOriginItem;
	uint32_t	nReqItem[MAX_ITEMS_REQ_FOR_UPGRADE];
	uint32_t	nReqNoah;
	uint8_t	bRateType;
	uint16_t	sGenRate;
	int32_t	nGiveItem;
};

struct _RENTAL_ITEM
{
	uint32_t	nRentalIndex;
	uint32_t	nItemID;
	uint16_t	sDurability;
	uint64_t	nSerialNum;
	uint8_t	byRegType;
	uint8_t	byItemType;
	uint8_t	byClass;
	uint16_t	sRentalTime;
	uint32_t	nRentalMoney;
	std::string strLenderCharID;
	std::string strBorrowerCharID;
};

struct _USER_RANKING 
{
	uint16_t m_socketID;
	uint16_t m_bZone;
	uint8_t m_bNation;
	uint32_t m_iLoyaltyDaily;
	uint16_t m_iLoyaltyPremiumBonus;
	uint16_t m_KillCount; // Chaos Dungeon
	uint16_t m_DeathCount; // Chaos Dungeon
};

struct _EVENT_TRIGGER
{
	uint32_t nIndex;
	uint16_t byNpcType;
	uint32_t sTrapNum;
	uint32_t nTriggerNum;
};

struct _MONUMENT_INFORMATION
{
	uint16_t sSid;
	uint16_t sNid;
	int32_t RepawnedTime;
};

struct _MONSTER_CHALLENGE
{
	uint16_t sIndex;
	uint8_t bStartTime1;
	uint8_t bStartTime2;
	uint8_t bStartTime3;
	uint8_t bLevelMin;
	uint8_t bLevelMax;
};

struct _MONSTER_CHALLENGE_SUMMON_LIST
{
	uint16_t sIndex;
	uint8_t bLevel;
	uint8_t bStage;
	uint8_t bStageLevel;
	uint16_t sTime;
	uint16_t sSid;
	uint16_t sCount;
	uint16_t sPosX;
	uint16_t sPosZ;
	uint8_t bRange;
};

struct _USER_ITEM
{
	uint32_t nItemID;
	std::vector<uint64_t> nItemSerial;
};

struct _KNIGHTS_SIEGE_WARFARE
{
	uint16_t	sCastleIndex;
	uint16_t	sMasterKnights;
	uint8_t	bySiegeType;
	uint8_t	byWarDay;
	uint8_t	byWarTime;
	uint8_t	byWarMinute;
	uint16_t	sChallengeList_1;
	uint16_t	sChallengeList_2;
	uint16_t	sChallengeList_3;
	uint16_t	sChallengeList_4;
	uint16_t	sChallengeList_5;
	uint16_t	sChallengeList_6;
	uint16_t	sChallengeList_7;
	uint16_t	sChallengeList_8;
	uint16_t	sChallengeList_9;
	uint16_t	sChallengeList_10;
	uint8_t	byWarRequestDay;
	uint8_t	byWarRequestTime;
	uint8_t	byWarRequestMinute;
	uint8_t	byGuerrillaWarDay;
	uint8_t	byGuerrillaWarTime;
	uint8_t	byGuerrillaWarMinute;
	std::string	strChallengeList;
	uint16_t	sMoradonTariff;
	uint16_t	sDellosTariff;
	int32_t	nDungeonCharge;
	int32_t	nMoradonTax;
	int32_t	nDellosTax;
	uint16_t	sRequestList_1;
	uint16_t	sRequestList_2;
	uint16_t	sRequestList_3;
	uint16_t	sRequestList_4;
	uint16_t	sRequestList_5;
	uint16_t	sRequestList_6;
	uint16_t	sRequestList_7;
	uint16_t	sRequestList_8;
	uint16_t	sRequestList_9;
	uint16_t	sRequestList_10;
};

enum BuffType
{
	BUFF_TYPE_NONE					= 0,
	BUFF_TYPE_HP_MP					= 1,
	BUFF_TYPE_AC					= 2,
	BUFF_TYPE_SIZE					= 3,
	BUFF_TYPE_DAMAGE				= 4,
	BUFF_TYPE_ATTACK_SPEED			= 5,
	BUFF_TYPE_SPEED					= 6,
	BUFF_TYPE_STATS					= 7,
	BUFF_TYPE_RESISTANCES			= 8,
	BUFF_TYPE_ACCURACY				= 9,
	BUFF_TYPE_MAGIC_POWER			= 10,
	BUFF_TYPE_EXPERIENCE			= 11,
	BUFF_TYPE_WEIGHT				= 12,
	BUFF_TYPE_WEAPON_DAMAGE			= 13,
	BUFF_TYPE_WEAPON_AC				= 14,
	BUFF_TYPE_LOYALTY				= 15,
	BUFF_TYPE_NOAH_BONUS			= 16,
	BUFF_TYPE_PREMIUM_MERCHANT		= 17,
	BUFF_TYPE_ATTACK_SPEED_ARMOR	= 18,  // Berserker
	BUFF_TYPE_DAMAGE_DOUBLE			= 19,  // Critical Point
	BUFF_TYPE_DISABLE_TARGETING		= 20,  // Smoke Screen / Light Shock
	BUFF_TYPE_BLIND					= 21,  // Blinding (Strafe)
	BUFF_TYPE_FREEZE				= 22,  // Freezing Distance
	BUFF_TYPE_INSTANT_MAGIC			= 23,  // Instantly Magic
	BUFF_TYPE_DECREASE_RESIST		= 24,  // Minor resist
	BUFF_TYPE_MAGE_ARMOR			= 25,  // Fire / Ice / Lightning Armor
	BUFF_TYPE_PROHIBIT_INVIS		= 26,  // Source Marking
	BUFF_TYPE_RESIS_AND_MAGIC_DMG	= 27,  // Elysian Web
	BUFF_TYPE_TRIPLEAC_HALFSPEED	= 28,  // Wall of Iron
	BUFF_TYPE_BLOCK_CURSE			= 29,  // Counter Curse
	BUFF_TYPE_BLOCK_CURSE_REFLECT	= 30,  // Curse Refraction
	BUFF_TYPE_MANA_ABSORB			= 31,  // Outrage / Frenzy
	BUFF_TYPE_IGNORE_WEAPON			= 32,  // Weapon cancellation
	BUFF_TYPE_VARIOUS_EFFECTS		= 33,  // ... whatever the event item grants.
	BUFF_TYPE_PASSION_OF_SOUL		= 35,  // Passion of the Soul
	BUFF_TYPE_FIRM_DETERMINATION	= 36,  // Firm Determination
	BUFF_TYPE_SPEED2				= 40,  // Cold Wave
	BUFF_TYPE_UNK_EXPERIENCE		= 42,  // unknown buff type, used for something relating to XP.
	BUFF_TYPE_ATTACK_RANGE_ARMOR	= 43,  // Inevitable Murderous
	BUFF_TYPE_MIRROR_DAMAGE_PARTY	= 44,  // Minak's Thorn
	BUFF_TYPE_DAGGER_BOW_DEFENSE	= 45,  // Eskrima
	BUFF_TYPE_STUN                  = 47,  // Lighting Skill 
	BUFF_TYPE_LOYALTY_AMOUNT		= 55,  // Santa's Present
	BUFF_TYPE_NO_RECALL				= 150, // "Cannot use against the ones to be summoned"
	BUFF_TYPE_REDUCE_TARGET			= 151, // "Reduction" (reduces target's stats, but enlarges their character to make them easier to attack)
	BUFF_TYPE_SILENCE_TARGET		= 152, // Silences the target to prevent them from using any skills (or potions)
	BUFF_TYPE_NO_POTIONS			= 153, // "No Potion" prevents target from using potions.
	BUFF_TYPE_KAUL_TRANSFORMATION	= 154, // Transforms the target into a Kaul (a pig thing), preventing you from /town'ing or attacking, but increases defense.
	BUFF_TYPE_UNDEAD				= 155, // User becomes undead, increasing defense but preventing the use of potions and converting all health received into damage.
	BUFF_TYPE_UNSIGHT				= 156, // Blocks the caster's sight (not the target's).
	BUFF_TYPE_BLOCK_PHYSICAL_DAMAGE	= 157, // Blocks all physical damage.
	BUFF_TYPE_BLOCK_MAGICAL_DAMAGE	= 158, // Blocks all magical/skill damage.
	BUFF_TYPE_UNK_POTION			= 159, // unknown potion, "Return of the Warrior", "Comeback potion", perhaps some sort of revive?
	BUFF_TYPE_SLEEP					= 160, // Zantman (Sandman)
	BUFF_TYPE_INVISIBILITY_POTION	= 163, // "Unidentified potion"
	BUFF_TYPE_GODS_BLESSING			= 164, // Increases your defense/max HP 
	BUFF_TYPE_HELP_COMPENSATION		= 165, // Compensation for using the help system (to help, ask for help, both?)
};

enum FlyingSantaOrAngel
{
	FLYING_NONE		= 0,
	FLYING_SANTA	= 1,
	FLYING_ANGEL	= 2
};

#include "../shared/database/structs.h"
