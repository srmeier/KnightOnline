#pragma once

class C3DMap;

#include "../shared/types.h"
#include "../shared/STLMap.h"
#include "GameDefine.h"
#include "Knights.h"
#include "Npc.h"

class CKingSystem;

typedef CSTLMap <C3DMap>					ZoneArray;
typedef std::map<int, int64>				LevelUpArray;
typedef CSTLMap <_CLASS_COEFFICIENT>		CoefficientArray;
typedef CSTLMap <_ITEM_TABLE>				ItemtableArray;
typedef CSTLMap <_MAGIC_TABLE>				MagictableArray;
typedef CSTLMap <_MAGIC_TYPE1>				Magictype1Array;
typedef CSTLMap <_MAGIC_TYPE2>				Magictype2Array;
typedef CSTLMap <_MAGIC_TYPE3>				Magictype3Array;
typedef CSTLMap	<_MAGIC_TYPE4>				Magictype4Array;
typedef CSTLMap <_MAGIC_TYPE5>				Magictype5Array;
typedef CSTLMap <_MAGIC_TYPE6>				Magictype6Array;
typedef CSTLMap <_MAGIC_TYPE7>				Magictype7Array;
typedef CSTLMap <_MAGIC_TYPE8>				Magictype8Array; 
typedef CSTLMap <_MAGIC_TYPE9>				Magictype9Array;
typedef CSTLMap <CNpc>						NpcArray;
typedef CSTLMap <_PARTY_GROUP>				PartyArray;
typedef CSTLMap <CKnights>					KnightsArray;
typedef CSTLMap <_KNIGHTS_RATING>			KnightsRatingArray;
typedef CSTLMap <_KNIGHTS_ALLIANCE>			KnightsAllianceArray;
typedef CSTLMap <_ZONE_SERVERINFO>			ServerArray;
typedef CSTLMap <_KNIGHTS_CAPE>				KnightsCapeArray;
typedef CSTLMap <_START_POSITION>			StartPositionArray;
typedef	CSTLMap	<_SERVER_RESOURCE>			ServerResourceArray;
typedef	CSTLMap	<_QUEST_HELPER>				QuestHelperArray;
typedef	CSTLMap	<_QUEST_MONSTER>			QuestMonsterArray;
typedef	CSTLMap	<_RENTAL_ITEM>				RentalItemArray;
typedef CSTLMap <_ITEM_EXCHANGE>			ItemExchangeArray;
typedef CSTLMap <_ITEM_UPGRADE>				ItemUpgradeArray;
typedef CSTLMap <_ITEM_OP>					ItemOpArray;
typedef CSTLMap <CKingSystem>				KingSystemArray;
typedef CSTLMap <_SET_ITEM>					SetItemArray;

typedef std::map<std::string, _USER_RANK *>			UserNameRankMap; 
typedef std::map<uint8, _USER_RANK *>				UserRankMap;

typedef std::vector<_QUEST_HELPER *>				QuestHelperList;
typedef std::map<uint16, QuestHelperList>			QuestNpcList;

typedef std::vector<_MONSTER_SUMMON_LIST>			MonsterSummonList;
typedef CSTLMap <MonsterSummonList>					MonsterSummonListArray;
typedef CSTLMap <_MONSTER_SUMMON_LIST_ZONE>			MonsterSummonListZoneArray;
typedef CSTLMap <_MONSTER_RESPAWN_LIST>				MonsterRespawnListArray;
typedef CSTLMap <_MONSTER_RESPAWN_LIST_INFORMATION>	MonsterRespawnListInformationArray;

typedef CSTLMap <_PREMIUM_ITEM>						PremiumItemArray;
typedef CSTLMap <_PREMIUM_ITEM_EXP>					PremiumItemExpArray;

typedef CSTLMap <_USER_RANKING>						UserRankingArray;
typedef std::map<std::string, _USER_DAILY_OP *>		UserDailyOpMap; 

typedef CSTLMap <_TEMPLE_EVENT_USER>				TempleEventUserArray;

typedef CSTLMap <_EVENT_TRIGGER>					EventTriggerArray;
typedef CSTLMap <_MONUMENT_INFORMATION>				NationMonumentInformationArray;
typedef CSTLMap <_MONSTER_CHALLENGE>				MonsterChallengeArray;
typedef CSTLMap <_MONSTER_CHALLENGE_SUMMON_LIST>	MonsterChallengeSummonListArray;
typedef CSTLMap <_START_POSITION_RANDOM>			StartPositionRandomArray;
typedef CSTLMap <_USER_ITEM>						UserItemArray;
typedef CSTLMap <_OBJECT_EVENT>						ObjectEventArray;
typedef CSTLMap <_KNIGHTS_SIEGE_WARFARE>			KnightsSiegeWarfareArray;