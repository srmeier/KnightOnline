﻿#include "stdafx.h"
#include "Map.h"
#include "KnightsManager.h"
#include "../shared/tstring.h"
#include "DBAgent.h"

// TODO: Move this to the CUser class.
void CKnightsManager::PacketProcess(CUser *pUser, Packet & pkt)
{
	if (pUser == nullptr)
		return;

	uint8_t opcode = pkt.read<uint8_t>();
	TRACE("Clan packet: %X\n", opcode); 
	switch (opcode)
	{
	case KNIGHTS_CREATE:
		CreateKnights(pUser, pkt);
		break;
	case KNIGHTS_JOIN:
		JoinKnights(pUser, pkt);
		break;
	case KNIGHTS_WITHDRAW:
		WithdrawKnights(pUser, pkt);
		break;
	case KNIGHTS_REMOVE:
	case KNIGHTS_ADMIT:
	case KNIGHTS_REJECT:
	case KNIGHTS_CHIEF:
	case KNIGHTS_VICECHIEF:
	case KNIGHTS_OFFICER:
	case KNIGHTS_PUNISH:
		ModifyKnightsMember(pUser, pkt, opcode);
		break;
	case KNIGHTS_DESTROY:
		DestroyKnights(pUser);
		break;
	case KNIGHTS_ALLLIST_REQ:
		AllKnightsList(pUser, pkt);
		break;
	case KNIGHTS_MEMBER_REQ:
		AllKnightsMember(pUser);
		break;
	case KNIGHTS_CURRENT_REQ:
		CurrentKnightsMember(pUser, pkt);
		break;
	case KNIGHTS_JOIN_REQ:
		JoinKnightsReq(pUser, pkt);
		break;
	case KNIGHTS_MARK_REGISTER:
		RegisterClanSymbol(pUser, pkt);
		break;
	case KNIGHTS_MARK_VERSION_REQ:
		RequestClanSymbolVersion(pUser, pkt);
		break;
	case KNIGHTS_MARK_REGION_REQ:
		RequestClanSymbols(pUser, pkt);
		break;
	case KNIGHTS_MARK_REQ:
		GetClanSymbol(pUser, pkt.read<uint16_t>());
		break;
	case KNIGHTS_ALLY_CREATE:
		KnightsAllianceCreate(pUser, pkt);
		break;
	case KNIGHTS_ALLY_REQ:
		KnightsAllianceRequest(pUser, pkt);
		break;
	case KNIGHTS_ALLY_INSERT:
		KnightsAllianceInsert(pUser, pkt);
		break;
	case KNIGHTS_ALLY_REMOVE:
		KnightsAllianceRemove(pUser, pkt);
		break;
	case KNIGHTS_ALLY_PUNISH:
		KnightsAlliancePunish(pUser, pkt);
		break;
	case KNIGHTS_ALLY_LIST:
		KnightsAllianceList(pUser, pkt);
		break;

	default:
		TRACE("Unhandled clan system opcode: %X\n", opcode);
	}
}

void CKnightsManager::CreateKnights(CUser* pUser, Packet & pkt)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	std::string idname;
	uint8_t ret_value = 0;
	pkt >> idname;

	if (idname.empty() || idname.size() > MAX_ID_SIZE
		|| !IsAvailableName(idname.c_str()))
		ret_value = 3;
	else if (pUser->GetClanID() != 0)
		ret_value = 5;
	else if (g_pMain->m_nServerGroup == 2)
		ret_value = 8;
	else if (!pUser->GetMap()->canUpdateClan())
		ret_value = 9;
	else if (pUser->GetLevel() < CLAN_LEVEL_REQUIREMENT)
		ret_value = 2;
	else if (!pUser->hasCoins(CLAN_COIN_REQUIREMENT))
		ret_value = 4;

	result << uint8_t(KNIGHTS_CREATE);
	if (ret_value == 0)
	{
		uint16_t knightindex = GetKnightsIndex(pUser->m_bNation);
		if (knightindex >= 0)
		{	
			result	<< uint8_t(ClanTypeTraining) 
				<< knightindex << pUser->GetNation()
				<< idname << pUser->GetName();
			g_pMain->AddDatabaseRequest(result, pUser);
			return;
		}
		ret_value = 6;
	}

	result << ret_value;
	pUser->Send(&result);
}

bool CKnightsManager::IsAvailableName( const char *strname)
{
	foreach_stlmap (itr, g_pMain->m_KnightsArray)
		if (STRCASECMP(itr->second->GetName().c_str(), strname) == 0)
			return false;

	return true;
}

int CKnightsManager::GetKnightsIndex( int nation )
{
	Guard lock(g_pMain->m_KnightsArray.m_lock);

	int knightindex = 0;
	if (nation == ELMORAD)	knightindex = 15000;

	foreach_stlmap (itr, g_pMain->m_KnightsArray)
	{
		if (itr->second != nullptr && 
			knightindex < itr->second->GetID())
		{
			if (nation == KARUS && itr->second->GetID() >= 15000)
				continue;

			knightindex = itr->second->GetID();
		}
	}

	knightindex++;
	if ((nation == KARUS && (knightindex >= 15000 || knightindex < 0))
		|| nation == ELMORAD && (knightindex < 15000 || knightindex > 30000)
		|| g_pMain->GetClanPtr(knightindex))
		return -1;

	return knightindex;
}

