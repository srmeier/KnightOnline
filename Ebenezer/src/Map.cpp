// Map.cpp: implementation of the CMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Ebenezer.h"
#include "Map.h"
#include "Region.h"
#include "Define.h"
#include "User.h"
#include "EventSet.h"
#include "EbenezerDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//extern CRITICAL_SECTION g_region_critical;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C3DMap::C3DMap()
{
	m_nMapSize = 0;
	m_fUnitDist = 0.0f;
	m_fHeight = NULL;

	m_nXRegion = 0;
	m_nZRegion = 0;

	m_ppRegion = NULL;
	m_nZoneNumber = 0;
	m_bType = 0;
	m_wBundle = 1;
	m_sMaxUser = 150;	// Max user in Battlezone!!!
	memset(m_MapName, NULL, 256);
	m_pMain = NULL;
}

C3DMap::~C3DMap()
{
	if (m_ppRegion) {
		for (int i = 0; i< m_nXRegion; i++) {
			delete[] m_ppRegion[i];
			m_ppRegion[i] = NULL;
		}
		delete[] m_ppRegion;
		m_ppRegion = NULL;
	}

	if (m_fHeight) {
		for (int i = 0; i< m_nMapSize; i++) {
			delete[] m_fHeight[i];
			m_fHeight[i] = NULL;
		}
		delete[] m_fHeight;
	}

	if (m_ppnEvent) {
		for (int i = 0; i<m_nMapSize; i++) {
			delete[] m_ppnEvent[i];
			m_ppnEvent[i] = NULL;
		}
		delete[] m_ppnEvent;
		m_ppnEvent = NULL;
	}

	if (!m_EventArray.IsEmpty())
		m_EventArray.DeleteAllData();
	if (!m_ObjectEventArray.IsEmpty())
		m_ObjectEventArray.DeleteAllData();
	if (!m_ObjectRegeneArray.IsEmpty())
		m_ObjectRegeneArray.DeleteAllData();
	if (!m_WarpArray.IsEmpty())
		m_WarpArray.DeleteAllData();

	//DeleteCriticalSection(&g_region_critical);
}

BOOL C3DMap::LoadMap(HANDLE hFile)
{
	m_pMain = (CEbenezerDlg*)AfxGetMainWnd();

	LogFileWrite("load teerr\r\n");

	LoadTerrain(hFile);
	m_N3ShapeMgr.Create((m_nMapSize - 1)*m_fUnitDist, (m_nMapSize - 1)*m_fUnitDist);
	if (!m_N3ShapeMgr.LoadCollisionData(hFile))
		return FALSE;

	if ((m_nMapSize - 1) * m_fUnitDist != m_N3ShapeMgr.Width() ||
		(m_nMapSize - 1) * m_fUnitDist != m_N3ShapeMgr.Height())
	{
		return FALSE;
	}

	LogFileWrite("mapfile adfasfdasdd\r\n");
	int mapwidth = (int)m_N3ShapeMgr.Width();

	m_nXRegion = (int)(mapwidth / VIEW_DISTANCE) + 1;
	m_nZRegion = (int)(mapwidth / VIEW_DISTANCE) + 1;

	m_ppRegion = new CRegion*[m_nXRegion];
	for (int i = 0; i<m_nXRegion; i++) {
		m_ppRegion[i] = new CRegion[m_nZRegion];
	}

	LoadObjectEvent(hFile);
	LogFileWrite("amp tile\r\n");
	LoadMapTile(hFile);
	LogFileWrite("regene\r\n");
	LoadRegeneEvent(hFile);		// 이건 내가 추가했슴
	LogFileWrite("warplist\r\n");
	LoadWarpList(hFile);

	LogFileWrite("load event before\r\n");
	if (!LoadEvent()) {
		AfxMessageBox("Event Load Fail!!");
		return FALSE;
	}

	//InitializeCriticalSection(&g_region_critical);

	return TRUE;
}

