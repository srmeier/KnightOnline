#include "stdafx.h"
#include "DBAgent.h"

using std::string;
using std::vector;

void CGameServerDlg::SendEventRemainingTime(bool bSendAll, CUser *pUser, uint8 ZoneID)
{
	Packet result(WIZ_BIFROST,uint8(BIFROST_EVENT));
	uint16 nRemainingTime = 0;

	if (ZoneID == ZONE_BATTLE4)
		nRemainingTime = m_byBattleRemainingTime / 2;
	else if (ZoneID == ZONE_BIFROST || ZoneID ==  ZONE_RONARK_LAND)
		nRemainingTime = m_sBifrostRemainingTime;

	result << nRemainingTime;

	if (pUser)
		pUser->Send(&result);
	else if (bSendAll)
	{
		if (ZoneID == ZONE_BATTLE4)
			Send_All(&result,nullptr, 0, ZONE_BATTLE4);
		else
		{
			Send_All(&result,nullptr, 0, ZONE_RONARK_LAND);
			Send_All(&result,nullptr, 0, ZONE_BIFROST);
		}
	}
}

void CUser::BifrostProcess(CUser * pUser)
{
	if (pUser == nullptr)
		return;

	if (g_pMain->m_BifrostVictory == 0 && g_pMain->m_bAttackBifrostMonument)
	{
		g_pMain->m_sBifrostTime = g_pMain->m_xBifrostTime;
		g_pMain->m_sBifrostRemainingTime = g_pMain->m_sBifrostTime;
		g_pMain->m_BifrostVictory = pUser->GetNation();
		g_pMain->SendFormattedResource(pUser->GetNation() == ELMORAD ? IDS_BEEF_ROAST_VICTORY_ELMORAD : IDS_BEEF_ROAST_VICTORY_KARUS, Nation::ALL,false);
		g_pMain->SendEventRemainingTime(true, nullptr, ZONE_BIFROST);

		if (g_pMain->m_bAttackBifrostMonument)
			g_pMain->m_bAttackBifrostMonument = false;
	}
	else if (g_pMain->m_BifrostVictory == 1 || g_pMain->m_BifrostVictory == 2) 
	{
		if (pUser->GetNation() != g_pMain->m_BifrostVictory && g_pMain->m_bAttackBifrostMonument)
		{
			g_pMain->m_BifrostVictory = 3;
			g_pMain->SendFormattedResource(pUser->GetNation() == ELMORAD ? IDS_BEEF_ROAST_VICTORY_ELMORAD : IDS_BEEF_ROAST_VICTORY_KARUS, Nation::ALL,false);

			if (g_pMain->m_bAttackBifrostMonument)
				g_pMain->m_bAttackBifrostMonument = false;
		}
	}
}

void CUser::CastleSiegeWarProcess(CUser * pUser)
{
	if (pUser == nullptr)
		return;

	_KNIGHTS_SIEGE_WARFARE *pKnightSiegeWar = g_pMain->GetSiegeMasterKnightsPtr(1);
	CKnights * pKnights = g_pMain->GetClanPtr(pUser->m_bKnights);

	pKnightSiegeWar->sMasterKnights = pKnights->m_sIndex;

	g_pMain->UpdateSiege(pKnightSiegeWar->sCastleIndex, pKnightSiegeWar->sMasterKnights, pKnightSiegeWar->bySiegeType, pKnightSiegeWar->byWarDay, pKnightSiegeWar->byWarTime, pKnightSiegeWar->byWarMinute);
	g_pMain->m_KnightsSiegeWarfareArray.GetData(pKnightSiegeWar->sMasterKnights);

	g_pMain->m_SiegeWarWinKnightsNotice = pKnights->GetName();
	g_pMain->Announcement(IDS_NPC_GUIDON_DESTORY);
	g_pMain->m_byBattleSiegeWarMomument = true;
	g_pMain->KickOutZoneUsers(ZONE_DELOS, ZONE_DELOS);
	if (pKnightSiegeWar != nullptr)
	{
	Packet result(WIZ_SIEGE, uint8(2));
	result << pKnights->GetID() << pKnights->m_sMarkVersion;
	g_pMain->Send_Zone(&result,ZONE_DELOS);
	}
}

void CUser::TempleProcess(Packet &pkt )
{
	uint8 opcode = pkt.read<uint8>();

	switch (opcode)
	{
	case MONSTER_STONE:
		MonsterStoneProcess();
		break;
	case TEMPLE_EVENT_JOIN:
		TempleOperations(opcode);
		break;
	case TEMPLE_EVENT_DISBAND:
		TempleOperations(opcode);
		break;
	}
}

void CUser::MonsterStoneProcess()
{
	if(CheckExistItem(ITEM_MONSTER_STONE,1))
	{
		RobItem(ITEM_MONSTER_STONE,1);
		ZoneChange(myrand(1004,1016),m_curx,m_curz);
	}
} 

