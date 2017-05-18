#include "stdafx.h"
#include "GameSocket.h"
#include "User.h"
#include "MAP.h"
#include "Region.h"
#include "../shared/globals.h"
#include "../GameServer/MagicProcess.h"
#include "Npc.h"

CGameSocket::~CGameSocket() {}

void CGameSocket::OnConnect()
{
	KOSocket::OnConnect();
	Initialize();
}

void CGameSocket::Initialize()
{
	m_Party.Initialize();
}

void CGameSocket::OnDisconnect()
{
	TRACE("*** CloseProcess - socketID=%d ... server=%s *** \n", GetSocketID(), GetRemoteIP().c_str());
	g_pMain->DeleteAllUserList(this);
}

bool CGameSocket::HandlePacket(Packet & pkt)
{
	switch (pkt.GetOpcode())
	{
	case AI_SERVER_CONNECT:
		RecvServerConnect(pkt);
		break;
	case AG_USER_INFO:
		RecvUserInfo(pkt);
		break;
	case AG_USER_INOUT:
		RecvUserInOut(pkt);
		break;
	case AG_USER_MOVE:
		RecvUserMove(pkt);
		break;
	case AG_USER_LOG_OUT:
		RecvUserLogOut(pkt);
		break;
	case AG_USER_REGENE:
		RecvUserRegene(pkt);
		break;
	case AG_USER_SET_HP:
		RecvUserSetHP(pkt);
		break;
	case AG_NPC_HP_CHANGE:
		RecvNpcHpChange(pkt);
		break;
	case AG_USER_UPDATE:
		RecvUserUpdate(pkt);
		break;
	case AG_ZONE_CHANGE:
		RecvZoneChange(pkt);
		break;
	case AG_USER_PARTY:
		m_Party.PartyProcess(pkt);
		break;
	case AG_USER_INFO_ALL:
		RecvUserInfoAllData(pkt);
		break;
	case AG_PARTY_INFO_ALL:
		RecvPartyInfoAllData(pkt);
		break;
	case AG_HEAL_MAGIC:
		RecvHealMagic(pkt);
		break;
	case AG_TIME_WEATHER:
		RecvTimeAndWeather(pkt);
		break;
	case AG_BATTLE_EVENT:
		RecvBattleEvent(pkt);
		break;
	case AG_NPC_GATE_OPEN:
		RecvGateOpen(pkt);
		break;
	case AG_USER_VISIBILITY:
		RecvUserVisibility(pkt);
		break;
	case AG_NPC_SPAWN_REQ:
		RecvNpcSpawnRequest(pkt);
		break;
	case AG_NPC_KILL_REQ:
		RecvNpcKillRequest(pkt);
		break;
	case AG_MAGIC_ATTACK_REQ:
		CMagicProcess::MagicPacket(pkt);
		break;
	case AG_NPC_UPDATE:
		RecvNpcUpdate(pkt);
		break;
	}
	return true;
}

void CGameSocket::RecvServerConnect(Packet & pkt)
{
	uint8_t byReconnect = pkt.read<uint8_t>();
	printf("Game Server connected - %s\n", GetRemoteIP().c_str());

	Packet result(AI_SERVER_CONNECT, byReconnect);
	Send(&result);

	if (byReconnect == 1)
		TRACE("**** ReConnect - server=%s,  socket = %d ****\n ", GetRemoteIP().c_str(), GetSocketID());
	else
		TRACE("**** Connect - server=%s,  socket = %d ****\n ", GetRemoteIP().c_str(), GetSocketID());

	g_pMain->m_bFirstServerFlag = true;
	g_pMain->AllNpcInfo();
}

void CGameSocket::RecvUserInfo(Packet & pkt)
{
	CUser *pUser = new CUser();
	pUser->Initialize();

	pkt >> pUser->m_iUserId;
	ReadUserInfo(pkt, pUser);

	if (pUser->GetName().empty() || pUser->GetName().length() > MAX_ID_SIZE)
	{
		delete pUser;
		return;
	}

	pUser->m_pMap = g_pMain->GetZoneByID(pUser->m_bZone);
	pUser->m_bLive = AI_USER_LIVE;

	TRACE("****  RecvUserInfo()---> uid = %d, name=%s ******\n", 
		pUser->GetID(), pUser->GetName().c_str());

	if (!g_pMain->SetUserPtr(pUser->GetID(), pUser))
		delete pUser;
}

