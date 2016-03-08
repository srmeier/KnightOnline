#include "stdafx.h"

namespace SocketOps
{
	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor()
	{
		// create a socket for use with overlapped i/o.
		return ::WSASocket(AF_INET, SOCK_STREAM, 0, 0, 0, WSA_FLAG_OVERLAPPED);
	}

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd)
	{
		u_long arg = 1;
		return (::ioctlsocket(fd, FIONBIO, &arg) == 0);
	}

	// Disable blocking send/recv calls.
	bool Blocking(SOCKET fd)
	{
		u_long arg = 0;
		return (ioctlsocket(fd, FIONBIO, &arg) == 0);
	}

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd)
	{
		uint32 arg = 1;
		return (setsockopt(fd, 0x6, TCP_NODELAY, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Enable nagle buffering algorithm
	bool EnableBuffering(SOCKET fd)
	{
		uint32 arg = 0;
		return (setsockopt(fd, 0x6, TCP_NODELAY, (const char*)&arg, sizeof(arg)) == 0);
	}

	// Closes a socket fully.
	void CloseSocket(SOCKET fd)
	{
		shutdown(fd, SD_BOTH);
		closesocket(fd);
	}

}
