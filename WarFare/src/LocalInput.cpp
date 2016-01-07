#include "StdAfx.h"
#include "LocalInput.h"
#include "mmsystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static LPDIRECTINPUTDEVICE lpDID = NULL;

//////////////////////////////////////////////////////////////////////////////////
// Default constructor. Simply initializes the member elements.
//////////////////////////////////////////////////////////////////////////////////
CLocalInput::CLocalInput(void)
{
	m_lpDI = NULL;
	m_lpDIDKeyboard = NULL;
//	m_lpDIDMouse = NULL;

	m_hWnd = NULL;

//	m_bMouse = FALSE;
//	m_bKeyboard = FALSE;

	m_nMouseFlag = 0;
	m_nMouseFlagOld = 0; // 마우스 버튼 눌림 플래그
	m_dwTickLBDown = 0; // 마우스 왼쪽 버튼 더블 클릭 감지용
	m_dwTickRBDown = 0; // 마우스 오른쪽 버튼 더블 클릭 감지용

	m_ptCurMouse.x = m_ptCurMouse.y = 0; // 현재 마우스 포인터
	m_ptOldMouse.x = m_ptOldMouse.y = 0; // 직전 마우스 포인터

	SetRect(&m_rcLBDrag, 0, 0, 0, 0); // 드래그 영역
	SetRect(&m_rcMBDrag, 0, 0, 0, 0); // 드래그 영역
	SetRect(&m_rcRBDrag, 0, 0, 0, 0); // 드래그 영역

	SetRect(&m_rcMLimit, 0, 0, 0, 0); // 마우스 움직임 제한 영역

	memset(m_byCurKeys, 0, sizeof(m_byCurKeys)); // 현재 키 상태
	memset(m_byOldKeys, 0, sizeof(m_byOldKeys)); // 직전 키 상태
	memset(m_bKeyPresses, 0, sizeof(m_bKeyPresses)); // 키를 누른 순간인지
	memset(m_bKeyPresseds, 0, sizeof(m_bKeyPresseds)); // 키를 눌렀다 떼는 순간인지
	m_bNoKeyDown = FALSE; // 아무 키입력도 없는지
	
	memset(m_dwTickKeyPress, 0, sizeof(m_dwTickKeyPress));
}	


//////////////////////////////////////////////////////////////////////////////////
// Default destructor. Releases all devices that accquired
//////////////////////////////////////////////////////////////////////////////////
CLocalInput::~CLocalInput(void)
{
	// shutdown keyboard
	if (m_lpDIDKeyboard)
	{
		UnacquireKeyboard();
		m_lpDIDKeyboard->Release();
		m_lpDIDKeyboard = NULL;
	}
	
	// shutdown mouse
//	if (m_lpDIDMouse)
//	{
//		UnacquireMouse();
//		m_lpDIDMouse->Release();
//		m_lpDIDMouse = NULL;
//	}
	
	// kill directinput
	if(m_lpDI)
	{
		m_lpDI->Release();
		m_lpDI = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////
// Try to accquire all devices. Use SetActiveDevices() if you do not want 
// some devices.
//////////////////////////////////////////////////////////////////////////////////
BOOL CLocalInput::Init(HINSTANCE hInst, HWND hWnd, BOOL bActivateKeyboard, BOOL bActivateMouse, BOOL ExclusiveMouseAccess)
{
	HRESULT rval;

	m_hWnd = hWnd; // 윈도우 핸들 기억..

	rval = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&m_lpDI), NULL);
	if(rval != DI_OK) return FALSE;

	// Create the keyboard device
	rval = m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpDIDKeyboard, NULL);
	if(rval == DI_OK)
	{
		m_lpDIDKeyboard->SetDataFormat(&c_dfDIKeyboard);
		m_lpDIDKeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);

		AcquireKeyboard();
	}

	// Create the mouse device