void C3DMap::LoadObjectEvent(HANDLE hFile)
{
	int 	iEventObjectCount = 0;
	__Vector3 vPos(0, 0, 0);
	DWORD	dwNum;
	_OBJECT_EVENT* pEvent = NULL;

	BYTE regene_point_counter_karus = 0;
	BYTE regene_point_counter_elmo = 0;

	ReadFile(hFile, &iEventObjectCount, 4, &dwNum, NULL);

	for (int i = 0; i<iEventObjectCount; i++)
	{
		pEvent = new _OBJECT_EVENT;
		ReadFile(hFile, &(pEvent->sBelong), 4, &dwNum, NULL);				// 소속 : 0 -> 무소속
		ReadFile(hFile, &(pEvent->sIndex), 2, &dwNum, NULL);				// Event Index
		ReadFile(hFile, &(pEvent->sType), 2, &dwNum, NULL);					// 0 : bind point, 1,2 : gate, 3 : lever, 4 : flag lever, 5 : Warp point
		ReadFile(hFile, &(pEvent->sControlNpcID), 2, &dwNum, NULL);
		ReadFile(hFile, &(pEvent->sStatus), 2, &dwNum, NULL);
		ReadFile(hFile, &(pEvent->fPosX), 4, &dwNum, NULL);
		ReadFile(hFile, &(pEvent->fPosY), 4, &dwNum, NULL);
		ReadFile(hFile, &(pEvent->fPosZ), 4, &dwNum, NULL);
		pEvent->byLife = 1;

		if (pEvent->sIndex <= 0) continue;
		if (!m_ObjectEventArray.PutData(pEvent->sIndex, pEvent)) {
			TRACE("Object Event PutData Fail - %d\n", pEvent->sIndex);
			delete pEvent;
			pEvent = NULL;
		}

		//		TRACE ("성래 요청 : %d , %d , %d , %d , %d \r\n", pEvent->sBelong, pEvent->sIndex, pEvent->sType, pEvent->sControlNpcID, pEvent->sStatus);
	}
}

void C3DMap::LoadMapTile(HANDLE hFile)
{
	DWORD dwNum;

	m_ppnEvent = new short*[m_nMapSize];
	for (int a = 0; a<m_nMapSize; a++)
		m_ppnEvent[a] = new short[m_nMapSize];
	for (int x = 0; x<m_nMapSize; x++)
		ReadFile(hFile, m_ppnEvent[x], sizeof(short)*m_nMapSize, &dwNum, NULL);
}

void C3DMap::LoadRegeneEvent(HANDLE hFile)
{
	int iEventObjectCount = 0;

	__Vector3 vPos(0, 0, 0);
	DWORD	dwNum;
	_REGENE_EVENT* pEvent = NULL;

	ReadFile(hFile, &iEventObjectCount, 4, &dwNum, NULL);

	for (int i = 0; i<iEventObjectCount; i++)
	{
		pEvent = new _REGENE_EVENT;

		pEvent->sRegenePoint = i;
		ReadFile(hFile, &(pEvent->fRegenePosX), 4, &dwNum, NULL);	// 캐릭터 나타나는 지역의 왼아래쪽 구석 좌표 X
		ReadFile(hFile, &(pEvent->fRegenePosY), 4, &dwNum, NULL);	// 캐릭터 나타나는 지역의 왼아래쪽 구석 좌표 Y
		ReadFile(hFile, &(pEvent->fRegenePosZ), 4, &dwNum, NULL);	// 캐릭터 나타나는 지역의 왼아래쪽 구석 좌표 Z
		ReadFile(hFile, &(pEvent->fRegeneAreaZ), 4, &dwNum, NULL);	// 캐릭터 나타나는 지역의 Z 축 길이 
		ReadFile(hFile, &(pEvent->fRegeneAreaX), 4, &dwNum, NULL);	// 캐릭터 나타나는 지역의 X 축 길이

		if (pEvent->sRegenePoint < 0) continue;

		if (!m_ObjectRegeneArray.PutData(pEvent->sRegenePoint, pEvent)) {
			TRACE("Regene Event PutData Fail - %d\n", pEvent->sRegenePoint);
			delete pEvent;
			pEvent = NULL;
		}

		// TRACE("Num: %d , (x, z, y) : (%f, %f, %f) , length_x : %f  length_z : %f\n", i,
		// pEvent->fRegenePosX, pEvent->fRegenePosZ, pEvent->fRegenePosY,
		// pEvent->fRegeneAreaX, pEvent->fRegeneAreaZ);
	}

	//TRACE("\n\n");

	//	m_pMain->m_bMaxRegenePoint = iEventObjectCount;
}

void C3DMap::LoadWarpList(HANDLE hFile)
{
	int WarpCount = 0;
	DWORD	dwNum;
	_WARP_INFO* pWarp = NULL;

	ReadFile(hFile, &WarpCount, 4, &dwNum, NULL);

	for (int i = 0; i<WarpCount; i++) {
		pWarp = new _WARP_INFO;

		ReadFile(hFile, pWarp, sizeof(_WARP_INFO), &dwNum, NULL);

		if (!m_WarpArray.PutData(pWarp->sWarpID, pWarp)) {
			TRACE("Warp list PutData Fail - %d\n", pWarp->sWarpID);
			delete pWarp;
			pWarp = NULL;
		}
	}
}

