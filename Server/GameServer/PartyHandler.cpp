#include "stdafx.h"
#include "../shared/KOSocketMgr.h"

using namespace std;

void CUser::PartyProcess(Packet & pkt)
{
	// TODO: Clean this entire system up.
	string strUserID;
	CUser *pUser;
	uint8 opcode = pkt.read<uint8>();
	switch (opcode)
	{
	case PARTY_CREATE: // Attempt to create a party.
	case PARTY_INSERT: // Attempt to invite a user to an existing party.
		pkt >> strUserID;
		if (strUserID.empty() || strUserID.size() > MAX_ID_SIZE)
			return;

		pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		if (pUser == nullptr)
			return;

		PartyRequest(pUser->GetSocketID(), (opcode == PARTY_CREATE));
		break;

		// Did the user we invited accept our party invite?
	case PARTY_PERMIT:
		if (pkt.read<uint8>()) 
			PartyInsert();
		else
			PartyCancel();
		break;

		// Authorises a member as the new party leader
	case PARTY_PROMOTE:
		PartyPromote(pkt.read<uint16>());
		break;

		// Remove a user from our party.
	case PARTY_REMOVE:
		PartyRemove(pkt.read<uint16>());
		break;

		// Disband our party.
	case PARTY_DELETE:
		PartyDelete();
		break;
	}
}

void CUser::PartyCancel()
{
	int leader_id = -1, count = 0;

	if (!isInParty())
		return;

	_PARTY_GROUP *pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr)
		return;

	leader_id = pParty->uid[0];
	CUser *pUser = g_pMain->GetUserPtr(leader_id);
	if (pUser == nullptr)
		return;

	m_bInParty = false;
	m_sPartyIndex = -1;

	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{		
		if (pParty->uid[i] >= 0)
			count++;
	}

	if (count == 1)
		pUser->PartyDelete();

	Packet result(WIZ_PARTY, uint8(PARTY_INSERT));
	result << int16(-1);
	pUser->Send(&result);
}

void CUser::PartyRequest(int memberid, bool bCreate)
{
	Packet result;
	int16 errorCode = -1, i=0;
	_PARTY_GROUP* pParty = nullptr;

	CUser *pUser = g_pMain->GetUserPtr(memberid);
	if (pUser == nullptr
		|| pUser == this
		|| pUser->isInParty()) 
		goto fail_return;

	// Only allow partying of the enemy nation in Moradon or FT.
	// Also, we threw in a check to prevent them from partying from other zones.
	if ((GetNation() != pUser->GetNation() && GetZoneID() != ZONE_MORADON && GetZoneID() != ZONE_FORGOTTEN_TEMPLE)
		|| GetZoneID() != pUser->GetZoneID()
		|| GetZoneID() == ZONE_CHAOS_DUNGEON)
	{
		errorCode = -3;
		goto fail_return;
	}

	// Players taking the beginner/"chicken" quest can party anyone.
	if (!m_bIsChicken)
	{
		// Players in the same clan can party together, no matter the level difference.
		if (!isInClan() 
			|| GetClanID() != pUser->GetClanID())
		{
			if (!((pUser->GetLevel() <= (int)(GetLevel() * 1.5) && pUser->GetLevel() >= (int)(GetLevel() * 2 / 3))
				|| (pUser->GetLevel() <= (GetLevel() + 8) && pUser->GetLevel() >= ((int)(GetLevel()) - 8))))
			{
				errorCode = -2;
				goto fail_return;
			}
		}
	}

	if (!bCreate)
	{
		pParty = g_pMain->GetPartyPtr(GetPartyID());
		if (pParty == nullptr)
			goto fail_return;

		for (i = 0; i < MAX_PARTY_USERS; i++)
		{
			if (pParty->uid[i] < 0) 
				break;
		}
		if (i == MAX_PARTY_USERS)
			goto fail_return;	// party is full
	}
	else
	{
		if (isInParty())
			goto fail_return;	// can't create a party if we're already in one

		pParty = g_pMain->CreateParty(this);
		if (pParty == nullptr)
			goto fail_return;

		m_bPartyLeader = true;
		StateChangeServerDirect(6, 1); // give party leader the 'P' symbol

		result.Initialize(AG_USER_PARTY);
		result << uint8(PARTY_CREATE) << pParty->wIndex << pParty->uid[0];
		Send_AIServer(&result);
	}

	pUser->m_sPartyIndex = m_sPartyIndex;
	pUser->m_bInParty = true;

	result.Initialize(WIZ_PARTY);
	result << uint8(PARTY_PERMIT) << GetSocketID() << GetName();
	pUser->Send(&result);
	return;

fail_return:
	result.Initialize(WIZ_PARTY);
	result << uint8(PARTY_INSERT) << errorCode;
	Send(&result);
}

