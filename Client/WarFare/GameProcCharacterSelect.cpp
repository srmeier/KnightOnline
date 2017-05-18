// GameProcCharacterSelect.cpp: implementation of the CGameProcCharacterSelect class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "GameProcCharacterSelect.h"

#include "resource.h"
#include "PacketDef.h"
#include "GameEng.h"
#include "LocalInput.h"
#include "APISocket.h"
#include "PlayerMySelf.h"

#include "UIManager.h"
#include "UICharacterSelect.h"
#include "UIMessageBox.h"
#include "UILoading.h"

#include "N3SndObjStream.h"
#include "N3Shape.h"
#include "N3Camera.h"
#include "N3Light.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcCharacterSelect::CGameProcCharacterSelect()
{
	m_pCamera = NULL;
	for ( int i = 0; i < 8; i++ ) m_pLights[i] = NULL;
	for ( int i = 0; i < MAX_AVAILABLE_CHARACTER; i++ )	{ m_pChrs[i] = NULL; }
	m_pActiveBg = NULL;

	m_eCurPos = POS_CENTER;
	m_eDestPos = POS_CENTER;
	m_eCurProcess = PROCESS_PRESELECT;
	m_fCurTheta = 0.0f;
	m_fFadeOut = 0.0f;
	m_bFadeOutRender = false;

	m_bReceivedCharacterSelect = false;
	m_pUICharacterSelect = NULL;

	m_pSnd_Rotate = NULL;
}

CGameProcCharacterSelect::~CGameProcCharacterSelect()
{
	delete m_pCamera;
	for ( int i = 0; i < 8; i++ ) delete m_pLights[i];
	for ( int i = 0; i < MAX_AVAILABLE_CHARACTER; i++ ) delete m_pChrs[i];
	delete m_pActiveBg;
	delete m_pUICharacterSelect;

	::ShowCursor(TRUE);
}

void CGameProcCharacterSelect::Release()
{
	CGameProcedure::Release();

	delete m_pCamera; m_pCamera = NULL;
	for ( int i = 0; i < 8; i++ ) { delete m_pLights[i]; m_pLights[i] = NULL; }
	for ( int i = 0; i < MAX_AVAILABLE_CHARACTER; i++ )
	{
		delete m_pChrs[i]; m_pChrs[i] = NULL;
		m_InfoChrs[i].clear();
	}

	delete m_pActiveBg; m_pActiveBg = NULL;
	delete m_pUICharacterSelect; m_pUICharacterSelect = NULL;
	CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Rotate);
	
	::ShowCursor(TRUE);
}

void CGameProcCharacterSelect::Init()
{
//..
	m_pCamera = NULL;
	for ( int i = 0; i < 8; i++ ) m_pLights[i] = NULL;
	for ( int i = 0; i < MAX_AVAILABLE_CHARACTER; i++ )	{ m_pChrs[i] = NULL; }
	m_pActiveBg = NULL;

	m_eCurPos = POS_CENTER;
	m_eDestPos = POS_CENTER;
	m_eCurProcess = PROCESS_PRESELECT;
	m_fCurTheta = 0.0f;
	m_fFadeOut = 0.0f;
	m_bFadeOutRender = false;

	m_bReceivedCharacterSelect = false;
	m_pUICharacterSelect = NULL;

	m_pSnd_Rotate = NULL;
//..

	CGameProcedure::Init();
//	CGameProcedure::s_pEng->BackupReleaseLighting();

	CN3Base::s_SndMgr.ReleaseObj(&m_pSnd_Rotate);
	m_pSnd_Rotate = CN3Base::s_SndMgr.CreateObj(ID_SOUND_CHR_SELECT_ROTATE);
	s_pUIMgr->EnableOperationSet(false); // 기존의 캐릭터 정보 패킷이 들어올때까지 UI 를 Disable 시킨다...

	m_pCamera = new CN3Camera();
	for ( int i = 0; i < 8; i++ ) m_pLights[i] = new CN3Light();
	for ( int i = 0; i < MAX_AVAILABLE_CHARACTER; i++ )	m_pChrs[i] = NULL;

	m_eCurPos = POS_CENTER;
	m_eDestPos = POS_CENTER;
	m_eCurProcess = PROCESS_PRESELECT;
	m_fCurTheta = 0.0f;

	CGameProcedure::s_iChrSelectIndex = 0;

	e_Nation eNation = s_pPlayer->m_InfoBase.eNation;
	__TABLE_UI_RESRC* pTbl = s_pTbl_UI->Find(eNation);

	m_pUICharacterSelect = new CUICharacterSelect();
	m_pUICharacterSelect->Init(s_pUIMgr);
	if(pTbl) m_pUICharacterSelect->LoadFromFile(pTbl->szCharacterSelect); // UI Manager 에 넣지 않고 따로 처리한다... 이유가 있다..
	m_pUICharacterSelect->SetPos(0,0);
	s_pUIMgr->SetFocusedUI((CN3UIBase*)m_pUICharacterSelect);

	switch ( eNation )
	{
		case NATION_KARUS:
			m_vEye.Set(0.0f, -0.2f, 7.4f); m_vAt.Set(0.0f, -0.4f, -0.0f); m_vUp.Set(0.0f, 1.0f,0.0f);
			break;

		case NATION_ELMORAD:
			m_vEye.Set(0.0f, -0.2f, 7.0f), m_vAt.Set(0.0f, -0.4f, -0.0f), m_vUp.Set(0.0f, 1.0f,0.0f);
			break;
	}

	m_vEyeBackup = m_vEye;

	// 배경..
	m_pActiveBg = new CN3Shape;	
	memset(&m_lgt[0], 0, sizeof(D3DLIGHT9));	
	memset(&m_lgt[1], 0, sizeof(D3DLIGHT9));	
	memset(&m_lgt[2], 0, sizeof(D3DLIGHT9));	

	// 0가운데.. 1왼쪽..
	m_lgt[2].Type = m_lgt[1].Type = m_lgt[0].Type = D3DLIGHT_SPOT;
	m_lgt[2].Attenuation0 = m_lgt[1].Attenuation0 = m_lgt[0].Attenuation0 = 0.1f;
	m_lgt[2].Attenuation1 = m_lgt[1].Attenuation1 = m_lgt[0].Attenuation1 = 0.0f;
	m_lgt[2].Attenuation2 = m_lgt[1].Attenuation2 = m_lgt[0].Attenuation2 = 0.0f;
	m_lgt[2].Range = m_lgt[1].Range = m_lgt[0].Range = 6.0f;
	m_lgt[2].Diffuse.r = m_lgt[1].Diffuse.r = m_lgt[0].Diffuse.r = 255/255.0f; 
	m_lgt[2].Diffuse.g = m_lgt[1].Diffuse.g = m_lgt[0].Diffuse.g = 255/255.0f; 
	m_lgt[2].Diffuse.b = m_lgt[1].Diffuse.b = m_lgt[0].Diffuse.b = 255/255.0f;
	m_lgt[2].Falloff = m_lgt[1].Falloff = m_lgt[0].Falloff = 20.0f;


	__Vector3 vTemp; 
	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			m_pActiveBg->LoadFromFile("ChrSelect\\ka_chairs.n3shape");
//			m_pActiveBg->LoadFromFile("Misc\\itembox.n3shape");

			// Light..
			m_pLights[0]->LoadFromFile("ChrSelect\\ka_light_0.n3light");	
			m_pLights[1]->LoadFromFile("ChrSelect\\ka_light_1.n3light");	
			m_pLights[2]->LoadFromFile("ChrSelect\\ka_light_2.n3light");	

			m_lgt[0].Position = m_vEye;	// 카루스
			m_lgt[0].Position.y += 2.0f;	// 카루스
			vTemp.Set(0.0f, 0.0f, 3.5f);	vTemp -= m_lgt[0].Position;
			m_lgt[0].Direction = vTemp;
			m_lgt[0].Phi   = 0.6f;

			m_lgt[1].Position = __Vector3(5.87f, 2.4f, 4.73f);	// 카루스
			vTemp.Set(2.32f, 0.0f, 2.54f);	vTemp -= m_lgt[1].Position;
			m_lgt[1].Direction = vTemp;
			m_lgt[1].Phi   = 0.6f;

			m_lgt[2].Position = __Vector3(-5.87f, 2.4f, 4.73f);	// 카루스
			vTemp.Set(-2.32f, 0.0f, 2.54f);	vTemp -= m_lgt[2].Position;
			m_lgt[2].Direction = vTemp;
			m_lgt[2].Phi   = 0.6f;
			break;

		case NATION_ELMORAD:
			m_pActiveBg->LoadFromFile("ChrSelect\\el_chairs.n3shape");

			// Light..
			m_pLights[0]->LoadFromFile("ChrSelect\\el_light_0.n3light");	
			m_pLights[1]->LoadFromFile("ChrSelect\\el_light_1.n3light");	
			m_pLights[2]->LoadFromFile("ChrSelect\\el_light_2.n3light");	

			m_lgt[0].Position = m_vEye;	// 카루스
			m_lgt[0].Position.y += 2.0f;	// 카루스
			vTemp.Set(0.0f, -0.1f, 3.0f);	vTemp -= m_lgt[0].Position;
			m_lgt[0].Direction = vTemp;
			m_lgt[0].Phi   = 0.45f;

			m_lgt[1].Position = __Vector3(5.6f, 2.4f, 4.68f);	// 카루스
			vTemp.Set(2.2f, -0.1f, 2.36f);	vTemp -= m_lgt[1].Position;
			m_lgt[1].Direction = vTemp;
			m_lgt[1].Phi   = 0.45f;

			m_lgt[2].Position = __Vector3(-5.6f, 2.4f, 4.68f);	// 카루스
			vTemp.Set(-2.4f, -0.1f, 2.23f);	vTemp -= m_lgt[2].Position;
			m_lgt[2].Direction = vTemp;
			m_lgt[2].Phi   = 0.45f;
			break;
	}

	this->MsgSend_RequestAllCharacterInfo(); // 캐릭터 정보 요청..
}

