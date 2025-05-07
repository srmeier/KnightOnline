#pragma once

#include <afxcmn.h>

class CListCtrlEx : public CListCtrl
{
	DECLARE_MESSAGE_MAP()

	class CEditEx : public CEdit
	{
		DECLARE_MESSAGE_MAP()

	public:
		CEditEx()
		{
			m_iLeft = 0;
		}

		afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos)
		{
			lpwndpos->x = m_iLeft;
			CEdit::OnWindowPosChanging(lpwndpos);
		}

		void SetLeft(int left) {
			m_iLeft = left;
		}

	protected:
		int m_iLeft;
	};

public:
	CListCtrlEx();

protected:
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	CEditEx	m_edit;

	int		m_iItem;
	int		m_iSubItem;
};

