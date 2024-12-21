// LauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include <wininet.h>

#include "PacketDef.h"
#include "APISocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLauncherDlg dialog

CLauncherDlg::CLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLauncherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLauncherDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	
	m_pSocket = NULL;

	memset( m_strServiceName, NULL, 256 );
	m_nCurVersion = 0;
	m_nServerVersion = 0;
	m_nGetFileNum = 0;
}



void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLauncherDlg)
	DDX_Control(pDX, IDC_STATUS, m_Status);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLauncherDlg, CDialog)
	//{{AFX_MSG_MAP(CLauncherDlg)
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CLauncherDlg message handlers

BOOL CLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog() ;

	CString szInfo; szInfo.LoadString(IDS_INFO_VERSION_CHECK);
	m_Status.SetWindowText(szInfo); // 화면에 표시..
			
	m_progress.SetColor(RGB(64, 255, 64)); // 프로그래스 색을 정한다.

	m_pSocket = new CAPISocket();

	CString szProduct, szKey = "SOFTWARE\\";
	szProduct.LoadString(IDS_PRODUCT);
	szKey += szProduct;

	m_hRegistryKey = NULL;
	long lStatus = RegOpenKey(HKEY_CURRENT_USER, szKey, &m_hRegistryKey);
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_OPEN); MessageBox(szErr); exit(-1); }

	DWORD dwType = REG_SZ; DWORD dwBytes = 0;
	char szBuff[256] = "";

	dwType = REG_DWORD; dwBytes = 4;
	lStatus = RegQueryValueEx(m_hRegistryKey, "VERSION", NULL, &dwType, (BYTE*)(&m_nCurVersion), &dwBytes);
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_READ_VERSION); MessageBox(szErr); exit(-1); }

	for(int j = 0 ; j < MAX_DOWNLOAD_FILE ; j++)
		m_nVersionNum[j] = m_nCurVersion;

	dwType = REG_SZ; dwBytes = 256;
	lStatus = RegQueryValueEx(m_hRegistryKey, "PATH", NULL, &dwType, (BYTE*)szBuff, &dwBytes); // 인스톨 경로
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_READ_PATH); MessageBox(szErr); exit(-1); }
	m_szInstalledPath = szBuff;

	dwType = REG_SZ; dwBytes = 256;
	lStatus = RegQueryValueEx(m_hRegistryKey, "EXE", NULL, &dwType, (BYTE*)szBuff, &dwBytes); // 실행파일 이름
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_READ_EXE); MessageBox(szErr); exit(-1); }
	m_szExeName = szBuff;

	dwType = REG_SZ; dwBytes = 256;
	lStatus = RegQueryValueEx(m_hRegistryKey, "SERVICE", NULL, &dwType, (BYTE*)m_strServiceName, &dwBytes); // 서비스 이름..
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_READ_SERVICE); MessageBox(szErr); exit(-1); }


	
	
	
	
	
	
	
	
	
	// 소켓 접속..
	char szIniPath[_MAX_PATH] = "";
	::GetCurrentDirectory(_MAX_PATH, szIniPath);
	lstrcat(szIniPath, "\\Server.Ini");
	int iServerCount = GetPrivateProfileInt("Server", "Count", 0, szIniPath);

	char szIPs[256][128]; memset(szIPs, 0, sizeof(szIPs));
	for(int i = 0; i < iServerCount; i++)
	{
		char szKey[32] = "";
		sprintf(szKey, "IP%d", i);
		GetPrivateProfileString("Server", szKey, "", szIPs[i], 32, szIniPath);
	}

	if(iServerCount > 0)
	{
		int iServer = rand()%iServerCount;
		while( !m_pSocket->Connect(m_hWnd, szIPs[iServer], CONNECT_PORT) )
		{
			int iErrCode = GetLastError();
			CString szFmt; szFmt.LoadString(IDS_FMT_FAILED_CONNECT_LOGIN_SERVER);
			CString szErr; szErr.Format(szFmt, iErrCode);
			if( MessageBox(szErr, "", MB_YESNO) == IDNO )
			{
				AfxPostQuitMessage(0);
				return FALSE;
			}
			else
				continue;
		}
	}
	else
	{
		CString szErr; szErr.LoadString(IDS_ERR_INVALID_SERVER_COUNT);
		this->MessageBox(szInfo); // 끝낸다.
		PostQuitMessage(0);
	}

	char titlebar[256];
	memset( titlebar, NULL, 256 );
	sprintf( titlebar, "%s AUTO UPGRADE LAUNCHER", m_strServiceName );
	SetWindowText( titlebar );

	this->PacketSend_VersionReq();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
