#include "stdafx.h"
#include "RoomEvent.h"
#include "resource.h"
#include "Npc.h"

CRoomEvent::CRoomEvent()
{
	m_iZoneNumber = 0;
	m_sRoomNumber = 0;
	m_byStatus	= RoomStatusInitialised;
	m_iInitMinX = 0;
	m_iInitMinZ = 0;
	m_iInitMaxX = 0;
	m_iInitMaxZ = 0;
	m_iEndMinX = 0;
	m_iEndMinZ = 0;
	m_iEndMaxX = 0;
	m_iEndMaxZ = 0;
	m_byCheck = 0;
	m_byRoomType = 0;

	Initialize();
}

CRoomEvent::~CRoomEvent()
{
}

void CRoomEvent::Initialize()
{
	m_tDelayTime = 0;
	m_byLogicNumber = 1;

	for(int i=0; i<MAX_CHECK_EVENT; i++)	{
		m_Logic[i].sNumber = 0;
		m_Logic[i].sOption_1 = 0;
		m_Logic[i].sOption_2 = 0;
		m_Exec[i].sNumber = 0;
		m_Exec[i].sOption_1 = 0;
		m_Exec[i].sOption_2 = 0;
	}
}

void CRoomEvent::MainRoom()
{
	// 조건 검색먼저 해야 겠지..
	bool bCheck = false, bRunCheck = false;
	int event_num  = m_Logic[m_byLogicNumber-1].sNumber;

	bCheck = CheckEvent(event_num);

	if( bCheck )	{
		event_num = m_Exec[m_byLogicNumber-1].sNumber; 
		bRunCheck = RunEvent( event_num );
		if( bRunCheck )	{
			//wsprintf(notify, "** 알림 : [%d]방이 클리어 되어습니다. **", m_sRoomNumber);
			//g_pMain->SendSystemMsg(notify, PUBLIC_CHAT);
			m_byStatus = RoomStatusCleared;
		}
	}
}

bool CRoomEvent::CheckEvent(int event_num)
{
	int nMinute = 0, nOption_1 = 0, nOption_2 = 0;
	CNpc* pNpc = nullptr;
	bool bRetValue = false;

	if( m_byLogicNumber == 0 || m_byLogicNumber > MAX_CHECK_EVENT )	{
		TRACE("### Check Event Fail :: array overflow = %d ###\n", m_byLogicNumber);
		return false;
	}

	switch( event_num )	{
	case 1:					// 특정 몬스터를 죽이는 경우
		nOption_1 = m_Logic[ m_byLogicNumber-1 ].sOption_1;
		pNpc = GetNpcPtr( nOption_1 );
		if( pNpc )	{
			if( pNpc->m_byChangeType == 100 )	return true;
		} else {
			if (nOption_1 != 11510 && nOption_1 != 21510)
				TRACE("### CheckEvent Error : monster nid = %d, logic=%d ###\n", nOption_1, m_byLogicNumber);
		}
		//TRACE("---Check Event : monster dead = %d \n", nMonsterNid);
		break;
	case 2:					// 모든 몬스터를 죽여라
		bRetValue = CheckMonsterCount( 0, 0, 3 );
		if( bRetValue )	{
			TRACE("모든 몬스터를 죽여라 죽임\n");
			return true;
		}
		break;
	case 3:					// 몇분동안 버텨라
		nMinute = m_Logic[ m_byLogicNumber-1 ].sOption_1;
		nMinute = nMinute * 60;								// 분을 초로 변환
		if (UNIXTIME >= m_tDelayTime + nMinute )	{		// 제한시간 종료
			return true;
		}
		break;
	case 4:					// 목표지점까지 이동

		break;
	case 5:					// 특정몬스터를 옵션2의 마리수 만큼 죽여라
		nOption_1 = m_Logic[ m_byLogicNumber-1 ].sOption_1;
		nOption_2 = m_Logic[ m_byLogicNumber-1 ].sOption_2;
		bRetValue = CheckMonsterCount( nOption_1, nOption_2, 1 );
		if( bRetValue )	{
			TRACE("특정몬스터(%d)를 %d마리 죽임\n", nOption_1, nOption_2);
			return true;
		}
		break;
	default:
		TRACE("### Check Event Fail :: event number = %d ###\n", event_num);
		break;
	}

	return false;
}