/*	rval = m_lpDI->CreateDevice(GUID_SysMouse, &m_lpDIDMouse, NULL);
	if(rval == DI_OK)
	{
		m_lpDIDMouse->SetDataFormat(&c_dfDIMouse);
		if( ExclusiveMouseAccess == FALSE )
			m_lpDIDMouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		else
			m_lpDIDMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		AcquireMouse();
	}*/

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Controls which devices you have accquired.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::SetActiveDevices(BOOL bKeyboard, BOOL bMouse)
{
	if (bKeyboard)
		AcquireKeyboard();
	else 
		UnacquireKeyboard();
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Runs the DirectInput control panel associated with the mouse. If the device does not 
// have a control panel associated with it, the default device control panel is launched.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::RunMouseControlPanel(HWND hWnd)
{
//	m_lpDIDMouse->RunControlPanel(hWnd, 0);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Wipes out the internal key data.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::KeyboardFlushData()
{
	memset(m_byOldKeys, 0, NUMDIKEYS);
	memset(m_byCurKeys, 0, NUMDIKEYS);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// This restricts the mouse to a defined area.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::MouseSetLimits(int x1, int y1, int x2, int y2)
{
	m_rcMLimit.left = x1;
	m_rcMLimit.top = y1;
	m_rcMLimit.right = x2;
	m_rcMLimit.bottom = y2;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Sets the mouse position. This restricts the position to the physical display.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::MouseSetPos(int x, int y)
{
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

/////////////////////////////////////////////////////////////////////////////////////////////
// Retrieves the keystate of a given key.
/////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLocalInput::KeyboardGetKeyState(int nDIKey)
{
	if(nDIKey < 0 || nDIKey >= NUMDIKEYS) return FALSE;
	return(m_byCurKeys[nDIKey]);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Obtains access to the keyboard.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::AcquireKeyboard()
{
	if (m_lpDIDKeyboard)
	{
		HRESULT rval = m_lpDIDKeyboard->Acquire();
//		if(rval != DI_OK) MessageBox(::GetActiveWindow(), "Acquire Keyboard Failed.", "DirectInput", MB_OK);
		if (rval == DI_OK || rval == S_FALSE)
		{
//			m_bKeyboard = TRUE;
			KeyboardFlushData();
			return;
		}
	}	
//	m_bKeyboard = FALSE;
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Releases access to the keyboard.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::UnacquireKeyboard()
{
	KeyboardFlushData();
//	m_bKeyboard = FALSE;
	if (m_lpDIDKeyboard)
	{
		HRESULT rval = m_lpDIDKeyboard->Unacquire();
//		if(rval != DI_OK) MessageBox(::GetActiveWindow(), "UnAcquire Keyboard Failed.", "DirectInput", MB_OK);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Obtains access to the mouse.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::AcquireMouse()
{
//	if (m_lpDIDMouse)
//	{
//		HRESULT rval = m_lpDIDMouse->Acquire();
//		if (rval == DI_OK || rval == S_FALSE)
//		{
//			return;
//		}
//	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Releases access to the mouse.
/////////////////////////////////////////////////////////////////////////////////////////////
void CLocalInput::UnacquireMouse()
{
//	m_bMouse = FALSE;
//	if (m_lpDIDMouse)
//	{
//		HRESULT rval = m_lpDIDMouse->Unacquire();
//	}
}

/////////////////////////////////////////////////////////////////////////////////////////////
// Updates all devices. Call this before you check for input.
/////////////////////////////////////////////////////////////////////////////////////////////
// 되도록이면 전체 프로시저 돌때 한번씩만 도는게 좋다.. 여러번 하면 혼란이 올수도 있다.
void CLocalInput::Tick(void)
{
	HRESULT err;
//	int oldx, oldy;
//	BOOL isPressed;
//	WORD i;
//	DWORD key;

	HWND hWndActive = ::GetActiveWindow(); // 포커싱되었을때만...
	if(hWndActive != m_hWnd) return;

	///////////////////////
	//  KEYBOARD
	///////////////////////
//	if(m_bKeyboard)
//	{
		memcpy(m_byOldKeys, m_byCurKeys, NUMDIKEYS); // 전의 키 상태 기록
		err = m_lpDIDKeyboard->GetDeviceState(NUMDIKEYS, m_byCurKeys); // 현재 키 상태 기록
		if(err != DI_OK) AcquireKeyboard();
		else
		{
			m_bNoKeyDown = TRUE; // 첨엔 아무것도 안눌림
			for(int i = 0; i < NUMDIKEYS; i++)
			{
				if(!m_byOldKeys[i] && m_byCurKeys[i]) m_bKeyPresses[i] = TRUE; // 눌리는 순간
				else m_bKeyPresses[i] = FALSE;
				
				if(m_byOldKeys[i] && !m_byCurKeys[i]) m_bKeyPresseds[i] = TRUE; // 눌렀다 떼는 순간..
				else m_bKeyPresseds[i] = FALSE;
				
				if(m_byCurKeys[i]) m_bNoKeyDown = FALSE;
			}
		}
//	}

	///////////////////////
	//  MOUSE
	///////////////////////
//	if(m_bMouse)
//	{
//		DIMOUSESTATE DIMState;
//		err = m_lpDIDMouse->GetDeviceState(sizeof(DIMState), &DIMState); // 마우스 상태 얻기 - 이상타.. 자꾸.. 에러가..
//		if(err != DI_OK)
//		{
//			AcquireMouse();
//			return;
//		}

		// 마우스 위치 새로 가져오기
//		m_ptCurMouse.x += DIMState.lX;
//		m_ptCurMouse.y += DIMState.lY;

//		if((DIMState.rgbButtons[0] & 0x80)) m_bLBDown = TRUE; 
//		else m_bLBDown = FALSE;
//		if((DIMState.rgbButtons[1] & 0x80)) m_bRBDown = TRUE;
//		else m_bRBDown = FALSE;

		// clamp non-free mouse values to limits
//		if (m_ptCurMouse.x >= m_rcMLimit.right)
//			m_ptCurMouse.x = m_rcMLimit.right-1;
//
//		if (m_ptCurMouse.y >= m_rcMLimit.bottom)
//			m_ptCurMouse.y = m_rcMLimit.bottom-1;
//
//		if (m_ptCurMouse.x < m_rcMLimit.left)
//			m_ptCurMouse.x = m_rcMLimit.left;
//
//		if (m_ptCurMouse.y < m_rcMLimit.top)
//			m_ptCurMouse.y = m_rcMLimit.top;

		m_ptOldMouse = m_ptCurMouse; // 일단 전의 것 복사...

		RECT rcClient;
		::GetClientRect(m_hWnd, &rcClient);
		::GetCursorPos(&m_ptCurMouse); // 좀 이상해서... 그냥 시스템 마우스 커서 위치 가져오기
		::ScreenToClient(m_hWnd, &m_ptCurMouse); // 클라이언트 영역으로 변환

		if(PtInRect(&rcClient, m_ptCurMouse) == FALSE) //  || GetFocus() != m_hWnd) // 스크린 영역 밖에 있거나 포커스가 가있지 않으면..
		{
		}
		else
		{
			// 마우스 버튼 상태 보관.
			m_nMouseFlagOld = m_nMouseFlag;
			m_nMouseFlag = 0;
			
			// 마우스 상태 가져오기
			if(_IsKeyDown(VK_LBUTTON)) m_nMouseFlag |= MOUSE_LBDOWN;
			if(_IsKeyDown(VK_MBUTTON)) m_nMouseFlag |= MOUSE_MBDOWN;
			if(_IsKeyDown(VK_RBUTTON)) m_nMouseFlag |= MOUSE_RBDOWN;

			// 버튼 클릭 직후..
			if((m_nMouseFlagOld & MOUSE_LBDOWN) == FALSE && (m_nMouseFlag & MOUSE_LBDOWN)) m_nMouseFlag |= MOUSE_LBCLICK;
			if((m_nMouseFlagOld & MOUSE_MBDOWN) == FALSE && (m_nMouseFlag & MOUSE_MBDOWN)) m_nMouseFlag |= MOUSE_MBCLICK;
			if((m_nMouseFlagOld & MOUSE_RBDOWN) == FALSE && (m_nMouseFlag & MOUSE_RBDOWN)) m_nMouseFlag |= MOUSE_RBCLICK;
			
			// 버튼에서 손을 떼면
			if((m_nMouseFlagOld & MOUSE_LBDOWN) && (m_nMouseFlag & MOUSE_LBDOWN) == FALSE) m_nMouseFlag |= MOUSE_LBCLICKED;
			if((m_nMouseFlagOld & MOUSE_MBDOWN) && (m_nMouseFlag & MOUSE_MBDOWN) == FALSE) m_nMouseFlag |= MOUSE_MBCLICKED;
			if((m_nMouseFlagOld & MOUSE_RBDOWN) && (m_nMouseFlag & MOUSE_RBDOWN) == FALSE) m_nMouseFlag |= MOUSE_RBCLICKED;
			
			static DWORD dwDblClk = GetDoubleClickTime(); // 윈도우의 더블 클릭시간을 가져오고..
			if(m_nMouseFlag & MOUSE_LBCLICKED) // 왼쪽 더블 클릭 감지
			{
				static DWORD dwCLicked = 0;
				if(timeGetTime() < dwCLicked + dwDblClk)
					m_nMouseFlag |= MOUSE_LBDBLCLK;
				dwCLicked = timeGetTime();
			}
			if(m_nMouseFlag & MOUSE_MBCLICKED) // 왼쪽 더블 클릭 감지
			{
				static DWORD dwCLicked = 0;
				if(timeGetTime() < dwCLicked + dwDblClk)
					m_nMouseFlag |= MOUSE_MBDBLCLK;
				dwCLicked = timeGetTime();
			}
			if(m_nMouseFlag & MOUSE_RBCLICKED) // 왼쪽 더블 클릭 감지
			{
				static DWORD dwCLicked = 0;
				if(timeGetTime() < dwCLicked + dwDblClk)
					m_nMouseFlag |= MOUSE_RBDBLCLK;
				dwCLicked = timeGetTime();
			}

			// 드래그 영역 처리
			if(m_nMouseFlag & MOUSE_LBDOWN) { m_rcLBDrag.right = m_ptCurMouse.x; m_rcLBDrag.bottom = m_ptCurMouse.y; }
			if(m_nMouseFlag & MOUSE_MBDOWN) { m_rcMBDrag.right = m_ptCurMouse.x; m_rcMBDrag.bottom = m_ptCurMouse.y; }
			if(m_nMouseFlag & MOUSE_RBDOWN) { m_rcRBDrag.right = m_ptCurMouse.x; m_rcRBDrag.bottom = m_ptCurMouse.y; }
			if(m_nMouseFlag & MOUSE_LBCLICK) { m_rcLBDrag.left = m_ptCurMouse.x; m_rcLBDrag.top = m_ptCurMouse.y; }
			if(m_nMouseFlag & MOUSE_MBCLICK) { m_rcMBDrag.left = m_ptCurMouse.x; m_rcMBDrag.top = m_ptCurMouse.y; }
			if(m_nMouseFlag & MOUSE_RBCLICK) { m_rcRBDrag.left = m_ptCurMouse.x; m_rcRBDrag.top = m_ptCurMouse.y; }
		}
//	} // end of mouse
}
