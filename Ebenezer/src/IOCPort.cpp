/*
*/

#include "IOCPort.h"
#include "IOCSocket.h"
#include "GameSocket.h"

//-----------------------------------------------------------------------------
SDL_mutex* g_critical;

//-----------------------------------------------------------------------------
int AcceptThread(void* pData) {
	// NOTE: this thread waits for incoming connects and hands them out to the
	// worker threads

	CIOCPort* pIocport = (CIOCPort*) pData;

	int iSid = -1;
	TCPsocket tcpSocket = {};
	CIOCSocket* pIocsocket = NULL;

	// NOTE: within the AcceptThread we only wait on the server socket
	SDLNet_SocketSet SocketSet = SDLNet_AllocSocketSet(1);
	if(!SocketSet) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDLNet_GetError());
		return -1;
	}

	if(SDLNet_TCP_AddSocket(SocketSet, pIocport->m_ListenSocket) == -1) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDLNet_GetError());
		SDLNet_FreeSocketSet(SocketSet);
		return -1;
	}

	while(!SDL_AtomicGet(&pIocport->m_aCloseAcceptThread)) {
		// NOTE: every so often check to see if this thread should exit
		if(SDLNet_CheckSockets(SocketSet, 500)>0) {

			SDL_LockMutex(g_critical);

			if(SDLNet_SocketReady(pIocport->m_ListenSocket)) {
				tcpSocket = SDLNet_TCP_Accept(pIocport->m_ListenSocket);

				if(tcpSocket) {

					iSid = pIocport->GetNewSid();
					if(iSid == -1) continue;

					printf("\nSocket %d has connected - got Sid %d\n", tcpSocket, iSid);

					pIocsocket = pIocport->GetIOCSocket(iSid);
					if(!pIocsocket) {
						SDLNet_FreeSocketSet(SocketSet);
						return -1;
					}

					pIocsocket->InitSocket(pIocport->m_pServer, tcpSocket);

					// NOTE: from this point on everyone else is aware of the
					// socket and the worker threads (and the send thread) share
					// the sockets

					pIocport->m_SidReceivedQueue.push(iSid);

					SDL_SemPost(pIocport->m_pWorkerSem);
				}
			}

			SDL_UnlockMutex(g_critical);
		}
	}

	SDLNet_FreeSocketSet(SocketSet);

	return 0;
}

