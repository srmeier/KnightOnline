#pragma once

#include "ByteBuffer.h"

class Packet : public ByteBuffer
{
public:
	INLINE Packet() : ByteBuffer()
	{
	}

	INLINE Packet(uint8_t opcode) : ByteBuffer(4096)
	{
		append(&opcode, 1);
	}

	INLINE Packet(uint8_t opcode, size_t res) : ByteBuffer(res)
	{
		append(&opcode, 1);
	}

	INLINE Packet(const Packet &packet) : ByteBuffer(packet)
	{
	}

	INLINE Packet(uint8_t opcode, uint8_t subOpcode)
	{
		append(&opcode, 1);
		append(&subOpcode, 1);
	}

	INLINE uint8_t GetOpcode() const
	{
		return size() == 0 ? 0 : _storage[0];
	}

	//! Clear packet and set opcode all in one mighty blow
	INLINE void Initialize(uint8_t opcode)
	{
		clear();
		append(&opcode, 1);
	}
};
