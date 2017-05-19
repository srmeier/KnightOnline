#include "stdafx.h"
#include "Map.h"

void CUser::SelNationToAgent(Packet & pkt)
{
	Packet result(WIZ_SEL_NATION);
	uint8_t nation = pkt.read<uint8_t>();
	if (nation != KARUS && nation != ELMORAD)
	{
		result << uint8_t(0);
		Send(&result);
		return;
	}

	result << nation; 
	g_pMain->AddDatabaseRequest(result, this);
}

void CUser::AllCharInfoToAgent()
{
	Packet result(WIZ_ALLCHAR_INFO_REQ);
	g_pMain->AddDatabaseRequest(result, this);
}

void CUser::ChangeHair(Packet & pkt)
{
	std::string strUserID;
	uint8_t nHair;
	uint8_t bOpcode, bFace;

	// The opcode:
	// 0 seems to be an in-game implementation for converting from old -> new hair data
	// 2 seems to be used with the hair change item(?).

	// Another note: there's 4 bytes at the end of the packet data that I can't account for (maybe a[nother] checksum?)

	pkt.SByte();
	pkt >> bOpcode >> strUserID >> bFace >> nHair;

	Packet result(WIZ_CHANGE_HAIR);
	result.SByte();
	result << bOpcode << strUserID << bFace << nHair;
	g_pMain->AddDatabaseRequest(result, this);
}

void CUser::NewCharToAgent(Packet & pkt)
{
	Packet result(WIZ_NEW_CHAR);
	uint8_t nHair;
	uint16_t sClass;
	uint8_t bCharIndex, bRace, bFace, str, sta, dex, intel, cha, errorCode = 0;
	std::string strUserID;

	pkt	>> bCharIndex >> strUserID >> bRace >> sClass >> bFace >> nHair
		>> str >> sta >> dex >> intel >> cha;

	_CLASS_COEFFICIENT* p_TableCoefficient = g_pMain->m_CoefficientArray.GetData(sClass);

	if (bCharIndex > 2)
		errorCode = NEWCHAR_NO_MORE;
	else if (p_TableCoefficient == nullptr
		|| (str + sta + dex + intel + cha) > 300) 
		errorCode = NEWCHAR_INVALID_DETAILS;
	else if (str < 50 || sta < 50 || dex < 50 || intel < 50 || cha < 50) 
		errorCode = NEWCHAR_STAT_TOO_LOW;

	if (errorCode != 0)
	{
		result << errorCode;
		Send(&result);
		return;
	}

	result	<< bCharIndex 
		<< strUserID << bRace << sClass << bFace << nHair
		<< str << sta << dex << intel << cha;
	g_pMain->AddDatabaseRequest(result, this);
}

void CUser::DelCharToAgent(Packet & pkt)
{
	Packet result(WIZ_DEL_CHAR);
	std::string strUserID, strSocNo;
	uint8_t bCharIndex;
	pkt >> bCharIndex >> strUserID >> strSocNo; 

	if (bCharIndex > 2
		|| strUserID.empty() || strUserID.size() > MAX_ID_SIZE
		|| strSocNo.empty() || strSocNo.size() > 15
		|| isClanLeader())
	{
		result << uint8_t(0) << uint8_t(-1);
		Send(&result);
		return;
	}

	// Process the deletion request in the database
	result	<< bCharIndex << strUserID << strSocNo;
	g_pMain->AddDatabaseRequest(result, this);
}

void CUser::SelCharToAgent(Packet & pkt)
{
	Packet result(WIZ_SEL_CHAR);
	std::string strUserID, strAccountID;
	uint8_t bInit;

	pkt >> strAccountID >> strUserID >> bInit;
	if (strAccountID.empty() || strAccountID.size() > MAX_ID_SIZE
		|| strUserID.empty() || strUserID.size() > MAX_ID_SIZE
		|| strAccountID != m_strAccountID)
	{
		Disconnect();
		return;
	}

	// Disconnect any currently logged in sessions.
	CUser *pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);

	if (pUser && (pUser->GetSocketID() != GetSocketID()))
	{
		pUser->Disconnect();

		// And reject the login attempt (otherwise we'll probably desync char data)
		result << uint8_t(0);
		Send(&result);
		return;
	}

	result << strUserID << bInit;
	g_pMain->AddDatabaseRequest(result, this);
}