void CGameProcCharacterSelect::Tick()
{
	CGameProcedure::Tick();	// 키, 마우스 입력 등등..

	__Vector3 vDir = CN3Base::s_CameraData.vAt - CN3Base::s_CameraData.vEye; vDir.Normalize();
	__Vector3 vEye = CN3Base::s_CameraData.vEye; 
	__Vector3 vUp(0,1,0);
	CN3SndObj::SetListenerPos(&vEye);
	CN3SndObj::SetListenerOrientation(&vDir, &vUp);

// 배경..
	m_pActiveBg->Tick();

	if(s_pUIMgr->m_bDoneSomething == false && s_pUIMgr->EnableOperation()) // 패킷을 받기 전에 아무짓도 못하게 한다.
	{
		s_pUIMgr->SetFocusedUI(m_pUICharacterSelect);
		int nMFlags = s_pLocalInput->MouseGetFlag();					// Mouse 상태 플래그..
		if(nMFlags & MOUSE_LBCLICK) // 누르는 순간..
		{
			if (m_eCurProcess == PROCESS_ROTATEING)
				goto NowRotating;

			D3DVIEWPORT9 vp;
			CN3Base::s_lpD3DDev->GetViewport(&vp);
			
			RECT rc = { vp.Width * 0.36f, vp.Height * 0.44f, vp.Width * 0.64f, vp.Height * 0.86f };
			POINT pt = s_pLocalInput->MouseGetPos();

			if ( ::PtInRect(&rc, pt) )
			{
				CharacterSelectOrCreate();
			}
		}
	}

NowRotating:

// 라이트..
	for(int i = 0; i < 8; i++) s_pEng->s_lpD3DDev->LightEnable(i, FALSE); // 일단 라이트 다 끄고..
	
	for(int i = 0; i < 2; i++)
	{
		m_pLights[i]->Tick(m_pLights[i]->m_fFrmCur);
		m_pLights[i]->Apply(); // 라이트 적용
	}
	
	CheckJobState();

	if (!IsUIKeyOperated())
	{
		if ( s_pLocalInput->IsKeyPress(SDL_SCANCODE_RETURN) )									 
			ProcessOnReturn();
	}
}