BOOL CLauncherDlg::DestroyWindow() 
{
	if(m_pSocket) {
		m_pSocket->Disconnect();
		delete m_pSocket;
		m_pSocket = NULL;
	}
	
	return CDialog::DestroyWindow();
}

CString CLauncherDlg::GetProgPath()
{
	char Buf[256], Path[256];
	char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];

	::GetModuleFileName(AfxGetApp()->m_hInstance, Buf, 256);
	_splitpath(Buf,drive,dir,fname,ext);
	strcpy(Path, drive);
	strcat(Path, dir);		
	CString _Path = Path;
	return _Path;
}

void CLauncherDlg::PacketSend_VersionReq()
{
	int iOffset = 0;
	BYTE byBuffs[128];
	memset( byBuffs, NULL, 128 );

	m_pSocket->MP_AddByte( byBuffs, iOffset, VERSION_REQ );
	m_pSocket->MP_AddShort( byBuffs, iOffset , m_nCurVersion );
	
	m_pSocket->Send(byBuffs, iOffset);
}

void CLauncherDlg::PacketSend_DownloadInfo()
{
	int iOffset = 0;
	BYTE byBuffs[128];
	memset( byBuffs, NULL, 128 );

	m_pSocket->MP_AddByte( byBuffs, iOffset, DOWNLOAD_INFO_REQ );
	m_pSocket->MP_AddShort( byBuffs, iOffset, (short)m_nCurVersion );
	
	m_pSocket->Send(byBuffs, iOffset);
}

void CLauncherDlg::PacketProcess(BYTE *pBuf, int size)
{
	if (NULL==pBuf) return;

	BYTE command;
	int iIndex = 0;
	command = m_pSocket->Parse_GetByte( pBuf, iIndex );
	switch(command)
	{
		case VERSION_REQ:		this->PacketReceive_Version(pBuf, iIndex); break;
		case DOWNLOAD_INFO_REQ:	this->PacketReceive_DownloadInfo( pBuf, iIndex ); break;
	}
}

void CLauncherDlg::PacketReceive_DownloadInfo(const BYTE *pBuf, int& iIndex)
{
	int iLen = 0;
	
	iLen = m_pSocket->Parse_GetShort( pBuf, iIndex );
	m_pSocket->Parse_GetString( pBuf, iIndex, m_szFtpUrl, iLen );
	iLen = m_pSocket->Parse_GetShort( pBuf, iIndex );
	m_pSocket->Parse_GetString( pBuf, iIndex, m_szFtpPath, iLen );
	m_nGetFileNum = m_pSocket->Parse_GetShort( pBuf, iIndex );
	
	if( m_nGetFileNum < 1 || m_nGetFileNum >= 32)
	{
		CString szErr; szErr.LoadString(IDS_ERR_INVALID_DOWNLOAD_FILE_COUNT);
		MessageBox(szErr);
		AfxPostQuitMessage(0);
	}
	
	char szBuf[MAX_PATH];
	std::string szVersion;
	int nVersion = 0;
	for( int i=0; i<m_nGetFileNum; i++ )
	{
		iLen = m_pSocket->Parse_GetShort( pBuf, iIndex );
		m_pSocket->Parse_GetString( pBuf, iIndex, m_szGetFileNames[i], iLen );

		sscanf(m_szGetFileNames[i].c_str(), "patch%s", szBuf);
		szVersion = szBuf;
		szVersion.resize(4);
		nVersion = atoi(szVersion.c_str());
		if(m_nVersionNum[i] < nVersion)
			m_nVersionNum[i] = nVersion;
	}

	DownloadProcess();
}

void CLauncherDlg::PacketReceive_Version(const BYTE *pBuf, int &iIndex)
{
	m_nServerVersion = m_pSocket->Parse_GetShort( pBuf, iIndex );
	if( m_nCurVersion == m_nServerVersion ) // 버전이 일치하면.. 
	{
		this->StartGame(); // 게임 실행..
	}
	else if( m_nCurVersion < m_nServerVersion ) // 버전이 낮으면..
	{
		PacketSend_DownloadInfo(); // 다운로드 요청..
	}
	else 
	{
		CString szErr; szErr.LoadString(IDS_ERR_INVALID_VERSION);
		MessageBox(szErr);
		PostQuitMessage(-1);
	}
}

