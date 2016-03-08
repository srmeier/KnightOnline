#pragma once

#include "SocketMgr.h"
#include "Packet.h"
#include "JvCryption.h"

extern "C"
{
	#include "lzf.h"
}

// KO sockets time out after at least 30 seconds of inactivity.
#define KOSOCKET_TIMEOUT (30) 

// Allow up to 30 minutes for the player to create their character / the client to load.
#define KOSOCKET_LOADING_TIMEOUT (30 * 60)

class KOSocket : public Socket
{
public:
	KOSocket(uint16 socketID, SocketMgr * mgr, SOCKET fd, uint32 sendBufferSize, uint32 recvBufferSize);

	INLINE bool isCryptoEnabled() { return m_usingCrypto; };
	INLINE uint16 GetSocketID() { return m_socketID; };
	INLINE uint16 GetTargetID() { return m_targetID; };
	INLINE time_t GetLastResponseTime() { return m_lastResponse; }

	virtual void OnConnect();
	virtual void OnRead();
	virtual bool DecryptPacket(uint8 *in_stream, Packet & pkt);
	virtual bool HandlePacket(Packet & pkt) = 0;

	virtual bool Send(Packet * pkt);
	virtual bool SendCompressed(Packet * pkt);

	virtual void OnDisconnect();

	void EnableCrypto();

protected:
	CJvCryption m_crypto;
	time_t m_lastResponse;
	uint32 m_sequence;
	uint16 m_remaining, m_socketID, m_targetID;
	uint8 m_readTries;
	bool m_usingCrypto;
};