void CGameProcCharacterSelect::Render()
{
	D3DCOLOR crEnv = 0x00000000;
	s_pEng->Clear(crEnv); // 배경은 검은색
	s_pEng->s_lpD3DDev->BeginScene();			// 씬 렌더 ㅅ작...

	__Matrix44 mtxWorld;
	mtxWorld.Identity();
	CN3Base::s_lpD3DDev->SetTransform(D3DTS_WORLD, &mtxWorld); 

	m_pCamera->EyePosSet(m_vEye);
	m_pCamera->AtPosSet(m_vAt);
	m_pCamera->UpVectorSet(0,1,0);
	m_pCamera->m_Data.fNP = 0.1f;
	m_pCamera->m_Data.fFP = 100.0f;
	m_pCamera->m_Data.fFOV = 0.96f;

	m_pCamera->Tick();
	m_pCamera->Apply();

// 배경..
	m_pActiveBg->Render();

// 캐릭터..
	if ( m_pChrs[0] )
	{
		m_pChrs[0]->m_nLOD = 0;
		m_pChrs[0]->Render();
	}
	if ( m_pChrs[1] )
	{
		m_pChrs[1]->m_nLOD = 0;
		m_pChrs[1]->Render();
	}
	if ( m_pChrs[2] )
	{
		m_pChrs[2]->m_nLOD = 0;
		m_pChrs[2]->Render();
	}

	CUIManager::RenderStateSet();
	m_pUICharacterSelect->Render();
	CUIManager::RenderStateRestore();

	CGameProcedure::Render(); // UI 나 그밖의 기본적인 것들 렌더링..

	if (m_bFadeOutRender) FadeOutRender();

	s_pEng->s_lpD3DDev->EndScene();			// 씬 렌더 시작...
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcCharacterSelect::AddChr(e_ChrPos eCP, __CharacterSelectInfo* pInfo)
{
	int	iPosIndex;
	switch( eCP )
	{
		case POS_CENTER:
			iPosIndex = 0;
			break;

		case POS_LEFT:
			iPosIndex = 1;
			break;

		case POS_RIGHT:
			iPosIndex = 2;
			break;
	}

	if(NULL == m_pChrs[iPosIndex])
		m_pChrs[iPosIndex] = new CN3Chr();
	else
		m_pChrs[iPosIndex]->Release();

	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks->Find(pInfo->eRace);	// User Player Character Skin 구조체 포인터..
	__ASSERT(pLooks, "Table 에서 기본 리소스 찾기 실패.");

	m_pChrs[iPosIndex]->PartAlloc(PART_POS_COUNT);
	m_pChrs[iPosIndex]->PlugAlloc(PLUG_POS_COUNT);
	std::string szJointFN, szAniFN, szPlug0FN, szPlug1FN;

	switch ( pInfo->eRace )
	{
		case RACE_EL_BABARIAN:
			// 남자 바바리안은 직업이 하나..
			szJointFN	= "ChrSelect\\upc_el_ba_wa.n3joint"; // 관절 세팅..
			szAniFN		= "ChrSelect\\upc_el_ba_wa.n3anim"; // 에니메이션 파일 이름.. :-D;
			szPlug0FN	= "ChrSelect\\wea_el_great_sword.n3cplug";
			szPlug1FN	= "";
			break;
		case RACE_EL_WOMEN:
			// 엘모 여자는 직업이 현재 세개(전사, 로그, 성직자).. 성직자는 그래픽이 없다..
			switch ( pInfo->eClass )
			{
				case CLASS_EL_WARRIOR:
				case CLASS_EL_BLADE:
				case CLASS_EL_PROTECTOR:
					szJointFN	= "ChrSelect\\upc_el_rf_wa.n3joint";
					szAniFN		= "ChrSelect\\upc_el_rf_wa.n3anim"; // 에니메이션 파일 이름.. :-D;
					szPlug0FN	= "ChrSelect\\wea_el_long_sword_left.n3cplug"; // 왼손에 검을 찬다..
					szPlug1FN	= "";
					break;
				case CLASS_EL_ROGUE:
				case CLASS_EL_RANGER:
				case CLASS_EL_ASSASIN:
					szJointFN	= "ChrSelect\\upc_el_rf_rog.n3joint"; // 관절 세팅..
					szAniFN		= "ChrSelect\\upc_el_rf_wa.n3anim"; // 에니메이션 파일 이름.. :-D;
					szPlug0FN	= "ChrSelect\\wea_el_rf_rog_bow.n3cplug";
					szPlug1FN	= "ChrSelect\\wea_el_quiver.n3cplug";
					break;
				case CLASS_EL_WIZARD:
				case CLASS_EL_MAGE:
				case CLASS_EL_ENCHANTER:
					szJointFN	= "ChrSelect\\upc_el_rf_wiz.n3joint"; // 관절 세팅..
					szAniFN		= "ChrSelect\\upc_el_rf_wa.n3anim"; // 에니메이션 파일 이름.. :-D;
					szPlug0FN	= "ChrSelect\\upc_el_rf_wiz.n3cplug";
					szPlug1FN	= "";
					break;
				case CLASS_EL_PRIEST:	
				case CLASS_EL_CLERIC:
				case CLASS_EL_DRUID:
					szJointFN	= "ChrSelect\\upc_el_rf_pri.n3joint"; // 관절 세팅..
					szAniFN		= "ChrSelect\\upc_el_rf_wa.n3anim"; // 에니메이션 파일 이름.. :-D;
					szPlug0FN	= "ChrSelect\\wea_el_wand.n3cplug";
					szPlug1FN	= "";
					break;
			}
			break;
		case RACE_EL_MAN:
			// 엘모 남자는 현재 직업이 세개..(로그, 성직자, 전사)
			switch ( pInfo->eClass )
			{
				case CLASS_EL_WARRIOR:
				case CLASS_EL_BLADE:
				case CLASS_EL_PROTECTOR:
					szJointFN	= "ChrSelect\\upc_el_rm_wa.n3joint";
					szAniFN		= "ChrSelect\\upc_el_rm_wa.n3anim";
					szPlug0FN	= "ChrSelect\\wea_el_long_sword.n3cplug";
					break;
				case CLASS_EL_ROGUE:
				case CLASS_EL_RANGER:
				case CLASS_EL_ASSASIN:
					szJointFN	= "ChrSelect\\upc_el_rm_rog.n3joint";
					szAniFN		= "ChrSelect\\upc_el_rm_rog.n3anim";
					szPlug0FN	= "ChrSelect\\upc_el_rm_rog_bow.n3cplug";
					szPlug1FN	= "ChrSelect\\wea_el_quiver.n3cplug";
					break;
				case CLASS_EL_WIZARD:
				case CLASS_EL_MAGE:
				case CLASS_EL_ENCHANTER:
					szJointFN	= "ChrSelect\\upc_el_rm_ma.n3joint";
					szAniFN		= "ChrSelect\\upc_el_rm_rog.n3anim";
					szPlug0FN	= "ChrSelect\\upc_el_rm_wiz.n3cplug";
					szPlug1FN	= "";
					break;
				case CLASS_EL_PRIEST:
				case CLASS_EL_CLERIC:
				case CLASS_EL_DRUID:
					szJointFN	= "ChrSelect\\upc_el_rm_pri.n3joint";
					szAniFN		= "ChrSelect\\upc_el_rm_rog.n3anim";
					szPlug0FN	= "ChrSelect\\wea_el_wand.n3cplug";
					szPlug1FN	= "";
					break;
			}
			break;
		case RACE_KA_ARKTUAREK:
			// 카루스 직업 하나.. 온리 전사..
			szJointFN	= "ChrSelect\\upc_ka_at_wa.n3joint";
			szAniFN		= "ChrSelect\\upc_ka_at_wa.n3anim";
			szPlug0FN	= "ChrSelect\\wea_ka_great_axe.n3cplug";
			szPlug1FN	= "";
			break;
		case RACE_KA_TUAREK:
			switch( pInfo->eClass )
			{
				case CLASS_KA_ROGUE:
				case CLASS_KA_HUNTER:
				case CLASS_KA_PENETRATOR:
					szJointFN	= "ChrSelect\\upc_ka_tu_rog.n3joint";
					szAniFN		= "ChrSelect\\upc_ka_at_wa.n3anim";
					szPlug0FN	= "ChrSelect\\wea_ka_bow.n3cplug";
					szPlug1FN	= "ChrSelect\\wea_ka_quiver.n3cplug";
					break;
				case CLASS_KA_PRIEST:
				case CLASS_KA_SHAMAN:
				case CLASS_KA_DARKPRIEST:
					szJointFN	= "ChrSelect\\upc_ka_tu_pri.n3joint";
					szAniFN		= "ChrSelect\\upc_ka_at_wa.n3anim";
					szPlug0FN	= "ChrSelect\\wea_ka_mace.n3cplug";
					szPlug1FN	= "";
					break;
			}
			break;
		case RACE_KA_WRINKLETUAREK:	
			szJointFN	= "ChrSelect\\upc_ka_wt_ma.n3joint";
			szAniFN		= "ChrSelect\\upc_ka_at_wa.n3anim";
			szPlug0FN	= "ChrSelect\\wea_ka_staff.n3cplug";
			szPlug1FN	= "";
			break;
		case RACE_KA_PURITUAREK:
			szJointFN	= "ChrSelect\\upc_el_rf_pri.n3joint"; // 관절 세팅..
			szAniFN		= "ChrSelect\\upc_el_rf_wa.n3anim"; // 에니메이션 파일 이름.. :-D;
			szPlug0FN	= "ChrSelect\\wea_ka_mace.n3cplug";
			szPlug1FN	= "";
			break;
		default:
			return;
	}

	m_pChrs[iPosIndex]->JointSet(szJointFN); // 관절 세팅..
	m_pChrs[iPosIndex]->AniCtrlSet(szAniFN); // 에니메이션 파일 이름.. :-D;
	m_pChrs[iPosIndex]->PlugSet(0, szPlug0FN);
	m_pChrs[iPosIndex]->PlugSet(1, szPlug1FN);

	// 상체..
	this->AddChrPart(iPosIndex, pLooks, PART_POS_UPPER, pInfo->dwItemUpper, pInfo->iItemUpperDurability);
	// 하체
	__TABLE_ITEM_BASIC* pItemUpper = this->s_pTbl_Items_Basic->Find(pInfo->dwItemUpper);
	if(pItemUpper && pItemUpper->byIsRobeType) m_pChrs[iPosIndex]->PartSet(PART_POS_LOWER, ""); // 하체일 경우 상체에 로브를 입었으면 ..
	else this->AddChrPart(iPosIndex, pLooks, PART_POS_LOWER, pInfo->dwItemLower, pInfo->iItemLowerDurability); //아님 입히고...
	// 팔
	this->AddChrPart(iPosIndex, pLooks, PART_POS_HANDS, pInfo->dwItemGloves, pInfo->iItemGlovesDurability);
	// 다리
	this->AddChrPart(iPosIndex, pLooks, PART_POS_FEET, pInfo->dwItemShoes, pInfo->iItemShoesDurability);

	char szBuff[256] = "";
	std::string szResrcFN;

	// 얼굴 - 
	if(!pLooks->szPartFNs[PART_POS_FACE].empty())
	{
		char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
		::_splitpath(pLooks->szPartFNs[PART_POS_FACE].c_str(), NULL, szDir, szFName, szExt);
		sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, pInfo->iFace, szExt);
		m_pChrs[iPosIndex]->PartSet(PART_POS_FACE, szBuff);
	}

	// 머리카락 혹은 헬멧 - 
	__TABLE_ITEM_BASIC* pItemHelmet = this->s_pTbl_Items_Basic->Find(pInfo->dwItemHelmet);
	if(pItemHelmet && pItemHelmet->dwIDResrc) // 헬멧아이템의 리소스(그림이 있는거면..)
	{
		this->AddChrPart(iPosIndex, pLooks, PART_POS_HAIR_HELMET, pInfo->dwItemHelmet, pInfo->iItemHelmetDurability);
	}
	else if(!pLooks->szPartFNs[PART_POS_HAIR_HELMET].empty()) // 아이템이 없으면 기본 머리..
	{
		char szBuff[256] = "", szDir[128] = "", szFName[128] = "", szExt[16] = "";
		::_splitpath(pLooks->szPartFNs[PART_POS_HAIR_HELMET].c_str(), NULL, szDir, szFName, szExt);
		sprintf(szBuff, "%s%s%.2d%s", szDir, szFName, pInfo->iHair, szExt);
		m_pChrs[iPosIndex]->PartSet(PART_POS_HAIR_HELMET, szBuff);
	}
	else 
	{
		m_pChrs[iPosIndex]->PartSet(PART_POS_HAIR_HELMET, ""); // 헬멧도 아니고 머리카락도 없으면.. 대머리다!
	}


	__Quaternion qt;

	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			switch ( iPosIndex )
			{
				case 0:
					m_pChrs[0]->PosSet(0.0f, -1.16f, 2.72f);		// ok..
					qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(0.0f));
					m_pChrs[0]->RotSet(qt);
					break;

				case 1:
					m_pChrs[1]->PosSet(1.86f, -1.16f, 2.1f);		// ok..		
					qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(42.0f));
					m_pChrs[1]->RotSet(qt);
					break;

				case 2:
					m_pChrs[2]->PosSet(-1.9f, -1.16f, 2.1f);		// ok..
					qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(-42.0f));
					m_pChrs[2]->RotSet(qt);
					break;
			};
			break;

		case NATION_ELMORAD:
			switch ( iPosIndex )
			{
				case 0:
					m_pChrs[0]->PosSet(0.0f, -1.20f, 2.74f);		// ok..
					qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(0.0f));
					m_pChrs[0]->RotSet(qt);
					break;

				case 1:
					m_pChrs[1]->PosSet(1.86f, -1.20f, 2.0f);		// ok
					qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(42.0f));
					m_pChrs[1]->RotSet(qt);
					break;

				case 2:
					m_pChrs[2]->PosSet(-1.9f, -1.20f, 2.0f);		// ok
					qt.RotationAxis(0.0f, 1.0f, 0.0f, D3DXToRadian(-46.0f));
					m_pChrs[2]->RotSet(qt);
					break;
			};
			break;
	}

	for(int i = 0; i < 3; i++ )
	{
		if ( m_pChrs[i] )
		{
			m_pChrs[i]->AniCurSet(SELECT_ANIM_PRE_SELECT, true, 0);
			m_pChrs[i]->Tick();
		}
	}
}