void CGameSocket::ReadUserInfo(Packet & pkt, CUser * pUser)
{
	uint32_t equippedItems = 0;

	pkt.SByte();
	pkt >> pUser->m_strUserID >> pUser->m_bZone >> pUser->m_bNation 
		>> pUser->m_bLevel >> pUser->m_sHP >> pUser->m_sMP 
		>> pUser->m_sTotalHit >> pUser->m_bAttackAmount
		>> pUser->m_sTotalAc >> pUser->m_sACAmount
		>> pUser->m_fTotalHitrate >> pUser->m_fTotalEvasionrate 
		>> pUser->m_sItemAc
		>> pUser->m_sPartyNumber 
		>> pUser->m_byIsOP >> pUser->m_bInvisibilityType
		>> equippedItems;

	if (pUser->m_sPartyNumber != -1)
		pUser->m_byNowParty = 1;

	Guard lock(pUser->m_equippedItemBonusLock);
	pUser->m_equippedItemBonuses.clear();

	for (uint32_t i = 0; i < equippedItems; i++)
	{
		uint8_t bSlot; uint32_t bonusCount;
		Unit::ItemBonusMap bonusMap;

		pkt >> bSlot >> bonusCount;
		for (uint32_t x = 0; x < bonusCount; x++)
		{
			uint8_t bType; int16_t sAmount;
			pkt >> bType >> sAmount;
			bonusMap.insert(std::make_pair(bType, sAmount));
		}

		pUser->m_equippedItemBonuses[bSlot] = bonusMap;
	}
}

void CGameSocket::RecvUserInOut(Packet & pkt)
{
	std::string strUserID;
	uint8_t bType;
	uint16_t uid;
	float fX, fZ;
	pkt.SByte();
	pkt >> bType >> uid >> strUserID >> fX >> fZ;
	if (fX < 0 || fZ < 0)
	{
		TRACE("Error:: RecvUserInOut(),, uid = %d, fX=%.2f, fZ=%.2f\n", uid, fX, fZ);
		return;
	}

	int region_x = 0, region_z=0;
	int x1 = (int)fX / TILE_SIZE;
	int z1 = (int)fZ / TILE_SIZE;
	region_x = (int)fX / VIEW_DIST; 
	region_z = (int)fZ / VIEW_DIST;

	MAP* pMap = nullptr;
	CUser* pUser = g_pMain->GetUserPtr(uid);
	if (pUser == nullptr)
		return;

	pMap = pUser->GetMap();
	if (pMap == nullptr)
	{
		TRACE("#### Fail : pMap == nullptr ####\n");
		return;
	}

	if (x1 < 0 || z1 < 0 || x1 >= pMap->GetMapSize() || z1 >= pMap->GetMapSize())
	{
		TRACE("#### RecvUserInOut Fail : [name=%s], x1=%d, z1=%d #####\n", pUser->GetName().c_str(), region_x, region_z);
		return;
	}

	//if (pMap->m_pMap[x1][z1].m_sEvent == 0) return;
	if (region_x > pMap->GetXRegionMax() || region_z > pMap->GetZRegionMax())
	{
		TRACE("#### GameSocket-RecvUserInOut() Fail : [name=%s], nRX=%d, nRZ=%d #####\n", pUser->GetName().c_str(), region_x, region_z);
		return;
	}

	pUser->m_curx = pUser->m_fWill_x = fX;
	pUser->m_curz = pUser->m_fWill_z = fZ;

	// leaving a region
	if (bType == 2)	
	{
		pMap->RegionUserRemove(region_x, region_z, uid);
	}
	// entering a region
	else if (pUser->m_sRegionX != region_x || pUser->m_sRegionZ != region_z)	
	{
		pUser->m_sRegionX = region_x;		
		pUser->m_sRegionZ = region_z;

		pMap->RegionUserAdd(region_x, region_z, uid);
	}
}

