#pragma once

#define CONF_GAME_SERVER	"./GameServer.ini"

#define _UDP_PORT			8888
#define CLIENT_SOCKSIZE		100

#define MAX_NPC_SIZE		30

#define MAX_TYPE3_REPEAT	40
#define MAX_TYPE4_BUFF		50

#define MAX_ITEM			28

#define NPC_HAVE_ITEM_LIST	6
#define ZONEITEM_MAX		2100000000
#define COIN_MAX			2100000000
#define LOYALTY_MAX			2100000000

#define MAX_CLASS			26
#define MAX_LEVEL			80
#define MAX_PLAYER_HP		14000
#define MAX_DAMAGE			32000 // Game uses a signed 2 byte integer, so the limit is technically 32,767. The game, however, caps it at 32,000.

#define MAX_MESSAGE_EVENT		12
#define MAX_TEMPLE_EVENT_ROOM	50

// BBS Related
#define MAX_BBS_PAGE			22
#define MAX_BBS_MESSAGE			40
#define MAX_BBS_TITLE			20
#define MAX_BBS_POST			500

#define BUY_POST_PRICE			500
#define SELL_POST_PRICE			1000

#define REMOTE_PURCHASE_PRICE	5000
#define BBS_CHECK_TIME			36000

#define BATTLE 6

#define KARUS_ARRAY (KARUS - 1)
#define ELMORAD_ARRAY (ELMORAD - 1)
#define MONSTER_CHALLENGE_COUNT 32

// Attack Type
#define DIRECT_ATTACK		0
#define LONG_ATTACK			1
#define MAGIC_ATTACK		2
#define DURATION_ATTACK		3

enum InOutType
{
	INOUT_IN		= 1,
	INOUT_OUT		= 2,
	INOUT_RESPAWN	= 3,
	INOUT_WARP		= 4,
	INOUT_SUMMON	= 5
};

#define BLINK_TIME				(15) // in seconds
#define CLAN_SUMMON_TIME		(180)
#define PLAYER_IDLE_TIME		(300)

// About Map Object
#define USER_BAND				0
#define NPC_BAND				10000
#define INVALID_BAND			30000

#define EVENT_MONSTER			20

// Snow Event
#define SNOW_EVENT_MONEY		2000
#define SNOW_EVENT_SKILL		490077

// Temple Event
#define BIFROST_EVENT_COUNT				3
#define CHAOS_EVENT_COUNT				3
#define BORDER_DEFENSE_WAR_EVENT_COUNT	7
#define JURAD_MOUNTAIN_EVENT_COUNT		3

// Users under level 35 require 3,000 coins to shout.
#define SHOUT_COIN_REQUIREMENT 3000

// Battlezone Announcement
#define BATTLEZONE_OPEN					0x00
#define BATTLEZONE_CLOSE				0x01
#define DECLARE_WINNER					0x02
#define DECLARE_LOSER					0x03
#define DECLARE_BAN						0x04
#define KARUS_CAPTAIN_NOTIFY			0x05
#define ELMORAD_CAPTAIN_NOTIFY			0x06
#define KARUS_CAPTAIN_DEPRIVE_NOTIFY	0x07
#define ELMORAD_CAPTAIN_DEPRIVE_NOTIFY	0x08
#define SNOW_BATTLEZONE_OPEN			0x09
#define UNDER_ATTACK_NOTIFY				0x10
#define DECLARE_BATTLE_ZONE_STATUS		0x11
#define DECLARE_BATTLE_MONUMENT_STATUS	0x12
#define DECLARE_NATION_MONUMENT_STATUS	0x13
#define DECLARE_NATION_REWARD_STATUS	0x14

#define WAR_TIME_COUNT			3
#define WAR_ZONE_COUNT			WAR_TIME_COUNT
#define WAR_DAY_COUNT			7

// Battle define
#define NO_BATTLE				0
#define NATION_BATTLE			1
#define SNOW_BATTLE				2
#define CLAN_BATTLE				3

// Zone IDs
#define ZONE_KARUS				1
#define ZONE_ELMORAD			2
#define ZONE_KARUS_ESLANT		11
#define ZONE_ELMORAD_ESLANT		12
#define ZONE_MORADON			21
#define ZONE_DELOS				30
#define ZONE_BIFROST			31
#define ZONE_DESPERATION_ABYSS	32
#define ZONE_HELL_ABYSS			33
#define ZONE_DRAGON_CAVE		34
#define ZONE_ARENA				48
#define ZONE_ORC_ARENA			51
#define ZONE_BLOOD_DON_ARENA	52
#define ZONE_GOBLIN_ARENA		53
#define ZONE_CAITHAROS_ARENA	54
#define ZONE_FORGOTTEN_TEMPLE	55

#define ZONE_BATTLE_BASE		60

#define ZONE_BATTLE				ZONE_BATTLE_BASE + 1 // Napies Gorge
#define ZONE_BATTLE2			ZONE_BATTLE_BASE + 2 // Alseids Prairie
#define ZONE_BATTLE3			ZONE_BATTLE_BASE + 3 // Nieds Triangle
#define ZONE_BATTLE4			ZONE_BATTLE_BASE + 4 // Nereid's Island
#define ZONE_BATTLE5			ZONE_BATTLE_BASE + 5 // Zipang
#define ZONE_BATTLE6			ZONE_BATTLE_BASE + 6 // Oreads

#define ZONE_SNOW_BATTLE		69
#define ZONE_RONARK_LAND		71
#define ZONE_ARDREAM			72
#define ZONE_RONARK_LAND_BASE	73

