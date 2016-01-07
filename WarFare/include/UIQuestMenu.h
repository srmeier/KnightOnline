// UIQuestMenu.h: interface for the CUIQuestMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIQUESTMENU_H__B74550FB_798B_4DB8_91DD_EE5994976EDE__INCLUDED_)
#define AFX_UIQUESTMENU_H__B74550FB_798B_4DB8_91DD_EE5994976EDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "PacketDef.h"
#include "APISocket.h"

#define MAX_STRING_MENU	5

class CUIQuestMenu   : public CN3UIBase
{
protected:
	class CN3UIString*		m_pTextTitle;
	class CN3UIString*		m_pTextSample;
	class CN3UIString*		m_pTextMenu[MAX_STRING_MENU];
	class CN3UIImage*		m_pImageLeft;
	class CN3UIImage*		m_pImageCenter;
	class CN3UIImage*		m_pImageRight;
	class CN3UIImage*		m_pImageBLeft;
	class CN3UIImage*		m_pImageBCenter;
	class CN3UIImage*		m_pImageBRight;

	UIList	m_ReSizeChildren;		// children pointer list

	int		m_iMenuCnt;
	RECT	m_rcLImage;
	RECT	m_rcRImage;
	RECT	m_rcCImage;

public:
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iKey);
	void	MsgSend_SelectMenu(int index);
	BOOL	MoveOffset(int iOffsetX, int iOffsetY);
	bool	ReceiveMessage(CN3UIBase* pSender, DWORD dwMsg);
	DWORD	MouseProc(DWORD dwFlags, const POINT& ptCur, const POINT& ptOld);
	void	Render();
	void	InitBase();
	bool	Load(HANDLE hFile);
	void	Open(class DataPack* pDataPack, int& iOffset);

	CUIQuestMenu();
	virtual ~CUIQuestMenu();

};

#endif // !defined(AFX_UIQUESTMENU_H__B74550FB_798B_4DB8_91DD_EE5994976EDE__INCLUDED_)
