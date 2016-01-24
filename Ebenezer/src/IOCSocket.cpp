/*
*/

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
void CIOCSocket::ReceivedData(uint8_t* pBuf, int length) {
	// TODO: maybe we can move the mutex from the worker thread and into this
	// function?

	if(length <= 6) return;

	if(pBuf[0]==PACKET_START1 && pBuf[1]==PACKET_START2 &&
		pBuf[length-2]==PACKET_END1 && pBuf[length-1]==PACKET_END2
	) {
		uint16_t sSize = *(uint16_t*) &pBuf[2];
		printf("Got packet of size %d\n", (int) sSize);

		Parsing(length-6, &pBuf[4]);
	}
}

//-----------------------------------------------------------------------------
void CIOCSocket::Parsing(int length, uint8_t* pData) {}

//-----------------------------------------------------------------------------
int CIOCSocket::Send(uint8_t* pBuf, uint16_t length) {
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
	int size = SDLNet_TCP_Send(m_tcpSocket, pTBuf, index);
	SDL_UnlockMutex(m_critData);

	return size;
}