void CKnightsManager::JoinKnights(CUser *pUser, Packet & pkt)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	uint8_t bResult = 0;

	do
	{
		if (!pUser->GetMap()->canUpdateClan())
			bResult = 12;
		else if (!pUser->isClanLeader() && !pUser->isClanAssistant())
			bResult = 6;

		if (bResult != 0)
			break;

		uint16_t sClanID = pUser->GetClanID();
		CKnights *pKnights = g_pMain->GetClanPtr(sClanID);
		if (pKnights == nullptr)
		{
			bResult = 7;
			break;
		}

		CUser *pTUser = g_pMain->GetUserPtr(pkt.read<uint16_t>());
		if (pTUser == nullptr)
			bResult = 2;
		else if (pTUser->isDead())
			bResult = 3;
		else if (pTUser->GetNation() != pUser->GetNation())
			bResult = 4;
		else if (pTUser->GetClanID() > 0)
			bResult = 5;

		if (bResult != 0)
			break;

		result	<< uint8_t(KNIGHTS_JOIN_REQ) << uint8_t(1)
			<< pUser->GetSocketID() << sClanID << pKnights->m_strName;
		pTUser->Send(&result);
		return;
	} while (0);


	result << uint8_t(KNIGHTS_JOIN) << bResult;
	pUser->Send(&result);
}

void CKnightsManager::JoinKnightsReq(CUser *pUser, Packet & pkt)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	uint8_t bFlag, bResult = 0;
	uint16_t sid, sClanID;
	pkt >> bFlag >> sid >> sClanID;
	CUser *pTUser = g_pMain->GetUserPtr(sid);
	if (pTUser == nullptr)
		bResult = 2;
	else if (bFlag == 0)
		bResult = 11;
	else
	{
		CKnights *pKnights = g_pMain->GetClanPtr(sClanID);
		if (pKnights == nullptr)
			bResult = 7;
		else if (pKnights->m_sMembers >= MAX_CLAN_USERS)
			bResult = 8;
	}

	result << uint8_t(KNIGHTS_JOIN);
	if (bResult != 0)
	{
		result << bResult;
		pUser->Send(&result);
	}
	else
	{
		result << sClanID;
		g_pMain->AddDatabaseRequest(result, pUser);
	}
}

void CKnightsManager::WithdrawKnights(CUser *pUser, Packet & pkt)
{
	if (pUser == nullptr)
		return;

	CKnights * pKinghts = g_pMain->GetClanPtr(pUser->m_bKnights);
	Packet result(WIZ_KNIGHTS_PROCESS);
	uint8_t bResult = 0;
	do
	{
		if (!pUser->isInClan())
			bResult = 10;
		else if (pUser->isClanLeader() && !pUser->GetMap()->canUpdateClan())
			bResult = 12;

		if (bResult != 0)
			break;

		result	<< uint8_t(pUser->isClanLeader() ? KNIGHTS_DESTROY : KNIGHTS_WITHDRAW)
			<< pUser->GetClanID();
		ReqKnightsAllianceRemove(pUser, pkt);
		g_pMain->AddDatabaseRequest(result, pUser);
		return;
	} while (0);
	result << bResult;
	pUser->Send(&result);
}

void CKnightsManager::DestroyKnights( CUser* pUser )
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	uint8_t bResult = 1;
	if (!pUser->isClanLeader())
		bResult = 0;
	else if (!pUser->GetMap()->canUpdateClan())
		bResult = 12;

	result << uint8_t(KNIGHTS_DESTROY);

	if (bResult == 1)
	{
		result << pUser->GetClanID();
		g_pMain->AddDatabaseRequest(result, pUser);
	}
	else
	{
		result << bResult;
		pUser->Send(&result);
	}
}

