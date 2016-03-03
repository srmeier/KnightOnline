#ifndef __IOCSOCKET_H_
#define __IOCSOCKET_H_

#include "Define.h" //#include "DefineTest.h"

//-----------------------------------------------------------------------------
class CIOCSocket {
protected:
	int m_Sid;
	Uint8 m_State;

public:
	TCPsocket m_tcpSocket;
	SDL_mutex* m_critData;
	class CEbenezerDlg* m_pServer;

public:
	Uint8 GetState(void) {return m_State;}

	int GetSocketID(void) {return m_Sid;}
	void SetSocketID(int sid) {m_Sid = sid;}

	void InitSocket(class CEbenezerDlg* pServer, TCPsocket socket);

	int Send(char* pBuf, uint16_t length);
	void SendCompressingPacket(const char* pData, int len);
	uint16_t ReceivedData(uint8_t* pBuf, int length);
	virtual void Initialize(void);
	virtual void Parsing(int len, char* pData) = 0;

public:
	CIOCSocket(void);
	virtual ~CIOCSocket(void);
};

#endif
