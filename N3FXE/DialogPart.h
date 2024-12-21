// DialogPart.h: interface for the CDialogPart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGPART_H__A41E6879_ECFF_4A12_9061_5BF495AB880C__INCLUDED_)
#define AFX_DIALOGPART_H__A41E6879_ECFF_4A12_9061_5BF495AB880C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDialogPart : public CDialog  
{
public:
	class CMainFrame*			m_pRefFrm;
	class CDlgEditScript*		m_pRefBundle;

	CDialogPart();

	virtual ~CDialogPart();

};

#endif // !defined(AFX_DIALOGPART_H__A41E6879_ECFF_4A12_9061_5BF495AB880C__INCLUDED_)