//-----------------------------------------------------------------------------
int ReceiveWorkerThread(void* pData) {
	// NOTE: each worker thread keeps track of all their clients by each having
	// their own socketset. new connections added by the AcceptThread will get
	// added to a parictular threads socketset and that worker will receive all
	// their packets

	//SDL_SetThreadPriority(SDL_THREAD_PRIORITY_HIGH);

	CIOCPort* pIocport = (CIOCPort*) pData;

	int iSid = -1;
	std::list<int> mySids;
	CIOCSocket* pIocsocket = NULL;

	SDLNet_SocketSet SocketSet = SDLNet_AllocSocketSet(MAX_SOCKET);
	if(!SocketSet) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDLNet_GetError());
		return -1;
	}

	while(!SDL_AtomicGet(&pIocport->m_aCloseWorkers)) {
		// NOTE: check every so often to see if we need to close thread or if
		// our connected clients have packets to process
		int status = SDL_SemWaitTimeout(pIocport->m_pWorkerSem, 10);

		if(status == 0) {
			// NOTE: AcceptThread has sent us a new client to register
			SDL_LockMutex(g_critical);

			if(pIocport->m_SidReceivedQueue.size() == 0) {
				fprintf(stderr, "ERROR: Semaphore out of sync with queue!\n");
				SDL_UnlockMutex(g_critical);
				SDLNet_FreeSocketSet(SocketSet);
				return -1;
			}

			iSid = pIocport->m_SidReceivedQueue.front();
			pIocport->m_SidReceivedQueue.pop();

			if(iSid>=pIocport->m_SocketArraySize || iSid<0) {
				fprintf(stderr, "ERROR: Sid is out of bounds!\n");
				SDL_UnlockMutex(g_critical);
				SDLNet_FreeSocketSet(SocketSet);
				return -1;
			}

			pIocsocket = pIocport->m_SockArray[iSid];
			if(!pIocsocket) {
				fprintf(stderr, "ERROR: AcceptThread sent NULL client!\n");
				SDL_UnlockMutex(g_critical);
				SDLNet_FreeSocketSet(SocketSet);
				return -1;
			}

			if(SDLNet_TCP_AddSocket(SocketSet, pIocsocket->m_tcpSocket) == -1) {
				fprintf(stderr, "ERROR: \"%s\"\n", SDLNet_GetError());
				SDL_UnlockMutex(g_critical);
				SDLNet_FreeSocketSet(SocketSet);
				return -1;
			}

			mySids.push_back(iSid);

			SDL_UnlockMutex(g_critical);

		} else if(status == SDL_MUTEX_TIMEDOUT) {
			// NOTE: if we don't have any new clients then check our current
			// clients for packets
			SDL_LockMutex(g_critical);

			int numrdy = SDLNet_CheckSockets(SocketSet, 10);

			if(numrdy > 0) {
				std::list<int>::const_iterator iter = mySids.begin();
				while(iter != mySids.end()) {
					// NOTE: in theory we shouldn't have to lock the mutex
					// because we are the only ones working with this socket id
					pIocsocket = pIocport->m_SockArray[*iter];
					if(!pIocsocket) {
						fprintf(stderr, "ERROR: Worker is watching over a NULL client!\n");
						SDLNet_FreeSocketSet(SocketSet);
						return -1;
					}

					// NOTE: prevent "sends" on this socket while it's being
					// processed
					SDL_LockMutex(pIocsocket->m_critData);

					if(SDLNet_SocketReady(pIocsocket->m_tcpSocket)) {
						// TODO: need to send this info to a receive function
						// or something
						uint8_t buffer[MAX_PACKET_SIZE];
						int result = SDLNet_TCP_Recv(
							pIocsocket->m_tcpSocket, buffer, MAX_PACKET_SIZE
						);

						if(result <= 0) {
							SDLNet_TCP_DelSocket(SocketSet, pIocsocket->m_tcpSocket);

							if(pIocsocket->m_tcpSocket) {
								// TODO: need to move these to functions?
								SDLNet_TCP_Close(pIocsocket->m_tcpSocket);
								SDL_UnlockMutex(pIocsocket->m_critData);

								printf("Closed socket %d\n", pIocsocket->m_tcpSocket);
								pIocsocket->m_tcpSocket = NULL;

								pIocsocket = NULL;

								delete pIocport->m_SockArray[*iter];
								pIocport->m_SockArray[*iter] = NULL;
								pIocport->m_SockArrayInActive[*iter] = new CGameSocket();

								// NOTE: it must be the front because otherwise m_AiSendSocket
								// gets messed up and we fail to send packets
								pIocport->m_SidList.push_front(*iter);

								iter = mySids.erase(iter);

								numrdy--;
								continue;
							}
						} else {
							// TODO: sent the packet to where ever it needs to go
							// NOTE: I may need to implement their circular buffer stuff

							uint8_t* pRecvBuff = (uint8_t*) malloc(result*sizeof(uint8_t));
							memcpy(pRecvBuff, buffer, result);

							// NOTE: there could be multiple packets so this needs some working
							//       this seems to work for now but is obviously not safe
							uint16_t sOffset = 0;
							while(sOffset < result) {
								uint16_t sOldOffset = sOffset;
								sOffset += pIocsocket->ReceivedData(&pRecvBuff[sOffset], result); // TEMP

								for(int m=sOldOffset; m<sOffset; ++m)
									printf("0x%02X ", buffer[m]);
								printf("\n\n");
							}

							free(pRecvBuff);
						}

						numrdy--;
					}

					if(pIocsocket) SDL_UnlockMutex(pIocsocket->m_critData);

					iter++;
				}

				if(numrdy > 0) {
					fprintf(stderr, "ERROR: Couldn't process all ready clients!\n");
					SDLNet_FreeSocketSet(SocketSet);
					return -1;
				}
			}

			SDL_UnlockMutex(g_critical);

		} else {
			fprintf(stderr, "ERROR: \"%s\"\n", SDL_GetError());
			SDLNet_FreeSocketSet(SocketSet);
			return -1;
		}
	}

	SDLNet_FreeSocketSet(SocketSet);

	return 0;
}