void CKnightsManager::ModifyKnightsMember(CUser *pUser, Packet & pkt, uint8_t opcode)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	uint8_t bResult = 1, bRemoveFlag = 0;
	std::string strUserID;

	pkt >> strUserID;

	if (pUser->GetName() == strUserID)
		return;

	result << opcode;
	do
	{
		if (strUserID.empty() || strUserID.size() > MAX_ID_SIZE)
			bResult = 2;
		else if (!pUser->GetMap()->canUpdateClan())
			bResult = 12;
		else if (STRCASECMP(strUserID.c_str(), pUser->GetName().c_str()) == 0)
			bResult = 9;
		else if (((opcode == KNIGHTS_ADMIT || opcode == KNIGHTS_REJECT) && pUser->GetFame() < OFFICER)
			|| (opcode == KNIGHTS_PUNISH && pUser->GetFame() < VICECHIEF))
			bResult = 0;	
		else if (opcode != KNIGHTS_ADMIT && opcode != KNIGHTS_REJECT && opcode != KNIGHTS_PUNISH 
			&& !pUser->isClanLeader())
			bResult = 6;

		CKnights *pKnight = g_pMain->GetClanPtr(pUser->GetClanID());
		if (pKnight->m_strViceChief_1 == "")
			bResult = 1;	
		else if (pKnight->m_strViceChief_2 == "")
			bResult = 1;	
		else if (pKnight->m_strViceChief_3 == "")
			bResult = 1;	
		else if (opcode == KNIGHTS_VICECHIEF)
			bResult = 0;

		if (bResult != 1)
			break;

		CUser *pTUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		if (pTUser == nullptr)
		{
			if (opcode != KNIGHTS_REMOVE)	
				bResult = 2;
		}
		else
		{
			if (pUser->GetNation() != pTUser->GetNation())
				bResult = 4;
			else if (pUser->GetClanID() != pTUser->GetClanID())
				bResult = 5;

			if (bResult == 1 && opcode == KNIGHTS_VICECHIEF)
			{
				if (pTUser->isClanAssistant())
					bResult = 8;
				else if (!g_pMain->GetClanPtr(pUser->GetClanID()))	
					bResult = 7;
			}

			bRemoveFlag = 1;
		}

		if (bResult != 1)
			break;

		result << pUser->GetClanID() << strUserID << bRemoveFlag;
		g_pMain->AddDatabaseRequest(result, pUser);
		return;
	} while (0);

	result << bResult;
	pUser->Send(&result);
}

void CKnightsManager::AllKnightsList(CUser *pUser, Packet & pkt)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	uint16_t sPage = pkt.read<uint16_t>(), start = sPage * 10, count = 0;
	result << uint8_t(KNIGHTS_ALLLIST_REQ) << uint8_t(1) << sPage;
	
	size_t wpos = result.wpos();
	result << count;

	foreach_stlmap (itr, g_pMain->m_KnightsArray)
	{
		CKnights* pKnights = itr->second;
		if (pKnights == nullptr
			|| !pKnights->isPromoted()
			|| pKnights->m_byNation != pUser->GetNation()
			|| count++ < start) 
			continue;

		result	<< uint16_t(pKnights->m_sIndex) << pKnights->m_strName 
			<< uint16_t(pKnights->m_sMembers) << pKnights->m_strChief 
			<< uint32_t(pKnights->m_nPoints);
		if (count >= start + 10)
			break;
	}

	count -= start;
	result.put(wpos, count);
	pUser->Send(&result);
}

void CKnightsManager::AllKnightsMember(CUser *pUser)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	result << uint8_t(KNIGHTS_MEMBER_REQ);

	uint8_t bResult = 1;

	if (!pUser->isInClan())
		bResult = 2;
	else if (g_pMain->GetClanPtr(pUser->GetClanID()) == nullptr)
		bResult = 7;

	result << bResult;
	if (bResult == 1)
	{
		uint16_t pktSize = 0, count = 0;
		size_t wpos = result.wpos();
		result << pktSize << count << count << count; // placeholders
		pktSize = (uint16_t)result.size();
		count = g_pMain->GetKnightsAllMembers(pUser->GetClanID(), result, pktSize, pUser->isClanLeader());
		if (count > MAX_CLAN_USERS) 
			return;

		pktSize = ((uint16_t)result.size() - pktSize) + 6;
		result.put(wpos, pktSize);
		result.put(wpos + 2, count);
		result.put(wpos + 4, count);
		result.put(wpos + 6, count);
	}
	pUser->Send(&result);
}

void CKnightsManager::CurrentKnightsMember(CUser *pUser, Packet & pkt)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	result << uint8_t(KNIGHTS_CURRENT_REQ);

	CKnights *pKnights = nullptr;
	if (!pUser->isInClan()
		|| (pKnights = g_pMain->GetClanPtr(pUser->GetClanID())) == nullptr)
	{
		result << uint8_t(0); // failed
		result << "is this error still used?";
		pUser->Send(&result);
		return;
	}

	uint16_t page = pkt.read<uint16_t>();
	uint16_t start = page * 10;
	uint16_t count = 0;

	result	<< uint8_t(1) // success
		<< pKnights->m_strChief
		<< page;

	size_t pos = result.wpos();
	result	<< count; // placeholder

	foreach_array (i, pKnights->m_arKnightsUser)
	{
		_KNIGHTS_USER *p = &pKnights->m_arKnightsUser[i];
		if (!p->byUsed || p->pSession == nullptr
			|| count++ < start)
			continue;

		CUser *pTUser = p->pSession;
		result << pUser->GetName() << pUser->GetFame() << pUser->GetLevel() << pUser->GetClass();
		count++;
		if (count >= start + 10)
			break;
	}

	count -= start;
	result.put(pos, count);
	pUser->Send(&result);
}