void CGameSocket::RecvUserMove(Packet & pkt)
{
	uint16_t uid, speed;
	float fX, fZ, fY;
	pkt >> uid >> fX >> fZ >> fY >> speed;
	SetUid(fX, fZ, uid, speed);
}

bool CGameSocket::SetUid(float x, float z, int id, int speed)
{
	int x1 = (int)x / TILE_SIZE;
	int z1 = (int)z / TILE_SIZE;
	int nRX = (int)x / VIEW_DIST;
	int nRZ = (int)z / VIEW_DIST;

	CUser* pUser = g_pMain->GetUserPtr(id);
	if(pUser == nullptr) 
	{
		TRACE("#### User등록 실패 sid = %d ####\n", id);
		return false;
	}

	MAP* pMap = pUser->GetMap();
	if (pMap == nullptr)
	{
		TRACE("#### User not in valid zone, sid = %d ####\n", id);
		return false;
	}

	if(x1 < 0 || z1 < 0 || x1 >= pMap->GetMapSize() || z1 >= pMap->GetMapSize())
	{
		TRACE("#### GameSocket ,, SetUid Fail : [nid=%d, name=%s], x1=%d, z1=%d #####\n", id, pUser->GetName().c_str(), x1, z1);
		return false;
	}

	if(nRX > pMap->GetXRegionMax() || nRZ > pMap->GetZRegionMax())
	{
		TRACE("#### GameSocket , SetUid Fail : [nid=%d, name=%s], nRX=%d, nRZ=%d #####\n", id, pUser->GetName().c_str(), nRX, nRZ);
		return false;
	}

	// if(pMap->m_pMap[x1][z1].m_sEvent == 0) return false;

	if (pUser != nullptr)
	{
		if (pUser->isDead())
			return false;

		///// attack ~ 
		if( speed != 0 )	{
			pUser->m_curx = pUser->m_fWill_x;
			pUser->m_curz = pUser->m_fWill_z;
			pUser->m_fWill_x = x;
			pUser->m_fWill_z = z;
		}
		else	{
			pUser->m_curx = pUser->m_fWill_x = x;
			pUser->m_curz = pUser->m_fWill_z = z;
		}
		/////~ attack 

		if(pUser->m_sRegionX != nRX || pUser->m_sRegionZ != nRZ)
		{
			pMap->RegionUserRemove(pUser->m_sRegionX, pUser->m_sRegionZ, id);
			pUser->m_sRegionX = nRX;		pUser->m_sRegionZ = nRZ;
			pMap->RegionUserAdd(pUser->m_sRegionX, pUser->m_sRegionZ, id);
		}
	}

	// dungeon work
	int room = pMap->IsRoomCheck( x, z );

	return true;
}

void CGameSocket::RecvUserLogOut(Packet & pkt)
{
	uint16_t sessionId;
	std::string strUserID;
	pkt >> sessionId >> strUserID; // double byte string for once

	g_pMain->DeleteUserPtr(sessionId);
	TRACE("**** User LogOut -- uid = %d, name = %s\n", sessionId, strUserID.c_str());
}

void CGameSocket::RecvUserRegene(Packet & pkt)
{
	uint16_t uid, sHP;
	pkt >> uid >> sHP;

	CUser* pUser = g_pMain->GetUserPtr(uid);
	if(pUser == nullptr)	
		return;

	pUser->m_bLive = AI_USER_LIVE;
	pUser->m_sHP = sHP;

	TRACE("**** RecvUserRegene -- uid = (%s,%d), HP = %d\n", pUser->GetName().c_str(), pUser->GetID(), pUser->m_sHP);
}

void CGameSocket::RecvUserSetHP(Packet & pkt)
{
	uint16_t sid, sHP, tid;
	pkt >> sid >> sHP >> tid;

	CUser* pUser = g_pMain->GetUserPtr(sid);
	Unit * pAttacker = g_pMain->GetUnitPtr(tid);

	if (pUser == nullptr
		|| pUser->m_sHP == sHP)
		return;

	pUser->m_sHP = sHP;
	if (sHP == 0)
		pUser->OnDeath(pAttacker);
}

