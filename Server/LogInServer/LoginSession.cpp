#include "stdafx.h"
#include "../shared/DateTime.h"

LSPacketHandler PacketHandlers[NUM_LS_OPCODES];

void InitPacketHandlers()
{
	memset(&PacketHandlers, 0, sizeof(LSPacketHandler) * NUM_LS_OPCODES);
	PacketHandlers[LS_VERSION_REQ]			= &LoginSession::HandleVersion;
	PacketHandlers[LS_DOWNLOADINFO_REQ]		= &LoginSession::HandlePatches;
	PacketHandlers[LS_LOGIN_REQ]			= &LoginSession::HandleLogin;
	PacketHandlers[LS_SERVERLIST]			= &LoginSession::HandleServerlist;
	PacketHandlers[LS_NEWS]					= &LoginSession::HandleNews;
}

LoginSession::LoginSession(
	uint16_t socketID,
	SocketMgr* mgr)
	: KOSocket(
	socketID,
	mgr,
	-1,
	2048,
	64)
{
}

bool LoginSession::HandlePacket(
	Packet& pkt)
{
	uint8_t opcode;
	pkt >> opcode;

	// Unknown packet
	if (opcode >= NUM_LS_OPCODES
		|| PacketHandlers[opcode] == nullptr)
		return false;

	(this->*PacketHandlers[opcode])(pkt);
	return true;
}

void LoginSession::HandleVersion(
	Packet& pkt)
{
	Packet result(LS_VERSION_REQ);
	result << uint16_t(g_pMain->GetVersion());
	Send(&result);
}

void LoginSession::HandlePatches(
	Packet& pkt)
{
	// The launcher officially only supports up to 31 per request.
	constexpr size_t MAX_PATCHES_PER_REQUEST = 31;

	std::set<std::string> patchSet;
	uint16_t version;
	pkt >> version;

	const auto& patchList = g_pMain->GetPatchList();
	for (auto itr = patchList->begin();
		itr != patchList->end();
		++itr)
	{
		_VERSION_INFO* pInfo = itr->second;
		if (pInfo->sVersion <= version)
			continue;

		patchSet.insert(pInfo->strFilename);

		if (patchSet.size() >= MAX_PATCHES_PER_REQUEST)
			break;
	}

	Packet result(LS_DOWNLOADINFO_REQ);
	result
		<< g_pMain->GetFTPUrl()
		<< g_pMain->GetFTPPath()
		<< uint16_t(patchSet.size());

	for (const std::string& filename : patchSet)
		result << filename;
	Send(&result);
}

void LoginSession::HandleLogin(
	Packet& pkt)
{
	Packet result(pkt.GetOpcode());
	e_AuthResult resultCode = AUTH_FAILED;
	std::string account, password, serverIP;
	DateTime time;
	uint16_t serverPortNo = 0;

	pkt
		>> account
		>> password;

	if (account.empty()
		|| account.size() > MAX_ID_SIZE
		|| password.empty()
		|| password.size() > MAX_PW_SIZE)
		resultCode = AUTH_NOT_FOUND;
	else
		resultCode = (e_AuthResult) g_pMain->m_DBProcess.AccountLogin(account, password);

	if (resultCode == AUTH_SUCCESS
		&& g_pMain->m_DBProcess.IsAccountLoggedIn(
		account,
		&serverPortNo,
		&serverIP))
		resultCode = AUTH_IN_GAME;

	result << uint8_t(resultCode);

	if (resultCode == AUTH_SUCCESS)
	{
		result
			<< int16_t(g_pMain->m_DBProcess.AccountPremium(account))
			<< account;
	}
	// User is in-game, we must supply this information back to then.
	else if (resultCode == AUTH_IN_GAME)
	{
		result
			<< serverIP
			<< uint16_t(serverPortNo);
	}

	Send(&result);

	std::string resultName = GetAuthResultName(resultCode);
	std::string szLogMessage = string_format(
		"[ LOGIN - %d:%d:%d ] ID=%s Authentication=%s\n",
		time.GetHour(),
		time.GetMinute(),
		time.GetSecond(),
		account.c_str(),
		password.c_str(),
		resultName.c_str());
	printf("%s", szLogMessage.c_str());
	g_pMain->WriteUserLogFile(szLogMessage);
}

void LoginSession::HandleServerlist(
	Packet& pkt)
{
	Packet result(LS_SERVERLIST);
	g_pMain->GetServerList(result);
	Send(&result);
}

void LoginSession::HandleNews(
	Packet& pkt)
{
	Packet result(pkt.GetOpcode());
	News* pNews = g_pMain->GetNews();

	if (pNews->Size)
	{
		result << "Login Notice" << uint16_t(pNews->Size);
		result.append(pNews->Content, pNews->Size);
	}
	else // dummy news, will skip past it
	{
		result << "Login Notice" << "<empty>";
	}

	Send(&result);
}

std::string LoginSession::GetAuthResultName(
	e_AuthResult eAuthResult)
{
	switch (eAuthResult)
	{
		case AUTH_SUCCESS:
			return "SUCCESS";

		case AUTH_NOT_FOUND:
			return "NOT FOUND";

		case AUTH_INVALID_PW:
			return "INVALID PW";

		case AUTH_BANNED:
			return "BANNED";

		case AUTH_IN_GAME:
			return "IN GAME";

		case AUTH_ERROR:
			return "ERROR";

		case AUTH_FAILED:
			return "FAILED";

		default:
			return string_format(
				"UNKNOWN (%d)",
				eAuthResult);
	}
}