void CKnightsManager::RecvUpdateKnights(CUser *pUser, Packet & pkt, uint8_t command)
{
	if (pUser == nullptr) 
		return;

	uint16_t sClanID = pkt.read<uint16_t>();
	CKnights *pKnights = g_pMain->GetClanPtr(sClanID);
	if (pKnights == nullptr)
		return;

	if (command == KNIGHTS_JOIN)
	{
		std::string noticeText;
		g_pMain->GetServerResource(IDS_KNIGHTS_JOIN, &noticeText, pUser->GetName().c_str());
		pKnights->AddUser(pUser);
		pKnights->SendChat("%s", noticeText.c_str());

	}
	else if (command == KNIGHTS_WITHDRAW || command == KNIGHTS_REMOVE)
	{
		pKnights->RemoveUser(pUser);
	}

	Packet result(WIZ_KNIGHTS_PROCESS);
	result << command << uint8_t(1) << pUser->GetSocketID() << pUser->GetClanID() << pUser->GetFame();
	
	CKnights *aKnights = g_pMain->GetClanPtr(pKnights->GetAllianceID());

	if (command == KNIGHTS_JOIN)
	{
		result << pKnights->m_byFlag
			<< pKnights->GetAllianceID()
			<< pKnights->GetCapeID(aKnights) 
			<< int16_t(pKnights->m_sMarkVersion) 
			<< pKnights->m_strName << pKnights->m_byGrade << pKnights->m_byRanking;
	}

	pUser->SendToRegion(&result);
}

void CKnightsManager::RecvModifyFame(CUser *pUser, Packet & pkt, uint8_t command)
{
	if (pUser == nullptr) 
		return;

	std::string clanNotice;
	std::string strUserID;
	uint16_t sClanID;

	pkt >> sClanID >> strUserID;

	CUser *pTUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
	CKnights *pKnights = g_pMain->GetClanPtr(sClanID);

	switch (command)
	{
	case KNIGHTS_REMOVE:
		if (pTUser != nullptr)
			pKnights->RemoveUser(pTUser);
		else
			pKnights->RemoveUser(strUserID);

		g_pMain->GetServerResource(IDS_KNIGHTS_REMOVE, &clanNotice, pTUser == nullptr ? strUserID.c_str() : pTUser->GetName().c_str());
		break;
	case KNIGHTS_ADMIT:
		if (pTUser != nullptr)
			pTUser->m_bFame = KNIGHT;
		break;
	case KNIGHTS_REJECT:
		if (pTUser != nullptr)
		{
			pTUser->SetClanID(0);
			pTUser->m_bFame = 0;

			RemoveKnightsUser(sClanID, pTUser->GetName());
		}
		break;
	case KNIGHTS_CHIEF:
		if (pTUser != nullptr)
		{
			pTUser->m_bFame = CHIEF;
			g_pMain->GetServerResource(IDS_KNIGHTS_CHIEF, &clanNotice, pTUser->GetName().c_str());
		}
		break;
	case KNIGHTS_VICECHIEF:
		if (pTUser != nullptr)
		{
			pTUser->m_bFame = VICECHIEF;
			g_pMain->GetServerResource(IDS_KNIGHTS_VICECHIEF, &clanNotice, pTUser->GetName().c_str());
		}
		break;
	case KNIGHTS_OFFICER:
		if (pTUser != nullptr)
			pTUser->m_bFame = OFFICER;
		break;
	case KNIGHTS_PUNISH:
		if (pTUser != nullptr)
			pTUser->m_bFame = PUNISH;
		break;
	}

	if (pTUser != nullptr)
		pTUser->SendClanUserStatusUpdate(command == KNIGHTS_REMOVE);

	if (clanNotice.empty())
		return;

	// Construct the clan system chat packet
	Packet result;
	ChatPacket::Construct(&result, KNIGHTS_CHAT, &clanNotice);

	// If we've been removed from a clan, tell the user as well (since they're no longer in the clan)
	if (command == KNIGHTS_REMOVE && pTUser != nullptr)
		pTUser->Send(&result);

	// Otherwise, since we're actually in the clan, we don't need to be explicitly told what happened.
	if (pKnights != nullptr)
		pKnights->Send(&result);
}

bool CKnightsManager::AddKnightsUser(int index, std::string & strUserID)
{
	CKnights *pKnights = g_pMain->GetClanPtr(index);
	return (pKnights == nullptr ? false : pKnights->AddUser(strUserID));
}

bool CKnightsManager::RemoveKnightsUser(int index, std::string & strUserID)
{
	CKnights *pKnights = g_pMain->GetClanPtr(index);
	return (pKnights == nullptr ? false : pKnights->RemoveUser(strUserID));
}

