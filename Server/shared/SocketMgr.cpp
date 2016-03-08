#include "stdafx.h"
#include "SocketMgr.h"

bool SocketMgr::s_bRunningCleanupThread = true;
std::recursive_mutex SocketMgr::s_disconnectionQueueLock;
std::queue<Socket *> SocketMgr::s_disconnectionQueue;

Thread SocketMgr::s_cleanupThread; 
Atomic<uint32> SocketMgr::s_refCounter;

uint32 THREADCALL SocketCleanupThread(void * lpParam)
{
	while (SocketMgr::s_bRunningCleanupThread)
	{
		SocketMgr::s_disconnectionQueueLock.lock();
		while (!SocketMgr::s_disconnectionQueue.empty())
		{
			Socket *pSock = SocketMgr::s_disconnectionQueue.front();
			if (pSock->GetSocketMgr())
				pSock->GetSocketMgr()->DisconnectCallback(pSock);
			SocketMgr::s_disconnectionQueue.pop();
		}
		SocketMgr::s_disconnectionQueueLock.unlock();
		sleep(100);
	}

	return 0;
}

SocketMgr::SocketMgr() : m_threadCount(0), 
	m_bWorkerThreadsActive(false),
	m_bShutdown(false)
{
	static bool bRefCounterInitialised = false;
	if (!bRefCounterInitialised)
	{
		s_refCounter = 0;
		bRefCounterInitialised = true;
	}

	IncRef();
	Initialise();
}

void SocketMgr::SpawnWorkerThreads()
{
	if (m_bWorkerThreadsActive)
		return;

	m_bWorkerThreadsActive = true;
	m_thread = new Thread(SocketWorkerThread, this);

	if (!s_cleanupThread.isStarted())
		s_cleanupThread.start(SocketCleanupThread);
}


uint32 THREADCALL SocketMgr::SocketWorkerThread(void * lpParam)
{
	SocketMgr *socketMgr = (SocketMgr *)lpParam;
	HANDLE cp = socketMgr->GetCompletionPort();
	DWORD len;
	Socket * s = nullptr;
	OverlappedStruct * ov = nullptr;
	LPOVERLAPPED ol_ptr;

	while (socketMgr->m_bWorkerThreadsActive)
	{
		if (!GetQueuedCompletionStatus(cp, &len, (LPDWORD)&s, &ol_ptr, INFINITE))
		{
			if (s != nullptr)
				s->Disconnect();
			continue;
		}

		ov = CONTAINING_RECORD(ol_ptr, OverlappedStruct, m_overlap);

		if (ov->m_event == SOCKET_IO_THREAD_SHUTDOWN)
		{
			delete ov;
			return 0;
		}

		if (ov->m_event < NUM_SOCKET_IO_EVENTS)
			ophandlers[ov->m_event](s, len);
	}

	return 0;
}

void SocketMgr::Initialise()
{
	m_completionPort = nullptr;
}

void SocketMgr::CreateCompletionPort()
{
	SetCompletionPort(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, (ULONG_PTR)0, 0));
}

void SocketMgr::SetupWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
}

void HandleReadComplete(Socket * s, uint32 len)
{
	if (s->IsDeleted())
		return;

	s->m_readEvent.Unmark();
	if (len)
	{
		s->GetReadBuffer().IncrementWritten(len);
		s->OnRead();
		s->SetupReadEvent();
	}
	else
	{
		// s->Delete();	  // Queue deletion.
		s->Disconnect();
	}
}

void HandleWriteComplete(Socket * s, uint32 len)
{
	if (s->IsDeleted())
		return;

	s->m_writeEvent.Unmark();
	s->BurstBegin();					// Lock
	s->GetWriteBuffer().Remove(len);
	if( s->GetWriteBuffer().GetContiguousBytes() > 0 )
		s->WriteCallback();
	else
		s->DecSendLock();
	s->BurstEnd();					  // Unlock
}

void HandleShutdown(Socket * s, uint32 len) {}

void SocketMgr::OnConnect(Socket *pSock) {}
void SocketMgr::DisconnectCallback(Socket *pSock) {}
void SocketMgr::OnDisconnect(Socket *pSock) 
{
	Guard lock(s_disconnectionQueueLock);
	s_disconnectionQueue.push(pSock);
}

void SocketMgr::ShutdownThreads()
{
	OverlappedStruct * ov = new OverlappedStruct(SOCKET_IO_THREAD_SHUTDOWN);
	PostQueuedCompletionStatus(m_completionPort, 0, (ULONG_PTR)0, &ov->m_overlap);

	m_bWorkerThreadsActive = false;

	m_thread->waitForExit();
	delete m_thread;
}

void SocketMgr::Shutdown()
{
	if (m_bShutdown)
		return;

	ShutdownThreads();

	DecRef();
	m_bShutdown = true;
}

void SocketMgr::SetupSockets()
{
	SetupWinsock();
}

void SocketMgr::CleanupSockets()
{
	if (s_cleanupThread.isStarted())
	{
		s_bRunningCleanupThread = false;
		s_cleanupThread.waitForExit();
	}

	WSACleanup();
}

SocketMgr::~SocketMgr()
{
	Shutdown();
}