//-----------------------------------------------------------------------------
int SendThreadMain(void* pData) {
	// TODO: grab packets and send them over the sockets?

	CIOCPort* pIocport = (CIOCPort*) pData;

	int iRemainCount = 0;
	SEND_DATA* pSendData = NULL;
	CGameSocket* pSocket = NULL;
	std::list<SEND_DATA*>::iterator iter;

	int size = 0, count = 0;

	while(!SDL_AtomicGet(&pIocport->m_aCloseSendThread)) {
		// NOTE: check every so often to see if we need to close the thread
		int status = SDL_SemWaitTimeout(pIocport->m_pSendSem, 10);

		if(status == 0) {
			SDL_LockMutex(pIocport->m_critSendData);

			iRemainCount = pIocport->m_SendDataList.size();
			if(iRemainCount <= 0) {
				SDL_UnlockMutex(pIocport->m_critSendData);
				continue;
			}

			while(pIocport->m_SendDataList.size()) {
				iter = pIocport->m_SendDataList.begin();
				pSendData = *iter;
				count = -1;

				for(int i=0; i<MAX_SOCKET; ++i) {
					SDL_LockMutex(g_critical);

					pSocket = (CGameSocket*) pIocport->m_SockArray[i]; // TODO: no need for mutex?
					if(pSocket == NULL) continue;
					count++;

					// NOTE: this would only ever happen if an uninitialize socket gets
					// put in the active socket array!!
					/*
					if(pSocket->GetSocketID() == -1) {
						// TODO: the AIServer waits for 10 sockets before data is handled
						// need to get a better hold on how this gets set back after disconnect
						fprintf(stderr, "ERROR: Invaild socket - Ebenezer likely disconnected!\n");

						if(pSendData) delete pSendData;
						pIocport->m_SendDataList.pop_front();
						SDL_UnlockMutex(pIocport->m_critSendData);

						return -1;
					}
					*/

					if(count == pIocport->m_AiSendSocket) {

						size = pSocket->Send(pSendData->pBuf, pSendData->sLength);

						if(size > 0) {
							pIocport->m_AiSendSocket++;
							if(pIocport->m_AiSendSocket >= MAX_AI_SOCKET) {
								pIocport->m_AiSendSocket = 0;
							}

							break;
						} else {
							fprintf(stderr, "ERROR: Failed to send on a socket!\n");
							count--;
						}
					}

					SDL_UnlockMutex(g_critical);
				}

				if(pSendData) delete pSendData;
				pIocport->m_SendDataList.pop_front();
			}

			SDL_UnlockMutex(pIocport->m_critSendData);

		} else if(status == SDL_MUTEX_TIMEDOUT) {

			continue;

		} else {
			fprintf(stderr, "ERROR: \"%s\"\n", SDL_GetError());
			return -1;
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
CIOCPort::CIOCPort(void) {
	m_ListenSocket = NULL;
	m_AiSendSocket = 0;
	m_SocketArraySize = 0;

	m_pSendSem = NULL;
	m_pWorkerSem = NULL;

	m_pAcceptThread = NULL;

	for(int i=0; i<MAX_USER; ++i) {
		m_ppWorkerThreads[i] = NULL;
	}
}

//-----------------------------------------------------------------------------
CIOCPort::~CIOCPort(void) {
	// NOTE: if you want to get the thread names from the thread you should do
	// so before ordering the thread to exit. after the thread exits it is no
	// longer valid

	// NOTE: close accepting thread
	SDL_AtomicSet(&m_aCloseAcceptThread, 1);

	if(m_pAcceptThread) {
		int atRet;
		SDL_WaitThread(m_pAcceptThread, &atRet);
		fprintf(stderr, "NOTE: Thread \"AcceptThread\" exited with code: %d\n", atRet);
	}

	// NOTE: close sending thread
	SDL_AtomicSet(&m_aCloseSendThread, 1);

	if(m_pSendThread) {
		int atRet;
		SDL_WaitThread(m_pSendThread, &atRet);
		fprintf(stderr, "NOTE: Thread \"SendThread\" exited with code: %d\n", atRet);
	}

	// NOTE: close worker threads
	SDL_AtomicSet(&m_aCloseWorkers, 1);

	int rets[MAX_USER];
	for(int i=0; i<(int) m_dwNumberOfWorkers; ++i) {
		if(m_ppWorkerThreads[i]) {
			SDL_WaitThread(m_ppWorkerThreads[i], &rets[i]);

			char name[0xFF]; memset(name, 0x00, 0xFF);
			sprintf(name, "ReceiveWorker%d", i);
			fprintf(stderr, "NOTE: Thread \"%s\" exited with code: %d\n", name, rets[i]);
		}
	}

	SDL_DestroySemaphore(m_pSendSem);
	SDL_DestroySemaphore(m_pWorkerSem);

	SDL_DestroyMutex(g_critical);
	SDL_DestroyMutex(m_critSendData);

	if(m_ListenSocket)
		SDLNet_TCP_Close(m_ListenSocket);

	SDLNet_Quit();

	printf("\n");
	system("pause");
}

//-----------------------------------------------------------------------------
void CIOCPort::CreateReceiveWorkerThread(int workernum) {
	// NOTE: create worker threads to process packets sent by the MAX_AI_SOCKET
	// number of Ebenezer sockets connecting to the AIServer

	// NOTE: these threads share a queue, sockets with incoming packets are
	// added to the queue. a semaphore is used to notify available threads of
	// incoming packets

	// NOTE: an atomic is used to close all the worker threads when the
	// AIServer is finished

	int numProcessors = SDL_GetCPUCount(); // number of logical cores

	if(!workernum)
		m_dwNumberOfWorkers = 2*numProcessors;
	else
		m_dwNumberOfWorkers = workernum;

	SDL_AtomicSet(&m_aCloseWorkers, 0);
	m_pWorkerSem = SDL_CreateSemaphore(0);

	for(int i=0; i<(int) m_dwNumberOfWorkers; ++i) {
		char name[0xFF]; memset(name, 0x00, 0xFF);
		sprintf(name, "ReceiveWorker%d", i); // TODO: why are these names fuked up? (probably not null-termed...)

		m_ppWorkerThreads[i] = SDL_CreateThread(
			ReceiveWorkerThread, name, (void*) this
		);

		if(m_ppWorkerThreads[i] == NULL) {
			fprintf(stderr,
				"ERROR: Failed to create receive worker thread \"%d\".\n", i
			);
			system("pause");
			exit(-1);
		}
	}
}

//-----------------------------------------------------------------------------
bool CIOCPort::Init(CEbenezerDlg* pServer, int serversocksize, int workernum /*= 0*/) {
	m_pServer = pServer;
	m_SocketArraySize = serversocksize;

	m_SockArray = new CIOCSocket*[serversocksize];
	for(int i=0; i<serversocksize; ++i) {
		m_SockArray[i] = NULL;
	}

	m_SockArrayInActive = new CIOCSocket*[serversocksize];
	for(int i=0; i<serversocksize; ++i) {
		m_SockArrayInActive[i] = NULL;
	}
	
	for(int i=0; i<serversocksize; ++i)
		m_SidList.push_back(i);
	
	g_critical = SDL_CreateMutex();
	m_critSendData = SDL_CreateMutex();

	if(!g_critical || !m_critSendData) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDL_GetError());
		return false;
	}

	CreateReceiveWorkerThread(workernum);
	CreateSendThread();

	if(SDLNet_Init() == -1) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDLNet_GetError());
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
bool CIOCPort::Listen(int port) {
	IPaddress ip;
	if(SDLNet_ResolveHost(&ip, NULL, port) == -1) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDLNet_GetError());
		return false;
	}

	m_ListenSocket = SDLNet_TCP_Open(&ip);
	if(!m_ListenSocket) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDLNet_GetError());
		return false;
	}

	CreateAcceptThread();

	return true;
}