void CGameProcCharacterSelect::AddChrPart(	int iPosIndex,
											const __TABLE_PLAYER_LOOKS* pLooks,
											e_PartPosition ePartPos,
											DWORD dwItemID,
											int iItemDurability)
{
	if(NULL == m_pChrs[iPosIndex] || NULL == pLooks)
	{
		__ASSERT(0, "Invalid Chr Part");
		return;
	}

	__TABLE_ITEM_BASIC* pItem = s_pTbl_Items_Basic->Find(dwItemID/1000*1000);
	if(dwItemID && NULL == pItem)
	{
		__ASSERT(0, "NULL Item!!!");
		return;
	}

	std::string szResrcFN;
	CN3CPart* pPart = NULL;
	e_PartPosition ePartPosTmp = PART_POS_UNKNOWN;
	e_PlugPosition ePlugPosTmp = PLUG_POS_UNKNOWN;

	CGameProcedure::MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, m_InfoChrs[iPosIndex].eRace);
	if(szResrcFN.empty()) pPart = m_pChrs[iPosIndex]->PartSet(ePartPos, pLooks->szPartFNs[ePartPos]);	// 기본 파트
	else pPart = m_pChrs[iPosIndex]->PartSet(ePartPos, szResrcFN);
	if(pPart && pItem && pItem->siMaxDurability > 0)
	{
		int iPercentage = iItemDurability * 100 / pItem->siMaxDurability;
		if(iPercentage <= 30) pPart->TexOverlapSet("Misc\\Dust_Hard.dxt");
		else if(iPercentage <= 70) pPart->TexOverlapSet("Misc\\Dust_Soft.dxt");
		else pPart->TexOverlapSet("");
	}
}

