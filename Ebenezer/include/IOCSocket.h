#ifndef __IOCSOCKET_H_
#define __IOCSOCKET_H_

#include "Define.h"

//-----------------------------------------------------------------------------
class CIOCSocket {
protected:
	int m_Sid;

public:
	TCPsocket m_tcpSocket;
	SDL_mutex* m_critData;
	class CEbenezerDlg* m_pServer;

public:
	int GetSocketID(void) {return m_Sid;}
	void SetSocketID(int sid) {m_Sid = sid;}

	void InitSocket(class CEbenezerDlg* pServer, TCPsocket socket);

	int Send(uint8_t* pBuf, uint16_t length);
	void ReceivedData(uint8_t* pBuf, int length);
	virtual void Initialize(void);
	virtual void Parsing(int length, uint8_t* pData);

public:
	CIOCSocket(void);
	virtual ~CIOCSocket(void);
};

#endif
