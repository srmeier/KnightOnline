#pragma once

#include "SocketMgr.h"
#include "Packet.h"
#include "JvCryption.h"

// KO sockets time out after at least 30 seconds of inactivity.
#define KOSOCKET_TIMEOUT (30) 

// Allow up to 30 minutes for the player to create their character / the client to load.
#define KOSOCKET_LOADING_TIMEOUT (30 * 60)

class KOSocket : public Socket
{
public:
	KOSocket(uint16_t socketID, SocketMgr * mgr, SOCKET fd, uint32_t sendBufferSize, uint32_t recvBufferSize);
	~KOSocket();

	INLINE bool isCryptoEnabled() { return m_usingCrypto; };
	INLINE uint16_t GetSocketID() { return m_socketID; };
	INLINE uint16_t GetTargetID() { return m_targetID; };
	INLINE time_t GetLastResponseTime() { return m_lastResponse; }

	virtual void OnConnect();
	virtual void OnRead();
	virtual bool DecryptPacket(uint8_t *in_stream, Packet & pkt);
	virtual bool HandlePacket(Packet & pkt) = 0;

	virtual bool Send(Packet * pkt);
	virtual bool SendCompressed(Packet * pkt);
	bool BuildCompressed(const Packet * pkt, Packet & result);

	virtual void OnDisconnect();

	void EnableCrypto();

protected:
	CJvCryption m_crypto;
	time_t m_lastResponse;
	uint32_t m_sequence;
	uint16_t m_remaining, m_socketID, m_targetID;
	uint8_t m_readTries;
	bool m_usingCrypto;
};