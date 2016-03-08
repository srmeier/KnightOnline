#pragma once

#include "version.h"
#include "packets.h"
#include "Packet.h"

#define MAP_DIR				"./map/"

#define MAX_USER			3000

#define MIN_ID_SIZE			6
#define MAX_ID_SIZE			20
#define MAX_PW_SIZE			28

#define MAX_ITEM_COUNT		9999

#define MAX_ZONE_ID			255

#define VIEW_DISTANCE		48

#define DAY                HOUR   * 24

#define HOUR               MINUTE * 60
// Define a minute as 60s.
#define MINUTE				60u
// Define a second as 1000ms.
#define SECOND				1000u

enum NameType
{
	TYPE_ACCOUNT,
	TYPE_CHARACTER
};

enum Nation
{
	ALL = 0,
	KARUS,
	ELMORAD,
	NONE
};

enum NpcState
{
	NPC_DEAD = 0,
	NPC_LIVE,
	NPC_ATTACKING,
	NPC_ATTACKED,
	NPC_ESCAPE,
	NPC_STANDING,
	NPC_MOVING,
	NPC_TRACING,
	NPC_FIGHTING,
	NPC_STRATEGY,
	NPC_BACK,
	NPC_SLEEPING,
	NPC_FAINTING,
	NPC_HEALING,
	NPC_CASTING
};

enum NpcType
{
	NPC_MONSTER				= 0,
	NPC_GENERAL				= 1,
	NPC_BOSS				= 3,
	NPC_DUNGEON_MONSTER		= 4,
	NPC_TRAP_MONSTER		= 5,
	NPC_GUARD				= 11,
	NPC_PATROL_GUARD		= 12,
	NPC_STORE_GUARD			= 13,
	NPC_WAR_GUARD			= 14,
	NPC_MERCHANT			= 21,
	NPC_TINKER				= 22,
	NPC_SELITH				= 23, // Selith[special store]
	NPC_ANVIL				= 24,

	NPC_MARK				= 25,
	NPC_CLAN_MATCH_ADVISOR	= 26,
	NPC_SIEGE_1				= 27,
	NPC_OPERATOR			= 29, // not sure what Operator Moira was ever supposed to do...
	NPC_WAREHOUSE			= 31,
	NPC_KISS				= 32, // pretty useless.
	NPC_ISAAC				= 33, // need to check which quests he handles
	NPC_KAISHAN				= 34, // need to see what he actually does to name this properly
	NPC_CAPTAIN				= 35,
	NPC_CLAN				= 36,
	NPC_CLERIC				= 37,
	NPC_LADY				= 38, // Calamus lady event -- need to see what they're used for
	NPC_ATHIAN				= 39, // Priest athian -- need to see what they're used for
	NPC_HEALER				= 40,
	NPC_ROOM				= 42,
	NPC_ARENA				= 43, // also recon guards
	NPC_SIEGE				= 44,
	NPC_SENTINEL_PATRICK	= 47, // need to check which quests he handles (was it the beginner quests, or was that isaac?)

