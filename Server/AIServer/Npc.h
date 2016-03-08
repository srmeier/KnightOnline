#pragma once

#include "NpcTable.h"
#include "NpcMagicProcess.h"

#define MAX_PATH_SIZE		100

#define NPC_MAX_USER_LIST	5

#define NPC_ATTACK_SHOUT	0
#define NPC_SUBTYPE_LONG_MON 1

#define NPC_TRACING_STEP	100

#define NPC_HAVE_USER_LIST	5
#define NPC_HAVE_ITEM_LIST	6
#define NPC_PATTEN_LIST		5
#define NPC_PATH_LIST		50
#define NPC_MAX_PATH_LIST	100
#define NPC_EXP_RANGE		50
#define NPC_EXP_PERSENT		50

#define NPC_SECFORMETER_MOVE	4
#define NPC_SECFORMETER_RUN		4
#define NPC_VIEW_RANGE			100

#define MAX_MAGIC_TYPE3			20
#define MAX_MAGIC_TYPE4			9

struct  _NpcSkillList
{
	short	sSid;
	uint8	tLevel;
	uint8	tOnOff;
};

struct  _NpcGiveItem
{
	int 	sSid;			// item serial number
	short	count;			// item 갯수(돈은 단위)
};

struct _Target
{
	uint16	id;							// 공격대상 User uid
	bool bSet;
	float x;						// User의 x pos
	float y;						// User의 y pos
	float z;						// User의 z pos
};

struct _PattenPos
{
	short x;
	short z;
};

struct _PathList
{
	_PattenPos pPattenPos[NPC_MAX_PATH_LIST];
};

struct _TargetHealer
{
	short	sNID;				// npc nid
	short	sValue;				// 점수
};

class MAP;

#include "../GameServer/Unit.h"

enum MonSearchType
{
	MonSearchSameFamily,	// find any available mobs of the same family
	MonSearchAny,			// find any available mob
	MonSearchNeedsHealing	// find any mob that needs healing
};

enum CloseTargetResult
{
	CloseTargetInvalid,
	CloseTargetNotInRange,
	CloseTargetInGeneralRange,
	CloseTargetInAttackRange
};

enum NpcSpecialType
{
	NpcSpecialTypeNone				= 0,
	NpcSpecialTypeCycleSpawn		= 7,
	NpcSpecialTypeKarusWarder1		= 90,
	NpcSpecialTypeKarusWarder2		= 91,
	NpcSpecialTypeElmoradWarder1	= 92,
	NpcSpecialTypeElmoradWarder2	= 93,
	NpcSpecialTypeKarusKeeper		= 98,
	NpcSpecialTypeElmoradKeeper		= 99
};

struct __Vector3;
class CNpc : public Unit
{
public:
	uint16 GetID() { return m_sNid; }
	INLINE uint16 GetProtoID() { return GetProto()->m_sSid; }
	std::string & GetName() { return GetProto()->m_strName; }

	int32 GetHealth() { return m_iHP; }
	int32 GetMaxHealth() { return GetProto()->m_iMaxHP; }
	int32 GetMana() { return m_sMP; }
	int32 GetMaxMana() { return GetProto()->m_sMaxMP; }

	void GetInOut(Packet &, uint8) {}
	void AddToRegion(int16 sRegionX, int16 sRegionZ) {}

	void HpChange(int amount, Unit *pAttacker = nullptr, bool bSendToGameServer = true);
	void MSpChange(int amount) {}

	INLINE CNpcTable * GetProto() { return m_proto; }
	INLINE uint8 GetType() { return GetProto()->m_tNpcType; }

	INLINE bool isHealer() { return GetType() == NPC_HEALER; } 

	INLINE bool isGuard()
	{
		return GetType() == NPC_GUARD || GetType() == NPC_PATROL_GUARD || GetType() == NPC_STORE_GUARD;
	}

	INLINE bool isGate() 
	{
		return GetType() == NPC_GATE 
			|| GetType() == NPC_GATE2
			|| GetType() == NPC_PHOENIX_GATE 
			|| GetType() == NPC_SPECIAL_GATE 
			|| GetType() == NPC_VICTORY_GATE
			|| GetType() == NPC_GATE_LEVER; 
	}

	INLINE bool isArtifact() 
	{ 
		return GetType() == NPC_ARTIFACT 
			|| GetType() == NPC_DESTROYED_ARTIFACT 
			|| GetType() == NPC_ARTIFACT1 
			|| GetType() == NPC_ARTIFACT2 
			|| GetType() == NPC_ARTIFACT3 
			|| GetType() == NPC_ARTIFACT4; 
	}

