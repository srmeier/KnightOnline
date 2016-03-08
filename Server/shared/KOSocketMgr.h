#pragma once

#include <map>
#include "RWLock.h"
#include "SocketMgr.h"
#include "KOSocket.h"

typedef std::map<uint16, KOSocket *> SessionMap;

template <class T>
class KOSocketMgr : public SocketMgr
{
public:
	KOSocketMgr<T>() : m_server(nullptr) {}

	virtual void InitSessions(uint16 sTotalSessions);
	virtual bool Listen(uint16 sPort, uint16 sTotalSessions);
	virtual bool Listen(std::string sIPAddress, uint16 sPort, uint16 sTotalSessions);

	virtual void OnConnect(Socket *pSock);
	virtual Socket *AssignSocket(SOCKET socket);
	virtual void DisconnectCallback(Socket *pSock);

	void RunServer()
	{
		SpawnWorkerThreads();
		GetServer()->run();
	}

	// Send a packet to all active sessions
	void SendAll(Packet * pkt) 
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		SessionMap & sessMap = m_activeSessions;
		for (auto itr = sessMap.begin(); itr != sessMap.end(); ++itr)
			itr->second->Send(pkt);
	}

	void SendAllCompressed(Packet * result) 
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);
		SessionMap & sessMap = m_activeSessions;
		for (auto itr = sessMap.begin(); itr != sessMap.end(); ++itr)
			itr->second->SendCompressed(result);
	}

	ListenSocket<T> * GetServer() { return m_server; }
	INLINE SessionMap & GetIdleSessionMap() { return m_idleSessions; }
	INLINE SessionMap & GetActiveSessionMap() { return m_activeSessions; }
	INLINE std::recursive_mutex& GetLock() { return m_lock; }

	T * operator[] (uint16 id)
	{
		std::lock_guard<std::recursive_mutex> lock(m_lock);

		auto itr = m_activeSessions.find(id);
		if (itr != m_activeSessions.end())
			return static_cast<T *>(itr->second);

		return nullptr;
	}

	void Shutdown();
	virtual ~KOSocketMgr();

protected:
	SessionMap m_idleSessions, m_activeSessions;
	std::recursive_mutex m_lock;

private:
	ListenSocket<T> * m_server;
};

template <class T>
void KOSocketMgr<T>::InitSessions(uint16 sTotalSessions)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);
	for (uint16 i = 0; i < sTotalSessions; i++)
		m_idleSessions.insert(std::make_pair(i, new T(i, this)));
}

template <class T>
bool KOSocketMgr<T>::Listen(uint16 sPort, uint16 sTotalSessions)
{
	return Listen("0.0.0.0", sPort, sTotalSessions);
}

template <class T>
bool KOSocketMgr<T>::Listen(std::string sIPAddress, uint16 sPort, uint16 sTotalSessions)
{
	if (m_server != nullptr)
		return false;

	CreateCompletionPort();

	m_server = new ListenSocket<T>(this, sIPAddress.c_str(), sPort);
	if (!m_server->IsOpen())
		return false;

	InitSessions(sTotalSessions);
	return true;
}

template <class T>
Socket * KOSocketMgr<T>::AssignSocket(SOCKET socket)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);
	Socket *pSock = nullptr;

	for (auto itr = m_idleSessions.begin(); itr != m_idleSessions.end(); itr++)
	{
		m_activeSessions.insert(std::make_pair(itr->first, itr->second));
		pSock = itr->second;
		m_idleSessions.erase(itr);
		pSock->SetFd(socket);
		break;
	}
	return pSock;
}

template <class T>
void KOSocketMgr<T>::OnConnect(Socket *pSock)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);
	auto itr = m_idleSessions.find(static_cast<KOSocket *>(pSock)->GetSocketID());
	if (itr != m_idleSessions.end())
	{
		m_activeSessions.insert(std::make_pair(itr->first, itr->second));
		m_idleSessions.erase(itr);
	}
}

template <class T>
void KOSocketMgr<T>::DisconnectCallback(Socket *pSock)
{
	std::lock_guard<std::recursive_mutex> lock(m_lock);
	auto itr = m_activeSessions.find(static_cast<T *>(pSock)->GetSocketID());
	if (itr != m_activeSessions.end())
	{
		m_idleSessions.insert(std::make_pair(itr->first, itr->second));
		m_activeSessions.erase(itr);
	}
}

template <class T>
void KOSocketMgr<T>::Shutdown()
{
	if (m_bShutdown)
		return;

	if (m_server != nullptr)
		delete m_server;

	SocketMgr::Shutdown();
}

template <class T>
KOSocketMgr<T>::~KOSocketMgr()
{
	Shutdown();
}