#include "stdafx.h"
#include "MAP.h"
#include "ServerDlg.h"
#include "Region.h"
#include "Npc.h"
#include "User.h"
#include "RoomEvent.h"
#include "../shared/packets.h"
#include <fstream>
#include <set>
#include "../shared/SMDFile.h"

INLINE int ParseSpace( char* tBuf, char* sBuf)
{
	int i = 0, index = 0;
	bool flag = false;

	while (sBuf[index] == ' ' || sBuf[index] == '\t')
		index++;

	while (sBuf[index] !=' ' && sBuf[index] !='\t' && sBuf[index] != 0)
	{
		tBuf[i++] = sBuf[index++];
		flag = true;
	}
	tBuf[i] = 0;

	while (sBuf[index] == ' ' || sBuf[index] == '\t')
		index++;

	return !flag ? 0 : index;
};

using namespace std;

/* passthru methods */
int MAP::GetMapSize() { return m_smdFile->GetMapSize(); }
float MAP::GetUnitDistance() { return m_smdFile->GetUnitDistance(); }
int MAP::GetXRegionMax() { return m_smdFile->GetXRegionMax(); }
int MAP::GetZRegionMax() { return m_smdFile->GetZRegionMax(); }
short * MAP::GetEventIDs() { return m_smdFile->GetEventIDs(); }

MAP::MAP() : m_smdFile(nullptr), m_ppRegion(nullptr),
	m_fHeight(nullptr), m_byRoomType(0), m_byRoomEvent(0),
	m_byRoomStatus(RoomStatusInitialised), m_byInitRoomCount(0),
	m_nZoneNumber(0), m_sKarusRoom(0), m_sElmoradRoom(0)
{
}

bool MAP::Initialize(_ZONE_INFO *pZone)
{
	m_nZoneNumber = pZone->m_nZoneNumber;
	m_byRoomEvent = pZone->m_byRoomEvent;

	m_smdFile = SMDFile::Load(pZone->m_MapName);

	if (m_smdFile != nullptr)
	{
		SetZoneAttributes(m_nZoneNumber);
		
		{ // NOTE: need a second scope because of foreach macro
			foreach_stlmap(itr, m_smdFile->m_ObjectEventArray) {
				_OBJECT_EVENT * pEvent = itr->second;
				_OBJECT_EVENT * pEventMap = new _OBJECT_EVENT;

				pEvent->nIndex  = 0; // NOTE: index into DB table?
				pEvent->sZoneID = m_nZoneNumber;
				pEvent->byLife  = 0; // NOTE: I guess if the "NPC" can take damage?

				*pEventMap = *pEvent;

				// NOTE: not sure how these should be actually indexed
				int iIndex = g_pMain->m_ObjectEventArray.GetSize();
				if (!g_pMain->m_ObjectEventArray.PutData(iIndex, pEventMap)) {
					TRACE("Object Event PutData Fail - %d\n", pEvent->sIndex);
				}
			}
		}

		foreach_stlmap(itr, g_pMain->m_ObjectEventArray)
		{
			if (itr->second->sZoneID == m_nZoneNumber)
			{
				_OBJECT_EVENT * pEvent = itr->second;
				if (pEvent->sType == OBJECT_GATE
					|| pEvent->sType == OBJECT_GATE2
					|| pEvent->sType == OBJECT_GATE_LEVER
					|| pEvent->sType == OBJECT_ANVIL
					|| pEvent->sType == OBJECT_ARTIFACT)
					g_pMain->AddObjectEventNpc(pEvent, this);
			}
		}

		m_ppRegion = new CRegion*[m_smdFile->m_nXRegion];
		for (int i = 0; i < m_smdFile->m_nXRegion; i++)
			m_ppRegion[i] = new CRegion[m_smdFile->m_nZRegion]();
	}

	if (m_byRoomEvent > 0)
	{
		if (!LoadRoomEvent())
		{
			printf("ERROR: Unable to load room event (%d.aievt) for map - %s\n", 
				m_byRoomEvent, pZone->m_MapName.c_str());
			m_byRoomEvent = 0;
		}
		else
		{
			m_byRoomEvent = 1;
		}
	}

	return (m_smdFile != nullptr);
}

