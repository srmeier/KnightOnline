#ifndef __IOCPORT_H_
#define __IOCPORT_H_

#include <list>
#include <queue>
#include "Define.h"//#include "DefineTest.h"
//#include "EbenezerDlg.h"

//-----------------------------------------------------------------------------
typedef std::list<int> SidList;
typedef std::queue<int> SidQueue;
typedef std::list<SEND_DATA*> SendDataList;

//-----------------------------------------------------------------------------
class CIOCPort {
protected:
	SDL_Thread* m_pSendThread;
	SDL_Thread* m_pAcceptThread;

	unsigned long m_dwNumberOfWorkers;
	SDL_Thread* m_ppWorkerThreads[MAX_USER];

public:
	class CEbenezerDlg* m_pServer;

	int m_AiSendSocket;
	int m_SocketArraySize;

	SDL_mutex* m_critSendData;
	class CIOCSocket** m_SockArray;
	class CIOCSocket** m_SockArrayInActive;

	SidList m_SidList;

	SDL_sem* m_pSendSem;
	SDL_sem* m_pWorkerSem;

	TCPsocket m_ListenSocket;

	SidQueue m_SidReceivedQueue;
	SendDataList m_SendDataList;

	SDL_atomic_t m_aCloseWorkers;
	SDL_atomic_t m_aCloseSendThread;
	SDL_atomic_t m_aCloseAcceptThread;

public:
	void CreateReceiveWorkerThread(int workernum);
	bool Init(class CEbenezerDlg* pServer, int serversocksize, int workernum = 0);
	bool Listen(int port);
	void CreateAcceptThread(void);
	void CreateSendThread(void);
	int GetNewSid(void);
	CIOCSocket* GetIOCSocket(int index);

public:
	CIOCPort(void);
	virtual ~CIOCPort(void);
};

#endif
