#ifndef _GAMEDEFINE_H
#define _GAMEDEFINE_H

//#define __VERSION			1081
#define __VERSION			1068 //1097

//////////////////// 직업별 Define ////////////////////
#define KARUWARRRIOR		101		// 카루전사
#define KARUROGUE			102		// 카루로그
#define KARUWIZARD			103		// 카루마법
#define KARUPRIEST			104		// 카루사제
#define BERSERKER			105		// 버서커
#define GUARDIAN			106		// 가디언
#define HUNTER				107		// 헌터
#define PENETRATOR			108		// 페너트레이터
#define SORSERER			109		// 소서러
#define NECROMANCER			110		// 네크로맨서
#define SHAMAN				111		// 샤만
#define DARKPRIEST			112		// 다크프리스트

#define ELMORWARRRIOR		201		// 엘모전사
#define ELMOROGUE			202		// 엘모로그
#define ELMOWIZARD			203		// 엘모마법
#define ELMOPRIEST			204		// 엘모사제
#define BLADE				205		// 블레이드
#define PROTECTOR			206		// 프로텍터
#define RANGER				207		// 레인져
#define ASSASSIN			208		// 어쌔신
#define MAGE				209		// 메이지
#define ENCHANTER			210		// 엔첸터
#define CLERIC				211		// 클레릭
#define DRUID				212		// 드루이드
/////////////////////////////////////////////////////

/////////////////////////////////////////////////////
// Race Define
/////////////////////////////////////////////////////
#define KARUS_BIG			1
#define KARUS_MIDDLE		2
#define KARUS_SMALL			3
#define KARUS_WOMAN			4
#define BABARIAN			11
#define ELMORAD_MAN			12
#define ELMORAD_WOMAN		13

/////////////////////////////////////////////////////
// ITEM_SLOT DEFINE
const BYTE RIGHTEAR			= 0;
const BYTE HEAD				= 1;
const BYTE LEFTEAR			= 2;
const BYTE NECK				= 3;
const BYTE BREAST			= 4;
const BYTE SHOULDER			= 5;
const BYTE RIGHTHAND		= 6;
const BYTE WAIST			= 7;
const BYTE LEFTHAND			= 8;
const BYTE RIGHTRING		= 9;
const BYTE LEG				= 10;
const BYTE LEFTRING			= 11;
const BYTE GLOVE			= 12;
const BYTE FOOT				= 13;
const BYTE RESERVED			= 14;
/////////////////////////////////////////////////////

// 타격비별 성공률 //
#define GREAT_SUCCESS			0X01		// 대성공
#define SUCCESS					0X02		// 성공
#define NORMAL					0X03		// 보통
#define	FAIL					0X04		// 실패 

// Item Move Direction Define 
#define ITEM_INVEN_SLOT			0x01
#define ITEM_SLOT_INVEN			0x02
#define ITEM_INVEN_INVEN		0x03
#define ITEM_SLOT_SLOT			0x04
#define ITEM_INVEN_ZONE			0x05
#define ITEM_ZONE_INVEN			0x06

// Item Weapon Type Define
#define WEAPON_DAGGER			1
#define WEAPON_SWORD			2
#define WEAPON_AXE				3
#define WEAPON_MACE				4
#define WEAPON_SPEAR			5
#define WEAPON_SHIELD			6
#define WEAPON_BOW				7
#define WEAPON_LONGBOW			8
#define WEAPON_LAUNCHER			10
#define WEAPON_STAFF			11
#define WEAPON_ARROW			12	// 스킬 없음
#define WEAPON_JAVELIN			13	// 스킬 없음
#define WEAPON_WORRIOR_AC		21	// 스킬 없음
#define WEAPON_LOG_AC			22	// 스킬 없음
#define WEAPON_WIZARD_AC		23	// 스킬 없음
#define WEAPON_PRIEST_AC		24	// 스킬 없음
////////////////////////////////////////////////////////////
// User Status //
#define USER_STANDING			0X01		// 서 있다.
#define USER_SITDOWN			0X02		// 앉아 있다.
#define USER_DEAD				0x03		// 듀거떠
//#define USER_BLINKING			0x04		// 방금 살아났어!!!
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Magic State
#define NONE				0x01
#define CASTING				0x02
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
// Durability Type
#define ATTACK				0x01
#define DEFENCE				0x02
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