void CKnightsManager::UpdateKnightsGrade(uint16_t sClanID, uint8_t byFlag)
{
	CKnights * pClan = g_pMain->GetClanPtr(sClanID);
	if (pClan == nullptr)
		return;

	if (byFlag == ClanTypeTraining)
		pClan->m_sCape = -1;
	else if (byFlag == ClanTypePromoted)
	{
		pClan->m_sCape = 0;

		// if (pClan->isInAlliance())
		//	KnightsAllianceRemove(pClan);
	}

	pClan->m_byFlag = byFlag;
	pClan->SendUpdate();

	// Update the database server
	Packet result(WIZ_KNIGHTS_PROCESS);
	result << uint8_t(KNIGHTS_UPDATE_GRADE) << sClanID << byFlag << pClan->m_sCape;
	g_pMain->AddDatabaseRequest(result);
}

void CKnightsManager::RecvKnightsAllList(Packet & pkt)
{
	Packet result(WIZ_KNIGHTS_PROCESS);
	uint8_t count = pkt.read<uint8_t>(), send_count = 0;
	result << uint8_t(KNIGHTS_ALLLIST_REQ);
	size_t wpos = result.wpos();
	result << send_count; // placeholder for count
	for (int i = 0; i < count; i++)
	{
		uint32_t nPoints; uint16_t sClanID; uint8_t bRank;
		pkt >> sClanID >> nPoints >> bRank;

		CKnights *pKnights = g_pMain->GetClanPtr(sClanID);
		if (pKnights == nullptr)
			continue;

		if (pKnights->m_nPoints != nPoints
			|| pKnights->m_byRanking != bRank)
		{
			pKnights->m_nPoints = nPoints;
			pKnights->m_byRanking = bRank;
			pKnights->m_byGrade = g_pMain->GetKnightsGrade(nPoints);

			result << sClanID << pKnights->m_byGrade << pKnights->m_byRanking;
			send_count++;
		}
	}

	result.put(wpos, send_count);
	g_pMain->Send_All(&result);
}

void CKnightsManager::RegisterClanSymbol(CUser* pUser, Packet & pkt)
{
	if (pUser == nullptr || !pUser->isInClan())
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	CKnights *pKnights = nullptr;
	char clanSymbol[MAX_KNIGHTS_MARK];
	uint16_t sFailCode = 1, sSymbolSize = pkt.read<uint16_t>();

	result << uint8_t(KNIGHTS_MARK_REGISTER);

	// Are they even a clan leader?
	if (!pUser->isClanLeader())
		sFailCode = 11;
	// Invalid zone (only in home zones)
	else if (pUser->GetZoneID() != pUser->GetNation())
		sFailCode = 12;
	// Invalid symbol size (or invalid packet)
	else if (sSymbolSize == 0 
		|| sSymbolSize > MAX_KNIGHTS_MARK
		|| pkt.wpos() < sSymbolSize)
		sFailCode = 13;
	// User doesn't have enough coins
	else if (pUser->m_iGold < CLAN_SYMBOL_COST)
		sFailCode = 14;
	// Clan doesn't exist
	else if ((pKnights = g_pMain->GetClanPtr(pUser->GetClanID())) == nullptr)
		sFailCode = 20;
	// Clan not promoted
	else if (!pKnights->isPromoted())
		sFailCode = 11;

	// Uh oh, did we error?
	if (sFailCode != 1)
	{
		result << sFailCode;
		pUser->Send(&result);
		return;
	}

	// Read the clan symbol from the packet
	pkt.read(clanSymbol, sSymbolSize);

	// Nope? Let's update the clan symbol.
	result	<< pUser->GetClanID() << sSymbolSize;
	result.append(clanSymbol, sSymbolSize);
	g_pMain->AddDatabaseRequest(result, pUser);
}

void CKnightsManager::RequestClanSymbolVersion(CUser* pUser, Packet & pkt)
{
	if (pUser == nullptr
		|| !pUser->isInClan())
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	int16_t sFailCode = 1;

	CKnights *pKnights = g_pMain->GetClanPtr(pUser->GetClanID());
	if (pKnights == nullptr || !pKnights->isPromoted() || !pUser->isClanLeader())
		sFailCode = 11;
	else if (pUser->GetZoneID() != pUser->GetNation())
		sFailCode = 12;

	result << uint8_t(KNIGHTS_MARK_VERSION_REQ) << sFailCode;

	if (sFailCode == 1)
		result << uint16_t(pKnights->m_sMarkVersion);

	pUser->Send(&result);
}

/**
* The clan member (leader only?) tells groups of users to update the clan symbols
* they have for this clan. This is a horrible, horrible idea.
**/
void CKnightsManager::RequestClanSymbols(CUser* pUser, Packet & pkt)
{
	// Should we force them to be a clan leader too? 
	// Need to check if *any* clan member can trigger this, or if it's just leaders.
	if (pUser == nullptr
		|| !pUser->isInClan())
		return;

	uint16_t sCount = pkt.read<uint16_t>();
	if (sCount > 100)
		sCount = 100;

	for (int i = 0; i < sCount; i++)
	{
		uint16_t sid = pkt.read<uint16_t>();
		CUser *pTUser = g_pMain->GetUserPtr(sid);
		if (pTUser == nullptr
			|| !pTUser->isInGame())
			continue;

		// This is really quite scary that users can send directly to specific players like this.
		// Quite possibly we should replace this with a completely server-side implementation.
		GetClanSymbol(pTUser, pUser->GetClanID());
	}
}

