// UIStateBar.h: interface for the CUIStateBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UICurtail_H)
#define AFX_UICurtail_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"

class CUICurtail : public CN3UIBase
{
protected:

	CN3UIButton*	m_pBtn_cancel;

	bool		m_bOpenningNow; // 열리고 있다..
	bool		m_bClosingNow;	// 닫히고 있다..
	float		m_fMoveDelta; // 부드럽게 열리고 닫히게 만들기 위해서 현재위치 계산에 부동소수점을 쓴다..

	int			m_iRBtnDownOffs;
	// Attributes
public:
	bool OnKeyPress(int iKey);
	void Open();
	void Close();
	void SetVisible(bool bVisible);

	bool	ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg); // 메시지를 받는다.. 보낸놈, msg

	virtual void	Tick();
	virtual void	Render(); // 미니맵 렌더링..
	virtual bool	Load(HANDLE hFile);

	virtual void	Release();
	CUICurtail();
	virtual ~CUICurtail();

};

#endif // !defined(AFX_UICurtail)



