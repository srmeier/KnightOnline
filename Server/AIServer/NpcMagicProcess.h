#pragma once

class CNpcMagicProcess  
{
public:
	static time_t MagicPacket(uint8_t opcode, uint32_t nSkillID, int16_t sCasterID, int16_t sTargetID, 
		int16_t sData1 = 0, int16_t sData2 = 0, int16_t sData3 = 0);
};
