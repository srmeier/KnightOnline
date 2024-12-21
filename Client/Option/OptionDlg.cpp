// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Option.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_Option.InitDefault(); // 옵션 초기화
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Control(pDX, IDC_SLD_SOUND_DISTANCE, m_SldEffectSoundDist);
	DDX_Control(pDX, IDC_CB_COLORDEPTH, m_CB_ColorDepth);
	DDX_Control(pDX, IDC_CB_RESOLUTION, m_CB_Resolution);
	DDX_Control(pDX, IDC_SLD_VIEWING_DISTANCE, m_SldViewDist);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_B_APPLY_AND_EXECUTE, OnBApplyAndExecute)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_B_VERSION, OnBVersion)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// 각종 컨트롤 초기화..
	m_SldViewDist.SetRange(256, 512);
	m_SldEffectSoundDist.SetRange(20, 48);

	int iAdd = 0;
	iAdd = m_CB_Resolution.AddString("1024 X 768");		m_CB_Resolution.SetItemData(iAdd, MAKELPARAM(768,1024));
	iAdd = m_CB_Resolution.AddString("1280 X 1024");	m_CB_Resolution.SetItemData(iAdd, MAKELPARAM(1024,1280));
	iAdd = m_CB_Resolution.AddString("1600 X 1200");	m_CB_Resolution.SetItemData(iAdd, MAKELPARAM(1200,1600));

	iAdd = m_CB_ColorDepth.AddString("16 Bit");		m_CB_ColorDepth.SetItemData(iAdd, 16);
	iAdd = m_CB_ColorDepth.AddString("32 Bit");		m_CB_ColorDepth.SetItemData(iAdd, 32);


	// 레지스트리에서 설치된 폴더를 읽어온다..
	CString szProduct, szKey = "SOFTWARE\\";
	szProduct.LoadString(IDS_PRODUCT);
	szKey += szProduct;

	HKEY hRegKey = NULL;
	long lStatus = RegOpenKey(HKEY_CURRENT_USER, szKey, &hRegKey);
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_OPEN); MessageBox(szErr); exit(-1); }

	DWORD dwType = REG_SZ; DWORD dwBytes = 0;
	char szBuff[256] = "";

	// 실행 파일 경로
	dwType = REG_SZ; dwBytes = 256;
	lStatus = RegQueryValueEx(hRegKey, "PATH", NULL, &dwType, (BYTE*)szBuff, &dwBytes); // 인스톨 경로
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_READ_PATH); MessageBox(szErr); exit(-1); }
	m_szInstalledPath = szBuff;

	// 실행 파일 이름
//	dwType = REG_SZ; dwBytes = 256;
//	lStatus = RegQueryValueEx(hRegKey, "EXE", NULL, &dwType, (BYTE*)szBuff, &dwBytes); // 실행파일 이름
//	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_READ_EXE); MessageBox(szErr); exit(-1); }
//	m_szExeName = szBuff;
	m_szExeName = "Launcher.exe";

	// Version 표시
	DWORD dwVersion = 0;
	dwType = REG_DWORD; dwBytes = 4;
	lStatus = RegQueryValueEx(hRegKey, "VERSION", NULL, &dwType, (BYTE*)(&dwVersion), &dwBytes); // 버전
	if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_READ_VERSION); MessageBox(szErr); exit(-1); }
	SetDlgItemInt(IDC_E_VERSION, dwVersion);

	RegCloseKey(hRegKey);
	hRegKey = NULL;

	// 세팅을 읽어온다..
	this->SettingLoad(m_szInstalledPath + "\\Option.ini");
	this->SettingUpdate();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COptionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COptionDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COptionDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void COptionDlg::OnOK() 
{
	this->SettingSave(m_szInstalledPath + "\\Option.ini");

	CDialog::OnOK();
}

void COptionDlg::OnBApplyAndExecute() 
{
	CString szExeFN = m_szInstalledPath + "\\" + m_szExeName; // 실행 파일 이름 만들고..
	ShellExecute(NULL, "open", szExeFN, "", m_szInstalledPath, SW_SHOWNORMAL); // 게임 실행..

	this->OnOK();
}

