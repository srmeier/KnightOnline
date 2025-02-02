﻿#include "stdafx.h"

#define MAX_FRIEND_COUNT	24

// From the client
void CUser::FriendProcess(Packet & pkt)
{
	uint8_t opcode = pkt.read<uint8_t>();
	switch (opcode)
	{
	case FRIEND_REQUEST:
		FriendRequest();
		break;
	case FRIEND_REPORT:
		FriendReport(pkt);
		break;
	case FRIEND_ADD:
	case FRIEND_REMOVE:
		FriendModify(pkt, opcode);
		break;
	}
}

// Request friend list.
void CUser::FriendRequest()
{
	Packet result(WIZ_FRIEND_PROCESS);
	result << uint8_t(FRIEND_REQUEST);
	g_pMain->AddDatabaseRequest(result, this);
}

// Add or remove a friend from your list.
void CUser::FriendModify(Packet & pkt, uint8_t opcode)
{
	std::string strUserID;
	CUser *pUser;
	pkt >> strUserID;

	if (strUserID.empty() || strUserID.size() > MAX_ID_SIZE 
		|| (opcode == FRIEND_ADD && (pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER)) == nullptr))
		return;

	Packet result(WIZ_FRIEND_PROCESS, opcode);
	if (opcode == FRIEND_ADD)
		result << pUser->GetSocketID();

	result.SByte();
	result << strUserID;
	g_pMain->AddDatabaseRequest(result, this);
}

// Refresh the status of your friends.
void CUser::FriendReport(Packet & pkt)
{
	uint16_t usercount = pkt.read<uint16_t>();

	if (usercount > MAX_FRIEND_COUNT) 
		return;

	Packet result(WIZ_FRIEND_PROCESS);
	result << uint8_t(FRIEND_REPORT) << usercount;
	for (int i = 0; i < usercount; i++) 
	{
		std::string strUserID;
		int16_t sid;

		pkt >> strUserID;
		if (strUserID.empty() || strUserID.size() > MAX_ID_SIZE)
			return; // malformed packet, just ignore it.

		uint8_t status = GetFriendStatus(strUserID, sid);
		result << strUserID << sid << status;
	}

	Send(&result);
}

// Retrieves the status (and socket ID) of a character.
uint8_t CUser::GetFriendStatus(std::string & charName, int16_t & sid)
{
	CUser *pUser;
	if (charName.empty()
		|| (pUser = g_pMain->GetUserPtr(charName, TYPE_CHARACTER)) == nullptr)
	{
		sid = -1;
		return 0; // user not found
	}

	sid = pUser->GetSocketID();
	if (pUser->isInParty()) // user in party
		return 3;

	return 1; // user not in party
}

void CUser::RecvFriendModify(Packet & pkt, uint8_t opcode)
{
	Packet result(WIZ_FRIEND_PROCESS);
	std::string strUserID;
	int16_t sid = -1;
	uint8_t bResult = 0;

	if (opcode == FRIEND_ADD)
		pkt >> sid;

	pkt >> bResult;

	pkt.SByte();
	pkt >> strUserID;

	uint8_t status = GetFriendStatus(strUserID, sid);
	result << opcode << bResult << strUserID << sid << status;
	Send(&result);
}