MAP::~MAP()
{
	RemoveMapData();

	if (m_smdFile != nullptr)
		m_smdFile->DecRef();
}

void MAP::RemoveMapData()
{
	if( m_ppRegion ) {
		for(int i=0; i <= GetXRegionMax(); i++) {
			delete[] m_ppRegion[i];
			m_ppRegion[i] = nullptr;
		}
		delete[] m_ppRegion;
		m_ppRegion = nullptr;
	}

	if (m_fHeight)
	{
		delete[] m_fHeight;
		m_fHeight = nullptr;
	}

	m_arRoomEventArray.DeleteAllData();
}

bool MAP::IsMovable(int dest_x, int dest_y)
{
	return m_smdFile->GetEventID(dest_x, dest_y) == 0;
}

void MAP::RegionUserAdd(int rx, int rz, int uid)
{
	if (rx < 0 || rz < 0 || rx > GetXRegionMax() || rz > GetZRegionMax())
		return;

	Guard lock(m_lock);
	CRegion * pRegion = &m_ppRegion[rx][rz];

	if (pRegion == nullptr)
		return;

	int *pInt = new int;
	*pInt = uid;
	if (!pRegion->m_RegionUserArray.PutData(uid, pInt))
		delete pInt;

	pRegion->m_byMoving = !pRegion->m_RegionUserArray.IsEmpty();
}

bool MAP::RegionUserRemove(int rx, int rz, int uid)
{
	if (rx < 0 || rz < 0 || rx > GetXRegionMax() || rz > GetZRegionMax())
		return false;

	Guard lock(m_lock);
	CRegion * pRegion = &m_ppRegion[rx][rz];

	if (pRegion == nullptr)
		return false;

	pRegion->m_RegionUserArray.DeleteData(uid);
	pRegion->m_byMoving = !pRegion->m_RegionUserArray.IsEmpty();
	return true;
}

void MAP::RegionNpcAdd(int rx, int rz, int nid)
{
	if (rx < 0 || rz < 0 || rx > GetXRegionMax() || rz > GetZRegionMax())
		return;

	Guard lock(m_lock);
	int *pInt = new int;
	*pInt = nid;
	if (!m_ppRegion[rx][rz].m_RegionNpcArray.PutData(nid, pInt))
		delete pInt;
}

bool MAP::RegionNpcRemove(int rx, int rz, int nid)
{
	if (rx < 0 || rz < 0 || rx > GetXRegionMax() || rz > GetZRegionMax())
		return false;

	Guard lock(m_lock);
	m_ppRegion[rx][rz].m_RegionNpcArray.DeleteData( nid );
	return true;
}


CRegion * MAP::GetRegion(uint16 regionX, uint16 regionZ)
{
	if (regionX > GetXRegionMax()
		|| regionZ > GetZRegionMax())
		return nullptr;

	Guard lock(m_lock);
	return &m_ppRegion[regionX][regionZ];
}

