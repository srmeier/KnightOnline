#include "stdafx.h"
#include "User.h"
#include "Region.h"
#include "GameSocket.h"
#include "Npc.h"
#include "MAP.h"

#define MORAL_GOOD		0x01
#define MORAL_BAD		0x02
#define MORAL_NEUTRAL	0x03

static float surround_fx[8] = {0.0f, -0.7071f, -1.0f, -0.7083f,  0.0f,  0.7059f,  1.0000f, 0.7083f};
static float surround_fz[8] = {1.0f,  0.7071f,  0.0f, -0.7059f, -1.0f, -0.7083f, -0.0017f, 0.7059f};

CUser::CUser() : Unit(UnitPlayer) {}
CUser::~CUser() {}

void CUser::Initialize()
{
	Unit::Initialize();

	m_iUserId = -1;
	m_bLive = AI_USER_DEAD;
	m_sHP = m_sMP = 0;
	m_sMaxHP = m_sMaxMP = 0;
	m_state = 0;
	m_sItemAc = 0;
	m_byNowParty = 0;
	m_sPartyTotalLevel = 0;
	m_byPartyTotalMan = 0;
	m_sPartyNumber = -1;
	m_byIsOP = 0;
	m_bInvisibilityType = 0;

	m_tLastRegeneTime = 0;
	m_sExpGainAmount = 0;
	m_bMaxWeightAmount = 100;
	m_bNPGainAmount = m_bNoahGainAmount = 100;
	m_bPlayerAttackAmount = 100; 
	m_bSkillNPBonus = 0;
	m_bAddWeaponDamage = 0;
	m_sAddArmourAc = 0;
	m_bPctArmourAc = 100;
	m_bPremiumMerchant = false;

	InitNpcAttack();
}

void CUser::OnDeath(Unit * pAttacker)
{
	if (m_bLive == AI_USER_DEAD)
		return;

	m_sHP = 0;
	m_bLive = AI_USER_DEAD;

	InitNpcAttack();

	MAP* pMap = GetMap();
	if (pMap == nullptr)
		return;

	pMap->RegionUserRemove(m_sRegionX, m_sRegionZ, GetID());
}

void CUser::InitNpcAttack()
{
	memset(&m_sSurroundNpcNumber, -1, sizeof(m_sSurroundNpcNumber));
}

int CUser::IsSurroundCheck(float fX, float fY, float fZ, int NpcID)
{
	int nDir = 0;
	__Vector3 vNpc, vUser, vDis;
	vNpc.Set(fX, fY, fZ);
	float fDX, fDZ;
	float fDis = 0.0f, fCurDis=1000.0f;
	bool bFlag = false;
	for(int i=0; i<8; i++)
	{
		//if(m_sSurroundNpcNumber[i] != -1) continue;
		if(m_sSurroundNpcNumber[i] == NpcID)
		{
			if (bFlag)
				m_sSurroundNpcNumber[i] = -1;
			else
			{
				m_sSurroundNpcNumber[i] = NpcID;
				nDir = i+1;
				bFlag = true;
			}
			//return nDir;
		}

		if(m_sSurroundNpcNumber[i] == -1 && bFlag==false)
		{
			fDX = GetX() + surround_fx[i]; 
			fDZ = GetZ() + surround_fz[i]; 
			vUser.Set(fDX, 0.0f, fDZ);
			vDis = vUser - vNpc;
			fDis = vDis.Magnitude();
			if(fDis < fCurDis)
			{
				nDir = i+1;
				fCurDis = fDis;
			}
		}
	}


	/*	TRACE("User-Sur : [0=%d,1=%d,2=%d,3=%d,4=%d,5=%d,6=%d,7=%d]\n", m_sSurroundNpcNumber[0], 
	m_sSurroundNpcNumber[1], m_sSurroundNpcNumber[2], m_sSurroundNpcNumber[3], m_sSurroundNpcNumber[4],
	m_sSurroundNpcNumber[5],m_sSurroundNpcNumber[6], m_sSurroundNpcNumber[7]);
	*/
	if(nDir != 0)
	{
		m_sSurroundNpcNumber[nDir-1] = NpcID;
	}

	return nDir;
}

void CUser::HealMagic()
{
	int region_x = (int)(GetX() / VIEW_DIST);
	int region_z = (int)(GetZ() / VIEW_DIST);

	MAP* pMap = GetMap();
	if (pMap == nullptr) return;
	int min_x = region_x - 1;	if(min_x < 0) min_x = 0;
	int min_z = region_z - 1;	if(min_z < 0) min_z = 0;
	int max_x = region_x + 1;	if(max_x > pMap->GetXRegionMax()) max_x = pMap->GetXRegionMax();
	int max_z = region_z + 1;	if(min_z > pMap->GetZRegionMax()) min_z = pMap->GetZRegionMax();

	int search_x = max_x - min_x + 1;		
	int search_z = max_z - min_z + 1;	

	int i, j;

	for(i = 0; i < search_x; i++)	{
		for(j = 0; j < search_z; j++)	{
			HealAreaCheck( min_x+i, min_z+j );
		}
	}
}

void CUser::HealAreaCheck(int rx, int rz)
{
	MAP* pMap = GetMap();
	if (pMap == nullptr) return;

	if (rx < 0 || rz < 0 || rx > pMap->GetXRegionMax() || rz > pMap->GetZRegionMax())	
	{
		TRACE("#### CUser-HealAreaCheck() Fail : [nid=%d, name=%s], nRX=%d, nRZ=%d #####\n", GetID(), GetName().c_str(), rx, rz);
		return;
	}

	static const float fRadius = 10.0f; // 30m

	Guard lock(pMap->m_lock);
	CRegion *pRegion = &pMap->m_ppRegion[rx][rz];

	if (pRegion == nullptr)
		return;

	foreach_stlmap (itr, pRegion->m_RegionNpcArray)
	{
		CNpc * pNpc = g_pMain->GetNpcPtr(itr->first);
		if (pNpc == nullptr || pNpc->isDead()
			|| !pNpc->isHostileTo(this))
			continue;

		if (isInRangeSlow(pNpc, fRadius))
			pNpc->ChangeTarget(1004, this);
	}
}