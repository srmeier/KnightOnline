// EbenezerDlg.cpp : implementation file
//

#include "EbenezerDlg.h"
#include "Define.h"
#include "User.h"


CIOCPort	CEbenezerDlg::m_Iocport;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg dialog

//CEbenezerDlg::CEbenezerDlg(void) {}

/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg message handlers

bool CEbenezerDlg::OnInitDialog()
{
	if(SDL_Init(SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0) {
		fprintf(stderr, "ERROR: \"%s\"\n", SDL_GetError());
		return false;
	}
	
	if(!m_Iocport.Init(this, MAX_USER, 1)) return false;

	for(int i=0; i<MAX_SOCKET; ++i) {
		m_Iocport.m_SockArrayInActive[i] = new CUser();
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


CUser* CEbenezerDlg::GetUserPtr(char *userid)
{
	CUser* pUser = NULL;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser ) {
			if( !_strnicmp( pUser->m_UserId, userid, MAX_ID_SIZE ) )
				break;
		}
	}

	return pUser;
}

void CEbenezerDlg::Send_All(Byte *pBuf, int len, bool tome)
{
	CUser* pUser = NULL;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && (pUser->GetState() == STATE_GAMESTART) ) {
			pUser->Send( pBuf, len );
		}
	}
}

void CEbenezerDlg::Send_Region(Byte *pBuf, int len, int x, int z, bool tome)
{
	CUser* pUser = NULL;

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && (pUser->GetState() == STATE_GAMESTART) ) {
			if( pUser->m_RegionX == x && pUser->m_RegionZ == z )
				pUser->Send( pBuf, len );
		}
	}
}

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

void CEbenezerDlg::UserInOutForMe(CUser *pSendUser)
{
	int send_index = 0;
	CUser* pUser = NULL;
	Byte buff[1024];
	memset( buff, 0x00, 1024 );

	for(int i=0; i<MAX_USER; i++) {
		pUser = (CUser*)m_Iocport.m_SockArray[i];
		if( pUser && pUser->GetSocketID() == pSendUser->GetSocketID() )
			continue;
		if( pUser && pUser->GetState() != STATE_DISCONNECTED ) {
			SetByte( buff, WIZ_USERINOUT, send_index );
			SetByte( buff, 0x01, send_index );
			SetShort( buff, pUser->GetSocketID(), send_index );
			SetShort( buff, strlen(pUser->m_UserId), send_index );
			SetString( buff, pUser->m_UserId, strlen(pUser->m_UserId), send_index );
			Setfloat( buff, pUser->m_curx, send_index );
			Setfloat( buff, pUser->m_curz, send_index );

			pSendUser->Send( buff, send_index );
		}
	}
}