	INLINE bool isNonAttackingObject() { return isGate() || GetType() == NPC_GATE_LEVER || isArtifact() || GetType() == NPC_SCARECROW; }
	INLINE bool isNonAttackableObject() { return isGate() || GetType() == NPC_GATE_LEVER; }

	INLINE bool isDead() { return m_NpcState == NPC_DEAD || m_iHP <= 0; }
	INLINE bool isAlive() { return !isDead(); }
	INLINE bool isMonster() { return m_bMonster; }

	INLINE bool hasTarget() { return m_Target.bSet; }

	CNpcTable *m_proto;

	_Target	m_Target;				// 공격할 유저 저장,,
	short		m_ItemUserLevel;		// 죽을때 매직 이상 아이템를 떨구기위해 참조해야하는 유저의레벨

	std::map<uint16, uint32> m_DamagedUserList; // max NPC_HAVE_USER_LIST?
	std::recursive_mutex m_damageListLock;

	int		m_TotalDamage;
	short   m_sMaxDamageUserid;		// 나에게 최고의 데미지를 준 유저의 아이디 저장..

	_PathList m_PathList;			// Npc의 패스 리스트 
	_PattenPos m_pPattenPos;		// Npc의 패턴,,

	//int m_iPattenNumber;		// 현재의 패턴번호
	short m_iPattenFrame;			// 패턴의 현재 위치..

	uint8 m_byMoveType;			// NPC의 행동타입(이동관련)
	uint8 m_byInitMoveType;		// NPC의 초기 행동타입(이동관련)
	short m_sPathCount;			// NPC의 PathList Count
	short m_sMaxPathCount;		// NPC의 PathList Max Count

	bool	m_bFirstLive;		// NPC 가 처음 생성되는지 죽었다 살아나는지 판단.
	uint8	m_OldNpcState, m_NpcState;

	short	m_sNid;

	float		m_nInitX;			// 처음 생성된 위치 X
	float		m_nInitY;			// 처음 생성된 위치 Y
	float		m_nInitZ;			// 처음 생성된 위치 Z

	float		m_fPrevX;			// Prev X Pos;
	float		m_fPrevY;			// Prev Y Pos;
	float		m_fPrevZ;			// Prev Z Pos;

	//
	//	PathFind Info
	//
	short		m_min_x;
	short		m_min_y;
	short		m_max_x;
	short		m_max_y;

	typedef struct { long cx; long cy; } Size;
	Size	m_vMapSize;

	float m_fStartPoint_X, m_fStartPoint_Y;
	float m_fEndPoint_X, m_fEndPoint_Y;

	short m_sStepCount;

	CPathFind m_vPathFind;
	_PathNode	*m_pPath;

	int		m_nInitMinX;					// 초기위치
	int		m_nInitMinY;
	int		m_nInitMaxX;
	int		m_nInitMaxY;

	time_t	m_fHPChangeTime;
	time_t	m_tFaintingTime;

	//----------------------------------------------------------------
	//	MONSTER DB 쪽에 있는 변수들
	//----------------------------------------------------------------
	short   m_sSize;						// 캐릭터의 비율(100 퍼센트 기준)
	int     m_iWeapon_1;			// 착용 무기
	int     m_iWeapon_2;			// 착용 무기
	uint8	m_byActType;		// 행동패턴
	uint8	m_byRank;			// 작위
	uint8	m_byTitle;			// 지위
	int 	m_iSellingGroup;	// 아이템 그룹(물건매매 담당 NPC의 경우만)
	int		m_iMaxHP;			// 최대 HP
	short	m_sMaxMP;			// 최대 MP
	short	m_sAttack;			// 공격값(지금 사용하지 않음..)
	short	m_sAttackDelay;		// 공격딜레이
	short	m_sSpeed;			// 이동속도	
	float   m_fSpeed_1;			// 기본 이동 타입		(1초에 갈 수 있는 거리)
	float   m_fSpeed_2;			// 뛰는 이동 타입..		(1초에 갈 수 있는 거리)
	short	m_sStandTime;		// 서있는 시간

	uint8	m_bySearchRange;	// 적 탐지 범위
	uint8	m_byAttackRange;	// 사정거리
	uint8	m_byTracingRange;	// 추격 거리

	int		m_iMoney;			// 떨어지는 돈
	int		m_iItem;			// 떨어지는 아이템

	int			m_iHP;				// 현재 HP
	short		m_sMP;				// 현재 MP

	float   m_fSecForMetor;		// 초당 갈 수 있는 거리..