void CGameSocket::RecvNpcHpChange(Packet & pkt)
{
	int16_t nid, sAttackerID;
	int32_t nHP, nAmount;
	uint8_t attributeType = AttributeNone;

	pkt >> nid >> sAttackerID >> nHP >> nAmount >> attributeType;
	CNpc * pNpc = g_pMain->GetNpcPtr(nid);
	if (pNpc == nullptr)
		return;

	if (nAmount < 0)
	{
		pNpc->RecvAttackReq(-nAmount, sAttackerID, (AttributeType) attributeType);
	}
	else
	{		
		pNpc->m_iHP += nAmount;
		if (pNpc->m_iHP > pNpc->m_iMaxHP)
			pNpc->m_iHP = pNpc->m_iMaxHP;
	}
}

void CGameSocket::RecvUserUpdate(Packet & pkt)
{
	CUser* pUser = g_pMain->GetUserPtr(pkt.read<uint16_t>());
	if (pUser == nullptr)
		return;

	ReadUserInfo(pkt, pUser);
}

void CGameSocket::RecvZoneChange(Packet & pkt)
{
	uint16_t uid = pkt.read<uint16_t>();
	uint8_t byZoneNumber = pkt.read<uint8_t>();

	CUser* pUser = g_pMain->GetUserPtr(uid);
	if (pUser == nullptr)	
		return;

	pUser->m_pMap = g_pMain->GetZoneByID(byZoneNumber);
	pUser->m_bZone = byZoneNumber;

	TRACE("**** RecvZoneChange -- user(%s, %d), cur_zone = %d\n", pUser->GetName().c_str(), pUser->GetID(), byZoneNumber);
}

void CGameSocket::RecvUserInfoAllData(Packet & pkt)
{
	uint8_t byCount = pkt.read<uint8_t>();
	for (int i = 0; i < byCount; i++)
		RecvUserInfo(pkt);
}

void CGameSocket::RecvGateOpen(Packet & pkt)
{
	uint16_t nid;
	bool byGateOpen;

	pkt >> nid >> byGateOpen;
	if (nid < NPC_BAND)	
	{
		TRACE("####   RecvGateOpen()  nid Fail --> nid = %d  ####\n", nid);
		return;
	}

	CNpc* pNpc = g_pMain->GetNpcPtr(nid);
	if (pNpc == nullptr)		
		return;

	if (!pNpc->isGate()) 
	{
		TRACE("####   RecvGateOpen()  NpcType Fail --> type = %d  ####\n", pNpc->GetType());
		return;
	}

	pNpc->m_byGateOpen = byGateOpen;
	TRACE("****  RecvGateOpen()---> nid = %d, byGateOpen = %d  ******\n", nid, byGateOpen);
}

void CGameSocket::RecvUserVisibility(Packet & pkt)
{
	uint16_t sid;
	uint8_t bIsInvisible;
	pkt >> sid >> bIsInvisible;

	CUser *pUser = g_pMain->GetUserPtr(sid);
	if (pUser == nullptr)
		return;

	pUser->m_bInvisibilityType = bIsInvisible;
}

void CGameSocket::RecvPartyInfoAllData(Packet & pkt)
{
	uint16_t sPartyIndex = pkt.read<uint16_t>();
	if (sPartyIndex >= SHRT_MAX)
	{
		TRACE("#### RecvPartyInfoAllData Index Fail -  index = %d ####\n", sPartyIndex);
		return;
	}

	_PARTY_GROUP *pParty = new _PARTY_GROUP;
	pParty->wIndex = sPartyIndex;

	for (int i = 0; i < MAX_PARTY_USERS; i++)
		pParty->uid[i] = pkt.read<uint16_t>();

	if (g_pMain->m_arParty.PutData(pParty->wIndex, pParty))
		TRACE("****  RecvPartyInfoAllData()---> PartyIndex = %d  ******\n", sPartyIndex);
}