	NPC_GATE				= 50,
	NPC_PHOENIX_GATE		= 51,
	NPC_SPECIAL_GATE		= 52,
	NPC_VICTORY_GATE		= 53,
	NPC_GATE_LEVER			= 55,
	NPC_ARTIFACT			= 60,
	NPC_DESTROYED_ARTIFACT	= 61,
	NPC_GUARD_TOWER1		= 62,
	NPC_GUARD_TOWER2		= 63,
	NPC_BOARD				= 64, // also encampment
	NPC_ARTIFACT1			= 65, // Protective artifact
	NPC_ARTIFACT2			= 66, // Guard Tower artifact
	NPC_ARTIFACT3			= 67, // Guard artifact
	NPC_ARTIFACT4			= 68,
	NPC_MONK_ELMORAD		= 71,
	NPC_MONK_KARUS			= 72,
	NPC_BLACKSMITH			= 77,
	NPC_RENTAL				= 78,
	NPC_ELECTION			= 79, // king elections
	NPC_TREASURY			= 80,
	NPC_DOMESTIC_ANIMAL		= 99,
	NPC_COUPON				= 100,
	NPC_HERO_STATUE_1		= 106, // 1st place
	NPC_HERO_STATUE_2		= 107, // 2nd place
	NPC_HERO_STATUE_3		= 108, // 3rd place
	NPC_KEY_QUEST_1			= 111, // Sentinel of the Key
	NPC_KEY_QUEST_2			= 112, // Watcher of the Key
	NPC_KEY_QUEST_3			= 113, // Protector of the Key
	NPC_KEY_QUEST_4			= 114, // Ranger of the Key
	NPC_KEY_QUEST_5			= 115, // Patroller of the Key
	NPC_KEY_QUEST_6			= 116, // Recon of the Key
	NPC_KEY_QUEST_7			= 117, // Keeper of the Key
	NPC_ROBOS				= 118, // need to see what he actually does to name this properly
	NPC_KARUS_MONUMENT		= 121, // Luferson Monument/Linate Monument/Bellua monument/Laon Camp Monument
	NPC_HUMAN_MONUMENT		= 122, // El Morad/Asga village/Raiba village/Doda camp monuments
	NPC_SERVER_TRANSFER		= 123,
	NPC_RANKING				= 124,
	NPC_LYONI				= 125, // need to see what this NPC actually does to name this properly
	NPC_BEGINNER_HELPER		= 127,
	NPC_FT_1				= 129,
	NPC_FT_2				= 130,
	NPC_FT_3				= 131, // also Priest Minerva
	NPC_KJWAR				= 133,
	NPC_SIEGE_2				= 134,
	NPC_CRAFTSMAN			= 135, // Craftsman boy, not sure what he's actually used for
	NPC_CHAOTIC_GENERATOR	= 137,
	NPC_SPY					= 141,
	NPC_ROYAL_GUARD			= 142,
	NPC_ROYAL_CHEF			= 143,
	NPC_ESLANT_WOMAN		= 144,
	NPC_FARMER				= 145,
	NPC_GATE_GUARD			= 148,
	NPC_ROYAL_ADVISOR		= 149,
	NPC_GATE2				= 150, // Doda camp gate
	NPC_ADELIA				= 153, // Goddess Adelia[event]
	NPC_BIFROST_MONUMENT	= 155,
	NPC_CHAOTIC_GENERATOR2	= 162, // newer type used by the Chaotic Generator
	NPC_SCARECROW			= 171, // official scarecrow byType
	NPC_KARUS_WARDER1		= 190,
	NPC_KARUS_WARDER2		= 191,
	NPC_ELMORAD_WARDER1		= 192,
	NPC_ELMORAD_WARDER2		= 193,
	NPC_KARUS_GATEKEEPER	= 198,
	NPC_ELMORAD_GATEKEEPER	= 199,
	NPC_CHAOS_STONE		 	= 200,
	NPC_PVP_MONUMENT		= 210,
	NPC_BATTLE_MONUMENT		= 211
};

enum ZoneAbilityType
{
	// these control neutrality-related settings client-side, 
	// including whether collision is enabled for other players.
	ZoneAbilityNeutral			= 0, // Players cannot attack each other, or NPCs. Can walk through players.
	ZoneAbilityPVP				= 1, // Players can attack each other, and only NPCs from the opposite nation. Cannot walk through players.
	ZoneAbilitySpectator		= 2, // player is spectating a 1v1 match (ZoneAbilityPVP is sent for the attacker)
	ZoneAbilitySiege1			= 3, // siege state 1 (unknown)
	ZoneAbilitySiege2			= 4, // siege state 2/4: if they have 0 NP & this is set, it will not show the message telling them to buy more.
	ZoneAbilitySiege3			= 5, // siege state 3 (unknown)
	ZoneAbilitySiegeDisabled	= 6, // CSW not running
	ZoneAbilityCaitharosArena	= 7, // Players can attack each other (don't seem to be able to anymore?), but not NPCs. Can walk through players.
	ZoneAbilityPVPNeutralNPCs	= 8 // Players can attack each other, but not NPCs. Cannot walk through players.
};

enum ZoneFlags
{
	ZF_TRADE_OTHER_NATION	= (1 << 0),
	ZF_TALK_OTHER_NATION	= (1 << 1),
	ZF_ATTACK_OTHER_NATION	= (1 << 2),
	ZF_ATTACK_SAME_NATION	= (1 << 3),
	ZF_FRIENDLY_NPCS		= (1 << 4),
	ZF_WAR_ZONE				= (1 << 5),
	ZF_CLAN_UPDATE			= (1 << 6)  // Joining, disbanding, creating etc.
};

