// EbenezerDlg.h : header file
//

#if !defined(AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_)
#define AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "afxdb.h"

#include "Iocport.h"
#include "Map.h"

#include "PacketDefine.h"
#include "STLMap.h"
#include "Npc.h"
#include "GameDefine.h"
#include "EVENT.h"

/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg dialog

typedef std::vector <C3DMap*>				ZoneArray;
typedef std::vector <_LEVELUP*>				LevelUpArray;
typedef CSTLMap <_CLASS_COEFFICIENT>		CoefficientArray;
typedef CSTLMap <_ITEM_TABLE>				ItemtableArray;
typedef CSTLMap <_MAGIC_TABLE>				MagictableArray;
typedef CSTLMap <_MAGIC_TYPE1>				Magictype1Array;
typedef CSTLMap <_MAGIC_TYPE2>				Magictype2Array;
typedef CSTLMap <_MAGIC_TYPE3>				Magictype3Array;
typedef CSTLMap	<_MAGIC_TYPE4>				Magictype4Array;
typedef CSTLMap <_MAGIC_TYPE5>				Magictype5Array;
typedef CSTLMap <_MAGIC_TYPE8>				Magictype8Array;
typedef CSTLMap <CNpc>						NpcArray;
//typedef CSTLMap <CAISocket>					AISocketArray;
typedef CSTLMap <_PARTY_GROUP>				PartyArray;
//typedef CSTLMap <CKnights>					KnightsArray;
typedef CSTLMap <_ZONE_SERVERINFO>			ServerArray;
typedef CSTLMap <_HOME_INFO>				HomeArray;
typedef	CSTLMap	<EVENT>						QuestArray;

struct _POSITION
{
	float initx;
	float initz;
};

class CUser;
class CEbenezerDlg
{
public:
	BOOL	m_bPermanentChatMode;
	BOOL	m_bPermanentChatFlag;
	char	m_strPermanentChat[1024];

	int m_iPacketCount;		// packet check
	int m_iSendPacketCount;		// packet check
	int m_iRecvPacketCount;		// packet check

	BYTE	m_bVictory, m_byOldVictory;
	BYTE    m_byBattleOpen, m_byOldBattleOpen;
	short	m_sKarusDead, m_sElmoradDead, m_sBanishDelay, m_sKarusCount, m_sElmoradCount;
	BYTE    m_byKarusOpenFlag, m_byElmoradOpenFlag, m_byBanishFlag, m_byBattleSave;
	int m_nYear, m_nMonth, m_nDate, m_nHour, m_nMin, m_nWeather, m_nAmount;

	BOOL	m_bPointCheckFlag;		// AI서버와 재접전에 NPC포인터 참조막기 (TRUE:포인터 참조, FALSE:포인터 참조 못함)

	LevelUpArray			m_LevelUpArray;
	ItemtableArray			m_ItemtableArray;
	ZoneArray				m_ZoneArray;
	MagictableArray			m_MagictableArray;
	Magictype1Array			m_Magictype1Array;
	Magictype2Array         m_Magictype2Array;
	Magictype3Array			m_Magictype3Array;
	Magictype4Array			m_Magictype4Array;
	Magictype5Array         m_Magictype5Array;
	Magictype8Array         m_Magictype8Array;
	CoefficientArray		m_CoefficientArray;
	QuestArray				m_Event;

	int					m_nServerNo, m_nServerGroupNo;
	int					m_nServerGroup;	// server의 번호(0:서버군이 없다, 1:서버1군, 2:서버2군)
	ServerArray			m_ServerArray;
	ServerArray			m_ServerGroupArray;
	HomeArray				m_HomeArray;

