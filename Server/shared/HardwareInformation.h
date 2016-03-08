#pragma once

#include "stdafx.h"
#include <Iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")

class HardwareInformation
{
public:
	HardwareInformation();
	int64 GetHardwareID();
	bool IsValidHardwareID(std::vector<int64> HardwareIDArray);
private:
	uint16 HashMacAddress(PIP_ADAPTER_INFO info)
	{        
		uint16 nHash = 0;          
		for ( uint32 i = 0; i < info->AddressLength; i++ )   
			nHash += (info->Address[i] << (( i & 1 ) * 8 ));
		return nHash;           
	}

	void GetMacHash(uint16 & MACOffset1, uint16 & MACOffset2)              
	{        
		IP_ADAPTER_INFO AdapterInfo[32];                  
		DWORD dwBufLen = sizeof( AdapterInfo );           

		DWORD dwStatus = GetAdaptersInfo( AdapterInfo, &dwBufLen );                  
		if ( dwStatus != ERROR_SUCCESS )                  
			return;  

		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;      
		MACOffset1 = HashMacAddress( pAdapterInfo );            
		if (pAdapterInfo->Next)
			MACOffset2 = HashMacAddress( pAdapterInfo->Next );   

		if (MACOffset1 > MACOffset2)     
		{     
			uint16 TempOffset = MACOffset2;     
			MACOffset2 = MACOffset1;        
			MACOffset1 = TempOffset;         
		}     
	}        

	uint16 GetCPUHash()          
	{        
		int CPUInfo[4] = { 0, 0, 0, 0 };                  
		__cpuid(CPUInfo, 0);
		uint16 nHash = 0;          
		uint16* nPointer = (uint16*)(&CPUInfo[0]);
		for (uint16 i = 0; i < 8; i++)
			nHash += nPointer[i];     

		return nHash;           
	}

	uint16 GetVolumeHash()       
	{        
		DWORD SerialNum = 0;   
		GetVolumeInformation( "C:\\", NULL, 0, &SerialNum, NULL, NULL, NULL, 0 );    
		uint16 nHash = (uint16)(( SerialNum + ( SerialNum >> 16 )) & 0xFFFF );
		return nHash;           
	}
};