#pragma once

class CNpcMagicProcess  
{
public:
	static time_t MagicPacket(uint8 opcode, uint32 nSkillID, int16 sCasterID, int16 sTargetID, 
		int16 sData1 = 0, int16 sData2 = 0, int16 sData3 = 0);
};
