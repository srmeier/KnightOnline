
// TblEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "framework.h"
#include "TblEditor.h"
#include "TblEditorDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "TblEditorBase.h"

#ifdef min
#undef min
#endif

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

constexpr int MAX_COLUMN_WIDTH = 300; // Allow maximum width of 300px

CTblEditorDlg::CTblEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TBLEDITOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pTblBase = new CTblEditorBase();
}

CTblEditorDlg::~CTblEditorDlg()
{
	delete m_pTblBase;
}

void CTblEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Control(pDX, IDC_EDIT1, m_editCell);
	DDX_Control(pDX, IDC_BUTTON1, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON2, m_btnSave);
}

BEGIN_MESSAGE_MAP(CTblEditorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, &CTblEditorDlg::OnFileOpen)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CTblEditorDlg::OnNMDblclkListCtrl)  // Double click to select list item for editing
	ON_EN_KILLFOCUS(IDC_EDIT1, &CTblEditorDlg::OnEnKillfocusEditCell)
	ON_BN_CLICKED(IDC_BUTTON1, &CTblEditorDlg::OnBnClickedOkButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CTblEditorDlg::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CTblEditorDlg::OnBnClickedBtnAddRow)
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CTblEditorDlg message handlers

BOOL CTblEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ListCtrl stil ayarları
	m_ListCtrl.SetExtendedStyle(
		m_ListCtrl.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_SUBITEMIMAGES |
		LVS_EX_ONECLICKACTIVATE);


	CMenu* pMenu = new CMenu();
	if (pMenu->LoadMenu(IDR_MENU1)) // IDR_MENU1
		SetMenu(pMenu);

	m_staticText.SubclassDlgItem(IDC_STATIC, this);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTblEditorDlg::OnFileOpen()
{
	CFileDialog dlg(
		TRUE,
		nullptr,
		nullptr,
		OFN_FILEMUSTEXIST,
		_T("Tbl Files (*.tbl)|*.tbl|All Files (*.*)|*.*||"));
	if (dlg.DoModal() != IDOK)
		return;

	//get file full path
	CString path = dlg.GetPathName();

	//get file extension
	CString extension = dlg.GetFileExt();
	extension.MakeLower();

	if (extension != _T("tbl"))
	{
		AfxMessageBox(IDS_ERROR_WRONG_FILE_FORMAT, MB_ICONERROR);
		return;
	}

	CTblEditorDlg::LoadTable(path);
}

void CTblEditorDlg::InsertRows(
	const std::map<int, std::vector<CString>>& datas)
{
	m_ListCtrl.DeleteAllItems();

	int nColumnCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	for (int i = nColumnCount - 1; i >= 0; i--)
		m_ListCtrl.DeleteColumn(i);

	size_t columnCount = m_pTblBase->m_DataTypes.size();
	std::vector<int> columnWidths(columnCount, 0);

	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(m_ListCtrl.GetFont());

	// Add column headers and measure initial width
	for (size_t i = 0; i < columnCount; i++)
	{
		CString headerText;
		headerText.Format(_T("Column %d"), static_cast<int>(i + 1));

		CSize size = dc.GetTextExtent(headerText);
		columnWidths[i] = size.cx + 20;  // +20 padding

		m_ListCtrl.InsertColumn(static_cast<int>(i), headerText, LVCFMT_LEFT, 100); // use a width of 100 for now
	}

	// Add rows and measure contents
	for (const auto& it : datas)
	{
		int rowIndex = it.first;
		const std::vector<CString>& columns = it.second;

		CString firstCol;
		if (!columns.empty())
			firstCol = columns[0];

		constexpr int Padding = 10;

		int nItem = m_ListCtrl.InsertItem(rowIndex, firstCol);

		CSize size = dc.GetTextExtent(firstCol);
		if (size.cx + Padding > columnWidths[0])
			columnWidths[0] = std::min<int>(size.cx + Padding, static_cast<int>(MAX_COLUMN_WIDTH));

		for (size_t i = 1; i < columns.size() && i < columnCount; i++)
		{
			const CString& text = columns[i];
			m_ListCtrl.SetItemText(nItem, static_cast<int>(i), text);

			CSize size = dc.GetTextExtent(text);
			if (size.cx + Padding > columnWidths[i])
				columnWidths[i] = std::min<int>(size.cx + Padding, static_cast<int>(MAX_COLUMN_WIDTH));
		}
	}

	// Restore font
	dc.SelectObject(pOldFont);

	// Apply real widths
	for (size_t i = 0; i < columnCount; i++)
		m_ListCtrl.SetColumnWidth(static_cast<int>(i), columnWidths[i]);
}

void CTblEditorDlg::LoadTable(const CString& path)
{
	if (!m_pTblBase->LoadFile(path))
	{
		AfxMessageBox(IDS_ERROR_FILE_NOT_OPENED, MB_ICONERROR);
		return;
	}

	InsertRows(m_pTblBase->m_Rows);

	//fill list than save data to compare later.
	int nItemCount = m_ListCtrl.GetItemCount();
	int nSubItemCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	m_OriginalData.clear();
	m_OriginalData.resize(nItemCount);

	for (int i = 0; i < nItemCount; ++i)
	{
		m_OriginalData[i].resize(nSubItemCount);
		for (int j = 0; j < nSubItemCount; ++j)
		{
			m_OriginalData[i][j] = m_ListCtrl.GetItemText(i, j);
		}
	}

	m_ListCtrl.ModifyStyle(0, LVS_EDITLABELS);
}