#define ZONE_KROWAZ_DOMINION	75
#define ZONE_BORDER_DEFENSE_WAR	84
#define ZONE_CHAOS_DUNGEON		85
#define ZONE_JURAD_MOUNTAIN		87
#define ZONE_PRISON				92
#define ZONE_ISILOON_ARENA		93
#define ZONE_FELANKOR_ARENA		94

#define MAX_BATTLE_ZONE_USERS	150

// Zone level requirements (should really be in a database or something...)
#define MIN_LEVEL_NATION_BASE		1
#define MIN_LEVEL_ESLANT			40
#define MIN_LEVEL_BIFROST			35
#define MIN_LEVEL_WAR_ZONE			35
#define MIN_LEVEL_NIEDS_TRIANGLE	35
#define MAX_LEVEL_NIEDS_TRIANGLE	59
#define MIN_LEVEL_RONARK_LAND		35
#define MIN_LEVEL_ARDREAM			35
#define MAX_LEVEL_ARDREAM			59
#define MIN_LEVEL_RONARK_LAND_BASE	35
#define MAX_LEVEL_RONARK_LAND_BASE	69
#define MIN_LEVEL_KROWAZ_DOMINION	70
#define MIN_LEVEL_JURAD_MOUNTAIN	70

// Where to respawn after dying in the Moradon arenas
#define MINI_ARENA_RESPAWN_X		734
#define MINI_ARENA_RESPAWN_Z		427
#define MINI_ARENA_RESPAWN_RADIUS	5

// Where to Warp dodo or laon camp in the enemy nation zone
#define DODO_CAMP_WARP_X			1054
#define DODO_CAMP_WARP_Z			1141
#define LAON_CAMP_WARP_X			1012
#define LAON_CAMP_WARP_Z			914
#define DODO_LAON_WARP_RADIUS		5

// Bonus experience in War zones or PK zones
#define PVP_BONUS_EXP 10000

// Automatic Ranking Minutes
#define RELOAD_KNIGHTS_AND_USER_RATING 30 // Minute

// Where to respawn after dying in the Chaos Stone boss
#define CHAOS_STONE_MONSTER_RESPAWN_RADIUS 20
#define CHAOS_STONE_MONSTER_LIVE_TIME 900  // (15 minutes)

// Standard (pre-squared) range used for moradon mini arena.
#define RANGE_20M 20.0f

// Standard (pre-squared) range used for party rewards and such.
#define RANGE_50M (50.0f * 50.0f)

// Item IDs
#define ITEM_CONT_RECOVERY		800370000
#define ITEM_SCROLL_OF_IDENTITY	800032000
#define ITEM_NATION_TRANSFER    800360000
#define ITEM_GENDER_CHANGE		800560000
#define ITEM_JOB_CHANGE			800560000
#define ITEM_MEAT_DUMPLING		508216000
#define GOLDEN_MATTOCK			389135000
#define MATTOCK					389132000
#define MYSTERIOUS_ORE			399210000
#define MYSTERIOUS_GOLD_ORE		399200000
#define SLING					389043000
#define KING_SCEPTER			910074311
#define CHAOS_MAP				910246000
#define VOUCHER_OF_CHAOS		900106000
#define VOUCHER_OF_ORACLE		900184000
#define CERTIFICATE_OF_VICTORY	900017000
#define BORDER_SECURITY_SCROLL	900055000
#define RED_TREASURE_CHEST		379154000
#define GREEN_TREASURE_CHEST	379155000
#define BLUE_TREASURE_CHEST		379156000
#define ITEM_MONSTER_STONE		900144023
#define ITEM_LIGHT_PIT			700041000
#define ITEM_DRAIN_RESTORE		700040000
#define ITEM_KILLING_BLADE		700037000

// ID of the Seed quest, to be automatically started on first load.
#define STARTER_SEED_QUEST 500

#define MONUMENT_KARUS_SPID		14003
#define MONUMENT_ELMORAD_SPID	14004
#define MONUMENT_ENEMY_SPID		14005

#define ELMORAD_MONUMENT_SID		10301
#define ASGA_VILLAGE_MONUMENT_SID	10302
#define RAIBA_VILLAGE_MONUMENT_SID	10303
#define DODO_CAMP_MONUMENT_SID		10304

#define LUFERSON_MONUMENT_SID		20301
#define LINATE_MONUMENT_SID			20302
#define BELLUA_MONUMENT_SID			20303
#define LAON_CAMP_MONUMENT_SID		20304

#define SAW_BLADE_SSID				32153
#define CHAOS_CUBE_SSID				31527

enum UserStatus
{
	USER_STATUS_DOT		= 1,
	USER_STATUS_POISON	= 2,
	USER_STATUS_SPEED	= 3,
	USER_STATUS_BLIND	= 4,
	USER_STATUS_BLACK	= 5
};

enum UserStatusBehaviour
{
	USER_STATUS_CURE	= 0,
	USER_STATUS_INFLICT	= 1
};

enum LoyaltyType
{
	LOYALTY_NATIONAL_POINTS = 1,
	LOYALTY_MANNER_POINTS	= 2
};

enum AttributeType
{
	AttributeNone		= 0,
	AttributeFire		= 1,
	AttributeIce		= 2,
	AttributeLightning	= 3,
	AttributeLightMagic	= 4,
	AttributeCurse		= 5,
	AttributePosion		= 6
};

typedef union{
	uint16		w;
	uint8		b[2];
} MYSHORT;

typedef union{
	uint64		i;
	uint8		b[8];
} MYINT64;

#define TO_USER(v)	static_cast<CUser *>(v)
#define TO_NPC(v)	static_cast<CNpc *>(v)

#include "../shared/globals.h"