bool CRoomEvent::RunEvent( int event_num )
{
	CNpc* pNpc = nullptr;
	int nOption_1 = 0, nOption_2 = 0;
	bool bRetValue = false;
	switch( event_num )	{
	case 1:					// 다른 몬스터의 출현
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		pNpc = GetNpcPtr( nOption_1 );
		if( pNpc )	{
			pNpc->m_byChangeType = 3;	// 몬스터 출현해주세여...
			pNpc->SetLive();
		}
		else	{
			TRACE("### RunEvent Error : 몬스터 출현 할 수 없당 = %d, logic=%d ###\n", nOption_1, m_byLogicNumber);
		}
		if( m_byCheck == m_byLogicNumber )	{	// 방이 클리어
			return true;
		}
		else		m_byLogicNumber++;

		break;
	case 2:					// 문이 열림
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		pNpc = GetNpcPtr( nOption_1 );
		if( pNpc )	{

		}
		else	{
			TRACE("### RunEvent Error : 문 담당 몬스터 출현 할 수 없당 = %d, logic=%d ###\n", nOption_1, m_byLogicNumber);
		}

		//wsprintf(notify, "** 알림 : [%d] 문이 열립니다 **", m_sRoomNumber);
		//g_pMain->SendSystemMsg(notify, PUBLIC_CHAT);

		if( m_byCheck == m_byLogicNumber )	{	// 방이 클리어
			return true;
		}
		else		m_byLogicNumber++;

		break;
	case 3:					// 다른 몬스터로 변환
		if( m_byCheck == m_byLogicNumber )	{	// 방이 클리어
			return true;
		}
		break;
	case 4:					// 특정몬스터 옵션2의 마리수만큼 출현
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		nOption_2 = m_Exec[ m_byLogicNumber-1 ].sOption_2;
		bRetValue = CheckMonsterCount( nOption_1, nOption_2, 2 );

		//wsprintf(notify, "** 알림 : [%d, %d] 몬스터 출현 **", nOption_1, nOption_2);
		//g_pMain->SendSystemMsg(notify, PUBLIC_CHAT);

		if( m_byCheck == m_byLogicNumber )	{	// 방이 클리어
			return true;
		}
		else		m_byLogicNumber++;
		break;
	case 100:					// 특정몬스터 옵션2의 마리수만큼 출현
		nOption_1 = m_Exec[ m_byLogicNumber-1 ].sOption_1;
		nOption_2 = m_Exec[ m_byLogicNumber-1 ].sOption_2;

		TRACE("RunEvent - room=%d, option1=%d, option2=%d\n", m_sRoomNumber, nOption_1, nOption_2);
		if( nOption_1 != 0 )	{
			EndEventSay( nOption_1, nOption_2 );
		}
		if( m_byCheck == m_byLogicNumber )	{	// 방이 클리어
			return true;
		}
		else		m_byLogicNumber++;
		break;
	default:
		TRACE("### RunEvent Fail :: event number = %d ###\n", event_num);
		break;
	}

	return false;
}

CNpc* CRoomEvent::GetNpcPtr( int sid )
{
	if (m_mapRoomNpcArray.IsEmpty())	
	{
		if (sid != 11510 && sid != 21510)
			TRACE("### RoomEvent-GetNpcPtr() : monster empty, sid=%d ###\n",sid);

		return nullptr;
	}

	foreach_stlmap (itr, m_mapRoomNpcArray)
	{
		CNpc *pNpc = g_pMain->GetNpcPtr(itr->first);
		if (pNpc == nullptr
			|| pNpc->GetProtoID() != sid)
			continue;

		return pNpc;
	}

	return nullptr;
}

bool  CRoomEvent::CheckMonsterCount( int sid, int count, int type )
{
	int nMonsterCount = 0, nTotalMonster = 0;
	bool bRetValue = false;

	int nMonster = m_mapRoomNpcArray.GetSize();
	if (nMonster == 0)
	{
		if (sid != 11510 && sid != 21510)
			TRACE("### RoomEvent-GetNpcPtr() : monster empty, sid=%d ###\n",sid);

		return nullptr;
	}

	foreach_stlmap (itr, m_mapRoomNpcArray)
	{
		CNpc *pNpc = g_pMain->GetNpcPtr(itr->first);
		if (pNpc == nullptr)
			continue;

		if (type == 4)
		{
			if (pNpc->m_byRegenType == 2)	pNpc->m_byRegenType = 0;
			pNpc->m_byChangeType = 0;
		}
		else if (type == 3)
		{
			if (pNpc->m_byDeadType == 100)	nMonsterCount++;
			if (nMonsterCount == nMonster)	bRetValue = true;
		}
		else if (pNpc->GetProtoID() == sid)
		{
			if (type == 1)
			{
				if (pNpc->m_byChangeType == 100)	nMonsterCount++;
				if (nMonsterCount == count)			bRetValue = true;
			}
			else if (type == 2)
			{
				pNpc->m_byChangeType = 3;		nMonsterCount++;
				if (nMonsterCount == count)		bRetValue = true;
			}
		}
	}

	return bRetValue;
}

void CRoomEvent::InitializeRoom()
{
	m_byStatus	= RoomStatusInitialised;			
	m_tDelayTime = 0;
	m_byLogicNumber = 1;

	CheckMonsterCount(0, 0, 4);	// 몬스터의 m_byChangeType=0으로 초기화 
}

void CRoomEvent::EndEventSay( int option1, int option2 )
{
	std::string buff;

	switch (option1)
	{
	case 1:
		{ 
			switch (option2)
			{
			case 1:
				g_pMain->GetServerResource(IDS_KARUS_CATCH_1, &buff);
				break;
			case 2:
				g_pMain->GetServerResource(IDS_KARUS_CATCH_2, &buff);
				break;
			case 11:
				g_pMain->GetServerResource(IDS_ELMORAD_CATCH_1, &buff);
				break;
			case 12:
				g_pMain->GetServerResource(IDS_ELMORAD_CATCH_2, &buff);
				break;
			}

			g_pMain->SendSystemMsg(buff, WAR_SYSTEM_CHAT);
		} break;

	case 2:
		g_pMain->GetServerResource(IDS_KARUS_PATHWAY + (option2-1), &buff);
		g_pMain->SendSystemMsg(buff, WAR_SYSTEM_CHAT);

		// this is normal, we need to send the following packet as well.

	case 3:
		{
			Packet result(AG_BATTLE_EVENT, uint8(BATTLE_MAP_EVENT_RESULT));
			result << uint8(option2);
			g_pMain->Send(&result);
		} break;
	}
}
