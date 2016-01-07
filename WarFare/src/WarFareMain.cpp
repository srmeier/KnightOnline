/*
*/
#include "StdAfx.h"
#include "UIChat.h"
#include "GameEng.h"
#include "Resource.h"
#include "N3SndMgr.h"
#include "N3UIEdit.h"
#include "PacketDef.h"
#include "APISocket.h"
#include "Warfaremain.h"
#include "PlayerMySelf.h"
#include "GameProcMain.h"
#include "KnightChrMgr.h"
#include "N3WorldManager.h"

//-----------------------------------------------------------------------------
BOOL g_bActive = true;

//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch(message) {
		case WM_COMMAND: {
			WORD wNotifyCode = HIWORD(wParam); // notification code
			CN3UIEdit* pEdit = CN3UIEdit::GetFocusedEdit();

			if(wNotifyCode == EN_CHANGE && pEdit) {
				WORD wID = LOWORD(wParam); // item, control, or accelerator identifier
				HWND hwndCtl = (HWND)lParam;

				if(CN3UIEdit::s_hWndEdit == hwndCtl) {
					pEdit->UpdateTextFromEditCtrl();
					pEdit->UpdateCaretPosFromEditCtrl();
					CGameProcedure::SetGameCursor(CGameProcedure::s_hCursorNormal);
				}
			}
		} break;

		case WM_NOTIFY: {
			int idCtrl = (int) wParam; 
			NMHDR* pnmh = (NMHDR*) lParam; 
		} break;

		case WM_KEYDOWN: {
			int iLangID = ::GetUserDefaultLangID();
			if(iLangID == 0x0404) { // Taiwan Language
				CUIChat* pUIChat = CGameProcedure::s_pProcMain->m_pUIChatDlg;
				int iVK = (int)wParam;

				if(
					pUIChat && iVK != VK_ESCAPE && iVK != VK_RETURN &&
					CGameProcedure::s_pProcMain &&
					CGameProcedure::s_pProcActive == CGameProcedure::s_pProcMain &&
					!pUIChat->IsChatMode()
				) {
					if(!(GetKeyState(VK_CONTROL)&0x8000)) {
						pUIChat->SetFocus();
						PostMessage(CN3UIEdit::s_hWndEdit, WM_KEYDOWN, wParam, lParam);
						return 0;
					}
				}
			}
		} break;

		case WM_SOCKETMSG: {
			switch(WSAGETSELECTEVENT(lParam))
			{
				case FD_CONNECT: {
					TRACE("Socket connected..\n");
				} break;
				case FD_CLOSE: {
					if(CGameProcedure::s_bNeedReportConnectionClosed) 
						CGameProcedure::ReportServerConnectionClosed(true);
					TRACE("Socket closed..\n");
				}  break;
				case FD_READ: {
					CGameProcedure::s_pSocket->Receive();
				} break;
				default: {
					__ASSERT(0, "WM_SOCKETMSG: unknown socket flag.");
				} break;
			}
		} break;

		case WM_ACTIVATE: {
			int iActive = LOWORD(wParam);           // activation flag 
			int iMinimized = (BOOL) HIWORD(wParam); // minimized flag 
			HWND hwndPrevious = (HWND) lParam;      // window handle 

			switch(iActive)
			{
				case WA_CLICKACTIVE:
				case WA_ACTIVE: {
						g_bActive = TRUE;
				} break;
				case WA_INACTIVE: {
						g_bActive = FALSE;

					if(CGameProcedure::s_bWindowed == false) {
						CLogWriter::Write("WA_INACTIVE.");
						PostQuitMessage(0);
					}
				} break;
			}
		} break;

		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT: {
			CGameProcedure::s_pSocket->Disconnect();
			CGameProcedure::s_pSocketSub->Disconnect();

			PostQuitMessage(0);
		} break;
		
		case WM_RECEIVEDATA: {
			if (CGameProcedure::s_pKnightChr)
				CGameProcedure::s_pKnightChr->OnReceiveSmq(wParam, lParam);
		} break;

		case WM_MOUSEWHEEL: {
			if(CGameProcedure::s_pProcActive == CGameProcedure::s_pProcMain) {
				float fDelta = ((short)HIWORD(wParam)) * 0.05f;
				CGameProcedure::s_pEng->CameraZoom(fDelta);
			}
		} break;
	}
	
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProcSub(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if(message == WM_SOCKETMSG) {
		switch(WSAGETSELECTEVENT(lParam))
		{
			case FD_CONNECT: {
			} break;
			case FD_ACCEPT: {

			} break;
			case FD_CLOSE: {

			} break;
			case FD_READ: {
				CGameProcedure::s_pSocketSub->Receive();
			} break;
			default: {
				__ASSERT(0, "WM_SOCKETMSG: unknown socket flag.");
			} break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
HWND CreateMainWindow(HINSTANCE hInstance)
{
	WNDCLASS    wc;

	wc.style         = 0;
	wc.lpfnWndProc   = (WNDPROC)WndProcMain;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN));
	wc.hCursor       = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "Knight OnLine Client";

	if(::RegisterClass(&wc) == 0) {
		CLogWriter::Write("Cannot register window class.");
		exit(-1);
	}

	DWORD style = WS_POPUP | WS_CLIPCHILDREN;
	return ::CreateWindow(
		"Knight OnLine Client",
		"Knight OnLine Client",
		style, 0, 0,
		CN3Base::s_Options.iViewWidth,
		CN3Base::s_Options.iViewHeight,
		NULL, NULL, hInstance, NULL
	);
}

//-----------------------------------------------------------------------------
/*
- NOTE: create the sub window
*/
HWND CreateSubWindow(HINSTANCE hInstance)
{
	WNDCLASS wc;

	wc.style         = 0;
	wc.lpfnWndProc   = (WNDPROC)WndProcSub;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = NULL;
	wc.hCursor       = NULL;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = "Knight OnLine Sub";

	if(::RegisterClass(&wc) == 0) {
		CLogWriter::Write("Cannot register window class.");
		exit(-1);
	}

	DWORD style = WS_POPUP;
	return ::CreateWindow(
		"Knight OnLine Sub",
		"Knight OnLine Sub",
		style, 0, 0, 0, 0,
		NULL, NULL, hInstance, NULL
	);
}

//-----------------------------------------------------------------------------
/*
- NOTE: this is the main intry point for the knight online program
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// NOTE: get the current directory and make it known to CN3Base
	char szPath[_MAX_PATH] = "";
	GetCurrentDirectory(_MAX_PATH, szPath);
	CN3Base::PathSet(szPath);

	// NOTE: we are anticipating an Options file to exist within this directory
	char szIniPath[_MAX_PATH] = "";
	lstrcpy(szIniPath, CN3Base::PathGet().c_str());
	lstrcat(szIniPath, "Option.Ini");

	// NOTE: what is the texture quality?
	/*
	- get the default level of detail for the characters from the option.ini
	*/
	CN3Base::s_Options.iTexLOD_Chr = GetPrivateProfileInt("Texture", "LOD_Chr", 0, szIniPath);
	if(CN3Base::s_Options.iTexLOD_Chr < 0) CN3Base::s_Options.iTexLOD_Chr = 0;
	if(CN3Base::s_Options.iTexLOD_Chr >= 2) CN3Base::s_Options.iTexLOD_Chr = 1;
	
	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Shape = GetPrivateProfileInt("Texture", "LOD_Shape", 0, szIniPath);
	if(CN3Base::s_Options.iTexLOD_Shape < 0) CN3Base::s_Options.iTexLOD_Shape = 0;
	if(CN3Base::s_Options.iTexLOD_Shape >= 2) CN3Base::s_Options.iTexLOD_Shape = 1;

	// NOTE: what is the texture quality?
	CN3Base::s_Options.iTexLOD_Terrain = GetPrivateProfileInt("Texture", "LOD_Terrain", 0, szIniPath);
	if(CN3Base::s_Options.iTexLOD_Terrain < 0) CN3Base::s_Options.iTexLOD_Terrain = 0;
	if(CN3Base::s_Options.iTexLOD_Terrain >= 2) CN3Base::s_Options.iTexLOD_Terrain = 1;

	// NOTE: should we use shadows?
	CN3Base::s_Options.iUseShadow = GetPrivateProfileInt("Shadow", "Use", 1, szIniPath);

	// NOTE: what is the screen resolution?
	CN3Base::s_Options.iViewWidth = GetPrivateProfileInt("ViewPort", "Width", 1024, szIniPath);
	CN3Base::s_Options.iViewHeight = GetPrivateProfileInt("ViewPort", "Height", 768, szIniPath);	
	if(CN3Base::s_Options.iViewWidth == 1024) CN3Base::s_Options.iViewHeight = 768;
	else if(1280 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1024;
	else if(1600 == CN3Base::s_Options.iViewWidth) CN3Base::s_Options.iViewHeight = 1200;
	else {
		CN3Base::s_Options.iViewWidth = 1024;
		CN3Base::s_Options.iViewHeight = 768;
	}

	// NOTE: what is the viewport's color depth?
	CN3Base::s_Options.iViewColorDepth = GetPrivateProfileInt("ViewPort", "ColorDepth", 16, szIniPath);
	if(CN3Base::s_Options.iViewColorDepth != 16 && CN3Base::s_Options.iViewColorDepth != 32)
		CN3Base::s_Options.iViewColorDepth = 32;

	// NOTE: what is the viewport's draw distance?
	CN3Base::s_Options.iViewDist = GetPrivateProfileInt("ViewPort", "Distance", 512, szIniPath);
	if(CN3Base::s_Options.iViewDist < 256) CN3Base::s_Options.iViewDist = 256;
	if(CN3Base::s_Options.iViewDist > 512) CN3Base::s_Options.iViewDist = 512;

	// NOTE: what is the distance for sound events?
	CN3Base::s_Options.iEffectSndDist = GetPrivateProfileInt("Sound", "Distance", 48, szIniPath);
	if(CN3Base::s_Options.iEffectSndDist < 20) CN3Base::s_Options.iEffectSndDist = 20;
	if(CN3Base::s_Options.iEffectSndDist > 48) CN3Base::s_Options.iEffectSndDist = 48;

	// NOTE: is sound enabled?
	int iSndEnable = GetPrivateProfileInt("Sound", "Enable", 1, szIniPath);
	CN3Base::s_Options.bSndEnable = (iSndEnable) ? true : false; // 사운드...

	// NOTE: is sound duplicated?
	int iSndDuplicate = GetPrivateProfileInt("Sound", "Duplicate", 0, szIniPath);
	CN3Base::s_Options.bSndDuplicated = (iSndDuplicate) ? true : false; // 사운드...

	// NOTE: should we show the window cursor?
	int iWindowCursor = GetPrivateProfileInt("Cursor", "WindowCursor", 1, szIniPath);
	CN3Base::s_Options.bWindowCursor = (iWindowCursor) ? true : false; // cursor...

	// NOTE: create the sub window
	HWND hWndSub = CreateSubWindow(hInstance);
	
	// NOTE: create the main window
	HWND hWndMain = CreateMainWindow(hInstance);

	// NOTE: check for success
	if(hWndMain == NULL || hWndSub == NULL) {
		CLogWriter::Write("Cannot create window.");
		exit(-1);
	}

	// NOTE: show the windows
	::ShowWindow(hWndSub, SW_HIDE);
	::ShowWindow(hWndMain, nCmdShow);

	// NOTE: set the main window to active
	::SetActiveWindow(hWndMain);

	/*
	- NOTE: here we check for sting commands passed as arguements to the
		program
	*/
	if(lpCmdLine && lstrlen(lpCmdLine) > 0 && lstrlen(lpCmdLine) < 64)
	{
		char szService[64], szAccountTmp[64], szPWTmp[64];
		sscanf(lpCmdLine, "%s %s %s", szService, szAccountTmp, szPWTmp);

		if(lstrcmpi(szService, "MGame") == 0)
			CGameProcedure::s_eLogInClassification = LIC_MGAME;
		else if(lstrcmpi(szService, "Daum") == 0)
			CGameProcedure::s_eLogInClassification = LIC_DAUM;
		else 
			CGameProcedure::s_eLogInClassification = LIC_KNIGHTONLINE;

		CGameProcedure::s_szAccount = szAccountTmp;
		CGameProcedure::s_szPassWord = szPWTmp;

		if(lstrcmpi(szService, "Windowed") == 0)
			CGameProcedure::s_bWindowed = true;
		else
			CGameProcedure::s_bWindowed = false;
	}

	CGameProcedure::s_bWindowed = true;

	// NOTE: allocate the static members
	CGameProcedure::StaticMemberInit(hInstance, hWndMain, hWndSub);

	// NOTE: set the games current procedure to s_pProcLogIn
	//CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcLogIn);

	// TESTING -----
	CGameBase::s_pPlayer->m_InfoBase.eRace = RACE_KA_ARKTUAREK;
	CGameBase::s_pPlayer->m_InfoBase.eNation = NATION_KARUS;
	CGameBase::s_pPlayer->m_InfoBase.eClass = CLASS_KA_WARRIOR;
	CGameBase::s_pPlayer->m_InfoExt.iZoneInit = 0x01;
	CGameBase::s_pPlayer->m_InfoExt.iZoneCur = 204;
	CGameBase::s_pPlayer->IDSet(-1, "testing", 0xffffffff);
	CGameBase::s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER;

	__TABLE_PLAYER_LOOKS* pLooks = CGameBase::s_pTbl_UPC_Looks->Find(CGameBase::s_pPlayer->m_InfoBase.eRace);
	CGameBase::s_pPlayer->InitChr(pLooks);

	CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcMain);
	//CGameProcedure::ProcActiveSet((CGameProcedure*)CGameProcedure::s_pProcCharacterSelect);
	// TESTING ------

	#if _DEBUG
		HACCEL hAccel = LoadAccelerators(NULL, MAKEINTRESOURCE(IDR_MAIN_ACCELATOR));
		HDC hDC = GetDC(hWndMain);
	#endif

	BOOL bGotMsg = FALSE;
	MSG msg; memset(&msg, 0, sizeof(MSG));

	// NOTE: this is the main game loop	
	while(msg.message != WM_QUIT)
	{
		if(g_bActive)
			bGotMsg = PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE);
		else
			bGotMsg = GetMessage(&msg, NULL, 0U, 0U);

		if(bGotMsg) {
			#if _DEBUG
				if(TranslateAccelerator(hWndMain, hAccel, &msg) == 0) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			#else
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			#endif
		} else {
			// NOTE: render a frame during idle time (no messages are waiting)
			if(g_bActive) {
				// NOTE: update and render the game
				CGameProcedure::TickActive();
				CGameProcedure::RenderActive();

				// NOTE: if we aredebugging then also draw helpful information
				#if _DEBUG
					static float fTimePrev = CN3Base::TimeGet();
					static char szDebugs[4][256] = { "", "", "", "" };
					float fTime = CN3Base::TimeGet();

					if(fTime > fTimePrev + 0.5f) {
						fTimePrev = fTime;

						sprintf(szDebugs[0], "nTerrain_Polygon(%d), nTerrain_Tile_Polygon(%d), nShape(%d), nShape_Part(%d), nShape_Polygon(%d)",
							CN3Base::s_RenderInfo.nTerrain_Polygon,
							CN3Base::s_RenderInfo.nTerrain_Tile_Polygon,
							CN3Base::s_RenderInfo.nShape,
							CN3Base::s_RenderInfo.nShape_Part,
							CN3Base::s_RenderInfo.nShape_Polygon);
						
						sprintf(szDebugs[1], "nChr(%d), nChr_Part(%d), nChr_Polygon(%d), nChr_Plug(%d), nChr_Plug_Polygon(%d)",
							CN3Base::s_RenderInfo.nChr,
							CN3Base::s_RenderInfo.nChr_Part,
							CN3Base::s_RenderInfo.nChr_Polygon,
							CN3Base::s_RenderInfo.nChr_Plug,
							CN3Base::s_RenderInfo.nChr_Plug_Polygon);

						sprintf(szDebugs[2], "Camera : FieldOfView(%.1f), NearPlane(%.1f) FarPlane(%.1f)",
							D3DXToDegree(CN3Base::s_CameraData.fFOV),
							CN3Base::s_CameraData.fNP,
							CN3Base::s_CameraData.fFP);

						if(CGameProcedure::s_pProcMain && CGameBase::ACT_WORLD && CGameBase::ACT_WORLD->GetSkyRef()) {
							int iYear = 0, iMonth = 0, iDay = 0, iH = 0, iM = 0;
							CGameBase::ACT_WORLD->GetSkyRef()->GetGameTime(&iYear, &iMonth, &iDay, &iH, &iM);
							sprintf(szDebugs[3], "%.2f:FPS, %d/%d/%d : %d:%d", CN3Base::s_fFrmPerSec, iYear, iMonth, iDay, iH, iM);
						} else szDebugs[3][0] = NULL;
					}

					for(int i = 0; i < 4; i++)
						if(szDebugs[i]) TextOut(hDC, 0, i*18, szDebugs[i], lstrlen(szDebugs[i]));
				#endif
			}
		}
	}

	#if _DEBUG
		ReleaseDC(hWndMain, hDC);
		DestroyAcceleratorTable(hAccel);
	#endif

	CGameProcedure::StaticMemberRelease();

	return msg.wParam;
}
