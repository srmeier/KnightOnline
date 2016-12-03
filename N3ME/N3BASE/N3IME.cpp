// N3IME.cpp: implementation of the CN3IME class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3IME.h"
#include "N3UIEdit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CN3IME::CN3IME()
{
	m_dwProperty = 0;
	m_dwState = 0;
	m_hKeyLayout = NULL;
	m_hIMC = NULL;
	m_hWnd = NULL;
}

CN3IME::~CN3IME()
{

}

BOOL CN3IME::InitIME(HWND hwnd)
{
	if (NULL == hwnd) return FALSE;
	m_hWnd = hwnd;

	m_hKeyLayout = GetKeyboardLayout(0);	// 0 means Current thread
	m_dwProperty = ImmGetProperty(m_hKeyLayout, IGP_PROPERTY);

	m_dwState = 0;
	return TRUE;
}

///////////////////////////////////////////////////////////////
// for IME Control Message

LRESULT	CN3IME::IMEMessageProc(UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	if(!m_hWnd)
		return 0L;
	switch (nMsg) { 
		case WM_IME_STARTCOMPOSITION:     // 글자 조합의 시작. WM_IME_COMPOSITION 메시지를           // 받을 준비를 한다. 
			OnIMEStartComposition(wParam,lParam);
			return TRUE;
		case WM_IME_ENDCOMPOSITION:          // 글자 조합의 끝. 조합된 문자열 처리를 끝낸다          break; 
			OnIMEEndComposition(wParam,lParam);
			return TRUE;
		case WM_IME_COMPOSITION:          // 현재 조합중인 문자열이나 완성된 문자열을 얻어서 화면 출력 
			return OnIMEComposition(wParam,lParam);
		case WM_IME_SETCONTEXT:    // 글자 조합 윈도우와 한자 변환 윈도우를 표시하지 않게 바꿈 
			OnIMESetContext(wParam,lParam);
			return TRUE;
		case WM_IME_NOTIFY:        
			OnIMENotify(wParam,lParam);
			return TRUE;
		case WM_INPUTLANGCHANGE:       // 키보드 레이아웃이 바뀌는 경우 IME를 초기화      
			OnInputLangChange(wParam,lParam);
			return TRUE;
		case WM_IME_CONTROL:
			OnIMEControl(wParam,lParam);
			return TRUE;
		case WM_IME_COMPOSITIONFULL:
			OnIMECompositionFull(wParam,lParam);
			return TRUE;
		case WM_CHAR:
			return OnChar(wParam);
		case WM_KEYDOWN:
			return OnKeyDown(wParam);
			
    }
	return FALSE;
}

LRESULT CN3IME::OnInputLangChange(WPARAM dwCommand, LPARAM dwData)
{
    if (ImmIsIME(m_hKeyLayout) && m_dwProperty & IME_PROP_AT_CARET) ClearData();

    // Set new keyboard layout.
	InitIME(m_hWnd);

	if(Enter()) {
		for (int i = 0; i < MAX_LISTCAND; i++) {
			CANDIDATEFORM cf;

			if (m_dwProperty & IME_PROP_AT_CARET) {
	
				// This application do not want to set candidate window to
				// any position. Anyway, if an application need to set the
				// candiadet position, it should remove the if 0 code
				// the position you want to set
				//cf.dwIndex = i;
				//cf.dwStyle = CFS_CANDIDATEPOS;
				//cf.ptCurrentPos.x = ptAppWantPosition[i].x;
				//cf.ptCurrentPos.y = ptAppWantPosition[i].y;
				//ImmSetCandidateWindow(m_hIMC, &CandForm );
	
			} else {
				if (!ImmGetCandidateWindow(m_hIMC, i, &cf))
					continue;
				if (cf.dwStyle == CFS_DEFAULT)
					continue;
				cf.dwStyle = CFS_DEFAULT;
				ImmSetCandidateWindow(m_hIMC, &cf);
			}
		}
		Leave();
	}
	return DefWindowProc(m_hWnd,WM_INPUTLANGCHANGE, dwCommand, dwData);
}

