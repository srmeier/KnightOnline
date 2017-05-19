// UIQuestTalk.h: interface for the CUIQuestTalk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UIQUESTTALK_H__DB9A4C59_4BE8_4698_9462_CF036C8D834D__INCLUDED_)
#define AFX_UIQUESTTALK_H__DB9A4C59_4BE8_4698_9462_CF036C8D834D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "PacketDef.h"

#define MAX_STRING_TALK		10

class CUIQuestTalk   : public CN3UIBase
{
protected:
	class CN3UIString*		m_pTextTalk;
	class CN3UIButton*		m_pBtnOk;

	std::string				m_szTalk[MAX_STRING_TALK];
	int						m_iNumTalk;
	int						m_iCurTalk;

public:
	void SetVisible(bool bVisible);
	bool OnKeyPress(int iKey);
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	void Open(Packet& pkt);
	CUIQuestTalk();
	virtual ~CUIQuestTalk();

};

#endif // !defined(AFX_UIQUESTTALK_H__DB9A4C59_4BE8_4698_9462_CF036C8D834D__INCLUDED_)
