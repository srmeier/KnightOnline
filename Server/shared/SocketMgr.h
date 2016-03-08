#pragma once

#include <queue>
#include <set>
#include <map>
#include "Socket.h"

uint32 THREADCALL SocketCleanupThread(void * lpParam);

class SocketMgr
{
public:
	SocketMgr();

	void Initialise();

	void SpawnWorkerThreads();
	void ShutdownThreads();

	static void SetupSockets();
	static void CleanupSockets();

	INLINE HANDLE GetCompletionPort() { return m_completionPort; }
	INLINE void SetCompletionPort(HANDLE cp) { m_completionPort = cp; }
	void CreateCompletionPort();

	static void SetupWinsock();
	static void CleanupWinsock();
	
	static uint32 THREADCALL SocketWorkerThread(void * lpParam);

	HANDLE m_completionPort;

	virtual Socket *AssignSocket(SOCKET socket) = 0;
	virtual void OnConnect(Socket *pSock);
	virtual void OnDisconnect(Socket *pSock);
	virtual void DisconnectCallback(Socket *pSock);
	virtual void Shutdown();
	virtual ~SocketMgr();

	static std::recursive_mutex s_disconnectionQueueLock;
	static std::queue<Socket *> s_disconnectionQueue;

protected:
	bool m_bShutdown;

	Thread * m_thread;
	static Thread s_cleanupThread;

	long m_threadCount;
	bool m_bWorkerThreadsActive;

	INLINE void IncRef() { if (s_refCounter.increment() == 1) SetupSockets(); }
	INLINE void DecRef() { if (s_refCounter.decrement() == 0) CleanupSockets(); }

	// reference counter (one app can hold multiple socket manager instances)
	static Atomic<uint32> s_refCounter;

public:
	static bool s_bRunningCleanupThread;
};

typedef void(*OperationHandler)(Socket * s, uint32 len);

void HandleReadComplete(Socket * s, uint32 len);
void HandleWriteComplete(Socket * s, uint32 len);
void HandleShutdown(Socket * s, uint32 len);

static OperationHandler ophandlers[] =
{
	&HandleReadComplete,
	&HandleWriteComplete,
	&HandleShutdown
};
