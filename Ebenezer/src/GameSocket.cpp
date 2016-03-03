/*
*/

#include "stdafx.h"
//#include "Server.h"
#include "GameSocket.h"

//-----------------------------------------------------------------------------
void CGameSocket::Initialize(void) {
	m_sSocketID = -1;
}

//-----------------------------------------------------------------------------
void CGameSocket::Parsing(int len, char* pData) {
	/*
	int index = 0;
	uint8_t bType = GetByte(pData, index);

	switch(bType) {
		case SERVER_CONNECT:
			RecvServerConnect(pData+index);
			break;
	}
	*/
}

//-----------------------------------------------------------------------------
void CGameSocket::RecvServerConnect(uint8_t* pBuf) {
	/*
	int index = 0; // NOTE: not 1 because I'm offseting the pointer when it gets passed
	
	uint8_t byZoneNumber = GetByte(pBuf, index);
	uint8_t byReConnect = GetByte(pBuf, index);

	printf("Got SERVER_CONNECT packet zone:%d, reconnect:%d\n", byZoneNumber, byReConnect);

	int out_index = 0;

	uint8_t pData[1024];
	memset(pData, 0x00, 1024);

	if(byZoneNumber < 0) {
		SetByte(pData, SERVER_CONNECT, out_index);
		SetByte(pData, -1, out_index);

		Send(pData, out_index);

		// TODO: isn't this an error? like we shouldn't continue... or even just ignore it?
		return;
	}

	m_sSocketID = byZoneNumber;

	SetByte(pData, SERVER_CONNECT, out_index);
	SetByte(pData, byZoneNumber, out_index);
	SetByte(pData, byReConnect, out_index);

	Send(pData, out_index);

	if(byReConnect) {
		// TODO: implement this
	} else {
		m_pServer->m_sSocketCount++;
		if(m_pServer->m_sSocketCount == MAX_AI_SOCKET) {
			m_pServer->m_bFirstServerFlag = true;
			m_pServer->m_sSocketCount = 0;
			m_pServer->AllNpcInfo();
		}
	}
	*/
}
