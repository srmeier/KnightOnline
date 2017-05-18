// AVIPlayer.h: interface for the CAVIPlayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIPLAYER_H__303DFB16_16A9_41C8_939F_0EB7B0AB0A03__INCLUDED_)
#define AFX_AVIPLAYER_H__303DFB16_16A9_41C8_939F_0EB7B0AB0A03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dshow.h>

class CAVIPlayer  
{
public:
	CAVIPlayer();
	virtual ~CAVIPlayer();

public:	
	bool PlayCutScene(LPTSTR pszMovie, HINSTANCE hInstance);
	static LONG WINAPI WindowProc_Player( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
	static bool		m_bInterrupted;
protected:
	HWND			m_hWndMain;
	IGraphBuilder	*m_pGraphBuilder;
	IMediaControl	*m_pMediaControl;
	IVideoWindow	*m_pVideoWindow;
	IMediaEvent		*m_pMediaEvent;

	BOOL	CreateHiddenWindow(HINSTANCE hInstance, LPTSTR szFile);
	bool	InitInterfaces();
	void	Release();
};

#endif // !defined(AFX_AVIPLAYER_H__303DFB16_16A9_41C8_939F_0EB7B0AB0A03__INCLUDED_)
