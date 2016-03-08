#include "stdafx.h"
#include "Region.h"
#include "Extern.h"
#include "Npc.h"
#include "User.h"
#include "NpcThread.h"

// 1m
//float surround_fx[8] = {0.0f, -0.7071f, -1.0f, -0.7083f,  0.0f,  0.7059f,  1.0000f, 0.7083f};
//float surround_fz[8] = {1.0f,  0.7071f,  0.0f, -0.7059f, -1.0f, -0.7083f, -0.0017f, 0.7059f};
// 2m
static float surround_fx[8] = {0.0f, -1.4142f, -2.0f, -1.4167f,  0.0f,  1.4117f,  2.0000f, 1.4167f};
static float surround_fz[8] = {2.0f,  1.4142f,  0.0f, -1.4167f, -2.0f, -1.4167f, -0.0035f, 1.4117f};

enum
{
	TENDER_ATTACK_TYPE		= 0, // The spawn is passive, i.e. won't attack until it's attacked, or it expects same-type spawns to help out.
	ATROCITY_ATTACK_TYPE	= 1  // The spawn is aggressive, i.e. it's just as happy to attack you first.
};

#define NO_ACTION				0
#define ATTACK_TO_TRACE			1				// 공격에서 추격
#define LONG_ATTACK_RANGE		30				// 장거리 공격 유효거리
#define SHORT_ATTACK_RANGE		3				// 직접공격 유효거리

#define ARROW_MIN				391010000
#define ARROW_MAX				392010000

#define FAINTING_TIME			2 // in seconds

bool CNpc::RegisterRegion(float x, float z)
{
	MAP* pMap = GetMap();
	if (pMap == nullptr) 
	{
		TRACE("#### Npc-SetUid Zone Fail : [name=%s], zone=%d #####\n", GetName().c_str(), GetZoneID());
		return false;
	}

	int x1 = (int)x / TILE_SIZE;
	int z1 = (int)z / TILE_SIZE;
	int nRX = (int)x / VIEW_DIST;
	int nRY = (int)z / VIEW_DIST;

	if (x1 < 0 || z1 < 0 || x1 >= pMap->GetMapSize() || z1 >= pMap->GetMapSize())
	{
		TRACE("#### SetUid failed : [nid=%d, sid=%d, zone=%d], coordinates out of range of map x=%d, z=%d, map size=%d #####\n", 
			GetID(), GetProtoID(), GetZoneID(), x1, z1, pMap->GetMapSize());
		return false;
	}

	// if(pMap->m_pMap[x1][z1].m_sEvent == 0) return false;
	if(nRX > pMap->GetXRegionMax() || nRY > pMap->GetZRegionMax() || nRX < 0 || nRY < 0)
	{
		TRACE("#### SetUid Fail : [nid=%d, sid=%d], nRX=%d, nRZ=%d #####\n", GetID(), GetProtoID(), nRX, nRY);
		return false;
	}

	if (GetRegionX() != nRX || GetRegionZ() != nRY)
	{
		int nOld_RX = GetRegionX();
		int nOld_RZ = GetRegionZ();

		m_sRegionX = nRX;
		m_sRegionZ = nRY;

		pMap->RegionNpcAdd(GetRegionX(), GetRegionZ(), GetID());
		pMap->RegionNpcRemove(nOld_RX, nOld_RZ, GetID());

		SendRegionUpdate();
	}

	return true;
}

void CNpc::SendInOut(InOutType type)
{
	Packet result(AG_NPC_INOUT);
	result << uint8(type) << GetID() << GetX() << GetZ() << GetY();
	g_pMain->Send(&result);
}

void CNpc::SendNpcInfo()
{
	Packet result(AG_NPC_INFO);
	result.SByte();
	FillNpcInfo(result);
	g_pMain->Send(&result);
}

/**
* @brief	Sends a region update packet to the game server
* 			to indicate the NPC has changed regions, so it should
* 			handle showing/removing the NPCs from applicable players.
*/
void CNpc::SendRegionUpdate()
{
	Packet result(AG_NPC_REGION_UPDATE);
	result << GetID() << GetX() << GetY() << GetZ();
	g_pMain->Send(&result);
}

CNpc::CNpc() : Unit(UnitNPC), m_bDelete(false),
	m_NpcState(NPC_LIVE), m_OldNpcState(m_NpcState), m_byGateOpen(false), m_byObjectType(NORMAL_OBJECT), m_byPathCount(0),
	m_byAttackPos(0), m_ItemUserLevel(0), m_Delay(0), m_nActiveSkillID(0), m_sActiveTargetID(-1), m_sActiveCastTime(0),
	m_byDirection(0),
	m_proto(nullptr), m_pPath(nullptr)
{
	InitTarget();

	m_fDelayTime = getMSTime();

	m_tNpcAttType = ATROCITY_ATTACK_TYPE;
	m_bHasFriends = false; // :'(
	m_byMoveType = 1;
	m_byInitMoveType  = 1;
	m_byRegenType = 0;
	m_byDungeonFamily = 0;
	m_bySpecialType = NpcSpecialTypeNone;
	m_byTrapNumber = 0;
	m_byChangeType = 0;
	m_byDeadType = 0;
	m_sPathCount = 0;
	m_sMaxPathCount = 0;

	m_bFirstLive = true;

	m_fHPChangeTime = getMSTime();
	m_tFaintingTime = 0;

	m_nLimitMinX = m_nLimitMinZ = 0;
	m_nLimitMaxX = m_nLimitMaxZ = 0;
	m_bIsEventNpc = false;
	m_fSecForRealMoveMetor = 0.0f;
	InitUserList();

	m_bTracing = false;
	m_fTracingStartX = m_fTracingStartZ = 0.0f;

	for(int i=0; i<NPC_MAX_PATH_LIST; i++)	{
		m_PathList.pPattenPos[i].x = -1;
		m_PathList.pPattenPos[i].z = -1;
	}
	m_pPattenPos.x = m_pPattenPos.z = 0;

	m_bMonster = false;

	m_oSocketID = -1;
	m_bEventRoom = 0;
	UnixGateOpen = 0; 
	UnixGateClose = 0;
}

CNpc::~CNpc()
{
	ClearPathFindData();
	InitUserList();
}

void CNpc::ClearPathFindData()
{
	m_bPathFlag = false;
	m_sStepCount = 0;
	m_iAniFrameCount = 0;
	m_iAniFrameIndex = 0;
	m_fAdd_x = m_fAdd_z = 0.0f;

	for (int i = 0; i < MAX_PATH_LINE; i++)
	{
		m_pPoint[i].byType = 0;
		m_pPoint[i].bySpeed = 0;
		m_pPoint[i].fXPos = -1.0f;
		m_pPoint[i].fZPos = -1.0f;
	}
}

void CNpc::InitUserList()
{
	m_sMaxDamageUserid = -1;
	m_TotalDamage = 0;
	m_DamagedUserList.clear();
}

void CNpc::InitTarget()
{
	if (m_byAttackPos != 0)
	{
		if (hasTarget() && m_Target.id < NPC_BAND)
		{
			CUser* pUser = g_pMain->GetUserPtr(m_Target.id);
			if (pUser != nullptr
				&& m_byAttackPos > 0 && m_byAttackPos < 9)
				pUser->m_sSurroundNpcNumber[m_byAttackPos - 1] = -1;
		}
	}

	m_byAttackPos = 0;
	m_Target.id = -1;
	m_Target.bSet = false;
	m_Target.x = m_Target.y = m_Target.z = 0.0f;
	m_bTracing = false;
}

void CNpc::Init()
{
	m_pMap = g_pMain->GetZoneByID(GetZoneID());
	m_Delay = 0;
	m_fDelayTime = getMSTime();

	if (GetMap() == nullptr) 
	{
		TRACE("#### Npc-Init Zone Fail : [name=%s], zone=%d #####\n", GetName().c_str(), GetZoneID());
		return;
	}
}

void CNpc::InitPos()
{
	static const float fDD = 1.5f;
	static const float fx[4][5] = 
	{														// battle pos 
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },					// 0
		{ 0.0f, -(fDD*2),  -(fDD*2), -(fDD*4),  -(fDD*4) },	// 1
		{ 0.0f,  0.0f, -(fDD*2), -(fDD*2), -(fDD*2) },		// 2
		{ 0.0f, -(fDD*2),  -(fDD*2), -(fDD*2), -(fDD*4) }	// 3
	};

	static const float fz[4][5] = 
	{														// battle pos 
		{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },					// 0
		{ 0.0f,  (fDD*1),  -(fDD*1),  (fDD*1),  -(fDD*1) },	// 1
		{ 0.0f, -(fDD*2), (fDD*1), (fDD*1), (fDD*3) },		// 2
		{ 0.0f,  (fDD*2),   0.0f,    -(fDD*2),  0.0f }		// 3
	};

	if (m_byBattlePos > 3)
		return;

	m_fBattlePos_x = fx[m_byBattlePos][m_byPathCount - 1];
	m_fBattlePos_z = fz[m_byBattlePos][m_byPathCount - 1];
}

void CNpc::Load(uint16 sNpcID, CNpcTable * proto, bool bMonster, uint8 nation)
{
	m_sNid = sNpcID + NPC_BAND;
	m_proto = proto;

	m_bMonster = bMonster;

	m_sSize				= proto->m_sSize;
	m_iWeapon_1			= proto->m_iWeapon_1;
	m_iWeapon_2			= proto->m_iWeapon_2;
	m_bNation			= nation == 0 ? proto->m_byGroup : nation;
	m_bLevel			= (uint8) proto->m_sLevel; // max level used that I know about is 250, no need for 2 bytes

	// Monsters cannot, by design, be friendly to everybody.
	if (isMonster() && GetNation() == Nation::ALL)
		m_bNation = Nation::NONE;

	m_byActType			= proto->m_byActType;
	m_byRank			= proto->m_byRank;
	m_byTitle			= proto->m_byTitle;
	m_iSellingGroup		= proto->m_iSellingGroup;
	m_iHP				= proto->m_iMaxHP;
	m_iMaxHP			= proto->m_iMaxHP;
	m_sMP				= proto->m_sMaxMP;
	m_sMaxMP			= proto->m_sMaxMP;
	m_sAttack			= proto->m_sAttack;
	m_sTotalAc			= proto->m_sDefense;
	m_fTotalHitrate		= proto->m_sHitRate;
	m_fTotalEvasionrate	= proto->m_sEvadeRate;
	m_sTotalHit			= proto->m_sDamage;
	m_sAttackDelay		= proto->m_sAttackDelay;
	m_sSpeed			= proto->m_sSpeed;

	// Object NPCs should have an effective speed of 1x (not that it should matter, mind)
	if (m_byObjectType == SPECIAL_OBJECT)
		m_sSpeed = 1000;

	m_fSpeed_1			= ((float)proto->m_bySpeed_1 * ((float)m_sSpeed / 1000));
	m_fSpeed_2			= ((float)proto->m_bySpeed_2 * ((float)m_sSpeed / 1000));
	m_fOldSpeed_1		= ((float)proto->m_bySpeed_1 * ((float)m_sSpeed / 1000));
	m_fOldSpeed_2		= ((float)proto->m_bySpeed_2 * ((float)m_sSpeed / 1000));

	m_fSecForMetor		= 4.0f;
	m_sStandTime		= proto->m_sStandTime;
	m_sFireR			= proto->m_byFireR;
	m_sColdR			= proto->m_byColdR;
	m_sLightningR		= proto->m_byLightningR;
	m_sMagicR			= proto->m_byMagicR;
	m_sDiseaseR			= proto->m_byDiseaseR;
	m_sPoisonR			= proto->m_byPoisonR;
	m_bySearchRange		= proto->m_bySearchRange;
	m_byAttackRange		= proto->m_byAttackRange;
	m_byTracingRange	= proto->m_byTracingRange;
	m_iMoney			= proto->m_iMoney;
	m_iItem				= proto->m_iItem;

	m_sRegenTime		= 160 * MINUTE;
	m_sMaxPathCount		= 0;
	
	if(GetType() == OBJECT_ARTIFACT && proto->m_sSid == 541)
		m_sRegenTime = 1*MINUTE;

	m_pMap = g_pMain->GetZoneByID(GetZoneID());
	m_bFirstLive = 1;
}

void CNpc::SendMoveResult(float fX, float fY, float fZ, float fSpeed /*= 0.0f*/)
{
	Packet result(MOVE_RESULT, uint8(SUCCESS));
	result << GetID() << fX << fZ << fY << fSpeed;
	g_pMain->Send(&result);
	RegisterRegion(fX, fZ);
}

time_t CNpc::NpcLive()
{
	// Dungeon Work 
	if (m_byRegenType == 2 || (m_byRegenType == 1 && m_byChangeType == 100))
	{
		m_NpcState = NPC_LIVE;
		return m_sRegenTime;
	}

	m_NpcState = SetLive() ? NPC_STANDING : NPC_LIVE;
	return m_sStandTime;
}