void CUser::TempleOperations(uint8 bType)
{
	uint16 nActiveEvent = (uint16)g_pMain->pTempleEvent.ActiveEvent;

	uint8 bResult = 1;
	Packet result(WIZ_EVENT);

	if(bType == TEMPLE_EVENT_JOIN && !isEventUser())
	{
		if (nActiveEvent == TEMPLE_EVENT_CHAOS)
		{
			if (CheckExistItem(CHAOS_MAP,1))
				bResult = 1;
			else if (m_sItemArray[RIGHTHAND].nNum == MATTOCK || m_sItemArray[RIGHTHAND].nNum == GOLDEN_MATTOCK || isMining())
				bResult = 4; 
			else
				bResult = 3;
		}

		result << bType << bResult << nActiveEvent;
		Send(&result);

		if (bResult == 1) 
		{
			GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount++ :g_pMain->pTempleEvent.ElMoradUserCount++;
			g_pMain->pTempleEvent.AllUserCount = (g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount);
			g_pMain->AddEventUser(this);
			TempleOperations(TEMPLE_EVENT_COUNTER);
		}
	}
	else if (bType == TEMPLE_EVENT_DISBAND && isEventUser())
	{
		GetNation() == KARUS ? g_pMain->pTempleEvent.KarusUserCount-- : g_pMain->pTempleEvent.ElMoradUserCount--;
		g_pMain->pTempleEvent.AllUserCount = g_pMain->pTempleEvent.KarusUserCount + g_pMain->pTempleEvent.ElMoradUserCount;
		g_pMain->RemoveEventUser(this);
		result <<  bType << bResult << nActiveEvent;
		Send(&result);
		TempleOperations(TEMPLE_EVENT_COUNTER);
	}
	else if (bType == TEMPLE_EVENT_COUNTER)
	{
		result << bType << nActiveEvent;

		if(nActiveEvent == TEMPLE_EVENT_CHAOS)
			result << g_pMain->pTempleEvent.AllUserCount;
		else
			result << g_pMain->pTempleEvent.KarusUserCount << g_pMain->pTempleEvent.ElMoradUserCount;

		g_pMain->Send_All(&result,nullptr,Nation::ALL,0,true,0);
	}
}

void CGameServerDlg::AddEventUser(CUser *pUser)
{
	if (pUser == nullptr)
	{
		TRACE("#### AddEventUser : pUser == nullptr ####\n");
		return;
	}

	_TEMPLE_EVENT_USER * pEventUser = new _TEMPLE_EVENT_USER;

	pEventUser->m_socketID =  pUser->GetSocketID();
	pEventUser->m_bEventRoom = pUser->GetEventRoom();

	if (!g_pMain->m_TempleEventUserArray.PutData(pEventUser->m_socketID, pEventUser))
		delete pEventUser;
}

void CGameServerDlg::RemoveEventUser(CUser *pUser)
{
	if (pUser == nullptr)
	{
		TRACE("#### RemoveEventUser : pUser == nullptr ####\n");
		return;
	}

	if (g_pMain->m_TempleEventUserArray.GetData(pUser->GetSocketID()) != nullptr)
		g_pMain->m_TempleEventUserArray.DeleteData(pUser->GetSocketID());

	pUser->m_bEventRoom = 0;
}

void CGameServerDlg::UpdateEventUser(CUser *pUser, uint16 nEventRoom)
{
	if (pUser == nullptr)
	{
		TRACE("#### UpdateEventUser : pUser == nullptr ####\n");
		return;
	}

	_TEMPLE_EVENT_USER * pEventUser = g_pMain->m_TempleEventUserArray.GetData(pUser->GetSocketID());

	if (pEventUser)
	{
		pEventUser->m_bEventRoom = nEventRoom;
		pUser->m_bEventRoom = nEventRoom;
	}
}

void CGameServerDlg::SetEventUser(CUser *pUser)
{
	if (pUser == nullptr)
	{
		TRACE("#### SetEventUser : pUser == nullptr ####\n");
		return;
	}

	uint8 nMaxUserCount = 0;

	switch (g_pMain->pTempleEvent.ActiveEvent)
	{
	case TEMPLE_EVENT_BORDER_DEFENCE_WAR:
		nMaxUserCount = 16;
		break;
	case TEMPLE_EVENT_CHAOS:
		nMaxUserCount = 20;
		break;
	case TEMPLE_EVENT_JURAD_MOUNTAIN:
		nMaxUserCount = 16;
		break;
	}

	if (g_pMain->TempleEventGetRoomUsers(g_pMain->pTempleEvent.LastEventRoom) >= nMaxUserCount)
		g_pMain->pTempleEvent.LastEventRoom++;

	if (g_pMain->TempleEventGetRoomUsers(g_pMain->pTempleEvent.LastEventRoom) < nMaxUserCount)
		g_pMain->UpdateEventUser(pUser, g_pMain->pTempleEvent.LastEventRoom);
}

bool CUser::isEventUser()
{
	_TEMPLE_EVENT_USER * pEventUser = g_pMain->m_TempleEventUserArray.GetData(GetSocketID());

	if (pEventUser != nullptr)
		return true;

	return false;
}

uint8 CUser::GetMonsterChallengeTime() 
{ 
	if (g_pMain->m_bForgettenTempleIsActive
		&& g_pMain->m_nForgettenTempleLevelMin != 0 
		&& g_pMain->m_nForgettenTempleLevelMax != 0
		&& GetLevel() >= g_pMain->m_nForgettenTempleLevelMin 
		&& GetLevel() <= g_pMain->m_nForgettenTempleLevelMax
		&& !g_pMain->m_bForgettenTempleSummonMonsters)
		return g_pMain->m_nForgettenTempleChallengeTime; 

	return 0;
}

uint8 CUser::GetMonsterChallengeUserCount() { return g_pMain->m_nForgettenTempleUsers.size(); }