	//----------------------------------------------------------------
	//	MONSTER AI에 관련된 변수들
	//----------------------------------------------------------------
	uint8	m_tNpcAttType;		// 공격 성향 : 선공(1), 후공(0)
	bool	m_bHasFriends;		// When set, monsters behave in groups (defined by their family type) and will seek out help from nearby similar mobs.
	uint8	m_byAttackPos;		// User의 어느 부분에서 공격하느지를 판단(8방향)
	uint8	m_byBattlePos;		// 어떤 진형을 선택할 것인지를 판단..
	bool	m_byGateOpen;		// 성문일 경우에.. 사용... Gate Npc Status -> 1 : open 0 : close
	uint8	m_byMaxDamagedNation;	// 나를 죽인 유저의 국가를 저장.. (1:카루스, 2:엘모라드)
	uint8	m_byObjectType;         // 보통은 0, object타입(성문, 레버)은 1
	uint8	m_byDungeonFamily;		// 던젼에서 같은 패밀리 묶음 (같은 방)
	NpcSpecialType	m_bySpecialType;		// 몬스터의 형태가 변하는지를 판단(0:변하지 않음, 1:변하는 몬스터, 
	// 2:죽는경우 조정하는 몬스터(대장몬스터 죽을경우 성문이 열림), 
	// 3:대장몬스터의 죽음과 관련이 있는 몬스터(대장몬스터가 죽으면 관계되는 몬스터는 같이 죽도록)
	// 4:변하면서 죽는경우 조정하는 몬스터 (m_sControlSid)
	// 5:처음에 죽었있다가 출현하는 몬스터,,
	// 6:일정시간이 지난 후에 행동하는 몬스터,,
	// 100:죽었을때 데미지를 많이 입힌 유저를 기록해 주세여
	uint8	m_byTrapNumber;			// 던젼에서 트랩의 번호,,
	uint8	m_byChangeType;			// 0:정상상태, 1:변하기 위한 준비, 2:다른몬스터로 변함, 3:몬스터의 출현, 100:몬스터의 죽음
	uint8	m_byRegenType;			// 0:정상적으로 리젠이 됨.. , 1:한번 죽으면 리젠이 안되는 특수 몸, 2:리젠이 안됨
	uint8    m_byDeadType;			// 0:살아 있는 경우, 100:전쟁이벤트중 죽은 경우


	//----------------------------------------------------------------
	//	MONSTER_POS DB 쪽에 있는 변수들
	//----------------------------------------------------------------
	time_t	m_Delay;			// this doesn't really need to be time_t, but we'll use it (at least for now) for consistency
	time_t	m_fDelayTime;		// Npc Thread체크 타임...

	uint8	m_byType;
	int		m_sRegenTime;		// NPC 재생시간

	uint8	m_byDirection;

	int		m_nLimitMinX;		// 활동 영역
	int		m_nLimitMinZ;
	int		m_nLimitMaxX;
	int		m_nLimitMaxZ;

	bool	m_bIsEventNpc;

	float m_fAdd_x;
	float m_fAdd_z;

	float m_fBattlePos_x;	
	float m_fBattlePos_z;

	float m_fSecForRealMoveMetor;		// 초당 갈 수 있는 거리..(실제 클라이언트에 보내주는 거리)

	bool m_bPathFlag;					// 패스 파인드 실행여부 체크 변수..

	//----------------------------------------------------------------
	//	NPC 이동 관련
	//----------------------------------------------------------------
	_NpcPosition	m_pPoint[MAX_PATH_LINE];			// 이동시 참고 좌표

	short m_iAniFrameIndex;
	short m_iAniFrameCount;
	uint8 m_byPathCount;					// 패스를 따라 이동하는 몬스터 끼리 겹치지 않도록,, 
	bool m_bStopFollowingTarget;		// when set, indicates that an NPC should stop following its target
	uint8 m_byActionFlag;				// 행동변화 플래그 ( 0 : 행동변화 없음, 1 : 공격에서 추격)

	bool m_bTracing;
	float m_fTracingStartX, m_fTracingStartZ;

	short m_iFind_X[4];					// find enemy에서 찾을 Region검사영역
	short m_iFind_Y[4];

	float   m_fOldSpeed_1;			// 기본 이동 타입		(1초에 갈 수 있는 거리)
	float   m_fOldSpeed_2;			// 뛰는 이동 타입..		(1초에 갈 수 있는 거리)

	bool	m_bMonster;

