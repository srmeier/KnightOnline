#ifndef __GAMESOCKET_H_
#define __GAMESOCKET_H_

#include "Define.h" //#include "DefineTest.h"
#include "IOCSocket.h"

//-----------------------------------------------------------------------------
class CGameSocket: public CIOCSocket {
public:
	uint16_t m_sSocketID;

public:
	void Initialize(void);
	virtual void Parsing(int len, char* pData);
	void RecvServerConnect(uint8_t* pBuf);
};

#endif
