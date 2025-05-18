#include "stdafx.h"
#include "TblListCtrl.h"
#include "TblEditorBase.h"
#include "resource.h"

#include <limits>

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
	m_pTblBase	= nullptr;
}

void CTblListCtrl::AttachTbl(
	const CTblEditorBase* pTblBase)
{
	m_pTblBase = pTblBase;
}

void CTblListCtrl::DetachTbl()
{
	m_pTblBase = nullptr;
}

void CTblListCtrl::EscapeForDisplay(
	CString& text)
{
	text.Replace(_T("\\r"), _T("\\\r"));
	text.Replace(_T("\\n"), _T("\\\n"));
	text.Replace(_T("\r"), _T("\\r"));
	text.Replace(_T("\n"), _T("\\n"));
}

void CTblListCtrl::UnescapeForSave(
	CString& text)
{
	text.Replace(_T("\\\r"), _T("\\!!r"));
	text.Replace(_T("\\\n"), _T("\\!!n"));
	text.Replace(_T("\\r"), _T("\r"));
	text.Replace(_T("\\n"), _T("\n"));
	text.Replace(_T("\\!!r"), _T("\\r"));
	text.Replace(_T("\\!!n"), _T("\\n"));
}

template <typename T>
static bool IsValidIntegerInRange(
	CString& szNewText)
{
	static_assert(std::is_integral_v<T>);

	// Ensure the error number is reset from any previous operations.
	errno = 0;

	// Test number assuming base 10 (don't let it guess), using 64-bit for max range.
	const TCHAR* p = szNewText.GetString();
	TCHAR* end = nullptr;
	int64_t testValue = _tcstoll(p, &end, 10);
	if (errno == ERANGE)
	{
		errno = 0; /* reset */
		return false;
	}

	// Leftover unparsed bytes. The entire string needs to be valid.
	if (p == end)
		return false;

	// Out of range for this type.
	if (testValue < std::numeric_limits<T>::min()
		|| testValue > std::numeric_limits<T>::max())
		return false;

	// Valid -- now reformat for consistency
	if constexpr (std::is_same_v<T, int8_t>)
	{
		szNewText.Format(_T("%c"), static_cast<T>(testValue));
	}
	else if constexpr (std::is_unsigned_v<T>)
	{
		szNewText.Format(_T("%u"), static_cast<T>(testValue));
	}
	else
	{
		szNewText.Format(_T("%d"), static_cast<T>(testValue));
	}

	return true;
}

static bool IsValidFloatInRange(
	CString& szNewText)
{
	// Ensure the error number is reset from any previous operations.
	errno = 0;

	// Test number
	const TCHAR* p = szNewText.GetString();
	TCHAR* end = nullptr;
	float testValue = _tcstof(p, &end);
	if (errno == ERANGE)
	{
		errno = 0; /* reset */
		return false;
	}

	// Leftover unparsed bytes. The entire string needs to be valid.
	if (p == end)
		return false;

	// Valid -- now reformat for consistency
	szNewText.Format(_T("%f"), testValue);
	return true;
}

static bool IsValidDoubleInRange(
	CString& szNewText)
{
	// Ensure the error number is reset from any previous operations.
	errno = 0;

	// Test number
	const TCHAR* p = szNewText.GetString();
	TCHAR* end = nullptr;
	double testValue = _tcstod(p, &end);
	if (errno == ERANGE)
	{
		errno = 0; /* reset */
		return false;
	}

	// Leftover unparsed bytes. The entire string needs to be valid.
	if (p == end)
		return false;

	// Valid -- now reformat for consistency
	szNewText.Format(_T("%f"), testValue);
	return true;
}

