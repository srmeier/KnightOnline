// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_USERTEST_H_)
#define _USERTEST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IOCSocket.h"
#include "Define.h"//#include "DefineTest.h"

class CEbenezerDlg;

class CUserTest : public CIOCSocket
{
protected:
	//int m_State;

public:
	//int GetState(void) {return m_State;}

	void SendMyInfo(void);
	void SendNotice(void);
	void UserInOut( Byte Type );
	void Initialize();
	void MoveProcess( Byte* pBuf );
	void LoginProcess( Byte* pBuf );
	void Parsing( int len, Byte* pData );
	void CloseProcess();
	CUserTest(CEbenezerDlg* pMain);
	virtual ~CUserTest();

	float m_curx;
	float m_curz;
	float m_cury;

	short m_RegionX;
	short m_RegionZ;

	char m_UserId[MAX_ID_SIZE];

	CEbenezerDlg* m_pMain;
};

#endif // !defined(AFX_USER_H__5FEC1968_ED75_4AAF_A4DB_CB48F6940B2E__INCLUDED_)