//-----------------------------------------------------------------------------
void CIOCPort::CreateAcceptThread(void) {
	// NOTE: creates a thread responsible for accepting incoming connections
	// and initializing CIOCSocket objects for the incoming connection

	// NOTE: this thread also grabs incoming packets from sockets which have
	// already connected and are added to the socketset

	SDL_AtomicSet(&m_aCloseAcceptThread, 0);

	m_pAcceptThread = SDL_CreateThread(
		AcceptThread, "AcceptThread", (void*) this
	);

	if(m_pAcceptThread == NULL) {
		fprintf(stderr, "ERROR: Failed to create accept thread.\n");
		system("pause");
		exit(-1);
	}
}

//-----------------------------------------------------------------------------
int CIOCPort::GetNewSid(void) {
	if(m_SidList.empty()) {
		fprintf(stderr, "NOTE: SidList is empty!\n");
		return -1;
	}

	int ret = -1;

	if(SDL_LockMutex(g_critical) == 0) {
		ret = m_SidList.front();
		m_SidList.pop_front();

		SDL_UnlockMutex(g_critical);
	} else {
		fprintf(stderr, "ERROR: \"%s\"\n", SDL_GetError());
		return -1;
	}

	return ret;
}

//-----------------------------------------------------------------------------
CIOCSocket* CIOCPort::GetIOCSocket(int index) {
	CIOCSocket* pIOCSock = NULL;

	if(index > m_SocketArraySize) {
		fprintf(stderr,
			"ERROR: InActiveSocket array overflow \"%d\".\n", index
		);
		return NULL;
	} else if(!m_SockArrayInActive[index]) {
		fprintf(stderr, "ERROR: InActiveSocket array invalid.\n");
		return NULL;
	} else
		pIOCSock = (CIOCSocket*) m_SockArrayInActive[index];

	if(SDL_LockMutex(g_critical) == 0) {
		m_SockArray[index] = pIOCSock;
		m_SockArrayInActive[index] = NULL;

		pIOCSock->SetSocketID(index);

		SDL_UnlockMutex(g_critical);
	} else {
		fprintf(stderr, "ERROR: \"%s\"\n", SDL_GetError());
		return NULL;
	}

	return pIOCSock;
}

//-----------------------------------------------------------------------------
void CIOCPort::CreateSendThread(void) {
	// NOTE: creates a thread responsible for sending packets out to Ebenezer.
	// a queue of the packets which need to be sent are distributed more or
	// less evenly over all the connected sockets. also, no information is sent
	// over a socket currently being processed by a worker thread

	SDL_AtomicSet(&m_aCloseSendThread, 0);
	m_pSendSem = SDL_CreateSemaphore(0);

	m_pSendThread = SDL_CreateThread(
		SendThreadMain, "SendThread", (void*) this
	);

	if(m_pSendThread == NULL) {
		fprintf(stderr, "ERROR: Failed to create accept thread.\n");
		system("pause");
		exit(-1);
	}
}