LRESULT CN3IME::OnIMESetContext(WPARAM dwCommand, LPARAM dwData)
{
	if (m_dwProperty & IME_PROP_AT_CARET) {
		// application wants to draw UI by itself.
		dwData &= ~(ISC_SHOWUICOMPOSITIONWINDOW | ISC_SHOWUIALLCANDIDATEWINDOW);
	}
	return DefWindowProc(m_hWnd,WM_IME_SETCONTEXT, dwCommand, dwData);
}

LRESULT CN3IME::OnIMEStartComposition(WPARAM dwCommand, LPARAM dwData)
{
	if (Check()) { // !IME_PROP_SPECIAL_UI && IME_PROP_AT_CARET
		m_dwState |= IME_IN_COMPOSITION;
	}
	else
	{
	}
	return 0;
}

LRESULT CN3IME::OnIMEComposition(WPARAM dwCommand, LPARAM dwData)
{
	if (Check()) { // !IME_PROP_SPECIAL_UI && IME_PROP_AT_CARET
		if (dwData & GCS_RESULTSTR)
		{
			GetResultString();			// Result String
			return TRUE;
		}
		else if (dwData & GCS_COMPSTR)	
		{
			GetCompString(dwData);		// Composition string
			return TRUE;
		}
	}
	return FALSE;
}

LRESULT CN3IME::OnIMEEndComposition(WPARAM dwCommand, LPARAM dwData)
{
	if (Check()) { // !IME_PROP_SPECIAL_UI && IME_PROP_AT_CARET
		m_dwState &= ~IME_IN_COMPOSITION;
	}
	else
	{
	}
	return FALSE;
}

LRESULT CN3IME::OnIMECompositionFull(WPARAM dwCommand, LPARAM dwData)
{
	// Make sure the size for drawing the composition string.
	// Application should draw the composition string correctly.
	return FALSE;
}

LRESULT CN3IME::OnIMENotify(WPARAM dwCommand, LPARAM dwData)
{
	// This class does not handle all notification message.
	// So we pass those notification messages which are not hanlded
	// by this application to the DefWindowProc.

    switch (dwCommand) {
        case IMN_OPENCANDIDATE:
			return OpenCandidate(dwData);

        case IMN_CLOSECANDIDATE:
			return CloseCandidate(dwData);

        case IMN_CHANGECANDIDATE:
			return ChangeCandidate(dwData);

        case IMN_SETOPENSTATUS:
			return SetOpenStatus();
    }
    return DefWindowProc(m_hWnd,WM_IME_NOTIFY, dwCommand, dwData);
}

LRESULT CN3IME::OnIMEControl(WPARAM dwCommand, LPARAM dwData)
{
	// This message is not received by the application window.
	// But don't pass it to DefWindowProc().
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////////////////

void CN3IME::ClearData()
{
//    for(int i = 0; i < MAX_LISTCAND; i++) {
//        if (m_hwndCand[i]) {
//            ::DestroyWindow(m_hwndCand[i]);
//			m_hwndCand[i] = NULL;
//            delete m_CandList[i];
//			m_CandList[i] = NULL;
//        }
//    }
    m_dwState = 0;
}

BOOL CN3IME::GetResultString()
{
    DWORD       len;					// Storage for length of result str.
    LPSTR       str;					// Pointer to result string.

	if (Enter()) {
		if ((len = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > 0) {
			str = new char[len + 1];
			ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, str, len);
			str[len] = 0;
			// 완성된 글자 처리하기
			CN3UIEdit* pFocusedEdit = CN3UIBase::GetFocusedEdit();
			if (pFocusedEdit) pFocusedEdit->OnIMEResultString(str);

			delete [] str;
		}
		Leave();
	}
	return TRUE;
}

BOOL CN3IME::GetCompString(LONG flag)
{
    DWORD      len;              // Stogare for len. of composition str
    LPSTR      str;              // Pointer to composition str.
    LPSTR      strAttr;          // Pointer to composition str array.
    DWORD      lenAttr;
   
	if (!Enter())
		return FALSE;
    if ((len = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0)) > 0) {
		str = new char[len+1];
		ImmGetCompositionString(m_hIMC, GCS_COMPSTR, str, len);
		str[len] = 0;
		strAttr = NULL;
		if (flag & GCS_COMPATTR) {
			if ((lenAttr = ImmGetCompositionString(m_hIMC, GCS_COMPATTR, NULL, 0)) > 0) {
				strAttr = new char[lenAttr+1];
				ImmGetCompositionString(m_hIMC, GCS_COMPATTR, strAttr, lenAttr);
				strAttr[lenAttr] = 0;
			}
		} 

		// 조합중인 글자 처리하기
		CN3UIEdit* pFocusedEdit = CN3UIBase::GetFocusedEdit();
		if (pFocusedEdit) pFocusedEdit->OnIMECompString(str);

		if (strAttr)
			delete [] strAttr;
		if (str)
			delete [] str;
	}
	else if (0 == len)
	{
        // 한글입력이 들어오다가 지워진 경우이다.
        // (Back Space가 들어 오는 경우)
        // (이런 경우에는 길이가 0이다.)
		CN3UIEdit* pFocusedEdit = CN3UIBase::s_pFocusedEdit;
		if (pFocusedEdit) pFocusedEdit->OnIMECompString("");
	}
	Leave();
	return TRUE;
}

