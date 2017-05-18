#include "stdafx.h"
#include "KOSocket.h"
#include "packets.h"
#include "version.h"
#include "Compression.h"

KOSocket::KOSocket(uint16 socketID, SocketMgr * mgr, SOCKET fd, uint32 sendBufferSize, uint32 recvBufferSize) 
	: Socket(fd, sendBufferSize, recvBufferSize), 
	m_socketID(socketID), m_remaining(0),  m_usingCrypto(false), 
	m_readTries(0), m_sequence(0), m_lastResponse(0) 
{
	SetSocketMgr(mgr);
}

KOSocket::~KOSocket()
{
}

void KOSocket::OnConnect()
{
	TRACE("Connection received from %s:%d\n", GetRemoteIP().c_str(), GetRemotePort());

	m_remaining = 0;
	m_usingCrypto = false;
	m_readTries = 0;
	m_sequence = 0;
	m_lastResponse = UNIXTIME;
}

/*
typedef struct
{
	int x;
	int y;
	unsigned char m[256];
}
arc4_context;

void arc4_setup(arc4_context *ctx, unsigned char *key, int keylen)
{
	int i, j, k, a;
	unsigned char *m;

	ctx->x = 0;
	ctx->y = 0;
	m = ctx->m;

	for(i = 0; i < 256; i++)
		m[i] = (unsigned char)i;

	j = k = 0;

	for(i = 0; i < 256; i++, k++)
	{
		if(k >= keylen) k = 0;

		a = m[i];
		j = (j + a + key[k]) & 0xFF;
		m[i] = m[j];
		m[j] = (unsigned char)a;
	}
}

void arc4_crypt(arc4_context *ctx, unsigned char *buf, int buflen)
{
	int i, x, y, a, b;
	unsigned char *m;

	x = ctx->x;
	y = ctx->y;
	m = ctx->m;

	for(i = 0; i < buflen; i++)
	{
		x = (x + 1) & 0xFF; a = m[x];
		y = (y + a) & 0xFF; b = m[y];

		m[x] = (unsigned char)b;
		m[y] = (unsigned char)a;

		buf[i] = (unsigned char)
			(buf[i] ^ m[(unsigned char)(a + b)]);
	}

	ctx->x = x;
	ctx->y = y;
}
*/

void KOSocket::OnRead() 
{
	Packet pkt;

	for (;;) 
	{
		if (m_remaining == 0) 
		{
			/*
			bool found = false;
			//unsigned char key[5] = {};
			long long unsigned int key = 0xFFFFFFFFFFFFFFFF;
			int len = GetReadBuffer().GetSize();
			unsigned char* tmpbuffer = (unsigned char*)malloc(len*sizeof(unsigned char));

			do {
				memcpy(tmpbuffer, GetReadBuffer().GetBufferStart(), len);

				arc4_context rc4_ctx;
				long long unsigned int key2 = RandUInt64();
				arc4_setup(&rc4_ctx, (unsigned char*)&key2, 5);//(unsigned char *)"\xC2\xBA\xE1\x87\xDB", 5);
				arc4_crypt(&rc4_ctx, tmpbuffer, len);
				key--;

				if(key%100000 == 0)
					TRACE("left: 0x%16llX\n", key2);

				char bit[9] = {};
				for(int i=0; i<len-8; ++i) {
					memcpy(bit, &tmpbuffer[i], 8);
					if(!strcmp(bit, "bitshift")) {
						found = true;
					}
				}

			} while(!found);
			*/

			if (GetReadBuffer().GetSize() < 5)
				return; //check for opcode as well

			uint16 header = 0;
			GetReadBuffer().Read(&header, 2);
			if (header != 0x55aa)
			{
				TRACE("%s: Got packet without header 0x55AA, got 0x%X\n", GetRemoteIP().c_str(), header);
				goto error_handler;
			}

			GetReadBuffer().Read(&m_remaining, 2);
			if (m_remaining == 0)
			{
				TRACE("%s: Got packet without an opcode, this should never happen.\n", GetRemoteIP().c_str());
				goto error_handler;
			}
		}

		if (m_remaining > GetReadBuffer().GetAllocatedSize()) 
		{
			TRACE("%s: Packet received which was %u bytes in size, maximum of %u.\n", GetRemoteIP().c_str(), m_remaining, GetReadBuffer().GetAllocatedSize());
			goto error_handler;
		}

		if (m_remaining > GetReadBuffer().GetSize()) 
		{
			if (m_readTries > 4)
			{
				TRACE("%s: packet fragmentation count is over 4, disconnecting as they're probably up to something bad\n", GetRemoteIP().c_str());
				goto error_handler;
			}
			m_readTries++;
			return;
		}

		uint8 *in_stream = new uint8[m_remaining];

		m_readTries = 0;
		GetReadBuffer().Read(in_stream, m_remaining);

		uint16 footer = 0;
		GetReadBuffer().Read(&footer, 2);

		if (footer != 0xaa55
			|| !DecryptPacket(in_stream, pkt))
		{
			TRACE("%s: Footer invalid (%X) or failed to decrypt.\n", GetRemoteIP().c_str(), footer);
			delete [] in_stream;
			goto error_handler;
		}

		delete [] in_stream;

		// Update the time of the last (valid) response from the client.
		m_lastResponse = UNIXTIME;

		if (!HandlePacket(pkt))
		{
			TRACE("%s: Handler for packet %X returned false\n", GetRemoteIP().c_str(), pkt.GetOpcode());
#ifndef _DEBUG
			goto error_handler;
#endif
		}

		m_remaining = 0;
	}

	return;

error_handler:
	Disconnect();
}