bool CTblListCtrl::Validate(
	CString& szNewText)
	const
{
	DATA_TYPE columnType = m_pTblBase->GetColumnType(m_iSubItem);
	switch (columnType)
	{
		case DT_CHAR:
			return IsValidIntegerInRange<int8_t>(szNewText);

		case DT_BYTE:
			return IsValidIntegerInRange<uint8_t>(szNewText);

		case DT_SHORT:
			return IsValidIntegerInRange<int16_t>(szNewText);

		case DT_WORD:
			return IsValidIntegerInRange<uint16_t>(szNewText);
			
		case DT_INT:
			return IsValidIntegerInRange<int32_t>(szNewText);
			
		case DT_DWORD:
			return IsValidIntegerInRange<uint32_t>(szNewText);

		case DT_STRING:
			return true;
			
		case DT_FLOAT:
			return IsValidFloatInRange(szNewText);
			
		case DT_DOUBLE:
			return IsValidDoubleInRange(szNewText);
	}

	return false;
}

void CTblListCtrl::ReportValidationFailed(
	const TCHAR* szNewText)
	const
{
	CString errorMsg;
	errorMsg.Format(
		IDS_ERROR_ITEM_VALIDATION_FAILED,
		szNewText,
		m_pTblBase->GetFullColumnName(m_iSubItem));
	AfxMessageBox(errorMsg, MB_ICONERROR);
}

bool CTblListCtrl::IsPrimaryKeyInUse(
	const CString& primaryKey)
	const
{
	// TODO: This should just be memory backed and easily looked up.
	// We shouldn't have to do a full scan of this...
	const int iRowCount = GetItemCount();
	const int iColCount = GetHeaderCtrl()->GetItemCount();

	if (iColCount <= 0)
		return false;

	for (int iRowNo = 0; iRowNo < iRowCount; iRowNo++)
	{
		CString value = GetItemText(iRowNo, 0);
		if (value == primaryKey)
			return true;
	}

	return false;
}

void CTblListCtrl::OnBeginLabelEdit(
	NMHDR* pNMHDR,
	LRESULT* pResult)
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

void CTblListCtrl::OnEndLabelEdit(
	NMHDR* pNMHDR,
	LRESULT* pResult)
{
	LV_DISPINFO* plvDispInfo = (LV_DISPINFO*) pNMHDR;
	LV_ITEM* plvItem = &plvDispInfo->item;

	if (m_iItem == plvItem->iItem
		&& m_iSubItem != 0)
	{
		// NOTE: If pszText is nullptr, we cancelled this edit.
		if (plvItem->pszText != nullptr
			&& m_pTblBase != nullptr)
		{
			CString szNewText = plvItem->pszText;
			if (Validate(szNewText))
				SetItemText(m_iItem, m_iSubItem, szNewText);
			else
				ReportValidationFailed(plvItem->pszText);
		}

		VERIFY(m_edit.UnsubclassWindow() != nullptr);

		m_iSubItem = 0;

		// always revert to original label (Windows thinks we are editing the leftmost item)
		*pResult = 0;
	}
	else
	{
		if (plvItem->pszText != nullptr
			&& m_pTblBase != nullptr)
		{
			CString szNewText = plvItem->pszText;
			if (!Validate(szNewText))
			{
				ReportValidationFailed(plvItem->pszText);

				*pResult = 0;
				return;
			}

			if (IsPrimaryKeyInUse(szNewText))
			{
				CString errorMsg;
				errorMsg.Format(
					IDS_ERROR_PRIMARY_KEY_ALREADY_IN_USE,
					szNewText);
				AfxMessageBox(errorMsg, MB_ICONERROR);

				*pResult = 0;
				return;
			}
		}

		// update label on leftmost item
		*pResult = 1;
	}
}

void CTblListCtrl::OnLButtonDblClk(
	UINT nFlags,
	CPoint point)
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

	CString text = GetItemText(m_iItem, 0);
	CFont* pFontPrev = pDC->SelectObject(GetFont());

	pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	pDC->DrawText(text, &itemRect, DT_LEFT | DT_VCENTER);

	pDC->SelectObject(pFontPrev);

	ReleaseDC(pDC);
}

void CTblListCtrl::OnSize(
	UINT nType,
	int cx,
	int cy)
{
	// stop editing if resizing
	if (GetFocus() != this)
		SetFocus();

	CListCtrl::OnSize(nType, cx, cy);
}
