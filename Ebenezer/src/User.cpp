// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

//#include "Ebenezer.h"
#include "User.h"
#include "EbenezerDlg.h"

extern int running;

//-----------------------------------------------------------------------------
/*
vPos.x = 358.0f;
vPos.z = 1610.0f;
vPos.y = 120.0f;
*/

int player_zone = 21;
int player_x = 2000;
int player_z = 2000;
int player_y = 0;

//-----------------------------------------------------------------------------
CUser::CUser(void) {}

//-----------------------------------------------------------------------------
CUser::~CUser(void) {}

void CUser::CloseProcess()
{
	/*
	CString logstr;
	logstr.Format("LogOut %s", m_UserId);
	m_pMain->m_StatusList.AddString( logstr );
	m_pMain->m_nCount--;
	memset( m_UserId, 0x00, MAX_ID_SIZE );

	CIOCPSocket2::CloseProcess();
	*/
}

void CUser::Parsing(int len, Byte *pData) {
	int index = 0;
	BYTE command = GetByte(pData, index);

	switch(command) {
		case N3_ROTATE: {
			float fYaw = ((float)GetShort(pData, index) / 100.0f);

			printf("DB: (N3_ROTATE) New angle = %f\n", fYaw);

			/*
			CAPISocket::MP_AddShort(byBuff, iOffset, fYaw*100);
			*/
		} break;

		case N3_TIME_NOTIFY: {
			// NOTE: a two second time notification?
		} break;

		case N3_MOVE: {
			MoveProcess(pData+index);
		} break;

		case N3_CHARACTER_SELECT: {

			char IDName[0xFF] = {};
			char AccName[0xFF] = {};

			Byte ZoneInit;
			Byte ZoneCur;

			short sNameLen = GetShort(pData, index);
			GetString((Byte*)AccName, pData, sNameLen, index);
			sNameLen = GetShort(pData, index);
			GetString((Byte*)IDName, pData, sNameLen, index);
			ZoneInit = GetByte(pData, index);
			ZoneCur = GetByte(pData, index);

			printf("DB: (N3_CHARACTER_SELECT) Account Name = \"%s\", ID = \"%s\", ZoneInit = %d, ZoneCur = %d\n", AccName, IDName, ZoneInit, ZoneCur);

			/*
			CAPISocket::MP_AddShort(byBuff, iOffset, s_szAccount.size());				// 계정 길이..
			CAPISocket::MP_AddString(byBuff, iOffset, s_szAccount);						// 계정 문자열..
			CAPISocket::MP_AddShort(byBuff, iOffset, s_pPlayer->IDString().size());		// 캐릭 아이디 길이..
			CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());			// 캐릭 아이디 문자열..
			CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->m_InfoExt.iZoneInit);	// 처음 접속인지 아닌지 0x01:처음 접속
			CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->m_InfoExt.iZoneCur);		// 캐릭터 선택창에서의 캐릭터 존 번호
			*/

			int send_index = 0;
			Byte send_buf[1024];
			memset(send_buf, 0x00, 1024);

			SetByte(send_buf, N3_CHARACTER_SELECT, send_index);
			SetByte(send_buf, 0x01, send_index);
			SetByte(send_buf, player_zone, send_index); // NOTE: set the init player zone

			// NOTE: need to figure out why the position gets sent twice
			SetShort(send_buf, player_x, send_index);
			SetShort(send_buf, player_z, send_index);
			SetShort(send_buf, player_y, send_index);

			SetByte(send_buf, 0x01, send_index);

			if(Send(send_buf, send_index) < send_index) {
				printf("ER: %s\n", SDLNet_GetError());
			}

			/*
			int iZoneCur = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
			float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
			float fY = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;
			int iVictoryNation = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			*/
		} break;

		case N3_ALL_CHARACTER_INFO_REQUEST: {

			int send_index = 0;
			Byte send_buf[1024];
			memset(send_buf, 0x00, 1024);

			SetByte(send_buf, N3_ALL_CHARACTER_INFO_REQUEST, send_index);

			if(Send(send_buf, send_index) < send_index) {
				printf("ER: %s\n", SDLNet_GetError());
			}

			//int iResult = 0x01;//CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 결과..
			/*
			for(int i = 0; i < MAX_AVAILABLE_CHARACTER; i++)
			{
				if(i!=0) {
					m_InfoChrs[i].szID = "";
					continue;
				}
				int iIDLength				= 0x04;//CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 캐릭터 아이디 길이 s,
				//CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, m_InfoChrs[i].szID, iIDLength);// 캐릭터 아이디 문자열 str
				m_InfoChrs[i].szID = "test";

				m_InfoChrs[i].eRace			= (e_Race)0x01;//(e_Race)(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset)); // 종족 b
				m_InfoChrs[i].eClass		= (e_Class)101;//(e_Class)(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)); // 직업 b
				m_InfoChrs[i].iLevel		= 1;//CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 레벨 b
				m_InfoChrs[i].iFace			= 0;//CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 얼굴모양 b
				m_InfoChrs[i].iHair			= 0;//CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 머리모양 b
				m_InfoChrs[i].iZone			= 1;//CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // zone number

				m_InfoChrs[i].dwItemHelmet				= 0;//CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 투구 dw
				m_InfoChrs[i].iItemHelmetDurability		= 0;//CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
				m_InfoChrs[i].dwItemUpper				= 0;//CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 상체 dw
				m_InfoChrs[i].iItemUpperDurability		= 0;//CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
				m_InfoChrs[i].dwItemCloak				= 0;//CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 어깨(망토) dw
				m_InfoChrs[i].iItemCloakDurability		= 0;//CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
				m_InfoChrs[i].dwItemLower				= 0;//CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 하체 dw
				m_InfoChrs[i].iItemLowerDurability		= 0;//CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
				m_InfoChrs[i].dwItemGloves				= 0;//CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 장갑 dw
				m_InfoChrs[i].iItemGlovesDurability		= 0;//CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
				m_InfoChrs[i].dwItemShoes				= 0;//CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 신발 dw
				m_InfoChrs[i].iItemShoesDurability		= 0;//CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
			}
			*/
		} break;

		case N3_GAMESTART: {

			char ID[0xFF] = {};

			Byte IDLen = GetByte(pData, index);
			GetString((Byte*)ID, pData, IDLen, index);

			printf("DB: (N3_GAMESTART) ID = \"%s\"\n", ID);

			/*
			CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->IDString().size());		// 아이디 길이 패킷에 넣기..
			CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());			// 아이디 문자열 패킷에 넣기..
			*/

			SendMyInfo();

			//UserInOut( USER_REGENE );
			//m_pMain->UserInOutForMe(this);
			//m_pMain->NpcInOutForMe(this);

			SendNotice();

			//SendTimeStatus();
			/*
			if(m_pMain->m_bPermanentChatMode) {		// If there is a permanent chat available!!!
				int buffindex = 0;
				char buff[1024]; memset( buff, 0x00, 1024 );

				SetByte( buff, WIZ_CHAT, buffindex );
				SetByte( buff, PERMANENT_CHAT, buffindex );
				SetByte( buff, 0x01, buffindex );		// Nation
				SetShort( buff, -1, buffindex );		// sid
				SetShort( buff, strlen(m_pMain->m_strPermanentChat), buffindex );
				SetString( buff, m_pMain->m_strPermanentChat, strlen(m_pMain->m_strPermanentChat), buffindex );
				Send( buff, buffindex );			
			}
			*/
		} break;

		case N3_CHECK_SPEEDHACK: {

			Byte bInit = GetByte(pData, index);
			float time = Getfloat(pData, index);

			printf("DB: (N3_CHECK_SPEEDHACK) is this the first? (%d) with time %f\n", bInit, time);

			/*
			s_pSocket->MP_AddByte(byBuff, iOffset, bInit);				// 서버가 기준 시간으로 쓸 타입 true 이면 기준시간 false면 체크타입
			s_pSocket->MP_AddFloat(byBuff, iOffset, fTime);				// 클라이언트 시간
			*/
		} break;

		case N3_GAME_SERVER_LOGIN: {
			short sUsernameLen = GetShort(pData, index);
			char* pUsername = (char*) calloc((sUsernameLen+1), sizeof(char));
			GetString((Byte*)pUsername, pData, sUsernameLen, index);
			short sPasswordLen = GetShort(pData, index);
			char* pPassword = (char*) calloc((sPasswordLen+1), sizeof(char));
			GetString((Byte*)pPassword, pData, sPasswordLen, index);

			printf("DB: (N3_GAME_SERVER_LOGIN) Username \"%s\", Password \"%s\" attempted to login\n", pUsername, pPassword);

			free(pUsername); free(pPassword);


			int send_index = 0;
			Byte send_buf[1024];
			memset(send_buf, 0x00, 1024);

			// NOTE: send 0x01 for nation
			SetByte(send_buf, N3_GAME_SERVER_LOGIN, send_index);
			SetByte(send_buf, 0x01, send_index);

			if(Send(send_buf, send_index) < send_index) {
				printf("ER: %s\n", SDLNet_GetError());
			}
		} break;

		case N3_VERSION_CHECK: {
			int send_index = 0;
			Byte send_buf[1024];
			memset(send_buf, 0x00, 1024);

			SetByte(send_buf, N3_VERSION_CHECK, send_index);
			SetShort(send_buf, CURRENT_VERSION, send_index);

			if(Send(send_buf, send_index) < send_index) {
				printf("ER: %s\n", SDLNet_GetError());
			}
		} break;

		case N3_ACCOUNT_LOGIN: {

			short sUsernameLen = GetShort(pData, index);
			char* pUsername = (char*) calloc((sUsernameLen+1), sizeof(char));
			GetString((Byte*)pUsername, pData, sUsernameLen, index);
			short sPasswordLen = GetShort(pData, index);
			char* pPassword = (char*) calloc((sPasswordLen+1), sizeof(char));
			GetString((Byte*)pPassword, pData, sPasswordLen, index);

			printf("DB: (N3_ACCOUNT_LOGIN) Username \"%s\", Password \"%s\" attempted to login\n", pUsername, pPassword);

			free(pUsername); free(pPassword);


			int send_index = 0;
			Byte send_buf[1024];
			memset(send_buf, 0x00, 1024);

			// NOTE: send 0x01 for successful login
			SetByte(send_buf, N3_ACCOUNT_LOGIN, send_index);
			SetByte(send_buf, 0x01, send_index);

			if(Send(send_buf, send_index) < send_index) {
				printf("ER: %s\n", SDLNet_GetError());
			}

			/*
			CAPISocket::MP_AddShort(byBuff, iOffset, s_szAccount.size());	// 아이디 길이..
			CAPISocket::MP_AddString(byBuff, iOffset, s_szAccount);		// 실제 아이디..
			CAPISocket::MP_AddShort(byBuff, iOffset, s_szPassWord.size());	// 패스워드 길이
			CAPISocket::MP_AddString(byBuff, iOffset, s_szPassWord);		// 실제 패스워드
			*/
		} break;

		case N3_GAMESERVER_GROUP_LIST: {

			int send_index = 0;
			Byte send_buf[1024];
			memset(send_buf, 0x00, 1024);

			Byte byServerCount = 0x01;
			char ServerIP[] = "127.0.0.1";
			char ServerName[] = "Testing";
			short sUserCount = 50;

			SetByte(send_buf, N3_GAMESERVER_GROUP_LIST, send_index);
			SetByte(send_buf, byServerCount, send_index);
			SetShort(send_buf, strlen(ServerIP), send_index);
			SetString(send_buf, ServerIP, strlen(ServerIP), send_index);
			SetShort(send_buf, strlen(ServerName), send_index);
			SetString(send_buf, ServerName, strlen(ServerName), send_index);
			SetShort(send_buf, sUserCount, send_index);

			if(Send(send_buf, send_index) < send_index) {
				printf("ER: %s\n", SDLNet_GetError());
			}

			/*
			int iServerCount = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 서버 갯수
			for(int i = 0; i < iServerCount; i++)
			{
				int iLen = 0;
				__GameServerInfo GSI;
				iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, GSI.szIP, iLen);
				iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, GSI.szName, iLen);
				GSI.iConcurrentUserCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 현재 동시 접속자수..
		
				m_pUILogIn->ServerInfoAdd(GSI); // ServerList
			}

			m_pUILogIn->ServerInfoUpdate();
			*/
		} break;

		/*
		case WIZ_LOGIN: {
			LoginProcess(pData+index);
		} break;

		case WIZ_GAMESTART: {
			if(m_State == STATE_GAMESTART)
				break;
			m_State = STATE_GAMESTART;
			UserInOut( 0x01 );
			m_pMain->UserInOutForMe(this);
		} break;

		case WIZ_MOVE: {
			MoveProcess(pData+index);
		} break;
		*/

		default: {
			printf("DB: Need to take care of packet 0x%02X\n", command);
			running = 0;
		} break;
	}
}

