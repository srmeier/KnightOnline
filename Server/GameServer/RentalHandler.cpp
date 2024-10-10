#include "stdafx.h"

void CUser::RentalSystem(Packet & pkt)
{
	if (isDead())
		return;

	uint8_t opcode = pkt.read<uint8_t>();

	// TODO
	/*if (opcode == 1)
	PremiumRentalProcess(pBuf + index);
	else if (opcode == 2)
	PvPRentalProcess(pBuf + index);*/
}

