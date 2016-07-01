#pragma once

#include "StdAfx.h"

#define USE_CRYPTION

extern "C"
{
	#include "crc32.h"
}

class CJvCryption
{
private:
	Uint64 m_public_key, m_tkey;

public:
	CJvCryption() : m_public_key(0) {}

	inline Uint64 GetPublicKey() { return m_public_key; }
	inline void SetPublicKey(Uint64 PublicKey) {
		m_public_key = PublicKey;
	}

	void Init();

	void JvEncryptionFast(int len, Uint8 *datain, Uint8 *dataout);
	inline void JvDecryptionFast(int len, Uint8 *datain, Uint8 *dataout) { JvEncryptionFast(len, datain, dataout); };
	
	int JvDecryptionWithCRC32(int len, Uint8 *datain, Uint8 *dataout);
};
