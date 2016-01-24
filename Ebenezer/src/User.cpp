// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

//#include "Ebenezer.h"
#include "User.h"
#include "EbenezerDlg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser()
{

}

CUser::~CUser()
{

}

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
		case N3_ACCOUNT_LOGIN: {

			short sUsernameLen = GetShort(pData, index);
			char* pUsername = (char*) calloc((sUsernameLen+1), sizeof(char));
			GetString((Byte*)pUsername, pData, sUsernameLen, index);
			short sPasswordLen = GetShort(pData, index);
			char* pPassword = (char*) calloc((sPasswordLen+1), sizeof(char));
			GetString((Byte*)pPassword, pData, sPasswordLen, index);

			printf("DB: Username \"%s\", Password \"%s\" attempted to login\n", pUsername, pPassword);

			free(pUsername); free(pPassword);

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

	//TRACE("Project WILL: %.2f, %.2f, %.2f", m_curx, m_cury, m_curz );

	m_pMain->Send_Region( send_buf, send_index, m_RegionX, m_RegionZ );

	//TRACE("Send Move Result - %s\n", m_UserId);
}

void CUser::Initialize()
{
	//m_pMain = (CEbenezerDlg*)AfxGetMainWnd();

	/*
	int send_index = 0;
	Byte send_buf[1024];
	memset(send_buf, 0x00, 1024);

	SetByte(send_buf, N3_VERSION_CHECK, send_index);
	SetShort(send_buf, CURRENT_VERSION, send_index);

	if(Send(send_buf, send_index) < send_index) {
		printf("ER: %s\n", SDLNet_GetError());
	}
	*/

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
