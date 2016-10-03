// WarMessage.cpp: implementation of the CWarMessage class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
//#include "Resource.h"
#include "DFont.h"
#include "WarMessage.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MESSAGE_FONT_SIZE		20
#define WAR_MESSAGE_SHOW_TIME	60

CWarMessage::CWarMessage()
{
	m_pMessageFont	= NULL;
	m_fTime			= 0.0f;
}

CWarMessage::~CWarMessage()
{
	Release();
}

void CWarMessage::InitFont()
{
	Release();

	std::string szFont = "Arial"; //::_LoadStringFromResource(IDS_FONT_ID, szFont);
	m_pMessageFont = new CDFont(szFont, MESSAGE_FONT_SIZE);
	__ASSERT(m_pMessageFont, "Font Create Fail!!");

	if(m_pMessageFont)
	{
		m_pMessageFont->InitDeviceObjects(CN3Base::s_lpD3DDev);
		m_pMessageFont->RestoreDeviceObjects();
		const int iW	= CN3Base::s_CameraData.vp.Width;
#ifdef _DEBUG
		m_ptMessage.y	= 80;
#else
		m_ptMessage.y	= 20;
#endif
		m_ptMessage.x	= iW;

	}
}

void CWarMessage::Release()
{
	if(m_pMessageFont) delete m_pMessageFont; m_pMessageFont = NULL;
}

void CWarMessage::SetMessage(const std::string& szText, DWORD dwFlags, DWORD dwColor)
{
	if(m_pMessageFont) 
	{
		m_pMessageFont->SetText(szText, dwFlags); // 폰트에 텍스트 지정.
		m_pMessageFont->SetFontColor(dwColor);
		m_fTime = WAR_MESSAGE_SHOW_TIME;
	}
}

void CWarMessage::RenderMessage()
{
	if(m_fTime < 0.0f) return;
	m_fTime -= CN3Base::s_fSecPerFrm;

	if(m_pMessageFont) 
	{
		POINT pt = m_ptMessage;
		D3DCOLOR crFont = m_pMessageFont->GetFontColor();

		m_pMessageFont->DrawText(pt.x - 1, pt.y - 1, 0xff000000, 0);
		m_pMessageFont->DrawText(pt.x + 1, pt.y + 1, 0xff000000, 0);
		m_pMessageFont->DrawText(pt.x , pt.y, crFont, 0);
	}
}

void CWarMessage::Tick()
{
	int iW = CN3Base::s_CameraData.vp.Width;
	if(m_fTime < 0.0f)
	{
		m_ptMessage.x = iW;
		return;
	}

	m_ptMessage.x -= CN3Base::s_fSecPerFrm * 100;
	SIZE size = m_pMessageFont->GetSize();
	if(m_ptMessage.x < (-size.cx))
	{
		m_ptMessage.x = iW;
	}
}