void COptionDlg::SettingSave(CString szIniFile)
{
	if(szIniFile.GetLength() <= 0) return;

	char szIniPath[_MAX_PATH] = "";
	
	if(szIniFile.Find(":")) lstrcpy(szIniPath, szIniFile);
	else
	{
		::GetCurrentDirectory(_MAX_PATH, szIniPath);
		lstrcat(szIniPath, szIniFile);
	}
	
	CString szBuff;

	if(IsDlgButtonChecked(IDC_R_TEX_CHR_HIGH)) m_Option.iTexLOD_Chr = 0;
	else if(IsDlgButtonChecked(IDC_R_TEX_CHR_LOW)) m_Option.iTexLOD_Chr = 1;
	else m_Option.iTexLOD_Chr = 1;

	if(IsDlgButtonChecked(IDC_R_TEX_SHAPE_HIGH)) m_Option.iTexLOD_Shape = 0;
	else if(IsDlgButtonChecked(IDC_R_TEX_SHAPE_LOW)) m_Option.iTexLOD_Shape = 1;
	else m_Option.iTexLOD_Shape = 0;

	if(IsDlgButtonChecked(IDC_R_TEX_TERRAIN_HIGH)) m_Option.iTexLOD_Terrain = 0;
	else if(IsDlgButtonChecked(IDC_R_TEX_TERRAIN_LOW)) m_Option.iTexLOD_Terrain = 1;
	else m_Option.iTexLOD_Terrain = 1;

	if(IsDlgButtonChecked(IDC_C_SHADOW)) m_Option.iUseShadow = 1;
	else m_Option.iUseShadow = 0;

	int iSel = m_CB_Resolution.GetCurSel();

	m_Option.iViewWidth = 1024; m_Option.iViewHeight = 768;
	if(0 == iSel) { m_Option.iViewWidth = 1024; m_Option.iViewHeight = 768; }
	else if(1 == iSel) { m_Option.iViewWidth = 1280; m_Option.iViewHeight = 1024; }
	else if(2 == iSel) { m_Option.iViewWidth = 1600; m_Option.iViewHeight = 1200; }

	iSel = m_CB_ColorDepth.GetCurSel();
	if(CB_ERR != iSel)
	{
		m_Option.iViewColorDepth = m_CB_ColorDepth.GetItemData(iSel);
		if(m_Option.iViewColorDepth != 16 && m_Option.iViewColorDepth != 32)
			m_Option.iViewColorDepth = 16;
	}
	else 
		m_Option.iViewColorDepth = 16;

	m_Option.iViewDist = m_SldViewDist.GetPos();
	if(m_Option.iViewDist < 256) m_Option.iViewDist = 256;
	else if(m_Option.iViewDist > 512) m_Option.iViewDist = 512;

	m_Option.iEffectSndDist = m_SldEffectSoundDist.GetPos();
	if(m_Option.iEffectSndDist < 20) m_Option.iEffectSndDist = 20;
	else if(m_Option.iEffectSndDist > 48) m_Option.iEffectSndDist = 48;

	szBuff.Format("%d", m_Option.iTexLOD_Chr);		WritePrivateProfileString("Texture", "LOD_Chr", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iTexLOD_Shape);	WritePrivateProfileString("Texture", "LOD_Shape", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iTexLOD_Terrain);	WritePrivateProfileString("Texture", "LOD_Terrain", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iUseShadow);		WritePrivateProfileString("Shadow", "Use", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iViewWidth);		WritePrivateProfileString("ViewPort", "Width", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iViewHeight);		WritePrivateProfileString("ViewPort", "Height", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iViewColorDepth);  WritePrivateProfileString("ViewPort", "ColorDepth", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iViewDist);		WritePrivateProfileString("ViewPort", "Distance", szBuff, szIniPath);
	szBuff.Format("%d", m_Option.iEffectSndDist);	WritePrivateProfileString("Sound", "Distance", szBuff, szIniPath);

	m_Option.bSoundEnable = (IsDlgButtonChecked(IDC_C_SOUND_ENABLE)) ? true : false;
	m_Option.bSoundEnable ? szBuff = "1" : szBuff = "0";
	WritePrivateProfileString("Sound", "Enable", szBuff, szIniPath);

	m_Option.bSndDuplicated = (IsDlgButtonChecked(IDC_C_SOUND_DUPLICATE)) ? true : false;
	m_Option.bSndDuplicated ? szBuff = "1" : szBuff = "0";
	WritePrivateProfileString("Sound", "Duplicate", szBuff, szIniPath);

	m_Option.bWindowCursor = (IsDlgButtonChecked(IDC_C_CURSOR_WINDOW)) ? true : false;
	m_Option.bWindowCursor ? szBuff = "1" : szBuff = "0";
	WritePrivateProfileString("Cursor", "WindowCursor", szBuff, szIniPath);
}