void CGameProcCharacterSelect::MsgRecv_DeleteChr(DataPack* pDataPack, int& iOffset)
{
	BYTE byResult, byIndex; 
	byResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	byIndex  = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	
	if ( byResult == 0x01 )
	{
		if ( m_pChrs[byIndex] != NULL )
		{
			m_pChrs[byIndex]->Release();
			delete m_pChrs[byIndex];
			m_pChrs[byIndex] = NULL;
			m_InfoChrs[byIndex].clear();
		}

		if ( (m_pChrs[0] == NULL) && (m_pChrs[1] == NULL) && (m_pChrs[2] == NULL) )
			CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcNationSelect); // 국가 선택으로 간다..
	}
}

int	CGameProcCharacterSelect::MsgRecv_VersionCheck(DataPack* pDataPack, int& iOffset) // virtual
{
	int iVersion = CGameProcedure::MsgRecv_VersionCheck(pDataPack, iOffset);
	if(iVersion == CURRENT_VERSION)
	{
		this->MsgSend_CharacterSelect(); // 게임 서버에 로그인..
	}

	return iVersion;
}


bool CGameProcCharacterSelect::MsgRecv_CharacterSelect(DataPack* pDataPack, int& iOffset) // virtual
{
	bool bSuccess = CGameProcedure::MsgRecv_CharacterSelect(pDataPack, iOffset);

	if(bSuccess) this->CharacterSelect(); // 캐릭터를 일으킨다..
	else this->CharacterSelectFailed();

	return bSuccess;
}

void CGameProcCharacterSelect::ProcessOnReturn()
{
	if(!m_bReceivedCharacterSelect) return;
	//엔터키 눌렸을때 라이트 때문에 깜빡이는것 없애기 위해...

	if ( m_eCurProcess != PROCESS_ROTATEING )
	{
		int iIndex;
		switch ( m_eCurPos )
		{
			case POS_CENTER:
				iIndex = 0;
				break;

			case POS_LEFT:
				iIndex = 1;
				break;

			case POS_RIGHT:
				iIndex = 2;
				break;
		}

		// Light..
		switch ( s_pPlayer->m_InfoBase.eNation )
		{
			case NATION_KARUS:
				m_lgt[iIndex].Theta = KARUS_THETA_MAX;

				s_pEng->s_lpD3DDev->LightEnable(iIndex+4, TRUE);
				s_pEng->s_lpD3DDev->SetLight(iIndex+4, &m_lgt[iIndex]);
				break;

			case NATION_ELMORAD:
				m_lgt[iIndex].Theta = ELMORAD_THERA_MAX;

				s_pEng->s_lpD3DDev->LightEnable(iIndex+4, TRUE);
				s_pEng->s_lpD3DDev->SetLight(iIndex+4, &m_lgt[iIndex]);
				break;
		}

		s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcMain); // 캐릭터 고르기에 성공하면.. 메인으로 가자!!
//		CGameProcedure::s_pEng->RestoreLighting();
		this->s_pUILoading->Render("Loading data...", 0);
	}
}

bool CGameProcCharacterSelect::CheckRotateLeft()
{
	__Matrix44 mtxrot; mtxrot.Identity();
	__Vector3 vOffs, v1, v2, v3;
	__Vector3 vEyeTemp;
	bool	bReturn = false; 

	vEyeTemp = m_vEye;
	mtxrot.RotationY(0.01f);
	vOffs = vEyeTemp - m_vAt;	
	vOffs *= mtxrot;
	vEyeTemp = vOffs + m_vAt;

	v1 = m_vEyeBackup - m_vAt;
	v2 = vEyeTemp - m_vAt;
	v1.Normalize(); v2.Normalize();

	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			if ( v1.Dot(v2) < 0.75f )
			{
				bReturn = true;
				m_eCurProcess = PROCESS_PRESELECT;
			}
			break;

		case NATION_ELMORAD:
			if ( v1.Dot(v2) < 0.74f )
			{
				bReturn = true;
				m_eCurProcess = PROCESS_PRESELECT;
			}
			break;
	}

	return bReturn;
}

bool CGameProcCharacterSelect::CheckRotateCenterToRight()
{
	if ( m_vEye.x <= 0.0f )
	{
		m_vEye = m_vEyeBackup;
		m_eCurProcess = PROCESS_PRESELECT;
		return true;
	}
	else
		return false;
}

bool CGameProcCharacterSelect::CheckRotateCenterToLeft()
{
	if ( m_vEye.x >= 0.0f )
	{
		m_vEye = m_vEyeBackup;
		m_eCurProcess = PROCESS_PRESELECT;
		return true;
	}
	else
		return false;}


bool CGameProcCharacterSelect::CheckRotateRight()
{
	__Matrix44 mtxrot; mtxrot.Identity();
	__Vector3 vOffs, v1, v2, v3;
	__Vector3 vEyeTemp;
	bool	bReturn = false; 

	vEyeTemp = m_vEye;
	mtxrot.RotationY(-0.01f);
	vOffs = vEyeTemp - m_vAt;	
	vOffs *= mtxrot;
	vEyeTemp = vOffs + m_vAt;

	v1 = m_vEyeBackup - m_vAt;
	v2 = vEyeTemp - m_vAt;
	v1.Normalize(); v2.Normalize();

	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			if ( v1.Dot(v2) < 0.77f )
			{
				bReturn = true;
				m_eCurProcess = PROCESS_PRESELECT;
			}
			break;

		case NATION_ELMORAD:
			if ( v1.Dot(v2) < 0.73f )
			{
				bReturn = true;
				m_eCurProcess = PROCESS_PRESELECT;
			}
			break;
	}

	return bReturn;
}

