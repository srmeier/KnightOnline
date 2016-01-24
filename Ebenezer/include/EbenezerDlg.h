// EbenezerDlg.h : header file
//

#if !defined(AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_)
#define AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Iocport.h"
#include "Map.h"
/////////////////////////////////////////////////////////////////////////////
// CEbenezerDlg dialog

struct _POSITION
{
	float initx;
	float initz;
};

class CUser;
class CEbenezerDlg
{
// Construction
public:
	bool OnInitDialog(void);

	void UserInOutForMe( CUser* pUser );
	int GetRegionCount( int x, int z );
	void Send_Region( Byte* pBuf, int len, int x, int z, bool tome = false );
	void Send_All( Byte* pBuf, int len, bool tome = false );
	static CUser* GetUserPtr( char* userid );

	static CIOCPort	m_Iocport;
	C3DMap	m_Map;

	int	m_nCount;

	_POSITION	m_pInitPos[5];


// Implementation
protected:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EBENEZERDLG_H__655A21EF_E029_42C0_890A_68DA7F542428__INCLUDED_)