void COptionDlg::SettingLoad(CString szIniFile)
{
	if(szIniFile.GetLength() <= 0) return;

	char szIniPath[_MAX_PATH] = "";
	
	if(szIniFile.Find(":")) lstrcpy(szIniPath, szIniFile);
	else
	{
		::GetCurrentDirectory(_MAX_PATH, szIniPath);
		lstrcat(szIniPath, "\\");
		lstrcat(szIniPath, szIniFile);
	}
	
	
	m_Option.iTexLOD_Chr = GetPrivateProfileInt("Texture", "LOD_Chr", 0, szIniPath);
	m_Option.iTexLOD_Shape = GetPrivateProfileInt("Texture", "LOD_Shape", 0, szIniPath);
	m_Option.iTexLOD_Terrain = GetPrivateProfileInt("Texture", "LOD_Terrain", 0, szIniPath);
	m_Option.iUseShadow = GetPrivateProfileInt("Shadow", "Use", 1, szIniPath);
	m_Option.iViewWidth = GetPrivateProfileInt("ViewPort", "Width", 1024, szIniPath);
	m_Option.iViewHeight = GetPrivateProfileInt("ViewPort", "Height", 768, szIniPath);
	m_Option.iViewColorDepth = GetPrivateProfileInt("ViewPort", "ColorDepth", 16, szIniPath);
	m_Option.iViewDist = GetPrivateProfileInt("ViewPort", "Distance", 512, szIniPath);
	m_Option.iEffectSndDist = GetPrivateProfileInt("Sound", "Distance", 48, szIniPath);

	int iSndEnable = GetPrivateProfileInt("Sound", "Enable", 1, szIniPath);
	m_Option.bSoundEnable = (iSndEnable) ? true : false;
	int iSndDuplicate = GetPrivateProfileInt("Sound", "Duplicate", 0, szIniPath);
	m_Option.bSndDuplicated = (iSndDuplicate) ? true : false;
	int iWindowCursor = GetPrivateProfileInt("Cursor", "WindowCursor", 1, szIniPath);
	m_Option.bWindowCursor = (iWindowCursor) ? true : false;
}

void COptionDlg::SettingUpdate()
{
	if(m_Option.iTexLOD_Chr) CheckRadioButton(IDC_R_TEX_CHR_HIGH, IDC_R_TEX_CHR_LOW, IDC_R_TEX_CHR_LOW);
	else CheckRadioButton(IDC_R_TEX_CHR_HIGH, IDC_R_TEX_CHR_LOW, IDC_R_TEX_CHR_HIGH);

	if(m_Option.iTexLOD_Shape) CheckRadioButton(IDC_R_TEX_SHAPE_HIGH, IDC_R_TEX_SHAPE_LOW, IDC_R_TEX_SHAPE_LOW);
	else CheckRadioButton(IDC_R_TEX_SHAPE_HIGH, IDC_R_TEX_SHAPE_LOW, IDC_R_TEX_SHAPE_HIGH);

	if(m_Option.iTexLOD_Terrain) CheckRadioButton(IDC_R_TEX_TERRAIN_HIGH, IDC_R_TEX_TERRAIN_LOW, IDC_R_TEX_TERRAIN_LOW);
	else CheckRadioButton(IDC_R_TEX_TERRAIN_HIGH, IDC_R_TEX_TERRAIN_LOW, IDC_R_TEX_TERRAIN_HIGH);

	CheckDlgButton(IDC_C_SHADOW, m_Option.iUseShadow);
	
	int iSel = 0;
	if(1024 == m_Option.iViewWidth) iSel = 0;
	else if(1280 == m_Option.iViewWidth) iSel = 1;
	else if(1600 == m_Option.iViewWidth) iSel = 2;
	m_CB_Resolution.SetCurSel(iSel);

	if(16 == m_Option.iViewColorDepth) iSel = 0;
	if(32 == m_Option.iViewColorDepth) iSel = 1;
	m_CB_ColorDepth.SetCurSel(iSel);

	m_SldViewDist.SetPos(m_Option.iViewDist);
	m_SldEffectSoundDist.SetPos(m_Option.iEffectSndDist);

	CheckDlgButton(IDC_C_SOUND_ENABLE, m_Option.bSoundEnable);
	CheckDlgButton(IDC_C_SOUND_DUPLICATE, m_Option.bSndDuplicated);
	CheckDlgButton(IDC_C_CURSOR_WINDOW, m_Option.bWindowCursor);
}

void COptionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if((void*)pScrollBar == (void*)(&m_SldViewDist))
	{
		m_Option.iViewDist = m_SldViewDist.GetPos();
	}
	else if((void*)pScrollBar == (void*)(&m_SldEffectSoundDist))
	{
		m_Option.iEffectSndDist = m_SldEffectSoundDist.GetPos();
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void COptionDlg::OnBVersion() 
{
	CString szMsg; szMsg.LoadString(IDS_CONFIRM_WRITE_REGISRY);
	if(IDNO == MessageBox(szMsg, "", MB_YESNO)) return; // 한번 물어본다..

	// 레지스트리에서 설치된 폴더를 읽어온다..
	CString szProduct, szKey = "SOFTWARE\\";
	szProduct.LoadString(IDS_PRODUCT);
	szKey += szProduct;

	HKEY hRegKey = NULL;
	long lStatus = RegOpenKey(HKEY_CURRENT_USER, szKey, &hRegKey);

	if(ERROR_SUCCESS == lStatus)
	{
		DWORD dwVersion = GetDlgItemInt(IDC_E_VERSION);
		DWORD dwType = REG_DWORD, dwBytes = 4;
		lStatus = RegSetValueEx(hRegKey, "VERSION", NULL, dwType, (BYTE*)(&dwVersion), 4); // 버전
		if(ERROR_SUCCESS != lStatus) { CString szErr; szErr.LoadString(IDS_ERR_REGISTRY_WRITE_VERSION); MessageBox(szErr); }

		RegCloseKey(hRegKey);
	}
}
