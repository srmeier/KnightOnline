#include "stdafx.h"

void CUser::VersionCheck(Packet & pkt)
{
	Packet result(WIZ_VERSION_CHECK);
	/*
	string strAccountID;
	int16_t unk = pkt.read<int16_t>(); // -1
	pkt >> strAccountID;
	if (strAccountID.empty() || strAccountID.size() > MAX_ID_SIZE)
	return;
	*/

#if __VERSION <= 1264
	result << uint16_t(__VERSION);
#else
	result << uint16_t(__VERSION) << m_crypto.GenerateKey(); // 0 = success, 1 = prem error
#endif
	Send(&result);

	// Enable encryption
#if __VERSION > 1264
	EnableCrypto();
#endif
}

void CUser::LoginProcess(Packet & pkt)
{
	// Enforce only one login request per session
	// It's common for players to spam this at the server list when a response isn't received immediately.
	if (!m_strAccountID.empty())
		return;

	CUser* pUser = nullptr;

	Packet result(WIZ_LOGIN);
	std::string strAccountID, strPasswd;
	pkt >> strAccountID >> strPasswd;

	if (strAccountID.empty()
		|| strAccountID.size() > MAX_ID_SIZE
		|| strPasswd.empty()
		|| strPasswd.size() > MAX_PW_SIZE)
		goto fail_return;

	pUser = g_pMain->GetUserPtr(strAccountID, TYPE_ACCOUNT);

	if (!WordGuardSystem(strAccountID, strAccountID.size()))
		goto fail_return;

	if (pUser != nullptr
		&& pUser->GetSocketID() != GetSocketID())
	{
		pUser->Disconnect();
		goto fail_return;
	}

	result << strPasswd;
	m_strAccountID = strAccountID;
	g_pMain->AddDatabaseRequest(result, this);
	return;

fail_return:
	result << uint8_t(-1);
	Send(&result);
}

bool CUser::WordGuardSystem(std::string Word, uint8_t WordStr)
{
	char *pword = &Word[0];
	bool bGuard[32] = {false};
	std::string WordGuard = "qwertyuopasdfghjklizxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890";
	char *pWordGuard = &WordGuard[0];
		for(uint8_t i=0; i < WordStr; i++)
		{
			for(uint8_t j=0; j < strlen(pWordGuard); j++)
				if (pword[i] == pWordGuard[j])
					bGuard[i] = true;

			if (bGuard[i] == false)
				return false;
		}
	return true;
}