void CLauncherDlg::StartGame()
{
	CString szCmd = GetCommandLine(); // 커맨드 라인을 가져오고..
	char szApp[_MAX_PATH] = "";
	GetModuleFileName(NULL, szApp, _MAX_PATH);
	int iML = lstrlen(szApp);

	CString szParam;
	if(iML >= 0)
	{
		int ii = szCmd.Find(szApp);
		if(ii >= 0 && szCmd.GetLength() > ii + iML + 2)
			szParam = szCmd.Mid(ii + iML + 2);
	}

	std::string szExeFN = m_szInstalledPath + "\\" + m_szExeName; // 실행 파일 이름 만들고..
	::ShellExecute(NULL, "open", szExeFN.c_str(), szParam, m_szInstalledPath.c_str(), SW_SHOWNORMAL); // 게임 실행..

	PostQuitMessage(0);
}

void CLauncherDlg::DownloadProcess()
{
	if( !FTP_Open() )
	{
		PostQuitMessage(0);
		return;
	}

	m_progress.SetRange(0, 100);
	m_progress.SetPos(0);

	std::string		szFullPath;
	std::string		szLocalFName;

	bool bExtractSuccess = true;

	for( int i=0; i<m_nGetFileNum; i++ )
	{
		szFullPath = m_szFtpPath + "/" + m_szGetFileNames[i];
		BOOL bDownloadSuccess = GetDownloadFile( szFullPath, m_szGetFileNames[i] );
		while(!bDownloadSuccess)
		{
			CString szErr; szErr.LoadString(IDS_ERR_DOWNLOAD_PATCH_FILE_AND_RETRY); // 다시 시도할까여??
			int iID = MessageBox(szErr, "Patch error", MB_YESNO);
			if(IDYES == iID) bDownloadSuccess = GetDownloadFile( szFullPath, m_szGetFileNames[i] );
			else 
			{
				AfxPostQuitMessage(-1);
				break;
			}
		}

		if(bDownloadSuccess)
		{
			szLocalFName = m_szInstalledPath + "\\" + m_szGetFileNames[i];

			CString szInfo; szInfo.LoadString(IDS_INFO_EXTRACTING);
			m_Status.SetWindowText(szInfo);

			if(false == ArchiveClose()) { bExtractSuccess = false; break; }
			if(false == ArchiveOpen(szLocalFName.c_str())) { bExtractSuccess = false; break; }
			if(false == ArchiveExtract(m_szInstalledPath.c_str())) { bExtractSuccess = false; break; }
			if(false == ArchiveClose()) { bExtractSuccess = false; break; }

			CFile file;
			if( file.Open( szLocalFName.c_str(), CFile::modeRead | CFile::shareDenyNone , NULL ) )
			{
				file.Close();
				file.Remove(szLocalFName.c_str());
				if(m_hRegistryKey) // 압축 풀기와 쓰기, 압축 파일 삭제에 성공하면 버전을 쓰고..
				{
					RegSetValueEx(m_hRegistryKey, "VERSION", NULL, REG_DWORD, ((BYTE*)(&m_nVersionNum[i])), 4);
				}
			}
			else
			{
				bExtractSuccess = false;
				break;
			}
		}
		else
		{
			FTP_Close();
			CString szErr; szErr.LoadString(IDS_ERR_DOWNLOAD_PATCH_FILE);
			MessageBox(m_szGetFileNames[i].c_str(), szErr);
			PostQuitMessage(0);
			bExtractSuccess = false;
			break;
		}
	}

	FTP_Close();

//	CString inipath, version;
//	inipath.Format( "%s\\server.ini", GetProgPath() );
//	itoa( m_nServerVersion, (char*)(LPCTSTR)version, 10 );
//	WritePrivateProfileString("VERSION","CURRENT",version, inipath);

	if(true == bExtractSuccess && m_hRegistryKey) // 압축 풀기와 쓰기, 압축 파일 삭제에 성공하면 버전을 쓰고..
	{
		long lStatus = RegSetValueEx(m_hRegistryKey, "VERSION", NULL, REG_DWORD, ((BYTE*)(&m_nServerVersion)), 4);
		this->StartGame(); // 게임 실행..
	}
	else
	{
		CString szErr; szErr.LoadString(IDS_ERR_PATCH);
		MessageBox(szErr);
		PostQuitMessage(-1);
	}
}

BOOL CLauncherDlg::FTP_Open()
{
	m_hInetSession = InternetOpen("3DOnline",
							  INTERNET_OPEN_TYPE_PRECONFIG,
							  NULL,
							  NULL,
							  0);
	if (!m_hInetSession)
	{
		CString szErr; szErr.LoadString(IDS_ERR_INIT_INTERNET);
		MessageBox(m_szFtpUrl.c_str(), szErr);
		return FALSE;
	}

	m_hFtpConnection = InternetConnect(m_hInetSession,
									   m_szFtpUrl.c_str(),
									   INTERNET_DEFAULT_FTP_PORT,
									   "anonymous",
									   "download",
									   INTERNET_SERVICE_FTP,
									   INTERNET_FLAG_PASSIVE,
									   0);

	if (!m_hFtpConnection)
	{
		CString szErr; szErr.LoadString(IDS_ERR_CONNECT_FTP);
		MessageBox(m_szFtpUrl.c_str(), szErr);
		return FALSE;
	}

	return TRUE;
}