	char	m_ppNotice[20][128];
	short   m_sDiscount;	// 능력치와 포인트 초기화 할인 (0:할인없음, 1:할인(50%) )

public:
	//void WriteEventLog(char* pBuf);
	//void FlySanta();
	//void BattleZoneCurrentUsers();
	//BOOL LoadKnightsRankTable();
	//void GetCaptainUserPtr();
	//void Send_CommandChat(char* pBuf, int len, int nation, CUser* pExceptUser = NULL);
	//BOOL LoadBattleTable();
	//void Send_UDP_All(char* pBuf, int len, int group_type = 0);
	//void KickOutZoneUsers(short zone);
	__int64 GenerateItemSerial();
	//void KickOutAllUsers();
	//void CheckAliveUser();
	//int GetKnightsGrade(int nPoints);
	//void WritePacketLog();
	//void MarketBBSSellDelete(short index);
	//void MarketBBSBuyDelete(short index);
	//void MarketBBSTimeCheck();
	//int  GetKnightsAllMembers(int knightsindex, char *temp_buff, int& buff_index, int type = 0);
	//BOOL LoadAllKnightsUserData();
	//BOOL LoadAllKnights();
	//BOOL LoadHomeTable();
	//void Announcement(BYTE type, int nation = 0, int chat_type = 8);
	//void ResetBattleZone();
	//void BanishLosers();
	//void BattleZoneVictoryCheck();
	//void BattleZoneOpenTimer();
	//void BattleZoneOpen(int nType);	// 0:open 1:close
	//void AliveUserCheck();
	//void WithdrawUserOut();
	//BOOL LoadMagicType8();
	//BOOL LoadMagicType4();
	//BOOL LoadMagicType5();
	//BOOL LoadMagicType3();
	//BOOL LoadMagicType2();
	//BOOL LoadMagicType1();
	//void KillUser(const char* strbuff);
	//void Send_PartyMember(int party, char* pBuf, int len);
	//void Send_KnightsMember(int index, char* pBuf, int len, int zone = 100);
	//BOOL AISocketConnect(int zone, int flag = 0);
	//int GetRegionNpcIn(C3DMap* pMap, int region_x, int region_z, char* buff, int & t_count);
	//BOOL LoadNoticeData();
	int GetZoneIndex(int zonenumber);
	//int GetRegionNpcList(C3DMap* pMap, int region_x, int region_z, char* nid_buff, int& t_count, int nType = 0); // Region All Npcs nid Packaging Function
	//void RegionNpcInfoForMe(CUser* pSendUser, int nType = 0);	// 9 Regions All Npcs nid Packaging Function
	int GetRegionUserList(C3DMap* pMap, int region_x, int region_z, char* buff, int &t_count); // Region All Users uid Packaging Function
	int GetRegionUserIn(C3DMap* pMap, int region_x, int region_z, char* buff, int &t_count);	// Region All Users USERINOUT Packet Packaging Function
	void RegionUserInOutForMe(CUser* pSendUser);	// 9 Regions All Users uid Packaging Function
	//BOOL LoadLevelUpTable();
	//void SetGameTime();
	//void UpdateWeather();
	//void UpdateGameTime();
	//void GetTimeFromIni();
	//void Send_NearRegion(char* pBuf, int len, int zone, int region_x, int region_z, float curx, float curz, CUser* pExceptUser = NULL);
	//void Send_FilterUnitRegion(char* pBuf, int len, int zoneindex, int x, int z, float ref_x, float ref_z, CUser* pExceptUser = NULL);
	void Send_UnitRegion(char *pBuf, int len, int zoneindex, int x, int z, CUser* pExceptUser = NULL, bool bDirect = true);
	BOOL LoadCoefficientTable();
	BOOL LoadMagicTable();
	BOOL LoadItemTable();
	BOOL MapFileLoad();
	//void UserAcceptThread();
	// sungyong 2001.11.06
	//BOOL AIServerConnect();
	//void SyncTest(int nType);
	//void SyncRegionTest(C3DMap* pMap, int rx, int rz, FILE* pfile, int nType);
	//void SendAllUserInfo();
	//void SendCompressedData();
	//void DeleteAllNpcList(int flag = 0);
	//CNpc*  GetNpcPtr(int sid, int cur_zone);
	// ~sungyong 2001.11.06
	//BOOL InitializeMMF();
	void UserInOutForMe(CUser* pSendUser);	// 9 Regions All Users USERINOUT Packet Packaging Function
	//void NpcInOutForMe(CUser* pSendUser);	// 9 Regions All Npcs NPCINOUT Packet Packaging Function
	void Send_Region(char* pBuf, int len, int zone, int x, int z, CUser* pExceptUser = NULL, bool bDirect = true);	// zone == real zone number
	void Send_All(char* pBuf, int len, CUser* pExceptUser = NULL, int nation = 0);	// pointer != NULL don`t send to that user pointer
	//void Send_AIServer(int zone, char* pBuf, int len);
	static CUser* GetUserPtr(const char* userid, BYTE type);

// Construction
public:
	bool OnInitDialog(void);

	//void UserInOutForMe( CUser* pUser );
	int GetRegionCount( int x, int z );
	//void Send_Region( Byte* pBuf, int len, int x, int z, bool tome = false );
	//void Send_All( Byte* pBuf, int len, bool tome = false );
	//static CUser* GetUserPtr( char* userid );

	static CIOCPort	m_Iocport;
	C3DMap	m_Map;

	int	m_nCount;

	_POSITION	m_pInitPos[5];


// Implementation
protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_)