void C3DMap::LoadTerrain(HANDLE hFile)
{
	DWORD dwRWC;
	ReadFile(hFile, &m_nMapSize, sizeof(int), &dwRWC, NULL);	// 가로세로 정보가 몇개씩인가?
	ReadFile(hFile, &m_fUnitDist, sizeof(float), &dwRWC, NULL);

	m_fHeight = new float*[m_nMapSize];
	for (int i = 0; i<m_nMapSize; i++) {
		m_fHeight[i] = new float[m_nMapSize];
	}

	int x, z;
	for (z = 0; z<m_nMapSize; z++)
	{
		for (x = 0; x<m_nMapSize; x++)
		{
			ReadFile(hFile, &(m_fHeight[x][z]), sizeof(float), &dwRWC, NULL);	// 높이값 읽어오기
		}
	}
}

float C3DMap::GetHeight(float x, float y, float z)
{
	int iX, iZ;
	iX = (int)(x / m_fUnitDist);
	iZ = (int)(z / m_fUnitDist);
	//_ASSERT( iX, iZ가 범위내에 있는 값인지 체크하기);

	float fYTerrain;
	float h1, h2, h3;
	float dX, dZ;
	dX = (x - iX*m_fUnitDist) / m_fUnitDist;
	dZ = (z - iZ*m_fUnitDist) / m_fUnitDist;

	//	_ASSERT(dX>=0.0f && dZ>=0.0f && dX<1.0f && dZ<1.0f);
	if (!(dX >= 0.0f && dZ >= 0.0f && dX<1.0f && dZ<1.0f))
		return FLT_MIN;

	if ((iX + iZ) % 2 == 1)
	{
		if ((dX + dZ) < 1.0f)
		{
			h1 = m_fHeight[iX][iZ + 1];
			h2 = m_fHeight[iX + 1][iZ];
			h3 = m_fHeight[iX][iZ];

			//if (dX == 1.0f) return h2;

			float h12 = h1 + (h2 - h1)*dX;	// h1과 h2사이의 높이값
			float h32 = h3 + (h2 - h3)*dX;	// h3과 h2사이의 높이값
			fYTerrain = h32 + (h12 - h32)*((dZ) / (1.0f - dX));	// 찾고자 하는 높이값
		}
		else
		{
			h1 = m_fHeight[iX][iZ + 1];
			h2 = m_fHeight[iX + 1][iZ];
			h3 = m_fHeight[iX + 1][iZ + 1];

			if (dX == 0.0f) return h1;

			float h12 = h1 + (h2 - h1)*dX;	// h1과 h2사이의 높이값
			float h13 = h1 + (h3 - h1)*dX;	// h1과 h3사이의 높이값
			fYTerrain = h13 + (h12 - h13)*((1.0f - dZ) / (dX));	// 찾고자 하는 높이값
		}
	}
	else
	{
		if (dZ > dX)
		{
			h1 = m_fHeight[iX][iZ + 1];
			h2 = m_fHeight[iX + 1][iZ + 1];
			h3 = m_fHeight[iX][iZ];

			//if (dX == 1.0f) return h2;

			float h12 = h1 + (h2 - h1)*dX;	// h1과 h2사이의 높이값
			float h32 = h3 + (h2 - h3)*dX;	// h3과 h2사이의 높이값
			fYTerrain = h12 + (h32 - h12)*((1.0f - dZ) / (1.0f - dX));	// 찾고자 하는 높이값
		}
		else
		{
			h1 = m_fHeight[iX][iZ];
			h2 = m_fHeight[iX + 1][iZ];
			h3 = m_fHeight[iX + 1][iZ + 1];

			if (dX == 0.0f) return h1;

			float h12 = h1 + (h2 - h1)*dX;	// h1과 h2사이의 높이값
			float h13 = h1 + (h3 - h1)*dX;	// h1과 h3사이의 높이값
			fYTerrain = h12 + (h13 - h12)*((dZ) / (dX));	// 찾고자 하는 높이값
		}
	}

	__Vector3 vPos(x, y, z);
	float fHeight = m_N3ShapeMgr.GetHeightNearstPos(vPos, 0.0f, NULL); // 가장 가까운 높이값을 돌려준다..
	if (-FLT_MAX != fHeight && fHeight > fYTerrain) return fHeight;
	else return fYTerrain;
}