#define CHIEF				0x01	// 단장
#define VICECHIEF			0x02	// 부단장
#define TRAINEE				0x05	// 멤버
#define COMMAND_CAPTAIN		100		// 지휘권자
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// COMMUNITY TYPE DEFINE
#define CLAN_TYPE			0x01
#define KNIGHTS_TYPE		0x02
////////////////////////////////////////////////////////////

#define MAX_CLAN			24
#define MAX_KNIGHTS_BANK	200
#define MAX_KNIGHTS_MARK	512

#define ITEM_GOLD			900000000	// 돈 아이템 번호...
#define ITEM_NO_TRADE		900000001	// 거래 불가 아이템들.... 비러머글 크리스마스 이밴트 >.<		

////////////////////////////////////////////////////////////
// EVENT TYPE DEFINE
#define ZONE_CHANGE			0x01
#define ZONE_TRAP_DEAD		0x02
#define ZONE_TRAP_AREA		0x03

////////////////////////////////////////////////////////////
// EVENT MISCELLANOUS DATA DEFINE
#define ZONE_TRAP_INTERVAL	   1		// Interval is one second right now.
#define ZONE_TRAP_DAMAGE	   10		// HP Damage is 10 for now :)

////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// USER POINT DEFINE
#define STR					0x01
#define STA					0x02
#define DEX					0x03
#define INTEL				0x04
#define CHA					0x05

#define ORDER_SKILL			0x01
#define MANNER_SKILL		0X02
#define LANGUAGE_SKILL		0x03
#define BATTLE_SKILL		0x04
#define PRO_SKILL1			0x05
#define PRO_SKILL2			0x06
#define PRO_SKILL3			0x07
#define PRO_SKILL4			0x08

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
// ITEM LOG TYPE 
#define ITEM_MERCHANT_BUY			0x01
#define ITEM_MERCHANT_SELL			0x02
#define ITEM_MONSTER_GET			0x03
#define ITEM_EXCHANGE_PUT			0x04
#define ITEM_EXCHANGE_GET			0x05
#define ITEM_DESTROY				0x06
#define ITEM_WAREHOUSE_PUT			0x07
#define ITEM_WAREHOUSE_GET			0x08
#define ITEM_UPGRADE				0x09

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
#define ABNORMAL_NORMAL			0x01
#define ABNORMAL_GIANT			0x02
#define ABNORMAL_DWARF			0x03
#define ABNORMAL_BLINKING		0x04

//////////////////////////////////////////////////////////////
// Object Type
#define NORMAL_OBJECT		0
#define SPECIAL_OBJECT		1

//////////////////////////////////////////////////////////////
// REGENE TYPES
#define REGENE_NORMAL		0
#define REGENE_MAGIC		1
#define REGENE_ZONECHANGE	2

//////////////////////////////////////////////////////////////
// TYPE 3 ATTRIBUTE TYPES
#define ATTRIBUTE_FIRE				1
#define ATTRIBUTE_ICE				2
#define ATTRIBUTE_LIGHTNING			3

//extern CRITICAL_SECTION g_LogFile_critical;

