#ifndef __GAME_DEF_H_
#define __GAME_DEF_H_

#include <string>
#include "DInput.h"
const int CURRENT_VERSION = 1068; // 현재 버전

const float PACKET_INTERVAL_MOVE = 1.5f;				// 정기적으로 보내는 패킷 시간 간격..
const float PACKET_INTERVAL_ROTATE = 4.0f;
const float PACKET_INTERVAL_REQUEST_TARGET_HP = 2.0f;

// 단축키 지정해 놓은 부분..
enum eKeyMap {	KM_HOTKEY1 = DIK_1, 
				KM_HOTKEY2 = DIK_2, 
				KM_HOTKEY3 = DIK_3, 
				KM_HOTKEY4 = DIK_4, 
				KM_HOTKEY5 = DIK_5, 
				KM_HOTKEY6 = DIK_6, 
				KM_HOTKEY7 = DIK_7, 
				KM_HOTKEY8 = DIK_8, 
				KM_TOGGLE_RUN = DIK_T, 
				KM_TOGGLE_MOVE_CONTINOUS = DIK_E, 
				KM_TOGGLE_ATTACK = DIK_R, 
				KM_TOGGLE_SITDOWN = DIK_C, 
				KM_TOGGLE_INVENTORY = DIK_I, 
				KM_TOGGLE_SKILL = DIK_K, 
				KM_TOGGLE_STATE = DIK_U, 
				KM_TOGGLE_MINIMAP = DIK_M, 
				KM_TOGGLE_HELP = DIK_F1, 
				KM_CAMERA_CHANGE = DIK_F9, 
				KM_DROPPED_ITEM_OPEN = DIK_F, 
				KM_MOVE_FOWARD = DIK_W, 
				KM_MOVE_BACKWARD = DIK_S, 
				KM_ROTATE_LEFT = DIK_A, 
				KM_ROTATE_RIGHT = DIK_D, 
				KM_TARGET_NEARST_ENEMY = DIK_Z, 
				KM_TARGET_NEARST_PARTY = DIK_X, 
				KM_TARGET_NEARST_FRIEND = DIK_V, 
				KM_TARGET_PARTY1 = DIK_F1, 
				KM_TARGET_PARTY2 = DIK_F2, 
				KM_TARGET_PARTY3 = DIK_F3, 
				KM_TARGET_PARTY4 = DIK_F4, 
				KM_TARGET_PARTY5 = DIK_F5, 
				KM_TARGET_PARTY6 = DIK_F6, 
				KM_TARGET_PARTY7 = DIK_F7, 
				KM_TARGET_PARTY8 = DIK_F8 };

enum e_PlayerType { PLAYER_BASE = 0, PLAYER_NPC = 1, PLAYER_OTHER = 2, PLAYER_MYSELF = 3 };

enum e_Race {	RACE_ALL = 0,
				RACE_KA_ARKTUAREK = 1, RACE_KA_TUAREK = 2, RACE_KA_WRINKLETUAREK = 3, RACE_KA_PURITUAREK = 4, 
				RACE_EL_BABARIAN = 11, RACE_EL_MAN = 12, RACE_EL_WOMEN = 13,
				//RACE_KA_NORMAL = 11, RACE_KA_WARRIOR = 12, RACE_KA_ROGUE = 13, RACE_KA_MAGE = 14,
				RACE_NPC = 100,
				RACE_UNKNOWN = 0xffffffff };

enum e_Class {	CLASS_KINDOF_WARRIOR = 1, CLASS_KINDOF_ROGUE, CLASS_KINDOF_WIZARD, CLASS_KINDOF_PRIEST,
				CLASS_KINDOF_ATTACK_WARRIOR, CLASS_KINDOF_DEFEND_WARRIOR, CLASS_KINDOF_ARCHER, CLASS_KINDOF_ASSASSIN, 
				CLASS_KINDOF_ATTACK_WIZARD, CLASS_KINDOF_PET_WIZARD, CLASS_KINDOF_HEAL_PRIEST, CLASS_KINDOF_CURSE_PRIEST,

				CLASS_KA_WARRIOR = 101, CLASS_KA_ROGUE, CLASS_KA_WIZARD, CLASS_KA_PRIEST, // 여기까지 기본 직업
				CLASS_KA_BERSERKER = 105, CLASS_KA_GUARDIAN, CLASS_KA_HUNTER = 107, CLASS_KA_PENETRATOR, 
				CLASS_KA_SORCERER = 109, CLASS_KA_NECROMANCER, CLASS_KA_SHAMAN = 111, CLASS_KA_DARKPRIEST, 
				
				CLASS_EL_WARRIOR = 201, CLASS_EL_ROGUE, CLASS_EL_WIZARD, CLASS_EL_PRIEST, // 여기까지 기본 직업 
				CLASS_EL_BLADE = 205, CLASS_EL_PROTECTOR, CLASS_EL_RANGER = 207, CLASS_EL_ASSASIN, 
				CLASS_EL_MAGE = 209, CLASS_EL_ENCHANTER, CLASS_EL_CLERIC = 211, CLASS_EL_DRUID,
				
				CLASS_UNKNOWN = 0xffffffff };

enum e_Class_Represent { CLASS_REPRESENT_WARRIOR = 0, CLASS_REPRESENT_ROGUE, CLASS_REPRESENT_WIZARD, CLASS_REPRESENT_PRIEST, CLASS_REPRESENT_UNKNOWN = 0xffffffff };

const float WEAPON_WEIGHT_STAND_SWORD = 5.0f; // 무기의 무게 기준...칼
const float WEAPON_WEIGHT_STAND_AXE = 5.0f; // 무기의 무게 기준...도끼
const float WEAPON_WEIGHT_STAND_BLUNT = 8.0f; // 무기의 무게 기준...몽둥이 종류

enum e_Ani {	ANI_BREATH = 0, ANI_WALK, ANI_RUN, ANI_WALK_BACKWARD, ANI_STRUCK0, ANI_STRUCK1, ANI_STRUCK2, ANI_GUARD,
				ANI_DEAD_NEATLY = 8, ANI_DEAD_KNOCKDOWN, ANI_DEAD_ROLL, ANI_SITDOWN, ANI_SITDOWN_BREATH, ANI_STANDUP,
				ANI_ATTACK_WITH_WEAPON_WHEN_MOVE = 14, ANI_ATTACK_WITH_NAKED_WHEN_MOVE, 

				ANI_SPELLMAGIC0_A = 16, ANI_SPELLMAGIC0_B, 
				ANI_SPELLMAGIC1_A = 18, ANI_SPELLMAGIC1_B, 
				ANI_SPELLMAGIC2_A = 20, ANI_SPELLMAGIC2_B, 
				ANI_SPELLMAGIC3_A = 22, ANI_SPELLMAGIC3_B, 
				ANI_SPELLMAGIC4_A = 24, ANI_SPELLMAGIC4_B, 
				
				ANI_SHOOT_ARROW_A = 26, ANI_SHOOT_ARROW_B, 
				ANI_SHOOT_QUARREL_A = 28, ANI_SHOOT_QUARREL_B, 
				ANI_SHOOT_JAVELIN_A = 30, ANI_SHOOT_JAVELIN_B, 
				
				ANI_SWORD_BREATH_A = 32,	ANI_SWORD_ATTACK_A0, ANI_SWORD_ATTACK_A1,
				ANI_SWORD_BREATH_B,			ANI_SWORD_ATTACK_B0, ANI_SWORD_ATTACK_B1,		// 한손검
				
				ANI_DAGGER_BREATH_A = 38,	ANI_DAGGER_ATTACK_A0, ANI_DAGGER_ATTACK_A1,
				ANI_DAGGER_BREATH_B,		ANI_DAGGER_ATTACK_B0, ANI_DAGGER_ATTACK_B1,		// 단도
				
				ANI_DUAL_BREATH_A = 44,		ANI_DUAL_ATTACK_A0, ANI_DUAL_ATTACK_A1, 
				ANI_DUAL_BREATH_B,			ANI_DUAL_ATTACK_B0, ANI_DUAL_ATTACK_B1,			// 이도류
				
				ANI_SWORD2H_BREATH_A = 50,	ANI_SWORD2H_ATTACK_A0, ANI_SWORD2H_ATTACK_A1, 
				ANI_SWORD2H_BREATH_B,		ANI_SWORD2H_ATTACK_B0, ANI_SWORD2H_ATTACK_B1,	// 양손검
				