BOOL C3DMap::ObjectCollision(float x1, float z1, float y1, float x2, float z2, float y2)
{
	__Vector3	vec1(x1, y1, z1), vec2(x2, y2, z2);
	__Vector3	vDir = vec2 - vec1;
	float fSpeed = vDir.Magnitude();
	vDir.Normalize();

	return m_N3ShapeMgr.CheckCollision(vec1, vDir, fSpeed);
}

BOOL C3DMap::RegionItemAdd(int rx, int rz, _ZONE_ITEM* pItem)
{
	if (rx<0 || rz<0 || rx >= m_nXRegion || rz >= m_nZRegion)
		return FALSE;
	if (!pItem)
		return FALSE;

	//EnterCriticalSection(&g_region_critical);

	m_ppRegion[rx][rz].m_RegionItemArray.PutData(pItem->bundle_index, pItem);

	m_wBundle++;
	if (m_wBundle > ZONEITEM_MAX)
		m_wBundle = 1;

	//LeaveCriticalSection(&g_region_critical);

	return TRUE;
}

BOOL C3DMap::RegionItemRemove(int rx, int rz, int bundle_index, int itemid, int count)
{
	if (rx<0 || rz<0 || rx >= m_nXRegion || rz >= m_nZRegion)
		return FALSE;

	_ZONE_ITEM* pItem = NULL;
	CRegion	*region = NULL;
	BOOL bFind = FALSE;
	short t_count = 0;

	//EnterCriticalSection(&g_region_critical);

	region = &m_ppRegion[rx][rz];
	pItem = (_ZONE_ITEM*)region->m_RegionItemArray.GetData(bundle_index);
	if (pItem) {
		for (int j = 0; j<6; j++) {
			if (pItem->itemid[j] == itemid && pItem->count[j] == count) {
				pItem->itemid[j] = 0; pItem->count[j] = 0;
				bFind = TRUE;
				break;
			}
		}
		if (bFind) {
			for (int j = 0; j<6; j++) {
				if (pItem->itemid[j] != 0)
					t_count++;
			}
			if (!t_count)
				region->m_RegionItemArray.DeleteData(bundle_index);
		}
	}

	//LeaveCriticalSection(&g_region_critical);

	return bFind;
}

void C3DMap::RegionUserAdd(int rx, int rz, int uid)
{
	if (rx<0 || rz<0 || rx >= m_nXRegion || rz >= m_nZRegion)
		return;

	int *pInt = NULL;

	//EnterCriticalSection(&g_region_critical);

	pInt = new int;
	*pInt = uid;
	m_ppRegion[rx][rz].m_RegionUserArray.PutData(uid, pInt);

	//TRACE("++++ Region Add(%d) : x=%d, z=%d, uid=%d ++++\n", m_nZoneNumber, rx, rz, uid);

	//LeaveCriticalSection(&g_region_critical);
}

Iterator C3DMap::RegionUserRemove(int rx, int rz, int uid)
{
	if (rx < 0 || rz < 0 || rx >= m_nXRegion || rz >= m_nZRegion) {
		map < int, int* >::iterator		Iter;
		return Iter;
	}

	CRegion	*region = NULL;
	map < int, int* >::iterator		Iter;

	//EnterCriticalSection(&g_region_critical);

	region = &m_ppRegion[rx][rz];
	Iter = region->m_RegionUserArray.DeleteData(uid);

	//TRACE("---- Region Remove(%d) : x=%d, z=%d, uid=%d ----\n", m_nZoneNumber, rx, rz, uid);

	//LeaveCriticalSection(&g_region_critical);

	return Iter;
}

void C3DMap::RegionNpcAdd(int rx, int rz, int nid)
{
	if (rx<0 || rz<0 || rx >= m_nXRegion || rz >= m_nZRegion)
		return;

	int *pInt = NULL;

	//EnterCriticalSection(&g_region_critical);

	pInt = new int;
	*pInt = nid;
	m_ppRegion[rx][rz].m_RegionNpcArray.PutData(nid, pInt);

	//LeaveCriticalSection(&g_region_critical);
}

Iterator C3DMap::RegionNpcRemove(int rx, int rz, int nid)
{
	if (rx < 0 || rz < 0 || rx >= m_nXRegion || rz >= m_nZRegion) {
		map < int, int* >::iterator		Iter;
		return Iter;
	}

	CRegion	*region = NULL;
	map < int, int* >::iterator		Iter;

	//EnterCriticalSection(&g_region_critical);

	region = &m_ppRegion[rx][rz];
	Iter = region->m_RegionNpcArray.DeleteData(nid);

	//LeaveCriticalSection(&g_region_critical);

	return Iter;
}

