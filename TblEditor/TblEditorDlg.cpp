#include "stdafx.h"
#include "TblEditor.h"
#include "TblEditorDlg.h"
#include "afxdialogex.h"
#include "Resource.h"
#include "TblEditorBase.h"

#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static constexpr int MAX_COLUMN_WIDTH		= 300; // Allow maximum width of 300px

static constexpr int CODEPAGE_KOREAN		= 949;
static constexpr int CODEPAGE_ENGLISH_US	= 1252;
static constexpr int CODEPAGE_TURKISH		= 1254;

CTblEditorDlg::CTblEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TBLEDITOR_DIALOG, pParent)
{
	m_hIcon				= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pTblBase			= new CTblEditorBase();

	m_bIsFileLoaded		= false;
	m_bIsFileModified	= false;
	m_iEditItem			= 0;
	m_iEditSubItem		= 0;

	m_iStringCodePage	= CODEPAGE_KOREAN;
}

CTblEditorDlg::~CTblEditorDlg()
{
	delete m_pTblBase;
}

void CTblEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CTblEditorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_ACCELERATOR_SAVE, OnFileSave)
	ON_COMMAND(ID_EXIT, OnMenuExit)
	ON_COMMAND(ID_ENCODING_KOREAN, OnSetEncoding_Korean)
	ON_COMMAND(ID_ENCODING_ENGLISH_US, OnSetEncoding_EnglishUS)
	ON_COMMAND(ID_ENCODING_TURKISH, OnSetEncoding_Turkish)
	ON_BN_CLICKED(IDC_BTN_ADD_ROW, OnBnClickedBtnAddRow)
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CTblEditorDlg message handlers