void CGameProcCharacterSelect::DoJobLeft()
{
	if ( m_eCurPos != POS_LEFT )
	{
		if ( m_eCurProcess == PROCESS_ROTATEING )
			return;
		else
			m_eCurProcess = PROCESS_ROTATEING;
	}

	switch ( m_eCurPos )
	{
		case POS_CENTER:
			m_eDestPos = POS_LEFT;
			CGameProcedure::s_iChrSelectIndex = 1;
			break;

		case POS_RIGHT:
			m_eDestPos = POS_CENTER;
			CGameProcedure::s_iChrSelectIndex = 0;
			break;
	}

	if ( (m_eCurPos != POS_LEFT) && m_pSnd_Rotate) m_pSnd_Rotate->Play();
}


void CGameProcCharacterSelect::DojobRight()
{
	if ( m_eCurPos != POS_RIGHT )
	{
		if ( m_eCurProcess == PROCESS_ROTATEING )
			return;
		else
			m_eCurProcess = PROCESS_ROTATEING;
	}

	switch ( m_eCurPos )
	{
		case POS_CENTER:
			m_eDestPos = POS_RIGHT;
			CGameProcedure::s_iChrSelectIndex = 2;
			break;

		case POS_LEFT:
			m_eDestPos = POS_CENTER;
			CGameProcedure::s_iChrSelectIndex = 0;
			break;
	}

	if ( (m_eCurPos != POS_RIGHT) && m_pSnd_Rotate) m_pSnd_Rotate->Play();
}

void CGameProcCharacterSelect::MsgSend_DeleteChr(const std::string& szKey)
{
	if(szKey.empty() || szKey.size() >= 32) return;

	// 현재 상태가 캐릭터를 선택하지 않은 상태..
	if ( m_eCurProcess != PROCESS_PRESELECT )
		return;

	int iIndex = -1;
	switch ( m_eCurPos )
	{
		case POS_CENTER: iIndex = 0; break;
		case POS_LEFT: iIndex = 1; break;
		case POS_RIGHT: iIndex = 2; break;
		default: return;
	}

	BYTE byBuff[64];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_DEL_CHAR);		// 커멘드.
	CAPISocket::MP_AddByte(byBuff, iOffset, (BYTE)iIndex);				// 인덱스 - b
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)m_InfoChrs[iIndex].szID.size());		// 아이디 길이
	CAPISocket::MP_AddString(byBuff, iOffset, m_InfoChrs[iIndex].szID); // 아이디 문자열
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)szKey.size());				// 주민등록번호 길이
	CAPISocket::MP_AddString(byBuff, iOffset, szKey);					// 주민등록번호 문자열
	
	s_pSocket->Send(byBuff, iOffset);								// 보낸다
}

void CGameProcCharacterSelect::CheckJobState()
{
	switch ( m_eCurProcess )
	{
		case PROCESS_ROTATEING:
			if ( m_eDestPos == m_eCurPos )
				return;

			switch ( m_eCurPos )
			{
				case POS_CENTER:
					switch ( m_eDestPos )
					{
						case POS_LEFT:
							// Check..
							if ( !CheckRotateLeft() )
								RotateLeft();
							else
								m_eCurPos = m_eDestPos;
							break;

						case POS_RIGHT:
							// Check..
							if ( !CheckRotateRight() )
								RotateRight();
							else
								m_eCurPos = m_eDestPos;
							break;
					}
					break;

				case POS_LEFT:
					if ( m_eDestPos == POS_CENTER )
					{
						// Check..
						if ( !CheckRotateCenterToRight() )
							RotateRight();
						else
							m_eCurPos = m_eDestPos;
					}
					break;

				case POS_RIGHT:
					if ( m_eDestPos == POS_CENTER )
					{
						// Check..
						if ( !CheckRotateCenterToLeft() )
							RotateLeft();
						else
							m_eCurPos = m_eDestPos;
					}
					break;
			}
			break;

		case PROCESS_PRESELECT:
			DoProcPreselect();
			break;

		case PROCESS_SELECTED:
			DoSelectedChrProc();
			break;
	}
}

void CGameProcCharacterSelect::RotateLeft()
{
	__Matrix44 mtxrot; mtxrot.Identity();
	__Vector3 vOffs;

	mtxrot.RotationY(CN3Base::s_fSecPerFrm*1.2f);
	vOffs = m_vEye - m_vAt;	
	vOffs *= mtxrot;
	m_vEye = vOffs + m_vAt;
}

void CGameProcCharacterSelect::RotateRight()
{
	__Matrix44 mtxrot; mtxrot.Identity();
	__Vector3 vOffs;

	mtxrot.RotationY(-CN3Base::s_fSecPerFrm*1.2f);
	vOffs = m_vEye - m_vAt;	
	vOffs *= mtxrot;
	m_vEye = vOffs + m_vAt;
}

void CGameProcCharacterSelect::CharacterSelect()
{
	int iIndex;
	switch ( m_eCurPos )
	{
		case POS_CENTER:
			iIndex = 0;
			break;

		case POS_LEFT:
			iIndex = 1;
			break;

		case POS_RIGHT:
			iIndex = 2;
			break;
	}

	if ( m_eCurProcess == PROCESS_PRESELECT )
	{
		if ( m_pChrs[iIndex] )
		{
			m_eCurProcess = PROCESS_SELECTED;
			m_pChrs[iIndex]->AniCurSet(SELECT_ANIM_SELECTED, true, 0.6f);
			m_pChrs[iIndex]->Tick();

			switch ( s_pPlayer->m_InfoBase.eNation )
			{
				case NATION_KARUS:
					m_fCurTheta = KARUS_THETA_MAX;
					break;

				case NATION_ELMORAD:
					m_fCurTheta = ELMORAD_THERA_MAX;
					break;
			}

			m_lgt[iIndex].Theta = m_fCurTheta;	
			s_pEng->s_lpD3DDev->LightEnable(iIndex+4, TRUE);
			s_pEng->s_lpD3DDev->SetLight(iIndex+4, &m_lgt[iIndex]);
		}
	}

	m_bReceivedCharacterSelect = true; // 캐릭터 고르기 완료..
	s_pUIMgr->EnableOperationSet(false); // 일단 고르면 UI 안되게 한다...
}

void CGameProcCharacterSelect::CharacterSelectFailed()
{
	m_bReceivedCharacterSelect = false; // 캐릭터 고르기 실패..
	std::string szErr; ::_LoadStringFromResource(IDS_ERR_CHARACTER_SELECT, szErr);
	CGameProcedure::MessageBoxPost(szErr, "", MB_OK, BEHAVIOR_EXIT);
	s_pUIMgr->EnableOperationSet(true);
}

