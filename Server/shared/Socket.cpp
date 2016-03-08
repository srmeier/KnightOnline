#include "stdafx.h"
#include "SocketMgr.h"

Socket::Socket(SOCKET fd, uint32 sendbuffersize, uint32 recvbuffersize) 
	: m_fd(fd), m_connected(false),	m_deleted(false), m_socketMgr(nullptr)
{
	// Allocate buffers
	readBuffer.Allocate(recvbuffersize);
	writeBuffer.Allocate(sendbuffersize);

	// IOCP member variables
	m_completionPort = 0;
	m_writeLock = 0;

	// Check for needed fd allocation.
	if (m_fd == 0)
		m_fd = SocketOps::CreateTCPFileDescriptor();
}

bool Socket::Connect(const char * Address, uint32 Port)
{
	struct hostent * ci = gethostbyname(Address);
	if (ci == 0)
		return false;

	m_client.sin_family = ci->h_addrtype;
	m_client.sin_port = ntohs((u_short)Port);
	memcpy(&m_client.sin_addr.s_addr, ci->h_addr_list[0], ci->h_length);

	SocketOps::Blocking(m_fd);

	if (m_fd == 0)
		m_fd = SocketOps::CreateTCPFileDescriptor();

	if (connect(m_fd, (const sockaddr*)&m_client, sizeof(m_client)) == -1)
		return false;

	// at this point the connection was established
	m_completionPort = m_socketMgr->GetCompletionPort();

	_OnConnect();
	return true;
}
void Socket::Accept(sockaddr_in * address)
{
	memcpy(&m_client, address, sizeof(*address));
	_OnConnect();
}

void Socket::_OnConnect()
{
	// set common parameters on the file descriptor
	m_connected = true;

	m_writeLockMutex.lock();
	m_writeLock = 0;
	m_writeLockMutex.unlock();

	AssignToCompletionPort();
	m_socketMgr->OnConnect(this);

	// Call virtual onconnect
	OnConnect();

	// Setting the read event up after calling OnConnect() ensures OnConnect() & subsequent connection setup code is run first (which is NOT GUARANTEED otherwise)
	SetupReadEvent();
}

bool Socket::Send(const uint8 * Bytes, uint32 Size)
{
	bool rv;

	// This is really just a wrapper for all the burst stuff.
	BurstBegin();
	rv = BurstSend(Bytes, Size);
	if (rv)
		BurstPush();
	BurstEnd();

	return rv;
}

bool Socket::BurstSend(const uint8 * Bytes, uint32 Size)
{
	return writeBuffer.Write(Bytes, Size);
}

std::string Socket::GetRemoteIP()
{
	char* ip = (char*)inet_ntoa(m_client.sin_addr);
	if (ip != nullptr)
		return std::string(ip);

	return std::string("noip");
}

void Socket::Disconnect()
{
	if (!IsConnected())
		return;

	m_connected = false;

	m_readEvent.Unmark();

	// Call virtual ondisconnect
	OnDisconnect();
	GetSocketMgr()->OnDisconnect(this);

	SocketOps::CloseSocket(m_fd);
	m_fd = 0;

	m_writeLockMutex.lock();
	m_writeLock = 0;
	m_writeLockMutex.unlock();

	// Reset the read/write buffers
	GetReadBuffer().Remove(GetReadBuffer().GetSize());
	GetWriteBuffer().Remove(GetWriteBuffer().GetSize());
}

void Socket::Delete()
{
	if (IsDeleted())
		return;

	m_deleted = true;

	if (IsConnected()) 
		Disconnect();

	delete this;
}

Socket::~Socket()
{
}