void CTblEditorDlg::OnNMDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = (LPNMITEMACTIVATE)pNMHDR;
	if (pNMItemActivate->iItem != -1)
	{
		// Load text from cell to edit controll
		CString strText = m_ListCtrl.GetItemText(pNMItemActivate->iItem, pNMItemActivate->iSubItem);
		m_editCell.SetWindowText(strText);
		m_editCell.SetFocus();

		m_iEditItem = pNMItemActivate->iItem;
		m_iEditSubItem = pNMItemActivate->iSubItem;
	}

	*pResult = 0;
}

void CTblEditorDlg::OnEnKillfocusEditCell()
{
	CString strText;
	m_editCell.GetWindowText(strText); // Get text from edit box

	// Update cell in list control with text
	m_ListCtrl.SetItemText(m_iEditItem, m_iEditSubItem, strText);
}

void CTblEditorDlg::OnBnClickedOkButton()
{
	// Get text from edit controll
	CString strText;
	m_editCell.GetWindowText(strText);

	// Update Clist with new text
	m_ListCtrl.SetItemText(m_iEditItem, m_iEditSubItem, strText);

	// Forcefully update the list
	m_ListCtrl.RedrawItems(m_iEditItem, m_iEditItem);
	m_ListCtrl.UpdateWindow();
}

void CTblEditorDlg::OnBnClickedSaveButton()
{
	if (m_ListCtrl.GetItemCount() == 0)
	{
		AfxMessageBox(IDS_ERROR_NO_DATA_FOUND, MB_ICONERROR);
		return;
	}

	CFileDialog saveDlg(
		FALSE,
		_T("tbl"),
		nullptr,
		OFN_OVERWRITEPROMPT,
		_T("TBL Files (*.tbl)|*.tbl|All Files (*.*)|*.*||"),
		this);
	if (saveDlg.DoModal() != IDOK)
		return;

	CString savePath = saveDlg.GetPathName();

	std::map<int, std::vector<CString>> newData;

	int nRowCount = m_ListCtrl.GetItemCount();
	int nColCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	TRACE("ROW COUNT: %d", nRowCount);

	for (int row = 0; row < nRowCount; ++row)
	{
		// Don't save wrongly edited lines, for example ID = empty
		CString strID = m_ListCtrl.GetItemText(row, 0);
		strID.Trim();
		if (strID.IsEmpty())
		{
			AfxMessageBox(_T("Controll the list, one or more item has no ID"));
			return;
		}
	}

	// Trace m_DataTypes if needed
	TRACE("m_DataTypes:\n");
	for (size_t i = 0; i < m_pTblBase->m_DataTypes.size(); i++)
		TRACE("  [%zu] = %d\n", i, m_pTblBase->m_DataTypes[i]);

	for (int iRowNo = 0; iRowNo < nRowCount; iRowNo++)
	{
		std::vector<CString> rowData;
		rowData.reserve(m_pTblBase->m_DataTypes.size());

		for (int iColNo = 0; iColNo < nColCount; iColNo++)
		{
			CString strText = m_ListCtrl.GetItemText(iRowNo, iColNo);
			strText.Trim(); // Trim text

			// Default to an empty string
			CString value;

			DATA_TYPE dataType = m_pTblBase->m_DataTypes[iColNo];
			switch (dataType)
			{
			case DT_STRING:
				value = CT2A(strText);
				break;

			case DT_CHAR:
			case DT_BYTE:
			case DT_SHORT:
			case DT_WORD:
			case DT_INT:
			case DT_DWORD:
			{
				if (strText.IsEmpty())
					value = _T("0");
				else
					value = CT2A(strText);
				break;
			}

			case DT_FLOAT:
			case DT_DOUBLE:
			{
				if (strText.IsEmpty())
					value = _T("0.0");
				else
					value = CT2A(strText);
				break;
			}

			default:
				value = _T("0");
				break;
			}

			rowData.push_back(value);
		}

		newData[iRowNo] = rowData;
	}

	if (!m_pTblBase->SaveFile(savePath, newData))
	{
		AfxMessageBox(IDS_SAVE_FAILED, MB_ICONERROR);
		return;
	}

	AfxMessageBox(IDS_SAVE_SUCCESS, MB_ICONINFORMATION);
}

void CTblEditorDlg::OnBnClickedBtnAddRow()
{
	int colCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	int selectedIndex = m_ListCtrl.GetSelectionMark();
	int insertIndex = (selectedIndex >= 0) ? selectedIndex + 1 : m_ListCtrl.GetItemCount();

	// Need to insert a row; the first column will be initialised by this.
	m_ListCtrl.InsertItem(insertIndex, _T(""));

	// Set remaining columns as empty
	// TODO: This should use the column-specific default
	for (int col = 1; col < colCount; col++)
		m_ListCtrl.SetItemText(insertIndex, col, _T(""));

	// Yeni eklenen satırı seçili hale getir
	m_ListCtrl.SetItemState(insertIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible(insertIndex, FALSE);
}

BOOL CTblEditorDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		// Avoid closing program with enter and ESC keys
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTblEditorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTblEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
