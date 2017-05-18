// AVIPlayer.cpp: implementation of the CAVIPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "stdio.h"
#include "AVIPlayer.h"
#include <atlbase.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

bool	CAVIPlayer::m_bInterrupted = false;

CAVIPlayer::CAVIPlayer()
{
	m_pGraphBuilder = NULL;
	m_pMediaControl = NULL;
	m_pVideoWindow = NULL;
	m_pMediaEvent = NULL;
	m_hWndMain = NULL;
}

CAVIPlayer::~CAVIPlayer()
{
}

void CAVIPlayer::Release()
{
	if (m_pGraphBuilder)	m_pGraphBuilder->Release(), m_pGraphBuilder = NULL;
	if (m_pMediaControl)	m_pMediaControl->Release(), m_pMediaControl = NULL;
	if (m_pVideoWindow)		m_pVideoWindow->Release(), m_pVideoWindow = NULL;
	if (m_pMediaEvent)		m_pMediaEvent->Release(), m_pMediaEvent = NULL;

	DestroyWindow(m_hWndMain);	m_hWndMain = NULL;
}

bool CAVIPlayer::InitInterfaces()
{
	if (FAILED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&m_pGraphBuilder)))
		return false;

    // Get interfaces to control playback & screensize
    if (FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaControl,  (void **)&m_pMediaControl)))
		return false;
    if (FAILED(m_pGraphBuilder->QueryInterface(IID_IVideoWindow,   (void **)&m_pVideoWindow)))
		return false;
    // Get interface to allow the app to wait for completion of playback
    if (FAILED(m_pGraphBuilder->QueryInterface(IID_IMediaEventEx,  (void **)&m_pMediaEvent)))
		return false;

	return true;
}

bool CAVIPlayer::PlayCutScene(LPTSTR pszMovie, HINSTANCE hInstance)
{
	if ( !CreateHiddenWindow(hInstance, pszMovie))
	{
		return false;
	}

	HRESULT hr;
	if (FAILED(hr = CoInitialize(NULL)))
        return false;

	if (!InitInterfaces())
    {
        CoUninitialize();
        return false;
    }

	// Play the movie / cutscene
	USES_CONVERSION;
	WCHAR wFileName[MAX_PATH];
	wcscpy(wFileName, T2W(pszMovie));
	if (S_OK != m_pGraphBuilder->RenderFile(wFileName, NULL))
		return false;
	m_pVideoWindow->put_MessageDrain((OAHWND)m_hWndMain);

	// Set Full screen mode
	LONG	lMode;
	m_pVideoWindow->get_FullScreenMode(&lMode);
	static HWND hDrain = 0;
	if (lMode == 0) 
	{
		m_pVideoWindow->get_MessageDrain((OAHWND*)&hDrain);
		m_pVideoWindow->put_MessageDrain((OAHWND)m_hWndMain);
		lMode = -1;		// OATRUE
		m_pVideoWindow->put_FullScreenMode(lMode);
	}

	//
	m_pMediaControl->Pause();
	m_pMediaControl->Run();

	//
	BOOL bSleep = TRUE;
	while (1)
    {
        MSG msg;
        long lEventCode, lParam1, lParam2;

        // Reset sleep flag
        bSleep = TRUE;

		if (m_bInterrupted)
		{
			m_pMediaControl->Stop();
			bSleep = FALSE;
			break;
		}

        // Has there been a media event?  Look for end of stream condition.
        if(E_ABORT != m_pMediaEvent->GetEvent(&lEventCode, (LONG_PTR *) &lParam1, 
                                    (LONG_PTR *) &lParam2, 0))
        {
            // Free the media event resources.
            m_pMediaEvent->FreeEventParams(lEventCode, lParam1, lParam2);

			

            // Is this the end of the movie?
            if (lEventCode == EC_COMPLETE)
            {
				bSleep = FALSE;
                break;                
            }
        }

        // Give system threads time to run (and don't sample user input madly)
		if (bSleep)            Sleep(100);

        // Check and process window messages (like our keystrokes)
        while (PeekMessage (&msg, m_hWndMain, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    // Release DirectShow interfaces
    Release();
    CoUninitialize();

	return true;
}

LONG WINAPI CAVIPlayer::WindowProc_Player( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
        // Monitor keystrokes for manipulating video window
        // and program options
        case WM_KEYDOWN:
            switch( wParam )
            {
                case VK_ESCAPE:
                case VK_SPACE:
                case VK_RETURN:
					CAVIPlayer::m_bInterrupted = true;
                    break;
            }
            break;
		case WM_LBUTTONDOWN:
			CAVIPlayer::m_bInterrupted = true;
			break;
    }

    return (LONG) DefWindowProc(hWnd, message, wParam, lParam);
}

BOOL CAVIPlayer::CreateHiddenWindow( HINSTANCE hInstance, LPTSTR szFile )
{
    TCHAR szTitle[MAX_PATH];

    // Set up and register window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = (WNDPROC) WindowProc_Player;
    wc.hInstance = hInstance;
    wc.lpszClassName = "AVI";
    if (!RegisterClass(&wc))
        return FALSE;

    wsprintf(szTitle, TEXT("%s: %s"), "CutScene", szFile);

    // Create a window of zero size that will serve as the sink for
    // keyboard input.  If this media file has a video component, then
    // a second ActiveMovie window will be displayed in which the video
    // will be rendered.  Setting keyboard focus on this application window
    // will allow the user to move the video window around the screen, make
    // it full screen, resize, center, etc. independent of the application
    // window.  If the media file has only an audio component, then this will
    // be the only window created.	
    m_hWndMain = CreateWindowEx(
        0, "AVI", szTitle,
        0,            // not visible
        0, 0, 0, 0,
        NULL, NULL, hInstance, NULL );

    return (m_hWndMain != NULL);
}

