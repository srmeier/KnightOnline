// UICmdEdit.h: interface for the UICmdEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__UICMDEDIT_H__)
#define __UICMDEDIT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"
#include "N3UIBase.h"

#include "N3UIString.h"
#include "N3UIEdit.h"

//////////////////////////////////////////////////////////////////////

class CUICmdEdit : public CN3UIBase
{
public:
	CN3UIString*	m_pText_Title;
	CN3UIButton*	m_pBtn_Ok;
	CN3UIButton*	m_pBtn_Cancel;
	CN3UIEdit*		m_pEdit_Box;
	std::string		m_szArg1;

public:
	void SetVisible(bool bVisible);
	void CUICmdEdit::Open(std::string msg);

	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);

	CUICmdEdit();
	virtual ~CUICmdEdit();
};

#endif //#if !defined(__UICMDEDIT_H__)
