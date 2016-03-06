// EbenezerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EbenezerDlg.h"
//#include "DefineTest.h"
#include "Define.h"
//#include "UserTest.h"
#include "User.h"

#include "ItemTableSet.h"
#include "MagicTableSet.h"
#include "CoefficientSet.h"
#include "ZoneInfoSet.h"

#define GAME_TIME       	100
#define SEND_TIME			200
#define PACKET_CHECK		300
#define ALIVE_TIME			400
#define MARKET_BBS_TIME		1000

// TEMP: access to client sockets
extern CUser* clients[MAX_USER];

WORD	g_increase_serial = 1;

CIOCPort	CEbenezerDlg::m_Iocport;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg dialog

//CEbenezerDlg::CEbenezerDlg(void) {}

/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg message handlers

//-----------------------------------------------------------------------------
bool CEbenezerDlg::OnInitDialog()
{
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDL_GetError());
		return false;
	}
	
	if(!m_Iocport.Init(this, MAX_USER, 1)) return false;

	for(int i=0; i<MAX_USER; ++i) {
		m_Iocport.m_SockArrayInActive[i] = new CUser(this);
	}



	//FILE* file;
	//std::string szFullPath, errormsg;
	/*
	szFullPath.Format(".\\MAP\\%s", "test.smd");
	if (!file.Open(szFullPath, CFile::modeRead))
	{
		errormsg.Format( "파일 Open 실패 - %s\n", szFullPath );
		//AfxMessageBox(errormsg);
		return FALSE;
	}

	if( m_Map.LoadMap( (HANDLE)file.m_hFile ) == FALSE ) {
		errormsg.Format( "Map Load 실패 - %s\n", szFullPath );
		//AfxMessageBox(errormsg);
		return FALSE;
	}

	file.Close();
	*/
	//SetTimer( 1, 5000, NULL );
	for(int i=0; i<5; i++) {
		m_pInitPos[i].initx = 640.0f + 5*i;
		m_pInitPos[i].initz = 590.0f + 5*i;
	}
	//::ResumeThread( m_Iocport.m_hAcceptThread );




	if (!m_Iocport.Listen(_LISTEN_PORT)) return false;

	

	return true;  // return TRUE  unless you set the focus to a control
}

//-----------------------------------------------------------------------------
CUser* CEbenezerDlg::GetUserPtr(const char *userid, BYTE type)
{
	CUser* pUser = NULL;
	BOOL bFind = FALSE;

	if (type == 0x01) {					// Account id check....
		for (int i = 0; i<MAX_USER; i++) {
			pUser = (CUser*)m_Iocport.m_SockArray[i];
			if (pUser) {
				if (!_strnicmp(pUser->m_strAccountID, userid, MAX_ID_SIZE)) {
					bFind = TRUE;
					break;
				}
			}
		}
	}
	else {									// character id check...
		for (int i = 0; i<MAX_USER; i++) {
			pUser = (CUser*)m_Iocport.m_SockArray[i];
			if (pUser) {
				if (!_strnicmp(pUser->m_pUserData->m_id, userid, MAX_ID_SIZE)) {
					bFind = TRUE;
					break;
				}
			}
		}
	}

	if (!bFind) return NULL;

	return pUser;
}

//-----------------------------------------------------------------------------
void CEbenezerDlg::Send_All(char *pBuf, int len, CUser* pExceptUser, int nation)
{
	CUser* pUser = NULL;

	for (int i = 0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if (!pUser)
			continue;
		if (pUser == pExceptUser)
			continue;
		if (pUser->GetState() == STATE_GAMESTART) {
			if (nation == 0)		pUser->Send(pBuf, len);
			else if (nation == pUser->m_pUserData->m_bNation)	pUser->Send(pBuf, len);
		}
	}
}

//-----------------------------------------------------------------------------
void CEbenezerDlg::Send_Region(char *pBuf, int len, int zone, int x, int z, CUser* pExceptUser, bool bDirect)
{
	int zoneindex = GetZoneIndex(zone);
	if (zoneindex == -1)
		return;

	Send_UnitRegion(pBuf, len, zoneindex, x, z, pExceptUser, bDirect);
	Send_UnitRegion(pBuf, len, zoneindex, x - 1, z - 1, pExceptUser, bDirect);	// NW
	Send_UnitRegion(pBuf, len, zoneindex, x, z - 1, pExceptUser, bDirect);		// N
	Send_UnitRegion(pBuf, len, zoneindex, x + 1, z - 1, pExceptUser, bDirect);	// NE
	Send_UnitRegion(pBuf, len, zoneindex, x - 1, z, pExceptUser, bDirect);		// W
	Send_UnitRegion(pBuf, len, zoneindex, x + 1, z, pExceptUser, bDirect);		// E
	Send_UnitRegion(pBuf, len, zoneindex, x - 1, z + 1, pExceptUser, bDirect);	// SW
	Send_UnitRegion(pBuf, len, zoneindex, x, z + 1, pExceptUser, bDirect);		// S
	Send_UnitRegion(pBuf, len, zoneindex, x + 1, z + 1, pExceptUser, bDirect);	// SE
}