				ANI_BLUNT_BREATH_A = 56,	ANI_BLUNT_ATTACK_A0, ANI_BLUNT_ATTACK_A1, 
				ANI_BLUNT_BREATH_B,			ANI_BLUNT_ATTACK_B0, ANI_BLUNT_ATTACK_B1,		// 둔기 - 몽둥이 ?
				
				ANI_BLUNT2H_BREATH_A = 62,	ANI_BLUNT2H_ATTACK_A0, ANI_BLUNT2H_ATTACK_A1, 
				ANI_BLUNT2H_BREATH_B,		ANI_BLUNT2H_ATTACK_B0, ANI_BLUNT2H_ATTACK_B1,	// 양손으로 잡는 둔기. - 양손 도끼와 같다.
				
				ANI_AXE_BREATH_A = 68,		ANI_AXE_ATTACK_A0, ANI_AXE_ATTACK_A1, 
				ANI_AXE_BREATH_B,			ANI_AXE_ATTACK_B0, ANI_AXE_ATTACK_B1,			// 한손 도끼 
				
				ANI_SPEAR_BREATH_A = 74,	ANI_SPEAR_ATTACK_A0, ANI_SPEAR_ATTACK_A1, 
				ANI_SPEAR_BREATH_B,			ANI_SPEAR_ATTACK_B0, ANI_SPEAR_ATTACK_B1,		// 창 - 베는 날이 없는 그냥 창이다.
				
				ANI_POLEARM_BREATH_A = 80,	ANI_POLEARM_ATTACK_A0, ANI_POLEARM_ATTACK_A1, 
				ANI_POLEARM_BREATH_B,		ANI_POLEARM_ATTACK_B0, ANI_POLEARM_ATTACK_B1,	// 양손으로 잡는 날있는 창 - 청룡도 ??
				
				ANI_NAKED_BREATH_A = 86,	ANI_NAKED_ATTACK_A0, ANI_NAKED_ATTACK_A1, 
				ANI_NAKED_BREATH_B,			ANI_NAKED_ATTACK_B0, ANI_NAKED_ATTACK_B1,		// 맨몸으로 ??
				
				ANI_BOW_BREATH = 92,		ANI_CROSS_BOW_BREATH, ANI_LAUNCHER_BREATH, 
				ANI_BOW_BREATH_B,			ANI_BOW_ATTACK_B0, ANI_BOW_ATTACK_B1,			// 활 공격
				
				ANI_SHIELD_BREATH_A = 98,	ANI_SHIELD_ATTACK_A0, ANI_SHIELD_ATTACK_A1, 
				ANI_SHIELD_BREATH_B,		ANI_SHIELD_ATTACK_B0, ANI_SHIELD_ATTACK_B1,		// 방패 공격

				ANI_GREETING0 = 104, ANI_GREETING1, ANI_GREETING2, 
				ANI_WAR_CRY0 = 107, ANI_WAR_CRY1, ANI_WAR_CRY2, ANI_WAR_CRY3, ANI_WAR_CRY4, 

				ANI_SKILL_AXE0 = 112, ANI_SKILL_AXE1, ANI_SKILL_AXE2, ANI_SKILL_AXE3, 
				ANI_SKILL_DAGGER0 = 116, ANI_SKILL_DAGGER1,
				ANI_SKILL_DUAL0 = 118, ANI_SKILL_DUAL1,
				ANI_SKILL_BLUNT0 = 120, ANI_SKILL_BLUNT1, ANI_SKILL_BLUNT2, ANI_SKILL_BLUNT3, 
				ANI_SKILL_POLEARM0 = 124, ANI_SKILL_POLEARM1,
				ANI_SKILL_SPEAR0 = 126, ANI_SKILL_SPEAR1,
				ANI_SKILL_SWORD0 = 128, ANI_SKILL_SWORD1, ANI_SKILL_SWORD2, ANI_SKILL_SWORD3, 
				ANI_SKILL_AXE2H0 = 132, ANI_SKILL_AXE2H1,
				ANI_SKILL_SWORD2H0 = 134, ANI_SKILL_SWORD2H1,

				// 여기부터는 NPC Animation
				ANI_NPC_BREATH = 0, ANI_NPC_WALK, ANI_NPC_RUN, ANI_NPC_WALK_BACKWARD,
				ANI_NPC_ATTACK0 = 4, ANI_NPC_ATTACK1, ANI_NPC_STRUCK0, ANI_NPC_STRUCK1, ANI_NPC_STRUCK2, ANI_NPC_GUARD, 
				ANI_NPC_DEAD0 = 10, ANI_NPC_DEAD1, ANI_NPC_TALK0, ANI_NPC_TALK1, ANI_NPC_TALK2, ANI_NPC_TALK3, 
				ANI_NPC_SPELLMAGIC0 = 16, ANI_NPC_SPELLMAGIC1, 

				ANI_UNKNOWN = 0xffffffff };


//MAX_INCLINE_CLIMB = sqrt( 1 - sin(90-최대경사각)^2 )
//const	float MAX_INCLINE_CLIMB = 0.5f;	   // 올라갈수 있는 최대 경사값 = 30 도
const	float MAX_INCLINE_CLIMB = 0.6430f; // 올라갈수 있는 최대 경사값 = 40 도
//const	float MAX_INCLINE_CLIMB = 0.7071f; // 올라갈수 있는 최대 경사값 = 45 도
//const	float MAX_INCLINE_CLIMB = 0.7660f; // 올라갈수 있는 최대 경사값 = 50 도
//const	float MAX_INCLINE_CLIMB = 0.8660f; // 올라갈수 있는 최대 경사값 = 60 도


enum e_MoveDirection { MD_STOP, MD_FOWARD, MD_BACKWARD, MD_UNKNOWN = 0xffffffff };

const float MOVE_DELTA_WHEN_RUNNING = 3.0f; // 뛸때 곱해지는 변수..
const float MOVE_SPEED_WHEN_WALK = 1.5f; // Player 들이 걸을때 표준속도

// 현재 상태...
enum e_StateMove {	PSM_STOP = 0,
					PSM_WALK,
					PSM_RUN,
					PSM_WALK_BACKWARD,
					PSM_COUNT };

enum e_StateAction {	PSA_BASIC = 0,		// 아무짓도 안하고 있음...
						PSA_ATTACK,			// 공격 상태.. 
						PSA_GUARD,			// 방어성공 - 막음..
						PSA_STRUCK,			// 얻어터짐.
						PSA_DYING,			// 죽고 있는중(쓰러지는중)
						PSA_DEATH,			// 죽어서 뻗음..
						PSA_SPELLMAGIC,		// 마법 주문 외우는 중..
						PSA_SITDOWN, 		// 앉아 있는중...
						PSA_COUNT }; 

enum e_StateDying {		PSD_DISJOINT = 0,	// 분해되어 죽는다.. 
						PSD_KNOCK_DOWN,		// 날아가거나 뒤로 밀리면서 죽는다.
						PSD_KEEP_POSITION,	// 제 자리에서 폼잡고 죽는다..
						PSD_COUNT,

						PSD_UNKNOWN = 0xffffffff };

enum e_StateParty {	PSP_NORMAL = 0,
					PSP_POISONING = 1,
					PSP_CURSED = 2,
					PSP_MAGIC_TAKEN = 4,
					PSP_BLESSED = 8,
					PSP_UNKNOWN = 0xffffffff };

enum e_PartPosition	{	PART_POS_UPPER = 0,
						PART_POS_LOWER,
						PART_POS_FACE,
						PART_POS_HANDS,
						PART_POS_FEET, 
						PART_POS_HAIR_HELMET,
						PART_POS_COUNT,
						PART_POS_UNKNOWN = 0xffffffff };

enum e_PlugPosition {	PLUG_POS_RIGHTHAND = 0,
						PLUG_POS_LEFTHAND, 
						PLUG_POS_BACK, 
						PLUG_POS_KNIGHTS_GRADE, 
						PLUG_POS_COUNT,
						PLUG_POS_UNKNOWN = 0xffffffff };