void CGameProcCharacterSelect::DoSelectedChrProc()
{
	int iIndex;
	switch ( m_eCurPos )
	{
		case POS_CENTER:
			iIndex = 0;
			break;

		case POS_LEFT:
			iIndex = 1;
			break;

		case POS_RIGHT:
			iIndex = 2;
			break;
	}

	// Light..
	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			s_pEng->s_lpD3DDev->LightEnable(iIndex+4, TRUE);
			s_pEng->s_lpD3DDev->SetLight(iIndex+4, &m_lgt[iIndex]);
			break;

		case NATION_ELMORAD:
			s_pEng->s_lpD3DDev->LightEnable(iIndex+4, TRUE);
			s_pEng->s_lpD3DDev->SetLight(iIndex+4, &m_lgt[iIndex]);
			break;
	}

	if ( !m_pChrs[iIndex]->IsAnimEnd() )
	{
		m_pChrs[iIndex]->AniCurSet(SELECT_ANIM_SELECTED, true, 0.6f);
		m_pChrs[iIndex]->Tick();
		m_bFadeOutRender = true;
		FadeOutProcess();
	}
	else
	{
		s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcMain); // 캐릭터 받았고.. 에니메이션도 끝났으면 메인으로 넘어가자..!!
//		CGameProcedure::s_pEng->RestoreLighting();
	}
}

void CGameProcCharacterSelect::FadeOutProcess()
{
	m_fFadeOut += (30.0f/CN3Base::s_fFrmPerSec)*1.5f;

	if ( m_fFadeOut > 255 )
		m_fFadeOut = 255;
}

void CGameProcCharacterSelect::FadeOutRender()
{
    __VertexTransformedColor pVertices[4];

	pVertices[0].Set( 0.0f, 0.0f, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut,0x00,0x00,0x00) );
	pVertices[1].Set( s_CameraData.vp.Width, 0.0f, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut,0x00,0x00,0x00) );
	pVertices[2].Set( s_CameraData.vp.Width, s_CameraData.vp.Height, 0.000002f, 0.99f, 	D3DCOLOR_ARGB((BYTE)m_fFadeOut,0x00,0x00,0x00) );
	pVertices[3].Set( 0.0f, s_CameraData.vp.Height, 0.000002f, 0.99f, D3DCOLOR_ARGB((BYTE)m_fFadeOut,0x00,0x00,0x00) );

	DWORD dwUsefog = TRUE;
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_FOGENABLE , &dwUsefog );

	DWORD dwUseLighting=TRUE;
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_LIGHTING, &dwUseLighting );

	DWORD dwUseColorVertex=FALSE;
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_COLORVERTEX , &dwUseColorVertex );

	unsigned long  bUseAlphaBlend=TRUE;
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_ALPHABLENDENABLE,	&bUseAlphaBlend );

	int	bLight[8];
	for ( int i = 0; i < 8; i++ )	CN3Base::s_lpD3DDev->GetLightEnable(i, &bLight[i]);

	if (bUseAlphaBlend == FALSE) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE,	TRUE );
	if (dwUseLighting) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	if (dwUsefog) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	// set render states
	if (dwUseColorVertex == FALSE) CN3Base::s_lpD3DDev->SetRenderState( D3DRS_COLORVERTEX, TRUE );
	for ( int i = 0; i < 8; i++ )	CN3Base::s_lpD3DDev->LightEnable(i, FALSE);

	DWORD dwTexStageCO, dwTexStageCARG1, dwTexStageAO, dwTexStageAARG1, dwRSSB, dwRSDB;

	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwTexStageCO);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_COLORARG1, &dwTexStageCARG1);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwTexStageAO);
	s_pEng->s_lpD3DDev->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwTexStageAARG1);
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_SRCBLEND, &dwRSSB );
	CN3Base::s_lpD3DDev->GetRenderState( D3DRS_DESTBLEND, &dwRSDB );

	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND,				D3DBLEND_SRCALPHA );
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND,				D3DBLEND_INVSRCALPHA );

	CN3Base::s_lpD3DDev->SetTexture(0, NULL);

	CN3Base::s_lpD3DDev->SetFVF(FVF_TRANSFORMEDCOLOR);
	CN3Base::s_lpD3DDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(__VertexTransformedColor));

	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLOROP, dwTexStageCO);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_COLORARG1, dwTexStageCARG1);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwTexStageAO);
	s_pEng->s_lpD3DDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwTexStageAARG1);
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_SRCBLEND, dwRSSB );
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_DESTBLEND, dwRSDB );

	CN3Base::s_lpD3DDev->SetRenderState(D3DRS_COLORVERTEX, dwUseColorVertex);
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_ALPHABLENDENABLE,		bUseAlphaBlend );
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_LIGHTING, dwUseLighting );
	CN3Base::s_lpD3DDev->SetRenderState( D3DRS_FOGENABLE , dwUsefog );
	for ( int i = 0; i < 8; i++ )	CN3Base::s_lpD3DDev->LightEnable(i, bLight[i]);
}


void CGameProcCharacterSelect::DoProcPreselect()
{
	D3DVIEWPORT9 vp;
	CN3Base::s_lpD3DDev->GetViewport(&vp);
	
	float left, right, top, bottom;
	left = vp.Width * 0.36f;
	right = vp.Width * 0.64f;
	top = vp.Height * 0.44f;
	bottom = vp.Height * 0.86f;

	POINT pt = s_pLocalInput->MouseGetPos(); 

	int	iPosIndex;
	switch( m_eCurPos )
	{
		case POS_CENTER:
			iPosIndex = 0;
			break;

		case POS_LEFT:
			iPosIndex = 1;
			break;

		case POS_RIGHT:
			iPosIndex = 2;
			break;
	}

	if ( (pt.x > left) && (pt.x < right) && (pt.y < bottom) && (pt.y > top) )
	{
		// Doing..
		if ( m_pChrs[iPosIndex] )
		{
			m_pChrs[iPosIndex]->AniCurSet(SELECT_ANIM_PRE_SELECT, true);
			m_pChrs[iPosIndex]->Tick();
		}

		IncreseLightFactor();

		m_pUICharacterSelect->DisplayChrInfo(&m_InfoChrs[iPosIndex]);
	}
	else
	{
		// Doing..
		if ( m_pChrs[iPosIndex] )
		{
			m_pChrs[iPosIndex]->AniCurSet(SELECT_ANIM_DUMMY, true);
			m_pChrs[iPosIndex]->Tick();
		}

		DecreseLightFactor();

		m_pUICharacterSelect->DontDisplayInfo();
	}

	// Light..
	for( int i = 0; i < 3; i++ ) m_lgt[i].Theta   = 0.0f;
	m_lgt[iPosIndex].Theta = m_fCurTheta;	

	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			if ( m_lgt[iPosIndex].Theta != 0.0f ) 
			{
				s_pEng->s_lpD3DDev->LightEnable(iPosIndex+4, TRUE);
				s_pEng->s_lpD3DDev->SetLight(iPosIndex+4, &m_lgt[iPosIndex]);
			}
			else
				s_pEng->s_lpD3DDev->LightEnable(iPosIndex+4, FALSE);
			break;

		case NATION_ELMORAD:
			if ( m_lgt[iPosIndex].Theta != 0.0f ) 
			{
				s_pEng->s_lpD3DDev->LightEnable(iPosIndex+4, TRUE);
				s_pEng->s_lpD3DDev->SetLight(iPosIndex+4, &m_lgt[iPosIndex]);
			}
			else
				s_pEng->s_lpD3DDev->LightEnable(iPosIndex+4, FALSE);
			break;
	}
}