void CUser::SelectCharacter(uint8_t bResult, uint8_t bInit)
{
	Packet result(WIZ_SEL_CHAR);

	if (isBanned())
	{
		Disconnect();
		return;
	}

	result << bResult;
	if (bResult == 0 || !GetZoneID()) 
		goto fail_return;

	m_pMap = g_pMain->GetZoneByID(GetZoneID());
	if (GetMap() == nullptr)
		goto fail_return;

	if (g_pMain->m_nServerNo != GetMap()->m_nServerNo)
	{
		_ZONE_SERVERINFO *pInfo = g_pMain->m_ServerArray.GetData(GetMap()->m_nServerNo);
		if (pInfo == nullptr) 
			goto fail_return;

		SendServerChange(pInfo->strServerIP, bInit);
		return;
	}

	if (!g_pMain->isWarOpen() && GetFame() == COMMAND_CAPTAIN)
		m_bFame = CHIEF;

	// Disallow players from relogging in the opposite nation's home zone when an invasion's not running.
	if (((GetZoneID() != GetNation() && GetZoneID() <= ZONE_ELMORAD && !g_pMain->m_byBattleOpen)
		// also disallow players from logging back into war zones that aren't currently active...
			|| (GetMap()->isWarZone() && !g_pMain->m_byBattleOpen)
			// Chaos, bdw and juraid montuain
			|| isInTempleEventZone()
			// forgetten temple
			|| GetZoneID() == ZONE_FORGOTTEN_TEMPLE
			// Ardream, Ronark Land Base, Ronark Land, Bifrost, Krowaz Dominion.
			|| (g_pMain->m_byBattleOpen && (GetZoneID() == ZONE_ARDREAM 
			|| GetZoneID() == ZONE_RONARK_LAND_BASE
			|| GetZoneID() == ZONE_RONARK_LAND 
			|| GetZoneID() == ZONE_BIFROST
			|| GetZoneID() == ZONE_KROWAZ_DOMINION))) && !isGM())
	{
		NativeZoneReturn();
		UserDataSaveToAgent();
		Disconnect();
		return;
	}

	SetLogInInfoToDB(bInit);

	result << GetZoneID() << GetSPosX() << GetSPosZ() << GetSPosY() << g_pMain->m_byOldVictory; //<< 0x20<<0x37<<0x42<<0x30<<0x38<<0x43<<0x46<<0x44<<0x37<<0x45<<0x44<<0x37<<0x34<<0x34<<0x37<<0x33<<0x39<<0x39<<0x31<<0x35<<0x36<<0x31<<0x30<<0x41<<0x39<<0x39<<0x34<<0x37<<0x32<<0x35<<0x41<<0x43<<0x46;
	m_bSelectedCharacter = true;
	Send(&result);

	SetUserAbility(false);

	if (GetLevel() > MAX_LEVEL) 
	{
		Disconnect();
		return;
	}

	m_iMaxExp = g_pMain->GetExpByLevel(GetLevel());
	SetRegion(GetNewRegionX(), GetNewRegionZ());

	if (GetClanID() == -1)
	{
		SetClanID(0);
		m_bFame = 0;
		return;
	}
	else if (GetClanID() != 0
		&& GetZoneID() > 2)
	{
		result.Initialize(WIZ_KNIGHTS_PROCESS);
		result << uint8_t(KNIGHTS_LIST_REQ) << GetClanID();
		g_pMain->AddDatabaseRequest(result, this);
	}
	return;

fail_return:
	Send(&result);
}

void CUser::SendServerChange(std::string & ip, uint8_t bInit)
{
	Packet result(WIZ_SERVER_CHANGE);
	result << ip << uint16_t(g_pMain->m_GameServerPort) << bInit << GetZoneID() << g_pMain->m_byOldVictory;
	Send(&result);
}

// happens on character selection
void CUser::SetLogInInfoToDB(uint8_t bInit)
{
	_ZONE_SERVERINFO *pInfo = g_pMain->m_ServerArray.GetData(g_pMain->m_nServerNo);
	if (pInfo == nullptr) 
	{
		Disconnect();
		return;
	}

	Packet result(WIZ_LOGIN_INFO);
	result	<< GetName() 
		<< pInfo->strServerIP << uint16_t(g_pMain->m_GameServerPort) << GetRemoteIP() 
		<< bInit;
	g_pMain->AddDatabaseRequest(result, this);
}

void CUser::RecvLoginInfo(Packet & pkt)
{
	int8_t bResult = pkt.read<uint8_t>();
	if (bResult < 0)
		Disconnect();
}

// This packet actually contains the char name after the opcode
void CUser::GameStart(Packet & pkt)
{
	if (isInGame())
		return;

	uint8_t opcode = pkt.read<uint8_t>();

	if (opcode == 1)
	{
		SendMyInfo();
		g_pMain->UserInOutForMe(this);
		g_pMain->MerchantUserInOutForMe(this);
		g_pMain->NpcInOutForMe(this);
		SendNotice();
		SendTime();
		SendWeather();

		//SendHackToolList();

		Packet result(WIZ_GAMESTART);
		Send(&result);
	}
	else if (opcode == 2)
	{
		m_state = GAME_STATE_INGAME;
		UserInOut(INOUT_RESPAWN);

		if (!m_bCity && m_sHp <= 0)
			m_bCity = -1;

		if (m_bCity > 0)
		{
			int level = GetLevel();
			if (m_bCity <= 100)
				level--;

			// make sure we don't exceed bounds
			if (level > MAX_LEVEL)
				level = MAX_LEVEL;
			else if (level < 1)
				level = 1;

			m_iLostExp = (g_pMain->GetExpByLevel(level) * (m_bCity % 10) / 100);
			if (((m_bCity % 10) / 100) == 1)
				m_iLostExp /= 2;
		}
		else
		{
			m_iLostExp = 0;
		}

		BlinkStart();
		SetUserAbility();
		// rental
		//ItemMallRecast

		// If we've relogged while dead, we need to make sure the client 
		// is still given the option to revive.
		if (isDead())
			SendDeathAnimation();

		g_pMain->TempleEventGetActiveEventTime(this);

		m_tGameStartTimeSavedMagic = UNIXTIME;

		m_LastX = GetX();
		m_LastZ = GetZ();
	}
	
	CKnights *pKnights = nullptr;
	_KNIGHTS_SIEGE_WARFARE *pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(1);

	if(pKnightSiege != nullptr) pKnights = g_pMain->GetClanPtr(pKnightSiege->sMasterKnights);

	if (GetZoneID() == ZONE_DELOS && pKnights != nullptr)
	{
		Packet result(WIZ_SIEGE, uint8_t(2));
		result << pKnights->GetID() << pKnights->m_sMarkVersion;
		Send(&result);	
	}

	m_tHPLastTimeNormal = UNIXTIME;
}