bool MAP::LoadRoomEvent()
{
	uint32		length, count;
	string		filename = string_format(MAP_DIR "%d.evt", m_byRoomEvent);
	char		byte;
	char		buf[4096];
	char		first[1024];
	char		temp[1024];
	int			index = 0;
	int			t_index = 0, logic=0, exec=0;
	int			event_num = 0, nation = 0;

	CRoomEvent*	pEvent = nullptr;
	ifstream is(filename);
	if (!is)
	{
		printf("ERROR: %s does not exist or no permission to access.\n", filename.c_str());
		return false;
	}

	is.seekg(0, is.end);
	length = (uint32)is.tellg();
	is.seekg (0, is.beg);

	count = 0;

	while (count < length)
	{
		is.read(&byte, 1);
		count ++;

		if( byte != '\r' && byte != '\n' ) buf[index++] = byte;

		if((byte == '\n' || count == length ) && index > 1 )	{
			buf[index] = (uint8) 0;
			t_index = 0;

			if( buf[t_index] == ';' || buf[t_index] == '/' )	{		// 주석에 대한 처리
				index = 0;
				continue;
			}

			t_index += ParseSpace( first, buf + t_index );

			if( !strcmp( first, "ROOM" ) )	{
				logic = 0; exec = 0;
				t_index += ParseSpace( temp, buf + t_index );	event_num = atoi( temp );

				if( m_arRoomEventArray.IsExist(event_num) )	{
					TRACE("Event Double !!\n" );
					goto cancel_event_load;
				}

				pEvent = nullptr;
				pEvent = SetRoomEvent( event_num );
			}
			else if( !strcmp( first, "TYPE" ) )	{
				t_index += ParseSpace( temp, buf + t_index );	m_byRoomType = atoi( temp );
			}
			else if( !strcmp( first, "L" ) )	{
				if( !pEvent )	{
					goto cancel_event_load;
				}
			}
			else if( !strcmp( first, "E" ) )	{
				if (!pEvent
					|| exec >= MAX_CHECK_EVENT)
					goto cancel_event_load;

				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_Exec[exec].sNumber = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_Exec[exec].sOption_1 = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_Exec[exec].sOption_2 = atoi( temp );
				exec++;
			}
			else if( !strcmp( first, "A" ) )	{
				if (!pEvent
					|| logic >= MAX_CHECK_EVENT)
					goto cancel_event_load;

				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_Logic[logic].sNumber = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_Logic[logic].sOption_1 = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_Logic[logic].sOption_2 = atoi( temp );
				logic++;
				pEvent->m_byCheck = logic;
			}
			else if( !strcmp( first, "O" ) )	{
				if( !pEvent )	{
					goto cancel_event_load;
				}
			}
			else if( !strcmp( first, "NATION" ) )	{
				if( !pEvent )	{
					goto cancel_event_load;
				}

				t_index += ParseSpace( temp, buf + t_index );	nation = atoi( temp );
				if( nation == KARUS )	{
					m_sKarusRoom++;
				}
				else if( nation == ELMORAD )	{
					m_sElmoradRoom++;
				}
			}
			else if( !strcmp( first, "POS" ) )	{
				if( !pEvent )	{
					goto cancel_event_load;
				}

				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iInitMinX = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iInitMinZ = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iInitMaxX = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iInitMaxZ = atoi( temp );
			}
			else if( !strcmp( first, "POSEND" ) )	{
				if( !pEvent )	{
					goto cancel_event_load;
				}

				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iEndMinX = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iEndMinZ = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iEndMaxX = atoi( temp );
				t_index += ParseSpace( temp, buf + t_index );	pEvent->m_iEndMaxZ = atoi( temp );
			}
			else if( !strcmp( first, "END" ) )	{
				if( !pEvent )	{
					goto cancel_event_load;
				}
			}

			index = 0;
		}
	}

	is.close();

	return true;

cancel_event_load:
	printf("Unable to load AI EVT (%d.aievt), failed in or near event number %d.\n", 
		m_byRoomEvent, event_num);
	is.close();
	return false;
}

