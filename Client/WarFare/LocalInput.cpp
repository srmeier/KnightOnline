/*
*/

//#include "StdAfx.h"
#include <winsock.h>

#include "LocalInput.h"
#include "mmsystem.h"
#include "GameBase.h"

#include "GameProcedure.h"
#include "N3UIEdit.h"
#include "UIChat.h"
#include "GameProcMain.h"
#include "APISocket.h"
#include "GameEng.h"

#include "SDL2\SDL_syswm.h"

#include "UIManager.h"
#include "IMouseWheelInputDlg.h"
//-----------------------------------------------------------------------------
CLocalInput::CLocalInput(void) {
	m_bNoKeyDown = FALSE;

	m_nMouseFlag = 0;
	m_nMouseFlagOld = 0;

	m_dwTickLBDown = 0;
	m_dwTickRBDown = 0;

	m_ptCurMouse.x = m_ptCurMouse.y = 0;
	m_ptOldMouse.x = m_ptOldMouse.y = 0;

	SetRect(&m_rcLBDrag, 0, 0, 0, 0);
	SetRect(&m_rcMBDrag, 0, 0, 0, 0);
	SetRect(&m_rcRBDrag, 0, 0, 0, 0);

	SetRect(&m_rcMLimit, 0, 0, 0, 0);

	memset(m_byCurKeys, 0, sizeof(m_byCurKeys));
	memset(m_byOldKeys, 0, sizeof(m_byOldKeys));
	memset(m_bKeyPresses, 0, sizeof(m_bKeyPresses));
	memset(m_bKeyPresseds, 0, sizeof(m_bKeyPresseds));
	memset(m_dwTickKeyPress, 0, sizeof(m_dwTickKeyPress));
}	

//-----------------------------------------------------------------------------
CLocalInput::~CLocalInput(void) {}

//-----------------------------------------------------------------------------
BOOL CLocalInput::Init(SDL_Window* pWindow, BOOL bActivateKeyboard, BOOL bActivateMouse, BOOL ExclusiveMouseAccess) {
	return TRUE;
}

//-----------------------------------------------------------------------------
void CLocalInput::KeyboardFlushData() {
	memset(m_byOldKeys, 0, NUMDIKEYS);
	memset(m_byCurKeys, 0, NUMDIKEYS);
}

//-----------------------------------------------------------------------------
void CLocalInput::MouseSetLimits(int x1, int y1, int x2, int y2) {
	m_rcMLimit.left = x1;
	m_rcMLimit.top = y1;
	m_rcMLimit.right = x2;
	m_rcMLimit.bottom = y2;
}

//-----------------------------------------------------------------------------
void CLocalInput::MouseSetPos(int x, int y) {
	// clamp non-free mouse values to limits
	if ((m_ptCurMouse.x = x) >= m_rcMLimit.right)
		m_ptCurMouse.x = m_rcMLimit.right-1;

	if ((m_ptCurMouse.y = y) >= m_rcMLimit.bottom)
		m_ptCurMouse.y = m_rcMLimit.bottom-1;

	if ((m_ptCurMouse.x = x) <= m_rcMLimit.left)
		m_ptCurMouse.x = m_rcMLimit.left+1;

	if ((m_ptCurMouse.y = y) <= m_rcMLimit.top)
		m_ptCurMouse.y = m_rcMLimit.top+1;
}

//-----------------------------------------------------------------------------
BOOL CLocalInput::KeyboardGetKeyState(int nDIKey) {
	if(nDIKey<0 || nDIKey>=NUMDIKEYS) return FALSE;
	return(m_byCurKeys[nDIKey]);
}

