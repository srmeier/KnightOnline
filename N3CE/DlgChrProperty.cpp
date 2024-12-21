// DlgChrProperty.cpp : implementation file
//

#include "stdafx.h"
#include "DlgChrProperty.h"
#include "MainFrm.h"
#include "N3CEDoc.h"
#include "N3CEView.h"

#include "../N3Base/N3Chr.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDlgChrProperty::CDlgChrProperty(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgChrProperty::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgChrProperty)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgChrProperty::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgChrProperty)
	DDX_Control(pDX, IDC_LP_MATERIAL, m_LPMaterial);
	DDX_Control(pDX, IDC_CB_PART_CHARACTER, m_CBChrPart);
	DDX_Control(pDX, IDC_CB_PLUG_CHARACTER, m_CBChrPlug);
	DDX_Control(pDX, IDC_LP_CHR, m_LPChr);
	DDX_Control(pDX, IDC_LP_CHR_PART, m_LPCPart);
	DDX_Control(pDX, IDC_LP_CHR_PLUG, m_LPCPlug);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgChrProperty, CDialog)
	//{{AFX_MSG_MAP(CDlgChrProperty)
	ON_CBN_SELCHANGE(IDC_CB_PART_CHARACTER, OnSelchangeCbPartCharacter)
	ON_CBN_SELCHANGE(IDC_CB_PLUG_CHARACTER, OnSelchangeCbPlugCharacter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgChrProperty message handlers


BOOL CDlgChrProperty::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int y = 5;
	CRect rc;
	int nH = 0;
	
	int i = 0;
	CString str, strTmp;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 캐릭터
	m_LPChr.AddPropItem("Joint File", "", PIT_FILE, "N3 Joint File(*.N3Joint)|*.N3Joint||");
	m_LPChr.AddPropItem("Collision Mesh File", "", PIT_FILE, "N3 Vector Mesh(*.N3VMesh)|*.N3VMesh||");
	m_LPChr.AddPropItem("Collision Mesh Delete", "Collision Mesh 삭제", PIT_BUTTON, "");
//	m_LPChr.AddPropItem("Collision Skin File", "", PIT_FILE, "N3 Skin File(*.N3Skin)|*.N3Skin||");
//	m_LPChr.AddPropItem("Collision Skin Delete", "Collision Skin 삭제", PIT_BUTTON, "");

	m_LPChr.AddPropItem("Part Add", "Part 추가", PIT_BUTTON, "");
	m_LPChr.AddPropItem("Part Delete", "Part 삭제", PIT_BUTTON, "");
	m_LPChr.AddPropItem("Plug Add", "Plug 추가", PIT_BUTTON, "");
	m_LPChr.AddPropItem("Plug Delete", "Plug 삭제", PIT_BUTTON, "");

	m_CBChrPart.ResetContent();
	m_CBChrPart.SetCurSel(0);

	m_LPCPart.AddPropItem("Part File", "", PIT_FILE, "N3 Character part file(*.N3CPart)|*.N3CPart||");
	m_LPCPart.AddPropItem("Part Type", "", PIT_COMBO, "머리카락|얼굴|상체|하체|손|발|??|");
	m_LPCPart.AddPropItem("Texture File", "", PIT_FILE, "Texture 로 쓸수 있는 그림 파일(*.DXT; *.BMP; *.TGA)|*.DXT; *.BMP; *.TGA|");

	m_LPCPlug.AddPropItem("Plug Name", "", PIT_EDIT, "");
	m_LPCPlug.AddPropItem("Plug Type", "", PIT_COMBO, "오른손장착|왼손장착|양손장착");
	m_LPCPlug.AddPropItem("Plug Joint", "", PIT_EDIT, "");
	m_LPCPlug.AddPropItem("Plug Mesh File", "", PIT_FILE, "N3 Progressive Mesh File(*.N3PMesh)|*.N3PMesh||");
	m_LPCPlug.AddPropItem("Plug Texture File", "", PIT_FILE, "Texture 로 쓸수 있는 그림 파일(*.DXT; *.BMP; *.TGA)|*.DXT; *.BMP; *.TGA|");
	m_LPCPlug.AddPropItem("Plug Offset", "", PIT_EDIT, "");
	m_LPCPlug.AddPropItem("Plug Rotation", "Reset", PIT_BUTTON, "");
	m_LPCPlug.AddPropItem("Plug Scale", "", PIT_EDIT, "");
	// 캐릭터
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////
	// Material 등록 정보
	m_LPMaterial.AddPropItem("Render Flags", "0", PIT_EDIT, "", 0);
	strTmp = "Null|D3DBLEND_ZERO|D3DBLEND_ONE|D3DBLEND_SRCCOLOR|D3DBLEND_INVSRCCOLOR|D3DBLEND_SRCALPHA|D3DBLEND_INVSRCALPHA|D3DBLEND_DESTALPHA|\
D3DBLEND_INVDESTALPHA|D3DBLEND_DESTCOLOR|D3DBLEND_INVDESTCOLOR|D3DBLEND_SRCALPHASAT|D3DBLEND_BOTHSRCALPHA|D3DBLEND_BOTHINVSRCALPHA|";
	m_LPMaterial.AddPropItem("Src Blend", "", PIT_COMBO, strTmp);
	m_LPMaterial.AddPropItem("Dest Blend", "", PIT_COMBO, strTmp);

	strTmp = "Null|D3DTOP_DISABLE|D3DTOP_SELECTARG1|D3DTOP_SELECTARG2|D3DTOP_MODULATE|D3DTOP_MODULATE2X|D3DTOP_MODULATE4X|\
D3DTOP_ADD|D3DTOP_ADDSIGNED|D3DTOP_ADDSIGNED2X|\
D3DTOP_SUBTRACT|D3DTOP_ADDSMOOTH|\
D3DTOP_BLENDDIFFUSEALPHA|D3DTOP_BLENDTEXTUREALPHA|D3DTOP_BLENDFACTORALPHA|D3DTOP_BLENDTEXTUREALPHAPM|D3DTOP_BLENDCURRENTALPHA|\
D3DTOP_PREMODULATE|D3DTOP_MODULATEALPHA_ADDCOLOR|D3DTOP_MODULATECOLOR_ADDALPHA|D3DTOP_MODULATEINVALPHA_ADDCOLOR|D3DTOP_MODULATEINVCOLOR_ADDALPHA|\
D3DTOP_BUMPENVMAP|D3DTOP_BUMPENVMAPLUMINANCE|D3DTOP_DOTPRODUCT|D3DTOP_MULTIPLYADD|D3DTOP_LERP|";
	m_LPMaterial.AddPropItem("Color Operation", "", PIT_COMBO, strTmp);
	strTmp = "D3DTA_DIFFUSE|D3DTA_CURRENTD|3DTA_TEXTURE|D3DTA_TFACTOR|D3DTA_SPECULAR|";
	m_LPMaterial.AddPropItem("Color Arg1", "", PIT_COMBO, strTmp);
	m_LPMaterial.AddPropItem("Color Arg2", "", PIT_COMBO, strTmp);
	m_LPMaterial.AddPropItem("Ambient Color", "", PIT_COLOR, "");
	m_LPMaterial.AddPropItem("Diffuse Color", "", PIT_COLOR, "");
	m_LPMaterial.AddPropItem("Specular Color", "", PIT_COLOR, "");
	m_LPMaterial.AddPropItem("Emissive Color", "", PIT_COLOR, "");
	// Material 등록 정보
	/////////////////////////////////////

	// 창크기 및 위치 조정하기.
	m_LPChr.GetWindowRect(&rc); 
	nH = m_LPChr.GetItemHeight(0) * m_LPChr.GetCount() + 4;
	m_LPChr.SetWindowPos(NULL, rc.left, y, rc.Width(), nH, SWP_NOZORDER);
	y += nH + 4;

	m_CBChrPart.GetWindowRect(&rc);
	m_CBChrPart.SetWindowPos(NULL, rc.left, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_CBChrPart.ShowWindow(SW_SHOW);
	y += rc.Height() + 2;

	m_LPCPart.GetWindowRect(&rc);
	nH = m_LPCPart.GetItemHeight(0) * m_LPCPart.GetCount() + 4;
	m_LPCPart.SetWindowPos(NULL, rc.left, y, rc.Width(), nH, SWP_NOZORDER);
	m_LPCPart.ShowWindow(SW_SHOW);
	y += nH + 4;

	m_CBChrPlug.GetWindowRect(&rc);
	m_CBChrPlug.SetWindowPos(NULL, rc.left, y, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	m_CBChrPlug.ShowWindow(SW_SHOW);
	y += rc.Height() + 2;

	m_LPCPlug.GetWindowRect(&rc);
	nH = m_LPCPlug.GetItemHeight(0) * m_LPCPlug.GetCount() + 4;
	m_LPCPlug.SetWindowPos(NULL, rc.left, y, rc.Width(), nH, SWP_NOZORDER);
	y += nH + 4;

	m_LPMaterial.GetWindowRect(&rc);
	nH = m_LPMaterial.GetItemHeight(0) * m_LPMaterial.GetCount() + 4;
	m_LPMaterial.SetWindowPos(NULL, rc.left, y, rc.Width(), nH, SWP_NOZORDER);
	y += nH + 4;

	// 대화상자 전체 크기..
	this->GetWindowRect(&rc);
	this->SetWindowPos(NULL, 0, 0, rc.Width(), y + GetSystemMetrics(SM_CYCAPTION) + 5, SWP_NOZORDER | SWP_NOMOVE);

	// 중간 분할선 조정
	int nW = 100;
	m_LPChr.SetDividerWidth(nW);
	m_LPCPart.SetDividerWidth(nW);
	m_LPCPlug.SetDividerWidth(nW);
	m_LPMaterial.SetDividerWidth(nW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgChrProperty::UpdateInfo()
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CN3CEDoc* pDoc = (CN3CEDoc*)pFrm->GetActiveDocument();
	CN3Chr* pChr = pDoc->m_Scene.ChrGet(0);

	if(NULL == pChr) return;

	CPropertyItem* pItem = NULL;

	int nPart = m_CBChrPart.GetCurSel();
	int nPartCount = pChr->PartCount();
	m_CBChrPart.ResetContent();
	for(int i = 0; i < nPartCount; i++)
	{
		CString szTmp;
		if(pChr->Part(i)) szTmp.Format("Part(%d) : %s", i, pChr->Part(i)->Name());
		else szTmp.Format("Part(%d) : Null Pointer !!!", i);
		m_CBChrPart.AddString(szTmp);
	}
	
	if(nPart < 0) nPart = 0;
	if(nPart >= nPartCount) nPart = nPartCount - 1;
	m_CBChrPart.SetCurSel(nPart);

	pItem = m_LPChr.GetPropItem("Joint File");
	if(pItem)
	{
		CN3Joint* pJoint = pChr->Joint();
		if(pJoint) pItem->m_curValue = pJoint->Name();
		else pItem->m_curValue = "";
	}

	pItem = m_LPChr.GetPropItem("Collision Mesh File");
	if(pItem)
	{
		CN3VMesh* pMC = pChr->CollisionMesh();
		if(pMC) pItem->m_curValue = pMC->Name();
		else pItem->m_curValue = "";
	}

//		pItem = m_LPChr.GetPropItem("Collision Skin File");
//		if(pItem)
//		{
//			CN3Skin* pSkin = pChr->CollisionSkin();
//			if(pSkin) pItem->m_curValue = pSkin->Name();
//			else pItem->m_curValue = "";
//		}

	__Material* pMtl= NULL;
	if(nPart >= 0 && nPart < nPartCount)
	{
		CN3CPart* pPart = pChr->Part(nPart);

		pMtl = &pPart->m_Mtl; // Material Pointer

		pItem = m_LPCPart.GetPropItem("Part File");
		if(pItem) pItem->m_curValue = pPart->Name();

		pItem = m_LPCPart.GetPropItem("Part Type");
		if(pItem) pItem->m_crColor = pPart->m_Type;

		pItem = m_LPCPart.GetPropItem("Texture File");
		if(pItem)
		{
			CN3Texture* pTex = pPart->Tex();
			if(pTex) pItem->m_curValue = pTex->Name();
			else pItem->m_curValue = "";
		}
	}

	// 붙이는 오브젝트(무기, 장신구 등...) 정보 표시
	int nPlug = m_CBChrPlug.GetCurSel();
	int nPlugCount = pChr->PlugCount();
	m_CBChrPlug.ResetContent();
	for(i = 0; i < nPlugCount; i++)
	{
		CString szTmp;
		szTmp.Format("Plug : %d", i);
		m_CBChrPlug.AddString(szTmp);
	}
	if(nPlug < 0) nPlug = 0;
	if(nPlug >= nPlugCount) nPlug = nPlugCount-1;
	m_CBChrPlug.SetCurSel(nPlug);

	if(nPlug >= 0 && nPlug < nPlugCount)
	{
		CN3CPlug* pPlug = pChr->Plug(nPlug);

		pItem = m_LPCPlug.GetPropItem("Plug Name");
		if(pItem) pItem->m_curValue = pPlug->Name();
		
		pItem = m_LPCPlug.GetPropItem("Plug Type");
		if(pItem) pItem->m_crColor = pPlug->m_Type;
		
		pItem = m_LPCPlug.GetPropItem("Plug Joint");
		if(pItem) pItem->m_curValue.Format("%d", pPlug->m_nJointIndex);
		
		pItem = m_LPCPlug.GetPropItem("Plug Offset");
		if(pItem) pItem->VectorSet(pPlug->Position());

		pItem = m_LPCPlug.GetPropItem("Plug Scale");
		if(pItem) pItem->VectorSet(pPlug->Scale());

		pItem = m_LPCPlug.GetPropItem("Plug Mesh File");
		if(pItem)
		{
			if(pPlug->PMesh()) pItem->m_curValue = pPlug->PMesh()->Name();
			else pItem->m_curValue = "";
		}
	
		pItem = m_LPCPlug.GetPropItem("Plug Texture File");
		if(pItem)
		{
			if(pPlug->Tex()) pItem->m_curValue = pPlug->Tex()->Name();
			else pItem->m_curValue = "";
		}
	}

	m_LPChr.Invalidate(); // Property List Update
	m_LPCPart.Invalidate(); // Property List Update
	m_LPCPlug.Invalidate(); // Property List Update

	// 재질
	if(pMtl)
	{
		pItem = m_LPMaterial.GetPropItem("Render Flags");
		if(pItem) pItem->m_curValue.Format("%d", pMtl->nRenderFlags);

		pItem = m_LPMaterial.GetPropItem("Src Blend");
		if(pItem) pItem->m_crColor = pMtl->dwSrcBlend;

		pItem = m_LPMaterial.GetPropItem("Dest Blend");
		if(pItem) pItem->m_crColor = pMtl->dwDestBlend;

		pItem = m_LPMaterial.GetPropItem("Color Operation");
		if(pItem) pItem->m_crColor = pMtl->dwColorOp;

		pItem = m_LPMaterial.GetPropItem("Color Arg1");
		if(pItem) pItem->m_crColor = pMtl->dwColorArg1;

		pItem = m_LPMaterial.GetPropItem("Color Arg2");
		if(pItem) pItem->m_crColor = pMtl->dwColorArg2;

		pItem = m_LPMaterial.GetPropItem("Ambient Color");
		if(pItem) pItem->D3DColorValueSet(pMtl->Ambient);

		pItem = m_LPMaterial.GetPropItem("Diffuse Color");
		if(pItem) pItem->D3DColorValueSet(pMtl->Diffuse);

		pItem = m_LPMaterial.GetPropItem("Specular Color");
		if(pItem) pItem->D3DColorValueSet(pMtl->Specular);

		pItem = m_LPMaterial.GetPropItem("Emissive Color");
		if(pItem) pItem->D3DColorValueSet(pMtl->Emissive);
		m_LPMaterial.Invalidate();
	}
}

BOOL CDlgChrProperty::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CN3CEDoc* pDoc = (CN3CEDoc*)pFrm->GetActiveDocument();
	CN3Chr* pChr = pDoc->m_Scene.ChrGet(0);

	if(NULL == pChr) return FALSE;

	if((void*)wParam == &m_LPChr)
	{
		CPropertyItem* pItem = (CPropertyItem*)lParam;

		int nPart = m_CBChrPart.GetCurSel();
		int nPlug = m_CBChrPlug.GetCurSel();
		int nPlugCount = pChr->PlugCount();
		
		if(pItem->m_propName == "Joint File" && pItem->m_curValue.GetLength() > 0)
		{
			CN3Base tmp;
			tmp.NameSet(pItem->m_curValue);
			pChr->JointSet(tmp.Name());
			this->UpdateInfo();
		}
//			else if(pItem->m_propName == "Collision Mesh File" && pItem->m_curValue.GetLength() > 0) 
//			{
//				pChr->CollisionMeshSet(pItem->m_curValue);
//				this->UpdateInfo();
//			}
//			else if(pItem->m_propName == "Collision Mesh Delete") 
//			{
//				pChr->CollisionMeshSet("");
//				this->UpdateInfo();
//			}
//			else if(pItem->m_propName == "Collision Skin File" && pItem->m_curValue.GetLength() > 0) 
//			{
//				pChr->CollisionSkinSet(pItem->m_curValue);
//				this->UpdateInfo();
//			}
//			else if(pItem->m_propName == "Collision Skin Delete") 
//			{
//				pChr->CollisionSkinSet("");
//				this->UpdateInfo();
//			}
		else if(pItem->m_propName == "Part Add")
		{
			pChr->PartAdd();
			this->UpdateInfo();
		}
		else if(pItem->m_propName == "Part Delete")
		{
			pChr->PartDelete(nPart);
			this->UpdateInfo();
		}

		else if(pItem->m_propName == "Plug Add")
		{
			pChr->PlugAdd();
			this->UpdateInfo();
		}
		else if(pItem->m_propName == "Plug Delete")
		{
			pChr->PlugDelete(nPlug);
			this->UpdateInfo();
		}
	}

	if((void*)wParam == &m_LPCPart)
	{
		int nPart = m_CBChrPart.GetCurSel();
		int nPartCount = pChr->PartCount();

		CPropertyItem* pItem = (CPropertyItem*)lParam;
		
		if(nPart >= 0 && nPart < nPartCount)
		{
			CN3CPart* pPart = pChr->Part(nPart);
			
			if(pItem->m_propName == "Part File") pPart->NameSet(pItem->m_curValue);
			else if(pItem->m_propName == "Part Type")
			{
				CN3CPart* pPD = pChr->Part(nPart);
				if(pPD)
				{
					if(pItem->m_curValue == "머리카락") pPD->m_Type = PART_HAIR;
					else if(pItem->m_curValue == "얼굴") pPD->m_Type = PART_FACE;
					else if(pItem->m_curValue == "상체") pPD->m_Type = PART_UPPER;
					else if(pItem->m_curValue == "하체") pPD->m_Type = PART_LOWER;
					else if(pItem->m_curValue == "손") pPD->m_Type = PART_HAND;
					else if(pItem->m_curValue == "발") pPD->m_Type = PART_FOOT;
					else if(pItem->m_curValue == "??") pPD->m_Type = PART_UNKNOWN;
				}
				this->UpdateInfo();
			}
			else if(pItem->m_propName == "Texture File" && pItem->m_curValue.GetLength() > 0)
			{
				CN3Base tmp;
				tmp.NameSet(pItem->m_curValue);
				pPart->TexSet(tmp.Name());

				this->UpdateInfo();
			}
		}
	}
	
	if((void*)wParam == &m_LPCPlug)
	{
		int nPlug = m_CBChrPlug.GetCurSel();
		int nPlugCount = pChr->PlugCount();
		CPropertyItem* pItem = (CPropertyItem*)lParam;
		if(nPlug >= 0 && nPlug < nPlugCount)
		{
			CN3CPlug* pPlug = pChr->Plug(nPlug);

			if(pItem->m_propName == "Plug Name") pPlug->NameSet(pItem->m_curValue);
			else if(pItem->m_propName == "Plug Type") pPlug->m_Type = (e_PlugType)pItem->m_crColor;
			else if(pItem->m_propName == "Plug Joint")
			{
				int nJI = atoi(pItem->m_curValue); // Joint Index 가 영역을 벗어나지 못하도록...
				int nJC = 0;
				CN3Joint* pJ = pChr->Joint();
				if(pJ) pJ->NodeCount(nJC); // Joint Node Count;
				if(nJI >= 0 && nJI < nJC) pPlug->m_nJointIndex = nJI;
			}
			else if(pItem->m_propName == "Plug Offset") pPlug->PositionSet(__Vector3(pItem->VectorGet()));
			else if(pItem->m_propName == "Plug Rotation") { __Matrix44 mtx; mtx.Identity(); pPlug->RotationSet(mtx); }
			else if(pItem->m_propName == "Plug Scale") pPlug->ScaleSet(__Vector3(pItem->VectorGet()));
			else if(pItem->m_propName == "Plug Mesh File" && pItem->m_curValue.GetLength() > 0)
			{
				if(pItem->m_curValue != "")
				{
					CN3Base tmp;
					tmp.NameSet(pItem->m_curValue);
					pPlug->PMeshSet(tmp.Name());
				}
				this->UpdateInfo();
			}
			else if(pItem->m_propName == "Plug Texture File" && pItem->m_curValue.GetLength() > 0)
			{
				CN3Base tmp;
				tmp.NameSet(pItem->m_curValue);
				pPlug->TexSet(tmp.Name());

				this->UpdateInfo();
			}
		}
	}

	if((void*)wParam == &m_LPMaterial)
	{
		int nPart = m_CBChrPart.GetCurSel();
		__Material* pMtl = NULL;

		if(pChr->Part(nPart)) pMtl = &(pChr->Part(nPart)->m_Mtl);

		if(pMtl)
		{
			CPropertyItem* pItem = (CPropertyItem*)lParam;

			if(pItem->m_propName == "Render Flags") pMtl->nRenderFlags = atoi(pItem->m_curValue);
			if(pItem->m_propName == "Src Blend") pMtl->dwSrcBlend = pItem->m_crColor;
			if(pItem->m_propName == "Dest Blend") pMtl->dwDestBlend = pItem->m_crColor;
			if(pItem->m_propName == "Color Operation") pMtl->dwColorOp = pItem->m_crColor;
			if(pItem->m_propName == "Color Arg1") pMtl->dwColorArg1 = pItem->m_crColor;
			if(pItem->m_propName == "Color Arg2") pMtl->dwColorArg2 = pItem->m_crColor;
			if(pItem->m_propName == "Ambient Color") pMtl->Ambient = pItem->D3DColorValueGet();
			if(pItem->m_propName == "Diffuse Color") pMtl->Diffuse = pItem->D3DColorValueGet();
			if(pItem->m_propName == "Specular Color") pMtl->Specular = pItem->D3DColorValueGet();
			if(pItem->m_propName == "Emissive Color") pMtl->Emissive = pItem->D3DColorValueGet();
		}
	}

	pFrm->GetPaneRender()->InvalidateRect(NULL, FALSE);
	
	return TRUE;
}

void CDlgChrProperty::OnSelchangeCbPartCharacter() 
{
	this->UpdateInfo();
}

void CDlgChrProperty::OnSelchangeCbPlugCharacter() 
{
	this->UpdateInfo();
}
