// UIEndingDisplay.cpp: implementation of the CUIEndingDisplay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIEndingDisplay.h"
#include "UIManager.h"
#include "GameEng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUIEndingDisplay::CUIEndingDisplay()
{
}

CUIEndingDisplay::~CUIEndingDisplay()
{
	Release();
}

void CUIEndingDisplay::Release()
{
	CN3UIBase::Release();
}

void CUIEndingDisplay::Render()
{
	// È­¸é °¡¿îµ¥·Î ¸ÂÃß±â..
	RECT rc = this->GetRegion();
	int iX = (s_CameraData.vp.Width - (rc.right - rc.left))/2;
	int iY = (s_CameraData.vp.Height - (rc.bottom - rc.top))/2;
	this->SetPos(iX, iY);

	D3DCOLOR crEnv = 0x00000000;
	CGameEng::Clear(crEnv); // ¹è°æÀº °ËÀº»ö
	CGameEng::s_lpD3DDev->BeginScene();			// ¾À ·»´õ ¤µÀÛ...

	CUIManager::RenderStateSet();
	CN3UIBase::Render();
	CUIManager::RenderStateRestore();
	
	CGameEng::s_lpD3DDev->EndScene();			// ¾À ·»´õ ½ÃÀÛ...
	CGameEng::Present(CN3Base::s_hWndBase);
}