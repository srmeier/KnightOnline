#ifndef _DEFINE_H
#define _DEFINE_H

//#include <mmsystem.h>
//#include "afxdb.h"
//#include "stdafx.h"

//#include <string>

//#include "SDL2/SDL.h"
//#include "SDL2/SDL_net.h"

typedef struct {
	uint16_t sCurZone;
	uint16_t sLength;
	char pBuf[2048];
} SEND_DATA;

typedef char Byte;

#define _LISTEN_PORT		15000
#define _UDP_PORT			8888
#define AI_KARUS_SOCKET_PORT		10020
#define AI_ELMO_SOCKET_PORT			10030
#define AI_BATTLE_SOCKET_PORT		10040
#define MAX_USER			3000
#define CLIENT_SOCKSIZE		100
#define MAX_AI_SOCKET		10			// sungyong~ 2002.05.22

#define MAX_TYPE3_REPEAT    20
#define MAX_TYPE4_BUFF		9

#define MAX_ITEM_COUNT		9999		// 한 슬롯에 가지는 최대 화살/송편 개수

#define MAX_ID_SIZE			20
#define MAX_PW_SIZE			12
#define MAX_ITEM			28
#define VIEW_DISTANCE		48			// 가시거리

const BYTE SLOT_MAX			= 14;		// 착용 아템 MAX
const BYTE HAVE_MAX			= 28;		// 소유 아템 MAX (인벤토리창)
const BYTE WAREHOUSE_MAX	= 196;		// 창고 아이템 MAX

#define NPC_HAVE_ITEM_LIST	6
#define ZONEITEM_MAX		2100000000	// 존에 떨어지는 최대 아이템수...

#define MAX_CLASS			26			// 직업 MAX
#define MAX_LEVEL			60			// 최고렙...

#define SERVER_INFO_START			0X01
#define SERVER_INFO_END				0X02

//////////////  Quest 관련 Define ////////////////////////////
#define MAX_EVENT					2000
#define MAX_EVENT_SIZE				400
#define MAX_EVENT_NUM				2000
#define MAX_EXEC_INT				30
#define MAX_LOGIC_ELSE_INT			10
#define MAX_MESSAGE_EVENT			10
#define MAX_COUPON_ID_LENGTH		20
#define MAX_CURRENT_EVENT			20

// 지금 쓰이는것만 정의 해 놨습니다.
// logic관련 define
#define LOGIC_CHECK_UNDER_WEIGHT	0X01
#define LOGIC_CHECK_OVER_WEIGHT		0X02
#define LOGIC_CHECK_SKILL_POINT		0X03
#define LOGIC_EXIST_ITEM			0X04
#define LOGIC_CHECK_CLASS			0x05
#define LOGIC_CHECK_WEIGHT			0x06
#define LOGIC_CHECK_EDITBOX			0x07
#define LOGIC_RAND					0x08
#define LOGIC_HOWMUCH_ITEM			0x09 
#define	LOGIC_CHECK_LEVEL			0x0A
#define LOGIC_NOEXIST_COM_EVENT		0x0B
#define LOGIC_EXIST_COM_EVENT		0x0C
#define LOGIC_CHECK_NOAH			0x0D

/*
#define	LOGIC_CHECK_NATION			0X01
#define	LOGIC_CHECK_LEVEL			0X02
#define	LOGIC_NOEXIST_ITEM			0X03
#define	LOGIC_QUEST_END				0X04
#define	LOGIC_QUEST_LOG				0X05
#define	LOGIC_EXIST_ITEM			0X06
#define	LOGIC_CHECK_NOAH			0X07
#define LOGIC_CHECK_CLASS			0x08
#define LOGIC_CHECK_WEIGHT			0x09
#define LOGIC_CHECK_RACE			0x0A
#define LOGIC_CHECK_LOYALTY			0x0B
#define LOGIC_CHECK_AUTHORITY		0x0C
#define LOGIC_CHECK_STR				0X0D
#define LOGIC_CHECK_STA				0x0D
#define LOGIC_CHECK_DEX				0x0E
#define LOGIC_CHECK_INT				0x0F
#define LOGIC_CHECK_CHA				0x10
#define LOGIC_CHECK_SKILLPOINT		0x11
#define LOGIC_CHECK_DAY				0x12
*/

// 실행관련 define
#define EXEC_SAY					0X01
#define EXEC_SELECT_MSG				0X02
#define EXEC_RUN_EVENT				0X03
#define EXEC_GIVE_ITEM				0X04
#define EXEC_ROB_ITEM				0X05
#define EXEC_RETURN					0X06
#define EXEC_OPEN_EDITBOX			0x07
#define EXEC_GIVE_NOAH				0x08
#define EXEC_LOG_COUPON_ITEM		0x09
#define EXEC_SAVE_COM_EVENT			0x0A
#define EXEC_ROB_NOAH				0x0B