void CKnightsManager::GetClanSymbol(CUser* pUser, uint16_t sClanID)
{
	if (pUser == nullptr)
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	CKnights *pKnights = g_pMain->GetClanPtr(sClanID);

	// Dose that clan exist?
	if (pKnights == nullptr 
		// Are they promoted?
			|| !pKnights->isPromoted()
			// Is their symbol version set?
			|| pKnights->m_sMarkVersion == 0
			// The clan symbol is more than 0 bytes, right?
			|| pKnights->m_sMarkLen <= 0)
			return;

	result	<< uint8_t(KNIGHTS_MARK_REQ) << uint16_t(1); // success
	result	<< uint16_t(pKnights->m_byNation) << sClanID
		<< uint16_t(pKnights->m_sMarkVersion) << uint16_t(pKnights->m_sMarkLen);
	result.append(pKnights->m_Image, pKnights->m_sMarkLen);
	pUser->SendCompressed(&result);
}

/**
* @brief	Performs CSW/Delos checks to determine whether an alliance
* 			can be made with the specified clan.
*
* @param	pMainClan  	The clan seeking to form an alliance.
* @param	pTargetClan	Target clan an alliance is being formed with.
*
* @return	true if it succeeds, false if it fails.
*/
bool CKnightsManager::CheckAlliance(CKnights * pMainClan, CKnights * pTargetClan)
{
	_KNIGHTS_ALLIANCE * pMainAlliance = g_pMain->GetAlliancePtr(pMainClan->GetAllianceID()) , 
					* pTargetAlliance = g_pMain->GetAlliancePtr(pTargetClan->GetAllianceID());
	if(pMainAlliance == pTargetAlliance && (pMainAlliance != nullptr || pTargetAlliance != nullptr))
		return true;
		else
			return false;
}

void CKnightsManager::KnightsAllianceDestroy(CUser* pUser, Packet & pkt)
{
	return;
}

/**
* @brief	Handles the client packet responsible for creating an alliance
* 			with another clan.
*
* @param	pUser	The user.
* @param	pkt  	The packet.
*/
void CKnightsManager::KnightsAllianceCreate(CUser* pUser, Packet & pkt)
{
	if (pUser == nullptr
		|| pUser->isDead())
		return;

	CKnights * pKnights = g_pMain->GetClanPtr(pUser->m_bKnights);

	if(pKnights->m_byFlag > 1 && pKnights->m_strChief == pUser->GetName())
	{
	Packet result(WIZ_KNIGHTS_PROCESS);
	result << uint8_t(KNIGHTS_ALLY_CREATE) << uint8_t(1) << pUser->m_bKnights;
	pUser->SendToRegion(&result);

	g_pMain->AddDatabaseRequest(result, pUser);
	}
}

/**
* @brief	Handles the client packet responsible for forming an alliance
* 			with another clan. We must already lead an alliance.
* 			
* 			NOTE: Most of the logic (error codes also) is for the most part 
* 			identical to the creation of the alliance. 
* 			It just doesn't _create_ an alliance (and requires the alliance to exist).
*
* @param	pUser	The user.
* @param	pkt  	The packet.
*/
void CKnightsManager::KnightsAllianceInsert(CUser* pUser, Packet & pkt)
{
	if (pUser == nullptr
		|| pUser->isDead())
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	CKnights * pKnights = g_pMain->GetClanPtr(pUser->m_bKnights);
	uint16_t TargetClanLeaderID = pkt.read<uint16_t>();
	CUser * pTargetUser = g_pMain->GetUserPtr(TargetClanLeaderID);
	CKnights * pTargetKnights = g_pMain->GetClanPtr(pTargetUser->m_bKnights);
	_KNIGHTS_ALLIANCE * pAlliance = g_pMain->GetAlliancePtr(pUser->m_bKnights);
	if (pAlliance == nullptr)
	{
		KnightsAllianceCreate(pUser,pkt);
		g_pMain->ReloadKnightAndUserRanks();
		pAlliance = g_pMain->GetAlliancePtr(pUser->m_bKnights);
	}
	if(pAlliance != nullptr)
	{
	CKnights * pKnights1 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_1),
		* pKnights2 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_2),
		* pKnights3 = g_pMain->GetClanPtr(pAlliance->sSubAllianceKnights);
	
	if(!pTargetUser->isClanLeader())
		return;

	if( pKnights->m_byFlag > 1 && 
		!pTargetKnights->isInAlliance() &&
		pKnights->m_strChief == pUser->GetName() &&
		pAlliance != nullptr &&
		pAlliance->sMainAllianceKnights == pKnights->GetAllianceID() &&
		pAlliance->sMercenaryClan_1 != pTargetKnights->m_sIndex &&
		pAlliance->sMercenaryClan_2 != pTargetKnights->m_sIndex &&
		pAlliance->sSubAllianceKnights != pTargetKnights->m_sIndex)
	{
		result << uint8_t(KNIGHTS_ALLY_INSERT) << uint8_t(1) << pUser->m_bKnights << pTargetUser->m_bKnights << pKnights->m_sCape;
		pTargetUser->SendToRegion(&result);
		
		g_pMain->AddDatabaseRequest(result, pUser);
		pTargetKnights->m_sCape = pKnights->m_sCape;
		std::string noticeText;
		g_pMain->GetServerResource(IDS_INSERT_KNIGHTS_ALLIANCE, &noticeText, pTargetKnights->GetName().c_str());
		pKnights->SendChatAlliance("%s", noticeText.c_str());
		if(pKnights1 != nullptr)
		pKnights1->SendChatAlliance("%s", noticeText.c_str());
		if(pKnights2 != nullptr)
		pKnights2->SendChatAlliance("%s", noticeText.c_str());
		if(pKnights3 != nullptr)
		pKnights3->SendChatAlliance("%s", noticeText.c_str());
		}
	}
}