//-----------------------------------------------------------------------------
int CEbenezerDlg::GetRegionCount(int x, int z)
{
	CUser* pUser = NULL;
	int count = 0;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && (pUser->GetState() != STATE_GAMESTART) ) {
			if( pUser->m_RegionX == x && pUser->m_RegionZ == z )
				count++;
		}
	}

	return count;
}

//-----------------------------------------------------------------------------
void CEbenezerDlg::UserInOutForMe(CUser *pSendUser)
{
	int send_index = 0, buff_index = 0, i = 0, j = 0, t_count = 0, prev_index = 0;
	C3DMap* pMap = NULL;
	int region_x = -1, region_z = -1, user_count = 0, uid = -1;
	char buff[16384], send_buff[49152];
	memset(buff, NULL, 16384);
	memset(send_buff, NULL, 49152);

	if (!pSendUser) return;
	if (pSendUser->m_iZoneIndex < 0 || pSendUser->m_iZoneIndex >= m_ZoneArray.size()) return;
	pMap = (C3DMap*)m_ZoneArray[pSendUser->m_iZoneIndex];
	if (!pMap) return;

	send_index = 3;		// packet command 와 user_count 를 나중에 셋팅한다...
	region_x = pSendUser->m_RegionX;	region_z = pSendUser->m_RegionZ;			// CENTER
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX - 1;	region_z = pSendUser->m_RegionZ - 1;	// NORTH WEST
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX;	region_z = pSendUser->m_RegionZ - 1;		// NORTH
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	prev_index = buff_index + send_index;
	if (prev_index >= 49152) {
		//TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
		return;
	}
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX + 1;	region_z = pSendUser->m_RegionZ - 1;	// NORTH EAST
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	prev_index = buff_index + send_index;
	if (prev_index >= 49152) {
		//TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
		return;
	}
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX - 1;	region_z = pSendUser->m_RegionZ;		// WEST
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	prev_index = buff_index + send_index;
	if (prev_index >= 49152) {
		//TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
		return;
	}
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX + 1;	region_z = pSendUser->m_RegionZ;		// EAST
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	prev_index = buff_index + send_index;
	if (prev_index >= 49152) {
		//TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
		return;
	}
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX - 1;	region_z = pSendUser->m_RegionZ + 1;	// SOUTH WEST
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	prev_index = buff_index + send_index;
	if (prev_index >= 49152) {
		//TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
		return;
	}
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX;	region_z = pSendUser->m_RegionZ + 1;		// SOUTH
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	prev_index = buff_index + send_index;
	if (prev_index >= 49152) {
		//TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
		return;
	}
	SetString(send_buff, buff, buff_index, send_index);
	memset(buff, NULL, 16384);
	region_x = pSendUser->m_RegionX + 1;	region_z = pSendUser->m_RegionZ + 1;	// SOUTH EAST
	buff_index = GetRegionUserIn(pMap, region_x, region_z, buff, t_count);
	prev_index = buff_index + send_index;
	if (prev_index >= 49152) {
		//TRACE("#### UserInOutForMe - buffer overflow = %d ####\n", prev_index);
		return;
	}
	SetString(send_buff, buff, buff_index, send_index);

	int temp_index = 0;
	SetByte(send_buff, WIZ_REQ_USERIN, temp_index);
	SetShort(send_buff, t_count, temp_index);

	pSendUser->SendCompressingPacket(send_buff, send_index);
}