/*
#define EXEC_SAY					0X01
#define EXEC_SELECT_MSG				0X02
#define EXEC_RUN_EVENT				0X03
#define EXEC_RETURN					0X04
#define EXEC_ROB_ITEM				0X05
#define EXEC_ROB_NOAH				0X06
#define EXEC_GIVE_ITEM				0X07
#define EXEC_GIVE_QUEST				0X08
#define EXEC_QUEST_END				0X09
#define EXEC_QUEST_SAVE				0X0A
#define EXEC_GIVE_NOAH				0x0B
*/

// EVENT 시작 번호들 :)
#define EVENT_POTION				1
#define EVENT_LOGOS_ELMORAD			1001
#define EVENT_LOGOS_KARUS			2001
#define EVENT_COUPON				3001


////////////////////////////////////////////////////////////

///////////////// BBS RELATED //////////////////////////////
#define MAX_BBS_PAGE			23
#define MAX_BBS_MESSAGE			40
#define MAX_BBS_TITLE			20
#define MAX_BBS_POST			500

#define BUY_POST_PRICE			500
#define SELL_POST_PRICE			1000

#define REMOTE_PURCHASE_PRICE	5000
#define BBS_CHECK_TIME			36000

///////////////// NPC  STATUS //////////////////////////////
#define NPC_DEAD				0X00
#define NPC_LIVE				0X01

///////////////// NPC TYPE /////////////////////////////////
#define NPC_MONSTER				00
#define NPC_GENERAL				01		//
#define NPC_BOSS				03		// Unique Mop
#define NPC_PATROL_GUARD		11		// 경비병
#define NPC_MERCHANT			21		// 상인
#define NPC_TINKER				22		// 대장장이
#define NPC_WAREHOUSE			31		// 창고지기
#define NPC_CAPTAIN				35		// 전직
#define NPC_OFFICER				36		// 기사단 관리 NPC
#define NPC_CLERIC				37		// 대사제 NPC
#define NPC_HEALER				40		// Healer
#define NPC_WARP				41		// Warp Npc
#define NPC_GATE				50		// 성문형태 NPC 
#define NPC_PHOENIX_GATE		51		// 깨지지 않는 문 (8->51)
#define NPC_SPECIAL_GATE		52		// 깨지지 않는 문이면서 2분마다 열렸다 닫혔다 하는 문
#define NPC_GATE_LEVER			55		// 성문 레버...	(9->55)	
#define NPC_ARTIFACT			60		// 결계석 (7->60)
#define NPC_DESTORY_ARTIFACT	61		// 파괴되는 결계석
#define NPC_MONK_ELMORAD		71		// 비러머글 수도승 >.<
#define NPC_MONK_KARUS			72		// 비러머글 수도승 >.<	
#define NPC_DOMESTIC_ANIMAL		99		// 가축 NPC
#define NPC_COUPON				100		// 비러머글 복권 >.<


///////////////// NATION ///////////////////////////////////
#define UNIFY_NATION		0
#define KARUS               1
#define ELMORAD             2
#define BATTLE				3

#define BATTLE_ZONE			101

////////////////////////////////////////////////////////////

// Attack Type
#define DIRECT_ATTACK		0
#define LONG_ATTACK			1
#define MAGIC_ATTACK		2
#define DURATION_ATTACK		3

////////////////// ETC Define //////////////////////////////
// UserInOut //
#define USER_IN					0X01
#define USER_OUT				0X02
#define USER_REGENE				0X03	// Userin하고 같은것인데 효과를 주기위해서.. 분리(게임시작, 리젠. 소환시)
#define USER_WARP				0X04
#define USER_SUMMON				0X05
#define NPC_IN					0X01
#define NPC_OUT					0X02

////////////////// Resurrection related ////////////////////
#define BLINK_TIME				10
#define CLAN_SUMMON_TIME		180
////////////////////////////////////////////////////////////

// Socket Define
////////////////////////////////////////////////////////////
#define SOCKET_BUFF_SIZE	(1024*16)
#define MAX_PACKET_SIZE		(1024*8)
#define REGION_BUFF_SIZE	(1024*16)

#define PACKET_START1				0XAA
#define PACKET_START2				0X55
#define PACKET_END1					0X55
#define PACKET_END2					0XAA

// status
#define STATE_CONNECTED			0X01
#define STATE_DISCONNECTED		0X02
#define STATE_GAMESTART			0x03

