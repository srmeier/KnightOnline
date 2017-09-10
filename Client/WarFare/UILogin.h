// UILogIn.h: interface for the CUILogIn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UILogIn_H__5D51312E_7B98_43C2_871B_12A9CC0C2096__INCLUDED_)
#define AFX_UILogIn_H__5D51312E_7B98_43C2_871B_12A9CC0C2096__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>

#include "N3UIBase.h"

struct __GameServerInfo : public std::binary_function<__GameServerInfo, __GameServerInfo, bool>
{
	std::string szName;
	std::string szIP;
	int	iConcurrentUserCount;

	void Init() { szName.clear(); szIP.clear(); iConcurrentUserCount = 0; }
	bool operator () (const __GameServerInfo& x, const __GameServerInfo& y) const 
	{
		return (x.iConcurrentUserCount >= y.iConcurrentUserCount);
	}

	__GameServerInfo() { this->Init(); };
	__GameServerInfo(const std::string szName2, const std::string szIP2, int iConcurrentUserCount2)
	{
		szName = szName2;
		szIP = szIP2;
		iConcurrentUserCount = iConcurrentUserCount2;
	}
};

class CUILogIn : public CN3UIBase  
{
protected:
	CN3UIEdit*	m_pEdit_id;
	CN3UIEdit*	m_pEdit_pw;
	
	CN3UIButton* m_pBtn_LogIn;
	CN3UIButton* m_pBtn_Connect;
	CN3UIButton* m_pBtn_Cancel;
	CN3UIButton* m_pBtn_Option;
	CN3UIButton* m_pBtn_Join;

	CN3UIBase*	m_pGroup_ServerList;
	CN3UIBase*	m_pGroup_LogIn;
	
	CN3UIBase* m_pText_Rights;
	CN3UIBase* m_pImg_MGameLogo;
	CN3UIBase* m_pImg_DaumLogo;
	CN3UIBase* m_pImg_GradeLogo;

	CN3UIList*	m_pList_Server;
	
	std::vector<__GameServerInfo> m_ListServerInfos;

	bool	m_bOpenningNow; // 위에서 아래로 스르륵...열려야 한다면..
	float 	m_fMoveDelta;
	bool	m_bLogIn; // 로그인 중복 방지..

public:
	void SetRequestedLogIn(bool bLogIn) { m_bLogIn = bLogIn; }
	bool OnKeyPress(int iKey);
	void RecalcGradePos();
	void SetVisibleLogInUIs(bool bEnable); // 계정 LogIn 에 필요한 UI 들을 숨긴다..
	void OpenServerList();
	void Tick();

	void InitEditControls();
	void FocusCircular();
	void FocusToID();
	bool Load(HANDLE hFile);
	bool ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg); // 메시지를 받는다.. 보낸놈, msg

	int		ServerInfoCount() { return m_ListServerInfos.size(); }
	bool	ServerInfoAdd(const __GameServerInfo& GSI);
	bool	ServerInfoGet(size_t iIndex, __GameServerInfo& GSI);
	bool	ServerInfoGetCur(__GameServerInfo& GSI);
	void	ServerInfoUpdate();

	void AccountIDGet(std::string& szID);
	void AccountPWGet(std::string& szPW);

	void ConnectButtonSetEnable(bool bEnable);

	CUILogIn();
	virtual ~CUILogIn();
};

#endif // !defined(AFX_UILogIn_H__5D51312E_7B98_43C2_871B_12A9CC0C2096__INCLUDED_)