/*
enum e_ItemClass	{	ITEM_CLASS_DAGGER = 1, // 단검(dagger)
						ITEM_CLASS_SWORD, // 2 : 한손검(onehandsword)
						ITEM_CLASS_SWORD_2H, // 3 : 양손검(twohandsword)
						ITEM_CLASS_AXE, // 4 : 한손도끼(onehandaxe)
						ITEM_CLASS_AXE_2H, // 5 : 두손도끼(twohandaxe)
						ITEM_CLASS_MACE, // 6 : 한손타격무기(mace)
						ITEM_CLASS_MACE_2H, // 7 : 두손타격무기(twohandmace)
						ITEM_CLASS_SPEAR, // 8 : 창(spear)
						ITEM_CLASS_POLEARM, // 9 : 폴암(polearm)
						
						ITEM_CLASS_SHIELD_SMALL = 11, // 11 : 스몰쉴드(smallshield)
						ITEM_CLASS_SHIELD_LARGE, // 12 : 라아지쉴드(largeshield)
						ITEM_CLASS_SHIELD_KITE, // 13 : 카이트쉴드(kiteshield)
						ITEM_CLASS_SHIELD_LARGETKITE, // 14 : 라아지카이트(largekite)
						ITEM_CLASS_SHIELD_PLATE, // 15 : 플레이트쉴드(plateshield)
						
						ITEM_CLASS_BOW_SHORT = 21, // 21 : 쇼트보우(Shortbow)
						ITEM_CLASS_BOW_WRAPT, // 22 : 랩트보우(wraptbow)
						ITEM_CLASS_BOW_COMPOSITE, // 23 : 콤포지트보우(compositebow)
						ITEM_CLASS_BOW_IRON, // 24 : 아이언보우(ironbow)
						ITEM_CLASS_BOW_LONG, // 25 : 롱보우(longbow)
						ITEM_CLASS_BOW_CROSS, // 28 : 크로스보우(crossbow)
						
						ITEM_CLASS_STAFF = 31, // 31 : 지팡이(staff)
						ITEM_CLASS_ETC, // 32 : 기타 마법 물품
						
						ITEM_CLASS_ARMOR_COTTON = 41, // 41 : 천방어구(cotton)
						ITEM_CLASS_ARMOR_FUR, // 42 : 털가죽(Fur)
						ITEM_CLASS_ARMOR_LEATHER, // 43 : 가죽방어구(leather)
						ITEM_CLASS_ARMOR_HADLEATHER, // 44 : 하드레더방어구(hardleather)
						ITEM_CLASS_ARMOR_RINGMAIL, // 45 : 링방어구(ringmail)
						ITEM_CLASS_ARMOR_SCALEMAIL, // 46 : 비늘방어구(scaledmail)
						ITEM_CLASS_ARMOR_HALFPLATE, // 47 : 하프 플레이트 방어구
						ITEM_CLASS_ARMOR_FULLPLATE, // 48 : 철판방어구(platemail)
						ITEM_CLASS_ROBE, // 49 : 마법사로브(robe)
						
						ITEM_CLASS_ARROW = 101,
						
						ITEM_CLASS_UNKNOWN = 0xffffffff }; // 101: 화살(arrow) 
*/
enum e_ItemAttrib	{
						ITEM_ATTRIB_GENERAL = 0,
						ITEM_ATTRIB_MAGIC	= 1,
						ITEM_ATTRIB_LAIR	= 2,
						ITEM_ATTRIB_CRAFT	= 3,
						ITEM_ATTRIB_UNIQUE	= 4,
						ITEM_ATTRIB_UPGRADE	= 5,		
						ITEM_ATTRIB_UNKNOWN = 0xffffffff };	

enum e_ItemClass	{	ITEM_CLASS_DAGGER = 11, // 단검(dagger)
						ITEM_CLASS_SWORD = 21, // 한손검(onehandsword)
						ITEM_CLASS_SWORD_2H = 22, // 3 : 양손검(twohandsword)
						ITEM_CLASS_AXE = 31, // 한손도끼(onehandaxe)
						ITEM_CLASS_AXE_2H = 32, // 두손도끼(twohandaxe)
						ITEM_CLASS_MACE = 41, // 한손타격무기(mace)
						ITEM_CLASS_MACE_2H = 42, // 두손타격무기(twohandmace)
						ITEM_CLASS_SPEAR = 51, // 창(spear)
						ITEM_CLASS_POLEARM = 52, // 폴암(polearm)
						
						ITEM_CLASS_SHIELD = 60, // 쉴드(shield)

						ITEM_CLASS_BOW = 70, //  쇼트보우(Shortbow)
						ITEM_CLASS_BOW_CROSS = 71, // 크로스보우(crossbow)
						ITEM_CLASS_BOW_LONG = 80, // 롱보우(longbow)

						ITEM_CLASS_EARRING = 91, // 귀걸이
						ITEM_CLASS_AMULET = 92, // 목걸이
						ITEM_CLASS_RING = 93, // 반지
						ITEM_CLASS_BELT = 94, // 허리띠
						ITEM_CLASS_CHARM = 95, //인벤토리에 지니고 있는 아이템
						ITEM_CLASS_JEWEL = 96, //보석종류
						ITEM_CLASS_POTION = 97, // 물약
						ITEM_CLASS_SCROLL = 98, // 스크롤

						ITEM_CLASS_LAUNCHER = 100, // 창던질때 쓰는 아이템..
						
						ITEM_CLASS_STAFF = 110, // 지팡이(staff)
						ITEM_CLASS_ARROW = 120, // 화살(Arrow)
						ITEM_CLASS_JAVELIN = 130, // 투창
						
						ITEM_CLASS_ARMOR_WARRIOR = 210, // 전사 방어구
						ITEM_CLASS_ARMOR_ROGUE = 220, // 로그 방어구
						ITEM_CLASS_ARMOR_MAGE = 230, // 마법사 방어구
						ITEM_CLASS_ARMOR_PRIEST = 240, // 사제 방어구

						ITEM_CLASS_ETC = 251, // 기타

						ITEM_CLASS_UNKNOWN = 0xffffffff }; // 

enum e_Nation { NATION_NOTSELECTED = 0, NATION_KARUS, NATION_ELMORAD, NATION_UNKNOWN = 0xffffffff };

struct __TABLE_ITEM_BASIC;
struct __TABLE_ITEM_EXT;
struct __TABLE_PLAYER;

enum e_Authority { AUTHORITY_MANAGER = 0, AUTHORITY_USER, AUTHORITY_NOCHAT, AUTHORITY_NPC, AUTHORITY_BLOCK_USER = 0xff };

struct __InfoPlayerBase
{
	int			iID;			// 고유 ID
	std::string szID;			// 이름
	D3DCOLOR	crID;			// 이름 색깔..
	e_Race		eRace;			// 캐릭터 골격에 따른 종족
	e_Nation	eNation;		// 소속 국가..
	e_Class		eClass;			// 직업
	int			iLevel;			// 레벨
	int			iHPMax;	
	int			iHP; 
	int			iAuthority;		// 권한 - 0 관리자, 1 - 일반유저, 255 - 블럭당한 유저...

	bool		bRenderID;		// 화면에 ID 를 찍는지..

	void Init()
	{
		iID = 0;					// 고유 ID
		szID = "";					// 이름
		crID = 0;					// 이름 색깔..
		eRace = RACE_UNKNOWN;		// 캐릭터 골격에 따른 종족
		eNation = NATION_UNKNOWN;	// 소속 국가..
		eClass = CLASS_UNKNOWN;		// 직업
		iLevel = 0;					// 레벨
		iHPMax = 0;	
		iHP = 0; 
		iAuthority = 1;				// 권한 - 0 관리자, 1 - 일반유저, 255 - 블럭당한 유저...
		bRenderID = true;
	}
};

struct __InfoPlayerOther
{
	int			iFace;			// 얼굴 모양..
	int			iHair;			// 머리카락 모양..

	int			iCity;			// 소속도시
	int			iKnightsID;		// 소속 기사단(길드) ID
	std::string szKnights;		// 소속 기사단(길드) 이름
	int			iKnightsGrade;	// 소속 기사단(길드) 등급
	int			iKnightsRank;	// 소속 기사단(길드) 순위

	int			iRank;			// 작위 - 백작, 공작 - 권한에 따른 분류
	int			iTitle;			// 직위 - 영주, 성주 - 단순한 신분 -> 길드장...

	void Init()
	{
		iFace = 0;			// 얼굴 모양..
		iHair = 0;			// 머리카락 모양..
		iCity;				// 소속도시
		iKnightsID = 0;		// 소속 기사단(길드)
		szKnights = "";		// 소속 기사단(길드)
		iKnightsGrade = 0;		// 소속 기사단(길드) 색깔
		iKnightsRank = 0;			// 작위 - 백작, 공작 - 권한에 따른 분류
		iTitle = 0;			// 직위 - 영주, 성주 - 단순한 신분 -> 길드장...
	}
};