struct _CLASS_COEFFICIENT
{
	short	sClassNum;
	char	strClassName[30];
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

struct _ZONE_ITEM {		// Bundle unit
	DWORD bundle_index;
	int itemid[6];
	short count[6];
	float x;
	float z;
	float y;
	float time;
};

struct	_ITEM_DATA
{
	int		nNum;		// item 번호
	short	sDuration;	// item 내구력
	short	sCount;		// item 갯수 or item 축복 속성에 해당 값
	__int64	nSerialNum;	// item serial code
};

struct	_EXCHANGE_ITEM
{
	int itemid;
	int count;
	short duration;
	BYTE pos;			//  교환후 들어갈 자리..
	__int64	nSerialNum;	// item serial code
};

struct _ITEM_TABLE
{
	int   m_iNum;				// item num
	char  m_strName[50];		// item Name
	BYTE  m_bKind;				// item 종류
	BYTE  m_bSlot;				// 장착위치
	BYTE  m_bRace;				// 사용 가능한 종족
	BYTE  m_bClass;				// 사용 가능한 Class
	short m_sDamage;			// 최대 타격치
	short m_sDelay;				// 공격시간
	short m_sRange;				// 범위(유효거리)
	short m_sWeight;			// 무게
	short m_sDuration;			// 내구성
	int	  m_iBuyPrice;			// 유저가 사는가격
	int	  m_iSellPrice;			// 유저가 파는가격
	short m_sAc;				// 방어력
	BYTE  m_bCountable;			// 개수 개념 아이템
	int	  m_iEffect1;			// 매직 이펙트1
	int	  m_iEffect2;			// 매직 이펙트2
	BYTE  m_bReqLevel;			// 요구 레벨
	BYTE  m_bReqRank;			// 요구 작위
	BYTE  m_bReqTitle;			// 요구 지위
	BYTE  m_bReqStr;			// 요구 힘
	BYTE  m_bReqSta;			// 요구 체력
	BYTE  m_bReqDex;			// 요구 민첩
	BYTE  m_bReqIntel;			// 요구 지능
	BYTE  m_bReqCha;			// 요구 매력
	BYTE  m_bSellingGroup;		// 상인 취급 물품
	BYTE  m_ItemType;			// 매직아이템 또는 레어아이템
	short m_sHitrate;			// 타격률
	short m_sEvarate;			// 회피율
	short m_sDaggerAc;			// 방어력1
	short m_sSwordAc;			// 방어력2
	short m_sMaceAc;			// 방어력3
	short m_sAxeAc;				// 방어력4
	short m_sSpearAc;			// 방어력5
	short m_sBowAc;				// 방어력6
	BYTE  m_bFireDamage;		// 불 속성
	BYTE  m_bIceDamage;			// 냉기 속성
	BYTE  m_bLightningDamage;	// 전격 속성
	BYTE  m_bPoisonDamage;		// 독 속성
	BYTE  m_bHPDrain;			// HP 흡수
	BYTE  m_bMPDamage;			// MP 타격
	BYTE  m_bMPDrain;			// MP 흡수
	BYTE  m_bMirrorDamage;		// 반사 타격
	BYTE  m_bDroprate;			// 드롭 비율
	BYTE  m_bStrB;				// 힘 보너스
	BYTE  m_bStaB;				// 체력 보너스
	BYTE  m_bDexB;				// 민첩성 보너스
	BYTE  m_bIntelB;			// 지능 보너스
	BYTE  m_bChaB;				// 매력 보너스
	short m_MaxHpB;				// MaxHP add
	short m_MaxMpB;				// MaxMP add
	BYTE  m_bFireR;				// 불 마법 저항력
	BYTE  m_bColdR;				// 얼음 마법 저항력
	BYTE  m_bLightningR;		// 전기 마법 저항력
	BYTE  m_bMagicR;			// 기타 마법 저항력
	BYTE  m_bPoisonR;			// 독 마법 저항력
	BYTE  m_bCurseR;			// 저주 마법 저항력
};

struct _USER_DATA
{
	char	m_id[MAX_ID_SIZE+1];				// 유저 ID
	char	m_Accountid[MAX_ID_SIZE+1];			// 계정 ID

	BYTE	m_bZone;						// 현재 Zone
	float	m_curx;							// 현재 X 좌표
	float	m_curz;							// 현재 Z 좌표
	float	m_cury;							// 현재 Y 좌표