void CGameSocket::RecvHealMagic(Packet & pkt)
{
	uint16_t sid = pkt.read<uint16_t>();
	CUser* pUser = g_pMain->GetUserPtr(sid);

	if (pUser == nullptr
		|| pUser->isDead())
		return;

	pUser->HealMagic();
}

void CGameSocket::RecvTimeAndWeather(Packet & pkt)
{
	pkt >> g_pMain->m_iYear >> g_pMain->m_iMonth >> g_pMain->m_iDate 
		>> g_pMain->m_iHour >> g_pMain->m_iMin 
		>> g_pMain->m_iWeather >> g_pMain->m_iAmount;

	// We'll class day time as 6am to 9pm.
	g_pMain->m_bIsNight = (g_pMain->m_iHour <= 5 || g_pMain->m_iHour >= 21);
}

void CGameSocket::RecvBattleEvent(Packet & pkt)
{
	uint8_t bType = pkt.read<uint8_t>(), bEvent = pkt.read<uint8_t>();

	if (bEvent == BATTLEZONE_OPEN || bEvent == BATTLEZONE_CLOSE)
	{
		g_pMain->m_sKillKarusNpc = 0;
		g_pMain->m_sKillElmoNpc = 0;
		g_pMain->m_byBattleEvent = bEvent;
		if (bEvent == BATTLEZONE_CLOSE)
			g_pMain->ResetBattleZone();
	}

	foreach_stlmap (itr, g_pMain->m_arNpc)
	{
		CNpc *pNpc = itr->second;
		if (pNpc == nullptr)
			continue;

		if (pNpc->GetType() > 10 && (pNpc->GetNation() == KARUS || pNpc->GetNation() == ELMORAD))
		{
			if (bEvent == BATTLEZONE_OPEN || bEvent == BATTLEZONE_CLOSE)
				pNpc->ChangeAbility(bEvent);
		}
	}
}

void CGameSocket::RecvNpcSpawnRequest(Packet & pkt)
{
	uint16_t sSid, sX, sY, sZ, sCount, sRadius, sDuration;
	int16_t socketID;
	uint16_t nEventRoom;
	uint8_t byZone;
	uint8_t nation;
	bool bIsMonster;
	float fX, fY, fZ;

	pkt >> sSid >> bIsMonster >> byZone >> sX >> sY >> sZ >> sCount >> sRadius >> sDuration >> nation >> socketID >> nEventRoom;

	fX = sX / 10.0f;
	fY = sY / 10.0f;
	fZ = sZ / 10.0f;

	int16_t minRange = -(sRadius / 2);
	for (uint16_t i = 0; i < sCount; i++)
	{
		g_pMain->SpawnEventNpc(sSid, bIsMonster, 
			byZone, 
			(float)(fX + myrand(minRange, sRadius)), 
			fY, 
			(float)(fZ + myrand(minRange, sRadius)), sDuration, nation, socketID, nEventRoom);
	}
}

void CGameSocket::RecvNpcKillRequest(Packet & pkt)
{
	uint16_t nid;
	pkt >> nid;

	if (nid < NPC_BAND)	// is player
	{
		foreach_stlmap (itr, g_pMain->m_arNpc)
		{
			CNpc *pNpc = itr->second;
			if (pNpc == nullptr)
				continue;

			if (pNpc->m_oSocketID != nid)
				continue;

			pNpc->m_oSocketID = -1;
			pNpc->Dead();
		}
	}
	else
	{
		CNpc* pNpc = g_pMain->GetNpcPtr(nid);

		if (pNpc)
			pNpc->Dead();
	}
}

void CGameSocket::RecvNpcUpdate(Packet & pkt)
{
	uint16_t sSid;
	bool bIsMonster;
	uint8_t byGroup = 0;
	uint16_t sPid = 0;	

	pkt >> sSid >> bIsMonster >> byGroup >> sPid;

	g_pMain->NpcUpdate(sSid, bIsMonster, byGroup, sPid);
}