LRESULT CN3IME::OnChar(WPARAM wParam)
{
	CN3UIEdit* pFocusedEdit = CN3UIBase::s_pFocusedEdit;
	if (pFocusedEdit) return pFocusedEdit->OnChar(wParam);
	return FALSE;
}

LRESULT CN3IME::OnKeyDown(WPARAM wParam)
{
	CN3UIEdit* pFocusedEdit = CN3UIBase::s_pFocusedEdit;
	if (pFocusedEdit) return pFocusedEdit->OnKeyDown(wParam);
	return FALSE;
}

/////////////////////////////////////////////////////////////////
// for Candidate window

// IMN_OPENCANDIDATE:
BOOL CN3IME::OpenCandidate(LONG lParam)
{
/*
    LPCANDIDATELIST lpCandList;         // Storage for LP to candidate list.
    DWORD       dwBufLen;               // Storage for candidate strings.
    LPSTR       lpStr;                  // Storage for LP to a string.
    int         max_width = 0;          // Storage for width of listCand
    int         CurNumCandList = 0;     // Storage for number of cand. lists.
    DWORD       dwPreferNumPerPage;     // Storage for PreferNumPerPage
 //   POINT       point;                  // Storage for caret position.

    if (!Enter())
        return FALSE;

    // SetWindowText(hwnd, (LPSTR)TITLE_CAND);
    for (int i = 0; i < MAX_LISTCAND; i++ ) {
        if (m_hwndCand[i])
            CurNumCandList++;
    }
    for (int iIndex = 0; iIndex < MAX_LISTCAND; iIndex++ ) {
        if (lParam & (1 << iIndex)) {   
            if (!(dwBufLen = ImmGetCandidateList(m_hIMC, iIndex, lpCandList, 0)))                      
                goto exit_opencand;
            if( !(m_CandList[iIndex] = (LPCANDIDATELIST)new char[dwBufLen]))
                goto exit_opencand;
            lpCandList = m_CandList[iIndex];
            ImmGetCandidateList(m_hIMC, iIndex, lpCandList, dwBufLen);

            POINT pt ;
			GetCaretPos(&pt);
            ClientToScreen(m_hWnd,&pt);

            dwPreferNumPerPage = ( !lpCandList->dwPageSize ) ?
                                 DEFAULT_CAND_NUM_PER_PAGE :
                                 lpCandList->dwPageSize;

			// get the longest string length
            for (int i = 0; i < (int)lpCandList->dwCount; i++ ) {
                lpStr = (LPSTR)lpCandList + lpCandList->dwOffset[i];
                max_width = (max_width < lstrlen(lpStr)) ? lstrlen(lpStr) : max_width;
            }

            m_hwndCand[iIndex] = CreateWindow(          
                                 "HanjaCandidate", "CandWindow",
                                 WS_BORDER | WS_POPUP | WS_DISABLED,
                                 CurNumCandList * m_charWidth + pt.x, pt.y + m_charHeight + 2,
                                 (max_width + 3) * m_charWidth + 4,
                                 (int)dwPreferNumPerPage * m_charHeight + 5,
                                 m_hWnd,
                                 (HMENU)NULL,
                                 (HINSTANCE)GetWindowLong(m_hWnd, GWL_HINSTANCE ),
                                 (LPVOID)NULL
                                 );

            if (m_hwndCand[iIndex] == NULL) {
				delete m_CandList[iIndex];
				m_CandList[iIndex] = NULL;
                goto exit_opencand;
            }
			SetWindowLong(m_hwndCand[iIndex], 0, (LONG)this);

            ::ShowWindow(m_hwndCand[iIndex], SW_SHOWNOACTIVATE);
            DisplayCandStrings(m_hwndCand[iIndex], lpCandList);
            CurNumCandList++;
        }
    }
    m_dwState |= IME_IN_CHOSECAND;

exit_opencand:
    Leave();
*/
	return TRUE;
}

