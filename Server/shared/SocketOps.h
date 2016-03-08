#pragma once

namespace SocketOps
{
	// Create file descriptor for socket i/o operations.
	SOCKET CreateTCPFileDescriptor();

	// Disable blocking send/recv calls.
	bool Nonblocking(SOCKET fd);

	// Enable blocking send/recv calls.
	bool Blocking(SOCKET fd);

	// Disable nagle buffering algorithm
	bool DisableBuffering(SOCKET fd);

	// Enables nagle buffering algorithm
	bool EnableBuffering(SOCKET fd);

	// Closes socket completely.
	void CloseSocket(SOCKET fd);
};