// 기사단 직위..
enum e_KnightsDuty {	KNIGHTS_DUTY_UNKNOWN = 0,		// ????? 쫓겨남??
						KNIGHTS_DUTY_CHIEF = 1,			// 단장
						KNIGHTS_DUTY_VICECHIEF = 2,		// 부단장 
						KNIGHTS_DUTY_PUNISH = 3,		// 징계중.
						KNIGHTS_DUTY_TRAINEE = 4,		// 견습기사
						KNIGHTS_DUTY_KNIGHT = 5,		// 일반기사
						KNIGHTS_DUTY_OFFICER = 6		// 장교
					};

#define VICTORY_ABSENCE		0
#define VICTORY_KARUS		1
#define VICTORY_ELMORAD		2

struct __InfoPlayerMySelf : public __InfoPlayerOther
{
	int					iBonusPointRemain; // 남는 보너스 포인트...
	int					iLevelPrev; // 직전의 레벨...

	int					iMSPMax; 
	int					iMSP; 
			
	int					iTargetHPPercent;
	int					iGold;
	int					iExpNext;
	int					iExp; 
	int					iRealmPoint;		// 국가 기여도
	e_KnightsDuty		eKnightsDuty;		// 기사단 권한
	int					iWeightMax;			// 아이템의 총무게
	int					iWeight;			// 들수 있는 아이템 무게..
	int					iStrength;			// 힘
	int					iStrength_Delta;		// 아이템이나 마법에 의해 가감된값.
	int					iStamina;			// 스테미너
	int					iStamina_Delta;		// 아이템이나 마법에 의해 가감된값.
	int					iDexterity;			// 민첨성..
	int					iDexterity_Delta;	// 아이템이나 마법에 의해 가감된값.
	int					iIntelligence;		// 지능
	int					iIntelligence_Delta; // 아이템이나 마법에 의해 가감된값.
	int 				iMagicAttak;		// 마력
	int 				iMagicAttak_Delta;	// 아이템이나 마법에 의해 가감된값.
	
	int 				iAttack;		// 공격력
	int 				iAttack_Delta;	// 마법에 의해 가감된 값..
	int 				iGuard;			// 방어력
	int 				iGuard_Delta;	// 마법에 의해 가감된 값..

	int 				iRegistFire;			// 저항력
	int 				iRegistFire_Delta;		// 마법에 의한 저항력 변화값..
	int 				iRegistCold;			// 저항력
	int 				iRegistCold_Delta;		// 마법에 의한 저항력 변화값..
	int 				iRegistLight;			// 저항력
	int 				iRegistLight_Delta;		// 마법에 의한 저항력 변화값..
	int 				iRegistMagic;			// 저항력
	int 				iRegistMagic_Delta;		// 마법에 의한 저항력 변화값..
	int 				iRegistCurse;			// 저항력
	int 				iRegistCurse_Delta;		// 마법에 의한 저항력 변화값..
	int 				iRegistPoison;			// 저항력
	int 				iRegistPoison_Delta;	// 마법에 의한 저항력 변화값..

	int					iZoneInit;				// 서버한테 처음에 받은 존번호
	int					iZoneCur;				// 현재 존..
	int					iVictoryNation;			// 0: 무승부 1:엘모라드 승리 2:카루스 승리

	void Init()
	{
		__InfoPlayerOther::Init();

		iBonusPointRemain = 0; // 남는 보너스 포인트...
		iLevelPrev = 0; // 직전의 레벨...

		iMSPMax = 0; 
		iMSP = 0; 
		
		iTargetHPPercent = 0;
		iGold = 0;
		iExpNext = 0;
		iExp = 0; 
		iRealmPoint = 0;		// 국가 기여도
		eKnightsDuty = KNIGHTS_DUTY_UNKNOWN;		// 기사단 권한
		iWeightMax = 0;			// 아이템의 총무게
		iWeight = 0;			// 들수 있는 아이템 무게..
		iStrength = 0;			// 힘
		iStrength_Delta = 0;	// 아이템이나 마법에 의해 가감된값.
		iStamina = 0;			// 스테미너
		iStamina_Delta = 0;		// 아이템이나 마법에 의해 가감된값.
		iDexterity = 0;			// 민첨성..
		iDexterity_Delta = 0;	// 아이템이나 마법에 의해 가감된값.
		iIntelligence = 0;		// 지능
		iIntelligence_Delta = 0; // 아이템이나 마법에 의해 가감된값.
		iMagicAttak = 0;		// 마력
		iMagicAttak_Delta = 0;	// 아이템이나 마법에 의해 가감된값.
		
		iAttack = 0;		// 공격력
		iAttack_Delta = 0;	// 마법에 의해 가감된 값..
		iGuard = 0;			// 방어력
		iGuard_Delta = 0;	// 마법에 의해 가감된 값..

		iRegistFire = 0;			// 저항력
		iRegistFire_Delta = 0;		// 마법에 의한 저항력 변화값..
		iRegistCold = 0;			// 저항력
		iRegistCold_Delta = 0;		// 마법에 의한 저항력 변화값..
		iRegistLight = 0;			// 저항력
		iRegistLight_Delta = 0;		// 마법에 의한 저항력 변화값..
		iRegistMagic = 0;			// 저항력
		iRegistMagic_Delta = 0;		// 마법에 의한 저항력 변화값..
		iRegistCurse = 0;			// 저항력
		iRegistCurse_Delta = 0;		// 마법에 의한 저항력 변화값..
		iRegistPoison = 0;			// 저항력
		iRegistPoison_Delta = 0;	// 마법에 의한 저항력 변화값..

		iZoneInit = 0x01;			// 서버한테 처음에 받은 존번호
		iZoneCur = 0;				// 현재 존..
		iVictoryNation = -1;		// 전쟁에서 이긴 국가
	}
};

const int MAX_PARTY_OR_FORCE = 8;

struct __InfoPartyOrForce
{
	int			iID;			// 파티원 ID
	int			iLevel;			// Level
	e_Class		eClass;			// 직업
	int			iHP;			// Hit Point
	int			iHPMax;			// Hit Point Max
	bool		bSufferDown_HP;			// Status - HP 떨어짐...
	bool		bSufferDown_Etc;		// Status - 저주계열 영향을 받을때
	std::string szID;		// 파티 이름 문자열

	void Init()
	{
		iID = -1;
		iLevel = 0;
		eClass = CLASS_UNKNOWN;
		iHP = 0;
		iHPMax = 0;
		szID = "";

		bSufferDown_HP = false;			// Status - HP 떨어짐...
		bSufferDown_Etc = false;		// Status - 저주계열 영향을 받을때
	};

	__InfoPartyOrForce()
	{
		this->Init();
	}
};

enum e_PartyStatus { PARTY_STATUS_DOWN_HP = 1, PARTY_STATUS_DOWN_ETC = 2 };

struct __InfoPartyBBS // 파티 지원 게시판 구조체..
{
	std::string szID;			// 파티 이름 문자열
	int			iID;			// 파티원 ID
	int			iLevel;			// Level
	e_Class		eClass;			// 직업
	int			iMemberCount;

	void Init()
	{
		szID = "";
		iID = -1;
		iLevel = 0;
		eClass = CLASS_UNKNOWN;
		iMemberCount = 0;
	};

	__InfoPartyBBS()
	{
		this->Init();
	}
};

enum {	RESRC_UI_LOGIN = 0, RESRC_UI_CHARACTERSELECT, RESRC_UI_CHARACTERCREATE, RESRC_UI_INVENTORY, RESRC_UI_CHAT,
		RESRC_UI_MYINFORMATION, RESRC_UI_TARGET, RESRC_UI_HOTKEY, RESRC_UI_FUNCTION };

typedef struct __TABLE_ZONE
{
	DWORD		dwID; // zone ID
	std::string	szTerrainFN; // Terrain FileName - Terrain, Tile Map, ColorMap....
	std::string	szColorMapFN;
	std::string	szLightMapFN;
	std::string	szObjectPostDataFN; // Object 위치 정보 - 이안에 충돌 체크 데이터도 들어 있다.
	std::string	szMiniMapFN;
	std::string szSkySetting; // 하늘과 분위기 조정한 세팅 정보 파일..
	BOOL		bIndicateEnemyPlayer;	// 적국 플레이어를 표시하나??
	int			iFixedSundDirection;	// 해의 방향을 고정시키는지..
	std::string szLightObjFN;		// 지형에 배치되어 있는 라이트정보파일..
} TABLE_ZONE;