//-----------------------------------------------------------------------------
/*
- NOTE: WndProcMain processes the messages for the main window
*/
LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_COMMAND: {
			uint16_t wNotifyCode = HIWORD(wParam); // notification code
			CN3UIEdit* pEdit = CN3UIEdit::GetFocusedEdit();

			if(wNotifyCode == EN_CHANGE && pEdit) {
				uint16_t wID = LOWORD(wParam); // item, control, or accelerator identifier
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
					//TRACE("Socket connected..\n");
				} break;
				case FD_CLOSE: {
					if(CGameProcedure::s_bNeedReportConnectionClosed) 
						CGameProcedure::ReportServerConnectionClosed(true);
					//TRACE("Socket closed..\n");
				}  break;
				case FD_READ: {
					CGameProcedure::s_pSocket->Receive();
				} break;
				default: {
					__ASSERT(0, "WM_SOCKETMSG: unknown socket flag.");
				} break;
			}
		} break;

			/*
		case WM_ACTIVATE: {
			int iActive = LOWORD(wParam);           // activation flag 
			int iMinimized = (BOOL) HIWORD(wParam); // minimized flag 
			HWND hwndPrevious = (HWND) lParam;      // window handle 

			switch(iActive)
			{
				case WA_CLICKACTIVE:
				case WA_ACTIVE: {
					#ifdef _DEBUG
						g_bActive = TRUE;
					#endif
				} break;
				case WA_INACTIVE: {
					#ifdef _DEBUG
						g_bActive = FALSE;
					#endif

					if(CGameProcedure::s_bWindowed == false) {
						CLogWriter::Write("WA_INACTIVE.");
						PostQuitMessage(0);
					}
				} break;
			}
		} break;
		*/

			/*
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT: {
			CGameProcedure::s_pSocket->Disconnect();
			CGameProcedure::s_pSocketSub->Disconnect();

			PostQuitMessage(0);
		} break;
		*/
		
		case WM_MOUSEWHEEL: {
			if(CGameProcedure::s_pProcActive == CGameProcedure::s_pProcMain) {
				float fDelta = ((int16_t)HIWORD(wParam)) * 0.05f;

				CN3UIBase* focused = CGameProcedure::s_pUIMgr->GetFocusedUI();
				
				if (focused)
				{
					int key = fDelta > 0 ? SDL_SCANCODE_PAGEUP : SDL_SCANCODE_PAGEDOWN;
					if (IMouseWheelInputDlg* t = dynamic_cast<IMouseWheelInputDlg*>(focused))
						t->OnKeyPress(key);
					else
						CGameProcedure::s_pEng->CameraZoom(fDelta);
				}
				else
					CGameProcedure::s_pEng->CameraZoom(fDelta);
			}
		} break;
	}
	
	return 0;//DefWindowProc(hWnd, message, wParam, lParam);
}