void CUser::PartyInsert()
{
	Packet result(WIZ_PARTY);
	CUser* pUser = nullptr;
	_PARTY_GROUP* pParty = nullptr;
	uint8 byIndex = 0xFF;
	int leader_id = -1;

	if (!isInParty())
		return;

	pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr)
	{
		m_bInParty = false;
		m_sPartyIndex = -1;
		return;
	}

	leader_id = pParty->uid[0];
	pUser = g_pMain->GetUserPtr(leader_id);
	if (pUser == nullptr)
		return;

	if (pUser->GetZoneID() != GetZoneID() 
		|| GetPartyMemberAmount(pParty) == MAX_PARTY_USERS)
	{
		PartyCancel();
		return;
	}

	// make sure user isn't already in the array...
	// kind of slow, but it works for the moment
	foreach_array (i, pParty->uid)
	{
		if (pParty->uid[i] == GetSocketID())
		{
			m_bInParty = false;
			m_sPartyIndex = -1;
			pParty->uid[i] = -1;
			return;
		}
	}

	// Send the played who just joined the existing party list
	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		// No player set?
		if (pParty->uid[i] < 0)
		{
			// If we're not in the list yet, add us.
			if (byIndex == 0xFF)
			{
				pParty->uid[i] = GetSocketID();
				byIndex = i;
			}
			continue;
		}

		// For everyone else, 
		pUser = g_pMain->GetUserPtr(pParty->uid[i]);
		if (pUser == nullptr)
			continue;

		result.clear();
		result	<< uint8(PARTY_INSERT) << pParty->uid[i]
		<< uint8(1) // success
			<< pUser->GetName()
			<< pUser->m_iMaxHp << pUser->m_sHp
			<< pUser->GetLevel() << pUser->m_sClass
			<< pUser->m_iMaxMp << pUser->m_sMp 
			<< pUser->GetNation();
		Send(&result);
	}

	pUser = g_pMain->GetUserPtr(pParty->uid[0]);
	if (pUser == nullptr)
		return;

	if (pUser->m_bNeedParty == 2 && pUser->isInParty())
		pUser->StateChangeServerDirect(2, 1);

	if (m_bNeedParty == 2 && isInParty()) 
		StateChangeServerDirect(2, 1);

	result.clear();
	result	<< uint8(PARTY_INSERT) << GetSocketID()
		<< uint8(1) // success
		<< GetName()
		<< m_iMaxHp << m_sHp
		<< GetLevel() << GetClass()
		<< m_iMaxMp << m_sMp
		<< GetNation();
	g_pMain->Send_PartyMember(GetPartyID(), &result);

	result.Initialize(AG_USER_PARTY);
	result	<< uint8(PARTY_INSERT) << pParty->wIndex << byIndex << GetSocketID();
	Send_AIServer(&result);
}

void CUser::PartyPromote(uint16 sMemberID)
{
	// Only the existing party leader can promote a new party leader.
	if (!isPartyLeader())
		return;

	// Ensure this user exists and that they're in our party already.
	CUser * pUser = g_pMain->GetUserPtr(sMemberID);
	if (pUser == nullptr
		|| pUser->GetPartyID() != GetPartyID())
		return;

	// Ensure this party exists.
	_PARTY_GROUP * pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr)
		return;

	// Find the position of the user to promote in the array.
	uint8 pos = 0;
	for (uint8 i = 1; i < MAX_PARTY_USERS; i++)
	{
		if (pParty->uid[i] != sMemberID)
			continue;

		pos = i;
		break;
	}

	// Didn't find it? (leader's always position 0, no need to check there)
	if (pos == 0)
		return;

	// Swap the IDs around, so they have the leadership position.
	std::swap(pParty->uid[0], pParty->uid[pos]);

	// Swap the seeking party & leader flags
	std::swap(m_bNeedParty, pUser->m_bNeedParty);
	std::swap(m_bPartyLeader, pUser->m_bPartyLeader);

	// Remove our leadership state from the client
	StateChangeServerDirect(6, 0); // remove 'P' symbol from old party leader	
	StateChangeServerDirect(2, m_bNeedParty); // seeking a party

	// Make them leader.
	pUser->StateChangeServerDirect(6, 1); // assign 'P' symbol to new party leader
	pUser->StateChangeServerDirect(2, pUser->m_bNeedParty); // seeking a party

	Packet result(WIZ_PARTY, uint8(PARTY_INSERT));
	result << pUser->GetSocketID()
		<< uint8(100) // reset position to leader
		<< pUser->GetName()
		<< pUser->m_iMaxHp << pUser->m_sHp
		<< pUser->GetLevel() << pUser->GetClass()
		<< pUser->m_iMaxMp << pUser->m_sMp
		<< pUser->GetNation();
	g_pMain->Send_PartyMember(GetPartyID(), &result);

	// Now update the slots in the AI server.
	result.Initialize(AG_USER_PARTY);

	// Shift the ex-leader to the promoted player's slot
	result	<< uint8(PARTY_INSERT) << pParty->wIndex << pos << GetSocketID();
	Send_AIServer(&result);

	// Shift the leader to the ex-leader's slot (0).
	result	<< uint8(PARTY_INSERT) << pParty->wIndex << uint8(0) << pUser->GetSocketID();
	Send_AIServer(&result);
}