BOOL CTblEditorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ListCtrl style settings
	m_ListCtrl.SetExtendedStyle(
		m_ListCtrl.GetExtendedStyle() |
		LVS_EX_FULLROWSELECT |
		LVS_EX_GRIDLINES |
		LVS_EX_SUBITEMIMAGES |
		LVS_EX_ONECLICKACTIVATE);

	CMenu* pMenu = new CMenu();
	if (pMenu->LoadMenu(IDR_MENU1))
		SetMenu(pMenu);

	m_staticText.SubclassDlgItem(IDC_STATIC, this);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTblEditorDlg::RefreshTable()
{
	const std::map<int, std::vector<CStringA>>& rows
		= m_pTblBase->m_Rows;

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
		const int iColNo = static_cast<int>(i);

		CString headerText;
		headerText.Format(
			_T("%d (%s)"),
			iColNo + 1,
			m_pTblBase->GetColumnName(iColNo));

		CSize size = dc.GetTextExtent(headerText);
		columnWidths[i] = size.cx + 20;  // +20 padding

		m_ListCtrl.InsertColumn(iColNo, headerText, LVCFMT_LEFT, 100); // use a width of 100 for now
	}

	// Add rows and measure contents
	for (const auto& it : rows)
	{
		constexpr int Padding = 10;

		int rowIndex = it.first;
		const std::vector<CStringA>& fields = it.second;

		CStringA firstFieldA;
		if (!fields.empty())
			firstFieldA = fields[0];

		CString firstField = DecodeField(firstFieldA, 0);

		int nItem = m_ListCtrl.InsertItem(rowIndex, firstField);

		CSize size = dc.GetTextExtent(firstField);
		if (size.cx + Padding > columnWidths[0])
			columnWidths[0] = std::min<int>(size.cx + Padding, static_cast<int>(MAX_COLUMN_WIDTH));

		for (size_t i = 1; i < fields.size() && i < columnCount; i++)
		{
			CString text = DecodeField(fields[i], i);
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

CString CTblEditorDlg::DecodeField(
	const CStringA& fieldA,
	int iColNo)
{
	CString field;

	DATA_TYPE columnType = m_pTblBase->GetColumnType(iColNo);
	if (columnType == DT_STRING)
	{
		field = CA2T(fieldA, m_iStringCodePage);
		CTblListCtrl::EscapeForDisplay(field);
	}
	else
	{
		field = fieldA;
	}

	return field;
}

void CTblEditorDlg::LoadTable(const CString& path)
{
	CString errorMsg;
	if (!m_pTblBase->LoadFile(path, errorMsg))
	{
		AfxMessageBox(errorMsg, MB_ICONERROR);
		return;
	}

	m_strLoadedPath = path;

	RefreshTable();

	m_bIsFileLoaded		= true;
	m_bIsFileModified	= false;
}

void CTblEditorDlg::BuildTableForSave(
	std::map<int, std::vector<CStringA>>& newRows)
{
	const int nRowCount = m_ListCtrl.GetItemCount();
	const int nColCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();

	for (int iRowNo = 0; iRowNo < nRowCount; iRowNo++)
	{
		std::vector<CStringA> rowData;
		rowData.reserve(m_pTblBase->m_DataTypes.size());

		for (int iColNo = 0; iColNo < nColCount; iColNo++)
		{
			CString value = m_ListCtrl.GetItemText(iRowNo, iColNo);
			if (value.IsEmpty())
				value = m_pTblBase->GetColumnDefault(iColNo);

			CStringA valueA;

			DATA_TYPE columnType = m_pTblBase->GetColumnType(iColNo);
			if (columnType == DT_STRING)
			{
				CTblListCtrl::UnescapeForSave(value);
				valueA = CT2A(value, m_iStringCodePage);
			}
			else
			{
				valueA = value;
			}

			rowData.push_back(valueA);
		}

		newRows.insert(std::make_pair(iRowNo, rowData));
	}
}

bool CTblEditorDlg::SaveTable(
	const CString& savePath,
	bool bShowConfirmation)
{
	if (!m_bIsFileLoaded)
		return true;

	const int nRowCount = m_ListCtrl.GetItemCount();

	TRACE("ROW COUNT: %d", nRowCount);

	for (int row = 0; row < nRowCount; ++row)
	{
		// Don't save wrongly edited lines, for example ID = empty
		CString strID = m_ListCtrl.GetItemText(row, 0);
		strID.Trim();
		if (strID.IsEmpty())
		{
			AfxMessageBox(_T("Controll the list, one or more item has no ID"));
			return false;
		}
	}

	// Trace m_DataTypes if needed
	TRACE("m_DataTypes:\n");
	for (size_t i = 0; i < m_pTblBase->m_DataTypes.size(); i++)
		TRACE("  [%zu] = %d\n", i, m_pTblBase->m_DataTypes[i]);

	std::map<int, std::vector<CStringA>> newRows;
	BuildTableForSave(newRows);

	if (!m_pTblBase->SaveFile(savePath, newRows))
	{
		AfxMessageBox(IDS_SAVE_FAILED, MB_ICONERROR);
		return false;
	}

	if (bShowConfirmation)
		AfxMessageBox(IDS_SAVE_SUCCESS, MB_ICONINFORMATION);

	// Only consider the file as no longer being modifeid if we saved over
	// the same file we loaded.
	if (savePath.CompareNoCase(m_strLoadedPath) == 0)
		m_bIsFileModified = false;

	return true;
}

void CTblEditorDlg::SetCodePage(
	int codepage)
{
	CMenu* pMenu = GetMenu();
	if (pMenu != nullptr)
	{
		pMenu->CheckMenuItem(ID_ENCODING_KOREAN,		codepage == CODEPAGE_KOREAN		? MF_CHECKED : MF_UNCHECKED);
		pMenu->CheckMenuItem(ID_ENCODING_ENGLISH_US,	codepage == CODEPAGE_ENGLISH_US	? MF_CHECKED : MF_UNCHECKED);
		pMenu->CheckMenuItem(ID_ENCODING_TURKISH,		codepage == CODEPAGE_TURKISH	? MF_CHECKED : MF_UNCHECKED);
	}

	// TODO: List should be backed by this row data to avoid all of this unnecessary work.
	m_pTblBase->m_Rows.clear();
	BuildTableForSave(m_pTblBase->m_Rows);

	m_iStringCodePage = codepage;
	RefreshTable();

	UpdateData(TRUE);
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

	// get file full path
	CString path = dlg.GetPathName();

	// get file extension
	CString extension = dlg.GetFileExt();
	extension.MakeLower();

	if (extension != _T("tbl"))
	{
		AfxMessageBox(IDS_ERROR_WRONG_FILE_FORMAT, MB_ICONERROR);
		return;
	}

	LoadTable(path);
}

void CTblEditorDlg::OnFileSave()
{
	if (!m_bIsFileLoaded)
		return;

	SaveTable(m_strLoadedPath, true);
}

void CTblEditorDlg::OnFileSaveAs()
{
	if (!m_bIsFileLoaded)
		return;

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
	SaveTable(savePath, true);
}

void CTblEditorDlg::OnSetEncoding_Korean()
{
	SetCodePage(CODEPAGE_KOREAN);
}

void CTblEditorDlg::OnSetEncoding_EnglishUS()
{
	SetCodePage(CODEPAGE_ENGLISH_US);
}

void CTblEditorDlg::OnSetEncoding_Turkish()
{
	SetCodePage(CODEPAGE_TURKISH);
}

void CTblEditorDlg::OnMenuExit()
{
	PostQuitMessage(0);
}

void CTblEditorDlg::OnClose()
{
	if (m_bIsFileLoaded
		&& m_bIsFileModified)
	{
		// Do you wish to save before exiting?
		int iResult = AfxMessageBox(IDS_CONFIRM_EXIT, MB_YESNOCANCEL);
		switch (iResult)
		{
			// Cancel: just abort the exit process.
			case IDCANCEL:
				return;

			// Yes: Invoke a save; exit if successful.
			case IDYES:
				// Attempt to save. If it fails, abort the exit process.
				if (!SaveTable(m_strLoadedPath, false))
					return;
				break;

			// No: Don't bother saving, but continue to exit.
			case IDNO:
				break;
		}
	}

	CDialogEx::OnClose();
}

void CTblEditorDlg::OnBnClickedBtnAddRow()
{
	if (!m_bIsFileLoaded)
		return;

	int iColCount = m_ListCtrl.GetHeaderCtrl()->GetItemCount();
	int iSelectedIndex = m_ListCtrl.GetSelectionMark();
	int iInsertIndex = (iSelectedIndex >= 0) ? iSelectedIndex + 1 : m_ListCtrl.GetItemCount();

	// Need to insert a row; the first column will be initialised by this.
	CString szDefault = m_pTblBase->GetColumnDefault(0);
	m_ListCtrl.InsertItem(iInsertIndex, szDefault);

	// Set remaining columns to their defaults.
	for (int iColNo = 1; iColNo < iColCount; iColNo++)
	{
		szDefault = m_pTblBase->GetColumnDefault(iColNo);
		m_ListCtrl.SetItemText(iInsertIndex, iColNo, szDefault);
	}

	// Select the newly added row
	m_ListCtrl.SetItemState(iInsertIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_ListCtrl.EnsureVisible(iInsertIndex, FALSE);
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