//-----------------------------------------------------------------------------
BOOL CEbenezerDlg::MapFileLoad()
{
	CFile file;
	CString szFullPath, errormsg, sZoneName;
	C3DMap* pMap = NULL;
	EVENT*	pEvent = NULL;

	CZoneInfoSet	ZoneInfoSet;

	if (!ZoneInfoSet.Open()) {
		//AfxMessageBox(_T("ZoneInfoTable Open Fail!"));
		return FALSE;
	}
	if (ZoneInfoSet.IsBOF() || ZoneInfoSet.IsEOF()) {
		//AfxMessageBox(_T("ZoneInfoTable Empty!"));
		return FALSE;
	}

	m_ZoneArray.reserve(10);

	ZoneInfoSet.MoveFirst();

	while (!ZoneInfoSet.IsEOF())
	{
		sZoneName = ZoneInfoSet.m_strZoneName;
		//sZoneName = "karus_0730.smd";
		szFullPath.Format(".\\MAP\\%s", sZoneName);

		//LogFileWrite("mapfile load\r\n");
		if (!file.Open(szFullPath, CFile::modeRead)) {
			errormsg.Format("File Open Fail - %s\n", szFullPath);
			//AfxMessageBox(errormsg);
			return FALSE;
		}

		pMap = new C3DMap;

		pMap->m_nServerNo = ZoneInfoSet.m_ServerNo;
		pMap->m_nZoneNumber = ZoneInfoSet.m_ZoneNo;
		strcpy(pMap->m_MapName, (char*)(LPCTSTR)sZoneName);
		pMap->m_fInitX = (float)(ZoneInfoSet.m_InitX / 100.0);
		pMap->m_fInitZ = (float)(ZoneInfoSet.m_InitZ / 100.0);
		pMap->m_fInitY = (float)(ZoneInfoSet.m_InitY / 100.0);
		pMap->m_bType = ZoneInfoSet.m_Type;

		if (!pMap->LoadMap((HANDLE)file.m_hFile)) {
			errormsg.Format("Map Load Fail - %s\n", szFullPath);
			AfxMessageBox(errormsg);
			delete pMap;
			return FALSE;
		}
		m_ZoneArray.push_back(pMap);

		// 스트립트를 읽어 들인다.
		//LogFileWrite("before script\r\n");

		pEvent = new EVENT;
		if (!pEvent->LoadEvent(ZoneInfoSet.m_ZoneNo))
		{
			delete pEvent;
			pEvent = NULL;
		}
		if (pEvent) {
			if (!m_Event.PutData(pEvent->m_Zone, pEvent)) {
				delete pEvent;
				pEvent = NULL;
			}
		}

		ZoneInfoSet.MoveNext();

		file.Close();
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
BOOL CEbenezerDlg::LoadItemTable()
{
	CItemTableSet	ItemTableSet;

	if (!ItemTableSet.Open()) {
		//AfxMessageBox(_T("ItemTable Open Fail!"));
		return FALSE;
	}
	if (ItemTableSet.IsBOF() || ItemTableSet.IsEOF()) {
		//AfxMessageBox(_T("ItemTable Empty!"));
		return FALSE;
	}

	ItemTableSet.MoveFirst();

	while (!ItemTableSet.IsEOF())
	{
		_ITEM_TABLE* pTableItem = new _ITEM_TABLE;

		pTableItem->m_iNum = ItemTableSet.m_Num;
		strcpy(pTableItem->m_strName, ItemTableSet.m_strName);
		pTableItem->m_bKind = ItemTableSet.m_Kind;
		pTableItem->m_bSlot = ItemTableSet.m_Slot;
		pTableItem->m_bRace = ItemTableSet.m_Race;
		pTableItem->m_bClass = ItemTableSet.m_Class;
		pTableItem->m_sDamage = ItemTableSet.m_Damage;
		pTableItem->m_sDelay = ItemTableSet.m_Delay;
		pTableItem->m_sRange = ItemTableSet.m_Range;
		pTableItem->m_sWeight = ItemTableSet.m_Weight;
		pTableItem->m_sDuration = ItemTableSet.m_Duration;
		pTableItem->m_iBuyPrice = ItemTableSet.m_BuyPrice;
		pTableItem->m_iSellPrice = ItemTableSet.m_SellPrice;
		pTableItem->m_sAc = ItemTableSet.m_Ac;
		pTableItem->m_bCountable = ItemTableSet.m_Countable;
		pTableItem->m_iEffect1 = ItemTableSet.m_Effect1;
		pTableItem->m_iEffect2 = ItemTableSet.m_Effect2;
		pTableItem->m_bReqLevel = ItemTableSet.m_ReqLevel;
		pTableItem->m_bReqRank = ItemTableSet.m_ReqRank;
		pTableItem->m_bReqTitle = ItemTableSet.m_ReqTitle;
		pTableItem->m_bReqStr = ItemTableSet.m_ReqStr;
		pTableItem->m_bReqSta = ItemTableSet.m_ReqSta;
		pTableItem->m_bReqDex = ItemTableSet.m_ReqDex;
		pTableItem->m_bReqIntel = ItemTableSet.m_ReqIntel;
		pTableItem->m_bReqCha = ItemTableSet.m_ReqCha;
		pTableItem->m_bSellingGroup = ItemTableSet.m_SellingGroup;
		pTableItem->m_ItemType = ItemTableSet.m_ItemType;
		pTableItem->m_sHitrate = ItemTableSet.m_Hitrate;
		pTableItem->m_sEvarate = ItemTableSet.m_Evasionrate;
		pTableItem->m_sDaggerAc = ItemTableSet.m_DaggerAc;
		pTableItem->m_sSwordAc = ItemTableSet.m_SwordAc;
		pTableItem->m_sMaceAc = ItemTableSet.m_MaceAc;
		pTableItem->m_sAxeAc = ItemTableSet.m_AxeAc;
		pTableItem->m_sSpearAc = ItemTableSet.m_SpearAc;
		pTableItem->m_sBowAc = ItemTableSet.m_BowAc;
		pTableItem->m_bFireDamage = ItemTableSet.m_FireDamage;
		pTableItem->m_bIceDamage = ItemTableSet.m_IceDamage;
		pTableItem->m_bLightningDamage = ItemTableSet.m_LightningDamage;
		pTableItem->m_bPoisonDamage = ItemTableSet.m_PoisonDamage;
		pTableItem->m_bHPDrain = ItemTableSet.m_HPDrain;
		pTableItem->m_bMPDamage = ItemTableSet.m_MPDamage;
		pTableItem->m_bMPDrain = ItemTableSet.m_MPDrain;
		pTableItem->m_bMirrorDamage = ItemTableSet.m_MirrorDamage;
		pTableItem->m_bDroprate = ItemTableSet.m_Droprate;
		pTableItem->m_bStrB = ItemTableSet.m_StrB;
		pTableItem->m_bStaB = ItemTableSet.m_StaB;
		pTableItem->m_bDexB = ItemTableSet.m_DexB;
		pTableItem->m_bIntelB = ItemTableSet.m_IntelB;
		pTableItem->m_bChaB = ItemTableSet.m_ChaB;
		pTableItem->m_MaxHpB = ItemTableSet.m_MaxHpB;
		pTableItem->m_MaxMpB = ItemTableSet.m_MaxMpB;
		pTableItem->m_bFireR = ItemTableSet.m_FireR;
		pTableItem->m_bColdR = ItemTableSet.m_ColdR;
		pTableItem->m_bLightningR = ItemTableSet.m_LightningR;
		pTableItem->m_bMagicR = ItemTableSet.m_MagicR;
		pTableItem->m_bPoisonR = ItemTableSet.m_PoisonR;
		pTableItem->m_bCurseR = ItemTableSet.m_CurseR;

		if (!m_ItemtableArray.PutData(pTableItem->m_iNum, pTableItem)) {
			//TRACE("ItemTable PutData Fail - %d\n", pTableItem->m_iNum);
			delete pTableItem;
			pTableItem = NULL;
		}

		ItemTableSet.MoveNext();
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
BOOL CEbenezerDlg::LoadMagicTable()
{
	CMagicTableSet	MagicTableSet;

	if (!MagicTableSet.Open()) {
		//AfxMessageBox(_T("MagicTable Open Fail!"));
		return FALSE;
	}
	if (MagicTableSet.IsBOF() || MagicTableSet.IsEOF()) {
		//AfxMessageBox(_T("MagicTable Empty!"));
		return FALSE;
	}

	MagicTableSet.MoveFirst();

	while (!MagicTableSet.IsEOF())
	{
		_MAGIC_TABLE* pTableMagic = new _MAGIC_TABLE;

		pTableMagic->iNum = MagicTableSet.m_MagicNum;
		pTableMagic->sFlyingEffect = MagicTableSet.m_FlyingEffect;
		pTableMagic->bMoral = MagicTableSet.m_Moral;
		pTableMagic->bSkillLevel = MagicTableSet.m_SkillLevel;
		pTableMagic->sSkill = MagicTableSet.m_Skill;
		pTableMagic->sMsp = MagicTableSet.m_Msp;
		pTableMagic->sHP = MagicTableSet.m_HP;
		pTableMagic->bItemGroup = MagicTableSet.m_ItemGroup;
		pTableMagic->iUseItem = MagicTableSet.m_UseItem;
		pTableMagic->bCastTime = MagicTableSet.m_CastTime;
		pTableMagic->bReCastTime = MagicTableSet.m_ReCastTime;
		pTableMagic->bSuccessRate = MagicTableSet.m_SuccessRate;
		pTableMagic->bType1 = MagicTableSet.m_Type1;
		pTableMagic->bType2 = MagicTableSet.m_Type2;
		pTableMagic->sRange = MagicTableSet.m_Range;
		pTableMagic->bEtc = MagicTableSet.m_Etc;

		if (!m_MagictableArray.PutData(pTableMagic->iNum, pTableMagic)) {
			//TRACE("MagicTable PutData Fail - %d\n", pTableMagic->iNum);
			delete pTableMagic;
			pTableMagic = NULL;
		}

		MagicTableSet.MoveNext();
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
BOOL CEbenezerDlg::LoadCoefficientTable()
{
	CCoefficientSet	CoefficientSet;

	if (!CoefficientSet.Open()) {
		AfxMessageBox(_T("CharacterDataTable Open Fail!"));
		return FALSE;
	}
	if (CoefficientSet.IsBOF() || CoefficientSet.IsEOF()) {
		AfxMessageBox(_T("CharaterDataTable Empty!"));
		return FALSE;
	}

	CoefficientSet.MoveFirst();

	while (!CoefficientSet.IsEOF())
	{
		_CLASS_COEFFICIENT* p_TableCoefficient = new _CLASS_COEFFICIENT;

		p_TableCoefficient->sClassNum = (short)CoefficientSet.m_sClass;
		p_TableCoefficient->ShortSword = (float)CoefficientSet.m_ShortSword;
		p_TableCoefficient->Sword = (float)CoefficientSet.m_Sword;
		p_TableCoefficient->Axe = (float)CoefficientSet.m_Axe;
		p_TableCoefficient->Club = (float)CoefficientSet.m_Club;
		p_TableCoefficient->Spear = (float)CoefficientSet.m_Spear;
		p_TableCoefficient->Pole = (float)CoefficientSet.m_Pole;
		p_TableCoefficient->Staff = (float)CoefficientSet.m_Staff;
		p_TableCoefficient->Bow = (float)CoefficientSet.m_Bow;
		p_TableCoefficient->HP = (float)CoefficientSet.m_Hp;
		p_TableCoefficient->MP = (float)CoefficientSet.m_Mp;
		p_TableCoefficient->SP = (float)CoefficientSet.m_Sp;
		p_TableCoefficient->AC = (float)CoefficientSet.m_Ac;
		p_TableCoefficient->Hitrate = (float)CoefficientSet.m_Hitrate;
		p_TableCoefficient->Evasionrate = (float)CoefficientSet.m_Evasionrate;

		if (!m_CoefficientArray.PutData(p_TableCoefficient->sClassNum, p_TableCoefficient)) {
			TRACE("Coefficient PutData Fail - %d\n", p_TableCoefficient->sClassNum);
			delete p_TableCoefficient;
			p_TableCoefficient = NULL;
		}

		CoefficientSet.MoveNext();
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
void CEbenezerDlg::Send_UnitRegion(char *pBuf, int len, int zoneindex, int x, int z, CUser *pExceptUser, bool bDirect)
{
	/*
	int t_count = 0, uid = -1;
	CUser* pUser = NULL;
	C3DMap* pMap = NULL;

	pMap = (C3DMap*)m_ZoneArray[zoneindex];
	if (!pMap)
		return;
	if (x<0 || z<0 || x>pMap->GetXRegionMax() || z>pMap->GetZRegionMax())
		return;

	//EnterCriticalSection(&g_region_critical);

	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;

	Iter1 = pMap->m_ppRegion[x][z].m_RegionUserArray.m_UserTypeMap.begin();
	Iter2 = pMap->m_ppRegion[x][z].m_RegionUserArray.m_UserTypeMap.end();

	for (; Iter1 != Iter2; Iter1++) {
		uid = *((*Iter1).second);
		if (uid < 0)
			continue;
		pUser = (CUser*)m_Iocport.m_SockArray[uid];
		if (pUser == pExceptUser)
			continue;
		if (pUser && (pUser->GetState() == STATE_GAMESTART)) {
			if (bDirect)
				pUser->Send((uint8_t*)pBuf, len);
			else
				pUser->RegionPacketAdd(pBuf, len);
		}
	}

	//LeaveCriticalSection(&g_region_critical);
	*/
}

//-----------------------------------------------------------------------------
int CEbenezerDlg::GetRegionUserIn(C3DMap *pMap, int region_x, int region_z, char *buff, int &t_count)
{
	int buff_index = 0, i = 0, j = 0;
	CUser* pUser = NULL;
	//CKnights* pKnights = NULL;
	int user_count = 0, uid = -1, iLength = 0;

	if (!pMap)
		return 0;
	if (region_x<0 || region_z<0 || region_x>pMap->GetXRegionMax() || region_z>pMap->GetZRegionMax())
		return 0;

	//EnterCriticalSection(&g_region_critical);

	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;

	Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.begin();
	Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.end();

	for (; Iter1 != Iter2; Iter1++) {
		uid = *((*Iter1).second);
		if (uid < 0)
			continue;
		pUser = (CUser*)m_Iocport.m_SockArray[uid];
		if (!pUser) continue;
		if (pUser->m_RegionX != region_x || pUser->m_RegionZ != region_z) continue;
		if (pUser->GetState() != STATE_GAMESTART) continue;

		SetShort(buff, pUser->GetSocketID(), buff_index);
		SetShort(buff, strlen(pUser->m_pUserData->m_id), buff_index);
		SetString(buff, pUser->m_pUserData->m_id, strlen(pUser->m_pUserData->m_id), buff_index);
		SetByte(buff, pUser->m_pUserData->m_bNation, buff_index);
		SetShort(buff, pUser->m_pUserData->m_bKnights, buff_index);
		// 666 work
		SetByte(buff, pUser->m_pUserData->m_bFame, buff_index);

		/*
		pKnights = m_KnightsArray.GetData(pUser->m_pUserData->m_bKnights);
		if (pUser->m_pUserData->m_bKnights == 0) {
		*/
			SetShort(buff, 0, buff_index);
			SetByte(buff, 0, buff_index);
			SetByte(buff, 0, buff_index);
			/*
		}
		else {
			//pKnights = m_pMain->m_KnightsArray.GetData( m_pUserData->m_bKnights );
			if (pKnights) {
				iLength = 0;
				iLength = strlen(pKnights->m_strName);
				SetShort(buff, (short)iLength, buff_index);
				SetString(buff, pKnights->m_strName, iLength, buff_index);
				SetByte(buff, pKnights->m_byGrade, buff_index);  // knights grade
				SetByte(buff, pKnights->m_byRanking, buff_index);  // knights grade
																   //TRACE("getregionuserin knights index = %d, kname=%s, name=%s\n" , iLength, pKnights->strName, pUser->m_pUserData->m_id);
			}
			else {
				SetShort(buff, 0, buff_index);
				SetByte(buff, 0, buff_index);
				SetByte(buff, 0, buff_index);
			}
		}*/

		SetByte(buff, pUser->m_pUserData->m_bLevel, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bRace, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sClass, buff_index);
		SetShort(buff, (WORD)pUser->m_pUserData->m_curx * 10, buff_index);
		SetShort(buff, pUser->m_pUserData->m_curz * 10, buff_index);
		SetShort(buff, pUser->m_pUserData->m_cury * 10, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bFace, buff_index);
		SetByte(buff, pUser->m_pUserData->m_bHairColor, buff_index);
		SetByte(buff, pUser->m_bResHpType, buff_index);
		// 비러머글 수능...
		SetByte(buff, pUser->m_bAbnormalType, buff_index);
		//
		SetByte(buff, pUser->m_bNeedParty, buff_index);
		// 여기두 주석처리
		SetByte(buff, pUser->m_pUserData->m_bAuthority, buff_index);
		//
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[BREAST].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[BREAST].sDuration, buff_index);
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[LEG].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[LEG].sDuration, buff_index);
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[HEAD].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[HEAD].sDuration, buff_index);
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[GLOVE].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[GLOVE].sDuration, buff_index);
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[FOOT].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[FOOT].sDuration, buff_index);
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[SHOULDER].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[SHOULDER].sDuration, buff_index);
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[RIGHTHAND].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[RIGHTHAND].sDuration, buff_index);
		SetDWORD(buff, pUser->m_pUserData->m_sItemArray[LEFTHAND].nNum, buff_index);
		SetShort(buff, pUser->m_pUserData->m_sItemArray[LEFTHAND].sDuration, buff_index);
		t_count++;
	}

	//LeaveCriticalSection(&g_region_critical);

	return buff_index;
}

//-----------------------------------------------------------------------------
int CEbenezerDlg::GetZoneIndex(int zonenumber)
{
	int t_count = m_ZoneArray.size();
	for (int i = 0; i<t_count; i++) {
		if (m_ZoneArray[i] && (zonenumber == m_ZoneArray[i]->m_nZoneNumber))
			return i;
	}

	return -1;
}

//-----------------------------------------------------------------------------
void CEbenezerDlg::RegionUserInOutForMe(CUser *pSendUser)
{
	int send_index = 0, buff_index = 0, i = 0, j = 0, t_count = 0;
	C3DMap* pMap = NULL;
	int region_x = -1, region_z = -1, user_count = 0, uid_sendindex = 0;
	char uid_buff[2048], send_buff[16384];
	memset(uid_buff, NULL, 2048);
	memset(send_buff, NULL, 16384);

	if (!pSendUser) return;
	if (pSendUser->m_iZoneIndex < 0 || pSendUser->m_iZoneIndex >= m_ZoneArray.size()) return;
	pMap = (C3DMap*)m_ZoneArray[pSendUser->m_iZoneIndex];
	if (!pMap) return;

	uid_sendindex = 3;	// packet command 와 user_count 는 나중에 셋팅한다...

	region_x = pSendUser->m_RegionX;	region_z = pSendUser->m_RegionZ;			// CENTER
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX - 1;	region_z = pSendUser->m_RegionZ - 1;	// NORTH WEST
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX;	region_z = pSendUser->m_RegionZ - 1;		// NORTH
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX + 1;	region_z = pSendUser->m_RegionZ - 1;	// NORTH EAST
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX - 1;	region_z = pSendUser->m_RegionZ;		// WEST
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX + 1;	region_z = pSendUser->m_RegionZ;		// EAST
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX - 1;	region_z = pSendUser->m_RegionZ + 1;	// SOUTH WEST
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX;	region_z = pSendUser->m_RegionZ + 1;		// SOUTH
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);
	memset(uid_buff, NULL, 2048);

	region_x = pSendUser->m_RegionX + 1;	region_z = pSendUser->m_RegionZ + 1;	// SOUTH EAST
	buff_index = GetRegionUserList(pMap, region_x, region_z, uid_buff, user_count);
	SetString(send_buff, uid_buff, buff_index, uid_sendindex);

	int temp_index = 0;
	SetByte(send_buff, WIZ_REGIONCHANGE, temp_index);
	SetShort(send_buff, user_count, temp_index);

	pSendUser->Send(send_buff, uid_sendindex);

	if (user_count > 500);
		//TRACE("Req UserIn: %d \n", user_count);
}