// IMN_CLOSECANDIDATE:
BOOL CN3IME::CloseCandidate(LONG CandList)
{
/*
    for (int i = 0; i < MAX_LISTCAND; i++) {
        if ((CandList & ( 1 << i ) ) && m_CandList[i]) {
            ::DestroyWindow(m_hwndCand[i]);
            m_hwndCand[i] = NULL;
			delete m_CandList[i];
			m_CandList[i] = NULL;
        }
    }
    m_dwState &= ~IME_IN_CHOSECAND;
*/
	return TRUE;
}

// IMN_CHANGECANDIDATE:
BOOL CN3IME::ChangeCandidate(LONG CandList)
{
/*
    LPCANDIDATELIST lpCandList = NULL;         
    DWORD           dwIndex;
    DWORD 			dwBufLen;
    LPSTR			lpStr;
    DWORD			i = 1;
    RECT			rect;
    int				max_width = 0;
    DWORD			dwPreferNumPerPage;


	if (!Enter())
		return FALSE;

    for (dwIndex = 0; dwIndex < MAX_LISTCAND; dwIndex++) {
        if (CandList & i)
            break;
		else
			i <<= 1;
	}

    if (dwIndex == MAX_LISTCAND)
        goto exit_changecand;

    if (!(dwBufLen = ImmGetCandidateList(m_hIMC, dwIndex, lpCandList, 0))) {
		goto exit_changecand;
	}
	delete m_CandList[dwIndex];
	m_CandList[dwIndex] = lpCandList = (LPCANDIDATELIST)new char[dwBufLen];

    ImmGetCandidateList(m_hIMC, dwIndex, lpCandList, dwBufLen);
    dwPreferNumPerPage = (!lpCandList->dwPageSize ) ?
                         DEFAULT_CAND_NUM_PER_PAGE : lpCandList->dwPageSize;

    for( i = 0; i < lpCandList->dwCount; i++ ) {
        lpStr = (LPSTR)lpCandList + lpCandList->dwOffset[i];
        max_width = (max_width < lstrlen(lpStr)) ? lstrlen(lpStr) : max_width;
    }

    ::GetWindowRect(m_hwndCand[dwIndex], (LPRECT) &rect);
    ::SetWindowPos(m_hwndCand[dwIndex], m_hWnd, rect.left, rect.top,
		  (max_width + 3) * m_charWidth + 4,
		  (int)(dwPreferNumPerPage) * m_charHeight + 5,
		  SWP_NOZORDER | SWP_NOACTIVATE );
		  
	DisplayCandStrings(m_hwndCand[dwIndex], lpCandList);

exit_changecand:
	Leave();
*/
	return TRUE;
}

// IMN_SETOPENSTATUS:
BOOL CN3IME::SetOpenStatus()
{
/*
    if (!Enter())
        return FALSE;

    if (ImmGetOpenStatus(m_hIMC)) {
        // the IME conversion engine is open
    } else {
        // Here we close and destroy all of candidate windows
        // if IME conversion engine is closed.

        for (int i = 0; i < MAX_LISTCAND; i++ ) {
            if (m_CandList[i]) {
                ::DestroyWindow(m_hwndCand[i]);
                m_hwndCand[i] = NULL;
				delete m_CandList[i];
				m_CandList[i] = NULL;
            }
        }
		m_nCompLen = 0;
		m_dwState = 0;
    }
	Leave();
*/
	return TRUE;
}