//-----------------------------------------------------------------------------
void CLocalInput::Tick(void) {
	memcpy(m_byOldKeys, m_byCurKeys, NUMDIKEYS);

	m_ptOldMouse = m_ptCurMouse;
	m_nMouseFlagOld = m_nMouseFlag;

	m_nMouseFlag  = 0;
	m_nMouseFlag |= (m_nMouseFlagOld & MOUSE_LBDOWN);
	m_nMouseFlag |= (m_nMouseFlagOld & MOUSE_MBDOWN);
	m_nMouseFlag |= (m_nMouseFlagOld & MOUSE_RBDOWN);

	SDL_Event uSDLEvents = {};
	while(SDL_PollEvent(&uSDLEvents)) {
		switch(uSDLEvents.type) {
			case SDL_QUIT: {
				CGameProcedure::s_pSocket->Disconnect();
				CGameProcedure::s_pSocketSub->Disconnect();
				CGameBase::s_bRunning = false;
			} break;

			case SDL_MOUSEMOTION: {
				m_ptCurMouse.x = uSDLEvents.motion.x;
				m_ptCurMouse.y = uSDLEvents.motion.y;
			} break;

			case SDL_MOUSEBUTTONUP: {
				switch(uSDLEvents.button.button) {
					case SDL_BUTTON_LEFT:   m_nMouseFlag ^= MOUSE_LBDOWN; break;
					case SDL_BUTTON_MIDDLE: m_nMouseFlag ^= MOUSE_MBDOWN; break;
					case SDL_BUTTON_RIGHT:  m_nMouseFlag ^= MOUSE_RBDOWN; break;
				}
			} break;

			case SDL_MOUSEBUTTONDOWN: {
				switch(uSDLEvents.button.button) {
					case SDL_BUTTON_LEFT:   m_nMouseFlag |= MOUSE_LBDOWN; break;
					case SDL_BUTTON_MIDDLE: m_nMouseFlag |= MOUSE_MBDOWN; break;
					case SDL_BUTTON_RIGHT:  m_nMouseFlag |= MOUSE_RBDOWN; break;
				}
			} break;

			case SDL_KEYUP: {
				m_byCurKeys[uSDLEvents.key.keysym.scancode] = 0x00;
			} break;

			case SDL_KEYDOWN: {
				m_byCurKeys[uSDLEvents.key.keysym.scancode] = 0x01;
			} break;

			case SDL_WINDOWEVENT: {
				switch (uSDLEvents.window.event) {
					case SDL_WINDOWEVENT_FOCUS_GAINED:
						CGameProcedure::s_bIsWindowInFocus = true;
						break;
					case SDL_WINDOWEVENT_FOCUS_LOST:
						CGameProcedure::s_bIsWindowInFocus = false;
						break;
					case SDL_WINDOWEVENT_ENTER:
						CGameProcedure::s_bWindowHasMouseFocus = true;
						break;
					case SDL_WINDOWEVENT_LEAVE:
						CGameProcedure::s_bWindowHasMouseFocus = false;
						break;
				}
			} break;

			case SDL_SYSWMEVENT: {
				// TEMP: until things become less window's dependent
				WndProcMain(uSDLEvents.syswm.msg->msg.win.hwnd,
					uSDLEvents.syswm.msg->msg.win.msg,
					uSDLEvents.syswm.msg->msg.win.wParam,
					uSDLEvents.syswm.msg->msg.win.lParam
				);
			} break;
		}
	}

	m_bNoKeyDown = TRUE;
	for(int i=0; i<NUMDIKEYS; ++i) {
		if(!m_byOldKeys[i] && m_byCurKeys[i]) m_bKeyPresses[i] = TRUE;
		else m_bKeyPresses[i] = FALSE;
		
		if(m_byOldKeys[i] && !m_byCurKeys[i]) m_bKeyPresseds[i] = TRUE;
		else m_bKeyPresseds[i] = FALSE;

		if(m_byCurKeys[i]) m_bNoKeyDown = FALSE;
	}

	if((m_nMouseFlagOld & MOUSE_LBDOWN) == FALSE && (m_nMouseFlag & MOUSE_LBDOWN)) m_nMouseFlag |= MOUSE_LBCLICK;
	if((m_nMouseFlagOld & MOUSE_MBDOWN) == FALSE && (m_nMouseFlag & MOUSE_MBDOWN)) m_nMouseFlag |= MOUSE_MBCLICK;
	if((m_nMouseFlagOld & MOUSE_RBDOWN) == FALSE && (m_nMouseFlag & MOUSE_RBDOWN)) m_nMouseFlag |= MOUSE_RBCLICK;
	
	if((m_nMouseFlagOld & MOUSE_LBDOWN) && (m_nMouseFlag & MOUSE_LBDOWN) == FALSE) m_nMouseFlag |= MOUSE_LBCLICKED;
	if((m_nMouseFlagOld & MOUSE_MBDOWN) && (m_nMouseFlag & MOUSE_MBDOWN) == FALSE) m_nMouseFlag |= MOUSE_MBCLICKED;
	if((m_nMouseFlagOld & MOUSE_RBDOWN) && (m_nMouseFlag & MOUSE_RBDOWN) == FALSE) m_nMouseFlag |= MOUSE_RBCLICKED;
	
	static uint32_t dwDblClk = GetDoubleClickTime();

	if(m_nMouseFlag & MOUSE_LBCLICKED) {
		static uint32_t dwCLicked = 0;
		if(timeGetTime() < dwCLicked + dwDblClk)
			m_nMouseFlag |= MOUSE_LBDBLCLK;
		dwCLicked = timeGetTime();
	}

	if(m_nMouseFlag & MOUSE_MBCLICKED) {
		static uint32_t dwCLicked = 0;
		if(timeGetTime() < dwCLicked + dwDblClk)
			m_nMouseFlag |= MOUSE_MBDBLCLK;
		dwCLicked = timeGetTime();
	}

	if(m_nMouseFlag & MOUSE_RBCLICKED) {
		static uint32_t dwCLicked = 0;
		if(timeGetTime() < dwCLicked + dwDblClk)
			m_nMouseFlag |= MOUSE_RBDBLCLK;
		dwCLicked = timeGetTime();
	}

	if(m_nMouseFlag & MOUSE_LBDOWN)  {m_rcLBDrag.right = m_ptCurMouse.x; m_rcLBDrag.bottom = m_ptCurMouse.y;}
	if(m_nMouseFlag & MOUSE_MBDOWN)  {m_rcMBDrag.right = m_ptCurMouse.x; m_rcMBDrag.bottom = m_ptCurMouse.y;}
	if(m_nMouseFlag & MOUSE_RBDOWN)  {m_rcRBDrag.right = m_ptCurMouse.x; m_rcRBDrag.bottom = m_ptCurMouse.y;}
	if(m_nMouseFlag & MOUSE_LBCLICK) {m_rcLBDrag.left  = m_ptCurMouse.x; m_rcLBDrag.top    = m_ptCurMouse.y;}
	if(m_nMouseFlag & MOUSE_MBCLICK) {m_rcMBDrag.left  = m_ptCurMouse.x; m_rcMBDrag.top    = m_ptCurMouse.y;}
	if(m_nMouseFlag & MOUSE_RBCLICK) {m_rcRBDrag.left  = m_ptCurMouse.x; m_rcRBDrag.top    = m_ptCurMouse.y;}
}