//-----------------------------------------------------------------------------
int CEbenezerDlg::GetRegionUserList(C3DMap* pMap, int region_x, int region_z, char *buff, int &t_count)
{
	int buff_index = 0, i = 0;
	int user_count = 0, uid = -1;
	CUser* pUser = NULL;

	if (!pMap)
		return 0;
	if (region_x<0 || region_z<0 || region_x>pMap->GetXRegionMax() || region_z>pMap->GetZRegionMax())
		return 0;

	//EnterCriticalSection(&g_region_critical);

	map < int, int* >::iterator		Iter1;
	map < int, int* >::iterator		Iter2;

	Iter1 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.begin();
	Iter2 = pMap->m_ppRegion[region_x][region_z].m_RegionUserArray.m_UserTypeMap.end();

	for (; Iter1 != Iter2; Iter1++) {
		uid = *((*Iter1).second);
		if (uid < 0)
			continue;
		pUser = (CUser*)m_Iocport.m_SockArray[uid];
		if (pUser && (pUser->GetState() == STATE_GAMESTART)) {
			SetShort(buff, pUser->GetSocketID(), buff_index);
			t_count++;
		}
	}

	//LeaveCriticalSection(&g_region_critical);

	return buff_index;
}

//-----------------------------------------------------------------------------
__int64 CEbenezerDlg::GenerateItemSerial()
{
	MYINT64 serial;
	MYSHORT	increase;
	serial.i = 0;
	increase.w = 0;

	CTime t = CTime::GetCurrentTime();

	//EnterCriticalSection(&g_serial_critical);

	increase.w = g_increase_serial++;

	serial.b[7] = (BYTE)m_nServerNo;
	serial.b[6] = (BYTE)(t.GetYear() % 100);
	serial.b[5] = (BYTE)t.GetMonth();
	serial.b[4] = (BYTE)t.GetDay();
	serial.b[3] = (BYTE)t.GetHour();
	serial.b[2] = (BYTE)t.GetMinute();
	serial.b[1] = increase.b[1];
	serial.b[0] = increase.b[0];

	//LeaveCriticalSection(&g_serial_critical);

	//	TRACE("Generate Item Serial : %I64d\n", serial.i);
	return serial.i;
}

