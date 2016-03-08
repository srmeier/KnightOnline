#include "stdafx.h"
#include "HardwareInformation.h"

HardwareInformation::HardwareInformation()
{ 
}

/**
* @brief	License Number Control.
*/
bool HardwareInformation::IsValidHardwareID(std::vector<int64> HardwareIDArray)
{
	uint16 MACData1, MACData2 = 0;
	GetMacHash(MACData1, MACData2);
	int64 nLicenseNumber = GetHardwareID();

	foreach (itr, HardwareIDArray)
		if (*itr == nLicenseNumber)
			return true;

	return false;
}

/**
* @brief	Get License Number
*/
int64 HardwareInformation::GetHardwareID()
{
	uint16 MACData1, MACData2 = 0;
	GetMacHash(MACData1, MACData2);
	return _atoi64(string_format("%d%d%d%d",MACData1,MACData2,GetCPUHash(),GetVolumeHash()).c_str());
}