//-----------------------------------------------------------------------------
void CUser::SendNotice(void) {
	int send_index = 0, buff_index = 0;
	char send_buff[2048], buff[1024];
	memset( buff, NULL, 1024 );
	memset( send_buff, NULL, 2048 );

	char string[] = "Hello 123 Testing 123";

	int count = 0;
	SetByte((Byte*)send_buff, N3_NOTICE, send_index);
	for( int i=0; i<20; i++ ) {
		if( !strlen(string) )
			continue;
		SetByte((Byte*)buff, strlen(string), buff_index);
		SetString((Byte*)buff, string, strlen(string), buff_index);
		count++;
	}

	SetByte((Byte*)send_buff, count, send_index);
	SetString((Byte*)send_buff, buff, buff_index, send_index);
	Send((Byte*)send_buff, send_index);
}

//-----------------------------------------------------------------------------
void CUser::SendMyInfo(void) {
	int send_index = 0;
	Byte send_buf[1024];
	memset(send_buf, 0x00, 1024);

	SetByte(send_buf, N3_MYINFO, send_index);

	SetShort(send_buf, 0x00, send_index);
	SetShort(send_buf, 0x04, send_index);
	SetString(send_buf, "Test", 4, send_index);

	// NOTE: actual player position
	SetShort(send_buf, player_x, send_index);
	SetShort(send_buf, player_z, send_index);
	SetShort(send_buf, player_y, send_index);



	// probably want to hit this client function as well MsgRecv_UserInRequested

	if(Send(send_buf, send_index) < send_index) {
		printf("ER: %s\n", SDLNet_GetError());
	}
}