//-----------------------------------------------------------------------------
void CEbenezerDlg::GetTimeFromIni()
{
	int year = 0, month = 0, date = 0, hour = 0, server_count = 0, sgroup_count = 0, i = 0;
	char ipkey[20]; memset(ipkey, 0x00, 20);

	m_Ini.SetPath("server.ini");
	m_nYear = m_Ini.GetProfileInt("TIMER", "YEAR", 1);
	m_nMonth = m_Ini.GetProfileInt("TIMER", "MONTH", 1);
	m_nDate = m_Ini.GetProfileInt("TIMER", "DATE", 1);
	m_nHour = m_Ini.GetProfileInt("TIMER", "HOUR", 1);
	m_nWeather = m_Ini.GetProfileInt("TIMER", "WEATHER", 1);

	//	m_nBattleZoneOpenWeek  = m_Ini.GetProfileInt("BATTLE", "WEEK", 3);
	m_nBattleZoneOpenWeek = m_Ini.GetProfileInt("BATTLE", "WEEK", 5);
	m_nBattleZoneOpenHourStart = m_Ini.GetProfileInt("BATTLE", "START_TIME", 20);
	m_nBattleZoneOpenHourEnd = m_Ini.GetProfileInt("BATTLE", "END_TIME", 0);

	m_nCastleCapture = m_Ini.GetProfileInt("CASTLE", "NATION", 1);
	m_nServerNo = m_Ini.GetProfileInt("ZONE_INFO", "MY_INFO", 1);
	m_nServerGroup = m_Ini.GetProfileInt("ZONE_INFO", "SERVER_NUM", 0);
	server_count = m_Ini.GetProfileInt("ZONE_INFO", "SERVER_COUNT", 1);
	if (server_count < 1) {
		AfxMessageBox("ServerCount Error!!");
		return;
	}

	for (i = 0; i<server_count; i++) {
		_ZONE_SERVERINFO *pInfo = new _ZONE_SERVERINFO;
		sprintf(ipkey, "SERVER_%02d", i);
		pInfo->sServerNo = m_Ini.GetProfileInt("ZONE_INFO", ipkey, 1);
		sprintf(ipkey, "SERVER_IP_%02d", i);
		strcpy(pInfo->strServerIP, m_Ini.GetProfileString("ZONE_INFO", ipkey, "210.92.91.242"));
		pInfo->sPort = _LISTEN_PORT + pInfo->sServerNo;

		m_ServerArray.PutData(pInfo->sServerNo, pInfo);
	}

	if (m_nServerGroup != 0) {
		m_nServerGroupNo = m_Ini.GetProfileInt("SG_INFO", "GMY_INFO", 1);
		sgroup_count = m_Ini.GetProfileInt("SG_INFO", "GSERVER_COUNT", 1);
		if (server_count < 1) {
			AfxMessageBox("ServerCount Error!!");
			return;
		}
		for (i = 0; i<sgroup_count; i++) {
			_ZONE_SERVERINFO *pInfo = new _ZONE_SERVERINFO;
			sprintf(ipkey, "GSERVER_%02d", i);
			pInfo->sServerNo = m_Ini.GetProfileInt("SG_INFO", ipkey, 1);
			sprintf(ipkey, "GSERVER_IP_%02d", i);
			strcpy(pInfo->strServerIP, m_Ini.GetProfileString("SG_INFO", ipkey, "210.92.91.242"));
			pInfo->sPort = _LISTEN_PORT + pInfo->sServerNo;

			m_ServerGroupArray.PutData(pInfo->sServerNo, pInfo);
		}
	}

	/*
	SetTimer(GAME_TIME, 6000, NULL);
	SetTimer(SEND_TIME, 200, NULL);
	SetTimer(ALIVE_TIME, 34000, NULL);
	SetTimer(MARKET_BBS_TIME, 300000, NULL);
	SetTimer(PACKET_CHECK, 360000, NULL);
	*/
}
