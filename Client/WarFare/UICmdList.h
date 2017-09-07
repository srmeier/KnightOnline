// UICmdList.h: interface for the CUICmdList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UICmdList_H)
#define AFX_UICurtail_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIButton.h"
#include "N3UIList.h"

class CUICmdList : public CN3UIBase
{
protected:

	CN3UIButton*	m_pBtn_cancel;
	CN3UIList*		m_pList_CmdCat;
	CN3UIList*		m_pList_Cmds;

	bool		m_bOpenningNow; // 열리고 있다..
	bool		m_bClosingNow;	// 닫히고 있다..
	float		m_fMoveDelta; // 부드럽게 열리고 닫히게 만들기 위해서 현재위치 계산에 부동소수점을 쓴다..

	int			m_iRBtnDownOffs;

	enum iCmd
	{
		PRIVATE_CMD_LIST	= 0x00,
		TRADE_CMD_LIST		= 0x02,
		PARTY_CMD_LIST		= 0x04,
		CLAN_CMD_LIST		= 0x06,
		KNIGHTS_CMD_LIST	= 0x08,
		GM_CMD_LIST		    = 0x0C,
		GUARDIAN_CMD_LIST	= 0x0E,
		KING_CMD_LIST		= 0x0A
		
	};

	std::map<uint16_t, std::string> m_mapCmds;
	// Attributes
public:
	bool OnKeyPress(int iKey);
	void Open();
	void Close();
	void SetVisible(bool bVisible);
	bool CreateCategoryList();
	bool UpdateCommandList(uint8_t cmd);
	bool ExecuteCommand(uint8_t cmdSel);

	bool	ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg); // 메시지를 받는다.. 보낸놈, msg

	virtual void	Tick();
	virtual void	Render(); // 미니맵 렌더링..
	virtual bool	Load(HANDLE hFile);

	virtual void	Release();
	CUICmdList();
	virtual ~CUICmdList();

};

#endif // !defined(AFX_UICmdList)