typedef struct __TABLE_UI_RESRC
{
	DWORD dwID;							// 플레이어의 소속 국가에 따른 키로 쓴다..

	std::string szLogIn;				// 0
	std::string szCmd;					// 1
	std::string szChat;					// 2
	std::string szMsgOutput;			// 3
	std::string szStateBar;				// 4
	std::string szVarious;				// 5 - 캐릭터 상태, 기사단 관리, 퀘스트, 친구.. 등등 네페이지가 한개에 들어있음.
	std::string szState;				// 6 - 캐릭터 상태
	std::string szKnights;				// 7 - 기사단 관리
	std::string szQuest;				// 8 - 퀘스트
	std::string szFriends;				// 9 - 친구 관리

	std::string szInventory;			// 10
	std::string szTransaction;			// 11
	std::string szDroppedItem;			// 12

	std::string szTargetBar;			// 13
	std::string szTargetSymbolShape;	// 14
	std::string szSkillTree;			// 15
	std::string szHotKey;				// 16

	std::string szMiniMap;				// 17
	std::string szPartyOrForce;			// 18
	std::string szPartyBBS;				// 19

	std::string szHelp;					// 20
	std::string szNotice;				// 21

	std::string szCharacterCreate;		// 22
	std::string szCharacterSelect;		// 23

	std::string szToolTip;				// 24
	std::string szMessageBox;			// 25
	std::string szLoading;				// 26
	std::string szItemInfo;				// 27
	std::string szPersonalTrade;		// 28
	std::string szPersonalTradeEdit;	// 29

	std::string szNpcEvent;				// 30
	std::string szZoneChangeOrWarp;		// 31
	std::string szExchangeRepair;		// 32

	std::string szRepairTooltip;		// 33
	std::string szNpcTalk;				// 34
	std::string szNpcExchangeList;		// 35

	std::string szKnightsOperation;		// 36
	std::string szClassChange;			// 37

	std::string szEndingDisplay;		// 38
	std::string szWareHouse;			// 39

	std::string szChangeClassInit;		// 40
	std::string szChangeInitBill;		// 41

	std::string szInn;					// 42
	std::string szInputClanName;		// 43
	std::string szTradeBBS;				// 44
	std::string szTradeBBSSelector;		// 45
	std::string szTradeExplanation;		// 46
	std::string szTradeMemolist;		// 47
	std::string szQuestMenu;			// 48
	std::string szQuestTalk;			// 49
	std::string szDead;					// 50
	std::string szElLoading;			// 51
	std::string szKaLoading;			// 52
	std::string szNationSelect;			// 53

} TABLE_UI_RESRC;

typedef struct __TABLE_ITEM_BASIC // 장착 아이템에 관한 리소스 레코드...
{
	DWORD		dwID;				// 00 코드화된 아이템번호 d - // 00 - Item 종류, 00 - Item 장착 위치(장착위치로 Plug 인지 Part 인지 판단이 가능하다.) - 0000 - ItemIndex
	BYTE 		byExtIndex;			// 01 확장 인덱스
	std::string	szName;				// 02 이름	
	std::string	szRemark;			// 03 아이템 설명	
	DWORD		dwIDResrc;			// 04 코드화된 아이템 리소스	d
	DWORD		dwIDIcon;			// 05 코드화된 아이템 아이콘 리소스	d
	DWORD		dwSoundID0;			// 06 Sound ID - 0 이면 사운드 없다~..
	DWORD		dwSoundID1;			// 07 Sound ID - 0 이면 사운드 없다~..

	BYTE	byClass;			// 08 아이템 형태	b enum e_ItemClass 참조....
	BYTE	byIsRobeType;		// 09 위아래가 통째로 된 로브타입의 아이템.....
	BYTE	byAttachPoint;		// 10 장착 위치	b
	BYTE	byNeedRace;			// 11 종족	b
	BYTE	byNeedClass;		// 12 직업	b

	short	siDamage;			// 13 무기타격	b
	short	siAttackInterval;	// 14 공격 시간 100 은 1 초
	short	siAttackRange;		// 15 유효 거리	.1 meter 단위
	short	siWeight;			// 16 무게 .1 중량단위
	short	siMaxDurability;	// 17 내구력
	int		iPrice;				// 18 매수가
	int		iPriceSale;			// 19 매도가
	short	siDefense;			// 20 방어력
	BYTE	byContable;			// 21 숫자가 있는 아이템인가??

	DWORD	dwEffectID1;		// 22 마법 효과 ID1
	DWORD	dwEffectID2;		// 23 마법 효과 ID1

	char	cNeedLevel;		// 24 요구 레벨		플레이어의 iLeve.. - 음수값이 있을수도 있다..
	BYTE	byNeedRank;			// 25 요구 작위		플레이어의 iRank..
	BYTE	byNeedTitle;		// 26 요구 타이틀 	플레이어의 iTitle..
	BYTE	byNeedStrength;		// 27 요구 힘 		플레이어의 iStrength..
	BYTE	byNeedStamina;		// 28 요구 정력 	플레이어의 iStamina..
	BYTE	byNeedDexterity;	// 29 요구 민첩		플레이어의 iDexterity..
	BYTE	byNeedInteli;		// 30 요구 지능		플레이어의 iIntelligence..
	BYTE	byNeedMagicAttack;	// 31 요구 마력		플레이어의 iMagicAttak..

	BYTE	bySellGroup;		// 32 상인이 파는데에 대한 그룹..
} TABLE_ITEM_BASIC;

const int MAX_ITEM_EXTENSION = 22; // 확장 아이템 테이블 갯수.
const int LIMIT_FX_DAMAGE = 64;
const int ITEM_UNIQUE = 4;
const int ITEM_LIMITED_EXHAUST = 17;

typedef struct __TABLE_ITEM_EXT // 장착 아이템에 관한 리소스 레코드...
{
	DWORD		dwID;				// 00 코드화된 아이템번호 - // 00 - Item 종류, 00 - Item 장착 위치(장착위치로 Plug 인지 Part 인지 판단이 가능하다.) - 0000 - ItemIndex
	std::string	szHeader;			// 01 접두사
	std::string	szRemark;			// 02 아이템 설명	
	BYTE		byMagicOrRare;		// 03 매직 혹은 레어 아이템인지...

	short	siDamage;				// 04 무기타격
	short	siAttackIntervalPercentage;		// 05 공격 시간 비율
	short	siHitRate;				// 06 타격률	- 퍼센트
	short	siEvationRate;			// 07 회피율	-   "

	short	siMaxDurability;		// 08 내구력
	short	siPriceMultiply;		// 09 매수가 배율
	short	siDefense;				// 10 방어력
	
	short	siDefenseRateDagger;	// 11 단검방어	- 퍼센트
	short	siDefenseRateSword;		// 12 검방어	- 퍼센트
	short	siDefenseRateBlow;		// 13 타격방어	- 퍼센트
	short	siDefenseRateAxe;		// 14 도끼방어	- 퍼센트
	short	siDefenseRateSpear;		// 15 창방어	- 퍼센트
	short	siDefenseRateArrow;		// 16 화살방어	- 퍼센트
	
	BYTE	byDamageFire;			// 17 추가데미지 - 불
	BYTE	byDamageIce;			// 18 추가데미지 - 얼음
	BYTE	byDamageThuner;			// 19 추가데미지 - 전격
	BYTE	byDamagePoison;			// 20 추가데미지 - 독

	BYTE	byStillHP;				// 21 HP 흡수
	BYTE	byDamageMP;				// 22 MP Damage
	BYTE	byStillMP;				// 23 MP 흡수
	BYTE	byReturnPhysicalDamage;	// 24 물리타격 반사

	BYTE	bySoulBind;				// 25소울 바인드 - 일대일시 아이템을 떨구는 정도 - 퍼센트 - 현재는 안쓴다.
	
	short	siBonusStr;				// 26 힘 보너스
	short	siBonusSta;				// 27 체력 보너스
	short	siBonusDex;				// 28 민첩성 보너스
	short	siBonusInt;				// 29 지능 보너스
	short	siBonusMagicAttak;		// 30 마력 보너스
	short	siBonusHP;				// 31 HP 보너스
	short	siBonusMSP;				// 32 MSP 보너스

	short	siRegistFire;			// 33 불 저항
	short	siRegistIce;			// 34 냉기 저항
	short	siRegistElec;			// 35 전기 저항
	short	siRegistMagic;			// 36 마법 저항
	short	siRegistPoison;			// 37 독 저항
	short	siRegistCurse;			// 38 저주 저항
	
	DWORD	dwEffectID1;		// 39 마법 효과 ID1
	DWORD	dwEffectID2;		// 40 마법 효과 ID2

	short	siNeedLevel;			// 41 요구 레벨		플레이어의 iLeve..
	short	siNeedRank;				// 42 요구 작위		플레이어의 iRank..
	short	siNeedTitle;			// 43 요구 타이틀 	플레이어의 iTitle..
	short	siNeedStrength;			// 44 요구 힘 		플레이어의 iStrength..
	short	siNeedStamina;			// 45 요구 정력 	플레이어의 iStamina..
	short	siNeedDexterity;		// 46 요구 민첩		플레이어의 iDexterity..
	short	siNeedInteli;			// 47 요구 지능		플레이어의 iIntelligence..
	short	siNeedMagicAttack;		// 48 요구 마력		플레이어의 iMagicAttak..
} TABLE_ITEM_EXT;