void CGameProcCharacterSelect::IncreseLightFactor()
{
	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			m_fCurTheta += KARUS_INCRESE_OFFSET;
			if ( m_fCurTheta > KARUS_THETA_MAX )
				m_fCurTheta = KARUS_THETA_MAX;
			break;

		case NATION_ELMORAD:
			m_fCurTheta += ELMORAD_INCRESE_OFFSET;
			if ( m_fCurTheta > ELMORAD_THERA_MAX )
				m_fCurTheta = ELMORAD_THERA_MAX;
			break;
	}
}

void CGameProcCharacterSelect::DecreseLightFactor()
{
	switch ( s_pPlayer->m_InfoBase.eNation )
	{
		case NATION_KARUS:
			m_fCurTheta -= KARUS_INCRESE_OFFSET;
			if ( m_fCurTheta < 0.0f )
				m_fCurTheta = 0.0f;
			break;

		case NATION_ELMORAD:
			m_fCurTheta -= ELMORAD_INCRESE_OFFSET;
			if ( m_fCurTheta < 0.0f )
				m_fCurTheta = 0.0f;
			break;
	}
}

void CGameProcCharacterSelect::MsgRecv_AllCharacterInfo(DataPack* pDataPack, int& iOffset)
{
	int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 결과..
	if(0x1 == iResult)
	{
		for(int i = 0; i < MAX_AVAILABLE_CHARACTER; i++)
		{
			int iIDLength = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 캐릭터 아이디 길이 s,

			if(iIDLength == 0) {
				m_InfoChrs[i].szID = "";
				continue;
			}

			CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, m_InfoChrs[i].szID, iIDLength);// 캐릭터 아이디 문자열 str

			m_InfoChrs[i].eRace			= (e_Race)(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset)); // 종족 b
			m_InfoChrs[i].eClass		= (e_Class)(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)); // 직업 b
			m_InfoChrs[i].iLevel		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 레벨 b
			m_InfoChrs[i].iFace			= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 얼굴모양 b
			m_InfoChrs[i].iHair			= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 머리모양 b
			m_InfoChrs[i].iZone			= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // zone number

			m_InfoChrs[i].dwItemHelmet				= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 투구 dw
			m_InfoChrs[i].iItemHelmetDurability		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
			m_InfoChrs[i].dwItemUpper				= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 상체 dw
			m_InfoChrs[i].iItemUpperDurability		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
			m_InfoChrs[i].dwItemCloak				= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 어깨(망토) dw
			m_InfoChrs[i].iItemCloakDurability		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값

			// NOTE(srmeier): this was added for 1298
			DWORD dwRightHand = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
			int iItemRightHandDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			DWORD dwLeftHand = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
			int iItemLeftHandDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

			m_InfoChrs[i].dwItemLower				= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 하체 dw
			m_InfoChrs[i].iItemLowerDurability		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
			m_InfoChrs[i].dwItemGloves				= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 장갑 dw
			m_InfoChrs[i].iItemGlovesDurability		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
			m_InfoChrs[i].dwItemShoes				= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 신발 dw
			m_InfoChrs[i].iItemShoesDurability		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 내구성값
		}

		// 캐릭터 추가..
		if ( m_InfoChrs[0].szID.size() > 0 )		AddChr(POS_CENTER, &(m_InfoChrs[0]));
		if ( m_InfoChrs[1].szID.size() > 0 )		AddChr(POS_LEFT, &(m_InfoChrs[1]));
		if ( m_InfoChrs[2].szID.size() > 0 )		AddChr(POS_RIGHT, &(m_InfoChrs[2]));
	}
	else
	{
		this->MsgSend_RequestAllCharacterInfo(); // 다시 정보 요청..
		return;
	}
}

void CGameProcCharacterSelect::MsgSend_RequestAllCharacterInfo()
{
	BYTE byBuff[4];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, WIZ_ALLCHAR_INFO_REQ);	// 커멘드.
	s_pSocket->Send(byBuff, iOffset);								// 보낸다
}

void CGameProcCharacterSelect::MsgSend_CharacterSelect() // virtual
{
	CGameProcedure::MsgSend_CharacterSelect();
	s_pUIMgr->EnableOperationSet(false); // UI 를 조작 못하게 한다..
}

bool CGameProcCharacterSelect::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	int iOffsetPrev = iOffset;
	if(false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) iOffset = iOffsetPrev;
	else return true;

	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 커멘드 파싱..
	switch ( iCmd )										// 커멘드에 다라서 분기..
	{
		case WIZ_ALLCHAR_INFO_REQ:				// 캐릭터 선택 메시지..
			this->MsgRecv_AllCharacterInfo(pDataPack, iOffset);
			s_pUIMgr->EnableOperationSet(true); // 캐릭터 정보가 다오면 UI 조작하게 한다..
			return true;
		case WIZ_DEL_CHAR:
			this->MsgRecv_DeleteChr(pDataPack, iOffset);
			return true;
	}
	
	return false;
}

void CGameProcCharacterSelect::CharacterSelectOrCreate()
{
	CGameProcedure::MessageBoxClose(-1);

	int iIndex = CGameProcedure::s_iChrSelectIndex;
	if(NULL == m_pChrs[iIndex]) // 캐릭터가 없으면..
	{
		CGameProcedure::ProcActiveSet((CGameProcedure*)s_pProcCharacterCreate); // 캐릭터 생성 프로시저를 호출한다..
	}
	else
	{
		s_pPlayer->m_InfoExt.iZoneInit = 0x01;
		s_pPlayer->m_InfoExt.iZoneCur = m_InfoChrs[iIndex].iZone;
		s_pPlayer->IDSet(-1, m_InfoChrs[iIndex].szID, 0xffffffff);
		this->MsgSend_CharacterSelect();
	}
}
