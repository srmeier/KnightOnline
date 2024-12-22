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
	enum LoginErrorCode
	{
		AUTH_SUCCESS		= 0x01,
		AUTH_NOT_FOUND		= 0x02,
		AUTH_INVALID		= 0x03,
		AUTH_BANNED			= 0x04,
		AUTH_IN_GAME		= 0x05,
		AUTH_ERROR			= 0x06,
		AUTH_FAILED			= 0xFF
	};

	Packet result(pkt.GetOpcode());
	uint16_t resultCode = 0;
	std::string account, password;
	DateTime time;

	pkt >> account >> password;
	if (account.empty()
		|| account.size() > MAX_ID_SIZE
		|| password.empty()
		|| password.size() > MAX_PW_SIZE)
		resultCode = AUTH_NOT_FOUND;
	else
		resultCode = g_pMain->m_DBProcess.AccountLogin(account, password);

	std::string sAuthMessage;

	switch (resultCode)
	{
		case AUTH_SUCCESS:
			sAuthMessage = "SUCCESS";
			break;
		case AUTH_NOT_FOUND:
			sAuthMessage = "NOT FOUND";
			break;
		case AUTH_INVALID:
			sAuthMessage = "INVALID";
			break;
		case AUTH_BANNED:
			sAuthMessage = "BANNED";
			break;
		case AUTH_IN_GAME:
			sAuthMessage = "IN GAME";
			break;
		case AUTH_ERROR:
			sAuthMessage = "ERROR";
			break;
		case AUTH_FAILED:
			sAuthMessage = "FAILED";
			break;
		default:
			sAuthMessage = string_format("UNKNOWN (%d)", resultCode);
			break;
	}

	printf("[ LOGIN - %d:%d:%d ] ID=%s Authentication=%s\n",
		time.GetHour(), time.GetMinute(), time.GetSecond(),
		account.c_str(), sAuthMessage.c_str());

	result << uint8_t(resultCode);
	if (resultCode == AUTH_SUCCESS)
	{
		result << g_pMain->m_DBProcess.AccountPremium(account);
		result << account;
	}

	g_pMain->WriteUserLogFile(string_format("[ LOGIN - %d:%d:%d ] ID=%s Authentication=%s\n", time.GetHour(), time.GetMinute(), time.GetSecond(), account.c_str(), password.c_str(), sAuthMessage.c_str()));

	Send(&result);
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