void CLauncherDlg::FTP_Close()
{
	if (m_hFtpConnection)
	{
		InternetCloseHandle(m_hFtpConnection);
		m_hFtpConnection = NULL;
	}

	if (m_hInetSession)
	{
		InternetCloseHandle(m_hInetSession);
		m_hInetSession = NULL;
	}
}

BOOL CLauncherDlg::GetDownloadFile(const std::string& szFtpUrl, const std::string& szFileName)
{
	if (!m_hFtpConnection)
		return FALSE;

	DWORD dwFileSize = IsFtpExistFile( szFtpUrl, szFileName );

	// open the file
	HINTERNET hFile;

	//CInternetFile *pFile;
	hFile = FtpOpenFile(m_hFtpConnection,
						szFtpUrl.c_str(),
						GENERIC_READ,
						FTP_TRANSFER_TYPE_BINARY,
						0);
	
	if (!hFile) return FALSE;

	// read & save the file...
	std::string		szLocalFName;
	szLocalFName = m_szInstalledPath + "\\" + szFileName;

	FILE *fp = fopen(szLocalFName.c_str(), "wb");
	if (fp == NULL)
	{
		MessageBox("Can`t open local file");

		InternetCloseHandle(hFile);
		return TRUE;
	}

	DWORD dwReadSize, dwTemp;
	char  Buffer[1024];
	BOOL  bRes;
	dwReadSize = 0;
	DWORD dwElaspedTime = 0;
	DWORD dwCurrent;
	MSG pMsg;
	BOOL bPeekMessage;

	DWORD dwLastTime = ::GetTickCount ();
	
	while (dwReadSize < dwFileSize)
	{
		bRes = InternetReadFile(hFile, Buffer, 1024, &dwTemp);
		fwrite(Buffer, 1, dwTemp, fp);
		dwReadSize += dwTemp;

		if ((dwFileSize/1024) == 0 )
			m_progress.SetPos( (dwReadSize/1024) * 100 / 1 );
		else
			m_progress.SetPos( (dwReadSize/1024) * 100 / (dwFileSize/1024) );

		dwCurrent = ::GetTickCount ();
		dwElaspedTime += dwCurrent - dwLastTime;

		std::string szInfo; szInfo = szFileName + " Downloading...";
		m_Status.SetWindowText(szInfo.c_str());

		bPeekMessage = ::PeekMessage(&pMsg, NULL, NULL, NULL, PM_REMOVE);
		if(bPeekMessage)
		{
			TranslateMessage(&pMsg);
			DispatchMessage(&pMsg);
		}

		dwLastTime = dwCurrent;

//		if ( m_fCancel || !bRes )
//			break;

//		Sleep(100);
	}
	fclose(fp);

	//if (dwReadSize != dwFileSize)
	if (dwReadSize < dwFileSize)
	{
		InternetCloseHandle(hFile);

/*		if ( m_fCancel )
		{
			MessageBox("File Size Error");
			//// added by manseek for DOWNLOAD
			CString sLink;
			sLink.Format("http://download.mgame.com/download/%s.exe", (LPCTSTR)m_strGameID );
			HINSTANCE t = ShellExecute(GetSafeHwnd(), "open", sLink, NULL,  NULL, SW_SHOWNORMAL);
			//// 2001.5.23

			return FALSE;
		};
*/
		MessageBox("File Size Error");

		return FALSE;
	}

	// close the file
	InternetCloseHandle(hFile);

	return TRUE;
}