const int MAX_NPC_SHOP_ITEM = 30;
typedef struct __TABLE_NPC_SHOP
{
	DWORD		dwNPCID;
	std::string	szName;
	DWORD		dwItems[MAX_NPC_SHOP_ITEM];
} TABLE_NPC_SHOP;

enum e_ItemType { ITEM_TYPE_PLUG = 1, ITEM_TYPE_PART, ITEM_TYPE_ICONONLY, ITEM_TYPE_GOLD = 9, ITEM_TYPE_SONGPYUN = 10, ITEM_TYPE_UNKNOWN = 0xffffffff };

enum e_ItemPosition {	ITEM_POS_DUAL = 0,	ITEM_POS_RIGHTHAND, ITEM_POS_LEFTHAND,	ITEM_POS_TWOHANDRIGHT,	ITEM_POS_TWOHANDLEFT,
						ITEM_POS_UPPER = 5, ITEM_POS_LOWER,		ITEM_POS_HEAD,		ITEM_POS_GLOVES,		ITEM_POS_SHOES,
						ITEM_POS_EAR = 10,	ITEM_POS_NECK,		ITEM_POS_FINGER,	ITEM_POS_SHOULDER,		ITEM_POS_BELT,
						ITEM_POS_INVENTORY = 15, ITEM_POS_GOLD = 16, ITEM_POS_SONGPYUN = 17,
						ITEM_POS_UNKNOWN = 0xffffffff };
					
enum e_ItemSlot {	ITEM_SLOT_EAR_RIGHT = 0,	ITEM_SLOT_HEAD	= 1,	ITEM_SLOT_EAR_LEFT	= 2,
					ITEM_SLOT_NECK = 3,			ITEM_SLOT_UPPER	= 4,	ITEM_SLOT_SHOULDER	= 5,
					ITEM_SLOT_HAND_RIGHT = 6,	ITEM_SLOT_BELT	= 7,	ITEM_SLOT_HAND_LEFT = 8,
					ITEM_SLOT_RING_RIGHT = 9,	ITEM_SLOT_LOWER = 10,	ITEM_SLOT_RING_LEFT = 11,
					ITEM_SLOT_GLOVES = 12,		ITEM_SLOT_SHOES = 13, 
					ITEM_SLOT_COUNT = 14, ITEM_SLOT_UNKNOWN = 0xffffffff };


typedef struct __TABLE_PLAYER_LOOKS // NPC, Mob 모습 관한 리소스 레코드...
{
	DWORD		dwID; // NPC 고유 ID
	std::string	szName; // 캐릭터 이름
	std::string	szJointFN; // 관절 파일 이름
	std::string	szAniFN; // 에니메이션 파일 이름
	std::string	szPartFNs[7]; // 각 Character Part - 상체, 하체, 머리, 팔, 다리, 머리카락, 망토

	int			iJointRH;			// 오른손 끝 관절번호
	int			iJointLH;			// 왼손 끝 관절번호
	int			iJointLH2;			// 왼손 팔뚝 관절번호
	int			iJointCloak;		// 망토 붙을 관절 번호
	
	int			iSndID_Move;
	int			iSndID_Attack0;
	int			iSndID_Attack1;
	int			iSndID_Struck0;
	int			iSndID_Struck1;
	int			iSndID_Dead0;
	int			iSndID_Dead1;
	int			iSndID_Breathe0;
	int			iSndID_Breathe1;
	int			iSndID_Reserved0;
	int			iSndID_Reserved1;
} TABLE_PLAYER;

typedef struct __TABLE_EXCHANGE_QUEST
{
	DWORD		dwID;					// 퀘스트 번호..	0
	DWORD		dwNpcNum;				// npc 번호..	1
	std::string szDesc;					// 설명..		2
	int			iCondition0;			// 조건 1..		3
	int			iCondition1;			// 조건 2..		4
	int			iCondition2;			// 조건 3..		5
	int			iCondition3;			// 조건 4..		6
	int			iNeedGold;				// 요구 노아..	7	
	BYTE		bNeedLevel;				// 요구 레벨	8
	BYTE		bNeedClass;				// 요구 직업	9
	BYTE		bNeedRank;				// 요구 작위	10	
	BYTE		bNeedExtra1;			// 요구 여분1	11	
	BYTE		bNeedExtra2;			// 요구 여분2	12
	BYTE		bCreatePercentage;		// 생성 확률	13
	int			iArkTuarek;				// 아크 투아렉..	14	
	int			iTuarek;				// 투아렉..			15
	int			iRinkleTuarek;			// 링클 투아렉..	16
	int			iBabarian;				// 바바리안..		17
	int			iMan;					// 남자..			18
	int			iWoman;					// 여자..			19
} TABLE_EXCHANGE_QUEST;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//마법테이블...

typedef struct __TABLE_UPC_SKILL
{
	DWORD		dwID;				// SKILL 고유 ID
	std::string	szEngName;			// 스킬 영어 이름
	std::string	szName;				// 스킬 한글 이름
	std::string	szDesc;				// 스킬 이름
	int			iSelfAnimID1;		// 시전자 동작시작
	int			iSelfAnimID2;		// 시전자 동작끝

	int			idwTargetAnimID;	// 타겟 동작
	int			iSelfFX1;			// 시전자 효과 1
	int			iSelfPart1;			// 시전자 효과 일어나는 위치 1
	int			iSelfFX2;			// 시전자 효과 2
	int			iSelfPart2;			// 시전자 효과 일어나는 위치 2	
	int			iFlyingFX;			// 비행 효과
	int			iTargetFX;			// 타겟 효과

	int			iTargetPart;		// 효과가 일어날 위치.
	int			iTarget;			// 타겟	
	int			iNeedLevel;			// 요구 레벨
	int			iNeedSkill;			// 요구 스킬
	int			iExhaustMSP;		// 소모 MSP

	int			iExhaustHP;			// 소모 HP
	DWORD		dwNeedItem;			// 요구 아이템 e_ItemClass 참조.. 10을 나눈값이다..
	DWORD		dwExhaustItem;
	int			iCastTime;			// 캐스팅 시간
	int			iReCastTime;		// 다시 캐스팅할때까지 걸리는 시간.
	
	int			iPercentSuccess;	// 성공률
	DWORD		dw1stTableType;		// 첫번째 타입.
	DWORD		dw2ndTableType;		// 두번째 타입.
	int			iValidDist;			// 유효거리
	
} TABLE_UPC_ATTACK_B;

typedef struct __TABLE_UPC_SKILL_TYPE_1
{
	DWORD		dwID;			// SKILL 고유 ID
	int			iSuccessType;	// 성공타입.
	int			iSuccessRatio;	// 성공률
	int			iPower;			// 공격력
	int			iDelay;			// 딜레이
	int			iComboType;		// 콤보타입
	int			iNumCombo;		// 콤보회수
	int			iComboDamage;	// 콤보대미지			
	int			iValidAngle;	// 공격반경
	int			iAct[3];
} TABLE_UPC_SKILL_TYPE_1;