void CUser::LoginProcess(Byte *pBuf )
{
	/*
	int index = 0, idlen = 0, idindex = 0, send_index = 0;
	CUser* pUser = NULL;
	Byte send_buf[1024];
	memset( send_buf, 0x00, 1024 );

	idlen = (int)GetByte( pBuf, index );
	GetString((Byte*) m_UserId, pBuf, idlen, index );
//	m_curx = Getfloat( pBuf, index );
//	m_curz = Getfloat( pBuf, index );
//	m_cury = Getfloat( pBuf, index );

//	idindex = GetShort( pBuf, index );

	pUser = m_pMain->GetUserPtr( m_UserId );
	if( pUser && (pUser->m_Sid != m_Sid) ) {
		//pUser->Close();
		SetByte( send_buf, WIZ_LOGIN, send_index );
		SetByte( send_buf, 0x02, send_index );		// Login Fail... 
//		SetShort( send_buf, idindex, send_index );
	}
	else {
		m_curx = m_pMain->m_pInitPos[pUser->m_Sid%5].initx;
		m_curz = m_pMain->m_pInitPos[pUser->m_Sid%5].initz;

		SetByte( send_buf, WIZ_LOGIN, send_index );
		SetByte( send_buf, 0x01, send_index );		// Login Success...
		SetShort( send_buf, m_Sid, send_index );
		Setfloat( send_buf, m_curx, send_index );
		Setfloat( send_buf, m_curz, send_index );
//		SetShort( send_buf, idindex, send_index );

		CString logstr;
		logstr.Format("Login Success - %s", m_UserId);
		m_pMain->m_StatusList.AddString(logstr);
		m_pMain->m_nCount++;

	}

	Send( send_buf, send_index );
	//TRACE("Send Login Result - %s\n", m_UserId);
	*/
}