void CUser::PartyRemove(int memberid)
{
	if (!isInParty()
		|| GetZoneID() == ZONE_JURAD_MOUNTAIN)  
		return;

	CUser *pUser = g_pMain->GetUserPtr(memberid);
	if (pUser == nullptr)
		return;

	_PARTY_GROUP *pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr) 
	{
		m_bInParty = pUser->m_bInParty = false;
		m_sPartyIndex = pUser->m_sPartyIndex = -1;
		return;
	}

	if (memberid != GetSocketID())
	{		
		if (pParty->uid[0] != GetSocketID()) 
			return;
	}
	else 
	{
		if (pParty->uid[0] == memberid)
		{
			PartyDelete();
			return;
		}
	}

	int count = 0, memberPos = -1;
	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		if (pParty->uid[i] < 0)
			continue;
		else if (pParty->uid[i] == memberid)
		{
			memberPos = i;
			continue;
		}

		count++;
	}

	if (count == 1)
	{
		CUser *pPartyLeader = g_pMain->GetUserPtr(pParty->uid[0]);
		if (pPartyLeader == nullptr)
			return;
		else
			pPartyLeader->PartyDelete();
		return;
	}

	Packet result(WIZ_PARTY, uint8(PARTY_REMOVE));
	result << uint16(memberid);
	g_pMain->Send_PartyMember(m_sPartyIndex, &result);

	if (memberPos >= 0)
	{
		pUser->m_bInParty = false;
		pUser->m_sPartyIndex = pParty->uid[memberPos] = -1;
	}

	// AI Server
	result.Initialize(AG_USER_PARTY);
	result << uint8(PARTY_REMOVE) << pParty->wIndex << uint16(memberid);
	Send_AIServer(&result);
}

void CUser::PartyDelete()
{
	if (!isInParty()
		|| GetZoneID() == ZONE_JURAD_MOUNTAIN)
		return;

	_PARTY_GROUP *pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr)
	{
		m_bInParty = false;
		m_sPartyIndex = -1;
		return;
	}

	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser *pUser = g_pMain->GetUserPtr(pParty->uid[i]);
		if (pUser != nullptr)
		{
			pUser->m_bInParty = false;
			pUser->m_sPartyIndex = -1;
		}
	}

	Packet result(WIZ_PARTY, uint8(PARTY_DELETE));
	g_pMain->Send_PartyMember(pParty->wIndex, &result);
	result.Initialize(AG_USER_PARTY);

	m_bPartyLeader = false;
	StateChangeServerDirect(6, 0); // remove 'P' symbol from party leader

	result << uint8(PARTY_DELETE) << uint16(pParty->wIndex);
	Send_AIServer(&result);
	g_pMain->DeleteParty(pParty->wIndex);
}

// Seeking party system
void CUser::PartyBBS(Packet & pkt)
{
	uint8 opcode = pkt.read<uint8>();
	switch (opcode)
	{
	case PARTY_BBS_REGISTER:
		PartyBBSRegister(pkt);
		break;
	case PARTY_BBS_DELETE:
		PartyBBSDelete(pkt);
		break;
	case PARTY_BBS_NEEDED:
		PartyBBSNeeded(pkt, PARTY_BBS_NEEDED);
		break;
	case PARTY_BBS_WANTED:
		PartyBBSWanted(pkt);
		break;
	case PARTY_BBS_LIST:
		SendPartyBBSNeeded(0, PARTY_BBS_LIST);
		break;
	}
}

void CUser::PartyBBSRegister(Packet & pkt)
{
	int counter = 0;

	if (isInParty() // You are already in a party!
		|| m_bNeedParty == 2) // You are already on the BBS!
	{
		Packet result(WIZ_PARTY_BBS, uint8(PARTY_BBS_REGISTER));
		result << uint8(0);
		Send(&result);
		return;
	}

	StateChangeServerDirect(2, 2); // seeking a party

	// TODO: Make this a more localised map
	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach (itr, sessMap)
	{
		CUser *pUser = TO_USER(itr->second);
		if (pUser->GetNation() != GetNation()
			|| pUser->m_bNeedParty == 1) 
			continue;

		if(!((pUser->GetLevel() <= (int)(GetLevel() * 1.5) && pUser->GetLevel() >= (int)(GetLevel() * 1.5)) 
			|| (pUser->GetLevel() <= (GetLevel() + 8) && pUser->GetLevel() >= ((int)(GetLevel()) - 8))))
			continue;

		if (pUser->GetSocketID() == GetSocketID()) break;
		counter++;
	}

	SendPartyBBSNeeded(counter / MAX_BBS_PAGE, PARTY_BBS_LIST);
}