	BYTE	m_bNation;						// 소속국가
	BYTE	m_bRace;						// 종족
	short	m_sClass;						// 직업
	BYTE	m_bHairColor;					// 머리색깔
	BYTE	m_bRank;						// 작위
	BYTE	m_bTitle;						// 지위
	BYTE	m_bLevel;						// 레벨
	int		m_iExp;							// 경험치
	int		m_iLoyalty;						// 로열티
	BYTE	m_bFace;						// 얼굴모양
	BYTE	m_bCity;						// 소속도시
	short	m_bKnights;						// 소속 기사단
	BYTE	m_bFame;						// 명성
	short	m_sHp;							// HP
	short	m_sMp;							// MP
	short	m_sSp;							// SP
	BYTE	m_bStr;							// 힘
	BYTE	m_bSta;							// 생명력
	BYTE	m_bDex;							// 공격, 회피율
	BYTE	m_bIntel;						// 지혜(?), 캐릭터 마법력 결정
	BYTE	m_bCha;							// 마법 성공률, 물건 가격 결정(?)
	BYTE	m_bAuthority;					// 유저 권한
	BYTE	m_bPoints;						// 보너스 포인트
	int		m_iGold;						// 캐릭이 지닌 돈(21억)
	short	m_sBind;						// Saved Bind Point
	int		m_iBank;						// 창고의 돈(21억)

	BYTE    m_bstrSkill[9];					// 직업별 스킬
	_ITEM_DATA m_sItemArray[HAVE_MAX+SLOT_MAX];		// 42*8 bytes
	_ITEM_DATA m_sWarehouseArray[WAREHOUSE_MAX];	// 창고 아이템	196*8 bytes

