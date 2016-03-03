/*
*/

#include "stdafx.h"
#include "IOCSocket.h"
#include "EbenezerDlg.h"

//-----------------------------------------------------------------------------
CIOCSocket::CIOCSocket(void) {
	m_Sid = -1;
	m_tcpSocket = NULL;
	m_critData = SDL_CreateMutex();
}

//-----------------------------------------------------------------------------
CIOCSocket::~CIOCSocket(void) {
	SDL_DestroyMutex(m_critData);
}

//-----------------------------------------------------------------------------
void CIOCSocket::InitSocket(CEbenezerDlg* pServer, TCPsocket socket) {
	m_pServer = pServer;
	m_tcpSocket = socket;

	Initialize();
}

//-----------------------------------------------------------------------------
void CIOCSocket::Initialize(void) {}

//-----------------------------------------------------------------------------
uint16_t CIOCSocket::ReceivedData(uint8_t* pBuf, int length) {
	// TODO: maybe we can move the mutex from the worker thread and into this
	// function?

	// TODO: this needs work because the SDL network functions return buffers
	// with multiple packets in them... and we might not be able to use the
	// SDLNet stuff anyways because of the multithread stuff

	if(length <= 6) return length;

	uint16_t sSize;
	if(pBuf[0]==PACKET_START1 && pBuf[1]==PACKET_START2 &&
		pBuf[length-2]==PACKET_END1 && pBuf[length-1]==PACKET_END2
	) {
		sSize = *(uint16_t*) &pBuf[2];
		printf("Got packet of size %d\n", (int) sSize);

		//Parsing(length-6, &pBuf[4]);
		Parsing(sSize, (char*) &pBuf[4]);
	}

	return (sSize+6);
}

//-----------------------------------------------------------------------------
//void CIOCSocket::Parsing(int len, char* pData) {}

//-----------------------------------------------------------------------------
int CIOCSocket::Send(char* pBuf, uint16_t length) {
	if(length > MAX_PACKET_SIZE) return 0;

	uint8_t pTBuf[MAX_PACKET_SIZE];
	memset(pTBuf, 0x00, MAX_PACKET_SIZE);

	int index = 0;

	pTBuf[index++] = (uint8_t) PACKET_START1;
	pTBuf[index++] = (uint8_t) PACKET_START2;
	memcpy(pTBuf+index, &length, sizeof(uint16_t));
	index += sizeof(uint16_t);
	memcpy(pTBuf+index, pBuf, length);
	index += length;
	pTBuf[index++] = (uint8_t) PACKET_END1;
	pTBuf[index++] = (uint8_t) PACKET_END2;

	// TODO: add a try mutex since the send thread will just move to the next
	// free socket
	SDL_LockMutex(m_critData);
	if(m_tcpSocket == NULL) {
		fprintf(stderr, "Failed to send before socket was closed!\n");
		return 0x00;
	}

	int size = SDLNet_TCP_Send(m_tcpSocket, pTBuf, index);
	SDL_UnlockMutex(m_critData);

	return size;
}

//-----------------------------------------------------------------------------
void CIOCSocket::SendCompressingPacket(const char* pData, int len) {
	Send((char*)pData, len);
}
