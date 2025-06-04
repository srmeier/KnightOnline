#pragma once

#if 0

#include <string>
#include <vector>

#include "N3UIBase.h"

struct __GameServerInfo_1098
{
	std::string	szName;
	std::string	szIP;
	int			iConcurrentUserCount;

	void Init()
	{
		szName.clear();
		szIP.clear();
		iConcurrentUserCount = 0;
	}

	bool operator () (const __GameServerInfo_1098& x, const __GameServerInfo_1098& y) const
	{
		return (x.iConcurrentUserCount >= y.iConcurrentUserCount);
	}

	__GameServerInfo_1098()
	{
		Init();
	}

	__GameServerInfo_1098(const std::string& szName2, const std::string& szIP2, int iConcurrentUserCount2)
	{
		szName = szName2;
		szIP = szIP2;
		iConcurrentUserCount = iConcurrentUserCount2;
	}
};

class CUILogIn_1098 : public CN3UIBase  
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
	
	std::vector<__GameServerInfo_1098> m_ListServerInfos;

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

	int		ServerInfoCount() { return (int) m_ListServerInfos.size(); }
	bool	ServerInfoAdd(const __GameServerInfo_1098& GSI);
	bool	ServerInfoGet(int iIndex, __GameServerInfo_1098& GSI);
	bool	ServerInfoGetCur(__GameServerInfo_1098& GSI);
	void	ServerInfoUpdate();

	void AccountIDGet(std::string& szID);
	void AccountPWGet(std::string& szPW);

	void ConnectButtonSetEnable(bool bEnable);

	CUILogIn_1098();
	~CUILogIn_1098() override;
};

#endif