	BYTE	m_bLogout;						// 로그아웃 플래그
	BYTE	m_bWarehouse;					// 창고 거래 했었나?
	DWORD	m_dwTime;						// 플레이 타임...

};

struct _LEVELUP
{
	short	m_sLevel;
	int		m_iExp;
};

struct	_PARTY_GROUP
{
	WORD wIndex;
	short uid[8];		// 하나의 파티에 8명까지 가입가능
	short sMaxHp[8];
	short sHp[8];
	BYTE bLevel[8];
	short sClass[8];
	BYTE bItemRouting;
	_PARTY_GROUP() {
		for(int i=0;i<8;i++) {
			uid[i] = -1; sMaxHp[i] = 0; sHp[i] = 0; bLevel[i] = 0; sClass[i] = 0;
		}
		bItemRouting = 0;
	};
};

struct _OBJECT_EVENT
{
	BYTE byLife;			// 1:살아있다, 0:켁,, 죽음
	int sBelong;			// 소속
	short sIndex;			// 100 번대 - 카루스 바인드 포인트 | 200 번대 엘모라드 바인드 포인트 | 1100 번대 - 카루스 성문들 1200 - 엘모라드 성문들
	short sType;			// 0 - 바인드 포인트, 1 - 좌우로 열리는 성문, 2 - 상하로 열리는 성문, 3 - 레버, 4 - 깃발레버, 6:철창, 7-깨지는 부활비석
	short sControlNpcID;	// 조종할 NPC ID (조종할 Object Index), Type-> 5 : Warp Group ID
	short sStatus;			// status
	float fPosX;			// 위치값
	float fPosY;
	float fPosZ;
};

struct _REGENE_EVENT
{
	int	  sRegenePoint;		// 캐릭터 나타나는 지역 번호
	float fRegenePosX;		// 캐릭터 나타나는 지역의 왼아래쪽 구석 좌표 X
	float fRegenePosY;		// 캐릭터 나타나는 지역의 왼아래쪽 구석 좌표 Y
	float fRegenePosZ;		// 캐릭터 나타나는 지역의 왼아래쪽 구석 좌표 Z
	float fRegeneAreaZ;		// 캐릭터 나타나는 지역의 Z 축 길이 
	float fRegeneAreaX;		// 캐릭터 나타나는 지역의 X 축 길이
};

struct _KNIGHTS_USER
{
	BYTE    byUsed;								// 사용중 : 1, 비사용중 : 0
	char	strUserName[MAX_ID_SIZE+1];			// 캐릭터의 이름
};

struct _MAGIC_TABLE
{
	int		iNum;
	short	sFlyingEffect;
	BYTE	bMoral;
	BYTE	bSkillLevel;	
	short	sSkill;
	short	sMsp;
	short   sHP;
	BYTE	bItemGroup;
	int		iUseItem;
	BYTE	bCastTime;
	BYTE	bReCastTime;
	BYTE	bSuccessRate;
	BYTE	bType1;
	BYTE	bType2;
	short   sRange;
	BYTE	bEtc;
};

struct _MAGIC_TYPE8
{
	int     iNum;
	BYTE    bTarget;
	short   sRadius;
	BYTE    bWarpType;
	short   sExpRecover;
};

struct _MAGIC_TYPE5
{
	int		iNum;
	BYTE	bType;
	BYTE	bExpRecover;
	short	sNeedStone;
};

struct _MAGIC_TYPE4
{
	int     iNum;
	short   sMSP;
	BYTE    bBuffType;
	BYTE    bRadius;
	short   sDuration;
	BYTE    bAttackSpeed;
	BYTE    bSpeed;
	short   sAC;
	BYTE    bAttack;
	short   sMaxHP;
	BYTE    bHitRate;
	short   sAvoidRate;
	BYTE    bStr;
	BYTE    bSta;
	BYTE    bDex;
	BYTE    bIntel;
	BYTE    bCha;
	BYTE    bFireR;
	BYTE    bColdR;
	BYTE    bLightningR;
	BYTE    bMagicR;
	BYTE    bDiseaseR;
	BYTE    bPoisonR;
};

struct _MAGIC_TYPE3
{
	int		iNum;
	BYTE	bRadius;
	short	sAngle;
	short	sFirstDamage;
	short	sEndDamage;
	short	sTimeDamage;
	BYTE	bDirectType;
	short	sDuration;
	BYTE	bAttribute;
};

struct _MAGIC_TYPE2
{
	int     iNum;
	BYTE    bHitType;
	short   sHitRate;
	short	sAddDamage;
	short   sAddRange;
	BYTE    bNeedArrow;
};

struct _MAGIC_TYPE1
{
	int		iNum;
	BYTE	bHitType;
	short	sHitRate;
	short	sHit;
	BYTE	bDelay;
	BYTE	bComboType;
	BYTE	bComboCount;
	short	sComboDamage;
	short	sRange;
};

struct _ZONE_SERVERINFO
{
	short		sServerNo;
	short		sPort;
	char		strServerIP[20];
	_ZONE_SERVERINFO() {memset(strServerIP, 0x00, 20);};
};

struct _WARP_INFO
{
	short	sWarpID;
	char	strWarpName[32];
	char	strAnnounce[256];
	DWORD	dwPay;
	short	sZone;
	float	fX;
	float	fY;
	float	fZ;
	float	fR;

	_WARP_INFO() {
		sWarpID = 0; sZone = 0;
		fX = fZ = fY = fR = 0.0f;
		memset( strWarpName, 0x00, 32 ); memset( strAnnounce, 0x00, 256 );
	};
};

struct _HOME_INFO
{
	BYTE	bNation;
	float	ElmoZoneX;
	float	ElmoZoneZ;
	BYTE	ElmoZoneLX;
	BYTE	ElmoZoneLZ;
	float	KarusZoneX;
	float	KarusZoneZ;
	BYTE	KarusZoneLX;
	BYTE	KarusZoneLZ;
	float	FreeZoneX;
	float	FreeZoneZ;
	BYTE	FreeZoneLX;
	BYTE	FreeZoneLZ;
//
	float	BattleZoneX;
	float	BattleZoneZ;
	BYTE	BattleZoneLX;
	BYTE	BattleZoneLZ;
//
};

#endif