BOOL CLauncherDlg::IsFtpExistFile(const std::string& szFtpUrl, const std::string& szFileName)
{
	CString strNameU, strNameL, strName;

	strNameU = szFtpUrl.c_str();
	strNameU.MakeUpper();

	strNameL = szFtpUrl.c_str();
	strNameL.MakeLower();

	CString strSub1, strSub2;

	CString strTemp;
	strTemp = szFileName.c_str();
	

	strSub1 = strNameU.Left( strNameU.GetLength() - strTemp.GetLength() + 1);
	strSub2 = strNameU.Right( strTemp.GetLength() - 1);
	
	strSub2.MakeLower();

	strName = strSub1+strSub2;


	if (!m_hFtpConnection)
		return 0;

	// find the file...
	WIN32_FIND_DATA FindFileData;
	HINTERNET hFind;

	hFind = FtpFindFirstFile(m_hFtpConnection,
							 (LPCTSTR)strName,
							 &FindFileData,
							 0,
							 0);
	if (!hFind)
	{	
		hFind = FtpFindFirstFile(m_hFtpConnection,
							 (LPCTSTR)strNameU,
							 &FindFileData,
							 0,
							 0);
		
		if (!hFind)
		{
			hFind = FtpFindFirstFile(m_hFtpConnection,
							 (LPCTSTR)strNameL,
							 &FindFileData,
							 0,
							 0);
		
			if (!hFind)
				return 0;
		}
	}

	InternetCloseHandle(hFind);

	return FindFileData.nFileSizeLow;
}

bool CLauncherDlg::ArchiveClose()
{
	int berr = false;
	
	//// ArchiveClose
	try
	{
		m_zip.Close(false);	
	}
	catch (CException* e)
	{
		e->Delete();
		berr = true;
	}
	catch(...) // thrown in the STL version
	{
		berr = true;		
	}
	if (berr)
	{
		MessageBox("Archive close failed", "", MB_ICONSTOP);
		return false;
	}

	return true;
	///
}

bool CLauncherDlg::ArchiveOpen( CString OpenFileName )
{
	int berr = 0;

	//// ArchiveOpen
	do 
	{
		try
		{
			m_zip.Open(OpenFileName, CZipArchive::open, 1 );
			berr = 0;
		}
		catch (CZipException* e)
		{
			if (e->m_iCause == CZipException::cdirNotFound)
				berr = -1;
			else
				berr = 1;
			e->Delete();
		}
		catch (CException* e)
		{
			e->Delete();
			berr = 1;
			
		}
		// thrown in the STL version
		catch (CZipException e)
		{
			if (e.m_iCause == CZipException::cdirNotFound)
				berr = -1;
			else
				berr = 1;

		}
		catch(...) 
		{
			berr = 1;
		}
		if (berr == -1)
		{
			if (MessageBox("The central directory was not found. If you're opening a multi-disk archive, make sure you have inserted the last disk. Retry?", "", MB_ICONSTOP|MB_YESNO) == IDNO)
				berr = 1;
			else
				m_zip.Close(true);
		}

		if (berr == 1)
		{
			MessageBox("Archive open failed", "", MB_ICONSTOP);
			return false;
		}

	} while (berr == -1);
	////
	
	return true;
}

bool CLauncherDlg::ArchiveExtract( CString ExtractFolder )
{
	int nZipCount = m_zip.GetNoEntries();

	bool bErr = false;
	try
	{	
		for (int i = 0; i < nZipCount; i++)
		{
			m_zip.ExtractFile((WORD)i, ExtractFolder, true, NULL);//, Callback, (void*) &p);
		}
	}
	catch (CException* e)
	{
		e->Delete();
		bErr = true;

	}
	catch(...) // thrown in the STL version
	{
		bErr = true;
	}
	if (bErr)
	{
		m_zip.CloseFile(NULL, true);
		MessageBox("Extract failed", "", MB_ICONSTOP);
		return false;
	}

	return true;
}

void CLauncherDlg::LoadStringFromResource(DWORD dwID, std::string& szString)
{
	static char szBuffer[512];
	szBuffer[0] = NULL;
//	::LoadString(NULL, MAKEINTRESOURCE(dwID), szBuffer, 256);
	::LoadString(NULL, dwID, szBuffer, 256);
	szString = szBuffer;
}


LRESULT CLauncherDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
    {
		case WM_SOCKETMSG:
		{
			switch(WSAGETSELECTEVENT(lParam))
			{
			case FD_CONNECT:
				TRACE("Socket connected..\n");
				break;
//			case FD_ACCEPT:
//				break;
			case FD_CLOSE:
				TRACE("Socket closed..\n");
				break;
			case FD_READ:
				{
					m_pSocket->Receive();

					while ( m_pSocket->m_qRecvPkt.size() > 0 )			// 패킷 리스트에 패킷이 있냐????
					{
						int iOffset = 0;
						DataPack* pDataPack = m_pSocket->m_qRecvPkt.front();	// 큐의 첫번째 것을 복사..
						this->PacketProcess(pDataPack->m_pData, iOffset);		// 패킷을 처리할 상황이 아니다.
						delete pDataPack;
						m_pSocket->m_qRecvPkt.pop();							// 패킷을 큐에서 꺼냄..
					}
				}
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	}
	
	return CDialog::WindowProc(message, wParam, lParam);
}