bool KOSocket::DecryptPacket(uint8 *in_stream, Packet & pkt)
{
	uint8* final_packet = nullptr;

	if (isCryptoEnabled())
	{
		// Invalid packet (all encrypted packets need a CRC32 checksum!)
		if (m_remaining < 4 
			// Invalid checksum 
				|| m_crypto.JvDecryptionWithCRC32(m_remaining, in_stream, in_stream) < 0 
				// Invalid sequence ID
				|| ++m_sequence != *(uint32 *)(in_stream)) 
				return false;

		m_remaining -= 8; // remove the sequence ID & CRC checksum
		final_packet = &in_stream[4];
	}
	else
	{
		final_packet = in_stream; // for simplicity :P
	}

	m_remaining--;
	pkt = Packet(final_packet[0], (size_t)m_remaining);
	if (m_remaining > 0) 
	{
		pkt.resize(m_remaining);
		memcpy((void*)pkt.contents(), &final_packet[1], m_remaining);
	}

	return true;
}

bool KOSocket::Send(Packet * pkt) 
{
	if (!IsConnected() || pkt->size() + 1 > GetWriteBuffer().GetAllocatedSize())
		return false;

	bool r;

	uint8 opcode = pkt->GetOpcode();
	uint8 * out_stream = nullptr;
	uint16 len = (uint16)(pkt->size() + 1); // +1 for opcode

	if (isCryptoEnabled())
	{
		len += 5; // +5 [1EFC][m_sequence][00]

		out_stream = new uint8[len];

		*(uint16 *)&out_stream[0] = 0x1efc;
		*(uint16 *)&out_stream[2] = (uint16)(m_sequence); // this isn't actually incremented here
		out_stream[4] = 0;
		out_stream[5] = pkt->GetOpcode();

		if (pkt->size() > 0)
			memcpy(&out_stream[6], pkt->contents(), pkt->size());

		m_crypto.JvEncryptionFast(len, out_stream, out_stream);
	}
	else
	{
		out_stream = new uint8[len];
		out_stream[0] = pkt->GetOpcode();
		if (pkt->size() > 0)
			memcpy(&out_stream[1], pkt->contents(), pkt->size());
	}

	BurstBegin();

	if (GetWriteBuffer().GetSpace() < size_t(len + 6))
	{
		BurstEnd();
		Disconnect();
		return false;
	}

	r = BurstSend((const uint8*)"\xaa\x55", 2);
	if (r) r = BurstSend((const uint8*)&len, 2);
	if (r) r = BurstSend((const uint8*)out_stream, len);
	if (r) r = BurstSend((const uint8*)"\x55\xaa", 2);
	if (r) BurstPush();
	BurstEnd();

	delete [] out_stream;
	return r;
}

bool KOSocket::SendCompressed(Packet * pkt) 
{
	Packet result(WIZ_COMPRESS_PACKET);
	if (!BuildCompressed(pkt, result))
		return Send(pkt);

	return Send(&result);
}

bool KOSocket::BuildCompressed(const Packet * pkt, Packet & result)
{
	uint32 inLength = pkt->size() + 1, crc = 0;
	uint32 outLength = 0;

	if(inLength < Compression::MinBytes)
		return false;

	uint8 *buffer = new uint8[inLength];

	*buffer = pkt->GetOpcode();
	if(pkt->size() > 0)
		memcpy(buffer + 1, pkt->contents(), pkt->size());
	uint8 * outBuffer = Compression::CompressWithCRC32(buffer, inLength, &outLength, &crc);

	result << uint16(outLength) << uint16(inLength) << crc;
	result.append(outBuffer, outLength);

	delete[] buffer;
	delete[] outBuffer;
	return true;
}

void KOSocket::OnDisconnect()
{
	TRACE("Connection closed from %s:%d\n", GetRemoteIP().c_str(), GetRemotePort());
}

void KOSocket::EnableCrypto()
{
#ifdef USE_CRYPTION
	m_crypto.Init();
	m_usingCrypto = true;
#endif
}