void CKnightsManager::KnightsAllianceRequest(CUser* pUser, Packet & pkt)
{
	if (pUser == nullptr
		|| pUser->isDead())
			return;
	else
		KnightsAllianceInsert(pUser,pkt);

	Packet result(WIZ_KNIGHTS_PROCESS, uint8_t(KNIGHTS_ALLY_REQ));
	CKnights * pKnights = g_pMain->GetClanPtr(pUser->m_bKnights);
	uint16_t TargetClanLeaderID = pkt.read<uint16_t>();
	CUser * pTargetUser = g_pMain->GetUserPtr(TargetClanLeaderID);
	CKnights * pTargetKnights = g_pMain->GetClanPtr(pTargetUser->m_bKnights);
	_KNIGHTS_ALLIANCE * pAlliance = g_pMain->GetAlliancePtr(pUser->m_bKnights);
	if( pAlliance == nullptr)
	{
		KnightsAllianceCreate(pUser,pkt);
		g_pMain->ReloadKnightAndUserRanks();
		pAlliance = g_pMain->GetAlliancePtr(pUser->m_bKnights);
	}
	if(pAlliance != nullptr)
	{
	CKnights * pKnights1 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_1),
		* pKnights2 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_2),
		* pKnights3 = g_pMain->GetClanPtr(pAlliance->sSubAllianceKnights);
	
	if(!pTargetUser->isClanLeader())
		return;

	if( pKnights->m_byFlag > 1 && 
		!pTargetKnights->isInAlliance() &&
		pKnights->m_strChief == pUser->GetName() &&
		pAlliance != nullptr &&
		pAlliance->sMainAllianceKnights == pKnights->GetAllianceID() &&
		pAlliance->sMercenaryClan_1 != pTargetKnights->m_sIndex &&
		pAlliance->sMercenaryClan_2 != pTargetKnights->m_sIndex &&
		pAlliance->sSubAllianceKnights != pTargetKnights->m_sIndex)
		{
		result.SByte();
		result << uint8_t(1) << pKnights->GetName();
		pTargetUser->Send(&result);
		}
		}
}

void CKnightsManager::KnightsAllianceRemove(CUser* pUser, Packet & pkt)
{	
	if (pUser == nullptr
		|| pUser->isDead())
		return;
	
	Packet result(WIZ_KNIGHTS_PROCESS);  // ally disband
	result << uint8_t(KNIGHTS_ALLY_REMOVE);

	CKnights * pKnights = g_pMain->GetClanPtr(pUser->m_bKnights) , *pTargetKnights = g_pMain->GetClanPtr(pKnights->m_sAlliance);
	_KNIGHTS_ALLIANCE * pAlliance = g_pMain->GetAlliancePtr(pKnights->m_sAlliance);

	if (pAlliance == nullptr)
	{
		KnightsAllianceCreate(pUser,pkt);
		g_pMain->ReloadKnightAndUserRanks();
		pAlliance = g_pMain->GetAlliancePtr(pUser->m_bKnights);
	}

	if (pAlliance != nullptr)
	{
		CKnights * pKnights1 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_1),
			* pKnights2 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_2),
			* pKnights3 = g_pMain->GetClanPtr(pAlliance->sSubAllianceKnights);

		if ((pKnights->m_strChief == pUser->GetName()
			|| pTargetKnights->m_strChief == pUser->GetName())
			&& !pTargetKnights->isInAlliance()
			&& pAlliance != nullptr
			&& pAlliance->sMainAllianceKnights == pKnights->GetAllianceID())
		{
			result << uint8_t(KNIGHTS_ALLY_REMOVE) << uint8_t(1) << pKnights->m_sAlliance << pUser->m_bKnights << uint16_t(-1);
			pUser->SendToRegion(&result);

			pTargetKnights->m_sCape = -1;

			g_pMain->AddDatabaseRequest(result, pUser);

			std::string noticeText;
			g_pMain->GetServerResource(IDS_REMOVE_KNIGHTS_ALLIANCE, &noticeText, pKnights->GetName().c_str());

			pTargetKnights->SendChatAlliance("%s", noticeText.c_str());

			if (pKnights1 != nullptr)
				pKnights1->SendChatAlliance("%s", noticeText.c_str());

			if (pKnights2 != nullptr)
				pKnights2->SendChatAlliance("%s", noticeText.c_str());

			if (pKnights3 != nullptr)
				pKnights3->SendChatAlliance("%s", noticeText.c_str());
		}
	}
}