// Socket type
#define TYPE_ACCEPT				0x01
#define TYPE_CONNECT			0x02

// Overlapped flag
#define OVL_RECEIVE				0X01
#define OVL_SEND				0X02
#define OVL_CLOSE				0X03
////////////////////////////////////////////////////////////

// ==================================================================
//	About Map Object
// ==================================================================
#define USER_BAND				0			// Map 위에 유저가 있다.
#define NPC_BAND				10000		// Map 위에 NPC(몹포함)가 있다.
#define INVALID_BAND			30000		// 잘못된 ID BAND

#define EVENT_MONSTER			20			// Event monster 총 수

///////////////// snow event define //////////////////////////////
#define SNOW_EVENT_MONEY		2000
#define SNOW_EVENT_SKILL		490043

//////////////////////////////////////////////////////////////////
// DEFINE Shared Memory Queue
//////////////////////////////////////////////////////////////////

#define E	0x00
#define R	0x01
#define W	0x02
#define WR	0x03

// DEFINE Shared Memory Queue Return VALUE

#define SMQ_BROKEN		10000
#define SMQ_FULL		10001
#define SMQ_EMPTY		10002
#define SMQ_PKTSIZEOVER	10003
#define SMQ_WRITING		10004
#define SMQ_READING		10005
#define SMQ_INVALID		10006

// DEFINE Shared Memory Costumizing

#define MAX_PKTSIZE		512
#define MAX_COUNT		4096
#define SMQ_LOGGERSEND	"KNIGHT_SEND"
#define SMQ_LOGGERRECV	"KNIGHT_RECV"

#define SMQ_ITEMLOGGER	"ITEMLOG_SEND"

// Reply packet define...

#define SEND_ME					0x01
#define SEND_REGION				0x02
#define SEND_ALL				0x03
#define SEND_ZONE				0x04

// Battlezone Announcement
#define BATTLEZONE_OPEN					0x00
#define BATTLEZONE_CLOSE				0x01           
#define DECLARE_WINNER					0x02
#define DECLARE_LOSER					0x03
#define DECLARE_BAN						0x04
#define KARUS_CAPTAIN_NOTIFY			0x05
#define ELMORAD_CAPTAIN_NOTIFY			0x06
#define KARUS_CAPTAIN_DEPRIVE_NOTIFY	0x07
#define ELMORAD_CAPTAIN_DEPRIVE_NOTIFY	0x08
#define SNOW_BATTLEZONE_OPEN			0x09

// Battle define
#define NO_BATTLE				0
#define NATION_BATTLE			1
#define SNOW_BATTLE				2

// Zone IDs
#define ZONE_KARUS				1
#define ZONE_ELMORAD			2
#define ZONE_BATTLE				101
#define ZONE_SNOW_BATTLE		102
#define	ZONE_FRONTIER			201

#define MAX_BATTLE_ZONE_USERS	150

//////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
typedef union{
	WORD		w;
	BYTE		b[2];
} MYSHORT;

typedef union{
	int			i;
	BYTE		b[4];
} MYINT;

typedef union{
	DWORD		w;
	BYTE		b[4];
} MYDWORD;

typedef union{
	__int64		i;
	BYTE		b[8];
} MYINT64;

struct _REGION_BUFFER {
	int		iLength;
	BYTE	bFlag;
	DWORD	dwThreadID;

	char	pDataBuff[REGION_BUFF_SIZE];
	_REGION_BUFFER() {
		iLength = 0;
		bFlag = E;
		dwThreadID = 0;
		memset(pDataBuff, 0x00, REGION_BUFF_SIZE);
	};
};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//
//	Global Function Define
//

inline void GetString(char* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf, sBuf+index, len);
	index += len;
};

inline BYTE GetByte(char* sBuf, int& index)
{
	int t_index = index;
	index++;
	return (BYTE)(*(sBuf+t_index));
};

inline int GetShort(char* sBuf, int& index)
{
	index += 2;
	return *(short*)(sBuf+index-2);
};

inline DWORD GetDWORD(char* sBuf, int& index)
{
	index += 4;
	return *(DWORD*)(sBuf+index-4);
};

inline float Getfloat(char* sBuf, int& index)
{
	index += 4;
	return *(float*)(sBuf+index-4);
};

inline __int64 GetInt64(char* sBuf, int& index)
{
	index += 8;
	return *(__int64*)(sBuf+index-8);
};

inline void SetString(char* tBuf, char* sBuf, int len, int& index)
{
	memcpy(tBuf+index, sBuf, len);
	index += len;
};