	uint32	m_nActiveSkillID;		// ID of skill currently being cast
	int16	m_sActiveTargetID;		// ID of the target of the skill currently being cast
	uint16	m_sActiveCastTime;		// Cast time of the skill currently being cast (in seconds)

	bool	m_bDelete; // when set, will remove the NPC from the server after execution.

	int16	m_oSocketID; // owner user
	uint16	m_bEventRoom;
	int UnixGateOpen, UnixGateClose;
public:
	CNpc();
	virtual ~CNpc();

	void Init();	//	NPC 기본정보 초기화
	void InitTarget(void);
	void InitUserList();
	void InitPos();

	void Load(uint16 sNpcID, CNpcTable * proto, bool bMonster, uint8 nation = 0);
	void SendMoveResult(float fX, float fY, float fZ, float fSpeed = 0.0f);

protected:
	void ClearPathFindData(void);

public:

	void FillNpcInfo(Packet & result);
	void NpcStrategy(uint8 type);
	int  FindFriend(MonSearchType type = MonSearchSameFamily);
	void  FindFriendRegion(int x, int z, MAP* pMap, _TargetHealer* pHealer, MonSearchType type = MonSearchSameFamily);
	bool IsCloseTarget(CUser *pUser, int nRange);
	void SendExpToUserList();
	void RecvAttackReq(int nDamage, uint16 sAttackerID, AttributeType attributeType = AttributeNone);
	void ChangeTarget(int nAttackType, CUser *pUser);
	void ChangeNTarget(CNpc *pNpc);
	bool ResetPath();
	bool GetTargetPos(float& x, float& z);
	bool IsChangePath();
	time_t Attack();
	time_t LongAndMagicAttack();
	void TracingAttack();
	int GetTargetPath(int option = 0);
	CloseTargetResult IsCloseTarget(int nRange, AttackType attackType);
	bool StepMove();
	bool StepNoPathMove();
	bool IsMovingEnd();
	int  IsSurround(CUser* pUser);
	bool IsDamagedUserList(CUser *pUser);
	bool IsPathFindCheck(float fDistance);
	void IsNoPathFind(float fDistance);
	void GiveNpcHaveItem();

	time_t NpcLive();
	time_t NpcTracing();
	time_t NpcAttacking();
	time_t NpcMoving();
	time_t NpcSleeping();
	time_t NpcFainting();
	time_t NpcHealing();
	time_t NpcCasting();
	time_t NpcStanding();
	time_t NpcBack();
	bool SetLive();

	bool isInSpawnRange(int nX, int nZ);
	bool RandomMove();
	bool RandomBackMove();
	bool IsInPathRange();
	int GetNearPathPoint();

	short GetDamage(Unit *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	short GetDamage(CUser *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);
	short GetDamage(CNpc *pTarget, _MAGIC_TABLE *pSkill = nullptr, bool bPreviewOnly = false);

	void SendAttackRequest(int16 tid);

	bool RegisterRegion(float x, float z);
	void SendInOut(InOutType type);
	void SendNpcInfo();
	void SendRegionUpdate();
	void Dead(Unit * pKiller = nullptr, bool bSendDeathPacket = true);
	bool isShowBox();

	bool FindEnemy();
	bool CheckFindEnemy();
	int FindEnemyRegion();
	float FindEnemyExpand(int nRX, int nRZ, float fCompDis, UnitType unitType);
	int GetMyField();

	int GetDir(float x1, float z1, float x2, float z2);
	void NpcMoveEnd();

	void GetVectorPosition(__Vector3 & vOrig, __Vector3 & vDest, float fDis, __Vector3 * vResult);
	void CalcAdaptivePosition(__Vector3 & vPosOrig, __Vector3 & vPosDest, float fAttackDistance, __Vector3 * vResult);
	void ComputeDestPos(__Vector3 & vCur, float fDegree, float fDistance, __Vector3 * vResult);
	void Yaw2D(float fDirX, float fDirZ, float& fYawResult);
	float GetDistance(__Vector3 & vOrig, __Vector3 & vDest);
	int  PathFind(CPoint start, CPoint end, float fDistance);
	bool GetUserInView();
	bool GetUserInViewRange(int x, int z);
	void MoveAttack();
	void HpChange();
	int	 ItemProdution(int item_number);
	int  GetItemGrade(int item_grade);
	int  GetItemCodeNumber(int level, int item_type);
	int  GetWeaponItemCodeNumber(bool bWeapon);
	int  GetPartyExp( int party_level, int man, int nNpcExp );
	void ChangeAbility(int iChangeType);
	bool Teleport();

	bool isHostileTo(Unit * pTarget);
};