void CKnightsManager::KnightsAlliancePunish(CUser* pUser, Packet & pkt)
{	
	if (pUser == nullptr
	|| pUser->isDead())
	return;

	Packet result(WIZ_KNIGHTS_PROCESS); // knights ban
	result << uint8_t(KNIGHTS_ALLY_PUNISH);
	uint16_t TargetClanID = pkt.read<uint16_t>(), MainCapeID;
	CKnights *pTargetKnights = g_pMain->GetClanPtr(TargetClanID) , *pKnights = g_pMain->GetClanPtr(pUser->m_bKnights);
	_KNIGHTS_ALLIANCE * pAlliance = g_pMain->GetAlliancePtr(pUser->m_bKnights);
	if( pAlliance != nullptr)
	{
		CKnights * pKnights1 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_1),
			* pKnights2 = g_pMain->GetClanPtr(pAlliance->sMercenaryClan_2),
			* pKnights3 = g_pMain->GetClanPtr(pAlliance->sSubAllianceKnights);

		CUser *pTargetUser = nullptr;
		if (pTargetKnights != nullptr
			&& !pTargetKnights->m_strChief.empty())
			pTargetUser = g_pMain->GetUserPtr(pTargetKnights->m_strChief, TYPE_CHARACTER);

	if (pAlliance->sMainAllianceKnights == pKnights->GetID())
		MainCapeID = pKnights->m_sCape;
	else
		MainCapeID = -1;

	if (pKnights->m_byFlag > 1
		&& pKnights->m_strChief == pUser->GetName()
		&& pAlliance != nullptr
		&& pTargetKnights->isInAlliance()
		&& pAlliance->sMainAllianceKnights == pKnights->GetAllianceID())
	{
		if (pTargetUser != nullptr)
		{
			result << uint8_t(1) << pUser->m_bKnights << pTargetUser->m_bKnights << MainCapeID;
			pTargetUser->SendToRegion(&result);
		}
		else
		{
			result << uint8_t(1) << pUser->m_bKnights << TargetClanID << MainCapeID;
			pUser->SendToRegion(&result);
		}

		g_pMain->AddDatabaseRequest(result, pUser);

		pTargetKnights->m_sCape = MainCapeID;

		std::string noticeText;
		g_pMain->GetServerResource(IDS_PUNISH_KNIGHTS_ALLIANCE, &noticeText, pTargetKnights->GetName().c_str());

		pKnights->SendChatAlliance("%s", noticeText.c_str());

		if (pKnights1 != nullptr)
			pKnights1->SendChatAlliance("%s", noticeText.c_str());

		if (pKnights2 != nullptr)
			pKnights2->SendChatAlliance("%s", noticeText.c_str());

		if (pKnights3 != nullptr)
			pKnights3->SendChatAlliance("%s", noticeText.c_str());
		}
	}
}

/**
* @brief	Handles the client packet responsible for displaying
* 			all clans that are part of the user's clan's alliance.
*
* @param	pUser	The user.
* @param	pkt  	The packet.
*/
void CKnightsManager::KnightsAllianceList(CUser* pUser, Packet & pkt)
{
	if (pUser == nullptr
		|| !pUser->isInClan())
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	_KNIGHTS_ALLIANCE * pAlliance;
	CKnights * pClan = g_pMain->GetClanPtr(pUser->GetClanID());

	result << uint8_t(KNIGHTS_ALLY_LIST);

	if (pClan == nullptr
		|| !pClan->isInAlliance()
		|| (pAlliance = g_pMain->GetAlliancePtr(pClan->GetAllianceID())) == nullptr)
	{
		result << uint8_t(0);
		pUser->Send(&result);
		return;
	}

	uint16_t clans[] = 
	{ 
		pAlliance->sMainAllianceKnights, pAlliance->sSubAllianceKnights, 
		pAlliance->sMercenaryClan_1, pAlliance->sMercenaryClan_2 
	};

	size_t wpos = result.wpos();
	uint8_t clanCount = 0;
	result << clanCount;

	result.SByte();
	foreach_array(i, clans)
	{
		uint16_t sClanID = clans[i]; 
		CKnights * pTmp = g_pMain->GetClanPtr(sClanID);
		if (pTmp == nullptr)
			continue;

		result << pTmp->GetID() << pTmp->GetName() << pTmp->isInAlliance(); /* unknown flag */
		clanCount++;
	}

	if (clanCount == 0)
		return;

	result.put(wpos, clanCount);
	pUser->Send(&result);
}
