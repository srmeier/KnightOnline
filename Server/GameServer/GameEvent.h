#pragma once

enum GameEventType
{
	ZONE_CHANGE		= 1,
	ZONE_TRAP_DEAD	= 2,
	ZONE_TRAP_AREA	= 3
};

class CUser;
class CGameEvent
{
public:
	CGameEvent();
	void RunEvent(CUser* pUser = nullptr);

	uint16_t	m_sIndex;
	uint8_t	m_bType;

	int32_t	m_iCond[5], m_iExec[5];
};