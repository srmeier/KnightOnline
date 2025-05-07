#include "stdafx.h"
#include "TblListCtrl.h"

BEGIN_MESSAGE_MAP(CTblListCtrl, CListCtrl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginLabelEdit)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndLabelEdit)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CTblListCtrl::CInPlaceEdit, CEdit)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

CTblListCtrl::CTblListCtrl()
{
	m_iItem		= 0;
	m_iSubItem	= 0;
}

void CTblListCtrl::OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*) pNMHDR;

	if (m_iSubItem != 0)
	{
		ASSERT(m_iItem == pDispInfo->item.iItem);

		// get edit control and subclass
		HWND hWnd = (HWND) SendMessage(LVM_GETEDITCONTROL);
		ASSERT(hWnd != nullptr);

		VERIFY(m_edit.SubclassWindow(hWnd));

		CRect subItemRect;
		GetSubItemRect(pDispInfo->item.iItem, m_iSubItem, LVIR_BOUNDS, subItemRect);

		// move edit control text to the right of original label, as Windows does it
		m_edit.SetLeft(subItemRect.left + 6);

		m_edit.SetWindowText(GetItemText(pDispInfo->item.iItem, m_iSubItem));

		// hide subitem text so it doesn't show if we delete some text in the edit control
		CRect rect;
		GetSubItemRect(pDispInfo->item.iItem, m_iSubItem, LVIR_LABEL, rect);

		CDC* pDC = GetDC();

		CBrush brush(GetSysColor(COLOR_WINDOW));
		pDC->FillRect(rect, &brush);

		ReleaseDC(pDC);
	}

	*pResult = 0;
}

void CTblListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO* plvDispInfo = (LV_DISPINFO*) pNMHDR;
	LV_ITEM* plvItem = &plvDispInfo->item;

	if (m_iSubItem != 0)
	{
		if (plvItem->pszText != nullptr)
			SetItemText(plvItem->iItem, m_iSubItem, plvItem->pszText);

		VERIFY(m_edit.UnsubclassWindow() != nullptr);

		m_iSubItem = 0;

		// always revert to original label (Windows thinks we are editing the leftmost item)
		*pResult = 0;
	}
	else
	{
		// update label on leftmost item
		*pResult = 1;
	}
}

void CTblListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	LVHITTESTINFO  lvhit;
	lvhit.pt = point;

	int item = SubItemHitTest(&lvhit);

	CListCtrl::OnLButtonDblClk(nFlags, point);

	if (item != -1
		&& lvhit.iSubItem != -1
		&& (lvhit.flags & LVHT_ONITEM))
	{
		if (m_iSubItem != lvhit.iSubItem
			|| m_iItem != item)
		{
			m_iSubItem = lvhit.iSubItem;
			m_iItem = item;

			EditLabel(item);
		}
	}
}

void CTblListCtrl::OnPaint()
{
	if (m_iSubItem == 0
		|| m_edit.GetSafeHwnd() == nullptr)
	{
		CListCtrl::OnPaint();
		return;
	}

	CRect rc, rcEdit;

	GetSubItemRect(m_iItem, m_iSubItem, LVIR_LABEL, rc);
	m_edit.GetWindowRect(rcEdit);
	ScreenToClient(rcEdit);

	// block text redraw of the subitems text (underneath the editcontrol)
	// if we don't  do this and deleted some text in the edit control,
	// the subitem's original label would show
	if (rcEdit.right < rc.right)
	{
		rc.left = rcEdit.right;
		ValidateRect(rc);
	}

	CListCtrl::OnPaint();

	CDC* pDC = GetDC();

	CRect itemRect;
	GetSubItemRect(m_iItem, 0, LVIR_LABEL, itemRect);

	// TODO: Handle this properly; but we're just trying to align this back to its original
	// intended position (where it gets drawn by Windows).
	itemRect.left += 2;
	itemRect.top += 2;

	CString originalText = GetItemText(m_iItem, 0);
	CFont* pFontPrev = pDC->SelectObject(GetFont());

	pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	
	pDC->DrawText(originalText, &itemRect, DT_LEFT | DT_VCENTER);

	pDC->SelectObject(pFontPrev);

	ReleaseDC(pDC);
}

void CTblListCtrl::OnSize(UINT nType, int cx, int cy)
{
	// stop editing if resizing
	if (GetFocus() != this)
		SetFocus();

	CListCtrl::OnSize(nType, cx, cy);
}