typedef struct __TABLE_UPC_SKILL_TYPE_2
{
	DWORD		dwID;			// SKILL 고유 ID
	int			iSuccessType;	// 성공타입.
	int			iPower;			// 공격력
	int			iAddDist;		// 거리증가
	int			iNumArrow;		// 화살요구수
} TABLE_UPC_SKILL_TYPE_2;

typedef struct __TABLE_UPC_SKILL_TYPE_3
{
	DWORD		dwID;			// SKILL 고유 ID
	int			iDDType;
	int			iStartDamage;
	int			iDuraDamage;
	int			iDurationTime;	// 지속시간
	int			iAttribute;
} TABLE_UPC_SKILL_TYPE_3;

typedef struct __TABLE_UPC_SKILL_TYPE_4
{
	DWORD		dwID;			// 일련번호
	int			iBuffType;		// 버프타입
	int			iDuration;
	int			iAttackSpeed;	// 공격속도
	int			iMoveSpeed;		// 이동속도
	int			iAC;			// 방어력
	int			iAttack;		// 공격력
	int			iMaxHP;			// MAXHP
	int			iStr;			// 힘
	int			iSta;			// 체력
	int			iDex;			// 민첩
	int			iInt;			// 지능
	int			iMAP;			// 마력
	int			iFireResist;	// 불저항
	int			iColdResist;	// 냉기저항
	int			iLightningResist;// 전기저항
	int			iMagicResist;	// 마법저항
	int			iDeseaseResist;	// 저주저항
	int			iPoisonResist;	// 독저항
} TABLE_UPC_SKILL_TYPE_4;

typedef struct __TABLE_UPC_SKILL_TYPE_5
{
	DWORD		dwID;			// 일련번호
	DWORD		dwTarget;		// 타겟
	int			iSuccessRatio;	// 성공률
	int			iValidDist;		// 유효거리
	int			iRadius;		// 반경
	float		fCastTime;		// 캐스팅타임
	float		fRecastTime;	// 리캐스팅타임
	int			iDurationTime;	// 지속시간
	DWORD		dwExhaustItem;	// 소모아이템
	DWORD		dwFX;			// 마법효과
} TABLE_UPC_SKILL_TYPE_5;

typedef struct __TABLE_UPC_SKILL_TYPE_6
{
	DWORD		dwID;			// 일련번호
	DWORD		dwTarget;		// 타겟
	int			iSuccessRatio;	// 성공률
	int			iValidDist;		// 유효거리
	int			iRadius;		// 반경
	float		fCastTime;		// 캐스팅타임
	float		fRecastTime;	// 리캐스팅타임
	int			iDurationTime;	// 지속시간
	DWORD		dwExhaustItem;	// 소모아이템
	DWORD		dwFX;			// 마법효과
	DWORD		dwTranform;		// 변신
} TABLE_UPC_SKILL_TYPE_6;

typedef struct __TABLE_UPC_SKILL_TYPE_7
{
	DWORD		dwID;			// 일련번호
	DWORD		dwTarget;		// 타겟
	DWORD		dwValidGroup;	// 유효그룹
	int			iSuccessRatio;	// 성공률
	int			iValidDist;		// 유효거리
	int			iRadius;		// 반경
	float		fCastTime;		// 캐스팅타임
	float		fRecastTime;	// 리캐스팅타임
	int			iDurationTime;	// 지속시간
	DWORD		dwExhaustItem;	// 소모아이템
	DWORD		dwFX;			// 마법효과	
} TABLE_UPC_SKILL_TYPE_7;

typedef struct __TABLE_UPC_SKILL_TYPE_8
{
	DWORD		dwID;			// 일련번호
	DWORD		dwTarget;		// 타겟
	int			iRadius;		// 반경
	DWORD		dwWarpType;		// 순간이동타임
	float		fRefillEXP;		// 경험치회복
	DWORD		dwZone1;		// 존넘버1
	DWORD		dwZone2;		// 존넘버2
	DWORD		dwZone3;		// 존넘버3
	DWORD		dwZone4;		// 존넘버4
	DWORD		dwZone5;		// 존넘버5
} TABLE_UPC_SKILL_TYPE_8;

typedef struct __TABLE_UPC_SKILL_TYPE_9
{
	DWORD		dwID;			// 일련번호
	DWORD		dwTarget;		// 타겟
	int			iSuccessRatio;	// 성공률
	int			iValidDist;		// 유효거리
	int			iRadius;		// 반경
	float		fCastTime;		// 캐스팅타임
	float		fRecastTime;	// 리캐스팅타임
	int			iDurationTime;	// 지속시간
	DWORD		dwExhaustItem;	// 소모아이템
	DWORD		dwAttr;			// 속성
	int			iDamage;		// 대미지	
} TABLE_UPC_SKILL_TYPE_9;

typedef struct __TABLE_UPC_SKILL_TYPE_10
{
	DWORD		dwID;			// 일련번호
	DWORD		dwTarget;		// 타겟
	int			iSuccessRatio;	// 성공률
	int			iValidDist;		// 유효거리
	int			iRadius;		// 반경
	float		fCastTime;		// 캐스팅타임
	float		fRecastTime;	// 리캐스팅타임
	DWORD		dwExhaustItem;	// 소모아이템
	DWORD		dwRemoveAttr;	// 제거속성	
} TABLE_UPC_SKILL_TYPE_10;

//마법테이블...
///////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct __TABLE_QUEST_MENU
{
	DWORD		dwID;		// 일련번호
	std::string szMenu;		// 선택 메뉴
} TABLE_QUEST_MENU;

typedef struct __TABLE_QUEST_TALK
{
	DWORD		dwID;		// 일련번호
	std::string szTalk;		// 퀘스트 지문
} TABLE_QUEST_TALK;

const int MAX_ITEM_SLOT_OPC = 8; // 착용 아이템 - 다른 플레이어(NPC 포함) 0 ~ 4 상체,하체,헬멧,팔,발 5 망토 6 오른손 7 왼손
const int MAX_ITEM_INVENTORY = 28;		// 소유 아템 MAX (인벤토리창)
const int MAX_ITEM_TRADE = 24;	// 상인과 거래..
const int MAX_ITEM_TRADE_PAGE = 12;
const int MAX_ITEM_WARE_PAGE = 8;
const int MAX_ITEM_PER_TRADE = 12;	// 개인과 거래..	
const int MAX_ITEM_BUNDLE_DROP_PIECE = 6;
const int MAX_ITEM_EX_RE_NPC = 4; // 교환, 수리창 NPC 영역..

const int MAX_SKILL_FROM_SERVER = 9;		// 서버에게서 받는 정보 슬롯 갯수..			

const int MAX_SKILL_KIND_OF = 5;			// Base Skill 1개, 전문 스킬 4개..			
const int MAX_SKILL_IN_PAGE = 8;			// 한 페이지 내의 아이콘 갯수..				
const int MAX_SKILL_PAGE_NUM = 3;			// 스킬 한개가 가지는 최대 페이지수..		


const int MAX_SKILL_HOTKEY_PAGE = 8;		// Hot Key의 총 페이지 수.. 				
const int MAX_SKILL_IN_HOTKEY = 8;			// Hot Key의 현 페이지내의 갯수..			
		
const int MAX_AVAILABLE_CHARACTER = 3;		// 한 서버당 선택할수 있는 최대 캐릭터 수..	

// 싸운도.... By ecli666
const int ID_SOUND_ITEM_ETC_IN_INVENTORY	= 2000;
const int ID_SOUND_ITEM_IN_REPAIR			= 2001;
const int ID_SOUND_ITEM_WEAPON_IN_INVENTORY = 2002;
const int ID_SOUND_ITEM_ARMOR_IN_INVENTORY	= 2003;
const int ID_SOUND_GOLD_IN_INVENTORY		= 3000;
const int ID_SOUND_SKILL_THROW_ARROW		= 5500;
const int ID_SOUND_BGM_TOWN					= 20000;
const int ID_SOUND_BGM_KA_BATTLE			= 20002;
const int ID_SOUND_BGM_EL_BATTLE			= 20003;
const int ID_SOUND_CHR_SELECT_ROTATE		= 2501;

const float SOUND_RANGE_TO_SET = 10.0f;
const float SOUND_RANGE_TO_RELEASE = 20.0f;

const float STUN_TIME = 3.0f;

enum e_Behavior {	BEHAVIOR_NOTHING = 0,
					BEHAVIOR_EXIT,			// 프로그램 끝내기..
					BEHAVIOR_RESTART_GAME,	// 게임 다시 시작하기(캐릭터 선택)
					BEHAVIOR_REGENERATION,	// 부활
					BEHAVIOR_CANCEL,		// 취소.. 현재는 아이템 거래 신청시 유저가 취소를 눌렀을때..

