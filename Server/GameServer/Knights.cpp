#include "stdafx.h"
#include "Knights.h"

CKnights::CKnights()
{
	m_sIndex = 0;
	m_byFlag = ClanTypeNone;
	m_byNation = 0;
	m_byGrade = 5;
	m_byRanking = 0;
	m_sMembers = 1;
	memset(&m_Image, 0, sizeof(m_Image));
	m_nMoney = 0;
	m_sDomination = 0;
	m_nPoints = 0;
	m_sCape = -1;
	m_sAlliance = 0;
	m_sMarkLen = 0;
	m_sMarkVersion = 0;
}

void CKnights::OnLogin(
	CUser* pUser)
{
	Packet result;

	// Set the active session for this user
	foreach_array(i, m_arKnightsUser)
	{
		_KNIGHTS_USER* p = &m_arKnightsUser[i];
		if (!p->byUsed
			|| STRCASECMP(p->strUserName.c_str(), pUser->GetName().c_str()) != 0)
			continue;

		p->pSession = pUser;
		pUser->m_pKnightsUser = p;
		break;
	}

	// Send login notice
	// TODO: Shift this to SERVER_RESOURCE
	std::string buffer = string_format("%s is online.", pUser->GetName().c_str());
	ChatPacket::Construct(&result, KNIGHTS_CHAT, &buffer);
	Send(&result);
}

void CKnights::OnLoginAlliance(
	CUser* pUser)
{
	Packet result;

	// Set the active session for this user
	foreach_array(i, m_arKnightsUser)
	{
		_KNIGHTS_USER* p = &m_arKnightsUser[i];
		if (!p->byUsed
			|| STRCASECMP(p->strUserName.c_str(), pUser->GetName().c_str()) != 0)
			continue;

		p->pSession = pUser;
		pUser->m_pKnightsUser = p;
		break;
	}

	// Send login notice
	// TODO: Shift this to SERVER_RESOURCE
	std::string buffer = string_format("%s is online.", pUser->GetName().c_str());
	ChatPacket::Construct(&result, ALLIANCE_CHAT, &buffer);
	CKnights* pKnights = g_pMain->GetClanPtr(pUser->m_bKnights);
	g_pMain->Send_KnightsAlliance(pKnights->GetAllianceID(), &result);
}

void CKnights::OnLogout(
	CUser* pUser)
{
	// Unset the active session for this user
	if (pUser->m_pKnightsUser != nullptr)
	{
		pUser->m_pKnightsUser->pSession = nullptr;
		pUser->m_pKnightsUser = nullptr;
	}

	Packet logoutNotice;
	// TODO: Shift this to SERVER_RESOURCE
	std::string buffer = string_format("%s is offline.", pUser->GetName().c_str());
	ChatPacket::Construct(&logoutNotice, KNIGHTS_CHAT, &buffer);
	Send(&logoutNotice);
}

void CKnights::OnLogoutAlliance(
	CUser* pUser)
{
	// Unset the active session for this user
	if (pUser->m_pKnightsUser != nullptr)
	{
		pUser->m_pKnightsUser->pSession = nullptr;
		pUser->m_pKnightsUser = nullptr;
	}

	Packet logoutNotice;
	// TODO: Shift this to SERVER_RESOURCE
	std::string buffer = string_format("%s is offline.", pUser->GetName().c_str());
	ChatPacket::Construct(&logoutNotice, ALLIANCE_CHAT, &buffer);
	CKnights* pKnights = g_pMain->GetClanPtr(pUser->m_bKnights);
	g_pMain->Send_KnightsAlliance(pKnights->GetAllianceID(), &logoutNotice);
}

bool CKnights::AddUser(
	const std::string& strUserID)
{
	for (int i = 0; i < MAX_CLAN_USERS; i++)
	{
		_KNIGHTS_USER* p = &m_arKnightsUser[i];
		if (p->byUsed)
			continue;

		p->byUsed = 1;
		p->strUserName = strUserID;
		p->pSession = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
		if (p->pSession != nullptr)
			p->pSession->m_pKnightsUser = p;

		return true;
	}

	return false;
}

bool CKnights::AddUser(
	CUser* pUser)
{
	if (pUser == nullptr
		|| !AddUser(pUser->GetName()))
		return false;

	pUser->SetClanID(m_sIndex);
	pUser->m_bFame = TRAINEE;
	return true;
}

/**
* @brief	Removes the specified user from the clan array.
*
* @param	strUserID	Identifier for the user.
*
* @return	.
*/
bool CKnights::RemoveUser(
	const std::string& strUserID)
{
	for (int i = 0; i < MAX_CLAN_USERS; i++)
	{
		_KNIGHTS_USER* p = &m_arKnightsUser[i];
		if (p->byUsed == 0)
			continue;

		if (STRCASECMP(p->strUserName.c_str(), strUserID.c_str()) == 0)
		{
			p->Initialise();
			return true;
		}
	}

	return false;
}

/**
* @brief	Removes the specified user from this clan.
*
* @param	pUser	The user.
*/
bool CKnights::RemoveUser(
	CUser* pUser)
{
	if (pUser == nullptr
		|| pUser->m_pKnightsUser == nullptr)
		return false;

	pUser->SetClanID(0);
	pUser->m_bFame = 0;

	pUser->m_pKnightsUser->Initialise();
	pUser->m_pKnightsUser = nullptr;

	if (!pUser->isClanLeader())
		pUser->SendClanUserStatusUpdate();

	return true;
}

void CKnights::Disband(
	CUser* pLeader /*= nullptr*/)
{
	std::string clanNotice;
	g_pMain->GetServerResource(m_byFlag == ClanTypeTraining ? IDS_CLAN_DESTROY : IDS_KNIGHTS_DESTROY,
		&clanNotice, m_strName.c_str());
	SendChat(clanNotice.c_str());

	foreach_array(i, m_arKnightsUser)
	{
		_KNIGHTS_USER* p = &m_arKnightsUser[i];
		if (!p->byUsed)
			continue;

		// If the user's logged in, handle the player data removal in-game.
		// It will be saved to the database when they log out.
		if (p->pSession != nullptr)
			RemoveUser(p->pSession);
		else
			RemoveUser(p->strUserName);

	}
	g_pMain->m_KnightsArray.DeleteData(m_sIndex);

	Packet result(WIZ_KNIGHTS_PROCESS);
	result << uint8_t(KNIGHTS_DESTROY) << uint8_t(1);
	pLeader->Send(&result);
}

void CKnights::SendUpdate()
{
	CKnights* aKnights = g_pMain->GetClanPtr(GetID());

	Packet result(WIZ_KNIGHTS_PROCESS);
	result << uint8_t(KNIGHTS_UPDATE) << GetID() << m_byFlag << GetCapeID(aKnights);
	Send(&result);
}

void CKnights::SendChat(
	const char* format,
	...)
{
	char buffer[128];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, 128, format, ap);
	va_end(ap);

	Packet result;
	ChatPacket::Construct(&result, KNIGHTS_CHAT, buffer);
	Send(&result);
}

void CKnights::SendChatAlliance(
	const char* format,
	...)
{
	char buffer[128];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buffer, 128, format, ap);
	va_end(ap);

	Packet result;
	ChatPacket::Construct(&result, ALLIANCE_CHAT, buffer);
	Send(&result);
}

void CKnights::Send(
	Packet* pkt)
{
	foreach_array(i, m_arKnightsUser)
	{
		_KNIGHTS_USER* p = &m_arKnightsUser[i];
		if (p->byUsed && p->pSession != nullptr)
			p->pSession->Send(pkt);
	}
}

CKnights::~CKnights()
{
}
