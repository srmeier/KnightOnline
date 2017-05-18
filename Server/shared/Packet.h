#pragma once

#include "ByteBuffer.h"

class Packet : public ByteBuffer
{
public:
	INLINE Packet() : ByteBuffer(), m_opcode(0) { }
	INLINE Packet(uint8_t opcode) : ByteBuffer(4096), m_opcode(opcode) {}
	INLINE Packet(uint8_t opcode, size_t res) : ByteBuffer(res), m_opcode(opcode) {}
	INLINE Packet(const Packet &packet) : ByteBuffer(packet), m_opcode(packet.m_opcode) {}
	INLINE Packet(uint8_t opcode, uint8_t subOpcode) : m_opcode(opcode)
	{
		append(&subOpcode, 1);
	}

	//! Clear packet and set opcode all in one mighty blow
	INLINE void Initialize(uint8_t opcode)
	{
		clear();
		m_opcode = opcode;
	}

	INLINE uint8_t GetOpcode() const { return m_opcode; }
	INLINE void SetOpcode(uint8_t opcode) { m_opcode = opcode; }

protected:
	uint8_t m_opcode;
};