void CUser::MoveProcess(Byte *pBuf )
{
	int index = 0;

	short x = GetShort(pBuf, index);
	short z = GetShort(pBuf, index);
	short y = GetShort(pBuf, index);
	short speed = GetShort(pBuf, index);
	Byte moveflag = GetByte(pBuf, index);

	printf("DB: (N3_MOVE) X:%d, Y:%d, Z:%d, SPEED:%d, MOVEFLAG:%d\n", x, y, z, speed, moveflag);

	/*
	CAPISocket::MP_AddWord(byBuff, iOffset, vPos.x*10);			// 다음 위치
	CAPISocket::MP_AddWord(byBuff, iOffset, vPos.z*10);
	CAPISocket::MP_AddShort(byBuff, iOffset, vPos.y*10);
	CAPISocket::MP_AddWord(byBuff, iOffset, fSpeed*10);			// 속도 
	CAPISocket::MP_AddByte(byBuff, iOffset, byMoveFlag );		// 움직임 플래그..
	*/

	/*
	int index = 0, send_index = 0, region = 0;
	float will_x, will_z, will_y, server_y;
	Byte send_buf[1024];
	memset( send_buf, 0x00, 1024 );

	will_x = Getfloat( pBuf, index );
	will_z = Getfloat( pBuf, index );
	will_y = Getfloat( pBuf, index );

	server_y = m_pMain->m_Map.GetHeight( will_x, will_z );
	if( (server_y > will_y+1) || (server_y < will_y-1) )
		will_y = server_y;

	m_RegionX = (int)(will_x / VIEW_DIST);
	m_RegionZ = (int)(will_z / VIEW_DIST);
	region = m_pMain->GetRegionCount(m_RegionX, m_RegionZ );
	*/

	//-------------

	/*
	SetByte( send_buf, WIZ_MOVE, send_index );
	SetShort( send_buf, m_Sid, send_index );
	Setfloat( send_buf, m_curx, send_index );
	Setfloat( send_buf, m_curz, send_index );
	Setfloat( send_buf, m_cury, send_index );
	Setfloat( send_buf, will_x, send_index );
	Setfloat( send_buf, will_z, send_index );
	Setfloat( send_buf, will_y, send_index );
//	SetShort( send_buf, m_pMain->m_nCount, send_index );
//	SetShort( send_buf, region, send_index );

	m_curx = will_x;
	m_curz = will_z;
	m_cury = will_y;

	//m_pMain->Send_Region( send_buf, send_index, m_RegionX, m_RegionZ );
	*/
}

void CUser::Initialize()
{
	//m_pMain = (CEbenezerDlg*)AfxGetMainWnd();

	memset( m_UserId, 0x00, MAX_ID_SIZE );
	m_curx = 0.0f;
	m_curz = 0.0f;
	m_RegionX = 0;
	m_RegionZ = 0;
}

void CUser::UserInOut(Byte Type)
{
	int send_index = 0;
	Byte buff[1024];
	memset( buff, 0x00, 1024 );

	SetByte( buff, WIZ_USERINOUT, send_index );
	SetByte( buff, Type, send_index );
	SetShort( buff, m_Sid, send_index );
	SetShort( buff, strlen(m_UserId), send_index );
	SetString( buff, m_UserId, strlen(m_UserId), send_index );
	Setfloat( buff, m_curx, send_index );
	Setfloat( buff, m_curz, send_index );

	m_pMain->Send_All( buff, send_index );
}