time_t CNpc::NpcTracing()
{
	if (m_sStepCount != 0)
	{
		if (m_fPrevX < 0 || m_fPrevZ < 0)
		{
			TRACE("### Npc-NpcTracing  Fail : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), m_fPrevX, m_fPrevZ);
		}
		else	
		{
			m_curx = m_fPrevX;		
			m_curz = m_fPrevZ; 
		}
	}

	if (isNonAttackingObject())
	{
		InitTarget();
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	// Prevent spawns like Guard Towers from following
	// targets while attacking.
	if (m_byMoveType == 4)
	{
		m_NpcState = NPC_FIGHTING;
		return m_sStandTime;
	}

	auto result = IsCloseTarget(m_byAttackRange, AttackTypePhysical);
	if (result == CloseTargetInGeneralRange)
	{
		NpcMoveEnd();
		m_NpcState = NPC_FIGHTING;
		return 0;
	}
	else if (result == CloseTargetInvalid)
	{
		InitTarget();
		NpcMoveEnd();
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}
	else if (result == CloseTargetInAttackRange && GetProto()->m_byDirectAttack == 2)
	{
		NpcMoveEnd();
		m_NpcState = NPC_FIGHTING;
		return 0;
	}

	if (m_byActionFlag == ATTACK_TO_TRACE)
	{
		m_byActionFlag = NO_ACTION;
		m_bStopFollowingTarget = true;

		// If we're not already following a user, define our start coords.
		if (!m_bTracing)
		{
			m_fTracingStartX = GetX();
			m_fTracingStartZ = GetZ();
			m_bTracing = true;
		}
	}

	if (m_bStopFollowingTarget)
	{
		if (!ResetPath())// && !m_tNpcTraceType)
		{
			TRACE("##### NpcTracing Fail : 패스파인드 실패 , NPC_STANDING으로 ######\n");
			InitTarget();
			NpcMoveEnd();	// 이동 끝..
			m_NpcState = NPC_STANDING;
			return m_sStandTime;
		}
	}

	if (  (!m_bPathFlag && !StepMove())
		|| (m_bPathFlag && !StepNoPathMove()))
	{
		m_NpcState = NPC_STANDING;
		TRACE("### NpcTracing Fail : StepMove 실패, %s, %d ### \n", GetName().c_str(), GetID());
		return m_sStandTime;
	}

	if (!IsMovingEnd())
		SendMoveResult(GetX(), GetY(), GetZ());
	else
		SendMoveResult(m_fPrevX, m_fPrevY, m_fPrevZ, (float)(m_fSecForRealMoveMetor / ((double)m_sSpeed / 1000)));

	if (result == CloseTargetInAttackRange 
		&& GetProto()->m_byDirectAttack == 0 
		&& !isHealer())
		TracingAttack();

	return m_sSpeed;	
}

time_t CNpc::NpcAttacking()
{
	if (isDead())
	{	
		Dead();
		return -1;
	}

	if (isNonAttackingObject())
	{
		m_NpcState = NPC_STANDING;
		return m_sStandTime/2;
	}

	auto result = IsCloseTarget(m_byAttackRange, AttackTypeNone);
	if (result == CloseTargetInGeneralRange)
	{
		m_NpcState = NPC_FIGHTING;
		return 0;
	}

	int nValue = GetTargetPath();
	if (nValue == -1)
	{
		if (!RandomMove())
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return m_sStandTime;
		}

		InitTarget();
		m_NpcState = NPC_MOVING;
		return m_sSpeed;
	}
	else if (nValue == 0)
	{
		m_fSecForMetor = m_fSpeed_2;
		IsNoPathFind(m_fSecForMetor);
	}

	m_NpcState = NPC_TRACING;
	return 0;
}

time_t CNpc::NpcMoving()
{
	if (isDead())
	{
		Dead();
		return -1;
	}

	if (m_sStepCount != 0)	
	{
		if (m_fPrevX < 0 || m_fPrevZ < 0)
		{
			TRACE("### Npc-Moving Fail : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), m_fPrevX, m_fPrevZ);
		}
		else	
		{
			m_curx = m_fPrevX;
			m_curz = m_fPrevZ; 
		}
	}

	if (FindEnemy())
	{
		/*	if (isGuard()) 
		{ 
		NpcMoveEnd();
		m_NpcState = NPC_FIGHTING; 
		return 0; 
		}
		else */
		{ 
			NpcMoveEnd();
			m_NpcState = NPC_ATTACKING;
			return m_sSpeed;
		}
	}	

	if (IsMovingEnd())
	{
		m_curx = m_fPrevX;		
		m_curz = m_fPrevZ; 
		if (GetX() < 0 || GetZ() < 0)	
			TRACE("Npc-NpcMoving-2 : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), GetX(), GetZ());

		//SendMoveResult(m_fPrevX, m_fPrevY, m_fPrevZ);
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	if (  (!m_bPathFlag && !StepMove())
		|| (m_bPathFlag && !StepNoPathMove()))
	{
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	SendMoveResult(m_fPrevX, m_fPrevY, m_fPrevZ, (float)(m_fSecForRealMoveMetor / ((double)m_sSpeed / 1000)));
	return m_sSpeed;	
}

time_t CNpc::NpcStanding()
{
	/*	if (g_pMain->m_bIsNight)
	{
	m_NpcState = NPC_SLEEPING;
	return 0;
	}	*/

	MAP* pMap = GetMap();
	if (pMap == nullptr)	
	{
		TRACE("### NpcStanding Zone Index Error : nid=%d, name=%s, zone=%d ###\n", GetID(), GetName().c_str(), GetZoneID());
		return -1;
	}

	CRoomEvent * pRoom = pMap->m_arRoomEventArray.GetData(m_byDungeonFamily);
	if (pRoom != nullptr 
		&& pRoom->m_byStatus == 1)
	{
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	if (RandomMove())
	{
		m_iAniFrameCount = 0;
		m_NpcState = NPC_MOVING;
		return m_sStandTime;
	}	

	m_NpcState = NPC_STANDING;

	if (GetType() == NPC_SPECIAL_GATE 
		&& g_pMain->m_byBattleEvent == BATTLEZONE_OPEN)
	{
		if(m_byGateOpen && UnixGateClose == 30)
		{
		m_byGateOpen = !m_byGateOpen;
		Packet result(AG_NPC_GATE_OPEN);
		result << GetID() << GetProtoID() << m_byGateOpen;
		g_pMain->Send(&result);
		UnixGateOpen = 0;
		}
		else if (!m_byGateOpen && UnixGateOpen == 180)
		{
		m_byGateOpen = !m_byGateOpen;
		Packet result(AG_NPC_GATE_OPEN);
		result << GetID() << GetProtoID() << m_byGateOpen;
		g_pMain->Send(&result);
		UnixGateClose = 0;
		}
	UnixGateClose++;
	UnixGateOpen++;
	}
	return m_sStandTime;
}

time_t CNpc::NpcBack()
{
	if (isDead())
	{
		Dead();
		return -1;
	}

	if (hasTarget()
		&& g_pMain->GetUnitPtr(m_Target.id) == nullptr)
	{
		m_NpcState = NPC_STANDING;
		return m_sSpeed;//STEP_DELAY;
	}

	if (m_sStepCount != 0)	
	{
		if (m_fPrevX < 0 || m_fPrevZ < 0)	
		{
			TRACE("### Npc-NpcBack Fail-1 : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), m_fPrevX, m_fPrevZ);
		}
		else	
		{
			m_curx = m_fPrevX;		
			m_curz = m_fPrevZ; 
		}
	}

	if (IsMovingEnd())	
	{
		m_curx = m_fPrevX;		
		m_curz = m_fPrevZ; 

		if (GetX() < 0 || GetZ() < 0)
			TRACE("Npc-NpcBack-2 : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), GetX(), GetZ());

		SendMoveResult(GetX(), GetY(), GetZ());
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	if (  (!m_bPathFlag && !StepMove())
		|| (m_bPathFlag && !StepNoPathMove()))
	{
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	SendMoveResult(m_fPrevX, m_fPrevY, m_fPrevZ, (float)(m_fSecForRealMoveMetor / ((double)m_sSpeed / 1000)));
	return m_sSpeed;
}

bool CNpc::SetLive()
{
	int i = 0, j = 0;
	m_iHP = m_iMaxHP;
	m_sMP = m_sMaxMP;
	m_sPathCount = 0;
	m_iPattenFrame = 0;
	m_bStopFollowingTarget = false;
	m_byActionFlag = NO_ACTION;
	m_byMaxDamagedNation = 0;

	m_sRegionX = m_sRegionZ = -1;
	m_fAdd_x = 0.0f;	m_fAdd_z = 0.0f;
	m_fStartPoint_X = 0.0f;		m_fStartPoint_Y = 0.0f; 
	m_fEndPoint_X = 0.0f;		m_fEndPoint_Y = 0.0f;
	m_min_x = m_min_y = m_max_x = m_max_y = 0;

	InitTarget();
	ClearPathFindData();
	InitUserList();
	//InitPos();

	CNpc* pNpc = nullptr;

	if (m_bIsEventNpc && !m_bFirstLive)
	{
		g_pMain->RemoveEventNPC(this);
		m_bDelete = true;
		return true;
	}

	MAP* pMap = GetMap();
	if (pMap == nullptr)	
		return false;

	if (m_bFirstLive)
	{
		m_nInitX = m_fPrevX = GetX();
		m_nInitY = m_fPrevY = GetY();
		m_nInitZ = m_fPrevZ = GetZ();
	}

	if (GetX() < 0 || GetZ() < 0)
		TRACE("Npc-SetLive-1 : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), GetX(), GetZ());

	int dest_x = (int)m_nInitX / TILE_SIZE;
	int dest_z = (int)m_nInitZ / TILE_SIZE;

	bool bMove = pMap->IsMovable(dest_x, dest_z);

	if (GetType() != NPCTYPE_MONSTER || m_bIsEventNpc)
	{
		m_curx = m_fPrevX = m_nInitX;
		m_cury = m_fPrevY = m_nInitY;
		m_curz = m_fPrevZ = m_nInitZ;
	}
	else	
	{
		int nX = 0;
		int nZ = 0;
		int nTileX = 0;
		int nTileZ = 0;
		int nRandom = 0;

		while (1)	
		{
			i++;
			nRandom = abs(m_nInitMinX - m_nInitMaxX);
			if (nRandom <= 1)	
				nX = m_nInitMinX;
			else	
			{
				if (m_nInitMinX < m_nInitMaxX)
					nX = myrand(m_nInitMinX, m_nInitMaxX);
				else
					nX = myrand(m_nInitMaxX, m_nInitMinX);
			}
			nRandom = abs(m_nInitMinY - m_nInitMaxY);
			if (nRandom <= 1)	
				nZ = m_nInitMinY;
			else	
			{
				if (m_nInitMinY < m_nInitMaxY)
					nZ = myrand(m_nInitMinY, m_nInitMaxY);
				else
					nZ = myrand(m_nInitMaxY, m_nInitMinY);
			}

			nTileX = nX / TILE_SIZE;
			nTileZ = nZ / TILE_SIZE;

			if (nTileX >= pMap->GetMapSize())
				nTileX = pMap->GetMapSize();
			if (nTileZ >= pMap->GetMapSize())
				nTileZ = pMap->GetMapSize();

			if (nTileX < 0 || nTileZ < 0)	
			{
				TRACE("#### Npc-SetLive() Fail : nTileX=%d, nTileZ=%d #####\n", nTileX, nTileZ);
				return false;
			}

			m_nInitX = m_fPrevX = m_curx = (float)nX;
			m_nInitZ = m_fPrevZ = m_curz = (float)nZ;

			if (GetX() < 0 || GetZ() < 0)
				TRACE("Npc-SetLive-2 : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), GetX(), GetZ());

			break;
		}	
	}

	m_fHPChangeTime = getMSTime();
	m_tFaintingTime = 0;

	if (GetZoneID() == ZONE_FORGOTTEN_TEMPLE)
		{
			m_byActType = 4;
			m_bySearchRange = myrand(200,255);
		}

	if (m_bFirstLive)	
	{
		switch (m_byActType)
		{
		case 1:
		case 2:
			m_tNpcAttType = TENDER_ATTACK_TYPE;
			break;

		case 3:
		case 4:
			m_bHasFriends = true; // yay!
			m_tNpcAttType = ATROCITY_ATTACK_TYPE;
			break;

		case 6:
			break;

		default:
			m_tNpcAttType = ATROCITY_ATTACK_TYPE;
		}

		m_bFirstLive = false;

		if (g_pMain->m_CurrentNPC.increment() == g_pMain->m_TotalNPC
			&& !m_bIsEventNpc)
		{
			printf("Monster All Init Success - %d\n", (uint16) g_pMain->m_TotalNPC);
			g_pMain->GameServerAcceptThread();
		}
	}

	if (m_byMoveType == 3 && m_sMaxPathCount == 2)
	{
		__Vector3 vS, vE, vDir;
		float fDir;
		vS.Set((float)m_PathList.pPattenPos[0].x, 0, (float)m_PathList.pPattenPos[0].z);
		vE.Set((float)m_PathList.pPattenPos[1].x, 0, (float)m_PathList.pPattenPos[1].z);
		vDir = vE - vS;
		vDir.Normalize();
		Yaw2D(vDir.x, vDir.z, fDir);
		m_byDirection = (uint8)fDir;
	}

	RegisterRegion(GetX(), GetZ());
	m_byDeadType = 0;

	SendNpcInfo();
	return true;
}

bool CNpc::RandomMove()
{
	m_fSecForMetor = m_fSpeed_1;

	if (GetMap() == nullptr
		|| m_bySearchRange == 0
		|| m_byMoveType == 0
		|| !GetUserInView()
		// 4 means non-moving.
		|| m_byMoveType == 4)
		return false;

	float fDestX = -1.0f, fDestZ = -1.0f;
	int max_xx = GetMap()->GetMapSize();
	int max_zz = GetMap()->GetMapSize();
	int x = 0, y = 0;

	__Vector3 vStart, vEnd, vNewPos;
	float fDis = 0.0f;

	int nPathCount = 0;

	int random_x = 0, random_z = 0;

	if (m_byMoveType == 1 || m_byMoveType == 2)	
	{
		random_x = 4;
		random_z = 4;

		switch (m_iPattenFrame)
		{
		case -4:
			fDestX = GetX() +  m_pPattenPos.x - (float)random_x;
			fDestZ = GetZ() +  m_pPattenPos.z - (float)random_z;
			m_iPattenFrame = 0;
			break;
		case -3:
			fDestX = GetX() +  m_pPattenPos.x - (float)random_x;
			fDestZ = GetZ() +  m_pPattenPos.z - (float)random_z;
			m_iPattenFrame--;
			break;
		case -2:
			fDestX = GetX() +  m_pPattenPos.x + (float)random_x;
			fDestZ = GetZ() +  m_pPattenPos.z + (float)random_z;
			m_iPattenFrame--;
			break;
		case -1:
			fDestX = GetX() +  m_pPattenPos.x + (float)random_x;
			fDestZ = GetZ() +  m_pPattenPos.z + (float)random_z;
			m_iPattenFrame--;
			break;
		case 0:
			fDestX = (short)m_nInitX;
			fDestZ = (short)m_nInitZ;
			m_iPattenFrame = myrand(-1,1);
			break;
		case 1:
			fDestX = GetX() + m_pPattenPos.x + (float)random_x;
			fDestZ = GetZ() + m_pPattenPos.z + (float)random_z;
			m_iPattenFrame++;
			break;
		case 2:
			fDestX = GetX() + m_pPattenPos.x + (float)random_x;
			fDestZ = GetZ() + m_pPattenPos.z + (float)random_z;
			m_iPattenFrame++;
			break;
		case 3:
			fDestX = GetX() +  m_pPattenPos.x - (float)random_x;
			fDestZ = GetZ() +  m_pPattenPos.z - (float)random_z;
			m_iPattenFrame++;
			break;
		case 4:
			fDestX = GetX() +  m_pPattenPos.x - (float)random_x;
			fDestZ = GetZ() +  m_pPattenPos.z - (float)random_z;
			m_iPattenFrame = 0;
			break;
		}

		vStart.Set(GetX(), GetY(), GetZ());
		vEnd.Set(fDestX, 0, fDestZ);
		fDis = GetDistance(vStart, vEnd);
		GetVectorPosition(vStart, vEnd, 40, &vNewPos);
	}
	else if (m_byMoveType == 3)	
	{
		if (m_sPathCount == m_sMaxPathCount)	
		{
			m_byMoveType = 0;
			m_sPathCount = 0;
			return false;
		}

		if (m_sPathCount != 0 && IsInPathRange() == false)	
		{
			m_sPathCount--;
			nPathCount = GetNearPathPoint();
			if (nPathCount  == -1)	
			{
				TRACE("##### RandomMove Fail : [nid = %d, sid=%d], path = %d/%d, 이동할 수 있는 거리에서 너무 멀어졌당,, 어케해 #####\n", GetID(), GetProtoID(), m_sPathCount, m_sMaxPathCount);
				vStart.Set(GetX(), GetY(), GetZ());
				fDestX = (float)m_PathList.pPattenPos[0].x + m_fBattlePos_x;
				fDestZ = (float)m_PathList.pPattenPos[0].z + m_fBattlePos_z;
				vEnd.Set(fDestX, 0, fDestZ);
				GetVectorPosition(vStart, vEnd, 40, &vNewPos);
				fDestX = vNewPos.x;
				fDestZ = vNewPos.z;
			}
			else	
			{
				if (nPathCount < 0)	
					return false;

				fDestX = (float)m_PathList.pPattenPos[nPathCount].x + m_fBattlePos_x;
				fDestZ = (float)m_PathList.pPattenPos[nPathCount].z + m_fBattlePos_x;
				m_sPathCount = nPathCount;
			}
		}
		else	
		{
			if (m_sPathCount < 0)	
				return false;

			fDestX = (float)m_PathList.pPattenPos[m_sPathCount].x + m_fBattlePos_x;
			fDestZ = (float)m_PathList.pPattenPos[m_sPathCount].z + m_fBattlePos_x;
		}

		m_sPathCount++;
	}

	vStart.Set(GetX(), 0, GetZ());
	vEnd.Set(fDestX, 0, fDestZ);

	if (GetX() < 0 || GetZ() < 0 || fDestX < 0 || fDestZ < 0)	
	{
		TRACE("##### RandomMove Fail : value is negative.. [nid = %d, name=%s], cur_x=%.2f, z=%.2f, dest_x=%.2f, dest_z=%.2f#####\n", 
			GetID(), GetName().c_str(), GetX(), GetZ(), fDestX, fDestZ);
		return false;
	}

	int mapWidth = (int)(max_xx * GetMap()->GetUnitDistance());

	if (GetX() >= mapWidth || GetZ() >= mapWidth || fDestX >= mapWidth || fDestZ >= mapWidth)	
	{
		TRACE("##### RandomMove Fail : value is overflow .. [nid = %d, name=%s], cur_x=%.2f, z=%.2f, dest_x=%.2f, dest_z=%.2f#####\n", 
			GetID(), GetName().c_str(), GetX(), GetZ(), fDestX, fDestZ);
		return false;
	}

	if (GetType() == NPC_DUNGEON_MONSTER
		&& !isInSpawnRange((int)fDestX, (int)fDestZ))
		return false;	

	fDis = GetDistance(vStart, vEnd);
	if (fDis > NPC_MAX_MOVE_RANGE)
	{
		if (m_byMoveType == 2 || m_byMoveType == 3)	
		{
			if (--m_sPathCount <= 0) 
				m_sPathCount=0;
		}

		//TRACE("##### RandomMove Fail : NPC_MAX_MOVE_RANGE overflow  .. [nid = %d, name=%s], cur_x=%.2f, z=%.2f, dest_x=%.2f, dest_z=%.2f, fDis=%.2f#####\n", GetID(), GetName().c_str(), GetX(), GetZ(), fDestX, fDestZ, fDis);
		return false;
	}

	if (fDis <= m_fSecForMetor)		
	{
		ClearPathFindData();
		m_fStartPoint_X = GetX();		
		m_fStartPoint_Y = GetZ();
		m_fEndPoint_X = fDestX;			
		m_fEndPoint_Y = fDestZ;
		m_bPathFlag = true;
		m_iAniFrameIndex = 1;
		m_pPoint[0].fXPos = m_fEndPoint_X;
		m_pPoint[0].fZPos = m_fEndPoint_Y;
		return true;
	}

	float fTempRange = (float)fDis+2;
	int min_x = (int)(GetX() - fTempRange)/TILE_SIZE;	if(min_x < 0) min_x = 0;
	int min_z = (int)(GetZ() - fTempRange)/TILE_SIZE;	if(min_z < 0) min_z = 0;
	int max_x = (int)(GetX() + fTempRange)/TILE_SIZE;	if(max_x >= max_xx) max_x = max_xx - 1;
	int max_z = (int)(GetZ() + fTempRange)/TILE_SIZE;	if(min_z >= max_zz) min_z = max_zz - 1;

	CPoint start, end;
	start.x = (int)(GetX()/TILE_SIZE) - min_x;
	start.y = (int)(GetZ()/TILE_SIZE) - min_z;
	end.x = (int)(fDestX/TILE_SIZE) - min_x;
	end.y = (int)(fDestZ/TILE_SIZE) - min_z;

	if(start.x < 0 || start.y < 0 || end.x < 0 || end.y < 0)	return false;

	m_fStartPoint_X = GetX();		m_fStartPoint_Y = GetZ();
	m_fEndPoint_X = fDestX;	m_fEndPoint_Y = fDestZ;

	m_min_x = min_x;
	m_min_y = min_z;
	m_max_x = max_x;
	m_max_y = max_z;

	if (m_byMoveType == 2 || m_byMoveType == 3) 	
	{
		IsNoPathFind(m_fSecForMetor);
		return true;
	}

	return PathFind(start, end, m_fSecForMetor) == 1;
}

bool CNpc::RandomBackMove()
{
	m_fSecForMetor = m_fSpeed_2;

	if (m_bySearchRange == 0) return false;

	float fDestX = -1.0f, fDestZ = -1.0f;
	if (GetMap() == nullptr) 
	{
		TRACE("#### Npc-RandomBackMove Zone Fail : [name=%s], zone=%d #####\n", GetName().c_str(), GetZoneID());
		return false;
	}

	int max_xx = GetMap()->GetMapSize();
	int max_zz = GetMap()->GetMapSize();
	int x = 0, y = 0;
	float fTempRange = (float)m_bySearchRange*2;
	int min_x = (int)(GetX() - fTempRange)/TILE_SIZE;	if(min_x < 0) min_x = 0;
	int min_z = (int)(GetZ() - fTempRange)/TILE_SIZE;	if(min_z < 0) min_z = 0;
	int max_x = (int)(GetX() + fTempRange)/TILE_SIZE;	if(max_x > max_xx) max_x = max_xx;
	int max_z = (int)(GetZ() + fTempRange)/TILE_SIZE;	if(max_z > max_zz) max_z = max_zz;

	__Vector3 vStart, vEnd, vEnd22;
	float fDis = 0.0f;
	vStart.Set(GetX(), GetY(), GetZ());

	uint16 nID = m_Target.id;					// Target 을 구한다.
	CUser* pUser = nullptr;

	int iDir = 0;

	int iRandomX = 0, iRandomZ = 0, iRandomValue=0;
	iRandomValue = rand() % 2;

	// Head towards player
	if (nID < NPC_BAND)
	{
		pUser = g_pMain->GetUserPtr(nID);
		if (pUser == nullptr)	
			return false;

		if((int)pUser->GetX() != (int)GetX())  
		{
			iRandomX = myrand((int)m_bySearchRange, (int)(m_bySearchRange*1.5));
			iRandomZ = myrand(0, (int)m_bySearchRange);

			if((int)pUser->GetX() > (int)GetX())  
				iDir = 1;
			else
				iDir = 2;
		}
		else	// z축으로
		{
			iRandomZ = myrand((int)m_bySearchRange, (int)(m_bySearchRange*1.5));
			iRandomX = myrand(0, (int)m_bySearchRange);
			if((int)pUser->GetZ() > (int)GetZ())  
				iDir = 3;
			else
				iDir = 4;
		}

		switch(iDir)
		{
		case 1:
			fDestX = GetX() - iRandomX;
			if(iRandomValue == 0)
				fDestZ = GetZ() - iRandomX;
			else
				fDestZ = GetZ() + iRandomX;
			break;
		case 2:
			fDestX = GetX() + iRandomX;
			if(iRandomValue == 0)
				fDestZ = GetZ() - iRandomX;
			else
				fDestZ = GetZ() + iRandomX;
			break;
		case 3:
			fDestZ = GetZ() - iRandomX;
			if(iRandomValue == 0)
				fDestX = GetX() - iRandomX;
			else
				fDestX = GetX() + iRandomX;
			break;
		case 4:
			fDestZ = GetZ() - iRandomX;
			if(iRandomValue == 0)
				fDestX = GetX() - iRandomX;
			else
				fDestX = GetX() + iRandomX;
			break;
		}

		vEnd.Set(fDestX, 0, fDestZ);
		fDis = GetDistance(vStart, vEnd);
		if(fDis > 20)	// 20미터 이상이면 20미터로 맞춘다,,
		{
			GetVectorPosition(vStart, vEnd, 20, &vEnd22);
			fDestX = vEnd22.x;
			fDestZ = vEnd22.z;
		}
	}
	// Head towards monster/NPC
	else 
	{
	}

	CPoint start, end;
	start.x = (int)(GetX()/TILE_SIZE) - min_x;
	start.y = (int)(GetZ()/TILE_SIZE) - min_z;
	end.x = (int)(fDestX/TILE_SIZE) - min_x;
	end.y = (int)(fDestZ/TILE_SIZE) - min_z;

	if (start.x < 0 || start.y < 0 || end.x < 0 || end.y < 0)
		return false;

	m_fStartPoint_X = GetX();		m_fStartPoint_Y = GetZ();
	m_fEndPoint_X = fDestX;	m_fEndPoint_Y = fDestZ;

	m_min_x = min_x;
	m_min_y = min_z;
	m_max_x = max_x;
	m_max_y = max_z;

	int nValue = PathFind(start, end, m_fSecForMetor);
	if (nValue == 1)
		return true;

	return false;
}

bool CNpc::IsInPathRange()
{
	if (m_sPathCount < 0)	
		return false;

	static const float fPathRange = 40.0f;
	return isInRangeSlow((float)m_PathList.pPattenPos[m_sPathCount].x + m_fBattlePos_x, 
		(float)m_PathList.pPattenPos[m_sPathCount].z + m_fBattlePos_z,
		fPathRange + 1);
}

int CNpc::GetNearPathPoint()
{
	__Vector3 vStart, vEnd;
	float fMaxPathRange = (float)NPC_MAX_MOVE_RANGE;
	float fDis1 = 0.0f, fDis2 = 0.0f;
	int nRet = -1;
	vStart.Set(GetX(), GetY(), GetZ());
	vEnd.Set((float)m_PathList.pPattenPos[m_sPathCount].x + m_fBattlePos_x, 0, (float)m_PathList.pPattenPos[m_sPathCount].z + m_fBattlePos_z);
	fDis1 = GetDistance(vStart, vEnd);

	if(m_sPathCount+1 >= m_sMaxPathCount)	{
		if(m_sPathCount-1 > 0)	{
			vEnd.Set((float)m_PathList.pPattenPos[m_sPathCount-1].x + m_fBattlePos_x, 0, (float)m_PathList.pPattenPos[m_sPathCount-1].z + m_fBattlePos_z);
			fDis2 = GetDistance(vStart, vEnd);
		}
		else {
			vEnd.Set((float)m_PathList.pPattenPos[0].x + m_fBattlePos_x, 0, (float)m_PathList.pPattenPos[0].z + m_fBattlePos_z);
			fDis2 = GetDistance(vStart, vEnd);
		}
	}
	else
	{
		vEnd.Set((float)m_PathList.pPattenPos[m_sPathCount+1].x + m_fBattlePos_x, 0, (float)m_PathList.pPattenPos[m_sPathCount+1].z + m_fBattlePos_z);
		fDis2 = GetDistance(vStart, vEnd);
	}

	if(fDis1 <= fDis2)
	{
		if(fDis1 <= fMaxPathRange)
			nRet =  m_sPathCount;
	}
	else
	{
		if(fDis2 <= fMaxPathRange)
			nRet =  m_sPathCount+1;
	}

	return nRet;
}

bool CNpc::isInSpawnRange(int nX, int nZ)
{
	CRect r(m_nLimitMinX, m_nLimitMinZ, m_nLimitMaxX, m_nLimitMaxZ);
	return r.PtInRect(nX, nZ);
}

/////////////////////////////////////////////////////////////////////////////////////////
//	PathFind 를 수행한다.
//
int CNpc::PathFind(CPoint start, CPoint end, float fDistance)
{
	ClearPathFindData();

	if (start.x < 0 || start.y < 0 
		|| end.x < 0 || end.y < 0)
		return -1;

	if (start.x == end.x && start.y == end.y)
	{
		m_bPathFlag = true;
		m_iAniFrameIndex = 1;
		m_pPoint[0].fXPos = m_fEndPoint_X;
		m_pPoint[0].fZPos = m_fEndPoint_Y;
		return 1;
	}

	if (IsPathFindCheck(fDistance))
	{
		m_bPathFlag = true;
		return 1;
	}

	m_vMapSize.cx = m_max_x - m_min_x + 1;
	m_vMapSize.cy = m_max_y - m_min_y + 1;

	m_pPath = nullptr;

	m_vPathFind.SetMap(m_vMapSize.cx, m_vMapSize.cy, GetMap(), m_min_x, m_min_y);
	m_pPath = m_vPathFind.FindPath(end.x, end.y, start.x, start.y);

	int count = 0;
	while (m_pPath != nullptr)
	{
		m_pPath = m_pPath->Parent;
		if (m_pPath == nullptr)			
			break;

		m_pPoint[count].pPoint.x = m_pPath->x + m_min_x;		
		m_pPoint[count++].pPoint.y = m_pPath->y + m_min_y;
	}	

	if (count <= 0 || count >= MAX_PATH_LINE)
		return 0;

	m_iAniFrameIndex = count - 1;
	int nAdd = GetDir(m_fStartPoint_X, m_fStartPoint_Y, m_fEndPoint_X, m_fEndPoint_Y);

	for (int i = 0; i < count; i++)
	{
		if (i == (count - 1))
		{
			m_pPoint[i].fXPos = m_fEndPoint_X;
			m_pPoint[i].fZPos = m_fEndPoint_Y;
		}
		else
		{
			m_pPoint[i].fXPos = (float)(m_pPoint[i].pPoint.x * TILE_SIZE + m_fAdd_x);
			m_pPoint[i].fZPos = (float)(m_pPoint[i].pPoint.y * TILE_SIZE + m_fAdd_z);
		}
	}

	return 1;
}

void CNpc::Dead(Unit * pKiller /*= nullptr*/, bool bSendDeathPacket /*= false*/)
{
	MAP* pMap = GetMap();
	if (pMap == nullptr)
		return;

	m_iHP = 0;
	m_NpcState = NPC_DEAD;
	m_Delay = m_sRegenTime;
	m_bFirstLive = false;
	m_byDeadType = 100;		// 전쟁이벤트중에서 죽는 경우

	if (GetRegionX() > pMap->GetXRegionMax() || GetRegionZ() > pMap->GetZRegionMax())
	{
		TRACE("#### Npc-Dead() Fail : [nid=%d, sid=%d], nRX=%d, nRZ=%d #####\n", 
			GetID(), GetProtoID(), GetRegionX(), GetRegionZ());
		return;
	}

	pMap->RegionNpcRemove(GetRegionX(), GetRegionZ(), GetID());

	if (bSendDeathPacket)
	{
		SendDeathAnimation(pKiller);
		SendExpToUserList();
		if (isShowBox())
			GiveNpcHaveItem(); 
	}
}

bool CNpc::isShowBox()
{ 
	uint8 bType = GetType();

	if (bType == NPC_CHAOS_STONE
		|| bType == NPC_PVP_MONUMENT
		|| bType == NPC_BIFROST_MONUMENT
		|| bType == NPC_GUARD_TOWER1
		|| bType == NPC_GUARD_TOWER2
		|| bType == NPC_SCARECROW
		|| bType == NPC_KARUS_WARDER1
		|| bType == NPC_KARUS_WARDER2
		|| bType == NPC_ELMORAD_WARDER1
		|| bType == NPC_ELMORAD_WARDER2
		|| bType == NPC_KARUS_GATEKEEPER
		|| bType == NPC_ELMORAD_GATEKEEPER
		|| bType == NPC_BATTLE_MONUMENT
		|| bType == NPC_KARUS_MONUMENT
		|| bType == NPC_HUMAN_MONUMENT
		|| GetZoneID() == ZONE_FORGOTTEN_TEMPLE
		|| GetZoneID() == ZONE_PRISON)
		return false;

	return true;
} 

bool CNpc::FindEnemy()
{
	if (isNonAttackingObject())
		return false;

	bool bIsGuard = isGuard();

	// We shouldn't really need this anymore...
	bool bIsNeutralZone = (GetZoneID() == ZONE_MORADON || GetZoneID() == ZONE_ARENA); 

	// Disable AI enemy finding (of users) in neutral zones.
	// Guards and monsters are, however, allowed.
	if (!isMonster()
		&& !bIsGuard 
		&& bIsNeutralZone)
		return false;

	// Healer Npc
	int iMonsterNid = 0;
	if (isHealer())
	{
		iMonsterNid = FindFriend(MonSearchNeedsHealing);
		if (iMonsterNid != 0)
			return true;
	}

	MAP* pMap = GetMap();
	if (pMap == nullptr)	return false;
	CUser *pUser = nullptr;
	CNpc *pNpc = nullptr;

	int target_uid = 0;
	__Vector3 vUser, vNpc;
	float fDis = 0.0f;
	float fCompareDis = 0.0f;
	vNpc.Set(GetX(), GetY(), GetZ());

	float fSearchRange = (float)m_bySearchRange;

	int iExpand = FindEnemyRegion();

	if (GetRegionX() > pMap->GetXRegionMax() 
		|| GetRegionZ() > pMap->GetZRegionMax())
		return false;

	/*** If we're a monster, we can find user enemies anywhere. If we're an NPC, we must not be friendly. ***/
	if (isMonster() 
		|| (!GetMap()->areNPCsFriendly() || GetNation() != Nation::ALL))
	{
		fCompareDis = FindEnemyExpand(GetRegionX(), GetRegionZ(), fCompareDis, UnitPlayer);

		int x=0, y=0;
		// 이웃해 있는 Region을 검색해서,,  몬의 위치와 제일 가까운 User을 향해.. 이동..
		for(int l=0; l<4; l++)	{
			if(m_iFind_X[l] == 0 && m_iFind_Y[l] == 0)		continue;

			x = GetRegionX() + (m_iFind_X[l]);
			y = GetRegionZ() + (m_iFind_Y[l]);

			// 이부분 수정요망,,
			if (x < 0 || y < 0 || x > pMap->GetXRegionMax() || y > pMap->GetZRegionMax())		continue;

			fCompareDis = FindEnemyExpand(x, y, fCompareDis, UnitPlayer);
		}

		if (hasTarget() && (fCompareDis <= fSearchRange))		
			return true;

		fCompareDis = 0.0f;
	}

	/*** Only find NPC/mob enemies if we are a guard ***/
	if (bIsGuard) // || GetType() == NPCTYPE_MONSTER)	
	{
		fCompareDis = FindEnemyExpand(GetRegionX(), GetRegionZ(), fCompareDis, UnitNPC);

		int x=0, y=0;

		// 이웃해 있는 Region을 검색해서,,  몬의 위치와 제일 가까운 User을 향해.. 이동..
		for(int l=0; l<4; l++)	{
			if(m_iFind_X[l] == 0 && m_iFind_Y[l] == 0)			continue;

			x = GetRegionX() + (m_iFind_X[l]);
			y = GetRegionZ() + (m_iFind_Y[l]);

			// 이부분 수정요망,,
			if(x < 0 || y < 0 || x > pMap->GetXRegionMax() || y > pMap->GetZRegionMax())	continue;

			fCompareDis = FindEnemyExpand(x, y, fCompareDis, UnitNPC);
		}

		if (hasTarget() && (fCompareDis <= fSearchRange))
			return true;
	}

	// 아무도 없으므로 리스트에 관리하는 유저를 초기화한다.
	InitUserList();		
	InitTarget();
	return false;
}

// Npc가 유저를 검색할때 어느 Region까지 검색해야 하는지를 판단..
int CNpc::FindEnemyRegion()
{
	/*
	1	2	3
	4	0	5
	6	7	8
	*/
	int iRetValue = 0;
	int  iSX = GetRegionX() * VIEW_DIST;
	int  iSZ = GetRegionZ() * VIEW_DIST;
	int  iEX = (GetRegionX()+1) * VIEW_DIST;
	int  iEZ = (GetRegionZ()+1) * VIEW_DIST;
	int  iSearchRange = m_bySearchRange;
	int iCurSX = (int)GetX() - m_bySearchRange;
	int iCurSY = (int)GetX() - m_bySearchRange;
	int iCurEX = (int)GetX() + m_bySearchRange;
	int iCurEY = (int)GetX() + m_bySearchRange;

	int iMyPos = GetMyField();

	switch(iMyPos)
	{
	case 1:
		if((iCurSX < iSX) && (iCurSY < iSZ))
			iRetValue = 1;
		else if((iCurSX > iSX) && (iCurSY < iSZ))
			iRetValue = 2;
		else if((iCurSX < iSX) && (iCurSY > iSZ))
			iRetValue = 4;
		else if((iCurSX >= iSX) && (iCurSY >= iSZ))
			iRetValue = 0;
		break;
	case 2:
		if((iCurEX < iEX) && (iCurSY < iSZ))
			iRetValue = 2;
		else if((iCurEX > iEX) && (iCurSY < iSZ))
			iRetValue = 3;
		else if((iCurEX <= iEX) && (iCurSY >= iSZ))
			iRetValue = 0;
		else if((iCurEX > iEX) && (iCurSY > iSZ))
			iRetValue = 5;
		break;
	case 3:
		if((iCurSX < iSX) && (iCurEY < iEZ))
			iRetValue = 4;
		else if((iCurSX >= iSX) && (iCurEY <= iEZ))
			iRetValue = 0;
		else if((iCurSX < iSX) && (iCurEY > iEZ))
			iRetValue = 6;
		else if((iCurSX > iSX) && (iCurEY > iEZ))
			iRetValue = 7;
		break;
	case 4:
		if((iCurEX <= iEX) && (iCurEY <= iEZ))
			iRetValue = 0;
		else if((iCurEX > iEX) && (iCurEY < iEZ))
			iRetValue = 5;
		else if((iCurEX < iEX) && (iCurEY > iEZ))
			iRetValue = 7;
		else if((iCurEX > iEX) && (iCurEY > iEZ))
			iRetValue = 8;
		break;
	}

	if(iRetValue <= 0) // 임시로 보정(문제시),, 하기 위한것..
		iRetValue = 0;

	switch(iRetValue)
	{
	case 0:
		m_iFind_X[0] = 0;  m_iFind_Y[0] = 0;
		m_iFind_X[1] = 0;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = 0;  m_iFind_Y[2] = 0;
		m_iFind_X[3] = 0;  m_iFind_Y[3] = 0;
		break;
	case 1:
		m_iFind_X[0] = -1;  m_iFind_Y[0] = -1;
		m_iFind_X[1] = 0;  m_iFind_Y[1] = -1;
		m_iFind_X[2] = -1;  m_iFind_Y[2] = 0;
		m_iFind_X[3] = 0;  m_iFind_Y[3] = 0;
		break;
	case 2:
		m_iFind_X[0] = 0;  m_iFind_Y[0] = -1;
		m_iFind_X[1] = 0;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = 0;  m_iFind_Y[2] = 0;
		m_iFind_X[3] = 0;  m_iFind_Y[3] = 0;
		break;
	case 3:
		m_iFind_X[0] = 0;  m_iFind_Y[0] = 0;
		m_iFind_X[1] = 1;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = 0;  m_iFind_Y[2] = 1;
		m_iFind_X[3] = 1;  m_iFind_Y[3] = 1;
		break;
	case 4:
		m_iFind_X[0] = -1;  m_iFind_Y[0] = 0;
		m_iFind_X[1] = 0;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = 0;  m_iFind_Y[2] = 0;
		m_iFind_X[3] = 0;  m_iFind_Y[3] = 0;
		break;
	case 5:
		m_iFind_X[0] = 0;  m_iFind_Y[0] = 0;
		m_iFind_X[1] = 1;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = 0;  m_iFind_Y[2] = 0;
		m_iFind_X[3] = 0;  m_iFind_Y[3] = 0;
		break;
	case 6:
		m_iFind_X[0] = -1;  m_iFind_Y[0] = 0;
		m_iFind_X[1] = 0;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = -1;  m_iFind_Y[2] = 1;
		m_iFind_X[3] = 0;  m_iFind_Y[3] = 1;
		break;
	case 7:
		m_iFind_X[0] = 0;  m_iFind_Y[0] = 0;
		m_iFind_X[1] = 0;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = 0;  m_iFind_Y[2] = 1;
		m_iFind_X[3] = 0;  m_iFind_Y[3] = 0;
		break;
	case 8:
		m_iFind_X[0] = 0;  m_iFind_Y[0] = 0;
		m_iFind_X[1] = 1;  m_iFind_Y[1] = 0;
		m_iFind_X[2] = 0;  m_iFind_Y[2] = 1;
		m_iFind_X[3] = 1;  m_iFind_Y[3] = 1;
		break;
	}

	return iRetValue;
}

float CNpc::FindEnemyExpand(int nRX, int nRZ, float fCompDis, UnitType unitType)
{
	MAP* pMap = GetMap();
	float fDis = 0.0f;
	if(pMap == nullptr)	return fDis;
	float fComp = fCompDis;
	float fSearchRange = (float)m_bySearchRange;
	uint16 target_uid;
	__Vector3 vUser, vNpc, vMon;
	vNpc.Set(GetX(), GetY(), GetZ());

	// Finding players
	if (unitType == UnitPlayer)	
	{
		Guard lock(pMap->m_lock);
		CRegion *pRegion = &pMap->m_ppRegion[nRX][nRZ];

		//TRACE("FindEnemyExpand type1,, region_x=%d, region_z=%d, user=%d, mon=%d\n", nRX, nRZ, nUser, nMonster);
		if (pRegion == nullptr || (pRegion && pRegion->m_RegionUserArray.IsEmpty()))
			return 0.0f;

		foreach_stlmap (itr, pRegion->m_RegionUserArray)
		{
			CUser *pUser = g_pMain->GetUserPtr(*itr->second);
			if (pUser == nullptr 
				|| pUser->isDead()
				|| !CanAttack(pUser)
				|| pUser->m_bInvisibilityType
				|| pUser->isGM())
				continue;

			float fDis = Unit::GetDistanceSqrt(pUser);
			if (fDis > fSearchRange
				|| fDis < fComp)
				continue;

			target_uid = pUser->GetID();
			fComp = fDis;

			// Aggressive spawns don't mind attacking first.
			if (m_tNpcAttType == ATROCITY_ATTACK_TYPE
				// Passive spawns will only attack if they've been attacked first, or they've got backup! (Cowards!)
					|| (m_tNpcAttType == TENDER_ATTACK_TYPE && (IsDamagedUserList(pUser) || (m_bHasFriends && m_Target.id == target_uid))))
			{
				m_Target.id	= target_uid;
				m_Target.bSet = true;
				m_Target.x	= pUser->GetX();
				m_Target.y	= pUser->GetY();
				m_Target.z	= pUser->GetZ();
			}
		}
	}
	// Finding NPCs/monsters
	else if (unitType == UnitNPC)		
	{
		Guard lock(pMap->m_lock);
		CRegion *pRegion = &pMap->m_ppRegion[nRX][nRZ];

		if (pRegion == nullptr || (pRegion && pRegion->m_RegionNpcArray.IsEmpty()))
			return 0.0f;

		foreach_stlmap (itr, pRegion->m_RegionNpcArray)
		{
			int nNpcid = *itr->second;
			if( nNpcid < NPC_BAND )	continue;
			CNpc *pNpc = g_pMain->GetNpcPtr(nNpcid);

			if (pNpc == nullptr
				|| pNpc == this
				|| pNpc->isDead()
				|| pNpc->isNonAttackableObject()
				|| !isHostileTo(pNpc))
				continue;

			vMon.Set(pNpc->GetX(), pNpc->GetY(), pNpc->GetZ()); 
			fDis = GetDistance(vMon, vNpc);

			if (fDis > fSearchRange || fDis < fComp)
				continue;

			target_uid = nNpcid;
			fComp = fDis;
			m_Target.id	= target_uid;
			m_Target.bSet = true;
			m_Target.x	= pNpc->GetX();
			m_Target.y	= pNpc->GetY();
			m_Target.z	= pNpc->GetZ();
		}
	}

	return fComp;
}

// region을 4등분해서 몬스터의 현재 위치가 region의 어느 부분에 들어가는지를 판단
int CNpc::GetMyField()
{
	int iRet = 0;
	int iX = GetRegionX() * VIEW_DIST;
	int iZ = GetRegionZ() * VIEW_DIST;
	int iAdd = VIEW_DIST / 2;
	int iCurX = (int)GetX();	// monster current position_x
	int iCurZ = (int)GetZ();
	if(COMPARE(iCurX, iX, iX+iAdd) && COMPARE(iCurZ, iZ, iZ+iAdd))
		iRet = 1;
	else if(COMPARE(iCurX, iX+iAdd, iX+VIEW_DIST) && COMPARE(iCurZ, iZ, iZ+iAdd))
		iRet = 2;
	else if(COMPARE(iCurX, iX, iX+iAdd) && COMPARE(iCurZ, iZ+iAdd, iZ+VIEW_DIST))
		iRet = 3;
	else if(COMPARE(iCurX, iX+iAdd, iX+VIEW_DIST) && COMPARE(iCurZ, iZ+iAdd, iZ+VIEW_DIST))
		iRet = 4;

	return iRet;
}

bool CNpc::IsDamagedUserList(CUser *pUser)
{
	if (pUser == nullptr) 
		return false;

	Guard lock(m_damageListLock);
	return (m_DamagedUserList.find(pUser->GetID()) != m_DamagedUserList.end());
}

int CNpc::IsSurround(CUser* pUser)
{
	if (GetProto()->m_byDirectAttack) 
		return 0;

	if(pUser == nullptr)	return -2;		// User가 없으므로 타겟지정 실패..
	int nDir = pUser->IsSurroundCheck(GetX(), 0.0f, GetZ(), GetID());
	if(nDir != 0)
	{
		m_byAttackPos = nDir;
		return nDir;
	}
	return -1;					// 타겟이 둘러 쌓여 있음...
}

//	Path Find 로 찾은길을 다 이동 했는지 판단
bool CNpc::IsMovingEnd()
{
	if (m_fPrevX == m_fEndPoint_X && m_fPrevZ == m_fEndPoint_Y) 
	{
		//m_sStepCount = 0;
		m_iAniFrameCount = 0;
		return true;
	}

	return false;
}

//	Step 수 만큼 타켓을 향해 이동한다.
bool CNpc::StepMove()
{
	if(m_NpcState != NPC_MOVING && m_NpcState != NPC_TRACING && m_NpcState != NPC_BACK) return false;

	__Vector3 vStart, vEnd, vDis;
	float fDis;

	float fOldCurX = 0.0f, fOldCurZ = 0.0f;

	if(m_sStepCount == 0)	{
		fOldCurX = GetX();  fOldCurZ = GetZ();
	}
	else	{
		fOldCurX = m_fPrevX; fOldCurZ = m_fPrevZ;
	}

	vStart.Set(fOldCurX, 0, fOldCurZ);
	vEnd.Set(m_pPoint[m_iAniFrameCount].fXPos, 0, m_pPoint[m_iAniFrameCount].fZPos);

	// 안전 코드..
	if(m_pPoint[m_iAniFrameCount].fXPos < 0 || m_pPoint[m_iAniFrameCount].fZPos < 0)
	{
		m_fPrevX = m_fEndPoint_X;
		m_fPrevZ = m_fEndPoint_Y;

		RegisterRegion(m_fPrevX, m_fPrevZ);
		return false;	
	}

	fDis = GetDistance(vStart, vEnd);
	if (fDis >= m_fSecForMetor)
	{
		GetVectorPosition(vStart, vEnd, m_fSecForMetor, &vDis);
		m_fPrevX = vDis.x;
		m_fPrevZ = vDis.z;
	}
	else
	{
		m_iAniFrameCount++;
		if(m_iAniFrameCount == m_iAniFrameIndex)
		{
			vEnd.Set(m_pPoint[m_iAniFrameCount].fXPos, 0, m_pPoint[m_iAniFrameCount].fZPos);
			fDis = GetDistance(vStart, vEnd);
			// 마지막 좌표는 m_fSecForMetor ~ m_fSecForMetor+1 사이도 가능하게 이동
			if(fDis > m_fSecForMetor)
			{
				GetVectorPosition(vStart, vEnd, m_fSecForMetor, &vDis);
				m_fPrevX = vDis.x;
				m_fPrevZ = vDis.z;
				//				m_iAniFrameCount--;
			}
			else
			{
				m_fPrevX = m_fEndPoint_X;
				m_fPrevZ = m_fEndPoint_Y;
			}
		}
		else
		{
			vEnd.Set(m_pPoint[m_iAniFrameCount].fXPos, 0, m_pPoint[m_iAniFrameCount].fZPos);
			fDis = GetDistance(vStart, vEnd);
			if(fDis >= m_fSecForMetor)
			{
				GetVectorPosition(vStart, vEnd, m_fSecForMetor, &vDis);
				m_fPrevX = vDis.x;
				m_fPrevZ = vDis.z;
			}
			else
			{
				m_fPrevX = m_fEndPoint_X;
				m_fPrevZ = m_fEndPoint_Y;
			}
		}
	}

	vStart.Set(fOldCurX, 0, fOldCurZ);
	vEnd.Set(m_fPrevX, 0, m_fPrevZ);

	m_fSecForRealMoveMetor = GetDistance(vStart, vEnd);

	if(m_fSecForRealMoveMetor > m_fSecForMetor+1)
	{
		TRACE("#### move fail : [nid = %d], m_fSecForMetor = %.2f\n", GetID(), m_fSecForRealMoveMetor);
	}

	if (m_sStepCount++ > 0)
	{
		m_curx = fOldCurX;		 m_curz = fOldCurZ;
		if(GetX() < 0 || GetZ() < 0)
			TRACE("Npc-StepMove : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), GetX(), GetZ());

		return RegisterRegion(GetX(), GetZ());
	}

	return true;
}

bool CNpc::StepNoPathMove()
{
	if(m_NpcState != NPC_MOVING && m_NpcState != NPC_TRACING && m_NpcState != NPC_BACK) return false;

	__Vector3 vStart, vEnd;
	float fOldCurX = 0.0f, fOldCurZ = 0.0f;

	if(m_sStepCount == 0)	{
		fOldCurX = GetX(); fOldCurZ = GetZ();
	}
	else	{
		fOldCurX = m_fPrevX; fOldCurZ = m_fPrevZ;
	}


	if(m_sStepCount < 0 || m_sStepCount >= m_iAniFrameIndex)	{	
		TRACE("#### IsNoPtahfind Fail : nid=%d,%s, count=%d/%d ####\n", GetID(), GetName().c_str(), m_sStepCount, m_iAniFrameIndex);
		return false;
	}

	vStart.Set(fOldCurX, 0, fOldCurZ);
	m_fPrevX = m_pPoint[m_sStepCount].fXPos;
	m_fPrevZ = m_pPoint[m_sStepCount].fZPos;
	vEnd.Set(m_fPrevX, 0, m_fPrevZ);

	if(m_fPrevX == -1 || m_fPrevZ == -1)	{
		TRACE("##### StepNoPath Fail : nid=%d,%s, x=%.2f, z=%.2f #####\n", GetID(), GetName().c_str(), m_fPrevX, m_fPrevZ);
		return false;
	}

	m_fSecForRealMoveMetor = GetDistance(vStart, vEnd);

	if (m_sStepCount++ > 0)
	{
		if(fOldCurX < 0 || fOldCurZ < 0)	{
			TRACE("#### Npc-StepNoPathMove Fail : nid=(%d, %s), x=%.2f, z=%.2f\n", GetID(), GetName().c_str(), fOldCurX, fOldCurZ);
			return false;
		}
		else	
		{
			m_curx = fOldCurX;	 m_curz = fOldCurZ;
		}

		return RegisterRegion(GetX(), GetZ());
	}

	return true;
}

CloseTargetResult CNpc::IsCloseTarget(int nRange, AttackType attackType)
{
	if (!hasTarget())
		return CloseTargetInvalid;

	CUser * pUser = nullptr;
	CNpc * pNpc = nullptr;
	__Vector3 vUser, vWillUser, vNpc, vDistance;
	float fDis = 0.0f, fWillDis = 0.0f, fX = 0.0f, fZ = 0.0f;
	bool  bUserType = false;	// 타겟이 유저이면 true
	vNpc.Set(GetX(), GetY(), GetZ());

	if (m_Target.id < NPC_BAND)	
	{
		pUser = g_pMain->GetUserPtr(m_Target.id);
		if (pUser == nullptr)	
		{
			InitTarget();
			return CloseTargetInvalid;
		}

		vUser.Set(pUser->GetX(), pUser->GetY(), pUser->GetZ()); 
		vWillUser.Set(pUser->m_fWill_x, pUser->m_fWill_y, pUser->m_fWill_z); 
		fX = pUser->GetX();		
		fZ = pUser->GetZ();

		vDistance = vWillUser - vNpc;
		fWillDis = vDistance.Magnitude();	
		fWillDis = fWillDis - m_proto->m_fBulk;
		bUserType = true;
	}
	else 
	{
		pNpc = g_pMain->GetNpcPtr(m_Target.id);
		if(pNpc == nullptr) 
		{
			InitTarget();
			return CloseTargetInvalid;
		}
		vUser.Set(pNpc->GetX(), pNpc->GetY(), pNpc->GetZ()); 
		fX = pNpc->GetX();
		fZ = pNpc->GetZ();
	}

	vDistance = vUser - vNpc;
	fDis = vDistance.Magnitude();	

	fDis = fDis - m_proto->m_fBulk;

	if (GetType() == NPC_DUNGEON_MONSTER
		&& !isInSpawnRange((int)vUser.x, (int)vUser.z))
		return CloseTargetInvalid;

	if (attackType == AttackTypePhysical)	
	{
		m_bStopFollowingTarget = true;
		if (pUser != nullptr)	
		{
			if (m_Target.x == pUser->GetX() && m_Target.z == pUser->GetZ())
				m_bStopFollowingTarget = false;
		}
	}

	if ((int)fDis > nRange)	
	{
		if (attackType == AttackTypeMagic)			
		{
			m_bStopFollowingTarget = true;
			m_Target.x = fX;
			m_Target.z = fZ;
		}

		return CloseTargetNotInRange; 
	}

	m_fEndPoint_X = GetX();
	m_fEndPoint_Y = GetZ();
	m_Target.x = fX;
	m_Target.z = fZ;

	if (GetProto()->m_byDirectAttack == 1)
	{
		if (fDis <= LONG_ATTACK_RANGE) return CloseTargetInGeneralRange;
		else if (fDis <= nRange) return CloseTargetInAttackRange;
	}
	else	
	{
		if (attackType == AttackTypeMagic)
		{
			if (fDis <= (SHORT_ATTACK_RANGE + m_proto->m_fBulk)) return CloseTargetInGeneralRange;
			else if (fDis <= nRange) return CloseTargetInAttackRange;

			if (bUserType && fWillDis > (SHORT_ATTACK_RANGE + m_proto->m_fBulk) && fWillDis <= nRange) 
				return CloseTargetInAttackRange;
		}
		else 
		{
			if (fDis <= (SHORT_ATTACK_RANGE + m_proto->m_fBulk)) return CloseTargetInGeneralRange;
			else if (fDis <= nRange) return CloseTargetInAttackRange;
		}
	}

	return CloseTargetNotInRange;
}

//	Target 과 NPC 간 Path Finding을 수행한다.
int CNpc::GetTargetPath(int option)
{
	int nInitType = m_byInitMoveType;
	if (m_byInitMoveType >= 100)
		nInitType -= 100;

	if (GetType() != NPC_MONSTER
		&& m_byMoveType != nInitType)
		m_byMoveType = nInitType;

	m_fSecForMetor = m_fSpeed_2;
	CUser* pUser = nullptr;
	CNpc* pNpc = nullptr;
	float iTempRange = 0.0f;
	__Vector3 vUser, vNpc, vDistance, vEnd22;
	float fDis = 0.0f;
	float fDegree = 0.0f, fTargetDistance = 0.0f;
	float fSurX = 0.0f, fSurZ = 0.0f;

	// Player
	if (m_Target.id < NPC_BAND)	
	{
		pUser = g_pMain->GetUserPtr(m_Target.id);
		if(pUser == nullptr
			|| pUser->isDead()
			|| pUser->GetZoneID() != GetZoneID())
		{
			InitTarget();
			return -1;
		}

		if(option == 1)	{	// magic이나 활등으로 공격 당했다면...
			vNpc.Set(GetX(), GetY(), GetZ());
			vUser.Set(pUser->GetX(), pUser->GetY(), pUser->GetZ()); 
			fDis = GetDistance(vNpc, vUser);
			if(fDis >= NPC_MAX_MOVE_RANGE)		return -1;	// 너무 거리가 멀어서,, 추적이 안되게..
			iTempRange = fDis + 10;
		}
		else	{
			iTempRange = (float)m_bySearchRange;				// 일시적으로 보정한다.
			if(IsDamagedUserList(pUser)) iTempRange = (float)m_byTracingRange;	// 공격받은 상태면 찾을 범위 증가.
			else iTempRange += 2;
		}

		if (m_bTracing
			&& !isInRangeSlow(m_fTracingStartX, m_fTracingStartZ, iTempRange))
		{
			InitTarget();
			return -1;
		}
	}
	// NPC
	else if(m_Target.id >= NPC_BAND && m_Target.id < INVALID_BAND)	{	// Target 이 mon 인 경우
		pNpc = g_pMain->GetNpcPtr(m_Target.id);
		if(pNpc == nullptr) {
			InitTarget();
			return false;
		}
		if(pNpc->m_iHP <= 0 || pNpc->m_NpcState == NPC_DEAD)	{
			InitTarget();
			return -1;
		}

		iTempRange = (float)m_byTracingRange;				// 일시적으로 보정한다.
	}

	MAP* pMap = GetMap();
	if (pMap == nullptr) 
		return -1;

	int max_xx = pMap->GetMapSize();
	int max_zz = pMap->GetMapSize();

	int min_x = (int)(GetX() - iTempRange)/TILE_SIZE;	if(min_x < 0) min_x = 0;
	int min_z = (int)(GetZ() - iTempRange)/TILE_SIZE;	if(min_z < 0) min_z = 0;
	int max_x = (int)(GetX() + iTempRange)/TILE_SIZE;	if(max_x > max_xx) max_x = max_xx;
	int max_z = (int)(GetZ() + iTempRange)/TILE_SIZE;	if(min_z > max_zz) min_z = max_zz;

	// Targeting player
	if (m_Target.id < NPC_BAND)	
	{
		if (pUser == nullptr)
			return -1;

		CRect r = CRect(min_x, min_z, max_x+1, max_z+1);
		if (!r.PtInRect((int)pUser->GetX()/TILE_SIZE, (int)pUser->GetZ()/TILE_SIZE))
		{
			TRACE("### Npc-GetTargetPath() User Fail return -1: [nid=%d] t_Name=%s, AttackPos=%d ###\n", GetID(), pUser->GetName().c_str(), m_byAttackPos);
			return -1;
		}

		m_fStartPoint_X = GetX();		m_fStartPoint_Y = GetZ();

		vNpc.Set(GetX(), GetY(), GetZ());
		vUser.Set(pUser->GetX(), pUser->GetY(), pUser->GetZ()); 

		IsSurround(pUser);

		if(m_byAttackPos > 0 && m_byAttackPos < 9)	{
			fDegree = (float)((m_byAttackPos-1)*45);
			fTargetDistance = 2.0f+m_proto->m_fBulk;
			ComputeDestPos(vUser, fDegree, fTargetDistance, &vEnd22);
			fSurX = vEnd22.x - vUser.x;			fSurZ = vEnd22.z - vUser.z;
			m_fEndPoint_X = vUser.x + fSurX;	m_fEndPoint_Y = vUser.z + fSurZ;
		}
		else
		{
			CalcAdaptivePosition(vNpc, vUser, 2.0f+m_proto->m_fBulk, &vEnd22);
			m_fEndPoint_X = vEnd22.x;	m_fEndPoint_Y = vEnd22.z;
		}
	}
	else 
	{
		if (pNpc == nullptr)
			return -1;

		CRect r = CRect(min_x, min_z, max_x+1, max_z+1);
		if (!r.PtInRect((int)pNpc->GetX()/TILE_SIZE, (int)pNpc->GetZ()/TILE_SIZE))
		{
			TRACE("### Npc-GetTargetPath() Npc Fail return -1: [nid=%d] t_Name=%s, AttackPos=%d ###\n", GetID(), pNpc->GetName().c_str(), m_byAttackPos);
			return -1;
		}

		m_fStartPoint_X = GetX();		m_fStartPoint_Y = GetZ();

		vNpc.Set(GetX(), GetY(), GetZ());
		vUser.Set(pNpc->GetX(), pNpc->GetY(), pNpc->GetZ()); 

		CalcAdaptivePosition(vNpc, vUser, 2.0f+m_proto->m_fBulk, &vEnd22);
		m_fEndPoint_X = vEnd22.x;	m_fEndPoint_Y = vEnd22.z;
	}

	vDistance = vEnd22 - vNpc;
	fDis = vDistance.Magnitude();

	if(fDis <= m_fSecForMetor)	{
		ClearPathFindData();
		m_bPathFlag = true;
		m_iAniFrameIndex = 1;
		m_pPoint[0].fXPos = m_fEndPoint_X;
		m_pPoint[0].fZPos = m_fEndPoint_Y;
		return true;
	}

	if((int)fDis > iTempRange)	{
		TRACE("Npc-GetTargetPath() searchrange over Fail return -1: [nid=%d,%s]\n", GetID(), GetName().c_str());
		return -1; 
	}


	if (GetType() != NPC_DUNGEON_MONSTER 
		&& hasTarget())	
		return 0;	

	CPoint start, end;
	start.x = (int)(GetX()/TILE_SIZE) - min_x; 
	start.y = (int)(GetZ()/TILE_SIZE) - min_z;
	end.x = (int)(vEnd22.x/TILE_SIZE) - min_x;
	end.y = (int)(vEnd22.z/TILE_SIZE) - min_z;

	if (GetType() == NPC_DUNGEON_MONSTER
		&& !isInSpawnRange((int)vEnd22.x, (int)vEnd22.z))
		return -1;	

	m_min_x = min_x;
	m_min_y = min_z;
	m_max_x = max_x;
	m_max_y = max_z;

	return PathFind(start, end, m_fSecForMetor);
}

time_t CNpc::Attack()
{
	if (isDead())
		return -1;

	int nRandom = 0, nPercent=1000;
	bool bTeleport = false;

	if (isNonAttackingObject())
	{
		m_NpcState = NPC_STANDING;
		InitTarget();
		return 0;
	}	

	if (GetProto()->m_byDirectAttack == 1)
		return LongAndMagicAttack();

	int nStandingTime = m_sStandTime;
	auto result = IsCloseTarget(m_byAttackRange, AttackTypeMagic);
	if (result == CloseTargetNotInRange)   
	{
		m_sStepCount = 0;
		m_byActionFlag = ATTACK_TO_TRACE;
		m_NpcState = NPC_TRACING;
		return 0;
	}	
	else if (result == CloseTargetInAttackRange)
	{
		if (GetProto()->m_byDirectAttack == 2)
			return LongAndMagicAttack();

		m_sStepCount = 0;
		m_byActionFlag = ATTACK_TO_TRACE;
		m_NpcState = NPC_TRACING;
		return 0;
	}
	else if (result == CloseTargetInvalid)
	{
		m_NpcState = NPC_STANDING;
		InitTarget();
		return 0;
	}

	int		nDamage		= 0;
	uint16 nID = m_Target.id;					// Target 을 구한다.

	// Targeting player
	if (nID < NPC_BAND)	
	{
		CUser * pUser = g_pMain->GetUserPtr(nID);
		if (pUser == nullptr
			|| pUser->isDead()
			|| pUser->m_bInvisibilityType)
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

		// Don't attack GMs.
		if (pUser->isGM())
		{
			InitTarget();
			m_NpcState = NPC_MOVING;
			return nStandingTime;
		}

		if (GetProto()->m_byMagicAttack == 4 || GetProto()->m_byMagicAttack == 5)
		{
			nRandom = myrand(1, 10000);
			if (nRandom < nPercent)	
			{
				CNpcMagicProcess::MagicPacket(MAGIC_EFFECTING, m_proto->m_iMagic1, GetID(), -1, int16(pUser->GetX()), int16(pUser->GetY()), int16(pUser->GetZ()));
				printf("AreaMagicAttack --- sid=%d, magicid=%d, name=%s\n", GetID(), m_proto->m_iMagic1, m_proto->m_strName.c_str());
				return m_sAttackDelay + 1000;
			}
		}
		else if (GetProto()->m_byMagicAttack == 2)	
		{
			nRandom = myrand(1, 10000);
			if (nRandom < nPercent)	
			{
				CNpcMagicProcess::MagicPacket(MAGIC_EFFECTING, m_proto->m_iMagic1, GetID(), pUser->GetID());
				printf("LongAndMagicAttack --- sid=%d, tid=%d, magicid=%d, name=%s\n", GetID(), pUser->GetID(), m_proto->m_iMagic1, m_proto->m_strName.c_str());
				return m_sAttackDelay;
			}
		}


		SendAttackRequest(pUser->GetID());
	}
	else // Targeting NPC
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(nID);
		if (pNpc == nullptr
			|| pNpc->isDead())
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

		if (isHealer() 
			&& !isHostileTo(pNpc))
		{
			m_NpcState = NPC_HEALING;
			return 0;
		}

		SendAttackRequest(pNpc->GetID());
	}

	return m_sAttackDelay;
}

void CNpc::SendAttackRequest(int16 tid)
{
	Packet result(AG_ATTACK_REQ);
	result << GetID() << tid;
	g_pMain->Send(&result);
}

time_t CNpc::LongAndMagicAttack()
{
	int nStandingTime = m_sStandTime;
	auto result = IsCloseTarget(m_byAttackRange, AttackTypeMagic);
	if (result == CloseTargetNotInRange)	
	{
		m_sStepCount = 0;
		m_byActionFlag = ATTACK_TO_TRACE;
		m_NpcState = NPC_TRACING;
		return 0;
	}	
	else if (result == CloseTargetInAttackRange && GetProto()->m_byDirectAttack == 1)
	{
		m_sStepCount = 0;
		m_byActionFlag = ATTACK_TO_TRACE;
		m_NpcState = NPC_TRACING;
		return 0;
	}
	else if (result == CloseTargetInvalid)
	{
		m_NpcState = NPC_STANDING;
		InitTarget();
		return 0;
	}

	CNpc*	pNpc		= nullptr;	
	CUser*	pUser		= nullptr;
	int		nDamage		= 0;
	uint16 nID = m_Target.id;

	if (nID < NPC_BAND)	
	{
		pUser = g_pMain->GetUserPtr(nID);
		if (pUser == nullptr
			|| pUser->isDead()
			|| pUser->m_bInvisibilityType
			// Don't cast skills on GMs.
			|| pUser->isGM())
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}

		return CNpcMagicProcess::MagicPacket(MAGIC_CASTING, m_proto->m_iMagic1, GetID(), pUser->GetID());
	}
	else // Target monster/NPC 
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(nID);
		if (pNpc == nullptr
			|| pNpc->isDead())
		{
			InitTarget();
			m_NpcState = NPC_STANDING;
			return nStandingTime;
		}
	}

	return m_sAttackDelay;
}

void CNpc::TracingAttack()
{
	uint16 nID = m_Target.id;
	if (nID < NPC_BAND)	// Target is a player
	{
		CUser * pUser = g_pMain->GetUserPtr(nID);
		if (pUser == nullptr
			|| pUser->isDead()
			|| pUser->m_bInvisibilityType
			|| pUser->isGM() 
			|| !GetMap()->canAttackOtherNation() 
			|| pUser->GetID() == m_oSocketID)
			return;
	}
	else // Target is an NPC/monster
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(nID);
		if (pNpc == nullptr
			|| pNpc->isDead())
			return;
	}
	SendAttackRequest(nID);
}

void CNpc::MoveAttack()
{
	if (!hasTarget())
		return;

	__Vector3 vUser, vNpc;
	__Vector3 vDistance, vEnd22;

	float fDis = 0.0f;
	float fX = 0.0f, fZ = 0.0f;

	vNpc.Set(GetX(), GetY(), GetZ());

	if (m_Target.id < NPC_BAND)	// Target is a player
	{
		__Vector3 vUser;
		CUser * pUser = g_pMain->GetUserPtr(m_Target.id);
		if (pUser == nullptr) 
		{
			InitTarget();
			return;
		}
		vUser.Set(pUser->GetX(), pUser->GetY(), pUser->GetZ()); 

		CalcAdaptivePosition(vNpc, vUser, 2, &vEnd22);

		if(m_byAttackPos > 0 && m_byAttackPos < 9)
		{
			fX = vUser.x + surround_fx[m_byAttackPos-1];	fZ = vUser.z + surround_fz[m_byAttackPos-1];
			vEnd22.Set(fX, 0, fZ);
		}
		else
		{
			fX = vEnd22.x;	fZ = vEnd22.z;
		}
	}
	else // Target is an NPC/monster
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(m_Target.id);		
		if (pNpc == nullptr) 
		{
			InitTarget();
			return;
		}
		vUser.Set(pNpc->GetX(), pNpc->GetY(), pNpc->GetZ()); 

		CalcAdaptivePosition(vNpc, vUser, 2, &vEnd22);
		fX = vEnd22.x;	fZ = vEnd22.z;
	}

	vDistance = vUser - vNpc;
	fDis = vDistance.Magnitude();	

	if ((int)fDis < 3) return;

	vDistance = vEnd22 - vNpc;
	fDis = vDistance.Magnitude();	
	m_curx = vEnd22.x;
	m_curz = vEnd22.z;

	if (GetX() < 0 || GetZ() < 0)
	{
		TRACE("Npc-MoveAttack : nid=(%d, %s), x=%.2f, z=%.2f\n", 
			GetID(), GetName().c_str(), GetX(), GetZ());
	}

	// Move to target... then stop (this is really awkward behaviour.)
	SendMoveResult(GetX(), GetY(), GetZ(), fDis);
	SendMoveResult(GetX(), GetY(), GetZ());

	RegisterRegion(GetX(), GetZ());

	m_fEndPoint_X = GetX();
	m_fEndPoint_Y = GetZ();
}

bool CNpc::IsChangePath()
{
	float fCurX=0.0f, fCurZ=0.0f;
	GetTargetPos(fCurX, fCurZ);

	__Vector3 vStart, vEnd;
	vStart.Set(m_fEndPoint_X, 0, m_fEndPoint_Y);
	vEnd.Set(fCurX, 0, fCurZ);

	float fDis = GetDistance(vStart, vEnd);
	float fCompDis = 3.0f;

	if(fDis < fCompDis)
		return false;

	return true;
}

bool CNpc::GetTargetPos(float& x, float& z)
{
	if (!hasTarget())
		return false;

	Unit * pUnit = g_pMain->GetUnitPtr(m_Target.id);
	if (pUnit == nullptr)
		return false;

	x = pUnit->GetX();
	z = pUnit->GetZ();

	return true;
}

//	Target 과 NPC 간에 길찾기를 다시한다.
bool CNpc::ResetPath()
{
	float cur_x, cur_z;
	GetTargetPos(cur_x, cur_z);

	//	TRACE("ResetPath : user pos ,, x=%.2f, z=%.2f\n", cur_x, cur_z);

	m_Target.x = cur_x;
	m_Target.z = cur_z;

	int nValue = GetTargetPath();
	if(nValue == -1)		// 타겟이 없어지거나,, 멀어졌음으로...
	{
		TRACE("Npc-ResetPath Fail - target_x = %.2f, z=%.2f, value=%d\n", m_Target.x, m_Target.z, nValue);
		return false;
	}
	else if(nValue == 0)	// 타겟 방향으로 바로 간다..
	{
		m_fSecForMetor = m_fSpeed_2;	// 공격일때는 뛰는 속도로... 
		IsNoPathFind(m_fSecForMetor);
	}

	//TRACE("Npc-ResetPath - target_x = %.2f, z=%.2f, value=%d\n", m_Target.x, m_Target.z, nValue);

	return true;	
}

void CNpc::ChangeTarget(int nAttackType, CUser *pUser)
{
	int preDamage, lastDamage;
	__Vector3 vUser, vNpc;
	float fDistance1 = 0.0f, fDistance2 = 0.0f;
	int iRandom = myrand(0, 100);

	if (pUser == nullptr
		|| pUser->isDead()
		|| !isHostileTo(pUser)
		|| pUser->m_bInvisibilityType
		|| pUser->isGM()
		|| m_NpcState == NPC_FAINTING
		|| isNonAttackingObject())
		return;

	CUser *preUser = nullptr;
	if (hasTarget() && m_Target.id < NPC_BAND)	
		preUser = g_pMain->GetUserPtr(m_Target.id);

	if (pUser == preUser)
	{
		if (m_bHasFriends || GetType() == NPC_BOSS) 
			FindFriend(GetType() == NPC_BOSS ? MonSearchAny : MonSearchSameFamily);

		return;
	}

	if (preUser != nullptr)
	{
		preDamage = 0; lastDamage = 0;

		if (iRandom >= 0 && iRandom < 50)	
		{
			preDamage = preUser->GetDamage(this, nullptr, true);
			lastDamage = pUser->GetDamage(this, nullptr, true);

			if (preDamage > lastDamage) 
				return;
		}
		else if(iRandom >= 50 && iRandom < 80)	
		{
			vNpc.Set(GetX(), GetY(), GetZ());
			vUser.Set(preUser->GetX(), 0, preUser->GetZ());
			fDistance1 = GetDistance(vNpc, vUser);
			vUser.Set(pUser->GetX(), 0, pUser->GetZ());
			fDistance2 = GetDistance(vNpc, vUser);

			if (fDistance2 > fDistance1)	
				return;
		}
		else if (iRandom >= 80 && iRandom < 95)		
		{
			preDamage = GetDamage(preUser, nullptr, true); /* preview the amount of damage that might be dealt for comparison */
			lastDamage = GetDamage(pUser, nullptr, true); 
			//TRACE("Npc-changeTarget 333 - iRandom=%d, pre=%d, last=%d\n", iRandom, preDamage, lastDamage);
			if(preDamage > lastDamage) return;
		}
	}
	else if(preUser == nullptr && nAttackType == 1004)		return;		// Heal magic에 반응하지 않도록..

	m_Target.id	= pUser->GetID();
	m_Target.bSet = true;
	m_Target.x	= pUser->GetX();
	m_Target.y	= pUser->GetY();
	m_Target.z  = pUser->GetZ();

	//TRACE("Npc-changeTarget - target_x = %.2f, z=%.2f\n", m_Target.x, m_Target.z);

	int nValue = 0;
	// 어슬렁 거리는데 공격하면 바로 반격
	if(m_NpcState == NPC_STANDING || m_NpcState == NPC_MOVING || m_NpcState == NPC_SLEEPING)
	{									// 가까이 있으면 반격으로 이어지구
		if(IsCloseTarget(pUser, m_byAttackRange) == true)
		{
			m_NpcState = NPC_FIGHTING;
			m_Delay = 0;
		}
		else							// 바로 도망가면 좌표를 갱신하고 추적	
		{
			nValue = GetTargetPath(1);
			if(nValue == 1)	// 반격 동작후 약간의 딜레이 시간이 있음	
			{
				m_NpcState = NPC_TRACING;
				m_Delay = 0;
			}
			else if(nValue == -1)
			{
				m_NpcState = NPC_STANDING;
				m_Delay = 0;
			}
			else if(nValue == 0)
			{
				m_fSecForMetor = m_fSpeed_2;	// 공격일때는 뛰는 속도로... 
				IsNoPathFind(m_fSecForMetor);
				m_NpcState = NPC_TRACING;
				m_Delay = 0;
			}
		}
	}
	//	else m_NpcState = NPC_ATTACKING;	// 한참 공격하는데 누가 방해하면 목표를 바꿈

	if (m_bHasFriends || GetType() == NPC_BOSS)	 
		FindFriend(GetType() == NPC_BOSS ? MonSearchAny : MonSearchSameFamily);
}

//	나를 공격한 Npc를 타겟으로 삼는다.(기준 : 렙과 HP를 기준으로 선정)
void CNpc::ChangeNTarget(CNpc *pNpc)
{
	int preDamage, lastDamage;
	__Vector3 vMonster, vNpc;
	float fDist = 0.0f;

	if (pNpc == nullptr
		|| pNpc->m_NpcState == NPC_DEAD
		|| !hasTarget()
		|| m_Target.id < NPC_BAND)
		return;

	CNpc *preNpc = g_pMain->GetNpcPtr(m_Target.id);
	if (preNpc == nullptr
		|| pNpc == preNpc) return;

	preDamage = GetDamage(preNpc, nullptr, true); /* preview the damage that might be dealt for comparison */
	lastDamage = GetDamage(pNpc, nullptr, true); 

	vNpc.Set(GetX(), GetY(), GetZ());
	vMonster.Set(preNpc->GetX(), 0, preNpc->GetZ());
	fDist = GetDistance(vNpc, vMonster);
	preDamage = (int)((double)preDamage/fDist + 0.5);
	vMonster.Set(pNpc->GetX(), 0, pNpc->GetZ());
	fDist = GetDistance(vNpc, vMonster);
	lastDamage = (int)((double)lastDamage/fDist + 0.5);

	if(preDamage > lastDamage) return;

	m_Target.id	= pNpc->GetID();
	m_Target.bSet = true;
	m_Target.x	= pNpc->GetX();
	m_Target.y	= pNpc->GetZ();
	m_Target.z  = pNpc->GetZ();

	int nValue = 0;
	if (m_NpcState == NPC_STANDING || m_NpcState == NPC_MOVING || m_NpcState == NPC_SLEEPING)
	{
		if (IsCloseTarget(m_byAttackRange, AttackTypeNone) == 1)
		{
			m_NpcState = NPC_FIGHTING;
			m_Delay = 0;
		}
		else
		{
			nValue = GetTargetPath();
			if (nValue == 1)
			{
				m_NpcState = NPC_TRACING;
				m_Delay = 0;
			}
			else if(nValue == -1)
			{
				m_NpcState = NPC_STANDING;
				m_Delay = 0;
			}
			else if(nValue == 0)
			{
				m_fSecForMetor = m_fSpeed_2;
				IsNoPathFind(m_fSecForMetor);
				m_NpcState = NPC_TRACING;
				m_Delay = 0;
			}
		}
	}
	//	else m_NpcState = NPC_ATTACKING;	// 한참 공격하는데 누가 방해하면 목표를 바꿈

	if (m_bHasFriends)
		FindFriend();
}

void CNpc::RecvAttackReq(int nDamage, uint16 sAttackerID, AttributeType attributeType /*= AttributeNone*/)
{
	if (isDead()
		|| nDamage < 0)
		return;

	Unit * pAttacker = g_pMain->GetUnitPtr(sAttackerID);

	if (pAttacker != nullptr
		&& pAttacker->isPlayer())
	{
		Guard lock(m_damageListLock);
		m_DamagedUserList[pAttacker->GetID()] += nDamage;
	}

	m_TotalDamage += nDamage;
	HpChange(-nDamage, pAttacker, false);

	if (pAttacker == nullptr)
		return;

	if (!pAttacker->isPlayer())
	{
		ChangeNTarget(TO_NPC(pAttacker));
		return;
	}

	if (m_NpcState != NPC_FAINTING)
	{
		int iRandom = myrand(1, 100);
		int sDamage=0;

		switch (attributeType)
		{
			case 1:
			sDamage = (int)(10 + (40 - 40 * ((double)m_sFireR / 80)));
			break;
			case 2:
			sDamage = (int)(10 + (40 - 40 * ((double)m_sColdR / 80)));
			break;
			case 3:
			sDamage = (int)(10 + (40 - 40 * ((double)m_sLightningR / 80)));
			break;
			case 4:
			sDamage = (int)(10 + (40 - 40 * ((double)m_sMagicR / 80)));
			break;
			case 5:
			sDamage = (int)(10 + (40 - 40 * ((double)m_sDiseaseR / 80)));
			break;
			case 6:
			sDamage = (int)(10 + (40 - 40 * ((double)m_sPoisonR / 80)));
			break;
			sDamage = nDamage;
			default:
				break;
		}

		if (COMPARE(iRandom, 0, sDamage))
		{
			m_NpcState = NPC_FAINTING;
			m_Delay = 0;
			m_tFaintingTime = UNIXTIME;
		}
		else	
		{
			ChangeTarget(0, TO_USER(pAttacker));
		}
	}
}

void CNpc::HpChange(int amount, Unit *pAttacker /*= nullptr*/, bool bSendToGameServer /*= true*/)
{
	uint16 tid = (pAttacker != nullptr ? pAttacker->GetID() : -1);
	int16 oldHP = m_iHP;

	// Implement damage/HP cap.
	if (amount < -MAX_DAMAGE)
		amount = -MAX_DAMAGE;
	else if (amount > MAX_DAMAGE)
		amount = MAX_DAMAGE;

	if (amount < 0 && -amount >= m_iHP)
		m_iHP = 0;
	else if (amount >= 0 && m_iHP + amount > m_iMaxHP)
		m_iHP = m_iMaxHP;
	else
		m_iHP += amount;

	if (bSendToGameServer)
	{
		Packet result(AG_NPC_HP_CHANGE);
		result << GetID() << tid << m_iHP << amount;
		g_pMain->Send(&result);
	}

	if (m_iHP == 0)
		Dead(pAttacker);
}

void CNpc::SendExpToUserList()
{
	if (GetMap() == nullptr) 
		return;

	std::string strMaxDamageUser;
	uint32 nMaxDamage = 0;

	Guard lock(m_damageListLock);
	std::map<CUser *, uint32> filteredDamageList;
	std::map<uint16, CUser *> partyIndex;

	// Filter the damage list first, so we only send one packet per party.
	// Rewards are shared based upon the total amount of damage dealt.

	// NOTE:	If a player logs out & another takes its place, it is currently possible
	//			For this new session to be counted as the old when the mob is killed.
	//			This does, however, requires the player to be in the same zone, in range at the time
	//			of the mob's death -- so it is rather unlikely. We should fix this later.
	foreach (itr, m_DamagedUserList)
	{
		CUser * pUser = g_pMain->GetUserPtr(itr->first);
		if (pUser == nullptr
			|| !isInRangeSlow(pUser, NPC_EXP_RANGE))
			continue;

		// Not in a party, we can add them to the list as a solo attacker.
		if (!pUser->isInParty())
		{
			filteredDamageList[pUser] = itr->second;
			continue;
		}

		// In a party, so check if another party member is already in the list first.
		auto partyItr = partyIndex.find(pUser->GetPartyID());

		// No other party member, so add us to the filtered damage list & the party index
		// for future reference.
		if (partyItr == partyIndex.end())
		{
			filteredDamageList[pUser] = itr->second;
			partyIndex.insert(std::make_pair(pUser->GetPartyID(), pUser));
		}
		// There is another pf pir party members in the damage list already, so just add 
		// to their damage total.
		else
		{
			filteredDamageList[partyItr->second] += itr->second;
		}
	}

	// Now we can go through the filtered list and tell the game server to distribute rewards
	// for the kill.
	foreach (itr, filteredDamageList)
	{
		CUser * pUser = itr->first;

		Packet result(AG_USER_EXP);
		result	<< pUser->GetID()
			<< GetID()
			// target's damage vs total damage dealt to the NPC
			<< itr->second << m_TotalDamage 
			<< m_proto->m_iExp << m_proto->m_iLoyalty;
		g_pMain->Send(&result);

		if (itr->second > nMaxDamage)
		{
			m_sMaxDamageUserid = pUser->GetID();
			strMaxDamageUser = pUser->GetName();
			nMaxDamage = itr->second;
		}
	}

	if (g_pMain->m_byBattleEvent == BATTLEZONE_OPEN
		&& !strMaxDamageUser.empty()
		&& m_bySpecialType >= NpcSpecialTypeKarusWarder1 && m_bySpecialType <= NpcSpecialTypeElmoradKeeper)
	{
		Packet result(AG_BATTLE_EVENT, uint8(BATTLE_EVENT_MAX_USER));

		switch (m_bySpecialType)
		{
		case NpcSpecialTypeKarusWarder1:	result << uint8(3); g_pMain->m_sKillKarusNpc++; break;
		case NpcSpecialTypeKarusWarder2:	result << uint8(4); g_pMain->m_sKillKarusNpc++; break;
		case NpcSpecialTypeElmoradWarder1:	result << uint8(5);	g_pMain->m_sKillElmoNpc++; break;
		case NpcSpecialTypeElmoradWarder2:	result << uint8(6); g_pMain->m_sKillElmoNpc++; break;
		case NpcSpecialTypeKarusKeeper:		result << uint8(7); g_pMain->m_sKillKarusNpc++; break;
		case NpcSpecialTypeElmoradKeeper:	result << uint8(8); g_pMain->m_sKillElmoNpc++; break;
		}

		result.SByte();
		result << strMaxDamageUser;
		g_pMain->Send(&result);

		bool	bKarusComplete = (g_pMain->m_sKillKarusNpc == GetMap()->m_sKarusRoom),
			bElMoradComplete = (g_pMain->m_sKillElmoNpc == GetMap()->m_sElmoradRoom);

		if (bKarusComplete || bElMoradComplete)
		{
			result.clear();
			result	<< uint8(BATTLE_EVENT_RESULT) 
				<< uint8(bKarusComplete ? KARUS : ELMORAD)
				<< strMaxDamageUser;
			g_pMain->Send(&result);
		}
	}
}

bool CNpc::IsCloseTarget(CUser *pUser, int nRange)
{
	if (pUser == nullptr
		|| pUser->isDead()
		|| !isInRangeSlow(pUser, nRange * 2.0f))
		return false; 

	m_Target.id = pUser->GetID();
	m_Target.bSet = true;
	m_Target.x = pUser->GetX();
	m_Target.y = pUser->GetY();
	m_Target.z = pUser->GetZ();

	return true;
}

int CNpc::FindFriend(MonSearchType type)
{
	CNpc* pNpc = nullptr;
	MAP* pMap = GetMap();
	if (pMap == nullptr
		|| m_bySearchRange == 0
		|| (type != MonSearchNeedsHealing && hasTarget()))
		return 0;

	int min_x = (int)(GetX() - m_bySearchRange)/VIEW_DIST;	if(min_x < 0) min_x = 0;
	int min_z = (int)(GetZ() - m_bySearchRange)/VIEW_DIST;	if(min_z < 0) min_z = 0;
	int max_x = (int)(GetX() + m_bySearchRange)/VIEW_DIST;	if(max_x > pMap->GetXRegionMax()) max_x = pMap->GetXRegionMax();
	int max_z = (int)(GetZ() + m_bySearchRange)/VIEW_DIST;	if(min_z > pMap->GetZRegionMax()) min_z = pMap->GetZRegionMax();

	int search_x = max_x - min_x + 1;		
	int search_z = max_z - min_z + 1;	

	int i, j, count = 0;
	_TargetHealer arHealer[9];
	for(i=0; i<9; i++)	{
		arHealer[i].sNID = -1;
		arHealer[i].sValue = 0;
	}

	for (i = 0; i < search_x; i++)	
		for (j = 0; j < search_z; j++)
			FindFriendRegion(min_x+i, min_z+j, pMap, &arHealer[count], type);

	int iValue = 0, iMonsterNid = 0;
	for (i=0; i<9; i++)	
	{
		if (iValue < arHealer[i].sValue)	
		{
			iValue = arHealer[i].sValue;
			iMonsterNid = arHealer[i].sNID;
		}
	}

	if (iMonsterNid != 0)	
	{
		m_Target.id = iMonsterNid;
		m_Target.bSet = true;
		return iMonsterNid;
	}

	return 0;
}

void CNpc::FindFriendRegion(int x, int z, MAP* pMap, _TargetHealer* pHealer, MonSearchType type)
{
	if (x < 0 || z < 0 || x > pMap->GetXRegionMax() || z > pMap->GetZRegionMax())
	{
		TRACE("#### Npc-FindFriendRegion() Fail : [nid=%d, sid=%d], nRX=%d, nRZ=%d #####\n", GetID(), GetProtoID(), x, z);
		return;
	}

	Guard lock(pMap->m_lock);
	CRegion *pRegion = &pMap->m_ppRegion[x][z];

	if (pRegion == nullptr)
		return;

	__Vector3 vStart, vEnd;
	float fDis = 0.0f, 
		fSearchRange = (type == MonSearchNeedsHealing ? (float)m_byAttackRange : (float)m_byTracingRange);
	int iValue = 0;

	vStart.Set(GetX(), GetY(), GetZ());

	foreach_stlmap (itr, pRegion->m_RegionNpcArray)
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(itr->first);

		if (pNpc != nullptr && pNpc->m_NpcState != NPC_DEAD && pNpc->GetID() != GetID())
		{
			vEnd.Set(pNpc->GetX(), pNpc->GetY(), pNpc->GetZ()); 
			fDis = GetDistance(vStart, vEnd);

			if (fDis <= fSearchRange)	
			{
				if (type == MonSearchAny)
				{
					if (GetID() != pNpc->GetID())	
					{
						if (pNpc->hasTarget() && pNpc->m_NpcState == NPC_FIGHTING) 
							continue;

						pNpc->m_Target.id = m_Target.id;
						pNpc->m_Target.bSet = true;
						pNpc->m_Target.x = m_Target.x;
						pNpc->m_Target.y = m_Target.y;
						pNpc->m_Target.z = m_Target.z;
						pNpc->NpcStrategy(NPC_ATTACK_SHOUT);
					}
				}
				else if (type == MonSearchSameFamily)
				{
					if (pNpc->m_bHasFriends && GetID() != pNpc->GetID() && pNpc->m_proto->m_byFamilyType == m_proto->m_byFamilyType)	{
						if (pNpc->hasTarget() && pNpc->m_NpcState == NPC_FIGHTING) 
							continue;

						pNpc->m_Target.id = m_Target.id;
						pNpc->m_Target.bSet = true;
						pNpc->m_Target.x = m_Target.x;
						pNpc->m_Target.y = m_Target.y;
						pNpc->m_Target.z = m_Target.z;
						pNpc->NpcStrategy(NPC_ATTACK_SHOUT);
					}
				}
				else if (type == MonSearchNeedsHealing)
				{
					if (pHealer == nullptr) 
						return;

					int iHP = (int)(pNpc->m_iMaxHP * 0.9);
					if(pNpc->m_iHP <= iHP)	{		// HP 체크
						int iCompValue = (int)((pNpc->m_iMaxHP - pNpc->m_iHP) / (pNpc->m_iMaxHP * 0.01));
						if (iValue < iCompValue)		{
							iValue = iCompValue;
							pHealer->sNID = pNpc->GetID();
							pHealer->sValue = iValue;
						}
					}
				}
			}	
			else continue;
		}
	}
}

void CNpc::NpcStrategy(uint8 type)
{
	switch(type)
	{
	case NPC_ATTACK_SHOUT:
		m_NpcState = NPC_TRACING;
		m_Delay = m_sSpeed;//STEP_DELAY;
		m_fDelayTime = getMSTime();
		break;
	}
}

void CNpc::FillNpcInfo(Packet & result)
{
	result	<< uint8(1) 
		<< GetID() << GetProtoID() << m_proto->m_sPid
		<< m_sSize << m_iWeapon_1 << m_iWeapon_2
		<< GetZoneID() << GetName()
		<< (m_proto->m_byGroupSpecial > 0 ? m_proto->m_byGroupSpecial : GetNation()) << GetLevel()
		<< GetX() << GetZ() << GetY() << m_byDirection
		<< GetType()
		<< m_iSellingGroup << m_iMaxHP << m_iHP
		<< m_byGateOpen 
		<< m_fTotalHitrate << m_fTotalEvasionrate 
		<< m_sTotalAc << m_sTotalHit
		<< m_byObjectType << m_byTrapNumber 
		<< m_bMonster << m_oSocketID << m_bEventRoom
		// Include resistance data, note that we don't need to send modified amounts as 
		// there's no skill handling here - it happens in GameServer.
		// We will probably need to update the AI server (from GameServer) with this data.
		<< m_sFireR << m_sColdR << m_sLightningR 
		<< m_sMagicR << m_sDiseaseR << m_sPoisonR;
}

int CNpc::GetDir(float x1, float z1, float x2, float z2)
{
	int nDir;					//	3 4 5
	//	2 8 6
	//	1 0 7

	int nDirCount = 0;

	int x11 = (int)x1 / TILE_SIZE;
	int y11 = (int)z1 / TILE_SIZE; 
	int x22 = (int)x2 / TILE_SIZE;
	int y22 = (int)z2 / TILE_SIZE; 

	int deltax = x22 - x11;
	int deltay = y22 - y11;

	int fx = ((int)x1/TILE_SIZE) * TILE_SIZE;
	int fy = ((int)z1/TILE_SIZE) * TILE_SIZE;

	float add_x = x1 - fx;
	float add_y = z1 - fy;

	if (deltay==0) {
		if (x22>x11) nDir = DIR_RIGHT;		
		else nDir = DIR_LEFT;	
		goto result_value;
	}
	if (deltax==0) 
	{
		if (y22>y11) nDir = DIR_DOWN;	
		else nDir = DIR_UP;		
		goto result_value;
	}
	else 
	{
		if (y22>y11) 
		{
			if (x22>x11) 
				nDir = DIR_DOWNRIGHT;
			else 
				nDir = DIR_DOWNLEFT;
		}
		else
		{
			if (x22 > x11) 
				nDir = DIR_UPRIGHT;		
			else 
				nDir = DIR_UPLEFT;
		}
	}

result_value:

	switch(nDir)
	{
	case DIR_DOWN:
		m_fAdd_x = add_x;
		m_fAdd_z = 3;
		break;
	case DIR_DOWNLEFT:
		m_fAdd_x = 1;
		m_fAdd_z = 3;
		break;
	case DIR_LEFT:
		m_fAdd_x = 1;
		m_fAdd_z = add_y;
		break;
	case DIR_UPLEFT:
		m_fAdd_x = 1;
		m_fAdd_z = 1;
		break;
	case DIR_UP:
		m_fAdd_x = add_x;
		m_fAdd_z = 1;
		break;
	case DIR_UPRIGHT:
		m_fAdd_x = 3;
		m_fAdd_z = 1;
		break;
	case DIR_RIGHT:
		m_fAdd_x = 3;
		m_fAdd_z = add_y;
		break;
	case DIR_DOWNRIGHT:
		m_fAdd_x = 3;
		m_fAdd_z = 3;
		break;
	}

	return nDir;
}

void CNpc::NpcMoveEnd()
{
	RegisterRegion(GetX(), GetZ());
	SendMoveResult(GetX(), GetY(), GetZ());
}

void CNpc::GetVectorPosition(__Vector3 & vOrig, __Vector3 & vDest, float fDis, __Vector3 * vResult)
{
	*vResult = vDest - vOrig;
	vResult->Magnitude();
	vResult->Normalize();
	*vResult *= fDis;
	*vResult += vOrig;
}

float CNpc::GetDistance(__Vector3 & vOrig, __Vector3 & vDest)
{
	return (vOrig - vDest).Magnitude();
}

bool CNpc::GetUserInView()
{
	MAP* pMap = GetMap();
	if (pMap == nullptr)	return false;
	//if( m_ZoneIndex > 5 || m_ZoneIndex < 0) return false;		// 임시코드 ( 2002.03.24 )
	int min_x = (int)(GetX() - NPC_VIEW_RANGE)/VIEW_DIST;	if(min_x < 0) min_x = 0;
	int min_z = (int)(GetZ() - NPC_VIEW_RANGE)/VIEW_DIST;	if(min_z < 0) min_z = 0;
	int max_x = (int)(GetX() + NPC_VIEW_RANGE)/VIEW_DIST;	if(max_x > pMap->GetXRegionMax()) max_x = pMap->GetXRegionMax();
	int max_z = (int)(GetZ() + NPC_VIEW_RANGE)/VIEW_DIST;	if(max_z > pMap->GetZRegionMax()) max_z = pMap->GetZRegionMax();

	int search_x = max_x - min_x + 1;		
	int search_z = max_z - min_z + 1;	

	bool bFlag = false;
	int i, j;

	for(i = 0; i < search_x; i++)	{
		for(j = 0; j < search_z; j++)	{
			bFlag = GetUserInViewRange(min_x+i, min_z+j);
			if(bFlag == true)	return true;
		}
	}

	return false;
}

bool CNpc::GetUserInViewRange(int x, int z)
{
	MAP* pMap = GetMap();
	if (pMap == nullptr || x < 0 || z < 0 || x > pMap->GetXRegionMax() || z > pMap->GetZRegionMax())
	{
		TRACE("#### Npc-GetUserInViewRange() Fail : [nid=%d, sid=%d], x1=%d, z1=%d #####\n", GetID(), GetProtoID(), x, z);
		return false;
	}

	Guard lock(pMap->m_lock);
	CRegion * pRegion = pMap->GetRegion(x, z);

	if (pRegion == nullptr)
		return false;

	float fDis = 0.0f; 

	foreach_stlmap (itr, pRegion->m_RegionUserArray)
	{
		CUser *pUser = g_pMain->GetUserPtr(*itr->second);
		if (pUser == nullptr)
			continue;

		if (isInRangeSlow(pUser, NPC_VIEW_RANGE))
			return true;
	}

	return false;
}

void CNpc::CalcAdaptivePosition(__Vector3 & vPosOrig, __Vector3 & vPosDest, float fAttackDistance, __Vector3 * vResult)
{
	*vResult = vPosOrig - vPosDest;	
	vResult->Normalize();	
	*vResult *= fAttackDistance;
	*vResult += vPosDest;
}

bool CNpc::IsPathFindCheck(float fDistance)
{
	int nX = 0, nZ = 0;
	__Vector3 vStart, vEnd, vDis, vOldDis;
	float fDis = 0.0f;
	vStart.Set(m_fStartPoint_X, 0, m_fStartPoint_Y);
	vEnd.Set(m_fEndPoint_X, 0, m_fEndPoint_Y);
	vDis.Set(m_fStartPoint_X, 0, m_fStartPoint_Y);
	int count = 0;
	int nError = 0;

	MAP* pMap = GetMap();

	nX = (int)(vStart.x / TILE_SIZE);
	nZ = (int)(vStart.z / TILE_SIZE);
	if (pMap->IsMovable(nX, nZ))
		return false;

	nX = (int)(vEnd.x / TILE_SIZE);
	nZ = (int)(vEnd.z / TILE_SIZE);
	if (pMap->IsMovable(nX, nZ))
		return false;

	do
	{
		vOldDis.Set(vDis.x, 0, vDis.z);
		GetVectorPosition(vDis, vEnd, fDistance, &vDis);
		fDis = GetDistance(vOldDis, vEnd);

		if (fDis > NPC_MAX_MOVE_RANGE)
		{
			nError = -1;
			break;
		}

		nX = (int)(vDis.x / TILE_SIZE);
		nZ = (int)(vDis.z / TILE_SIZE);

		if (pMap->IsMovable(nX, nZ)
			|| count >= MAX_PATH_LINE)
		{
			nError = -1;
			break;
		}

		m_pPoint[count].fXPos = vEnd.x;
		m_pPoint[count++].fZPos = vEnd.z;

	} while (fDis <= fDistance);

	m_iAniFrameIndex = count;

	if (nError == -1)
		return false;

	return true;
}

// 패스 파인드를 하지 않고 공격대상으로 가는 루틴..
void CNpc::IsNoPathFind(float fDistance)
{
	ClearPathFindData();
	m_bPathFlag = true;

	int nX = 0, nZ = 0;
	__Vector3 vStart, vEnd, vDis, vOldDis;
	float fDis = 0.0f;
	vStart.Set(m_fStartPoint_X, 0, m_fStartPoint_Y);
	vEnd.Set(m_fEndPoint_X, 0, m_fEndPoint_Y);
	vDis.Set(m_fStartPoint_X, 0, m_fStartPoint_Y);
	int count = 0;
	int nError = 0;

	fDis = GetDistance(vStart, vEnd);	
	if(fDis > NPC_MAX_MOVE_RANGE)	{						// 100미터 보다 넓으면 스탠딩상태로..
		ClearPathFindData();
		TRACE("#### Npc-IsNoPathFind Fail : NPC_MAX_MOVE_RANGE overflow  .. [nid = %d, name=%s], cur_x=%.2f, z=%.2f, dest_x=%.2f, dest_z=%.2f, fDis=%.2f#####\n", 
			GetID(), GetName().c_str(), m_fStartPoint_X, m_fStartPoint_Y, m_fEndPoint_X, m_fEndPoint_Y, fDis);
		return;
	}

	if (GetMap() == nullptr)
	{
		ClearPathFindData();
		TRACE("#### Npc-IsNoPathFind No map : [nid=%d, name=%s], zone=%d #####\n", GetID(), GetName().c_str(), GetZoneID());
		return;
	}
	MAP* pMap = GetMap();

	do
	{
		vOldDis.Set(vDis.x, 0, vDis.z);
		GetVectorPosition(vDis, vEnd, fDistance, &vDis);
		fDis = GetDistance(vOldDis, vEnd);

		nX = (int)(vDis.x / TILE_SIZE);
		nZ = (int)(vDis.z / TILE_SIZE);
		if(count < 0 || count >= MAX_PATH_LINE)	{	
			ClearPathFindData();
			TRACE("#### Npc-IsNoPathFind index overflow Fail 1 :  count=%d ####\n", count);
			return;
		}	

		m_pPoint[count].fXPos = vEnd.x;
		m_pPoint[count++].fZPos = vEnd.z;
	} while (fDis <= fDistance);

	if(count <= 0 || count >= MAX_PATH_LINE)	{	
		ClearPathFindData();
		TRACE("#### IsNoPtahfind Fail : nid=%d,%s, count=%d ####\n", GetID(), GetName().c_str(), count);
		return;
	}
	m_iAniFrameIndex = count;

}

void CNpc::GiveNpcHaveItem()
{
	if (m_sMaxDamageUserid < 0 || m_sMaxDamageUserid > MAX_USER)
		return;

	int iPer = 0, iMakeItemCode = 0, iMoney = 0, iRandom, nCount = 0;

	iRandom = myrand(70, 100);
	iMoney = m_iMoney * iRandom / 100;

	_NpcGiveItem m_GiveItemList[NPC_HAVE_ITEM_LIST];
	if (iMoney > 0)
	{
		if (iMoney >= SHRT_MAX)
			iMoney = 32000;	

		m_GiveItemList[nCount].sSid = TYPE_MONEY_SID;
		m_GiveItemList[nCount++].count = iMoney;
	}

	_K_MONSTER_ITEM * pItem = g_pMain->m_NpcItemArray.GetData(m_iItem);
	if (pItem != nullptr)
	{
		// j = iItem
		for (int j = 0; j < 5; j++)
		{
			if (pItem->iItem[j] == 0
				|| pItem->sPercent[j] == 0)
				continue;

			iRandom = myrand(1, 10000);
			iPer = pItem->sPercent[j];
			if (iRandom > iPer)
				continue;

			if (j < 2)
			{
				if (pItem->iItem[j] < 100)
				{
					iMakeItemCode = ItemProdution(pItem->iItem[j]);
				}
				else 
				{
					_MAKE_ITEM_GROUP * pGroup = g_pMain->m_MakeItemGroupArray.GetData(pItem->iItem[j]);
					if (pGroup == nullptr
						|| pGroup->iItems.size() != 30)
						continue;

					iMakeItemCode = pGroup->iItems[myrand(1, 30) - 1];
				}

				if (iMakeItemCode == 0) 
					continue;

				m_GiveItemList[nCount].sSid = iMakeItemCode;
				m_GiveItemList[nCount].count = 1;
			}
			else	
			{
				m_GiveItemList[nCount].sSid = pItem->iItem[j];
				if (COMPARE(m_GiveItemList[nCount].sSid, ARROW_MIN, ARROW_MAX))
					m_GiveItemList[nCount].count = 20;
				else	
					m_GiveItemList[nCount].count = 1;
			}
			nCount++;
		}
	}

	Packet result(AG_NPC_GIVE_ITEM);
	result	<< m_sMaxDamageUserid << GetID()
		<< GetZoneID() << GetRegionX() << GetRegionZ()
		<< GetX() << GetZ() << GetY()
		<< uint8(nCount);

	for (int i = 0; i < nCount; i++)
		result << m_GiveItemList[i].sSid << m_GiveItemList[i].count;

	g_pMain->Send(&result);
}


void CNpc::Yaw2D(float fDirX, float fDirZ, float& fYawResult)
{
	if ( fDirX >= 0.0f ) 
	{ 
		if ( fDirZ >= 0.0f ) 
			fYawResult = (float)(asin(fDirX)); 
		else 
			fYawResult = D3DXToRadian(90.0f) + (float)(acos(fDirX)); 
	}
	else 
	{ 
		if ( fDirZ >= 0.0f ) 
			fYawResult = D3DXToRadian(270.0f) + (float)(acos(-fDirX)); 
		else 
			fYawResult = D3DXToRadian(180.0f) + (float)(asin(-fDirX)); 
	}
}

void CNpc::ComputeDestPos(__Vector3 & vCur, float fDegree, float fDistance, __Vector3 * vResult)
{
	__Matrix44 mtxRot; 
	vResult->Set(0.0f, 0.0f, 1.0f);
	mtxRot.RotationY(D3DXToRadian(fDegree));
	*vResult *= mtxRot;
	*vResult *= fDistance;
	*vResult += vCur;
}

void CNpc::HpChange()
{
	m_fHPChangeTime = getMSTime();

	if (isDead() 
		|| GetHealth() == GetMaxHealth())
		return;

	HpChange((int)(m_iMaxHP / 20));
}

bool CNpc::CheckFindEnemy()
{
	if (isGuard())
		return true;

	MAP* pMap = GetMap();

	if (pMap == nullptr)
		return false;

	if (pMap->GetRegion(GetRegionX(),GetRegionZ()) == nullptr)
		return false;
	else 
		return pMap->GetRegion(GetRegionX(),GetRegionZ())->m_byMoving == 1;

	return false;
}

int	CNpc::ItemProdution(int item_number)							// 아이템 제작
{
	int iItemNumber = 0, iRandom = 0, i=0, iItemGrade = 0, iItemLevel = 0;
	int iDefault = 0, iItemCode=0, iItemKey=0, iRand2=0, iRand3=0, iRand4=0, iRand5=0;
	int iTemp1 = 0, iTemp2 = 0, iTemp3 = 0;

	iRandom = myrand(1, 10000);

	iItemGrade = GetItemGrade(item_number);
	if(iItemGrade == 0)		return 0;
	iItemLevel = GetLevel() / 5;

	if( COMPARE( iRandom, 1, 4001) )	{			// 무기구 아이템
		iDefault = 100000000;
		iRandom = myrand( 1, 10000 );				// 무기의 종류를 결정(단검, 검, 도끼,,,,)
		if( COMPARE ( iRandom, 1, 701 ) )			iRand2 = 10000000;
		else if( COMPARE ( iRandom, 701, 1401 ) )	iRand2 = 20000000;
		else if( COMPARE ( iRandom, 1401, 2101 ) )	iRand2 = 30000000;
		else if( COMPARE ( iRandom, 2101, 2801 ) )	iRand2 = 40000000;
		else if( COMPARE ( iRandom, 2801, 3501 ) )	iRand2 = 50000000;
		else if( COMPARE ( iRandom, 3501, 5501 ) )	iRand2 = 60000000;
		else if( COMPARE ( iRandom, 5501, 6501 ) )	iRand2 = 70000000;
		else if( COMPARE ( iRandom, 6501, 8501 ) )	iRand2 = 80000000;
		else if( COMPARE ( iRandom, 8501, 10001 ) )	iRand2 = 90000000;

		iTemp1 = GetWeaponItemCodeNumber(true);
		//TRACE("ItemProdution : GetWeaponItemCodeNumber() = %d, iRand2=%d\n", iTemp1, iRand2);
		if( iTemp1 == 0 )	return 0;
		iItemCode = iTemp1 * 100000;	// 루팅분포표 참조

		iRand3 = myrand(1, 10000);					// 종족(엘모, 카루스)
		if( COMPARE( iRand3, 1, 5000) )	iRand3 = 10000;
		else	iRand3 = 50000;
		iRand4 = myrand(1, 10000);					// 한손, 양손무기인지를 결정
		if( COMPARE( iRand4, 1, 5000) )	iRand4 = 0;
		else	iRand4 = 5000000;

		iRandom = GetItemCodeNumber(iItemLevel, 1);	// 레이매직표 적용
		//TRACE("ItemProdution : GetItemCodeNumber() = %d, iRand2=%d, iRand3=%d, iRand4=%d\n", iRandom, iRand2, iRand3, iRand4);
		if(iRandom == -1)	{						// 잘못된 아이템 생성실패
			return 0;
		}
		iRand5 = iRandom * 10;
		iItemNumber = iDefault + iItemCode + iRand2 + iRand3 + iRand4 + iRand5 + iItemGrade;

		//TRACE("ItemProdution : Weapon Success item_number = %d, default=%d, itemcode=%d, iRand2=%d, iRand3=%d, iRand4=%d, iRand5, iItemGrade=%d\n", iItemNumber, iDefault, iItemCode, iRand2, iRand3, iRand4, iRand5, iItemGrade);
	}
	else if( COMPARE( iRandom, 4001, 8001) )	{		// 방어구 아이템
		iDefault = 200000000;			

		iTemp1 = GetWeaponItemCodeNumber(false);
		//TRACE("ItemProdution : GetWeaponItemCodeNumber() = %d\n", iTemp1 );
		if( iTemp1 == 0 )	return 0;
		iItemCode = iTemp1 * 1000000;		// 루팅분포표 참조

		if( m_byMaxDamagedNation == KARUS )	{		// 종족
			iRandom = myrand(0, 10000);					// 직업의 갑옷을 결정		
			if( COMPARE( iRandom, 0, 2000) )	{		
				iRand2 = 0;	
				iRand3 = 10000;							// 전사갑옷은 아크투아렉만 가지도록
			}
			else if( COMPARE( iRandom, 2000, 4000) )	{
				iRand2 = 40000000;
				iRand3 = 20000;							// 로그갑옷은 투아렉만 가지도록
			}
			else if( COMPARE( iRandom, 4000, 6000) )	{
				iRand2 = 60000000;
				iRand3 = 30000;							// 마법사갑옷은 링클 투아렉만 가지도록
			}
			else if( COMPARE( iRandom, 6000, 10001) )	{
				iRand2 = 80000000;
				iRandom = myrand(0, 10000);
				if( COMPARE( iRandom, 0, 5000) )	iRand3 = 20000;	// 사제갑옷은 투아렉
				else								iRand3 = 40000;	// 사제갑옷은 퓨리투아렉
			}
		}
		else if( m_byMaxDamagedNation == ELMORAD )	{
			iRandom = myrand(0, 10000);					// 직업의 갑옷을 결정		
			if( COMPARE( iRandom, 0, 3300) )	{		
				iRand2 = 0;	
				iItemKey = myrand(0, 10000);			// 전사갑옷은 모든 종족이 가짐
				if( COMPARE( iItemKey, 0, 3333) )			iRand3 = 110000;
				else if( COMPARE( iItemKey, 3333, 6666) )	iRand3 = 120000;
				else if( COMPARE( iItemKey, 6666, 10001) )	iRand3 = 130000;
			}
			else if( COMPARE( iRandom, 3300, 5600) )	{
				iRand2 = 40000000;
				iItemKey = myrand(0, 10000);			// 로그갑옷은 남자와 여자만 가짐
				if( COMPARE( iItemKey, 0, 5000) )	iRand3 = 120000;
				else								iRand3 = 130000;
			}
			else if( COMPARE( iRandom, 5600, 7800) )	{
				iRand2 = 60000000;
				iItemKey = myrand(0, 10000);			// 마법사갑옷은 남자와 여자만 가짐
				if( COMPARE( iItemKey, 0, 5000) )	iRand3 = 120000;
				else								iRand3 = 130000;
			}
			else if( COMPARE( iRandom, 7800, 10001) )	{
				iRand2 = 80000000;
				iItemKey = myrand(0, 10000);			// 사제갑옷은 남자와 여자만 가짐
				if( COMPARE( iItemKey, 0, 5000) )	iRand3 = 120000;
				else								iRand3 = 130000;
			}

		}

		iTemp2 = myrand(0, 10000);					// 몸의 부위 아이템 결정
		if( COMPARE( iTemp2, 0, 2000) )				iRand4 = 1000;
		else if( COMPARE( iTemp2, 2000, 4000) )		iRand4 = 2000;
		else if( COMPARE( iTemp2, 4000, 6000) )		iRand4 = 3000;
		else if( COMPARE( iTemp2, 6000, 8000) )		iRand4 = 4000;
		else if( COMPARE( iTemp2, 8000, 10001) )	iRand4 = 5000;
		iRandom = GetItemCodeNumber(iItemLevel, 2);				// 레이매직표 적용
		if(iRandom == -1)	{		// 잘못된 아이템 생성실패
			return 0;
		}
		iRand5 = iRandom * 10;
		iItemNumber = iDefault + iRand2 + iItemCode + iRand3 + iRand4 + iRand5 + iItemGrade;	// iItemGrade : 아이템 등급생성표 적용
		//TRACE("ItemProdution : Defensive Success item_number = %d, default=%d, iRand2=%d, itemcode=%d, iRand3=%d, iRand4=%d, iRand5, iItemGrade=%d\n", iItemNumber, iDefault, iRand2, iItemCode, iRand3, iRand4, iRand5, iItemGrade);
	}
	else if( COMPARE( iRandom, 8001, 10001) )	{       // 악세사리 아이템
		iDefault = 300000000;
		iRandom = myrand(0, 10000);					// 악세사리 종류결정(귀고리, 목걸이, 반지, 벨트)
		if( COMPARE( iRandom, 0, 2500) )			iRand2 = 10000000;
		else if( COMPARE( iRandom, 2500, 5000) )	iRand2 = 20000000;
		else if( COMPARE( iRandom, 5000, 7500) )	iRand2 = 30000000;
		else if( COMPARE( iRandom, 7500, 10001) )	iRand2 = 40000000;
		iRand3 = myrand(1, 10000);					// 종족(엘모라드, 카루스)
		if( COMPARE( iRand3, 1, 5000) )	iRand3 = 110000;
		else	iRand3 = 150000;
		iRandom = GetItemCodeNumber(iItemLevel, 3);	// 레이매직표 적용
		//TRACE("ItemProdution : GetItemCodeNumber() = %d\n", iRandom);
		if(iRandom == -1)	{		// 잘못된 아이템 생성실패
			return 0;
		}
		iRand4 = iRandom * 10;
		iItemNumber = iDefault + iRand2 + iRand3 + iRand4 + iItemGrade;
		//TRACE("ItemProdution : Accessary Success item_number = %d, default=%d, iRand2=%d, iRand3=%d, iRand4=%d, iItemGrade=%d\n", iItemNumber, iDefault, iRand2, iRand3, iRand4, iItemGrade);
	}

	return iItemNumber;
}

int  CNpc::GetItemGrade(int item_grade)
{
	int iPercent = 0, iRandom = 0, i=0;
	_MAKE_ITEM_GRADE_CODE* pItemData = nullptr;

	iRandom = myrand(1, 1000);
	pItemData = g_pMain->m_MakeGradeItemArray.GetData(item_grade); 
	if(pItemData == nullptr)	return 0;


	for(i=0; i<9; i++)	{
		if(i == 0)	{
			if(pItemData->sGrade[i] == 0)	{
				iPercent += pItemData->sGrade[i];
				continue;
			}
			if( COMPARE( iRandom, 0, pItemData->sGrade[i]) )	return i+1;
			else	{
				iPercent += pItemData->sGrade[i];
				continue;
			}
		}
		else	{
			if(pItemData->sGrade[i] == 0)	{
				iPercent += pItemData->sGrade[i];
				continue;
			}

			if( COMPARE( iRandom, iPercent, iPercent+pItemData->sGrade[i]) )	return i+1;
			else	{
				iPercent += pItemData->sGrade[i];
				continue;
			}
		}

	}

	return 0;
}

int CNpc::GetWeaponItemCodeNumber(bool bWeapon)
{
	_MAKE_WEAPON * pItemData = nullptr;

	if (bWeapon)
		pItemData = g_pMain->m_MakeWeaponItemArray.GetData(GetLevel() / 10); 
	else
		pItemData = g_pMain->m_MakeDefensiveItemArray.GetData(GetLevel() / 10); 

	if (pItemData == nullptr)
		return 0;

	for (int i = 0, iPercent = 0, iRandom = myrand(0, 1000); i < MAX_UPGRADE_WEAPON; i++)
	{
		if (pItemData->sClass[i] == 0)	
		{
			iPercent += pItemData->sClass[i];
			continue;
		}

		if (COMPARE(iRandom, iPercent, iPercent + pItemData->sClass[i]))
			return i + 1;

		iPercent += pItemData->sClass[i];
	}

	return 0;
}

int  CNpc::GetItemCodeNumber(int level, int item_type)
{
	int iItemCode = 0, iItemType = 0, iPercent = 0;

	_MAKE_ITEM_LARE_CODE * pItemData = g_pMain->m_MakeLareItemArray.GetData(level); 
	if (pItemData == nullptr)	
		return -1;

	int iItemPercent[] = { pItemData->sLareItem, pItemData->sMagicItem, pItemData->sGeneralItem };
	int iRandom = myrand(0, 1000);
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)	
		{
			if (COMPARE(iRandom, 0, iItemPercent[i]))
			{
				iItemType = i+1;
				break;
			}
			else	
			{
				iPercent += iItemPercent[i];
				continue;
			}
		}
		else
		{
			if (COMPARE(iRandom, iPercent, iPercent+iItemPercent[i]))
			{
				iItemType = i+1;
				break;
			}
			else	{
				iPercent += iItemPercent[i];
				continue;
			}
		}
	}

	switch (iItemType)
	{
	case 1:						// lare item
		if (item_type == 1)
			iItemCode = myrand(16, 24);
		else if (item_type == 2)
			iItemCode = myrand(12, 24);
		else if (item_type == 3)
			iItemCode = myrand(0, 10);
		break;

	case 2:						// magic item
		if (item_type == 1)
			iItemCode = myrand(6, 15);
		else if (item_type == 2)
			iItemCode = myrand(6, 11);
		else if (item_type == 3)
			iItemCode = myrand(0, 10);
		break;

	case 3:						// general item
		if (item_type == 1
			|| item_type == 2)
			iItemCode = 5;
		else if (item_type == 3)
			iItemCode = myrand(0, 10);
		break;	
	}

	return iItemCode;
}

time_t CNpc::NpcSleeping()
{
	if (!g_pMain->m_bIsNight)	
	{
		m_NpcState = NPC_STANDING;
		return m_Delay;
	}

	m_NpcState = NPC_SLEEPING;
	return m_sStandTime;
}

time_t CNpc::NpcFainting()
{
	if (UNIXTIME < (m_tFaintingTime + FAINTING_TIME)) 
		return -1;

	m_NpcState = NPC_STANDING;
	m_tFaintingTime = 0;
	return 0;
}

time_t CNpc::NpcHealing()
{
	if (!isHealer())
	{
		InitTarget();
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	auto result = IsCloseTarget(m_byAttackRange, AttackTypeMagic);
	if (result == CloseTargetNotInRange)   
	{
		m_sStepCount = 0;
		m_byActionFlag = ATTACK_TO_TRACE;
		m_NpcState = NPC_TRACING;
		return 0;
	}	
	else if (result == CloseTargetInAttackRange)
	{
		if (GetProto()->m_byDirectAttack == 2)
			return LongAndMagicAttack();

		m_sStepCount = 0;
		m_byActionFlag = ATTACK_TO_TRACE;
		m_NpcState = NPC_TRACING;
		return 0;
	}
	else if (result == CloseTargetInvalid)
	{
		m_NpcState = NPC_STANDING;
		InitTarget();
		return 0;
	}

	if (hasTarget()
		&& m_Target.id >= NPC_BAND)	
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(m_Target.id);
		if (pNpc == nullptr
			|| pNpc->isDead())
		{
			InitTarget();
			return m_sStandTime;
		}

		int iHP = (int)(pNpc->GetMaxHealth() * 0.9);
		if (pNpc->GetHealth() >= iHP)	
		{
			InitTarget();
		}
		else	
		{
			CNpcMagicProcess::MagicPacket(MAGIC_EFFECTING, m_proto->m_iMagic3, GetID(), m_Target.id);
			return m_sAttackDelay;
		}
	}

	int iMonsterNid = FindFriend(MonSearchNeedsHealing);
	if (iMonsterNid == 0)	
	{
		InitTarget();
		m_NpcState = NPC_STANDING;
		return m_sStandTime;
	}

	CNpcMagicProcess::MagicPacket(MAGIC_EFFECTING, m_proto->m_iMagic3, GetID(), iMonsterNid);
	return m_sAttackDelay;
}

time_t CNpc::NpcCasting()
{
	if (isDead())
		return -1;

	// Officially the attack delay overlaps with the cast time, so more often than not
	// by the time the skill's cast, there should be very little to no delay for all cases I can see.
	// Regardless, we'll allow for longer delays if set.
	// NOTE: If it goes below 0 (which it will most of the time), the caller won't care to handle it.
	time_t tAttackDelay = m_sAttackDelay - m_sActiveCastTime;

	CNpcMagicProcess::MagicPacket(MAGIC_EFFECTING, m_nActiveSkillID, GetID(), m_sActiveTargetID);

	m_NpcState = m_OldNpcState;
	m_nActiveSkillID = 0;
	m_sActiveTargetID = -1;
	m_sActiveCastTime = 0;

	return tAttackDelay;
}

int CNpc::GetPartyExp(int party_level, int man, int nNpcExp)
{
	int nPartyExp = 0;
	int nLevel = party_level / man;
	double TempValue = 0;
	nLevel = GetLevel() - nLevel;

	if (nLevel < 2)
	{
		nPartyExp = nNpcExp; // x1
	}
	else if (nLevel >= 2 && nLevel < 5)
	{
		TempValue = nNpcExp * 1.2; // x1.2
		nPartyExp = (int)TempValue;
		if (TempValue > nPartyExp)  
			nPartyExp++;
	}
	else if (nLevel >= 5 && nLevel < 8)	
	{
		TempValue = nNpcExp * 1.5; // x1.5
		nPartyExp = (int)TempValue;
		if (TempValue > nPartyExp)  
			nPartyExp++;
	}
	else if (nLevel >= 8)	
	{
		nPartyExp = nNpcExp * 2; // x2
	}	

	return nPartyExp;
}

void CNpc::ChangeAbility(int iChangeType)
{
	if (iChangeType > 2)
		return;

	int nHP = 0, nAC=0, nDamage=0, nMagicR=0, nDiseaseR=0, nPoisonR=0, nLightningR=0, nFireR=0, nColdR=0;
	CNpcTable*	pNpcTable = GetProto();

	if (iChangeType == BATTLEZONE_OPEN)
	{
		nHP = (int)(pNpcTable->m_iMaxHP / 2);
		nAC = (int)(pNpcTable->m_sDefense * 0.2);
		nDamage = (int)(pNpcTable->m_sDamage * 0.3);
		nMagicR = (int)(pNpcTable->m_byMagicR / 2);
		nDiseaseR = (int)(pNpcTable->m_byDiseaseR / 2);
		nPoisonR = (int)(pNpcTable->m_byPoisonR / 2);
		nLightningR = (int)(pNpcTable->m_byLightningR / 2);
		nFireR = (int)(pNpcTable->m_byFireR / 2);
		nColdR = (int)(pNpcTable->m_byColdR / 2);
		m_iMaxHP = nHP;

		if (GetHealth() > nHP)
			HpChange();

		m_sTotalAc		= nAC;
		m_sTotalHit		= nDamage;
		m_sFireR		= nFireR;
		m_sColdR		= nColdR;
		m_sLightningR	= nLightningR;
		m_sMagicR		= nMagicR;
		m_sDiseaseR		= nDiseaseR;
		m_sPoisonR		= nPoisonR;
	}
	else if (iChangeType == BATTLEZONE_CLOSE)
	{
		m_iMaxHP		= pNpcTable->m_iMaxHP;
		if (GetMaxHealth() > GetHealth())
		{
			m_iHP = GetMaxHealth() - 50;
			HpChange();
		}

		m_sTotalHit		= pNpcTable->m_sDamage;
		m_sTotalAc		= pNpcTable->m_sDefense;
		m_sFireR		= pNpcTable->m_byFireR;
		m_sColdR		= pNpcTable->m_byColdR;
		m_sLightningR	= pNpcTable->m_byLightningR;
		m_sMagicR		= pNpcTable->m_byMagicR;
		m_sDiseaseR		= pNpcTable->m_byDiseaseR;
		m_sPoisonR		= pNpcTable->m_byPoisonR;
	}
}