// ITEM_SLOT DEFINE
const uint8 RIGHTEAR			= 0;
const uint8 HEAD				= 1;
const uint8 LEFTEAR				= 2;
const uint8 NECK				= 3;
const uint8 BREAST				= 4;
const uint8 SHOULDER			= 5;
const uint8 RIGHTHAND			= 6;
const uint8 WAIST				= 7;
const uint8 LEFTHAND			= 8;
const uint8 RIGHTRING			= 9;
const uint8 LEG					= 10;
const uint8 LEFTRING			= 11;
const uint8 GLOVE				= 12;
const uint8 FOOT				= 13;
const uint8 RESERVED			= 14;

const uint8 CWING				= 42;
const uint8 CHELMET				= 43;
const uint8 CLEFT				= 44;
const uint8 CRIGHT				= 45;
const uint8 CTOP				= 46;
const uint8 BAG1				= 47;
const uint8 BAG2				= 48;

const uint8 COSP_WINGS			= 0;
const uint8 COSP_HELMET			= 1;
const uint8 COSP_GLOVE			= 2;
const uint8 COSP_GLOVE2			= 3;
const uint8 COSP_BREAST			= 4;

const uint8 COSP_BAG1			= 5; // relative bag slot from cospre items
const uint8 COSP_BAG2			= 6; // relative bag slot from cospre items

const uint8 SLOT_MAX			= 14; // 14 equipped item slots
const uint8 HAVE_MAX			= 28; // 28 inventory slots
const uint8 COSP_MAX			= 5; // 5 cospre slots
const uint8 MBAG_COUNT			= 2; // 2 magic bag slots
const uint8 MBAG_MAX			= 12; // 12 slots per magic bag

// Total number of magic bag slots
#define MBAG_TOTAL			(MBAG_MAX * MBAG_COUNT)

// Start of inventory area
#define INVENTORY_INVENT	(SLOT_MAX)

// Start of cospre area
#define INVENTORY_COSP		(SLOT_MAX+HAVE_MAX)

// Start of magic bag slots (after the slots for the bags themselves)
#define INVENTORY_MBAG		(SLOT_MAX+HAVE_MAX+COSP_MAX+MBAG_COUNT)

// Start of magic bag 1 slots (after the slots for the bags themselves)
#define INVENTORY_MBAG1		(INVENTORY_MBAG)

// Start of magic bag 2 slots (after the slots for the bags themselves)
#define INVENTORY_MBAG2		(INVENTORY_MBAG+MBAG_MAX)

// Total slots in the general-purpose inventory storage
#define INVENTORY_TOTAL		(INVENTORY_MBAG2+MBAG_MAX)

const uint8 WAREHOUSE_MAX	= 192;
const uint8 MAX_MERCH_ITEMS	= 12;

#define MAX_MERCH_MESSAGE	40

const int ITEMCOUNT_MAX		= 9999;

#define MAX_KNIGHTS_MARK	2400
#define CLAN_SYMBOL_COST	5000000

#define NEWCHAR_SUCCESS						uint8(0)
#define NEWCHAR_NO_MORE						uint8(1)
#define NEWCHAR_INVALID_DETAILS				uint8(2)
#define NEWCHAR_EXISTS						uint8(3)
#define NEWCHAR_DB_ERROR					uint8(4)
#define NEWCHAR_INVALID_NAME				uint8(5)
#define NEWCHAR_BAD_NAME					uint8(6)
#define NEWCHAR_INVALID_RACE				uint8(7)
#define NEWCHAR_NOT_SUPPORTED_RACE			uint8(8)
#define NEWCHAR_INVALID_CLASS				uint8(9)
#define NEWCHAR_POINTS_REMAINING			uint8(10)
#define NEWCHAR_STAT_TOO_LOW				uint8(11)

enum ItemFlag
{
	ITEM_FLAG_NONE		= 0,
	ITEM_FLAG_RENTED	= 1,
	ITEM_FLAG_DUPLICATE = 3,
	ITEM_FLAG_SEALED	= 4,
	ITEM_FLAG_NOT_BOUND	= 7,
	ITEM_FLAG_BOUND		= 8
};

struct	_ITEM_DATA
{
	uint32		nNum;
	int16		sDuration;
	uint16		sCount;	
	uint8		bFlag; // see ItemFlag
	uint16		sRemainingRentalTime; // in minutes
	uint32		nExpirationTime; // in unix time
	uint64		nSerialNum;
	bool		IsSelling;