void CUser::PartyBBSDelete(Packet & pkt)
{
	// You don't need anymore 
	if (m_bNeedParty == 1) 
	{
		Packet result(WIZ_PARTY_BBS, uint8(PARTY_BBS_DELETE));
		result << uint8(0);
		Send(&result);
		return;
	}

	StateChangeServerDirect(2, 1); // not looking for a party
	SendPartyBBSNeeded(0, PARTY_BBS_DELETE);
}

void CUser::PartyBBSNeeded(Packet & pkt, uint8 type)
{
	SendPartyBBSNeeded(pkt.read<uint16>(), type);
}

void CUser::SendPartyBBSNeeded(uint16 page_index, uint8 bType)
{
	Packet result(WIZ_PARTY_BBS);

	uint16 start_counter = 0, BBS_Counter = 0;
	uint8 valid_counter = 0;
	int j = 0;

	start_counter = page_index * MAX_BBS_PAGE;

	if (start_counter >= MAX_USER)
	{
		result << uint8(PARTY_BBS_NEEDED) << uint8(0);
		Send(&result);
		return;
	}

	result << bType << uint8(1) << page_index << uint8(0) << uint8(0); //Not sure what the last 2 bytes are.

	// TODO: Make this a more localised map
	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	int i = -1; // start at -1, first iteration gets us to 0.
	foreach (itr, sessMap)
	{
		CUser *pUser = TO_USER(itr->second);
		_PARTY_GROUP *pParty = nullptr;
		string WantedMessage = "Seeking Party";
		uint8 PartyMembers = 0;
		uint16 sClass = pUser->m_sClass;
		i++;

		if (GetZoneID() != pUser->GetZoneID()
			|| (GetNation() != pUser->GetNation() 
			&& GetZoneID() != ZONE_MORADON
			&& GetZoneID() != ZONE_FORGOTTEN_TEMPLE)
			|| (pUser->m_bNeedParty == 1 && !pUser->m_bPartyLeader)
			|| !((pUser->GetLevel() <= (int)(GetLevel() * 1.5) && pUser->GetLevel() >= (int)(GetLevel() * 2 / 3)) 
			|| (pUser->GetLevel() <= (GetLevel() + 8) && pUser->GetLevel() >= ((int)(GetLevel()) - 8))))
			continue;

		BBS_Counter++;

		if (i < start_counter
			|| valid_counter >= MAX_BBS_PAGE)
			continue;

		if (pUser->m_bPartyLeader)
		{
			pParty = g_pMain->GetPartyPtr(pUser->GetPartyID());
			if (pParty == nullptr) //Shouldn't be hit.
				return;
			WantedMessage = pParty->WantedMessage;
			PartyMembers = GetPartyMemberAmount(pParty);
			sClass = pParty->sWantedClass;
		}

		result.DByte();
		result	<< pUser->GetName()
			<< sClass
			<< uint16(0) << pUser->GetLevel() //Not sure what that uint16 does.
			<< uint8(pUser->m_bPartyLeader ? 3 : 2); //2 is player, 3 is party leader
		result.SByte();
		result	<< WantedMessage
			<< pUser->GetZoneID()
			<< PartyMembers
			<< pUser->GetNation();
		valid_counter++;
	}

	// You still need to fill up ten slots.
	if (valid_counter < MAX_BBS_PAGE)
	{
		for (int j = valid_counter; j < MAX_BBS_PAGE; j++)
			result	<< uint16(0) << uint16(0)
			<< uint16(0) << uint8(0)
			<< uint8(0) << uint8(0)
			<< uint16(0)
			<< uint8(0);
	}

	result << page_index << BBS_Counter;
	Send(&result);
}

void CUser::PartyBBSWanted(Packet & pkt)
{
	uint16 page_index = 0;
	if (!isPartyLeader())
		return;

	_PARTY_GROUP *pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr)
		return;

	pkt >> pParty->sWantedClass >> page_index >> pParty->WantedMessage;
	SendPartyBBSNeeded(page_index, PARTY_BBS_WANTED);
}

uint8 CUser::GetPartyMemberAmount(_PARTY_GROUP *pParty)
{
	if (pParty == nullptr)
		pParty = g_pMain->GetPartyPtr(GetPartyID());

	if (pParty == nullptr)
		return 0;

	uint8 PartyMembers = 0;
	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		if(pParty->uid[i] >= 0)
			PartyMembers++;
	}
	return PartyMembers;
}