int MAP::IsRoomCheck(float fx, float fz)
{
	int nX = (int)fx, nZ = (int)fz;
	int minX=0, minZ=0, maxX=0, maxZ=0;
	int room_number = 0;

	foreach_stlmap (itr, m_arRoomEventArray)		
	{
		CRoomEvent * pRoom = itr->second;
		if (pRoom == nullptr
			|| pRoom->isCleared())
			continue;

		if (pRoom->isInitialised())
		{
			minX = pRoom->m_iInitMinX;		minZ = pRoom->m_iInitMinZ;
			maxX = pRoom->m_iInitMaxX;		maxZ = pRoom->m_iInitMaxZ;
		}
		else // if (pRoom->isInProgress())
		{
			if (pRoom->m_Logic[0].sNumber != 4)	
				continue;

			minX = pRoom->m_iEndMinX;		minZ = pRoom->m_iEndMinZ;
			maxX = pRoom->m_iEndMaxX;		maxZ = pRoom->m_iEndMaxZ;
		}

		CRect r(minX, minZ, maxX, maxZ);
		if (r.PtInRect(nX, nZ))
		{
			if (pRoom->isInitialised())	
			{
				pRoom->m_byStatus = RoomStatusInProgress;
				pRoom->m_tDelayTime = UNIXTIME;
				room_number = itr->first;
				TRACE(" Room Check - number = %d, x=%d, z=%d\n", room_number, nX, nZ);
				//wsprintf(notify, "** 알림 : [%d Zone][%d] 방에 들어오신것을 환영합니다 **", m_nZoneNumber, pRoom->m_sRoomNumber);
				//g_pMain->SendSystemMsg(notify, PUBLIC_CHAT);
			}
			else // room already in progress
			{
				pRoom->m_byStatus = RoomStatusCleared;
				//wsprintf(notify, "** 알림 : [%d Zone][%d] 목표지점까지 도착해서 클리어 됩니다ㅇ **", m_nZoneNumber, pRoom->m_sRoomNumber);
				//g_pMain->SendSystemMsg(notify, PUBLIC_CHAT);
			}

			return room_number;	
		}
	}

	return room_number;
}

CRoomEvent* MAP::SetRoomEvent( int number )
{
	CRoomEvent* pEvent = m_arRoomEventArray.GetData( number );
	if( pEvent )	{
		TRACE("#### SetRoom Error : double event number = %d ####\n", number);
		return nullptr;
	}

	pEvent = new CRoomEvent();
	pEvent->m_iZoneNumber = m_nZoneNumber;
	pEvent->m_sRoomNumber = number;
	if( !m_arRoomEventArray.PutData( pEvent->m_sRoomNumber, pEvent) ) {
		delete pEvent;
		pEvent = nullptr;
		return nullptr;
	}

	return pEvent;
}

bool MAP::IsRoomStatusCheck()
{
	int nClearRoom = 1,
		nTotalRoom = m_arRoomEventArray.GetSize() + 1;

	if (m_byRoomStatus == RoomStatusInProgress)
		m_byInitRoomCount++;

	foreach_stlmap (itr, m_arRoomEventArray)
	{
		CRoomEvent *pRoom = itr->second;
		if (pRoom == nullptr)
		{
			TRACE("#### IsRoomStatusCheck Error : room empty number = %d ####\n", itr->first);
			continue;
		}

		if (m_byRoomStatus == RoomStatusInitialised)
		{
			if (pRoom->isCleared())	
				nClearRoom += 1;

			if (m_byRoomType == 0)	
			{
				if (nTotalRoom == nClearRoom)
				{
					m_byRoomStatus = RoomStatusInProgress;
					return true;
				}
			}
		}
		else if (m_byRoomStatus == RoomStatusInProgress)
		{
			if (m_byInitRoomCount >= 10)
			{
				pRoom->InitializeRoom();
				nClearRoom += 1;
				if (nTotalRoom == nClearRoom)
				{
					m_byRoomStatus = RoomStatusCleared;
					return true;
				}
			}
		}
		else if (m_byRoomStatus == RoomStatusCleared)
		{
			m_byRoomStatus = RoomStatusInitialised;
			m_byInitRoomCount = 0;
			return true;
		}
	}
	return false;
}

void MAP::InitializeRoom()
{
	foreach_stlmap (itr, m_arRoomEventArray)
	{
		CRoomEvent *pRoom = itr->second;
		if (pRoom == nullptr)
		{
			TRACE("#### InitializeRoom Error : room empty number = %d ####\n", itr->first);
			continue;
		}

		pRoom->InitializeRoom();
		m_byRoomStatus = RoomStatusInitialised;
		m_byInitRoomCount = 0;
	}
}