					BEHAVIOR_PARTY_PERMIT,	// 상대방이 파티가입 요청시 들어가는것을 허락.
					BEHAVIOR_PARTY_DISBAND, // 파티 탈퇴..
					BEHAVIOR_FORCE_PERMIT,	// 상대방이 부대가입 요청시 들어가는것을 허락.
					BEHAVIOR_FORCE_DISBAND, // 부대 탈퇴..

					BEHAVIOR_REQUEST_BINDPOINT, // 바인딩 포인트로 ... 

					BEHAVIOR_DELETE_CHR,

					BEHAVIOR_KNIGHTS_CREATE,
					BEHAVIOR_KNIGHTS_DESTROY, // 기사단 해체..
					BEHAVIOR_KNIGHTS_WITHDRAW, // 기사단 해체..

					BEHAVIOR_PERSONAL_TRADE_FMT_WAIT,	// 개인거래.. 내가 신청한 경우..
					BEHAVIOR_PERSONAL_TRADE_PERMIT,		// 개인거래.. 내가 신청받은 경우..

					BEHAVIOR_MGAME_LOGIN,
					
					BEHAVIOR_CLAN_JOIN,
					BEHAVIOR_PARTY_BBS_REGISTER,		// 파티 게시판에 등록 
					BEHAVIOR_PARTY_BBS_REGISTER_CANCEL, // 파티 게시판에 등록 해제

					BEHAVIOR_EXECUTE_OPTION,			// 게임 종료하고 옵션 실행..
				
					BEHAVIOR_UNKNOWN = 0xffffffff
				};

enum e_SkillMagicTaget	{	SKILLMAGIC_TARGET_SELF = 1,				// 나 자신..
							SKILLMAGIC_TARGET_FRIEND_WITHME = 2,	// 나를 포함한 우리편(국가) 중 하나 ..
							SKILLMAGIC_TARGET_FRIEND_ONLY = 3,		// 나를 뺀 우리편 중 하나 
							SKILLMAGIC_TARGET_PARTY = 4,			// 나를 포함한 우리파티 중 하나..
							SKILLMAGIC_TARGET_NPC_ONLY = 5,			// NPC중 하나.
							SKILLMAGIC_TARGET_PARTY_ALL = 6,		// 나를 호함한 우리파티 모두..
							SKILLMAGIC_TARGET_ENEMY_ONLY = 7,		// 울편을 제외한 모든 적중 하나(NPC포함)
							SKILLMAGIC_TARGET_ALL = 8,				// 겜상에 존재하는 모든 것중 하나.
							
							SKILLMAGIC_TARGET_AREA_ENEMY = 10,		// 그 지역에 있는 적들...
							SKILLMAGIC_TARGET_AREA_FRIEND = 11,		// 그 지역에 있는 울편.
							SKILLMAGIC_TARGET_AREA_ALL = 12,		// 그 지역에 있는 모두들.
							SKILLMAGIC_TARGET_AREA = 13,			// 나를 중심으로 한 지역
							SKILLMAGIC_TARGET_DEAD_FRIEND_ONLY = 25,	//죽은 사람중 나를 뺀 우리편 중 하나.
							
							SKILLMAGIC_TARGET_UNKNOWN = 0xffffffff
						};


//define fx...
typedef struct __TABLE_FX	// FX 리소스 레코드...
{
	DWORD		dwID;		// 고유 ID
	std::string	szFN;		// file name
	DWORD		dwSoundID;	// 효과에 쓰는 사운드 아디.
} TABLE_FX;

const int	MAX_COMBO = 3;

const int	FXID_BLOOD = 10002;
const int	FXID_LEVELUP_KARUS			= 10012;
const int	FXID_LEVELUP_ELMORAD		= 10018;
const int	FXID_REGEN_ELMORAD			= 10019;
const int	FXID_REGEN_KARUS			= 10020;
const int	FXID_SWORD_FIRE_MAIN		= 10021;
const int	FXID_SWORD_FIRE_TAIL		= 10022;
const int	FXID_SWORD_FIRE_TARGET		= 10031;
const int	FXID_SWORD_ICE_MAIN			= 10023;
const int	FXID_SWORD_ICE_TAIL			= 10024;
const int	FXID_SWORD_ICE_TARGET		= 10032;
const int	FXID_SWORD_LIGHTNING_MAIN	= 10025;
const int	FXID_SWORD_LIGHTNING_TAIL	= 10026;
const int	FXID_SWORD_LIGHTNING_TARGET = 10033;
const int	FXID_SWORD_POISON_MAIN		= 10027;
const int	FXID_SWORD_POISON_TAIL		= 10028;
const int	FXID_SWORD_POISON_TARGET	= 10034;
//const int	FXID_GROUND_TARGET = 10035;
const int	FXID_REGION_TARGET_EL_ROGUE		= 10035;
const int	FXID_REGION_TARGET_EL_WIZARD	= 10036;
const int	FXID_REGION_TARGET_EL_PRIEST	= 10037;
const int	FXID_REGION_TARGET_KA_ROGUE		= 10038;
const int	FXID_REGION_TARGET_KA_WIZARD	= 10039;
const int	FXID_REGION_TARGET_KA_PRIEST	= 10040;
const int	FXID_CLAN_RANK_1				= 10041;
const int	FXID_REGION_POISON				= 10100;

//define skillmagic_type4_bufftype
enum e_SkillMagicType4	{	BUFFTYPE_MAXHP = 1,				//MaxHP변화..
							BUFFTYPE_AC = 2,				//AC변화..
							BUFFTYPE_RESIZE = 3,			//캐릭터 크기 조정..
							BUFFTYPE_ATTACK = 4,			//공격력..
							BUFFTYPE_ATTACKSPEED = 5,		//공격스피드..
							BUFFTYPE_SPEED = 6,				//이동속도..
							BUFFTYPE_ABILITY = 7,			//다섯가지 능력(str, sta, cha, dex int)
							BUFFTYPE_RESIST = 8,			//다섯가지 저항력..
							BUFFTYPE_HITRATE_AVOIDRATE = 9,	//hitrate n avoidrate
							BUFFTYPE_TRANS = 10,			//변신, 투명..
							BUFFTYPE_SLEEP = 11,			//잠재우기..
							BUFFTYPE_EYE = 12				//시력관련...							
};

enum e_SkillMagicType3	{	DDTYPE_TYPE3_DUR_OUR = 100,
							DDTYPE_TYPE3_DUR_ENEMY = 200
};



enum e_SkillMagicFailMsg {	SKILLMAGIC_FAIL_CASTING	= -100,		//캐스팅 실패..
							SKILLMAGIC_FAIL_KILLFLYING = -101,	//Flying객체 죽여..
							SKILLMAGIC_FAIL_ENDCOMBO = -102,	//combo 동작 끝났다.
							SKILLMAGIC_FAIL_NOEFFECT = -103,	//Effecting 무산...
							SKILLMAGIC_FAIL_ATTACKZERO = -104,	//타격치 0...(mp는 소모하고 타격실패..)

							SKILLMAGIC_FAIL_UNKNOWN = 0xffffffff
};

enum e_ObjectType	{	OBJECT_TYPE_BINDPOINT,
						OBJECT_TYPE_DOOR_LEFTRIGHT,
						OBJECT_TYPE_DOOR_TOPDOWN,
						OBJECT_TYPE_LEVER_TOPDOWN,
						OBJECT_TYPE_FLAG,
						OBJECT_TYPE_WARP_POINT,
						OBJECT_TYPE_UNKNOWN = 0xffffffff
					};

//definitions related clan....
const int	CLAN_LEVEL_LIMIT	= 20;
const int	CLAN_COST			= 500000;
const DWORD KNIGHTS_FONT_COLOR	= 0xffff0000; // 기사단(클랜)이름 폰트 컬러..

enum e_Cursor		{	CURSOR_ATTACK,
						CURSOR_EL_NORMAL,
						CURSOR_EL_CLICK,
						CURSOR_KA_NORMAL,
						CURSOR_KA_CLICK,
						CURSOR_PRE_REPAIR,
						CURSOR_NOW_REPAIR,
						CURSOR_COUNT,
						CURSOR_UNKNOWN = 0xffffffff
					};

#endif // end of #define __GAME_DEF_H_