BOOL C3DMap::CheckEvent(float x, float z, CUser* pUser)
{
	CGameEvent* pEvent = NULL;
	int iX, iZ, event_index = 0;

	iX = (int)(x / m_fUnitDist);
	iZ = (int)(z / m_fUnitDist);
	if (iX < 0 || iX >= m_nMapSize || iZ < 0 || iZ >= m_nMapSize)
		return FALSE;

	event_index = m_ppnEvent[iX][iZ];
	if (event_index < 2)
		return FALSE;

	pEvent = m_EventArray.GetData(event_index);
	if (pEvent) {
		if (pEvent->m_bType == 1 && pEvent->m_iExec[0] == ZONE_BATTLE && m_pMain->m_byBattleOpen != NATION_BATTLE) return FALSE;
		if (pEvent->m_bType == 1 && pEvent->m_iExec[0] == ZONE_SNOW_BATTLE && m_pMain->m_byBattleOpen != SNOW_BATTLE) return FALSE;
		if (pUser->m_pUserData->m_bNation == KARUS && pEvent->m_iExec[0] == ZONE_BATTLE) {
			if (m_pMain->m_sKarusCount > MAX_BATTLE_ZONE_USERS) {
				TRACE("### BattleZone karus full users = %d, name=%s \n", m_pMain->m_sKarusCount, pUser->m_pUserData->m_id);
				return FALSE;
			}
		}
		else if (pUser->m_pUserData->m_bNation == ELMORAD && pEvent->m_iExec[0] == ZONE_BATTLE) {
			if (m_pMain->m_sElmoradCount > MAX_BATTLE_ZONE_USERS) {
				TRACE("### BattleZone elmorad full users = %d, name=%s \n", m_pMain->m_sElmoradCount, pUser->m_pUserData->m_id);
				return FALSE;
			}
		}
		pEvent->RunEvent(pUser);
		return TRUE;
	}

	return FALSE;
}

BOOL C3DMap::LoadEvent()
{
	CEventSet	EventSet;
	CGameEvent* pEvent = NULL;

	LogFileWrite("LoadEvent start \r\n");

	if (!EventSet.Open()) {
		LogFileWrite("LoadEvent 22 \r\n");
		AfxMessageBox(_T("EventTable Open Fail!"));
		return FALSE;
	}
	if (EventSet.IsBOF() || EventSet.IsEOF()) {
		LogFileWrite("LoadEvent 33 \r\n");
		AfxMessageBox(_T("EventTable Empty!"));
		return FALSE;
	}
	EventSet.MoveFirst();

	while (!EventSet.IsEOF()) {
		if (EventSet.m_ZoneNum == m_nZoneNumber) {
			pEvent = new CGameEvent;

			pEvent->m_sIndex = EventSet.m_EventNum;
			pEvent->m_bType = EventSet.m_Type;
			pEvent->m_iCond[0] = atoi(EventSet.m_Cond1);
			pEvent->m_iCond[1] = atoi(EventSet.m_Cond2);
			pEvent->m_iCond[2] = atoi(EventSet.m_Cond3);
			pEvent->m_iCond[3] = atoi(EventSet.m_Cond4);
			pEvent->m_iCond[4] = atoi(EventSet.m_Cond5);

			pEvent->m_iExec[0] = atoi(EventSet.m_Exec1);
			pEvent->m_iExec[1] = atoi(EventSet.m_Exec2);
			pEvent->m_iExec[2] = atoi(EventSet.m_Exec3);
			pEvent->m_iExec[3] = atoi(EventSet.m_Exec4);
			pEvent->m_iExec[4] = atoi(EventSet.m_Exec5);

			strcpy(pEvent->m_strExec[0], EventSet.m_Exec1);
			strcpy(pEvent->m_strExec[1], EventSet.m_Exec2);
			strcpy(pEvent->m_strExec[2], EventSet.m_Exec3);
			strcpy(pEvent->m_strExec[3], EventSet.m_Exec4);
			strcpy(pEvent->m_strExec[4], EventSet.m_Exec5);

			if (!m_EventArray.PutData(pEvent->m_sIndex, pEvent)) {
				TRACE("Event PutData Fail - %d\n", pEvent->m_sIndex);
				delete pEvent;
				pEvent = NULL;
			}
		}
		EventSet.MoveNext();
	}

	LogFileWrite("LoadEvent 44 \r\n");
	return TRUE;
}

BOOL C3DMap::IsValidPosition(float x, float z, float y)
{
	if (x >= m_N3ShapeMgr.Width()) return FALSE;
	if (z >= m_N3ShapeMgr.Width()) return FALSE;

	return TRUE;
}