	INLINE bool isSealed() { return bFlag == ITEM_FLAG_SEALED; }
	INLINE bool isBound() { return bFlag == ITEM_FLAG_BOUND; }
	INLINE bool isRented() { return bFlag == ITEM_FLAG_RENTED; }
	INLINE bool isDuplicate() { return bFlag == ITEM_FLAG_DUPLICATE; }
};

enum HairData
{
	HAIR_R,
	HAIR_G,
	HAIR_B,
	HAIR_TYPE
};

struct _MERCH_DATA
{
	uint32 nNum;
	int16 sDuration;
	uint16 sCount;
	uint16 bCount;
	uint64 nSerialNum;
	uint32 nPrice;
	uint8 bOriginalSlot;
	bool IsSoldOut;
};

enum AuthorityTypes
{
	AUTHORITY_GAME_MASTER			= 0,
	AUTHORITY_PLAYER				= 1,
	AUTHORITY_MUTED					= 11,
	AUTHORITY_ATTACK_DISABLED		= 12,
	AUTHORITY_LIMITED_GAME_MASTER	= 250,
	AUTHORITY_BANNED				= 255
};

enum StatType
{
	STAT_STR = 0,
	STAT_STA = 1,
	STAT_DEX = 2,
	STAT_INT = 3, 
	STAT_CHA = 4, // MP
	STAT_COUNT
};

enum AttackResult
{
	ATTACK_FAIL					= 0,
	ATTACK_SUCCESS				= 1,
	ATTACK_TARGET_DEAD			= 2,
	ATTACK_TARGET_DEAD_OK		= 3,
	MAGIC_ATTACK_TARGET_DEAD	= 4
};

#define STAT_MAX 255
#define QUEST_ARRAY_SIZE	600 // That's a limit of 200 quests (3 bytes per quest)
#define QUEST_LIMIT			(QUEST_ARRAY_SIZE / 3)

enum InvisibilityType
{
	INVIS_NONE				= 0,
	INVIS_DISPEL_ON_MOVE	= 1,
	INVIS_DISPEL_ON_ATTACK	= 2
};

int32 myrand(int32 min, int32 max);
uint64 RandUInt64();

INLINE bool CheckPercent(short percent)
{
	if (percent < 0 || percent > 1000) 
		return false;

	return (percent > myrand(0, 1000));
}

INLINE time_t getMSTime()
{
#ifdef _WIN32
#if WINVER >= 0x0600
	typedef ULONGLONG (WINAPI *GetTickCount64_t)(void);
	static GetTickCount64_t pGetTickCount64 = nullptr;

	if (!pGetTickCount64)
	{
		HMODULE hModule = LoadLibraryA("KERNEL32.DLL");
		pGetTickCount64 = (GetTickCount64_t)GetProcAddress(hModule, "GetTickCount64");
		if (!pGetTickCount64)
			pGetTickCount64 = (GetTickCount64_t)GetTickCount;
		FreeLibrary(hModule);
	}

	return pGetTickCount64();
#else
	return GetTickCount();
#endif
#else
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (tv.tv_sec * SECOND) + (tv.tv_usec / SECOND);
#endif
}

INLINE void STRTOLOWER(std::string& str)
{
	for(size_t i = 0; i < str.length(); ++i)
		str[i] = (char)tolower(str[i]);
};

INLINE void STRTOUPPER(std::string& str)
{
	for(size_t i = 0; i < str.length(); ++i)
		str[i] = (char)toupper(str[i]);
};

#define foreach(itr, arr) \
	for (auto itr = arr.begin(); itr != arr.end(); itr++)

// ideally this guard should be scoped within the loop...
#define foreach_stlmap(itr, arr) \
	Guard _lock(arr.m_lock); \
	foreach_stlmap_nolock(itr, arr)

#define foreach_stlmap_nolock(itr, arr) \
	for (auto itr = arr.m_UserTypeMap.begin(); itr != arr.m_UserTypeMap.end(); itr++)

#define foreach_array(itr, arr) foreach_array_n(itr, arr, sizeof(arr) / sizeof(arr[0]))
#define foreach_array_n(itr, arr, len) for (auto itr = 0; itr < len; itr++)

#define foreach_region(x, z) for (int x = -1; x <= 1; x++) \
	for (int z = -1; z <= 1; z++)