inline void SetByte(char* tBuf, BYTE sByte, int& index)
{
	*(tBuf+index) = (char)sByte;
	index++;
};

inline void SetShort(char* tBuf, int sShort, int& index)
{
	short temp = (short)sShort;

	CopyMemory( tBuf+index, &temp, 2);
	index += 2;
};

inline void SetDWORD(char* tBuf, DWORD sDWORD, int& index)
{
	CopyMemory( tBuf+index, &sDWORD, 4);
	index += 4;
};

inline void Setfloat ( char* tBuf, float sFloat, int& index )
{
	CopyMemory( tBuf+index, &sFloat, 4);
	index += 4;
};

inline void SetInt64 ( char* tBuf, __int64 nInt64, int& index )
{
	CopyMemory( tBuf+index, &nInt64, 8);
	index += 8;
};
// sungyong 2001.11.06
inline int GetVarString(TCHAR* tBuf, TCHAR* sBuf, int nSize, int& index)
{
	int nLen = 0;
	
	if(nSize == sizeof(BYTE))	nLen = GetByte(sBuf, index);
	else nLen = GetShort(sBuf, index);

	GetString(tBuf, sBuf, nLen, index);
	*(tBuf + nLen) = 0;

	return nLen;
};

inline void SetVarString(TCHAR *tBuf, TCHAR* sBuf, int len, int &index)
{
	*(tBuf+index) = (BYTE)len;
	index ++;

	CopyMemory(tBuf+index, sBuf, len);
	index += len;
};
// ~sungyong 2001.11.06
inline int ParseSpace( char* tBuf, char* sBuf)
{
	int i = 0, index = 0;
	BOOL flag = FALSE;
	
	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	while(sBuf[index] !=' ' && sBuf[index] !='\t' && sBuf[index] !=(BYTE) 0){
		tBuf[i++] = sBuf[index++];
		flag = TRUE;
	}
	tBuf[i] = 0;

	while(sBuf[index] == ' ' || sBuf[index] == '\t')index++;
	if(!flag) return 0;	
	return index;
};

inline CString GetProgPath()
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	_splitpath(Buf,drive,dir,fname,ext);
	strcpy(Path, drive);
	strcat(Path, dir);		
	CString _Path = Path;
	return _Path;
};

inline void LogFileWrite( LPCTSTR logstr )
{
	CString ProgPath, LogFileName;
	CFile file;
	int loglength;

	ProgPath = GetProgPath();
	loglength = strlen( logstr );

	LogFileName.Format("%s\\Ebenezer.log", ProgPath);
	
	file.Open( LogFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite );

	file.SeekToEnd();
	file.Write(logstr, loglength);
	file.Close();
};

inline void DisplayErrorMsg(SQLHANDLE hstmt)
{
	SQLCHAR       SqlState[6], Msg[1024];
	SQLINTEGER    NativeError;
	SQLSMALLINT   i, MsgLen;
	SQLRETURN     rc2;

	i = 1;
	while ((rc2 = SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen)) != SQL_NO_DATA)
	{
		TRACE("*** %s, %d, %s, %d ***\n", SqlState,NativeError,Msg,MsgLen);

		i++;
	}
};

inline int myrand( int min, int max )
{
	if( min == max ) return min;
	if( min > max )
	{
		int temp = min;
		min = max;
		max = temp;
	}

	double gap = max - min + 1;
	double rrr = (double)RAND_MAX / gap;

	double rand_result;

	rand_result = (double)rand() / rrr;

	if( (int)( min + (int)rand_result ) < min ) return min;
	if( (int)( min + (int)rand_result ) > max ) return max;

	return (int)( min + (int)rand_result );
};

inline float TimeGet()
{
	static bool bInit = false;
	static bool bUseHWTimer = FALSE;
	static LARGE_INTEGER nTime, nFrequency;
	
	if(bInit == false)
	{
		if(TRUE == ::QueryPerformanceCounter(&nTime))
		{
			::QueryPerformanceFrequency(&nFrequency);
			bUseHWTimer = TRUE;
		}
		else 
		{
			bUseHWTimer = FALSE;
		}

		bInit = true;
	}

	if(bUseHWTimer)
	{
		::QueryPerformanceCounter(&nTime);
		return (float)((double)(nTime.QuadPart)/(double)nFrequency.QuadPart);
	}

	return (float)SDL_GetTicks();//timeGetTime();
};

inline void	TimeTrace(TCHAR* pMsg)
{
	CString szMsg = _T("");
	CTime time = CTime::GetCurrentTime();
	szMsg.Format("%s,,  time : %d-%d-%d, %d:%d]\n", pMsg, time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute() );
	TRACE(szMsg);
};

#endif