// GameProcMain.cpp: implementation of the CGameProcMain class.
//
//////////////////////////////////////////////////////////////////////
#include <io.h>

#include "stdafx.h"
#include "Resource.h"

#include "GameEng.h"
#include "GameProcMain.h"
#include "LocalInput.h"

#include "APISocket.h"
#include "Compress.h"
#include "PacketDef.h"

#include "PlayerMySelf.h"
#include "PlayerOtherMgr.h"
#include "ServerMesh.h"
#include "N3FXMgr.h"

#include "UIManager.h"
#include "UILoading.h"
#include "UIChat.h"
#include "UIInventory.h"
#include "UICmd.h"
#include "UIVarious.h"
#include "UIStateBar.h"
#include "UITargetBar.h"
#include "UITransactionDlg.h"
#include "UIHelp.h"
#include "UIMessageWnd.h"
#include "UINotice.h"
#include "UIDroppedItemDlg.h"
#include "UIPerTradeDlg.h"
#include "UIPartyOrForce.h"
#include "UISkillTreeDlg.h"
#include "UIHotKeyDlg.h"
#include "UIClassChange.h"
#include "UINpcEvent.h"
#include "UIItemExchange.h"
#include "UIRepairTooltipDlg.h"
#include "UINpcTalk.h"
#include "UIKnightsOperation.h"
#include "UIPartyBBS.h"
#include "UIWareHouseDlg.h"
#include "UINPCChangeEvent.h"
#include "UIWarp.h"
#include "UIInn.h"
#include "UICreateClanName.h"
#include "UIPartyBBS.h"
#include "UITradeSellBBS.h"
#include "UITradeBBSSelector.h"
#include "UITradeBBSEditDlg.h"
#include "UIQuestMenu.h"
#include "UIQuestTalk.h"
#include "UIDead.h"

#include "SubProcPerTrade.h"
#include "CountableItemEditDlg.h"
#include "MagicSkillMng.h"
#include "WarMessage.h"
#include "GameCursor.h"

#include "N3WorldManager.h"

#include "LightMgr.h"

#include "N3SkyMng.h"
#include "N3ShapeExtra.h"
#include "N3Camera.h"
#include "N3SndObj.h"
#include "N3SndObjStream.h"
#include "N3SndMgr.h"
#include "N3TableBase.h"

#include "lzf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum e_ChatCmd { 	CMD_WHISPER, CMD_TOWN, CMD_TRADE, CMD_EXIT, CMD_PARTY,
					CMD_LEAVEPARTY, CMD_RECRUITPARTY, CMD_JOINCLAN, CMD_WITHDRAWCLAN, CMD_FIRECLAN, 
					CMD_APPOINTVICECHIEF, CMD_GREETING, CMD_EXCITE, CMD_VISIBLE, CMD_INVISIBLE, 
					CMD_CLEAN, CMD_RAINING, CMD_SNOWING, CMD_TIME, CMD_CU_COUNT,
					CMD_NOTICE, CMD_ARREST, CMD_FORBIDCONNECT, CMD_FORBIDCHAT, CMD_PERMITCHAT,
					CMD_GAME_SAVE, 
					CMD_COUNT,
					CMD_UNKNOWN = 0xffffffff };
static std::string s_szCmdMsg[CMD_COUNT]; // 게임상 명령어



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcMain::CGameProcMain()				// r기본 생성자.. 각 변수의 역활은 헤더 참조..
{	
	m_fLBClickTime = 0.0f;
	m_bLoadComplete	= FALSE;
	m_fRequestGameSave = 300.0f;

	//sound obj...
	m_pSnd_Town = NULL;
	m_pSnd_Battle = NULL;

	m_iJoinReqClan = 0;
	m_iJoinReqClanRequierID = 0;

	//UI
	m_pUIMsgDlg = new CUIMessageWnd;
	m_pUIChatDlg = new CUIChat();
	m_pUIStateBarAndMiniMap = new CUIStateBar();
	m_pUIVar = new CUIVarious();
	m_pUICmd = new CUICmd();
	m_pUITargetBar = new CUITargetBar();
	m_pUIHelp = new CUIHelp();
	m_pUINotice = new CUINotice();
	m_pUIClassChange = new CUIClassChange();
	m_pUINpcEvent = new CUINPCEvent();
	m_pUIItemREDlg = new CUIItemExchange();
	m_pUIRepairTooltip = new CUIRepairTooltipDlg();
	m_pUIDroppedItemDlg = new CUIDroppedItemDlg();
	m_pUITransactionDlg = new CUITransactionDlg();
	m_pUIInventory = new CUIInventory();
	m_pUIPartyOrForce = new CUIPartyOrForce();
	m_pUISkillTreeDlg = new CUISkillTreeDlg();
	m_pUIHotKeyDlg = new CUIHotKeyDlg();
	m_pUINpcTalk = new CUINpcTalk();
	m_pUIKnightsOp = new CUIKnightsOperation();			// 기사단 리스트 보기, 가입, 등...
	m_pUIPartyBBS = new CUIPartyBBS(); // 파티 지원 시스템 게시판??..
	m_pUIWareHouseDlg = new CUIWareHouseDlg();
	m_pUINpcChange = new CUINPCChangeEvent();	
	m_pUIWarp = new CUIWarp();
	m_pUIInn = new CUIInn();
	m_pUICreateClanName = new CUICreateClanName();
	m_pUITradeBBS = new CUITradeSellBBS();
	m_pUITradeBBSSelector = new CUITradeBBSSelector();	
	m_pUITradeBBSEdit = new CUITradeBBSEditDlg();
	m_pUIQuestMenu = new CUIQuestMenu();
	m_pUIQuestTalk = new CUIQuestTalk();
	m_pUIDead = new CUIDead();

	m_pSubProcPerTrade = new CSubProcPerTrade();
	m_pMagicSkillMng = new CMagicSkillMng(this);
	m_pTargetSymbol = new CN3Shape(); // 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..
	m_pWarMessage = new CWarMessage;

	m_pLightMgr = new CLightMgr;
}

CGameProcMain::~CGameProcMain()
{
	this->Release();

	//UI
	delete m_pUIMsgDlg;
	delete m_pUIChatDlg;
	delete m_pUIStateBarAndMiniMap;
	delete m_pUIVar;
	delete m_pUICmd;
	delete m_pUITargetBar;
	delete m_pUIHelp;
	delete m_pUINotice;
	delete m_pUIClassChange;
	delete m_pUINpcEvent;
	delete m_pUIItemREDlg;
	delete m_pUIRepairTooltip;
	delete m_pUIDroppedItemDlg;
	delete m_pUITransactionDlg;
	delete m_pUIInventory;
	delete m_pUIPartyOrForce;
	delete m_pUISkillTreeDlg;
	delete m_pUIHotKeyDlg;
	delete m_pUINpcTalk;
	delete m_pUIKnightsOp;
	delete m_pUIPartyBBS;
	delete m_pUIWareHouseDlg;
	delete m_pUINpcChange;
	delete m_pUIWarp;
	delete m_pUIInn;
	delete m_pUICreateClanName;
	delete m_pUITradeBBS;
	delete m_pUITradeBBSSelector;
	delete m_pUITradeBBSEdit;
	delete m_pUIQuestMenu;
	delete m_pUIQuestTalk;
	delete m_pUIDead;

	delete m_pSubProcPerTrade;
	delete m_pMagicSkillMng;
	delete m_pWarMessage;
	delete m_pTargetSymbol; // 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..

	delete m_pLightMgr;
}

void CGameProcMain::Release()
{
	this->ReleaseSound();
	this->ReleaseUIs();

	m_pSubProcPerTrade->Release();
	m_pMagicSkillMng->Release();
	m_pWarMessage->Release();
	m_pTargetSymbol->Release();
	m_pLightMgr->Release();

	CGameProcedure::Release();
}

void CGameProcMain::ReleaseUIs()
{
	m_pUIChatDlg->Release();
	m_pUIMsgDlg->Release();
	m_pUICmd->Release();
	m_pUIVar->Release();
	m_pUIStateBarAndMiniMap->Release();
	m_pUITargetBar->Release();
	m_pUIHelp->Release();
	m_pUINotice->Release();
	m_pUIClassChange->Release();
	m_pUINpcEvent->Release();
	m_pUIItemREDlg->Release();
	m_pUIRepairTooltip->Release();
	m_pUIPartyOrForce->Release();
	m_pUISkillTreeDlg->Release();
	m_pUIHotKeyDlg->Release();
	m_pUINpcTalk->Release();
//	m_pUITradeList->Release();
	m_pUIKnightsOp->Release();			// 기사단 리스트 보기, 가입, 등...
	m_pUIPartyBBS->Release();
	m_pUIWareHouseDlg->Release();
	m_pUINpcChange->Release();
	m_pUIWarp->Release();
	m_pUIInn->Release();
	m_pUICreateClanName->Release();

	CN3UIBase::DestroyTooltip();
}

void CGameProcMain::Init()
{
	CGameProcedure::Init();
	m_pLightMgr->Release();
	s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

	// NOTE: set the chat commands
	/*
	enum e_ChatCmd {
		CMD_WHISPER, CMD_TOWN, CMD_TRADE, CMD_EXIT, CMD_PARTY,
		CMD_LEAVEPARTY, CMD_RECRUITPARTY, CMD_JOINCLAN, CMD_WITHDRAWCLAN, CMD_FIRECLAN,
		CMD_APPOINTVICECHIEF, CMD_GREETING, CMD_EXCITE, CMD_VISIBLE, CMD_INVISIBLE,
		CMD_CLEAN, CMD_RAINING, CMD_SNOWING, CMD_TIME, CMD_CU_COUNT,
		CMD_NOTICE, CMD_ARREST, CMD_FORBIDCONNECT, CMD_FORBIDCHAT, CMD_PERMITCHAT,
		CMD_GAME_SAVE,
		CMD_COUNT,
		CMD_UNKNOWN = 0xffffffff
	};
	*/
	std::string szTemp[CMD_COUNT] = {
		"whisper", "town",
		"trade", "exit", "party", "leaveparty", "recruitparty", "joinclan", "withdrawclan", "fireclan",
		"appointvicechief", "greeting", "excite", "visible", "invisible", "clean", "raining", "snowing",
		"time", "count", "notice", "arrest", "forbidconnect", "forbidchat", "permitchat", "gamesave",
	};

	for( int i = IDS_CMD_WHISPER ; i <= IDS_CMD_GAME_SAVE ; i++ ) //명령어 로딩...
	{
		s_szCmdMsg[i - IDS_CMD_WHISPER] = szTemp[i - IDS_CMD_WHISPER];
		//::_LoadStringFromResource(i, s_szCmdMsg[i - IDS_CMD_WHISPER]);
	}

	s_SndMgr.ReleaseStreamObj(&(CGameProcedure::s_pSnd_BGM));

	if(m_pWarMessage) m_pWarMessage->InitFont();
	this->InitUI(); // 국가에 따라 다른 UI 로딩...
	this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // 존 로딩..

	//sound obj...
	if(m_pSnd_Battle==NULL)
	{
		int iIDSndBattle = ((NATION_KARUS == s_pPlayer->m_InfoBase.eNation) ? ID_SOUND_BGM_KA_BATTLE : ID_SOUND_BGM_EL_BATTLE);
		m_pSnd_Battle = s_pEng->s_SndMgr.CreateStreamObj(iIDSndBattle);	// 전투음악 ID
		if(m_pSnd_Battle) 
		{
			m_pSnd_Battle->Looping(true);
			m_pSnd_Battle->Stop();
		}
	}
	if(m_pSnd_Town==NULL)
	{
		m_pSnd_Town = s_pEng->s_SndMgr.CreateStreamObj(ID_SOUND_BGM_TOWN);	// 마을음악 ID
		if(m_pSnd_Town)
		{
			m_pSnd_Town->Looping(true);
			m_pSnd_Town->Play(NULL, 3.0f);
		}
	}

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data...", 0);

	// 경로 기억..
	char szPathOld[_MAX_PATH], szPathFind[_MAX_PATH];
	::GetCurrentDirectory(_MAX_PATH, szPathOld);

	_finddata_t fi;
	long hFind = -1;

	// 리소스 다 읽기..
	// 에니메이션 다 읽기..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Chr");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3Anim", &fi);
	if(hFind)
	{
		std::string szFN = "Chr\\";
		szFN += fi.name;
		CN3AnimControl* pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Chr\\";
			szFN += fi.name;
			pObjTmp = s_MngAniCtrl.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 10 %", 10);

	// 리소스 다 읽기..
	// 텍스처 다 읽기..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.dxt", &fi);
	if(hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3Texture* pObjTmp = s_MngTex.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngTex.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 25 %", 25);
	
	// 리소스 다 읽기..
	// 조인트 다 읽기..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Chr");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3Joint", &fi);
	if(hFind)
	{
		std::string szFN = "Chr\\";
		szFN += fi.name;
		CN3Joint* pObjTmp = s_MngJoint.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Chr\\";
			szFN += fi.name;
			pObjTmp = s_MngJoint.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 50 %", 50);

	// 리소스 다 읽기..
	// 스킨 읽기..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3CSkins", &fi);
	if(hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3CPartSkins* pObjTmp = s_MngSkins.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngSkins.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 75 %", 75);
	
	// 리소스 다 읽기..
	// PMesh 읽기..
	lstrcpy(szPathFind, szPathOld);
	lstrcat(szPathFind, "\\Item");
	::SetCurrentDirectory(szPathFind);
	hFind = _findfirst("*.N3PMesh", &fi);
	if(hFind)
	{
		std::string szFN = "Item\\";
		szFN += fi.name;
		CN3PMesh* pObjTmp = s_MngPMesh.Get(szFN.c_str());
		while(_findnext(hFind, &fi) != -1)
		{
			szFN = "Item\\";
			szFN += fi.name;
			pObjTmp = s_MngPMesh.Get(szFN.c_str());
		}
	}
	_findclose(hFind);

	if(s_pUILoading) s_pUILoading->Render("Loading Character Data... 100 %", 100);

	this->MsgSend_GameStart();

	//int offset = 0;
	//DataPack tempData = {};
	//this->MsgRecv_MyInfo_All(&tempData, offset);

	// 경로 돌리기..
	::SetCurrentDirectory(szPathOld);

}

void CGameProcMain::InitPlayerPosition(const __Vector3& vPos) // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
{
	__Vector3 vPosFinal = vPos;
	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(vPos.x, vPos.z);	// 지형의 높이값 얻기..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(vPos, 1.0f); // 오브젝트에서 가장 가까운 높이값 얻기..
	if (!s_pWorldMgr->IsIndoor())
	{
		if (T_Abs(vPos.y - fYObject) < T_Abs(vPos.y - fYTerrain)) vPosFinal.y = fYObject; // 좀더 가까운 곳에 놓는다..
		else vPosFinal.y = fYTerrain;
	}
	else
	{
		if (fYObject > fYTerrain)
			vPosFinal.y = fYObject;
		else
			vPosFinal.y = fYTerrain;
	}

	s_pPlayer->PositionSet(vPosFinal, true);	// 캐릭터 위치 셋팅..	
	s_pPlayer->m_vPosFromServer = vPos;
	m_vPlayerPosSended = vPos;					// 최근에 보낸 위치 세팅..
	m_fMsgSendTimeMove = 0;						// 시간을 기록한다..

	this->CommandSitDown(false, false, true); // 일으켜 세운다.. 앉아있는 상태에서 워프하면.. 버그가 있다..
	this->TargetSelect(-1, false); // 타겟 해제..
	this->UpdateCameraAndLight(); // 카메라와 라이트 다시 계산..

	s_pPlayer->Action(PSA_BASIC, true, NULL, true); // 강제로 기본 자세..
}

void CGameProcMain::Tick()
{
	CGameProcedure::Tick();	// 키, 마우스 입력 등등..

	if ( FALSE == m_bLoadComplete ) return;				// 로딩이 안되었으면.. 돌아간다.
	if(!s_pSocket->IsConnected()) return;

#ifdef _DEBUG
	if(s_pLocalInput->IsKeyPressed(SDL_SCANCODE_F8))
	{
		BYTE byBuff[32];
		int iOffset = 0;
		CAPISocket::MP_AddByte(byBuff, iOffset, N3_TEMP_TEST);
		s_pSocket->Send(byBuff, iOffset);


		int iNPC = s_pOPMgr->m_NPCs.size();
		char szBuff[1024];
		sprintf(szBuff, "NPC Region Test : %d", iNPC);
		CLogWriter::Write(szBuff);
		it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
		for(; it != itEnd; it++)
		{
			CPlayerNPC* pNPC = it->second;

			sprintf(szBuff, "    ID(%d) Name(%s) Pos(%.1f, %.1f)",
				pNPC->IDNumber(),
				pNPC->IDString().c_str(), 
				pNPC->m_vPosFromServer.x,
				pNPC->m_vPosFromServer.z );
			CLogWriter::Write(szBuff);
		}

	}
#endif

	DWORD dwMouseFlags = s_pLocalInput->MouseGetFlag();	// 마우스 버튼 플래그 - LocalInput.h 참조
	this->ProcessLocalInput(dwMouseFlags);					// 키보드나 마우스 입력은 UI 다음에 처리...

	MsgSend_Continous();									// 일정 시간마다 움직임과 회전값, 공격등을 체크해서 패킷 만들어 보냄..

	s_pPlayer->Tick();									// 플레이어 틱(갱신)
	s_pWorldMgr->Tick();
	s_pOPMgr->Tick(s_pPlayer->Position());				// 다른 유저 관리자 틱(갱신)
//	s_pFX->Tick(); //내부에서 카메라 값을 쓸 경우 위치가 오차가 생겨 Render()함수 안으로 옮김...

	__Vector3 ListenerPos = s_pPlayer->Position();
	__Vector3 ListenerDir = s_pPlayer->Direction();
	__Vector3 ListenerUp(0,1,0);

	// Sound Tick...
	CN3SndObj::SetListenerPos(&ListenerPos);
	CN3SndObj::SetListenerOrientation(&ListenerDir, &ListenerUp);

	this->UpdateUI_MiniMap(); // 미니맵 업데이트..
	this->UpdateUI_TargetBar(); // 타겟바 처리..
	this->UpdateBGM(); // 배경음악을 상황에 따라 처리..
	this->UpdateCameraAndLight(); // 카메라와 라이트 처리..
	
//	ProcessPlayerInclination();							// 경사 처리..(가만히 있어도 경사가 급하면 미끄러짐..).
#ifdef _N3_64GRID_
	m_SMesh.Tick(s_pPlayer, &m_Terrain);				// 서버 메시 틱.(갱신)
#endif

	m_pUIStateBarAndMiniMap->UpdatePosition(s_pPlayer->Position(), s_pPlayer->Yaw()); // 위치 업데이트.

	if(m_pMagicSkillMng) m_pMagicSkillMng->Tick();
	if(m_pWarMessage) m_pWarMessage->Tick();
	if(m_pLightMgr) m_pLightMgr->Tick();
	
	////////////////////////////////////////////////////////////////////////////////////
	// 아무 패킷도 안보냈으면 2초에 한번 N3_TIME_NOTIFY 보낸다..
	float fTime = CN3Base::TimeGet();
	static float fTimePrev = fTime;
	
	static float fTimeInterval1 = 0;
	if(0 == s_pSocket->m_iSendByteCount)
	{
		fTimeInterval1 += fTime - fTimePrev;
		if(fTimeInterval1 >= 2.0f)
		{
			BYTE byCmd = N3_TIME_NOTIFY;
			s_pSocket->Send(&byCmd, 1);
			s_pSocket->m_iSendByteCount = 0;
			fTimeInterval1 = 0;
		}
	}
	else
	{
		s_pSocket->m_iSendByteCount = 0;
		fTimeInterval1 = 0;
	}
	// 아무 패킷도 안보냈으면 2초에 한번 N3_TIME_NOTIFY 보낸다..
	////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////
	// 타이머 비슷한 루틴..
	static float fInterval2 = 0, fInterval3 = 0, fInterval4 = 0, fInterval5 = 0;
	fInterval2 += fTime - fTimePrev;
	fInterval3 += fTime - fTimePrev;
	fInterval4 += fTime - fTimePrev;
	fInterval5 += fTime - fTimePrev;
	m_fRequestGameSave += fTime - fTimePrev;
	if(fInterval2 > 1200.0f) // 저장 요청..
	{
		BYTE byBuff[4];												// 버퍼.. 
		int iOffset=0;												// 옵셋..
		s_pSocket->MP_AddByte(byBuff, iOffset, N3_REQUEST_GAME_SAVE);	// 저장 요청 커멘드..
		s_pSocket->Send(byBuff, iOffset);				// 보냄..

		fInterval2 = 0.0f;
	}
	if(fInterval3 > 10.0f) // 스피드핵 체크.. 
	{
		MsgSend_SpeedCheck();										// 스피드핵 체크 하기

		fInterval3 = 0.0f;
	}
	if(s_pPlayer->m_InfoBase.iLevel < 12 && fInterval4 > 20.0f) // 시간이 지나면 팁 하나씩 표시..
	{
		std::string szMsg;
		szMsg = "IDS_HELP_TIP_ALL";//::_LoadStringFromResource(IDS_HELP_TIP_ALL, szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00); // 헬프 표시..
		szMsg = "IDS_HELP_TIP1";//::_LoadStringFromResource(IDS_HELP_TIP1 + rand()%30, szMsg);
		this->m_pUIMsgDlg->AddMsg(szMsg, 0xffffff00); // 헬프 표시..
		fInterval4 = 0;
	}
	if(fInterval5 > 5.0f) // 시간이 지나면 팁 하나씩 표시..
	{
//		m_pUIChatDlg->ChangeChattingMode(N3_CHAT_CONTINUE); // 채팅모드 강제로 바꾸기...
		m_pUIChatDlg->ShowContinueMsg();
		fInterval5 = 0;
	}
	fTimePrev = fTime;
	// 타이머 비슷한 루틴..
	////////////////////////////////////////////////////////////////////////////////////
}

void CGameProcMain::Render()
{
	if ( FALSE == m_bLoadComplete )	return; 		// 로딩이 끝났냐??

	D3DCOLOR crSky = ACT_WORLD->GetSkyColorWithSky();
	s_pEng->Clear(crSky); // 안개 색깔을 넣어서 클리어.. -> 하늘색깔로 클리어 해야 하늘이 제대로 나온다..
	s_pEng->s_lpD3DDev->BeginScene();			// 씬 렌더 ㅅ작...
	
	ACT_WORLD->RenderSky();								// 하늘 렌더링..
	float fSunAngle = ACT_WORLD->GetSunAngleByRadinWithSky(); // 해의 각도를 가져오고..

	DWORD dwFilter = D3DTEXF_LINEAR;
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(0, D3DSAMP_MIPFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MINFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MAGFILTER, dwFilter);
	CN3Base::s_lpD3DDev->SetSamplerState(1, D3DSAMP_MIPFILTER, dwFilter);

	ACT_WORLD->RenderTerrain();						// 지형 렌더..
	ACT_WORLD->RenderShape();						// 물체 렌더..
	s_pOPMgr->Render(fSunAngle);				// 다른 플레이어 렌더..
	s_pPlayer->Render(fSunAngle);			// 플레이어 렌더..

	// NOTE(srmeier): uncomment to render the collision meshes
	//ACT_WORLD->RenderCollisionWithShape(s_pPlayer->Position());				// 충돌 메쉬 렌더..

#ifdef _N3_64GRID_
	m_SMesh.Render();							// 서버 메쉬 렌더..
#endif

	this->RenderTarget();						// 타겟으로 잡은 캐릭터 혹은 오브젝트 렌더링..

	ACT_WORLD->RenderGrass();						//	풀 렌더 (asm)
	s_pFX->Tick();
	s_pFX->Render();
	ACT_WORLD->RenderBirdMgr();

	CN3Base::s_AlphaMgr.Render(); // 알파 정렬된 폴리곤들 렌더링..
	
	ACT_WORLD->RenderSkyWeather();							// 하늘 렌더링..
	
	CGameProcedure::Render(); // UI 나 그밖의 기본적인 것들 렌더링..
	if(m_pWarMessage) m_pWarMessage->RenderMessage();
	if(s_pGameCursor) s_pGameCursor->Render();

	s_pEng->s_lpD3DDev->EndScene();
	s_pEng->Present(CN3Base::s_hWndBase);
}

void CGameProcMain::RenderTarget()
{
	if(NULL == m_pTargetSymbol) return;

	// 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..
	CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);//시체로 판정되기 전까지의 캐릭은 포커스를 준다.
	if(NULL == pTarget && NULL == s_pPlayer->m_pObjectTarget) return;

	float fScale = 1;
	float fYScale = 1;
	__Vector3 vPos(0,0,0);

	if(pTarget)
	{
		fYScale = pTarget->Height() * 1.3f;
		fScale = pTarget->Radius() * 2.0f;
		vPos = pTarget->Position();
	}
	else 
	{
		__Vector3 vTmp = s_pPlayer->m_pObjectTarget->Max() - s_pPlayer->m_pObjectTarget->Min();
		fYScale = vTmp.y;
		vTmp.y = 0;
		fScale = vTmp.Magnitude();
		vPos = s_pPlayer->m_pObjectTarget->Pos();
	}

	m_pTargetSymbol->ScaleSet(fScale, fYScale, fScale);
	m_pTargetSymbol->PosSet(vPos);
	m_pTargetSymbol->Tick();
	if(m_pTargetSymbol->Part(1)) // 바닥의 심벌을 땅바닥 위로 맞춘다..
	{
		CN3PMesh* pPMesh = m_pTargetSymbol->Part(1)->Mesh();
		if(pPMesh && pPMesh->GetMaxNumVertices() == 4)
		{
			__VertexT1* pVs = m_pTargetSymbol->Part(1)->Mesh()->GetVertices();
			for(int i = 0; i < 4; i++)
			{
				pVs[i].y =	ACT_WORLD->GetHeightWithTerrain(vPos.x + (pVs[i].x * fYScale), vPos.z + (pVs[i].z * fYScale));
				pVs[i].y -= vPos.y;
				pVs[i].y /= fYScale;
				pVs[i].y += 0.1f / fYScale;
			}
		}
	}
	m_pTargetSymbol->Render();

	// NOTE(srmeier): uncomment to render the collision meshes
	//if(pTarget) pTarget->RenderCollisionMesh();
	//if(s_pPlayer->m_pObjectTarget) s_pPlayer->m_pObjectTarget->RenderCollisionMesh();
}

bool CGameProcMain::ProcessPacket(DataPack* pDataPack, int& iOffset)
{
	int iOffsetPrev = iOffset;
	if(false == CGameProcedure::ProcessPacket(pDataPack, iOffset)) iOffset = iOffsetPrev;
	else return true;

	int iCmd = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// 커멘드 파싱..

	switch ( iCmd )										// 커멘드에 다라서 분기..
	{



#ifdef _DEBUG
	case N3_TEMP_TEST:
		{
			int iNPC = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			char szBuff[32];
			sprintf(szBuff, "NPC Region Test : %d -> ", iNPC);
			std::string szLog = szBuff;
			for(int i = 0; i < iNPC; i++)
			{
				int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				sprintf(szBuff, "%d, ", iID);
				szLog += szBuff;
			}
			CLogWriter::Write(szLog.c_str());
		}
		return true;
#endif
		case N3_ZONEABILITY: {
			// NOTE(srmeier): this is a custom packet used to set terrain zoneability
			Uint8 opcode = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

			switch (opcode) {
				case 0x03://0x01:
					Uint16 zoneFlags = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
					ZoneAbilityType zoneType = (ZoneAbilityType)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
					Uint8 zoneTariff = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
					Uint8 minLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
					Uint8 maxLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

					ACT_WORLD->m_zoneFlags  = zoneFlags;
					ACT_WORLD->m_zoneType   = zoneType;
					ACT_WORLD->m_byTariff   = zoneTariff;
					ACT_WORLD->m_byMinLevel = minLevel;
					ACT_WORLD->m_byMaxLevel = maxLevel;
				break;
			}
		} return true;

		case N3_DEBUG_STRING_TEST: {
			// NOTE(srmeier): testing this debug string functionality

			int iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

			std::string szDebugString;
			CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szDebugString, iLen);

			MsgOutput("DEBUG: "+szDebugString, D3DCOLOR_ARGB(255, 255, 255, 0));

		} return true;

		case N3_GAMESTART: {
			// NOTE(srmeier): send for the second half of the gamestart process

			BYTE byBuff[32];
			int iOffset = 0;

			CAPISocket::MP_AddByte(byBuff, iOffset, N3_GAMESTART);
			CAPISocket::MP_AddByte(byBuff, iOffset, 0x02);

			s_pSocket->Send(byBuff, iOffset);

		} return true;

		case N3_MYINFO:									// 나의 정보 메시지..
			this->MsgRecv_MyInfo_All(pDataPack, iOffset);
			return true;
		case N3_HP_CHANGE:
			this->MsgRecv_MyInfo_HP(pDataPack, iOffset);
			return true;
		case N3_MSP_CHANGE:
			this->MsgRecv_MyInfo_MSP(pDataPack, iOffset);
			return true;
		case N3_EXP_CHANGE:
			this->MsgRecv_MyInfo_EXP(pDataPack, iOffset);
			return true;
		case N3_REALM_POINT_CHANGE: // 국가 기여도..
			this->MsgRecv_MyInfo_RealmPoint(pDataPack, iOffset);
			return true;
		case N3_LEVEL_CHANGE:
			this->MsgRecv_MyInfo_LevelChange(pDataPack, iOffset);
			return true;
		case N3_POINT_CHANGE:
			this->MsgRecv_MyInfo_PointChange(pDataPack, iOffset);
			return true;
		case N3_CHAT:														// 채팅 메시지..	
			this->MsgRecv_Chat(pDataPack, iOffset);
			return true;
		case N3_WARP:
			{
				float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
				float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;

				float fY = ACT_WORLD->GetHeightWithTerrain(fX, fZ, true);
				float fYObject = ACT_WORLD->GetHeightWithShape(fX, fZ);
				if(fYObject > fY) fY = fYObject;

				this->InitPlayerPosition(__Vector3(fX, fY, fZ)); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
			}
			return true;
		case N3_MOVE:
			this->MsgRecv_UserMove(pDataPack, iOffset);
			return true;
		case N3_ROTATE:												// 회전 커멘드..
			this->MsgRecv_Rotation(pDataPack, iOffset);
			return true;
		case N3_REGENE:
			{
//				if(m_pUIDead) m_pUIDead->MsgRecv_Revival(pDataPack, iOffset);
				this->MsgRecv_Regen(pDataPack, iOffset);
				std::string szMsg = "IDS_REGENERATION"; //::_LoadStringFromResource(IDS_REGENERATION, szMsg);
				MessageBoxClose(szMsg);
				m_pUITargetBar->SetVisible(false);
			}
			return true;
		case N3_DEAD:
			this->MsgRecv_Dead(pDataPack, iOffset);
			return true;
		case N3_TIME:
			this->MsgRecv_Time(pDataPack, iOffset);
			return true;
		case N3_WEATHER:
			this->MsgRecv_Weather(pDataPack, iOffset);
			return true;
		case N3_USER_INOUT:												// 다른 유저 인/아웃..
			this->MsgRecv_UserInOut(pDataPack, iOffset);
			return true;
		case N3_UPDATE_REGION_UPC:										// 첨에 로그온하면 그 주변 지역의 캐릭터들 업데이트...
			this->MsgRecv_UserInAndRequest(pDataPack, iOffset);
			return true;
		case N3_REQUEST_USER_IN:										// 서버에 요청한 UserIn 에 대한 자세한 정보 받기..
			this->MsgRecv_UserInRequested(pDataPack, iOffset);						// 
			return true;
		case N3_UPDATE_REGION_NPC:										// 첨에 로그온하면 그 주변 지역의 캐릭터들 업데이트...
			this->MsgRecv_NPCInAndRequest(pDataPack, iOffset);
			return true;
		case N3_REQUEST_NPC_IN:											// 서버에 요청한 UserIn 에 대한 자세한 정보 받기..
			this->MsgRecv_NPCInRequested(pDataPack, iOffset);						// 
			return true;
		case N3_NPC_INOUT:												// NPC 인/아웃..
			this->MsgRecv_NPCInOut(pDataPack, iOffset);
			return true;
		case N3_ATTACK:
			this->MsgRecv_Attack(pDataPack, iOffset);
			return true;
		case N3_NPC_MOVE:												// NPC 움직임 패킷..
			this->MsgRecv_NPCMove(pDataPack, iOffset);
			return true;
		case N3_TARGET_HP:
			this->MsgRecv_TargetHP(pDataPack, iOffset);
			return true;
		case N3_ITEM_MOVE:
			this->MsgRecv_ItemMove(pDataPack, iOffset);				// Item Move에 대한 응답..
			return true;
		case N3_ITEM_BUNDLE_DROP:
			this->MsgRecv_ItemBundleDrop(pDataPack, iOffset);
			return true;
		case N3_ITEM_BUNDLE_OPEN_REQUEST:
			this->MsgRecv_ItemBundleOpen(pDataPack, iOffset);
			return true;
		case N3_ITEM_TRADE_START:
			this->MsgRecv_ItemTradeStart(pDataPack, iOffset);
			return true;
		case N3_ITEM_TRADE:
			this->MsgRecv_ItemTradeResult(pDataPack, iOffset);
			return true;
		case N3_ITEM_DROPPED_GET:
			this->MsgRecv_ItemDroppedGetResult(pDataPack, iOffset);					// 땅에 떨어진 아이템 먹기 결과..
			return true;
		case N3_ITEM_TRADE_REPAIR:
			this->MsgRecv_NpcEvent(pDataPack, iOffset);
			return true;
		case N3_ITEM_REPAIR_REQUEST:
			this->MsgRecv_ItemRepair(pDataPack, iOffset);
			return true;
		case N3_ITEM_COUNT_CHANGE:
			this->MsgRecv_ItemCountChange(pDataPack, iOffset);
			return true;
		case N3_ITEM_DESTROY:
			this->MsgRecv_ItemDestroy(pDataPack, iOffset);
			return true;
		case N3_ITEM_WEIGHT_CHANGE:
			this->MsgRecv_ItemWeightChange(pDataPack, iOffset);
			return true;
		case N3_USER_LOOK_CHANGE:
			this->MsgRecv_UserLookChange(pDataPack, iOffset);
			return true;
		case N3_ZONE_CHANGE:
			this->MsgRecv_ZoneChange(pDataPack, iOffset);
			return true;
		case N3_STATE_CHANGE:
			this->MsgRecv_UserState(pDataPack, iOffset);
			return true;
		case N3_NOTICE:
			this->MsgRecv_Notice(pDataPack, iOffset);
			return true;
		case N3_PARTY_OR_FORCE:
			this->MsgRecv_PartyOrForce(pDataPack, iOffset);
			return true;
		case N3_PER_TRADE:
			this->MsgRecv_PerTrade(pDataPack, iOffset);
			return true;
		case N3_SKILL_CHANGE:
			this->MsgRecv_SkillChange(pDataPack, iOffset);
			return true;
		case N3_MAGIC:
			this->MsgRecv_MagicProcess(pDataPack, iOffset);
			return true;
		case N3_CLASS_CHANGE:
			this->MsgRecv_NpcChangeOpen(pDataPack, iOffset);
			return true;
		case N3_OBJECT_EVENT:
			this->MsgRecv_ObjectEvent(pDataPack, iOffset);
			return true;
		case N3_CHAT_SELECT_TARGET:
			{
				Uint8 type = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
				int err = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

				std::string szID, szMsg;
				int iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// ID 문자열 길이..
				CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);	// ID 문자열..

				e_ChatMode eCM = N3_CHAT_UNKNOWN;
				if(szID.empty())
				{
					szMsg = "IDS_CHAT_SELECT_TARGET_FAIL";
					//::_LoadStringFromResource(IDS_CHAT_SELECT_TARGET_FAIL, szMsg);
					eCM = N3_CHAT_NORMAL;
				}
				else
				{
					szMsg = "IDS_CHAT_SELECT_TARGET_SUCCESS";
					//::_LoadStringFromResource(IDS_CHAT_SELECT_TARGET_SUCCESS, szMsg);
					eCM = N3_CHAT_PRIVATE;
				}
				
				this->MsgOutput(szID + " " + szMsg, 0xffffff00);
				m_pUIChatDlg->ChangeChattingMode(eCM); // 자동으로 귓속말 모드로 바꾸어 준다..
			}
			return true;
		case N3_CONCURRENT_USER_COUNT: // 동시 접속자수 ...
			{
				int iUserCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// ID 문자열 길이..

				std::string szFmt = "IDS_FMT_CONCURRENT_USER_COUNT (%d)";
				//::_LoadStringFromResource(IDS_FMT_CONCURRENT_USER_COUNT, szFmt);
				char szBuff[128] = "";
				sprintf(szBuff, szFmt.c_str(), iUserCount);
				this->MsgOutput(szBuff, D3DCOLOR_ARGB(255,255,255,0));
			}
			return true;
		case N3_DURABILITY_CHANGE:
			this->MsgRecv_DurabilityChange(pDataPack, iOffset);
			return true;
		case N3_KNIGHTS:
			this->MsgRecv_Knights(pDataPack, iOffset);
			return true;
		case N3_KNIGHTS_LIST_BASIC:
			this->MsgRecv_KnightsListBasic(pDataPack, iOffset);
			return true;
		// NOTE(srmeier): moved to GameProcedure class...
//		case N3_COMPRESSED_PACKET: // 압축된 데이터 이다... 한번 더 파싱해야 한다!!!
//			this->MsgRecv_CompressedPacket(pDataPack, iOffset);
//			return true;
		case N3_CONTINOUS_PACKET: // 압축된 데이터 이다... 한번 더 파싱해야 한다!!!
			this->MsgRecv_ContinousPacket(pDataPack, iOffset);
			return true;
		case N3_WAREHOUSE:	// 보관함..
			this->MsgRecv_WareHouse(pDataPack, iOffset);			// 보관함 관련 패킷..
			return true;
		case N3_FRIEND_INFO:
			if(m_pUIVar->m_pPageFriends) m_pUIVar->m_pPageFriends->MsgRecv_MemberInfo(pDataPack, iOffset);
			return true;
		case N3_NOAH_CHANGE:
			this->MsgRecv_NoahChange(pDataPack, iOffset);
			return true;
		case N3_WARP_LIST:
			this->MsgRecv_WarpList(pDataPack, iOffset);
			return true;
//		case N3_SERVER_CHECK:
//			this->MsgRecv_ServerCheckAndRequestConcurrentUserCount(pDataPack, iOffset);
//			return true;
//		case N3_SERVER_CONCURRENT_CONNECT:
//			this->MsgRecv_ConcurrentUserCountAndSendServerCheck(pDataPack, iOffset);
//			return true;
		case N3_CORPSE_CHAR: //regen을 하여 주위 유저에게 시체임을 알린다.
			this->MsgRecv_Corpse(pDataPack, iOffset);
			return true;
		case N3_PARTY_BBS:
			if(m_pUIPartyBBS) m_pUIPartyBBS->MsgRecv_RefreshData(pDataPack, iOffset);
			return true;
		case N3_TRADE_BBS:
			if(m_pUITradeBBS) m_pUITradeBBS->MsgRecv_TradeBBS(pDataPack, iOffset);
			return true;
		case N3_QUEST_SELECT:
			if(m_pUIQuestMenu) m_pUIQuestMenu->Open(pDataPack, iOffset);
			return true;
		case N3_QUEST_TALK:
			if(m_pUIQuestTalk) m_pUIQuestTalk->Open(pDataPack, iOffset);
			return true;
//		case N3_CLAN:
//			this->MsgRecv_Clan(pDataPack, iOffset);
			return true;
	}
	
	return false;
}

// 키보드와 마우스 눌린것을 처리한다..
void CGameProcMain::ProcessLocalInput(DWORD dwMouseFlags)
{
	// Loading이 된 후..
	if ( FALSE == m_bLoadComplete ) return; 

	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return;

	//////////////////////////////////////////
	//
	// 마우스 처리.
	//
	POINT ptPrev = s_pLocalInput->MouseGetPosOld();
	POINT ptCur = s_pLocalInput->MouseGetPos();

	OnMouseMove(ptCur, ptPrev);

	if( dwMouseFlags & MOUSE_RBCLICK )
	{
		// NOTE: right click on NPCs, interactable shapes, item boxes, etc.
		OnMouseRBtnPress(ptCur, ptPrev);
	}
	if( dwMouseFlags & MOUSE_RBDOWN )
	{
		// NOTE: this is where the right click rotation and zoom out occur
		OnMouseRbtnDown(ptCur, ptPrev);
	}
	if( dwMouseFlags & MOUSE_RBCLICK )
	{
		OnMouseRBtnPressd(ptCur, ptPrev);
	}
	if( dwMouseFlags & MOUSE_RBDBLCLK )
	{ 
		OnMouseRDBtnPress(ptCur, ptPrev);
	}
	if( dwMouseFlags & MOUSE_LBCLICK )
	{
		// NOTE: move on click
		OnMouseLBtnPress(ptCur, ptPrev);
	}
	if( dwMouseFlags & MOUSE_LBDOWN )
	{
		// NOTE: move on held down click
		OnMouseLbtnDown(ptCur, ptPrev);
	}
	if( dwMouseFlags & MOUSE_LBCLICKED )
	{
		OnMouseLBtnPressd(ptCur, ptPrev);
	}
	if( dwMouseFlags & MOUSE_LBDBLCLK )
	{
		OnMouseLDBtnPress(ptCur, ptPrev);
	}

	/*
	if (!(dwMouseFlags & MOUSE_RBDOWN)) {
		float fRotY = 0, fRotX = 0;
		if (ptCur.x <= 0) fRotY = -2.0f;
		else if (ptCur.x >= (CN3Base::s_CameraData.vp.Width - 1)) fRotY = 2.0f;
		if (ptCur.y <= 0) fRotX = -1.0f;
		else if (ptCur.y >= (CN3Base::s_CameraData.vp.Height - 1)) fRotX = 1.0f;
		if (fRotY)
		{
			if (VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
			else s_pPlayer->RotAdd(fRotY);
		}
		if (fRotX && VP_THIRD_PERSON != s_pEng->ViewPoint()) s_pEng->CameraPitchAdd(fRotX);
	}
	*/

	// NOTE: move camera when cursor is on the border
	/*
	// 마우스에 따른 카메라 회전...
	float fRotY = 0, fRotX = 0;
	if(0 == ptCur.x) fRotY = -2.0f;
	else if((CN3Base::s_CameraData.vp.Width - 1) == ptCur.x) fRotY = 2.0f;
	if(0 == ptCur.y) fRotX = -1.0f;
	else if((CN3Base::s_CameraData.vp.Height - 1) == ptCur.y) fRotX = 1.0f;
	if(fRotY)
	{
		if(VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
		else s_pPlayer->RotAdd(fRotY);
	}
	if(fRotX && VP_THIRD_PERSON != s_pEng->ViewPoint()) s_pEng->CameraPitchAdd(fRotX);
	*/

	//
	// 마우스 처리.
	//
	//////////////////////////////////////////

	//////////////////////////////////////////
	// 핫키
	int iHotKey = -1;
	if( s_pLocalInput->IsKeyPress(KM_HOTKEY1) ) iHotKey = 0;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY2) ) iHotKey = 1;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY3) ) iHotKey = 2;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY4) ) iHotKey = 3;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY5) ) iHotKey = 4;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY6) ) iHotKey = 5;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY7) ) iHotKey = 6;
	else if( s_pLocalInput->IsKeyPress(KM_HOTKEY8) ) iHotKey = 7;
		
	if ((iHotKey >= 0 && iHotKey < 8) &&
		CN3UIBase::GetFocusedEdit() == NULL && 
		m_pSubProcPerTrade->m_ePerTradeState == PER_TRADE_STATE_NONE )
	{
		m_pUIHotKeyDlg->EffectTriggerByHotKey(iHotKey);
	}
	// 핫키
	//////////////////////////////////////////

	if(s_pLocalInput->IsKeyPress(KM_CAMERA_CHANGE))												// 시점 변환..
	{
		this->CommandCameraChange(); // 카메라 시점 바꾸기..
	}

	// 삼인칭일때 홈, 엔드키로 카메로 올리고 내리기..
	if(s_pEng->ViewPoint() == VP_THIRD_PERSON)
	{
		float fPitch = 0;
		if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_HOME)) fPitch = D3DXToRadian(45.0f);		// home 키가 눌리면..
		else if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_END)) fPitch = D3DXToRadian(-45.0f);	// End 키가 눌리면..
		if(fPitch) s_pEng->CameraPitchAdd(fPitch);
	}

	if (!IsUIKeyOperated() && NULL == CN3UIBase::GetFocusedEdit() )			// 채팅모드가 아닐때 
	{
		if(s_pPlayer->m_InfoBase.iAuthority == AUTHORITY_MANAGER) //게임 운영자는 이 기능을 사용할수 있다.
		{
			if ( s_pLocalInput->IsKeyDown(SDL_SCANCODE_Q) ) s_pPlayer->m_bTempMoveTurbo = true; // 엄청 빨리 움직이게 한다..  // 임시 함수.. 나중에 없애자..
			else s_pPlayer->m_bTempMoveTurbo = false; // 엄청 빨리 움직이게 한다..  // 임시 함수.. 나중에 없애자..
		}

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_ATTACK))
			this->CommandToggleAttackContinous();		// 자동 공격..}
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_RUN))
			this->CommandToggleWalkRun();				// 걷기 / 뛰기 토글	
		if(s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_ENEMY))
			this->CommandTargetSelect_NearstEnemy();	// 가장 가까운 적 타겟 잡기..
		if(s_pLocalInput->IsKeyPress(KM_TARGET_NEARST_PARTY))
			this->CommandTargetSelect_NearstOurForce(); // 가장 가까운 파티 타겟잡기..

		float fRotKeyDelta = D3DXToRadian(60); // 초당 60 도 돌기..
		if(s_pLocalInput->IsKeyDown(KM_ROTATE_LEFT) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_LEFT))	
		{
			if(s_pPlayer->IsAlive()) s_pPlayer->RotAdd(-fRotKeyDelta); // 초당 180 도 왼쪽으로 돌기.
			if(m_pUIDroppedItemDlg->IsVisible()) m_pUIDroppedItemDlg->LeaveDroppedState();	
		}
		if(s_pLocalInput->IsKeyDown(KM_ROTATE_RIGHT) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RIGHT))	
		{
			if(s_pPlayer->IsAlive()) s_pPlayer->RotAdd(fRotKeyDelta); // 초당 180 도 오른쪽으로 돌기.
			if(m_pUIDroppedItemDlg->IsVisible()) m_pUIDroppedItemDlg->LeaveDroppedState();	
		}
		
		if(s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_UP))
		{
			bool bStart = false;
			if(s_pLocalInput->IsKeyPress(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPress(SDL_SCANCODE_UP))
			{
				if(VP_THIRD_PERSON == s_pEng->ViewPoint())
				{
					if(!s_pPlayer->m_bTargetOrPosMove) bStart = true;
					s_pPlayer->m_bTargetOrPosMove = false;
					if(s_pPlayer->m_bAttackContinous)
						CommandToggleAttackContinous();
				}
				else
					bStart = true;// 누르는 순간이면
			}
			this->CommandMove(MD_FOWARD, bStart); // 앞으로 이동..
		}
		else if(s_pLocalInput->IsKeyDown(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_DOWN))
		{
			bool bStart = false;
			if(s_pLocalInput->IsKeyPress(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPress(SDL_SCANCODE_DOWN)) 
			{
				if(VP_THIRD_PERSON == s_pEng->ViewPoint())
				{
					if(!s_pPlayer->m_bTargetOrPosMove) bStart = true;
					s_pPlayer->m_bTargetOrPosMove = false;
					if(s_pPlayer->m_bAttackContinous)
						CommandToggleAttackContinous();
				}
				else
					bStart = true;// 누르는 순간이면
			}
			this->CommandMove(MD_BACKWARD, bStart); // 뒤로 이동..
		}
		else if(s_pLocalInput->IsKeyPress(KM_TOGGLE_MOVE_CONTINOUS))
		{
			this->CommandToggleMoveContinous();
		}

		if(	s_pLocalInput->IsKeyPressed(KM_MOVE_FOWARD) || s_pLocalInput->IsKeyPressed(SDL_SCANCODE_UP) || 
			s_pLocalInput->IsKeyPressed(KM_MOVE_BACKWARD) || s_pLocalInput->IsKeyPressed(SDL_SCANCODE_DOWN) ) // 전진/후진 키를 떼는 순간. 
		{
			this->CommandMove(MD_STOP, true);
		}

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_INVENTORY)) this->CommandToggleUIInventory();
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_STATE)) this->CommandToggleUIState();
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_SKILL)) this->CommandToggleUISkillTree();
		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_SITDOWN)) this->CommandSitDown(true, !s_pPlayer->m_bSitDown);

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_HELP)) 
		{
			if(m_pUIHelp->IsVisible()) m_pUIHelp->SetVisible(false);
			else m_pUIHelp->SetVisible(true);
		}

		if(s_pLocalInput->IsKeyPress(KM_TOGGLE_MINIMAP))
		{
			this->CommandToggleUIMiniMap();
		}

		if (s_pLocalInput->IsKeyPress(SDL_SCANCODE_PAGEUP))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->PageUp();
		
		if (s_pLocalInput->IsKeyPress(SDL_SCANCODE_PAGEDOWN))
			if (m_pUIHotKeyDlg)	m_pUIHotKeyDlg->PageDown();

	} // end of if ( !m_UIChatDlg.IsChatMode() )

	// ..... 나머지 키보드 처리..
//#if _DEBUG
	if(s_pLocalInput->IsKeyPress(SDL_SCANCODE_F12)) // 디버깅 테스트..
		s_pEng->Lightning(); // 번개 치기..
//#endif
}

void CGameProcMain::ProcessPlayerInclination()											// 경사에 서 있을때..
{
/*	float fFrm = GetIndepTimeFrame();

	__Vector3 vNorm, vNormXZ; 
	s_pTerrain->GetNormal( s_pEng->m_matPlayer.Pos().x, s_pEng->m_matPlayer.Pos().z, vNorm );

	vNorm.Normalize();
	vNormXZ = vNorm;
	vNormXZ.y = 0.0f;

	if ( vNormXZ.Magnitude() > 0.8f )
	{
		vNormXZ.x *= 0.01f*fFrm;
		vNormXZ.z *= 0.01f*fFrm;
		s_pPlayer->m_vPlayerIncline = vNormXZ;
	}*/
}

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

void CGameProcMain::MsgSend_Continous()						// 특정 조건(?)하에서 서버에게 정기적으로 메시지를 보냄..
{
	float fTime = s_pEng->TimeGet();

	if ( fTime >= m_fMsgSendTimeMove + PACKET_INTERVAL_MOVE )					// 1초가 지났으면..	
	{
		__Vector3 vPos = s_pPlayer->Position();
		if(m_vPlayerPosSended != vPos) this->MsgSend_Move(true, true); // 조금이라도 움직였으면 움직임 패킷 보냄..
	}

	if( false == s_pPlayer->m_bMoveContinous && fTime >= m_fMsgSendTimeRot + PACKET_INTERVAL_ROTATE ) // 플레이어가 정지해 있고.. 2초가 지났으면..
	{
		float fYaw = s_pPlayer->Yaw();
		if( fYaw != m_fPlayerYawSended ) MsgSend_Rotation(); // 조금이라도 회전했으면 회전 패킷 보냄
	}
}



//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////

void CGameProcMain::MsgSend_Attack(int iTargetID, float fInterval, float fDistance) // 공격 패킷 날리기 - 테이블의 공격 주기를 같이 줘서 해킹을 막는다.
{
	if(s_pPlayer->m_fTimeAfterDeath > 0 || s_pPlayer->IsDead())	return; // 죽은 넘이다..

	BYTE byBuff[32];												// 버퍼.. 
	int iOffset=0;													// 옵셋..

	BYTE bySuccess = true;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_ATTACK);						// 공격 커멘드..
	CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);							// ??? 데미지??
	CAPISocket::MP_AddByte(byBuff, iOffset, bySuccess);						// 성공 여부.. - 일단 성공으로 보낸다.

	fInterval += 0.1f;

	CAPISocket::MP_AddShort(byBuff, iOffset, iTargetID );					// 상대방 아이디..
	CAPISocket::MP_AddShort(byBuff, iOffset, (int)(fInterval * 100));	// 공격한 시간
	CAPISocket::MP_AddShort(byBuff, iOffset, (int)(fDistance * 10));	// 공격한 거리

	s_pSocket->Send(byBuff, iOffset);										// 보냄..
}

void CGameProcMain::MsgSend_Move(bool bMove, bool bContinous)
{
	__Vector3 vPos(0,0,0);
	float fSpeed = s_pPlayer->MoveSpeed();
	
	BYTE byMoveFlag = 0;

	if(true == bMove) // 움직이기 시작할때나 움직일때는
	{
		if(s_pPlayer->m_fTimeAfterDeath > 0 || s_pPlayer->IsDead())	return; // 죽은 넘이다..

		vPos = s_pPlayer->NextPos(PACKET_INTERVAL_MOVE); // 다음 1초후의 위치를 계산해서.
		byMoveFlag |= 0x01;
	}
	else // 정지시에는 
	{
		s_pPlayer->m_bTargetOrPosMove	= false;
		s_pPlayer->m_iMoveTarget		= -1;
		vPos = s_pPlayer->Position();
		fSpeed = 0;
	}

	if(true == bContinous) // 정기적으로 움직이는 거라면..
	{
		byMoveFlag |= 0x02;
	}

	BYTE byBuff[64];											// 버퍼 설정..
	int iOffset=0;											// 옵셋..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_MOVE);			// 커멘드..
	CAPISocket::MP_AddWord(byBuff, iOffset, vPos.x*10);			// 다음 위치
	CAPISocket::MP_AddWord(byBuff, iOffset, vPos.z*10);
	CAPISocket::MP_AddShort(byBuff, iOffset, vPos.y*10);
	CAPISocket::MP_AddWord(byBuff, iOffset, fSpeed*10);			// 속도 
	CAPISocket::MP_AddByte(byBuff, iOffset, byMoveFlag );		// 움직임 플래그..
	s_pSocket->Send(byBuff, iOffset);							// 패킷을 보냄..

	m_vPlayerPosSended = s_pPlayer->Position(); // 최근에 보낸 위치 세팅..
	
	if(true == bMove) m_fMsgSendTimeMove = CN3Base::TimeGet(); // 시간을 기록한다..
}

void CGameProcMain::MsgSend_Rotation()
{
	if(s_pPlayer->IsDead()) return; // 죽은 넘이 어딜 감히!!

	BYTE byBuff[8];
	int iOffset=0;

	float fYaw = s_pPlayer->Yaw(); // 방향..
	
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_ROTATE);
	CAPISocket::MP_AddShort(byBuff, iOffset, fYaw*100);

	s_pSocket->Send(byBuff, iOffset);

	m_fPlayerYawSended = fYaw; // 회전값을 기록
	m_fMsgSendTimeRot = CN3Base::TimeGet(); // 시간을 기록한다..
}


void CGameProcMain::MsgSend_Chat(e_ChatMode eMode, const std::string& szChat)
{
	if(szChat.empty() || szChat.size() >= 128) return;
	if(eMode==N3_CHAT_CLAN && s_pPlayer->m_InfoExt.iKnightsID <=0) return;

	BYTE byBuff[512];
	int iOffset=0;
	
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_CHAT);
	CAPISocket::MP_AddByte(byBuff, iOffset, eMode);
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)szChat.size());
	CAPISocket::MP_AddString(byBuff, iOffset, szChat);

	__ASSERT(iOffset<512, "Send Buffer OverFlow");
	s_pSocket->Send(byBuff, iOffset); // 보낸다..
}

void CGameProcMain::MsgSend_ChatSelectTarget(const std::string& szTargetID)
{
	if(szTargetID.empty() || szTargetID.size() > 20) return;

	int iOffset = 0;
	BYTE byBuff[32];

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_CHAT_SELECT_TARGET);

	// TEMP(srmeier): testing private messages
	CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);

	CAPISocket::MP_AddShort(byBuff, iOffset, (short)szTargetID.size() );
	CAPISocket::MP_AddString(byBuff, iOffset, szTargetID );

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Regen()
{
	if(s_pPlayer->m_iSendRegeneration >= 2) return; // 한번 보내면 다시 죽을때까지 안보내는 플래그

	BYTE byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_REGENE);
	CAPISocket::MP_AddByte(byBuff, iOffset, 1); //1: 마을로 살아나기..
	
	CLogWriter::Write("Send Regeneration");

	s_pSocket->Send(byBuff, iOffset); // 보낸다..

	s_pPlayer->m_iSendRegeneration = 2; // 한번 보내면 다시 죽을때까지 안보내는 플래그
	TRACE("보냄 - 다시 살아나기\n");
}

bool CGameProcMain::MsgSend_RequestItemBundleOpen(CPlayerNPC* pCorpse)
{
	if( NULL == pCorpse || pCorpse->m_iDroppedItemID <= 0) return false;

	float fDistTmp = (pCorpse->Position() - s_pPlayer->Position()).Magnitude();
	if(	fDistTmp >= (pCorpse->Radius() * 2.0f + 6.0f)) return false;

	int iItemBundleID = pCorpse->m_iDroppedItemID;
	m_pUIDroppedItemDlg->m_iItemBundleID = pCorpse->m_iDroppedItemID;
		
	s_pOPMgr->CorpseRemove(pCorpse, false); // 점점 투명하게 없앤다..
	
	BYTE byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_ITEM_BUNDLE_OPEN_REQUEST);
	CAPISocket::MP_AddDword(byBuff, iOffset, iItemBundleID);

	s_pSocket->Send(byBuff, iOffset); // 보낸다..

	return true;
}

void CGameProcMain::MsgSend_PartyOrForcePermit(int iPartyOrForce, bool bYesNo)
{
	BYTE byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_OR_FORCE);
//	CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_PERMIT);
	CAPISocket::MP_AddByte(byBuff, iOffset, bYesNo);

	s_pSocket->Send(byBuff, iOffset); // 보낸다..
}

bool CGameProcMain::MsgSend_PartyOrForceCreate(int iPartyOrForce, const std::string& szID)
{
	if(szID.empty() || szID.size() > 20) return false;

	bool bIAmLeader, bIAmMember;
	int iMemberIndex = -1;
	CPlayerBase* pTarget = NULL;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMember, iMemberIndex, pTarget);
	if(true == bIAmMember && false == bIAmLeader) return false; // 내가 파티에 들어 있고 리더가 아니면 실패..

	BYTE byBuff[32];
	int iOffset=0;

	e_SubPacket_Party eCmdParty = N3_SP_PARTY_OR_FORCE_CREATE;
	if(m_pUIPartyOrForce->MemberCount() >= 2)
	{
		eCmdParty = N3_SP_PARTY_OR_FORCE_INSERT;
	}

	m_pUIPartyOrForce->m_iPartyOrForce = iPartyOrForce;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_OR_FORCE);
//	CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
	CAPISocket::MP_AddByte(byBuff, iOffset, eCmdParty);
	CAPISocket::MP_AddShort(byBuff, iOffset, szID.size());
	CAPISocket::MP_AddString(byBuff, iOffset, szID);

	s_pSocket->Send(byBuff, iOffset); // 보낸다..
	
	if(m_pUIPartyOrForce->MemberCount() <= 0) // 처음 생성하는 경우...
	{
		m_pUIPartyOrForce->MemberAdd(
			s_pPlayer->IDNumber(),
			s_pPlayer->IDString(),
			s_pPlayer->m_InfoBase.iLevel, 
			s_pPlayer->m_InfoBase.eClass, 
			s_pPlayer->m_InfoBase.iHP, 
			s_pPlayer->m_InfoBase.iHPMax);  // 내건 미리 넣어 놓는다..
	}

	TRACE ("Party or Force 생성 신청 - Target ID(%s)\n", szID.c_str());

	return true;
}

void CGameProcMain::MsgSend_PartyOrForceLeave(int iPartyOrForce)
{
	if(m_pUIPartyOrForce->MemberCount() <= 0) return; // 파티원이 없다..

	CPlayerBase* pTarget = NULL;
	bool bIAmLeader, bIAmMember;
	int iMemberIndex = -1;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMember, iMemberIndex, pTarget);

	BYTE byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_PARTY_OR_FORCE);
//	CAPISocket::MP_AddByte(byBuff, iOffset, iPartyOrForce);
	if(bIAmLeader) // 내가 리더일경우..
	{
		if(iMemberIndex > 0 && pTarget) // 파티원이다.. 쫓아내자..
		{
			CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_REMOVE);
			CAPISocket::MP_AddShort(byBuff, iOffset, pTarget->IDNumber());
		}
		else // 타겟이 파티원이 아니면 파티 뽀개기..
		{
			CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_DESTROY);
		}
	}
	else if(bIAmMember) // 리더가 아니면 탈퇴 메시지를 보낸다..
	{
		CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PARTY_OR_FORCE_REMOVE);
		CAPISocket::MP_AddShort(byBuff, iOffset, s_pPlayer->IDNumber());
	}
	s_pSocket->Send(byBuff, iOffset); // 보낸다..
}

void CGameProcMain::MsgSend_ObjectEvent(int iEventID, int iNPCID)
{
	BYTE byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_OBJECT_EVENT);
	CAPISocket::MP_AddShort(byBuff, iOffset, iEventID);	// Index
	CAPISocket::MP_AddShort(byBuff, iOffset, iNPCID);	// Parameter

	s_pSocket->Send(byBuff, iOffset); // 보낸다..
}

void CGameProcMain::MsgSend_Weather(int iWeather, int iPercent)
{
	if(iWeather < 1 || iWeather > 3) return;
	if(iPercent < 0 || iPercent > 100) return;

	BYTE byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_WEATHER); // -> byte - 기후.... 0x01 - 맑음.. 0x02 -  비 0x03
	CAPISocket::MP_AddByte(byBuff, iOffset, iWeather); // -> byte - 기후.... 0x01 - 맑음.. 0x02 -  비 0x03
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)iPercent); // short -> 맑은날 안개, 비, 눈 의 양 퍼센트로 

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Time(int iHour, int iMin)
{
	BYTE byBuff[12];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_TIME); 
	CAPISocket::MP_AddShort(byBuff, iOffset, 0);		// year
	CAPISocket::MP_AddShort(byBuff, iOffset, 0);		// month
	CAPISocket::MP_AddShort(byBuff, iOffset, 0);		// day
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)iHour);
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)iMin);

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_Administrator(e_SubPacket_Administrator eSP, const std::string& szID)
{
	if(szID.empty() || szID.size() >= 20) return;

	BYTE byBuff[64];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_ADMINISTRATOR); // 관리자 전용패킷..
	CAPISocket::MP_AddByte(byBuff, iOffset, eSP);
	CAPISocket::MP_AddShort(byBuff, iOffset, szID.size());
	CAPISocket::MP_AddString(byBuff, iOffset, szID);	

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsJoinReq(bool bJoin)
{
	BYTE byBuff[8];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // 관리자 전용패킷..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_JOIN_REQ);
	CAPISocket::MP_AddByte(byBuff, iOffset, (BYTE)bJoin);
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)m_iJoinReqClanRequierID);
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)m_iJoinReqClan);
	
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsJoin(int iTargetID)
{
	BYTE byBuff[4];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // 관리자 전용패킷..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_JOIN);
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)iTargetID);
	
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsLeave(std::string& szName)
{
	BYTE byBuff[64];
	int iOffset=0;

	int iLen = szName.size();

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // 관리자 전용패킷..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_MEMBER_REMOVE);
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)iLen);
	CAPISocket::MP_AddString(byBuff, iOffset, szName);	// 아이디 문자열 패킷에 넣기..
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsWithdraw()
{
	BYTE byBuff[2];
	int iOffset=0;

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); 
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_WITHDRAW);
	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_KnightsAppointViceChief(std::string& szName)
{
	BYTE byBuff[64];
	int iOffset=0;

	int iLen = szName.size();

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_KNIGHTS); // 관리자 전용패킷..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_KNIGHTS_APPOINT_VICECHIEF);
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)iLen);
	CAPISocket::MP_AddString(byBuff, iOffset, szName);	// 아이디 문자열 패킷에 넣기..
	s_pSocket->Send(byBuff, iOffset);
}

bool CGameProcMain::MsgRecv_MyInfo_All(DataPack* pDataPack, int& iOffset)
{
	int iZone = s_pPlayer->m_InfoExt.iZoneCur;
	s_pPlayer->Release(); // 일단 몽창 다 해제 하고....
	s_pPlayer->m_InfoExt.iZoneCur = iZone;

	int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	int iLen = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	std::string szID;
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);
	s_pPlayer->IDSet(iID, szID, D3DCOLOR_XRGB(100, 210, 255)); // 밝은 파란색과 하늘색 중간..

	float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fY = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;
	
	s_pPlayer->m_InfoBase.eNation = (e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoBase.eRace = (e_Race)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoBase.eClass = (e_Class)CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iFace = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 얼굴 모양..
	s_pPlayer->m_InfoExt.iHair = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 머리카락

	__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks->Find(s_pPlayer->m_InfoBase.eRace);	// User Player Character Skin 구조체 포인터..
	if(NULL == pLooks) CLogWriter::Write("CGameProcMain::MsgRecv_MyInfo_All : failed find character resource data (Race : %d)", s_pPlayer->m_InfoBase.eRace);
	__ASSERT(pLooks, "failed find character resource data");
	s_pPlayer->InitChr(pLooks); // 관절 세팅..

	s_pPlayer->m_InfoExt.iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iTitle = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoBase.iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
	s_pPlayer->m_InfoExt.iBonusPointRemain = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 남은 보너스 포인트..

	s_pPlayer->m_InfoExt.iExpNext = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); 
	s_pPlayer->m_InfoExt.iExp = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); 
	s_pPlayer->m_InfoExt.iRealmPoint = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

	// NOTE(srmeier): adding the monthly loyalty
	int iMonthlyLoyalty = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

	s_pPlayer->m_InfoExt.iCity = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	
	std::string szKnightsName = "";
	int iKnightsID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 소속 기사단 ID
	e_KnightsDuty eKnightsDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 기사단에서의 권한..
	
	// NOTE(srmeier): adding alliance ID and knight's byFlag
	int iAllianceID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	BYTE byFlag = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	
	int iKnightNameLen = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 소속 기사단 이름 길이.
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iKnightNameLen);
	int iKnightsGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 소속 기사단 등급
	int	iKnightsRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 소속 기사단 순위
	
	// NOTE(srmeier): adding mark version and cape ID
	short sMarkVersion = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short sCapeID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	// 기사단 관련 세팅..
	s_pPlayer->m_InfoExt.eKnightsDuty = eKnightsDuty; // 기사단에서의 권한..
	s_pPlayer->KnightsInfoSet(iKnightsID, szKnightsName, iKnightsGrade, iKnightsRank);
	m_pUIVar->UpdateKnightsInfo();
	
	s_pPlayer->m_InfoBase.iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	
	s_pPlayer->m_InfoBase.iHP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); 
	s_pPlayer->m_InfoExt.iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iMSP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iWeightMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); 
	s_pPlayer->m_InfoExt.iWeight = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); 

	s_pPlayer->m_InfoExt.iStrength = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iStrength_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iStamina = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iStamina_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iDexterity = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iDexterity_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iIntelligence = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iIntelligence_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iMagicAttak = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iMagicAttak_Delta = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	
	s_pPlayer->m_InfoExt.iAttack = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
//	s_pPlayer->m_InfoExt.iAttack_Delta		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iGuard = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
//	s_pPlayer->m_InfoExt.iGuard_Delta		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iRegistFire = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iRegistCold = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iRegistLight = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iRegistMagic = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iRegistCurse = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iRegistPoison = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	s_pPlayer->m_InfoExt.iGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoBase.iAuthority = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); //권한.. 

	// NOTE(srmeier): adding national rank and leader rank
	BYTE bKnightsRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	BYTE bPersonalRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	// 스킬 UI 갱신..
	for ( int i = 0; i < 9; i++ )
	{
		m_pUISkillTreeDlg->m_iSkillInfo[i] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	}
	m_pUISkillTreeDlg->InitIconUpdate();
	m_pUIHotKeyDlg->ReleaseItem();
	m_pUIHotKeyDlg->InitIconUpdate();			// 핫키가 유효한지 검사하고 유효하면 레지스트리에서 읽어온다..

	// 장착하고 있는 거..
	int iItemIDInSlots[ITEM_SLOT_COUNT]; memset(iItemIDInSlots, -1, sizeof(iItemIDInSlots));
	int iItemDurabilityInSlots[ITEM_SLOT_COUNT]; memset(iItemDurabilityInSlots, -1, sizeof(iItemDurabilityInSlots));
	int iItemCountInSlots[ITEM_SLOT_COUNT]; memset(iItemCountInSlots, -1, sizeof(iItemCountInSlots));

	for ( int i = 0; i < ITEM_SLOT_COUNT; i++ )				// 슬롯 갯수마큼..
	{
		iItemIDInSlots[i] = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		iItemDurabilityInSlots[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		iItemCountInSlots[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		// NOTE(srmeier): adding rental flag and remaining time
		BYTE bRentFlag = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		short sRemainingRentalTime = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	}

	m_fMsgSendTimeMove		= 0;						// Network ReQuest 타이머 초기화..
	m_fMsgSendTimeRot		= 0;
	m_fPlayerYawSended		= 0;						// 최근에 메시지를 보낸 시점의 플레이어 y 축 회전값.
	m_vPlayerPosSended		= s_pPlayer->Position();	// 최근에 메시지를 보낸 시점의 플레이어 위치.

	// 상태창 수치를 모두 적용
	if(m_pUIVar->m_pPageState) m_pUIVar->m_pPageState->UpdateID(szID); // 이름 적용.
	m_pUIVar->UpdateAllStates(&(s_pPlayer->m_InfoBase), &(s_pPlayer->m_InfoExt));

	//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(s_pPlayer->m_InfoExt.iKnightsID);
	//if(pKIB) m_pUIVar->m_pPageKnights->UpdateKnightsName(pKIB->szName);
	
	// 상태 바 갱신
	m_pUIStateBarAndMiniMap->UpdateExp(s_pPlayer->m_InfoExt.iExp, s_pPlayer->m_InfoExt.iExpNext, true);
	m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, true);
	m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, true);

	m_pUIPartyOrForce->MemberInfoReInit(); // 파티 창.. 갱신..
	
	__TABLE_ITEM_BASIC* pItem = NULL;								// 아이템 테이블 구조체 포인터..	
	__TABLE_ITEM_EXT* pItemExt = NULL;								// 아이템 테이블 구조체 포인터..	

	int iItemIDInInventorys[MAX_ITEM_INVENTORY]; memset(iItemIDInInventorys, -1, sizeof(iItemIDInInventorys));
	int iItemCountInInventorys[MAX_ITEM_INVENTORY]; memset(iItemCountInInventorys, -1, sizeof(iItemCountInInventorys));
	int iItemDurabilityInInventorys[MAX_ITEM_INVENTORY]; memset(iItemDurabilityInInventorys, -1, sizeof(iItemDurabilityInInventorys));

	for ( int i = 0; i < MAX_ITEM_INVENTORY; i++ )				// 슬롯 갯수마큼..
	{
		iItemIDInInventorys[i] = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		iItemDurabilityInInventorys[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		iItemCountInInventorys[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		// NOTE(srmeier): adding rental flag and remaining time
		BYTE bRentFlag = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		short sRemainingRentalTime = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	}


	// NOTE(srmeier): adding is chicken and manner points
	BYTE bIDK; short sIDK;
	bIDK = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	bIDK = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	sIDK = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	BYTE bIsChicken = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	int iMannerPoints = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);


	m_pUIInventory->ReleaseItem();

	std::string szResrcFN, szIconFN;
	for ( int i = 0; i < ITEM_SLOT_COUNT; i++ )				// 슬롯 갯수마큼..
	{
		if(0 == iItemIDInSlots[i]) continue;

		pItem = s_pTbl_Items_Basic->Find(iItemIDInSlots[i]/1000*1000);	// 열 데이터 얻기..
		if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
			pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemIDInSlots[i]%1000);	// 열 데이터 얻기..
		else
			pItemExt = NULL;

		if ( NULL == pItem || NULL == pItemExt )
		{
			__ASSERT(0, "NULL Item!!!");
			CLogWriter::Write("MyInfo - slot - Unknown Item %d, IDNumber", iItemIDInSlots[i]);
			continue; // 아이템이 없으면..
		}

		e_PartPosition ePart;
		e_PlugPosition ePlug;
		e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, &szResrcFN, &szIconFN, ePart, ePlug, s_pPlayer->m_InfoBase.eRace); // 아이템에 따른 파일 이름을 만들어서
		if(ITEM_TYPE_UNKNOWN == eType) CLogWriter::Write("MyInfo - slot - Unknown Item");
		__ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item Type");
		e_ItemSlot eSlot = (e_ItemSlot)i;

		switch(eSlot)
		{
		case ITEM_SLOT_EAR_RIGHT:
		case ITEM_SLOT_EAR_LEFT:
		case ITEM_SLOT_NECK:
		case ITEM_SLOT_RING_RIGHT:
		case ITEM_SLOT_RING_LEFT:
			{
				if(ITEM_TYPE_ICONONLY != eType) CLogWriter::Write("MyInfo - slot - Invalid Item");
				__ASSERT(ITEM_TYPE_ICONONLY == eType, "Invalid Item");
			}
			break;
		case ITEM_SLOT_UPPER: // Parts - 상체
		case ITEM_SLOT_LOWER: // Parts - 하체
		case ITEM_SLOT_GLOVES: // 장갑
		case ITEM_SLOT_SHOES: // 신발
		case ITEM_SLOT_HEAD: // 머리카락 ?? -> 투구 ??
			{
				if(ITEM_TYPE_PART != eType)  CLogWriter::Write("MyInfo - slot - Invalid Item");
				__ASSERT(ITEM_TYPE_PART == eType, "Invalid Item");
				s_pPlayer->PartSet(ePart, szResrcFN, pItem, pItemExt);	// 파트를 셋팅..
			}
			break;
		case ITEM_SLOT_HAND_RIGHT: // 오른손
		case ITEM_SLOT_HAND_LEFT: // 왼손
			{
				if(ITEM_TYPE_PLUG != eType) CLogWriter::Write("MyInfo - slot - Invalid Item");
				__ASSERT(ITEM_TYPE_PLUG == eType, "Invalid Item");
				
				e_PlugPosition ePlugPos;
				if(ITEM_SLOT_HAND_RIGHT == eSlot) ePlugPos = PLUG_POS_RIGHTHAND;
				else ePlugPos = PLUG_POS_LEFTHAND;
				s_pPlayer->PlugSet(ePlugPos, szResrcFN, pItem, pItemExt);	// 파트를 셋팅..
			}
			break;
		case ITEM_SLOT_SHOULDER: // 망토
			{
			}
			break;
		case ITEM_SLOT_BELT:
			{
			}
			break;
		}

		s_pPlayer->DurabilitySet(eSlot, iItemDurabilityInSlots[eSlot]);	// 무기 낡음 처리..

		__IconItemSkill* spItem = new __IconItemSkill;
		spItem->pItemBasic	= pItem;
		spItem->pItemExt	= pItemExt;
		spItem->szIconFN	= szIconFN; // 아이콘 파일 이름 복사..
		spItem->iCount		= iItemCountInSlots[i];
		spItem->iDurability = iItemDurabilityInSlots[i];

		// 인벤토리 슬롯에 넣는다.
		m_pUIInventory->m_pMySlot[i] = spItem;
		TRACE("Init Inv Msg Slot %d \n", iItemIDInSlots[i]);
	}

	// 인벤토리..
	int iItemCount = 0;
	for ( int i = 0; i < MAX_ITEM_INVENTORY; i++ )				// 인벤토리 갯수만큼..	
	{
		if(!iItemIDInInventorys[i]) continue;

		pItem = s_pTbl_Items_Basic->Find(iItemIDInInventorys[i]/1000*1000);	// 열 데이터 얻기..
		pItemExt = NULL;
		if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
			pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(iItemIDInInventorys[i]%1000);	// 열 데이터 얻기..
		if ( NULL == pItem || NULL == pItemExt )
		{
			__ASSERT(0, "NULL Item");
			CLogWriter::Write("MyInfo - Inv - Unknown Item %d, IDNumber", iItemIDInInventorys[i]);
			continue; // 아이템이 없으면..
		}

		e_PartPosition ePart;
		e_PlugPosition ePlug;
		e_ItemType eType = CGameProcedure::MakeResrcFileNameForUPC(pItem, NULL, &szIconFN, ePart, ePlug, s_pPlayer->m_InfoBase.eRace); // 아이템에 따른 파일 이름을 만들어서
		if(ITEM_TYPE_UNKNOWN == eType) CLogWriter::Write("MyInfo - slot - Unknown Item");
		__ASSERT(ITEM_TYPE_UNKNOWN != eType, "Unknown Item");
		
		__IconItemSkill* spItem = new __IconItemSkill;
		spItem->pItemBasic	= pItem;
		spItem->pItemExt	= pItemExt;
		spItem->szIconFN = szIconFN; // 아이콘 파일 이름 복사..
		spItem->iCount	= iItemCountInInventorys[i];
		spItem->iDurability = iItemDurabilityInInventorys[i];

		m_pUIInventory->m_pMyInvWnd[i] = spItem; 
		TRACE("Init Inv Msg Inve %d, iOrder %d \n", iItemIDInInventorys[i], i);
	}

	// Inventory & Slot Icon Operation.. ^^
	m_pUIInventory->InitIconUpdate();

	if (m_pUISkillTreeDlg) m_pUISkillTreeDlg->UpdateDisableCheck();
	if (m_pUIHotKeyDlg) m_pUIHotKeyDlg->UpdateDisableCheck();

	if(s_pPlayer->Part(PART_POS_UPPER)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_UPPER, pLooks->szPartFNs[PART_POS_UPPER], NULL, NULL);	// 상체..
	if(s_pPlayer->Part(PART_POS_LOWER)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_LOWER, pLooks->szPartFNs[PART_POS_LOWER], NULL, NULL);	// 하체..
	if(s_pPlayer->Part(PART_POS_HANDS)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_HANDS,  pLooks->szPartFNs[PART_POS_HANDS], NULL, NULL);	// 팔..
	if(s_pPlayer->Part(PART_POS_FEET)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->PartSet(PART_POS_FEET,  pLooks->szPartFNs[PART_POS_FEET], NULL, NULL);	// 다리..
	if(s_pPlayer->Part(PART_POS_FACE)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->InitFace();
	if(s_pPlayer->Part(PART_POS_HAIR_HELMET)->FileName().empty()) // 아무것도 세팅안되어 있으면 파일 이름이 없다..
		s_pPlayer->InitHair();

	s_pPlayer->InventoryChrAnimationInitialize();
	s_pPlayer->SetSoundAndInitFont();

//	s_pPlayer->SettingCollisionCheckPoint();

	////////////////////////////////////////////////////////////
	// 기본값 읽기..
	int iRun = 1;
//	if(false == CGameProcedure::RegGetSetting("UserRun", &iRun, 4)) iRun = 1; // 걷고 뛰는 상태를 레지스트리에서 읽고.. 기본값은 뛰는 상태이다..
//	if(1 == iRun)
	this->CommandToggleWalkRun(); // 뛰게 만든다..

	e_ViewPoint eVP = VP_THIRD_PERSON;
	if(false == CGameProcedure::RegGetSetting("CameraMode", &eVP, 4)) eVP = VP_THIRD_PERSON; // 카메라 상태 기록
	s_pEng->ViewPointChange(eVP);
	// 기본값 읽기..
	////////////////////////////////////////////////////////////

	this->InitPlayerPosition(__Vector3(fX, fY, fZ)); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.

	// berserk temp
	//s_pPlayer->PlugSet(PLUG_POS_BACK, "item/babacloak.n3cplug_cloak", NULL);	// 파트를 셋팅..
	// end berserk temp

	// berserk
	//s_pPlayer->AttachCloak();

	//..
	s_pOPMgr->Release();							// 다른 유저 관리 클래스 초기화..
	
	m_bLoadComplete = TRUE;						// 로딩 끝..

	return true;
}

bool CGameProcMain::MsgRecv_Chat(DataPack* pDataPack, int& iOffset)
{
	std::string szChat;				// 버퍼..
	e_ChatMode eCM	=	(e_ChatMode)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 채팅 타입
	e_Nation eNation =	(e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 보낸사람 국가
	int iID =			CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);			// 보낸사람

	std::string szName;
	int iNameLen = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szName, iNameLen);

	std::string szMsg;
	int iMsgLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szMsg, iMsgLen);

	szChat = szName + " : " + szMsg;
	int iChatLen = szChat.size();
	
	if(eCM == N3_CHAT_CONTINUE_DELETE)
	{//지속 공지 삭제...
		m_pUIChatDlg->DeleteContinueMsg();
		return true;
	}
	else if(eCM == N3_CHAT_TITLE_DELETE)
	{//타이틀 공지 삭제...
		m_pUIChatDlg->SetNoticeTitle("", 0xffffffff);
		return true;
	}

	D3DCOLOR crChat = 0xffffffff;
	switch(eCM)
	{
	case N3_CHAT_NORMAL:	crChat = D3DCOLOR_ARGB(255,255,255,255);	break;
	case N3_CHAT_PRIVATE:	crChat = D3DCOLOR_ARGB(255, 128, 255, 255);/*D3DCOLOR_ARGB(255,192,192,0);*/		break;
	case N3_CHAT_PARTY:		crChat = D3DCOLOR_ARGB(255,0,192,192);		break;
	case N3_CHAT_FORCE:		crChat = D3DCOLOR_ARGB(255,0,192,192);		break;
	//case N3_CHAT_SHOUT:	crChat = D3DCOLOR_ARGB(255,255,0,0);		break;
	case N3_CHAT_SHOUT:		crChat = D3DCOLOR_ARGB(255,0xf8,0x66,0x05);	break;
	case N3_CHAT_CLAN:		crChat = D3DCOLOR_ARGB(255,0,255,0);		break;
	case N3_CHAT_PUBLIC:	crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	case N3_CHAT_WAR:		crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	case N3_CHAT_TITLE:		crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	case N3_CHAT_CONTINUE:	crChat = D3DCOLOR_ARGB(255,255,255,0);		break;
	}

	if(eCM == N3_CHAT_TITLE)
	{
		m_pUIChatDlg->SetNoticeTitle(szChat, crChat);
		return true;
	}
	else if(N3_CHAT_WAR == eCM)
	{
		if(m_pWarMessage) m_pWarMessage->SetMessage(szChat, D3DFONT_BOLD, crChat);
		return true;
	}
	else if(N3_CHAT_CONTINUE == eCM)
	{
		m_pUIChatDlg->AddContinueMsg(eCM, szChat, crChat);
		return true;
	}


	// 통역 서비스...           ㅡ,.ㅡ a
	if( N3_CHAT_NORMAL == eCM || 
		N3_CHAT_PRIVATE == eCM || 
		N3_CHAT_SHOUT == eCM )
	{
		// TEMP(srmeier): this again depends on zoneability and will need to be looked at
		/*
		if(eNation != s_pPlayer->m_InfoBase.eNation)
		{
			CPlayerBase* pTalker = s_pOPMgr->UPCGetByID(iID, false);
			bool bIamManager = (0 == s_pPlayer->m_InfoBase.iAuthority) ? true : false;
			bool bTalkerIsManager = (pTalker && 0 == pTalker->m_InfoBase.iAuthority)  ? true : false;
				
			if(!(bIamManager || bTalkerIsManager)) // 내가 운영자가 아니고 상대방도 운영자가 아니면
			{
				int i = szChat.find(':');
				if(i >= 0)
				{
					for(; i < iChatLen; i++)
					{
						szChat[i] = '!' + rand()%10; // 이상한 말로 바꾼다..
					}
				}
			}
		}
		*/
	}
	
	// 풍선말 넣기..
	CPlayerBase* pBPC = NULL;
	if(iID == s_pPlayer->IDNumber()) pBPC = s_pPlayer;
	else pBPC = s_pOPMgr->CharacterGetByID(iID, false);
	if(pBPC && s_pPlayer->Distance(pBPC->Position()) < 4.0f &&
		(N3_CHAT_NORMAL == eCM || N3_CHAT_SHOUT == eCM) ) // 보통 채팅 혹은 외치기일때만..
		pBPC->BalloonStringSet(szChat, crChat);

	// 채팅창에 넣기..
	m_pUIChatDlg->AddChatMsg(eCM, szChat, crChat);

	return true;
}

bool CGameProcMain::MsgRecv_UserMove(DataPack* pDataPack, int& iOffset)
{
	int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	float fX		= (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f; // 출발하거나 이동중일때에는 다음 위치. 정지할때는 현재 위치를 받는다.
	float fZ		= (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fY		= (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;
	float fSpeed	= (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f; // 출발하거나 이동중일때에는 움직이는 속도. 정지할때는 0 이 온다.
	BYTE byMoveFlag = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 움직이는 플래그.. 0 정지 1 출발, 2 계속 움직임

	// 함수가 와야 할 부분.. ^^
	// 아이디, 플레이어 상태 1, 플레이어 상태 2, 현재 xzy 위치, 현재 xzy 방향, 1초뒤 x, z, y dnlcl..

	if(0xff == byMoveFlag) // 강제위치 업데이트 패킷이다..
	{
		if(iID == s_pPlayer->IDNumber())	// 내 매시지면..
		{
			this->InitPlayerPosition(__Vector3(fX, fY, fZ)); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
		}
		else
		{
			CPlayerNPC* pBPC = s_pOPMgr->CharacterGetByID(iID, true);	// 다른 플레이어를 얻어서..
			if (NULL == pBPC)
			{
				this->MsgSend_UserInRequest(iID); // User 정보가 없을 경우 요청한다..
				return false;
			}
			else if(pBPC->IsDead())
				return false;
			
			pBPC->MoveTo(fX, fY, fZ, 0, 0);	// 현재 위치..
			pBPC->PositionSet(__Vector3(fX, fY, fZ), true);
		}
		return false;
	}

	if(iID == s_pPlayer->IDNumber())	// 내 매시지면..
	{
		s_pPlayer->m_vPosFromServer.Set(fX, fY, fZ);
		return false;
	}

	CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, true);	// 다른 플레이어를 얻어서..
	if (NULL == pUPC) return false;

//	if(0.0f != fSpeed) TRACE("Move - %4.1f ( %d : Mode %d ) %.2f초\n", fSpeed, iID, byMoveFlag, CN3Base::TimeGet());
//	else TRACE("Stop - %4.1f ( %d : Mode %d )  %.2f초\n", fSpeed, iID, byMoveFlag, CN3Base::TimeGet());

	//	서버로 부터터 받은 정보로 업데이트..
	pUPC->MoveTo(fX, fY, fZ, fSpeed, byMoveFlag);	// 현재 위치..

	return true;
}

bool CGameProcMain::MsgRecv_Rotation(DataPack* pDataPack, int& iOffset)
{
	int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	float fYaw		= (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/100.0f;

	if(s_pPlayer->IDNumber() == iID) return false;

	CPlayerNPC* pBPC = NULL;
	pBPC = s_pOPMgr->CharacterGetByID(iID, true);	// 다른 유저 얻어서..
	if ( NULL == pBPC ) return false;

	pBPC->RotateTo(fYaw, false);	// 서버로 부터 받은 x, y, z값으로 

	return true;
}

/*
bool CGameProcMain::MsgRecv_Dead(DataPack* pDataPack, int& iOffset)
{
	int iID	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
//	TRACE("Dead message Receive.. !!!!!!!! %d \n", iID);

	if ( iID == s_pPlayer->IDNumber() )
	{
		s_pPlayer->Action(PSA_PRE_DYING, true, NULL, true); // 플레이어 죽이기.
	}
	else
	{
		CPlayerOther* pReadyToDead = s_pOPMgr->PlayerGetByID(iID, true); // 살아 있는 넘들중에서 검색해보고..
		if(pReadyToDead) // 있으면 죽기직전으로 만든다.
		{
			pReadyToDead->Acttion(PSA_PRE_DYING, true, NULL, true);
		}
	}

	return true;
}
*/


bool CGameProcMain::MsgRecv_Regen(DataPack* pDataPack, int& iOffset)
{
	__Vector3 vPosPlayer;
	vPosPlayer.x = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	vPosPlayer.z = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	vPosPlayer.y = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;
	
	this->InitPlayerPosition(vPosPlayer); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
	s_pPlayer->RegenerateCollisionMesh(); // 충돌 메시를 다시 만든다..

	s_pPlayer->m_iSendRegeneration = 0; // 한번 보내면 다시 죽을때까지 안보내는 플래그
	s_pPlayer->m_fTimeAfterDeath = 0; // 한번 보내면 다시 죽을때까지 안보내는 플래그
	TRACE("받음 - 다시 살아나기(%.1f, %.1f)\n", vPosPlayer.x, vPosPlayer.z);

	//
	//마법 & 효과 초기화..
	if(m_pUIStateBarAndMiniMap) m_pUIStateBarAndMiniMap->ClearMagic();
	if(m_pMagicSkillMng) m_pMagicSkillMng->ClearDurationalMagic();
	if(CGameProcedure::s_pFX) s_pFX->StopMine();

	CLogWriter::Write("Receive Regeneration");

	if(s_pPlayer->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(), -1);
	else if(s_pPlayer->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(), -1);

	return true;
}

bool CGameProcMain::MsgRecv_Time(DataPack* pDataPack, int& iOffset)
{
	short year	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short month = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short day	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short hour	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short min	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	ACT_WORLD->SetGameTimeWithSky(year, month, day, hour, min);

	return true;
}

bool CGameProcMain::MsgRecv_Weather(DataPack* pDataPack, int& iOffset)
{
	int iWeather = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // -> byte - 기후.... 0x01 - 맑음.. 0x02 -  비 0x03
	int iPercent = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // short -> 맑은날 안개, 비, 눈 의 양 퍼센트로 

	
	////////////////////////////////////////////////////
	// 스피드 핵 체크용 시간....
/*	float fTimeFromServer = CAPISocket::Parse_GetFloat(pDataPack->m_pData, iOffset); // float -> 정기적으로 주는 시간...
	float fTimeLocal = CN3Base::TimeGet();
	
	static float fTimeFromServerPrev = fTimeFromServer;
	static float fTimeLocalPrev = fTimeLocal;
	static int iTSH = 0; // Trying SpeedHacking...
	if(fTimeFromServer != 0.0f)
	{
		float fTDS = fTimeFromServer - fTimeFromServerPrev;
		if(fTDS > 355.0f && fTDS < 365.0f) // 6분에 한번 주게끔 되어 있다..
		{
			float fTDL = fTimeLocal - fTimeLocalPrev;
			if(fTDL < (fTDS * 0.8f) || fTDL > (fTDS * 1.2f)) // 로컬 타임이 서버 타임보다 20퍼센트 이상 크면..
			{
				iTSH++; // Try Speed Hacking ... ++
			}
			else 
			{
				iTSH = 0;
			}

			if(iTSH >= 3) CGameProcedure::s_bUseSpeedHack = true; // 스피드 해킹....
		}

		fTimeFromServerPrev = fTimeFromServer;
		fTimeLocalPrev = fTimeLocal;
	}
	// 스피드 핵 체크용 시간....
	////////////////////////////////////////////////////
*/
	
	if(iPercent < 20) iPercent = 20;
	if(iPercent > 100) iPercent = 100;
	float fPercent = iPercent / 100.0f;

//enum {	GEW_CLEAR=0,		// 맑음
//		GEW_DRIZZLE_RAIN=1,		// 가랑비
//		GEW_RAINY=2,			// 적당량의 비
//		GEW_HEAVY_RAIN=3,		// 폭우
//		GEW_SNOW1=11,			// 포근한 눈
//		GEW_SNOW2=12,			// 적당한 눈
//		GEW_HEAVY_SNOW=13		// 폭설
	TRACE("받음 - 날씨(%d - %d)\n", iWeather, iPercent);

	ACT_WORLD->SetWeatherWithSky((CN3SkyMng::eSKY_WEATHER)iWeather,iPercent);	// 하늘 변화하기
	float fDelta = 1.0f;
	if(0x1 == iWeather) fDelta = 0.5f + (1.0f-fPercent)*0.5f; // 맑은 날씨. 퍼센트는 안개...
	else if(0x02 == iWeather) fDelta = 0.25f + (1.0f - fPercent)*0.75f; // 비옴 .
	else if(0x03 == iWeather) fDelta = 0.25f + (1.0f - fPercent)*0.75f; // 눈옴..

	s_pEng->FarPlaneDeltaSet(fDelta, false);

	return true;
}

bool CGameProcMain::MsgRecv_UserInOut(DataPack* pDataPack, int& iOffset)
{
	int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);//CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	if ( 0x01 == iType ) // 유저 들어올때.(원래 게임상의 유저 지역 업뎃하면서 갱신..)
		this->MsgRecv_UserIn(pDataPack, iOffset, false);
	else if ( 0x02 == iType ) // User 나갈때.
		this->MsgRecv_UserOut(pDataPack, iOffset);
	else if ( 0x03 == iType ) // 유저 들어올때 (죽었다 살아나거나, game start하는 유저들.)
		this->MsgRecv_UserIn(pDataPack, iOffset, true);
	else if ( 0x04 == iType ) // 유저 들어올때 (warp)
		this->MsgRecv_UserIn(pDataPack, iOffset, false);
	
	return true;
}

bool CGameProcMain::MsgRecv_UserIn(DataPack* pDataPack, int& iOffset, bool bWithFX)
{
	int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	std::string szName;
	int iNameLen	= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szName, iNameLen );

	e_Nation eNation =				(e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 소속 국가. 0 이면 없다. 1

	// 기사단 관련
	int iKnightsID =					CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 기사단 ID
	e_KnightsDuty eKnightsDuty =	(e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 소속 국가. 0 이면 없다. 1

	// NOTE(srmeier): adding alliance ID
	short sAllianceID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	int iKnightNameLen = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 소속 기사단 이름 길이.
	std::string szKnightsName;
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iKnightNameLen);
	int iKnightsGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);			// 등급
	int iKnightsRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);			// 순위

	// NOTE(srmeier): adding mark version and cape ID
	short sMarkVersion = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short sCapeID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	int iLevel =					CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 레벨...
	e_Race eRace =					(e_Race)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	e_Class eClass =				(e_Class)CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	float fXPos	=					(CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fZPos	=					(CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fYPos	=					(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;
	
	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos);	// 지형의 높이값 얻기..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(__Vector3(fXPos, fYPos, fZPos), 1.0f); // 오브젝트에서 가장 가까운 높이값 얻기..
	if (fYObject > fYTerrain) fYPos = fYObject;
	else fYPos = fYTerrain;

	int iFace = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 머리카락..
	int iHair = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 얼굴 모양

	int iStatus = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 1 - 서있기.. 2 - 앉아있기.. 3 ... 죽어있다..

	// NOTE(srmeier): updating status
	//int iStatusSize = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0 - 보통 크기, 1 - 커져 있다. 2 - 작아졌다..
	int iStatusSize = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

	int iRecruitParty = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 1 - 보통. 2 - 파티 구함..
	BYTE byAuthority = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 권한...

	// NOTE(srmeier): adding is party leader, invisibilitytype, direction, chicken, rank, knight rank, personal rank
	bool bPartyLeader = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	BYTE bInvisibilityType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	short sDirection = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	bool bIsChicken = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	BYTE bRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	BYTE m_bKnightsRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	BYTE m_bPersonalRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);


	DWORD	dwItemIDs[MAX_ITEM_SLOT_OPC];  // 착용 아이템 - 다른 플레이어(NPC 포함) 0 ~ 4 상체,하체,헬멧,팔,발 5 망토 6 오른손 7 왼손
	int		iItemDurabilities[MAX_ITEM_SLOT_OPC]; // 착용 아이템의 내구력..
	for(int i = 0; i < MAX_ITEM_SLOT_OPC; i++)
	{
		dwItemIDs[i] = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);			// 착용하고 있는 아이템들의 ID
		iItemDurabilities[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	// 착용하고 있는 아이템들의 현재 내구력

		// NOTE(srmeier): adding bFlag, probably the rental thing
		BYTE bFlag = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	}

	// NOTE(srmeier): adding the zone id
	BYTE bZoneID = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);


	if ( iID == s_pPlayer->IDNumber()) 
		return false; 									// 내 패킷이면 .. // 무시한다..

	CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
	if(pUPC) // 이미 아이디 같은 캐릭이 있으면..
	{
		CLogWriter::Write("User In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)", iID, szName.c_str(), fXPos, fYPos, fZPos);
		TRACE("User In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)\n", iID, szName.c_str(), fXPos, fYPos, fZPos);

		// TEMP(srmeier): need to figure out what is going on here and how to fix it
		// commenting out what's below to keep the OtherPlayer in the manager...

		// NOTE(srmeier): probably shouldn't be requesting this player's info if it's already in the manager...

		//pUPC->Action(PSA_BASIC, true, NULL, true); // 강제로 살리고..
		//pUPC->m_fTimeAfterDeath = 0;
		//pUPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);

		return false;
	}
	
	D3DCOLOR crID = (eNation != s_pPlayer->m_InfoBase.eNation) ? D3DCOLOR_XRGB(255,96,96) : D3DCOLOR_XRGB(128,128,255); // 국가에 따라 다른색 적용

	pUPC = new CPlayerOther();
	pUPC->IDSet(iID, szName, crID);
	pUPC->m_InfoBase.eNation = eNation;
	pUPC->m_InfoBase.eClass = eClass;
	pUPC->m_InfoBase.iLevel = iLevel;
	pUPC->m_InfoBase.iAuthority = byAuthority;
	pUPC->Init(eRace, iFace, iHair, dwItemIDs, iItemDurabilities);
	pUPC->RotateTo(D3DXToRadian(rand()%360), true);
	pUPC->KnightsInfoSet(iKnightsID, szKnightsName, iKnightsGrade, iKnightsRank);

	//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(iKightsID);
	//if(pKIB) pUPC->KnightsNameSet(pKIB->szName, 0xffff0000);

	pUPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);			// 다른 플레이어 현재 위치 셋팅..
	pUPC->MoveTo(fXPos, fYPos, fZPos, 0, 0);					// 현재 위치..
	pUPC->RotateTo(D3DXToRadian(rand()%360), true);
	s_pOPMgr->UPCAdd(pUPC);										// 캐릭터 추가...

	//if(bWithFX)
	if(bWithFX && pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER)
	{
		if(eNation==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_KARUS, iID, -1);
		else if(eNation==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_REGEN_ELMORAD, iID, -1);
	}

	// 앉아 있기.., 서있기 결정..
	if(1 == iStatus) // 서있기..
	{
		pUPC->Action(PSA_BASIC, true, NULL, true);
		pUPC->ActionMove(PSM_STOP);
	}
	else if(2 == iStatus) // 앉아 있기..
	{
		pUPC->Action(PSA_SITDOWN, true, NULL, true);
		pUPC->ActionMove(PSM_STOP);
	}
	else if(3 == iStatus) // 죽어 있다..
	{
		s_pOPMgr->MoveToCorpsesForcely(pUPC, false); // 시체로 옮긴다..
	}

	if(0x01 == iStatusSize)
	{
		pUPC->ScaleSetGradually(1.0f); // 보통 크기..
		pUPC->FlickerFactorSet(1.0f); // 부활되서 반투명.. 깜박거린다..
	}
	else if(0x02 == iStatusSize) pUPC->ScaleSetGradually(2.0f); // 커졌다..
	else if(0x03 == iStatusSize) pUPC->ScaleSetGradually(0.5f);// 작아졌다.
	else if(0x04 == iStatusSize) pUPC->FlickerFactorSet(0.7f); // 부활되서 반투명.. 깜박거린다..

	if(1 == iRecruitParty)
	{
		pUPC->InfoStringSet("", 0);
	}
	else if(2 == iRecruitParty) // 파티 구함..
	{
		int iLevel = pUPC->m_InfoBase.iLevel;
		int iLMin = iLevel - 8;
		if(iLMin < 0) iLMin = 0;
		int iLMax = iLevel + 8;
		if(iLMax > 60) iLMax = 60;

		char szBuff[128];
		std::string szMsg;
		szMsg = "IDS_WANT_PARTY_MEMBER";//::_LoadStringFromResource(IDS_WANT_PARTY_MEMBER, szMsg);
		sprintf(szBuff, szMsg.c_str(), iLMin, iLMax);
		pUPC->InfoStringSet(szBuff, 0xff00ff00);
	}

	return true;
}

bool CGameProcMain::MsgRecv_UserOut(DataPack* pDataPack, int& iOffset)
{
	int iID	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	//죽은 상태의 캐릭터를 시체로 만든다.
	CPlayerNPC* pUPC = s_pOPMgr->UPCGetByID(iID, false);
	if( pUPC == NULL ) return false;
	if( pUPC->IsDead() )
	{//캐릭터가 죽은 상태에서 UserOut을 하게 되면 시체로 바꿔준다.
		s_pOPMgr->CorpseAdd(iID);
		return true;
	}

	return s_pOPMgr->UPCDelete(iID);					// 캐릭터 제거...	
}

// 주위 영역의 모든 아이디를 카운트만큼 받는다... 글구.. 업데이트가 필요한 것만 서버에게 요청..
bool CGameProcMain::MsgRecv_UserInAndRequest(DataPack* pDataPack, int& iOffset)
{
	int iUPCCountReceived = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
//	TRACE("UPC region update : %d\n", iUPCCountReceived);
	
	if(0 == iUPCCountReceived) return false;
	if(iUPCCountReceived < 0 || iUPCCountReceived >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "영역 요청 오류 - 플레이어 갯수 %d", iUPCCountReceived);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

	int iID = 0;
	m_SetUPCID.clear(); // 싹 지우고...
	it_ID itID, itIDEnd = m_SetUPCID.end();
	pair_ID pairID;

	for ( int i = 0; i < iUPCCountReceived; i++ )
	{
		iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
//		TRACE("               ID : %d\n", iID);
		if(iID == s_pPlayer->IDNumber()) continue; // 내 아이디이다.. 고로 넣으면 안된다.

		pairID = m_SetUPCID.insert(iID);
	}

	if(m_SetUPCID.empty()) // 새로 받은게 한개도 없다면 몽땅 날린다..
	{
		s_pOPMgr->ReleaseUPCs();
		return false;
	}

	// 새로 받은 아이디와 리스트에 있는 UPC ID 를 검색해서..
	CPlayerOther* pUPC = NULL;
	it_UPC itUPC = s_pOPMgr->m_UPCs.begin(), itUPCEnd = s_pOPMgr->m_UPCs.end();
	for(; itUPC != itUPCEnd; )
	{
		pUPC = itUPC->second;
		iID = pUPC->IDNumber();
		
		itID = m_SetUPCID.find(iID);
		if(itID != itIDEnd) // 새로 들어온 리스트에 있으면.. 
		{
			m_SetUPCID.erase(itID); // 요청할 리스트에서 빼고..
			itUPC++;
		}
		else  // 새로 들어온곳에 없으면 지운다..
		{
//			TRACE("           delete : %d\n", iID);

			if(pUPC && pUPC->IsDead()) 
			{
				itUPC++;
			}
			else
			{
				delete pUPC; pUPC = NULL;
				itUPC = s_pOPMgr->m_UPCs.erase(itUPC);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// 바로 요청 패킷을 만들어 보낸다..
	int iNewUPCCount = m_SetUPCID.size();
	if(iNewUPCCount > 0)
	{
		int iOffset=0;														// 버퍼의 오프셋..
		std::vector<BYTE> byBuff(iNewUPCCount * 2 + 10, 0);					// 패킷 버퍼..
		CAPISocket::MP_AddByte(&(byBuff[0]), iOffset, N3_REQUEST_USER_IN);	// 커멘드.
		CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iNewUPCCount);		// 아이디 갯수..
		
		itID = m_SetUPCID.begin(); itIDEnd = m_SetUPCID.end();
		for(int i = 0; itID != itIDEnd; itID++, i++)
		{
			iID = *itID;
			CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iID);			// 자세한 정보가 필요한 아이디들..
		}
		s_pSocket->Send(&(byBuff[0]), iOffset); // 보낸다
	}
	// 바로 요청 패킷을 만들어 보낸다..
	////////////////////////////////////////////////////////////////////////////

	return true;
}

bool CGameProcMain::MsgRecv_UserInRequested(DataPack* pDataPack, int& iOffset)
{
	int iPlayerCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	if(0 == iPlayerCount) return false;
	if(iPlayerCount < 0 || iPlayerCount >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "영역정보 받기 오류 - 플레이어 갯수 %d", iPlayerCount);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

#ifdef _DEBUG
	float fTime = CN3Base::TimeGet();
#endif

	//	int iOffset2 = iOffset;
	for ( int i = 0; i < iPlayerCount; i++ )
	{
		this->MsgRecv_UserIn(pDataPack, iOffset); // 플레이어 갯수 만큼 유저 인...
	}

#ifdef _DEBUG
	TRACE("  User In Requested (Count:%d) : Avg(%.4f Sec)\n", iPlayerCount, (CN3Base::TimeGet() - fTime)/iPlayerCount);
#endif

	return true;
}


bool CGameProcMain::MsgRecv_NPCInOut(DataPack* pDataPack, int& iOffset)
{
	BYTE byType		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	if ( byType == 0x01 ) // NPC 들어올때
	{
		return this->MsgRecv_NPCIn(pDataPack, iOffset); // NPC In 처리
	}
	else // NPC 나갈때.
	{
		return this->MsgRecv_NPCOut(pDataPack, iOffset);
	}

	return true;
}

bool CGameProcMain::MsgRecv_NPCIn(DataPack* pDataPack, int& iOffset)
{
	int		iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // Server에서 관리하는 고유 ID
	int		iIDResrc	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 리소스 ID
	int		iType		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// NPC Type - 0x05 : 상인
	int		iItemTrdeID	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);	// 아이템 거래할 그룹 ID 서버에 요청할 ID
	int		iScale		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 스케일 100 은 1.0 
	int		iItemID0	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 리소스 ID
	int		iItemID1	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 리소스 ID
	int 	iNameLen	= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	std::string szName;									// NPC 아이디..
	if(iNameLen > 0) CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szName, iNameLen );
	else szName = "";

//#ifdef _DEBUG
//	CLogWriter::Write("NPC In - ID(%d) Name(%s) Time(%.1f)", iID, szName.c_str(), CN3Base::TimeGet()); // 캐릭 세팅..
//#endif

	e_Nation eNation = (e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 소속 국가. 0 이면 없다. 1
	int iLevel		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	float fXPos	= (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fZPos	= (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fYPos	= (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;

	float fYTerrain = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos);	// 지형의 높이값 얻기..
	float fYObject = ACT_WORLD->GetHeightNearstPosWithShape(__Vector3(fXPos, fYPos, fZPos), 1.0f); // 오브젝트에서 가장 가까운 높이값 얻기..
	if (fYObject > fYTerrain) fYPos = fYObject;
	else fYPos = fYTerrain;

	DWORD dwStatus =	CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); // 상태... 여러가지로 or 연산해서 쓴다. 0 문 열림, 1 닫힘. 2, 4, 8, 16 ....
	DWORD dwType =		CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 타입... 0 이면 캐릭터 타입 NPC, 1 이면 오브젝트 타입 NPC


	// NOTE(srmeier): adding the unknown bytes and the direction
	short sIDK0 = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	short sIDK1 = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	BYTE byDirection = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);


	CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(iID, false);
	if(pNPC) // 이미 아이디 같은 캐릭이 있으면..
	{
		CLogWriter::Write("NPC In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)", iID, szName.c_str(), fXPos, fYPos, fZPos);
		TRACE("NPC In - Duplicated ID (%d, %s) Pos(%.2f,%.2f,%.2f)\n", iID, szName.c_str(), fXPos, fYPos, fZPos);
		pNPC->Action(PSA_BASIC, true, NULL, true); // 강제로 살리고..
		pNPC->m_fTimeAfterDeath = 0;
		pNPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);
		return false;
	}

	D3DCOLOR crID = (eNation != s_pPlayer->m_InfoBase.eNation) ? D3DCOLOR_XRGB(255,128,128) : D3DCOLOR_XRGB(192,192,255);

	pNPC = new CPlayerNPC();
	pNPC->IDSet(iID, szName, crID);				// 초기화.. 및 ID 세팅.
	pNPC->m_InfoBase.eNation = eNation;
	pNPC->m_InfoBase.eRace = RACE_NPC;			// NPC 라는 걸 알린다.
	pNPC->m_InfoBase.iLevel = iLevel;
	pNPC->m_InfoBase.iAuthority = AUTHORITY_NPC;// 권한 NPC는 권한이 없고..
	
	s_pOPMgr->NPCAdd(pNPC);						// 캐릭터 추가...

	// 이제 패킷에 따라 캐릭터를 치장..(?) 시켜준다.. 아이템장착, 무기 장착등...
	CN3Shape* pShape = NULL;
	__TABLE_PLAYER_LOOKS* pLooks = NULL;
	if(0 == dwType)
	{
		pLooks = s_pTbl_NPC_Looks->Find(iIDResrc);	// 기본 스킨..
		if(NULL == pLooks) // 캐릭터 기본 모습 테이블이 없으면... 
		{
			pLooks = s_pTbl_NPC_Looks->GetIndexedData(0);
			char szBuff[256];
			sprintf(szBuff, "Normal NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
			this->MsgOutput(szBuff, 0xffff00ff);
			CLogWriter::Write(szBuff);
		}
	}
	else
	{
		pShape = ACT_WORLD->ShapeGetByIDWithShape(iIDResrc); // Object NPC 에서 찾아 본다...
		if(NULL == pShape)
		{
			char szBuff[256];
			sprintf(szBuff, "Object NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
			this->MsgOutput(szBuff, 0xffff00ff);
			CLogWriter::Write(szBuff);
			pLooks = s_pTbl_NPC_Looks->GetIndexedData(0);
		}
	}
		
	if(pLooks)
	{
		pNPC->InitChr(pLooks);	// 관절 세팅..
		pNPC->ScaleSet(iScale / 100.0f);

		std::string szPartFN;
		e_PartPosition eParts[6] = { PART_POS_UPPER, PART_POS_LOWER, PART_POS_FACE, PART_POS_HANDS, PART_POS_FEET, PART_POS_HAIR_HELMET };
		for(int i = 0; i < 6; i++)
		{
			pNPC->PartSet(eParts[i], pLooks->szPartFNs[eParts[i]], NULL, NULL);
		}

		if(iItemID0)
		{
			__TABLE_ITEM_BASIC* pItem0 = s_pTbl_Items_Basic->Find(iItemID0/1000*1000);
			__TABLE_ITEM_EXT* pItemExt0 = NULL;
			if(pItem0 && pItem0->byExtIndex >= 0 && pItem0->byExtIndex < MAX_ITEM_EXTENSION)
				pItemExt0 = s_pTbl_Items_Exts[pItem0->byExtIndex]->Find(iItemID0%1000);	// 열 데이터 얻기..
			if(pItem0 && pItemExt0)
			{
				e_PartPosition ePart;
				e_PlugPosition ePlug;
				std::string szItemFN;
				CGameProcedure::MakeResrcFileNameForUPC(pItem0, &szItemFN, NULL, ePart, ePlug, s_pPlayer->m_InfoBase.eRace);
				pNPC->PlugSet(PLUG_POS_RIGHTHAND, szItemFN, pItem0, pItemExt0);
			}
			else
			{
				__ASSERT(0, "Invalid Item ID And Extension");
			}
		}

		if(iItemID1)
		{
			__TABLE_ITEM_BASIC* pItem1 = s_pTbl_Items_Basic->Find(iItemID1/1000*1000);
			__TABLE_ITEM_EXT* pItemExt1 = NULL;
			if(pItem1 && pItem1->byExtIndex >= 0 && pItem1->byExtIndex < MAX_ITEM_EXTENSION)
				pItemExt1 = s_pTbl_Items_Exts[pItem1->byExtIndex]->Find(iItemID1%1000);	// 열 데이터 얻기..
			if(pItem1 && pItemExt1)
			{
				e_PartPosition ePart;
				e_PlugPosition ePlug;
				std::string szItemFN;
				CGameProcedure::MakeResrcFileNameForUPC(pItem1, &szItemFN, NULL, ePart, ePlug, s_pPlayer->m_InfoBase.eRace);
				pNPC->PlugSet(PLUG_POS_LEFTHAND, szItemFN, pItem1, pItemExt1);
			}
			else
			{
				__ASSERT(0, "Invalid Item ID And Extension");
			}
		}
	}
	else if(pShape && (pShape->Type() & OBJ_SHAPE_EXTRA))
	{
		CN3ShapeExtra* pSE = (CN3ShapeExtra*)pShape;
		pNPC->m_pShapeExtraRef = pSE; // 참조 오브젝트 세팅..
		pSE->m_bVisible = true; // 강제로 렌더링하지 않게 하는 플래그 해제..
		
		bool bShouldBeRotate = true;
		__Vector3 vAxis(0,1,0);
		float fRadian = 0, fRadian2 = 0;
		bool bVisible = false;

		if(OBJECT_TYPE_DOOR_LEFTRIGHT == pSE->m_iEventType) // 좌우열림 성문
		{
			vAxis.Set(0,1,0);
			fRadian = D3DXToRadian(80);
			fRadian2 = 0;
		} 
		else if(OBJECT_TYPE_DOOR_TOPDOWN == pSE->m_iEventType)
		{
			vAxis.Set(0,0,1);
			fRadian = D3DXToRadian(90);
			fRadian2 = 0;
		} // 상하열림 성문
		else if(OBJECT_TYPE_LEVER_TOPDOWN == pSE->m_iEventType)
		{
			vAxis.Set(1,0,0);
			fRadian = D3DXToRadian(45);
			fRadian2 = D3DXToRadian(-45);
		} // 상하 레버
		else if(OBJECT_TYPE_FLAG == pSE->m_iEventType)
		{
			bShouldBeRotate = false;
		}

		if(0x01 == dwStatus) // 보이게 만들거나 좌우로 열려 있는 경우..
		{
			if(bShouldBeRotate) // 열기
			{
				pSE->RotateTo(0, vAxis, fRadian, 1, true); // 바로 열기.
				pSE->RotateTo(1, vAxis, -fRadian, 1, true); // 바로 열기.
			}
			else // 보이기
			{
				pSE->m_bVisible = true;
			}
		}
		else if(0x00 == dwStatus) // 안보이게 만들거나 닫혀있다..
		{
			if(bShouldBeRotate) // 닫기
			{
				pSE->RotateTo(0, vAxis, fRadian2, 1, true); // 바로 열기.
				pSE->RotateTo(1, vAxis, -fRadian2, 1, true); // 바로 열기.
			}
			else // 안보이기.
			{
				pSE->m_bVisible = false;
			}
		}
	}
	else
	{
		char szBuff[256];
		sprintf(szBuff, "No Resource NPC In : [Name(%s), ResourceID(%d)]", szName.c_str(), iIDResrc);
		CLogWriter::Write(szBuff);
	}

	pNPC->PositionSet(__Vector3(fXPos, fYPos, fZPos), true);	// 현재 위치 셋팅..
	pNPC->MoveTo(fXPos, fYPos, fZPos, 0, 0);					// 현재 위치..
	pNPC->RotateTo(D3DXToRadian(rand()%360), false);
	pNPC->Action(PSA_BASIC, true, NULL, true);
	pNPC->ActionMove(PSM_STOP);

	return true;
}

bool CGameProcMain::MsgRecv_NPCOut(DataPack* pDataPack, int& iOffset)
{
	int  iID		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // Server에서 관리하는 고유 ID
	return s_pOPMgr->NPCDelete(iID);										// 캐릭터 제거...	
}


// 주위 영역의 모든 아이디를 카운트만큼 받는다... 글구.. 업데이트가 필요한 것만 서버에게 요청..
bool CGameProcMain::MsgRecv_NPCInAndRequest(DataPack* pDataPack, int& iOffset)
{
	int iNPCCountReceived = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	TRACE("NPC region update : %d\n", iNPCCountReceived);

	if(0 == iNPCCountReceived) return false;
	if(iNPCCountReceived < 0 || iNPCCountReceived >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "영역정보 요청 오류 - NPC 갯수 %d", iNPCCountReceived);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

	int iID = 0;
	m_SetNPCID.clear(); // 싹 지우고...
	it_ID itID, itIDEnd = m_SetNPCID.end();
	pair_ID pairID;

	for ( int i = 0; i < iNPCCountReceived; i++ )
	{
		iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		pairID = m_SetNPCID.insert(iID);
		TRACE("               ID : %d\n", iID);
	}

	if(m_SetNPCID.empty()) // 새로 받은게 한개도 없다면 몽땅 날린다..
	{
		s_pOPMgr->ReleaseNPCs();
		return false;
	}

	// 새로 받은 아이디와 리스트에 있는 NPC ID 를 검색해서..
	CPlayerNPC* pNPC = NULL;
	it_NPC itNPC = s_pOPMgr->m_NPCs.begin(), itNPCEnd = s_pOPMgr->m_NPCs.end();
	for(; itNPC != itNPCEnd; )
	{
		pNPC = itNPC->second;
		iID = pNPC->IDNumber();
		
		itID = m_SetNPCID.find(iID);
		if(itID != itIDEnd) // 새로 들어온 리스트에 있으면.. 
		{
			m_SetNPCID.erase(itID); // 요청할 리스트에서 빼고..
			itNPC++;
		}
		else  // 새로 들어온곳에 없으면 지운다..
		{
			TRACE("           delete : %d\n", iID);

			if(pNPC && pNPC->IsDead()) 
			{
				itNPC++;
			}
			else
			{
				delete pNPC; pNPC = NULL;
				itNPC = s_pOPMgr->m_NPCs.erase(itNPC);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// 바로 요청 패킷을 만들어 보낸다..
	int iNewNPCCount = m_SetNPCID.size();
	if(iNewNPCCount > 0)
	{
		int iOffset=0;														// 버퍼의 오프셋..
		std::vector<BYTE> byBuff(iNewNPCCount * 2 + 10, 0);					// 패킷 버퍼..
		CAPISocket::MP_AddByte(&(byBuff[0]), iOffset, N3_REQUEST_NPC_IN);	// 커멘드.
		CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iNewNPCCount);		// 아이디 갯수..
		
		itID = m_SetNPCID.begin(); itIDEnd = m_SetNPCID.end();
		for(int i = 0; itID != itIDEnd; itID++, i++)
		{
			iID = *itID;
			CAPISocket::MP_AddShort(&(byBuff[0]), iOffset, iID);			// 자세한 정보가 필요한 아이디들..
		}
		s_pSocket->Send(&(byBuff[0]), iOffset); // 보낸다
	}
	// 바로 요청 패킷을 만들어 보낸다..
	////////////////////////////////////////////////////////////////////////////

	return true;
}

bool CGameProcMain::MsgRecv_NPCInRequested(DataPack* pDataPack, int& iOffset)
{
	int iNPCCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	if(0 == iNPCCount) return false;
	if(iNPCCount < 0 || iNPCCount >= 1000)
	{
		char szErr[256];
		sprintf(szErr, "영역정보 받기오류 - NPC 갯수 %d", iNPCCount);
		CGameProcedure::ReportDebugStringAndSendToServer(szErr);
		__ASSERT(0, szErr);
		return false;
	}

#ifdef _DEBUG
	float fTime = CN3Base::TimeGet();
#endif

	for ( int i = 0; i < iNPCCount; i++ )
	{
		this->MsgRecv_NPCIn(pDataPack, iOffset); // 플레이어 갯수 만큼 유저 인...
	}
	
#ifdef _DEBUG
	TRACE("  NPC In Requested (Count:%d) : Avg(%.4f Sec)\n", iNPCCount, (CN3Base::TimeGet() - fTime)/iNPCCount);
#endif

	return true;
}

bool CGameProcMain::MsgRecv_NPCMove(DataPack* pDataPack, int& iOffset)
{
	int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	float fXPos		= (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fZPos		= (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset))/10.0f;
	float fYPos		= (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;
	float fSpeed	= (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;

	// 함수가 와야 할 부분.. ^^
	CPlayerNPC* pNPC = NULL;
	pNPC = s_pOPMgr->NPCGetByID(iID, true);				// NPC을 ID로서 얻고..
	if ( NULL == pNPC )
	{
		this->MsgSend_NPCInRequest(iID);
		return false;		// 살아있는 NPC가 있으면..
	}

	float fY = ACT_WORLD->GetHeightWithTerrain(fXPos, fZPos);		// 지형 높이값..
	int iMoveMode = (fSpeed > 0) ? 2 : 0;
	pNPC->MoveTo(fXPos, fY, fZPos, fSpeed, iMoveMode);	// NPC 위치 갱신..

	__ASSERT(!(iMoveMode && fSpeed == 0), "Invalid NPC Move Packet");

	return true;
}

bool CGameProcMain::MsgRecv_Attack(DataPack* pDataPack, int& iOffset)
{
	int iType		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01 - 물리적인 공격, 0x02 마법 공격 0x03, 지속 마법 공격
	int iResult		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x00 실패, 0x01 성공
	int iIDAttacker = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 공격한 넘
	int iIDTarget	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 타겟이 되서 공격당한넘.

	if ( iIDAttacker == iIDTarget ) return false;		// 공격자와 피해자가 같은 경우????

	bool bIAmTarget = (iIDTarget == s_pPlayer->IDNumber()) ? true : false;
	bool bIAmAttacker = (iIDAttacker == s_pPlayer->IDNumber()) ? true : false;
	
	CPlayerBase* pAttacker = NULL;
	if ( bIAmAttacker ) pAttacker = s_pPlayer;	// 공격하는 넘이 내 자신이면
	else pAttacker = s_pOPMgr->CharacterGetByID(iIDAttacker, true);
	if(NULL == pAttacker) // 어라 공격하는 넘이 없네??
	{
		if(iIDAttacker > 10000) // NPC 는 1000 이상이다.
			this->MsgSend_NPCInRequest(iIDAttacker); // NPC 정보가 없을 경우 요청한다..
		else if(iIDAttacker < 3000)
			this->MsgSend_UserInRequest(iIDAttacker); // NPC 정보가 없을 경우 요청한다..
		return false; // 공격하는 넘이 없으면 돌아간다.
	}

	CPlayerBase* pTarget = NULL;
	if(bIAmTarget) // 타겟이 나라면..
	{
		this->CommandSitDown(false, false); // 일으켜 세운다.
		if(m_pMagicSkillMng->IsCasting())
		{
			__TABLE_UPC_SKILL* pSkill = s_pTbl_Skill->Find(s_pPlayer->m_dwMagicID);
			if(pSkill)
			{
				int SuccessValue = rand()%100;
				if(SuccessValue >= pSkill->iPercentSuccess) // 스킬 테이블에 있는 확률대로 실패한다..
					s_pPlayer->Action(PSA_BASIC, false, NULL, true); // 캐스팅 취소, 기본동작으로 강제 세팅..
			}
		}
		pTarget = s_pPlayer;
	}
	else // 타겟이 다른넘이면..
	{
		pTarget = s_pOPMgr->CharacterGetByID(iIDTarget, true); //  일단 살아있는 넘들중에서 가져와보고.. 
		if(NULL == pTarget) pTarget = s_pOPMgr->CharacterGetByID(iIDTarget, false); // 없다면 죽어가는 넘도 상관없이 타겟으로 잡고
	}

	if(NULL == pTarget) return false; // 타겟이 없다!!!!

//	pTarget->m_iIDTargetMe = iIDAttacker; // 어떤 놈이 공격하는 거야??

	if(pAttacker != s_pPlayer && pAttacker && pAttacker->IsAlive()) // 공격하는 넘이 내가 아니고 다른 살아있는 넘일때..
	{
		((CPlayerNPC*)pAttacker)->RotateTo(pTarget); // 타겟을 향해서 방향 계산
		pAttacker->m_iIDTarget = iIDTarget; // 타겟 ID 설정..
		if(0x01 == iType) pAttacker->Action(PSA_ATTACK, false, pTarget); // 물리적인 직접 공격..
		else if(0x02 == iType) pAttacker->Action(PSA_SPELLMAGIC, false, pTarget); // 마법 공격..
//		else if(0x03 == iType) pAttacker->Action(PSA_SPELLMAGIC, false, pTarget); // 지속 마법 공격..
	}

	pTarget->m_bGuardSuccess = false; // 방어에 성공했는지에 대한 플래그..
	if(0x0 == iResult) // 공격 실패
	{
		if(pTarget->IsAlive() && 0 == pTarget->m_iSkillStep) // 죽은 넘이 아니고 스킬을 쓰즌 중이 아니면 막는 동작을 한다..
		{
			pTarget->m_bGuardSuccess = true; // 방어에 성공했는지에 대한 플래그..
			pTarget->Action(PSA_GUARD, false);
		}

		if(pAttacker == s_pPlayer) // 공격하는 사람이 플레이어 자신이면..
		{
			char szBuf[128] = "";
			std::string szFmt = "IDS_MSG_FMT_TARGET_ATTACK_FAILED (%s)";
			//::_LoadStringFromResource(IDS_MSG_FMT_TARGET_ATTACK_FAILED, szFmt);
			sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str());
			MsgOutput(szBuf, 0xffffffff);
		}
	}
	else if(0x2 == iResult) // Attack And Dead - 이번 공격으로 죽는다!!!
	{
		if(pTarget == s_pPlayer)
		{
//			if(m_pUIDead) m_pUIDead->SetVisible(true);
			std::string szMsg = "IDS_REGENERATION"; //::_LoadStringFromResource(IDS_REGENERATION, szMsg);
			CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION); // 다시 생성 메시지 보냄..
		}
		
		pTarget->m_fTimeAfterDeath = 0.1f; // 죽기직전..
		if(pTarget->m_pShapeExtraRef) s_pOPMgr->CharacterDelete(pTarget->IDNumber()); // 오브젝트 일때는 없애버린다..!!
	}

	if(pTarget == s_pPlayer) 
	{
		TRACE("player is under of attack (%d)\n", iIDAttacker);
	}

	return true;
}

bool CGameProcMain::MsgRecv_Dead(DataPack* pDataPack, int& iOffset)
{
	int iIDTarget	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 타겟이 되서 공격당한넘.

	
	CPlayerBase* pTarget = NULL;
	if(iIDTarget == s_pPlayer->IDNumber())
	{
		pTarget = s_pPlayer;

//		if(m_pUIDead) m_pUIDead->SetVisible(true);
		std::string szMsg = "IDS_REGENERATION"; //::_LoadStringFromResource(IDS_REGENERATION, szMsg);
		CGameProcedure::MessageBoxPost(szMsg, "", MB_OK, BEHAVIOR_REGENERATION); // 다시 생성 메시지 보냄..
		CLogWriter::Write("Dead!!!");
	}
	else
	{
		pTarget = this->CharacterGetByID(iIDTarget, false);
	}

	if(pTarget)
	{
		if(pTarget->m_pShapeExtraRef) s_pOPMgr->CharacterDelete(pTarget->IDNumber());
		else
		{
			pTarget->m_fTimeAfterDeath = 0.1f; // 죽기직전..
			pTarget->ActionDying(PSD_KEEP_POSITION, __Vector3(0,0,1));
		}

		return true;
	}

	return false;
}

bool CGameProcMain::MsgRecv_ItemMove(DataPack* pDataPack, int& iOffset)
{
	__InfoPlayerMySelf* pInfoExt = &(s_pPlayer->m_InfoExt);
	__InfoPlayerBase* pInfoBase = &(s_pPlayer->m_InfoBase);

	BYTE bResult =		CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 0x01 : true, 0x00 : false..
	if (bResult)
	{
		pInfoExt->iAttack = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		pInfoExt->iGuard =	CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		pInfoExt->iWeightMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		
		pInfoBase->iHPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		pInfoExt->iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		
		// 아이템에 의해 가감된값이다..
		pInfoExt->iStrength_Delta =		CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iStamina_Delta	=	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iDexterity_Delta =	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iIntelligence_Delta =	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iMagicAttak_Delta =	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

		pInfoExt->iRegistFire =		CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iRegistCold =		CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iRegistLight =	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iRegistMagic =	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iRegistCurse =	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		pInfoExt->iRegistPoison	=	CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

		if(pInfoBase->iHP > pInfoBase->iHPMax) pInfoBase->iHP = pInfoBase->iHPMax; // 범위검사..
		if(pInfoExt->iMSP > pInfoExt->iMSPMax) pInfoExt->iMSP = pInfoExt->iMSPMax; // 범위검사..

		m_pUIVar->m_pPageState->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax);
		m_pUIVar->m_pPageState->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax);

		m_pUIStateBarAndMiniMap->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax, false);
		m_pUIStateBarAndMiniMap->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax, false);
		
		m_pUIVar->m_pPageState->UpdateAttackPoint(pInfoExt->iAttack, pInfoExt->iAttack_Delta);
		m_pUIVar->m_pPageState->UpdateGuardPoint(pInfoExt->iGuard, pInfoExt->iGuard_Delta);
		m_pUIVar->m_pPageState->UpdateWeight(pInfoExt->iWeight, pInfoExt->iWeightMax);
		
		m_pUIVar->m_pPageState->UpdateStrength(pInfoExt->iStrength, pInfoExt->iStrength_Delta);
		m_pUIVar->m_pPageState->UpdateStamina(pInfoExt->iStamina, pInfoExt->iStamina_Delta);
		m_pUIVar->m_pPageState->UpdateDexterity(pInfoExt->iDexterity, pInfoExt->iDexterity_Delta);
		m_pUIVar->m_pPageState->UpdateIntelligence(pInfoExt->iIntelligence, pInfoExt->iIntelligence_Delta);
		m_pUIVar->m_pPageState->UpdateMagicAttak(pInfoExt->iMagicAttak, pInfoExt->iMagicAttak_Delta);

		m_pUIVar->m_pPageState->UpdateRegistFire(pInfoExt->iRegistFire, pInfoExt->iRegistFire_Delta);
		m_pUIVar->m_pPageState->UpdateRegistCold(pInfoExt->iRegistCold, pInfoExt->iRegistCold_Delta);
		m_pUIVar->m_pPageState->UpdateRegistLight(pInfoExt->iRegistLight, pInfoExt->iRegistLight_Delta);
		m_pUIVar->m_pPageState->UpdateRegistMagic(pInfoExt->iRegistMagic, pInfoExt->iRegistMagic_Delta);
		m_pUIVar->m_pPageState->UpdateRegistCurse(pInfoExt->iRegistCurse, pInfoExt->iRegistCurse_Delta);
		m_pUIVar->m_pPageState->UpdateRegistPoison(pInfoExt->iRegistPoison, pInfoExt->iRegistPoison_Delta);
	}

	m_pUIInventory->ReceiveResultFromServer(bResult);
	TRACE("Init Inv Move Res %d \n", (int)bResult);

	return true;
}

bool CGameProcMain::MsgRecv_ItemWeightChange(DataPack* pDataPack, int& iOffset)		// 아이템 무게 변화..
{
	__InfoPlayerMySelf* pInfoExt = &(s_pPlayer->m_InfoExt);
	pInfoExt->iWeight = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	m_pUIVar->m_pPageState->UpdateWeight(pInfoExt->iWeight, pInfoExt->iWeightMax);

	return true;
}

bool CGameProcMain::MsgRecv_UserLookChange(DataPack* pDataPack, int& iOffset)
{
	int iID				= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	e_ItemSlot eSlot	= (e_ItemSlot)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	DWORD dwItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
	int iDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	if(iID == s_pPlayer->IDNumber()) return false;
	
	CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
	if(NULL == pUPC) return false;

	__TABLE_ITEM_BASIC* pItem = s_pTbl_Items_Basic->Find(dwItemID/1000*1000);

	__TABLE_ITEM_EXT* pItemExt = NULL;
	if(pItem && pItem->byExtIndex >= 0 && pItem->byExtIndex < MAX_ITEM_EXTENSION)
		pItemExt = s_pTbl_Items_Exts[pItem->byExtIndex]->Find(dwItemID%1000);	// 열 데이터 얻기..
	if(dwItemID && (NULL == pItem || NULL == pItemExt))
	{
		__ASSERT(0, "NULL Item!!!");
		return false;
	}

	e_PartPosition ePartPos = PART_POS_UNKNOWN, ePartPos2 = PART_POS_UNKNOWN;
	e_PlugPosition ePlugPos = PLUG_POS_UNKNOWN, ePlugPos2 = PLUG_POS_UNKNOWN;

	if(ITEM_SLOT_HEAD == eSlot) ePartPos = PART_POS_HAIR_HELMET;
	else if(ITEM_SLOT_UPPER == eSlot) ePartPos = PART_POS_UPPER;
	else if(ITEM_SLOT_LOWER == eSlot) ePartPos = PART_POS_LOWER;
	else if(ITEM_SLOT_GLOVES == eSlot) ePartPos = PART_POS_HANDS;
	else if(ITEM_SLOT_SHOES == eSlot) ePartPos = PART_POS_FEET;
	else if(ITEM_SLOT_HAND_RIGHT == eSlot) ePlugPos = PLUG_POS_RIGHTHAND;
	else if(ITEM_SLOT_HAND_LEFT == eSlot) ePlugPos = PLUG_POS_LEFTHAND;

	if(ePartPos != PART_POS_UNKNOWN) 
	{
		if(dwItemID) // 아이템이 있는 경우
		{
			std::string szItemFN;
			CGameProcedure::MakeResrcFileNameForUPC(pItem, &szItemFN, NULL, ePartPos2, ePlugPos2, s_pPlayer->m_InfoBase.eRace);
			pUPC->PartSet(ePartPos, szItemFN, pItem, pItemExt); // 아이템 붙이기..
			pUPC->DurabilitySet(eSlot, iDurability);
		}
		else
		{
			__TABLE_PLAYER_LOOKS* pLooks = s_pTbl_UPC_Looks->Find(pUPC->m_InfoBase.eRace);	// User Player Character Skin 구조체 포인터..
			if(NULL == pLooks)
			{
				CLogWriter::Write("CGameProcMain::MsgRecv_UserLookChange() - failed find table : Race (%d)", pUPC->m_InfoBase.eRace);
				__ASSERT(pLooks, "failed find table");
			}
			else
			{
				if(PART_POS_HAIR_HELMET == ePartPos) pUPC->InitHair(); // 머리인데 없는 경우는..
				else pUPC->PartSet(ePartPos, pLooks->szPartFNs[ePartPos], NULL, NULL);
			}
		}

		return true;
	}
	else if(ePlugPos != PLUG_POS_UNKNOWN)
	{
		if(dwItemID)
		{
			std::string szItemFN;
			CGameProcedure::MakeResrcFileNameForUPC(pItem, &szItemFN, NULL, ePartPos2, ePlugPos2, s_pPlayer->m_InfoBase.eRace);
			pUPC->PlugSet(ePlugPos, szItemFN, pItem, pItemExt);
			pUPC->DurabilitySet(eSlot, iDurability);
		}
		else
		{
			pUPC->PlugSet(ePlugPos, "", NULL, NULL);
			pUPC->DurabilitySet(eSlot, 0);
		}

		// Sound Range 안쪽이면 사운드를 바꾸어준다..
		__Vector3 vPosPlayer = s_pPlayer->Position();
		float fDist = pUPC->Distance(vPosPlayer);
		if(fDist < SOUND_RANGE_TO_SET) pUPC->SetSoundPlug(pItem); // SOUND_RANGE 안에 있으면.

		return true;
	}
	else
	{
		CLogWriter::Write("CGameProcMain::MsgRecv_UserLookChange() - Unknown Slot(%d) Item(%d)", eSlot, dwItemID);
		return false;
	}

	return false;
}

bool CGameProcMain::MsgRecv_ItemBundleDrop(DataPack* pDataPack, int& iOffset)		// Item 이 필드에 나타나는데에 대한 응답
{
	int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	int iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

	CPlayerBase* pCorpse = s_pOPMgr->NPCGetByID(iID, false);
	if(NULL == pCorpse) pCorpse = s_pOPMgr->CorpseGetByID(iID);
	
	if(pCorpse)
		pCorpse->m_iDroppedItemID = iItemID; // 떨어트린 아이템 아이디 뭉치

	return true;
}

bool CGameProcMain::MsgRecv_ItemBundleOpen(DataPack* pDataPack, int& iOffset)		// 아이템 상자를 열거나 시체를 뒤진다..
{
	DWORD dwItemID = 0;
	int iItemCount = 0;

	POINT ptCur = s_pLocalInput->MouseGetPos();
	m_pUIDroppedItemDlg->EnterDroppedState(ptCur.x, ptCur.y);
	TRACE ("아이템 리스트 도착.. \n");

	for(int i = 0; i < MAX_ITEM_BUNDLE_DROP_PIECE; i++)
	{
		dwItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		iItemCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		// 이부분에 몬스터 아이템창을 열고 준비한다..
		if ( dwItemID )
			m_pUIDroppedItemDlg->AddToItemTable(dwItemID, iItemCount, i);
	}

	m_pUIDroppedItemDlg->InitIconUpdate();

	return true;
}

void CGameProcMain::MsgRecv_ItemRepair(DataPack* pDataPack, int& iOffset)			// Item Repair Result.. 
{
	int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// Trade id
	int iGold   = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);		// Trade id
	m_pUIInventory->ReceiveResultFromServer( iResult, iGold );
}

void CGameProcMain::MsgRecv_ItemCountChange(DataPack* pDataPack, int& iOffset)		// Item Count Change..
{
	int iTotalCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// Trade id

	for( int i = 0; i < iTotalCount; i++ )
	{
		int iDistrict	= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// Trade id
		int iIndex		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// Trade id
		int iID			  = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);		// Trade id
		int	iCount		= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);		// Trade id
		m_pUIInventory->ItemCountChange(iDistrict, iIndex, iCount, iID);
	}	

	if (m_pUISkillTreeDlg) m_pUISkillTreeDlg->UpdateDisableCheck();
	if (m_pUIHotKeyDlg) m_pUIHotKeyDlg->UpdateDisableCheck();
}

void CGameProcMain::MsgRecv_ItemDestroy(DataPack* pDataPack, int& iOffset)			// Item Count Change..
{
	int	iResult		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// Trade id
	m_pUIInventory->ReceiveResultItemRemoveFromServer(iResult);
};

void CGameProcMain::MsgRecv_MyInfo_HP(DataPack* pDataPack, int& iOffset)
{
	int iHPMax =	CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	
	int iHP =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	

	int iHPChange = iHP - s_pPlayer->m_InfoBase.iHP;
	char szBuf[256] = "";
	if(iHPChange < 0)
	{
		std::string szFmt = "%d HP Damage";
		//::_LoadStringFromResource(IDS_MSG_FMT_HP_LOST, szFmt);
		sprintf(szBuf, szFmt.c_str(), -iHPChange);
		MsgOutput(szBuf, 0xffff3b3b);
	}
	else if(iHPChange > 0)
	{
		std::string szFmt = "%d HP Damage";
		//::_LoadStringFromResource(IDS_MSG_FMT_HP_RECOVER, szFmt);
		sprintf(szBuf, szFmt.c_str(), iHPChange);
		MsgOutput(szBuf, 0xff6565ff);
	}

	s_pPlayer->m_InfoBase.iHPMax = iHPMax;
	s_pPlayer->m_InfoBase.iHP = iHP;

	m_pUIVar->m_pPageState->UpdateHP(iHP, iHPMax);
	m_pUIStateBarAndMiniMap->UpdateHP(iHP, iHPMax, false);
}

void CGameProcMain::MsgRecv_MyInfo_MSP(DataPack* pDataPack, int& iOffset)
{
	int iMSPMax = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	
	int iMSP = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	
	int iMSPChange = iMSP - s_pPlayer->m_InfoExt.iMSP;
	
	e_Class_Represent eCR = CGameProcedure::GetRepresentClass(s_pPlayer->m_InfoBase.eClass);
	bool bUseMP = (CLASS_REPRESENT_WARRIOR != eCR && CLASS_REPRESENT_ROGUE != eCR) ? true : false;
	
	char szBuf[256] = "";
	if(iMSPChange < 0)
	{
		std::string szFmt;
		if (bUseMP) szFmt = "IDS_MSG_FMT_MP_USE (%d)";//::_LoadStringFromResource(IDS_MSG_FMT_MP_USE, szFmt);
		else szFmt = "IDS_MSG_FMT_SP_USE (%d)";//::_LoadStringFromResource(IDS_MSG_FMT_SP_USE, szFmt);
		sprintf(szBuf, szFmt.c_str(), -iMSPChange);
		MsgOutput(szBuf, 0xffff3b3b);
	}
	else if(iMSPChange > 0)
	{
		std::string szFmt;
		if (bUseMP) szFmt = "IDS_MSG_FMT_MP_RECOVER (%d)";//::_LoadStringFromResource(IDS_MSG_FMT_MP_RECOVER, szFmt);
		else szFmt = "IDS_MSG_FMT_SP_RECOVER (%d)";//::_LoadStringFromResource(IDS_MSG_FMT_SP_RECOVER, szFmt);
		sprintf(szBuf, szFmt.c_str(), iMSPChange);
		MsgOutput(szBuf, 0xff6565ff);
	}

	s_pPlayer->m_InfoExt.iMSPMax = iMSPMax;
	s_pPlayer->m_InfoExt.iMSP = iMSP;

	m_pUIVar->m_pPageState->UpdateMSP(iMSP, iMSPMax);
	m_pUIStateBarAndMiniMap->UpdateMSP(iMSP, iMSPMax, false);

	if (m_pUISkillTreeDlg) m_pUISkillTreeDlg->UpdateDisableCheck();
	if (m_pUIHotKeyDlg) m_pUIHotKeyDlg->UpdateDisableCheck();
}

void CGameProcMain::MsgRecv_MyInfo_EXP(DataPack* pDataPack, int& iOffset)
{
	Uint64 iExp = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
	Uint64 iExpChange = iExp - s_pPlayer->m_InfoExt.iExp;
	s_pPlayer->m_InfoExt.iExp = iExp;
	m_pUIVar->m_pPageState->UpdateExp(iExp, s_pPlayer->m_InfoExt.iExpNext);
	m_pUIStateBarAndMiniMap->UpdateExp(iExp, s_pPlayer->m_InfoExt.iExpNext, false);

	if(s_pPlayer->m_InfoExt.iLevelPrev == s_pPlayer->m_InfoBase.iLevel && iExpChange != 0)
	{
		char szBuf[256] = "";
		if(iExpChange > 0)
		{
			std::string szFmt = "Earned %d Experience Points";
			//::_LoadStringFromResource(IDS_MSG_FMT_EXP_GET, szFmt);
			sprintf(szBuf, szFmt.c_str(), iExpChange);
		}
		else if(iExpChange < 0)
		{
			std::string szFmt = "IDS_MSG_FMT_EXP_LOST (%d)";
			//::_LoadStringFromResource(IDS_MSG_FMT_EXP_LOST, szFmt);
			sprintf(szBuf, szFmt.c_str(), -iExpChange);
		}
		MsgOutput(szBuf, 0xffffff00);
	}

	s_pPlayer->m_InfoExt.iLevelPrev = s_pPlayer->m_InfoBase.iLevel;
}

bool CGameProcMain::MsgRecv_MyInfo_LevelChange(DataPack* pDataPack, int& iOffset)
{
	int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	int iLevel = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	if(iID == s_pPlayer->IDNumber())
	{
		__InfoPlayerBase*	pInfoBase = &(s_pPlayer->m_InfoBase);
		__InfoPlayerMySelf*	pInfoExt = &(s_pPlayer->m_InfoExt);
		
		int iLevelPrev = pInfoBase->iLevel;
		pInfoBase->iLevel = iLevel;
		pInfoExt->iBonusPointRemain = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 남은 보너스 포인트..

		BYTE	bExtraSkillPoint		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 토탈 포인트
		TRACE("Skill change Extra value %d\n", bExtraSkillPoint);

		int iExpNext	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); 
		int iExp		= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); 

		// 새로 얻은 경험치를 계산해준다..
		int iExpChange = 0;
		if(iLevel > iLevelPrev)
			iExpChange = (pInfoExt->iExpNext - pInfoExt->iExp) + iExp; // 레벨업 한경우..
		else
			iExpChange = -(pInfoExt->iExp + iExpNext - iExp); // 레벨다운 한경우..
		char szBuf[256] = "";
		if(iExpChange > 0)
		{
			std::string szFmt = "Earned %d Experience Points";
			//::_LoadStringFromResource(IDS_MSG_FMT_EXP_GET, szFmt);
			sprintf(szBuf, szFmt.c_str(), iExpChange);
		}
		else if(iExpChange < 0)
		{
			std::string szFmt = "IDS_MSG_FMT_EXP_LOST (%d)";
			//::_LoadStringFromResource(IDS_MSG_FMT_EXP_LOST, szFmt);
			sprintf(szBuf, szFmt.c_str(), -iExpChange);
		}
		MsgOutput(szBuf, 0xffffff00);

			
		pInfoExt->iExpNext	= iExpNext; 
		pInfoExt->iExp		= iExp; 

		pInfoBase->iHPMax =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	
		pInfoBase->iHP =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		pInfoExt->iMSPMax =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		pInfoExt->iMSP =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		pInfoExt->iWeightMax =	CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		pInfoExt->iWeight	=	CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

		m_pUIVar->UpdateAllStates(&(s_pPlayer->m_InfoBase), &(s_pPlayer->m_InfoExt)); // 모든 정보 업데이트..

		m_pUIStateBarAndMiniMap->UpdateExp(pInfoExt->iExp, pInfoExt->iExpNext, true);
		m_pUIStateBarAndMiniMap->UpdateHP(pInfoBase->iHP, pInfoBase->iHPMax, false);
		m_pUIStateBarAndMiniMap->UpdateMSP(pInfoExt->iMSP, pInfoExt->iMSPMax, false);

		m_pUISkillTreeDlg->m_iSkillInfo[0] = bExtraSkillPoint;
		m_pUISkillTreeDlg->InitIconUpdate();		// 레벨이 변화되었으므로 .. 스킬도 추가될 수 있다..

		if(iLevel > iLevelPrev) // 이곳에 Level Up 효과를 넣어준다..
		{
			if(s_pPlayer->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_KARUS, iID, -1);
			else if(s_pPlayer->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_ELMORAD, iID, -1);
		}
	}
	else // 다른 넘이다..
	{
		CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
		if(pUPC)
		{
			if(iLevel > pUPC->m_InfoBase.iLevel)
			{
				if(pUPC->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_KARUS, iID, -1);
				else if(pUPC->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(iID, -1, FXID_LEVELUP_ELMORAD, iID, -1);
			}
			pUPC->m_InfoBase.iLevel = iLevel;
		}
	}

	return TRUE;
}

void CGameProcMain::MsgRecv_MyInfo_RealmPoint(DataPack* pDataPack, int& iOffset)
{
	s_pPlayer->m_InfoExt.iRealmPoint	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset); 
	if(m_pUIVar->m_pPageState)
		m_pUIVar->m_pPageState->UpdateRealmPoint(s_pPlayer->m_InfoExt.iRealmPoint); // 국가 기여도는 10을 나누어서 표시
}

void CGameProcMain::MsgRecv_MyInfo_PointChange(DataPack* pDataPack, int& iOffset)
{
	int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 0x01 : true, 0x00 : false..
	int iVal =	CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	// 0x01 : true, 0x00 : false..

	s_pPlayer->m_InfoBase.iHPMax =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iMSPMax =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iAttack =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	s_pPlayer->m_InfoExt.iWeightMax =	CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

	m_pUIVar->m_pPageState->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax);
	m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, false);
	
	m_pUIVar->m_pPageState->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax);
	m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, false);

	m_pUIVar->m_pPageState->UpdateAttackPoint(s_pPlayer->m_InfoExt.iAttack, s_pPlayer->m_InfoExt.iAttack_Delta);
	m_pUIVar->m_pPageState->UpdateWeight(s_pPlayer->m_InfoExt.iWeight, s_pPlayer->m_InfoExt.iWeightMax);


// 포인트 체인지 -	 보낼때 b1(1힘 2체력 3민첩 4지능 5마력) s(-1 +1)
//					 받을때 b1(1힘 2체력 3민첩 4지능 5마력) s(절대수치)

	if(0x01 == iType) // Strength
	{
		s_pPlayer->m_InfoExt.iStrength = iVal;
		m_pUIVar->m_pPageState->UpdateStrength(s_pPlayer->m_InfoExt.iStrength, s_pPlayer->m_InfoExt.iStrength_Delta);
	}
	else if(0x02 == iType) // Stamina
	{
		s_pPlayer->m_InfoExt.iStamina = iVal;
		m_pUIVar->m_pPageState->UpdateStamina(s_pPlayer->m_InfoExt.iStamina, s_pPlayer->m_InfoExt.iStamina_Delta);
	}
	else if(0x03 == iType) // Dex
	{
		s_pPlayer->m_InfoExt.iDexterity = iVal;
		m_pUIVar->m_pPageState->UpdateDexterity(s_pPlayer->m_InfoExt.iDexterity, s_pPlayer->m_InfoExt.iDexterity_Delta);
	}
	else if(0x04 == iType) // Intelligence
	{
		s_pPlayer->m_InfoExt.iIntelligence = iVal;
		m_pUIVar->m_pPageState->UpdateIntelligence(s_pPlayer->m_InfoExt.iIntelligence, s_pPlayer->m_InfoExt.iIntelligence_Delta);
	}
	else if(0x05 == iType) // Magic Attack
	{
		s_pPlayer->m_InfoExt.iMagicAttak = iVal;
		m_pUIVar->m_pPageState->UpdateMagicAttak(s_pPlayer->m_InfoExt.iMagicAttak, s_pPlayer->m_InfoExt.iMagicAttak_Delta);
	}

	if(iType >= 1 && iType <= 5)
	{
		s_pPlayer->m_InfoExt.iBonusPointRemain--;
		m_pUIVar->m_pPageState->UpdateBonusPointAndButtons(s_pPlayer->m_InfoExt.iBonusPointRemain); // 보너스 포인트 적용이 가능한가??
	}
}

void CGameProcMain::InitUI()
{
	const int iW = CN3Base::s_CameraData.vp.Width;
	const int iH = CN3Base::s_CameraData.vp.Height;

	RECT rc;
	int iX = 0, iY = 0;

	e_Nation eNation = s_pPlayer->m_InfoBase.eNation; // 국가....

	__TABLE_UI_RESRC* pTbl = s_pTbl_UI->Find(eNation);
	if(NULL == pTbl) return;
	
	m_pUICmd->Init(s_pUIMgr);
	m_pUICmd->LoadFromFile(pTbl->szCmd);
	rc = m_pUICmd->GetRegion();
	m_pUICmd->SetPos((iW - (rc.right - rc.left))/2, iH - (rc.bottom - rc.top));
	m_pUICmd->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);

	m_pUIChatDlg->Init(s_pUIMgr);					//Manager 자식으로 리스트에 추가 
	m_pUIChatDlg->LoadFromFile(pTbl->szChat);
	rc = m_pUIChatDlg->GetRegion();
	RECT rcCmd = m_pUICmd->GetRegion(); rcCmd.top += 5; // .. 하드 코딩..
	iX = 0;
	iY = iH - ((rc.bottom - rc.top) + (rcCmd.bottom - rcCmd.top));
	CGameProcedure::UIPostData_Read(UI_POST_WND_CHAT, m_pUIChatDlg, iX, iY);
	m_pUIChatDlg->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
	m_pUIChatDlg->SetVisibleWithNoSound(true);

	m_pUIMsgDlg->Init(s_pUIMgr);
	m_pUIMsgDlg->LoadFromFile(pTbl->szMsgOutput);
	m_pUIMsgDlg->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);

	// 채팅창과 메시지 창 위치 맞추기..
	m_pUIChatDlg->MoveOffset(0, -1);

	m_pUIStateBarAndMiniMap->Init(s_pUIMgr);
	m_pUIStateBarAndMiniMap->LoadFromFile(pTbl->szStateBar);
	m_pUIStateBarAndMiniMap->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
#ifdef _DEBUG
	m_pUIStateBarAndMiniMap->SetPos(0, 70); // 디버그 정보 표시때문에 조금 내린다....
#else
	m_pUIStateBarAndMiniMap->SetPos(0, 0);
#endif

	// 다용도 UI - 상태, 기사단관리, 퀘스트, 친구 관리등...
	m_pUIVar->Init(s_pUIMgr);
	m_pUIVar->LoadFromFile(pTbl->szVarious);
	m_pUIVar->SetVisibleWithNoSound(false);
	m_pUIVar->m_pPageState->LoadFromFile(pTbl->szState);
	m_pUIVar->m_pPageState->SetVisibleWithNoSound(true);
	m_pUIVar->m_pPageKnights->LoadFromFile(pTbl->szKnights);
	m_pUIVar->m_pPageKnights->SetVisibleWithNoSound(false);
	m_pUIVar->m_pPageFriends->LoadFromFile(pTbl->szFriends);
	m_pUIVar->m_pPageFriends->SetVisibleWithNoSound(false);
	m_pUIVar->SetStyle(m_pUIVar->GetStyle() | UISTYLE_POS_LEFT);

	// TODO(srmeier): need to implement this
	m_pUIVar->m_pPageQuest->LoadFromFile(pTbl->szQuest);
	m_pUIVar->m_pPageQuest->SetVisibleWithNoSound(false);


	
	m_pUITargetBar->Init(s_pUIMgr);
	m_pUITargetBar->LoadFromFile(pTbl->szTargetBar);
	m_pUITargetBar->SetStyle(UISTYLE_FOCUS_UNABLE | UISTYLE_HIDE_UNABLE);
	rc = m_pUITargetBar->GetRegion();
	m_pUITargetBar->SetPos((iW - (rc.right - rc.left))/2, 0);
	m_pUITargetBar->SetVisibleWithNoSound(false);

	m_pUIHelp->Init(s_pUIMgr);
	m_pUIHelp->LoadFromFile(pTbl->szHelp);
	CGameProcedure::UIPostData_Read(UI_POST_WND_HELP, m_pUIHelp, 0, 0);
	rc = m_pUIHelp->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIHelp->SetPos(iX, iY);
//	m_pUIHelp->SetStyle(UISTYLE_SHOW_ME_ALONE);
	
	// 공지사항..
	m_pUINotice->Init(s_pUIMgr);
	m_pUINotice->LoadFromFile(pTbl->szNotice);
	m_pUINotice->SetVisibleWithNoSound(false);

	m_pUIClassChange->Init(s_pUIMgr);
	m_pUIClassChange->LoadFromFile(pTbl->szClassChange);
	m_pUIClassChange->SetVisibleWithNoSound(false);
	rc = m_pUIClassChange->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIClassChange->SetPos(iX, iY);
	m_pUIClassChange->SetStyle(UISTYLE_USER_MOVE_HIDE);

	m_pUINpcChange->Init(s_pUIMgr);
	m_pUINpcChange->LoadFromFile(pTbl->szChangeClassInit);
	m_pUINpcChange->SetVisibleWithNoSound(false);
	rc = m_pUINpcChange->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUINpcChange->SetPos(iX, iY);
	m_pUINpcChange->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);

	m_pUINpcEvent->Init(s_pUIMgr);
	m_pUINpcEvent->LoadFromFile(pTbl->szNpcEvent);
	m_pUINpcEvent->SetVisibleWithNoSound(false);
	m_pUINpcEvent->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);
	rc = m_pUINpcEvent->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUINpcEvent->SetPos(iX, iY);

	m_pUINpcTalk->Init(s_pUIMgr);
	m_pUINpcTalk->LoadFromFile(pTbl->szNpcTalk);
	m_pUINpcTalk->SetVisibleWithNoSound(false);
	rc = m_pUINpcTalk->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUINpcTalk->SetPos(iX, iY);

	m_pUIWarp->Init(s_pUIMgr);
	m_pUIWarp->LoadFromFile(pTbl->szZoneChangeOrWarp);
	m_pUIWarp->SetVisibleWithNoSound(false);
	rc = m_pUIWarp->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIWarp->SetPos(iX, iY);
	m_pUIWarp->SetStyle(UISTYLE_USER_MOVE_HIDE | UISTYLE_SHOW_ME_ALONE);

//	m_pUITradeList->Init(s_pUIMgr);
//	m_pUITradeList->LoadFromFile(pTbl->szNpcExchangeList);
//	m_pUITradeList->SetVisibleWithNoSound(false);
//	rc = m_pUITradeList->GetRegion();
//	iX = (iW - (rc.right - rc.left))/2;
//	iY = (iH - (rc.bottom - rc.top))/2;
//	m_pUITradeList->SetPos(iX, iY);

	m_pUIItemREDlg->Init(s_pUIMgr);
	m_pUIItemREDlg->LoadFromFile(pTbl->szExchangeRepair);
	m_pUIItemREDlg->SetVisibleWithNoSound(false);
	rc = m_pUIItemREDlg->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIItemREDlg->SetPos(iX, iY);
	m_pUIItemREDlg->InitIconWnd(UIWND_EXCHANGE_REPAIR);
	m_pUIItemREDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIItemREDlg->SetState(UI_STATE_COMMON_NONE);

	m_pUIRepairTooltip->Init(s_pUIMgr);
	m_pUIRepairTooltip->LoadFromFile(pTbl->szRepairTooltip);
	m_pUIRepairTooltip->SetVisibleWithNoSound(false);
	m_pUIRepairTooltip->InitPos();
	m_pUIRepairTooltip->SetStyle(UISTYLE_ALWAYSTOP|UISTYLE_FOCUS_UNABLE);

	m_pUIPartyOrForce->Init(s_pUIMgr);
	m_pUIPartyOrForce->LoadFromFile(pTbl->szPartyOrForce);
	m_pUIPartyOrForce->SetStyle(UISTYLE_HIDE_UNABLE);
	rc = m_pUIPartyOrForce->GetRegion();
	iX = iW - (rc.right - rc.left);
	m_pUIPartyOrForce->SetPos(iX, 0);
	m_pUIPartyOrForce->SetVisible(false); // 강제로 안보이기~
//	CGameProcedure::UIPostData_Read(UI_POST_WND_PARTY, m_pUIPartyOrForce, iX, 0);

	// Dropped Item Dlg.. 
	m_pUIDroppedItemDlg->Init(s_pUIMgr);
	m_pUIDroppedItemDlg->LoadFromFile(pTbl->szDroppedItem);
	m_pUIDroppedItemDlg->SetVisibleWithNoSound(false);
	m_pUIDroppedItemDlg->InitIconWnd(UIWND_DROPITEM);
	m_pUIDroppedItemDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIDroppedItemDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUIDroppedItemDlg->SetStyle(m_pUIDroppedItemDlg->GetStyle() | UISTYLE_USER_MOVE_HIDE);

	m_pUITransactionDlg->Init(s_pUIMgr);
	m_pUITransactionDlg->LoadFromFile(pTbl->szTransaction);
	rc = m_pUITransactionDlg->GetRegion();
	m_pUITransactionDlg->SetPos(iW - (rc.right-rc.left), 10);
	m_pUITransactionDlg->SetVisibleWithNoSound(false);
	m_pUITransactionDlg->InitIconWnd(UIWND_TRANSACTION);
	m_pUITransactionDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUITransactionDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUITransactionDlg->SetStyle(m_pUITransactionDlg->GetStyle() | UISTYLE_POS_RIGHT | UISTYLE_USER_MOVE_HIDE);

	m_pSubProcPerTrade->InitPerTradeDlg(s_pUIMgr);

	m_pUIInventory->Init(s_pUIMgr);
	m_pUIInventory->LoadFromFile(pTbl->szInventory);
	m_pUIInventory->SetVisibleWithNoSound(false);
	m_pUIInventory->SetPos(465,10);
	m_pUIInventory->InitIconWnd(UIWND_INVENTORY);
	m_pUIInventory->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIInventory->SetState(UI_STATE_COMMON_NONE);
	m_pUIInventory->SetStyle(m_pUIInventory->GetStyle() | UISTYLE_POS_RIGHT);

	// Countable Item Edit Dlg..
	CN3UIWndBase::m_pCountableItemEdit = new CCountableItemEditDlg;
	CN3UIWndBase::m_pCountableItemEdit->Init(s_pUIMgr);
	CN3UIWndBase::m_pCountableItemEdit->LoadFromFile(pTbl->szPersonalTradeEdit);
	CN3UIWndBase::m_pCountableItemEdit->SetStyle(UISTYLE_ALWAYSTOP);
	// 위치 계산 ..
	rc = CN3UIWndBase::m_pCountableItemEdit->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	CN3UIWndBase::m_pCountableItemEdit->SetPos(iX, iY);	
	CN3UIWndBase::m_pCountableItemEdit->SetVisibleWithNoSound(false);
	CN3UIWndBase::m_pCountableItemEdit->SetUIType(UI_TYPE_BASE);
	CN3UIWndBase::m_pCountableItemEdit->SetState(UI_STATE_COMMON_NONE);

	m_pUISkillTreeDlg->Init(s_pUIMgr);
	m_pUISkillTreeDlg->LoadFromFile(pTbl->szSkillTree);
	m_pUISkillTreeDlg->SetVisibleWithNoSound(false);
	rc = m_pUISkillTreeDlg->GetRegion();
	m_pUISkillTreeDlg->SetPos(iW - (rc.right-rc.left), 10);
	m_pUISkillTreeDlg->InitIconWnd(UIWND_SKILL_TREE);
	m_pUISkillTreeDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUISkillTreeDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUISkillTreeDlg->SetStyle(m_pUISkillTreeDlg->GetStyle() | UISTYLE_POS_RIGHT);

	
	// default ui pos ..	해상도가 변경되면.. 상대 위치를 구해야 한다.. by ecli666
	rc = m_pUIStateBarAndMiniMap->GetRegion();
	m_pUIHotKeyDlg->Init(s_pUIMgr);
	m_pUIHotKeyDlg->LoadFromFile(pTbl->szHotKey);
	m_pUIHotKeyDlg->SetStyle(UISTYLE_HIDE_UNABLE);
	CGameProcedure::UIPostData_Read(UI_POST_WND_HOTKEY, m_pUIHotKeyDlg, rc.left, rc.bottom);
	m_pUIHotKeyDlg->SetVisibleWithNoSound(true); // 무조건 보인다!!!
	m_pUIHotKeyDlg->InitIconWnd(UIWND_HOTKEY);
	m_pUIHotKeyDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIHotKeyDlg->SetState(UI_STATE_COMMON_NONE);

	m_pUIKnightsOp->Init(s_pUIMgr);	// 기사단 리스트 보기, 가입, 등...
	m_pUIKnightsOp->LoadFromFile(pTbl->szKnightsOperation);
	m_pUIKnightsOp->SetVisibleWithNoSound(false);

	// 파티 지원 게시판..
	m_pUIPartyBBS->Init(s_pUIMgr);
	m_pUIPartyBBS->LoadFromFile(pTbl->szPartyBBS);//, N3FORMAT_VER_1298);
	m_pUIPartyBBS->SetVisibleWithNoSound(false);
	rc = m_pUIPartyBBS->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIPartyBBS->SetPos(iX, iY);


	m_pUIWareHouseDlg->Init(s_pUIMgr);
	m_pUIWareHouseDlg->LoadFromFile(pTbl->szWareHouse);
	rc = m_pUIWareHouseDlg->GetRegion();
	m_pUIWareHouseDlg->SetPos(iW - (rc.right-rc.left), 10);
	m_pUIWareHouseDlg->SetVisibleWithNoSound(false);
	m_pUIWareHouseDlg->InitIconWnd(UIWND_WARE_HOUSE);
	m_pUIWareHouseDlg->SetUIType(UI_TYPE_ICON_MANAGER);
	m_pUIWareHouseDlg->SetState(UI_STATE_COMMON_NONE);
	m_pUIWareHouseDlg->SetStyle(UISTYLE_USER_MOVE_HIDE | UISTYLE_POS_RIGHT);

	m_pTargetSymbol->LoadFromFile(pTbl->szTargetSymbolShape); // 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..

	m_pUIInn->Init(s_pUIMgr);
	m_pUIInn->LoadFromFile(pTbl->szInn);
	m_pUIInn->SetVisibleWithNoSound(false);
	m_pUIInn->SetStyle(UISTYLE_SHOW_ME_ALONE | UISTYLE_USER_MOVE_HIDE);
	rc = m_pUIInn->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIInn->SetPos(iX, iY);
	
	m_pUICreateClanName->Init(s_pUIMgr);
	m_pUICreateClanName->LoadFromFile(pTbl->szInputClanName);
	m_pUICreateClanName->SetVisibleWithNoSound(false);
	rc = m_pUICreateClanName->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUICreateClanName->SetPos(iX, iY);
	m_pUICreateClanName->SetStyle(UISTYLE_USER_MOVE_HIDE);

	// Quest Menu
	m_pUIQuestMenu->Init(s_pUIMgr);
	m_pUIQuestMenu->LoadFromFile(pTbl->szQuestMenu);
	m_pUIQuestMenu->SetVisibleWithNoSound(false);
	m_pUIQuestMenu->SetStyle(UISTYLE_USER_MOVE_HIDE);

	// Quest Talk
	m_pUIQuestTalk->Init(s_pUIMgr);
	m_pUIQuestTalk->LoadFromFile(pTbl->szQuestTalk);
	m_pUIQuestTalk->SetVisibleWithNoSound(false);
	m_pUIQuestTalk->SetStyle(UISTYLE_USER_MOVE_HIDE);
	rc = m_pUIQuestTalk->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIQuestTalk->SetPos(iX, iY);

	// dead ui
	m_pUIDead->Init(s_pUIMgr);
	m_pUIDead->LoadFromFile(pTbl->szDead);
	m_pUIDead->SetVisibleWithNoSound(false);
	rc = m_pUIDead->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUIDead->SetPos(iX, iY);


	// 상거래 게시판
	m_pUITradeBBS->Init(s_pUIMgr);
	m_pUITradeBBS->LoadFromFile(pTbl->szTradeBBS);
	m_pUITradeBBS->SetVisibleWithNoSound(false);
	m_pUITradeBBS->SetStyle(UISTYLE_USER_MOVE_HIDE);
	rc = m_pUITradeBBS->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUITradeBBS->SetPos(iX, iY);

	// 상거래 종류 선택 
	m_pUITradeBBSSelector->Init(s_pUIMgr);
	m_pUITradeBBSSelector->LoadFromFile(pTbl->szTradeBBSSelector);
	m_pUITradeBBSSelector->SetVisibleWithNoSound(false);
	rc = m_pUITradeBBSSelector->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUITradeBBSSelector->SetPos(iX, iY);
	m_pUITradeBBSSelector->SetStyle(UISTYLE_USER_MOVE_HIDE);

	// 상거래 항목 내용
	m_pUITradeBBSEdit->LoadFromFile(pTbl->szTradeExplanation);
	m_pUITradeBBSEdit->SetVisibleWithNoSound(false);
	rc = m_pUITradeBBSEdit->GetRegion();
	iX = (iW - (rc.right - rc.left))/2;
	iY = (iH - (rc.bottom - rc.top))/2;
	m_pUITradeBBSEdit->SetPos(iX, iY);
}

void CGameProcMain::MsgSend_RequestTargetHP(short siIDTarget, BYTE byUpdateImmediately)
{
	BYTE byBuff[4];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_TARGET_HP);
	CAPISocket::MP_AddShort(byBuff, iOffset, siIDTarget);
	CAPISocket::MP_AddByte(byBuff, iOffset, byUpdateImmediately); // 0x00 - 점차 늘어나게끔.. 0x01 - 즉시 업데이트..

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgRecv_TargetHP(DataPack* pDataPack, int& iOffset)
{
	int iID				= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	// SID
	BYTE byUpdateImmediately = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 0x00 - 점차 늘어나게끔.. 0x01 - 즉시 업데이트..
	int iTargetHPMax	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);  // HP 
	int iTargetHPCur	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);  // HP 
	int iTargetHPChange	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);  // HP 

	if(iTargetHPMax <= 0)
	{
		CPlayerNPC* pTarget = this->s_pOPMgr->CharacterGetByID(iID, true);
		std::string szBuff("Invalid HP Change - 0 ID(");
		if(pTarget) szBuff += pTarget->IDString();
		szBuff += ')';
		CLogWriter::Write(szBuff.c_str());
	}
	__ASSERT(iTargetHPMax > 0, "최대 체력 수치는 0이상이어야 합니다.");

	if(iID == s_pPlayer->m_iIDTarget) // 내가 공격하는 넘이면..
	{
		bool bUI = false;
		if(byUpdateImmediately) bUI = true;
		m_pUITargetBar->UpdateHP(iTargetHPCur, iTargetHPMax, bUI);
	//	TRACE("Target HP Update %d %d\n", iID, iTargetHPPercent);
	}

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(iID, true);
	if(pTarget)
	{
		pTarget->m_InfoBase.iHP = iTargetHPCur;
		pTarget->m_InfoBase.iHPMax = iTargetHPMax;

		char szBuf[256] = "";
		if(iTargetHPChange < 0)
		{
			std::string szFmt = "%s received %d damage";
			//::_LoadStringFromResource(IDS_MSG_FMT_TARGET_HP_LOST, szFmt);
			sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str(), -iTargetHPChange);
			MsgOutput(szBuf, 0xffffffff);
		}
		else if(iTargetHPChange > 0)
		{
			std::string szFmt = "IDS_MSG_FMT_TARGET_HP_RECOVER (%s, %d)";
			//::_LoadStringFromResource(IDS_MSG_FMT_TARGET_HP_RECOVER, szFmt);
			sprintf(szBuf, szFmt.c_str(), pTarget->IDString().c_str(), iTargetHPChange);
			MsgOutput(szBuf, 0xff6565ff);
		}
	}
}

// 상거래..................
bool CGameProcMain::MsgSend_NPCEvent(short siIDTarget)
{
	BYTE byBuff[4];
	int iOffset = 0;
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_NPC_EVENT);
	CAPISocket::MP_AddShort(byBuff, iOffset, siIDTarget);

	s_pSocket->Send(byBuff, iOffset);

	return true;
}

void CGameProcMain::MsgSend_NPCInRequest(int iID) // NPC 정보가 없을 경우 요청한다..
{
	TRACE("found NPC ghost (ID:%d)\n", iID);

	int iOffset=0;													// 버퍼의 오프셋..
	BYTE byBuff[32];;												// 패킷 버퍼..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_REQUEST_NPC_IN);		// 커멘드.
	CAPISocket::MP_AddShort(byBuff, iOffset, 1);					// 아이디 갯수..
	CAPISocket::MP_AddShort(byBuff, iOffset, iID);					// 자세한 정보가 필요한 아이디들..

	s_pSocket->Send(byBuff, iOffset);
}

void CGameProcMain::MsgSend_UserInRequest(int iID) // User 정보가 없을 경우 요청한다..
{
	TRACE("found User ghost (ID:%d)\n", iID);

	int iOffset=0;													// 버퍼의 오프셋..
	BYTE byBuff[32];;												// 패킷 버퍼..
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_REQUEST_USER_IN);	// 커멘드.
	CAPISocket::MP_AddShort(byBuff, iOffset, 1);					// 아이디 갯수..
	CAPISocket::MP_AddShort(byBuff, iOffset, iID);					// 자세한 정보가 필요한 아이디들..

	s_pSocket->Send(byBuff, iOffset);
}


void CGameProcMain::MsgSend_Warp() // 워프 - 존이동이 될수도 있다..
{	
	__WarpInfo WI;
	int iSel = m_pUIWarp->InfoGetCur(WI);
	if(iSel < 0 || WI.szName.empty()) return;

	BYTE byBuff[8];
	int iOffset = 0;
	
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_WARP_LIST);
	CAPISocket::MP_AddShort(byBuff, iOffset, WI.iID); // 워프 아이디 보내기...
	s_pSocket->Send(byBuff, iOffset);

	/*
	__Vector3 vec3;
	vec3.x = 361.278503f;
	vec3.y = 2.822370f;
	vec3.z = 137.339859f;

	InitZone(WI.iZone, vec3);
	*/
}

void CGameProcMain::DoCommercialTransaction(int iTradeID)
{
	if (m_pUITransactionDlg->IsVisible())
		return;

	m_pUITransactionDlg->m_iTradeID = iTradeID;

	if ( !m_pUITransactionDlg->IsVisible() )
		m_pUITransactionDlg->SetVisible(true);

	if (m_pUIInventory->IsVisible()) // 인벤토리가 안열려 있으면..
		this->CommandToggleUIInventory();

	if (m_pUISkillTreeDlg->IsVisible())
		m_pUISkillTreeDlg->Close();

	m_pUITransactionDlg->EnterTransactionState();
}

bool CGameProcMain::MsgRecv_ItemTradeStart(DataPack* pDataPack, int& iOffset)			// 아이템 상거래..
{
	int iTradeID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);		// Trade id
	CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(s_pPlayer->m_iIDTarget, true);
	if (!pNPC)	return true;
	m_pUINpcEvent->Open(NPC_EVENT_ITEM_TRADE, iTradeID, pNPC->GetNPCOriginID());

	return true;
}

bool CGameProcMain::MsgRecv_ItemTradeResult(DataPack* pDataPack, int& iOffset)			// 아이템 상거래 결과..
{
	byte bfType = 0x00;	int	iMoney = 0;
	byte bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// Trade id

	switch ( bResult )
	{
		case 0x00:
			bfType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// Trade id
			m_pUITransactionDlg->ReceiveResultTradeFromServer( bResult, bfType, iMoney );
			break;

		case 0x01:
			iMoney = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);		// Trade id
			m_pUITransactionDlg->ReceiveResultTradeFromServer( bResult, bfType, iMoney );
			break;

		case 0x03:
			m_pUITransactionDlg->ReceiveResultTradeMoveSuccess();
			break;

		case 0x04:
			m_pUITransactionDlg->ReceiveResultTradeMoveFail();
			break;
	}
	
	return true;
}

void CGameProcMain::InitZone(int iZone, const __Vector3& vPosPlayer)
{
	if(m_pSnd_Battle) m_pSnd_Battle->Stop(0.0f); // 음악 멈추기..
	if(m_pSnd_Town) m_pSnd_Town->Stop(0.0f);

	MsgSend_SpeedCheck(true);		// 스피드핵 체크 기준시간 정하기
	static int iZonePrev = -1;
	if(iZonePrev != iZone) // 다른 존일 경우에만 로딩..
	{
		m_pLightMgr->Release();
		s_pEng->SetDefaultLight(m_pLightMgr->Light(0), m_pLightMgr->Light(1), m_pLightMgr->Light(2));

		if(m_pUIChatDlg) m_pUIChatDlg->ChangeChattingMode(N3_CHAT_NORMAL); //존 이동을 하면 일반 채팅상태로 바꿔준다.
		if(m_pUIPartyOrForce) m_pUIPartyOrForce->MemberDestroy();	//존 이동을 할때 파티를 깬다...
		this->UpdateUI_PartyOrForceButtons(); // 커맨드 줄에 있는 파티 버튼을 상황에 따라 업데이트 해준다.

		s_pPlayer->m_bMoveContinous = true; // 멈춘다..
		this->CommandToggleMoveContinous();

		CLogWriter::Write("CGameProcMain::InitZone -> Zone Change (%d -> %d) Position(%.1f, %.1f, %.1f)", iZonePrev, iZone, vPosPlayer.x, vPosPlayer.y, vPosPlayer.z);

		m_bLoadComplete = false; // 로딩 끝남..
		CLogWriter::Write("%d->ClearDurationalMagic()",m_pMagicSkillMng); // TmpLog1122
		m_pMagicSkillMng->ClearDurationalMagic();
		CLogWriter::Write("%d->ClearAll()", s_pFX); // TmpLog1122
		s_pFX->ClearAll();

		if(s_pUILoading)
		{
			CLogWriter::Write("s_pUILoading->Render()"); // TmpLog1122
			s_pUILoading->Render("", 0);
		}
		
		s_pPlayer->m_InfoExt.iZoneCur = iZone;
		iZonePrev = iZone; // 최근에 읽은 존 번호를 기억해둔다.

		CLogWriter::Write("%d->Find(s_pPlayer->m_InfoExt.iZoneCur)",s_pTbl_Zones); // TmpLog1122
		__TABLE_ZONE* pZoneData = s_pTbl_Zones->Find(s_pPlayer->m_InfoExt.iZoneCur);
		if(NULL == pZoneData) {
			CLogWriter::Write("can't find zone data. (zone : %d)", s_pPlayer->m_InfoExt.iZoneCur);
			__ASSERT(0, "Zone Data Not Found!");
			system("pause");
			return;
		}

		CLogWriter::Write("%d->Release()",s_pOPMgr); // TmpLog1122

		s_pOPMgr->Release(); // 다른 넘들 다 날린다..
		CLogWriter::Write("%d->InitWorld()",s_pWorldMgr); // TmpLog1122
		s_pWorldMgr->InitWorld(iZone, vPosPlayer);

		// 미니맵 로딩..
		CLogWriter::Write("%d->GetWidthByMeterWithTerrain()",ACT_WORLD); // TmpLog1122
		float fWidth = ACT_WORLD->GetWidthByMeterWithTerrain();
		CLogWriter::Write("%d->LoadMap()",m_pUIStateBarAndMiniMap); // TmpLog1122
		CLogWriter::Write("%d->szMiniMapFNszMiniMapFN",pZoneData); // TmpLog1122
		m_pUIStateBarAndMiniMap->LoadMap(pZoneData->szMiniMapFN, fWidth, fWidth);

		CLogWriter::Write("GetRepresentClass()"); // TmpLog1122
		// 줌 비율 정하기..
		float fZoom = 6.0f;
		e_Class_Represent eCR = CGameProcedure::GetRepresentClass(s_pPlayer->m_InfoBase.eClass);
		if(CLASS_REPRESENT_ROGUE == eCR) fZoom = 3.0f; // 로그 계열은 맵이 좀더 널리 자세히 보인다..
		CLogWriter::Write("%d->ZoomSet()",m_pUIStateBarAndMiniMap); // TmpLog1122
		m_pUIStateBarAndMiniMap->ZoomSet(fZoom);

		CLogWriter::Write("%d->szTerrainFN.c_str()",pZoneData); // TmpLog1122
		//char szBuf[256];
		char szFName[_MAX_PATH];
		_splitpath(pZoneData->szTerrainFN.c_str(), NULL, NULL, szFName, NULL);
		char szFName2[_MAX_PATH];
		char szFullPathName[_MAX_PATH];
		sprintf(szFName2,"%s_Bird",szFName);
		_makepath(szFullPathName, NULL, "misc\\bird", szFName2, "lst");

		m_pLightMgr->LoadZoneLight(pZoneData->szLightObjFN.c_str());

		m_bLoadComplete = true; // 로딩 끝남..
	}
		
	// 카메라 세팅..
	CN3Camera* pCamera		= s_pEng->CameraGetActive();		// 활성화된 카메라 얻기..
	if(pCamera)
	{
		__Vector3 vPosPlayer = s_pPlayer->Position();
		pCamera->m_Data.fFOV	= D3DXToRadian(70);				// Field of View ..
		pCamera->m_Data.fFP		= 512.0f;						// Far Plane..
		pCamera->m_Data.fNP		= 0.5f;							// Near Plane..
		CLogWriter::Write("pCamera->LookAt()"); // TmpLog1122
		pCamera->LookAt(vPosPlayer + __Vector3(0,0,-1), vPosPlayer, __Vector3(0,1,0));
		CLogWriter::Write("pCamera->Tick()"); // TmpLog1122
		pCamera->Tick();
		CLogWriter::Write("pCamera->Apply()"); // TmpLog1122
		pCamera->Apply();
	}
	// 기본적인 캐릭터위치와 카메라 위치 잡기..
	////////////////////////////////////////////////////////////////////////////////

	CLogWriter::Write("InitPlayerPosition() Position(%.1f, %.1f, %.1f)",vPosPlayer.x, vPosPlayer.y, vPosPlayer.z); // TmpLog1122
	this->InitPlayerPosition(vPosPlayer); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
	CLogWriter::Write("%d->Release()",s_pOPMgr); // TmpLog1122
	s_pOPMgr->Release(); // 다른 플레이어 삭제...
}


void CGameProcMain::MsgSend_GameStart()
{
	BYTE byBuff[32];															// 패킷 버퍼..
	int iOffset=0;															// 패킷 오프셋..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_GAMESTART);						// 게임 스타트 패킷 커멘드..
	//CAPISocket::MP_AddByte(byBuff, iOffset, s_pPlayer->IDString().size());		// 아이디 길이 패킷에 넣기..
	//CAPISocket::MP_AddString(byBuff, iOffset, s_pPlayer->IDString());			// 아이디 문자열 패킷에 넣기..

	// NOTE(srmeier): start the first part of the login process
	CAPISocket::MP_AddByte(byBuff, iOffset, 0x01);

	s_pSocket->Send(byBuff, iOffset);
}

bool CGameProcMain::CommandToggleWalkRun()
{
	bool bRun = s_pPlayer->ToggleRunMode();
	bool bMove = s_pPlayer->m_bMoveContinous;

	// 커맨드 버튼 업데이트..
	if(m_pUICmd->m_pBtn_Act_Walk)
	{
		m_pUICmd->m_pBtn_Act_Walk->SetVisible(bRun);
		if(bMove) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
	}
	if(m_pUICmd->m_pBtn_Act_Run)
	{
		m_pUICmd->m_pBtn_Act_Run->SetVisible(!bRun);
		if(bMove) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return bRun;
}

bool CGameProcMain::CommandToggleAttackContinous()
{
	CPlayerBase* pTarget = s_pPlayer->TargetPointerCheck(false);
	this->CommandEnableAttackContinous(!s_pPlayer->m_bAttackContinous, pTarget);

	return s_pPlayer->m_bAttackContinous;
}

bool CGameProcMain::CommandToggleMoveContinous()
{
	if(s_pPlayer->IsDead()) return false;

	s_pPlayer->ToggleMoveMode();				// 자동 전진 토글.. 
	if(s_pPlayer->m_bMoveContinous)
	{
		this->CommandMove(MD_FOWARD, true);
		if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
		if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
	}
	else
	{
		this->CommandMove(MD_STOP, true); // 움직임 패킷 보내기..
		if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);
	}

	return s_pPlayer->m_bMoveContinous;
}

void CGameProcMain::CommandMove(e_MoveDirection eMD, bool bStartOrEnd)
{
	if(bStartOrEnd) this->CloseUIs(); // 각종 상거래, 워프등등... UI 닫기..

	if(s_pPlayer->IsDead()) return; // 죽은 넘이 어딜 감히!!

	if(MD_FOWARD == eMD || MD_BACKWARD == eMD)
	{
		s_pUIMgr->UserMoveHideUIs();
		this->CommandSitDown(false, false, true); // 일으켜 세우고..
		if(s_pPlayer->m_bStun) return; // 기절해 있음 움직이지 못함..
		if(MD_FOWARD == eMD)
		{
			if(s_pPlayer->IsRunning()) s_pPlayer->ActionMove(PSM_RUN); // 뛰어가기..
			else s_pPlayer->ActionMove(PSM_WALK); // 걸어가기..
		}
		else
		{
			s_pPlayer->ActionMove(PSM_WALK_BACKWARD); // 후진..
		}

		if( bStartOrEnd ) // 움직이기 시작할때에는 충돌체크를 먼저 해본다..
		{
			float fSpeed = s_pPlayer->MoveSpeedCalculationAndCheckCollision(); // 속도를 구하고 그 속도로 충돌 체크를 한다. 리턴값이 0 이면 충돌이다..
			if(0 == fSpeed) // 못움직이는 상황이면..
			{
				s_pPlayer->ActionMove(PSM_STOP); // 멈춤..
			}
			else
			{
				this->MsgSend_Move(true, false); // 움직이는 순간이면.. 움직임 시작 패킷 보내기.
			}

			if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_DOWN);
			if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_DOWN);
		}
	}
	else if(MD_STOP == eMD)
	{
		s_pPlayer->m_bMoveContinous = false; // 계속 걸음 멈춤..
		s_pPlayer->ActionMove(PSM_STOP);
		
		if(m_pUICmd->m_pBtn_Act_Run) m_pUICmd->m_pBtn_Act_Run->SetState(UI_STATE_BUTTON_NORMAL);
		if(m_pUICmd->m_pBtn_Act_Walk) m_pUICmd->m_pBtn_Act_Walk->SetState(UI_STATE_BUTTON_NORMAL);

		this->MsgSend_Move(false, false); // 움직임 멈춤 패킷 보내기.
	}

	if(s_pPlayer->m_bAttackContinous) // 공격중이면..
	{
		CPlayerBase* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
		if(s_pPlayer->IsAttackableTarget(pTarget)) // 공격 가능하면..
			s_pPlayer->Action(PSA_ATTACK, false, pTarget); // 공격
	}
}

void CGameProcMain::CommandEnableAttackContinous(bool bEnable, CPlayerBase* pTarget)
{
	if(bEnable == s_pPlayer->m_bAttackContinous) return;
	if(bEnable)
	{
		this->CloseUIs(); // 각종 상거래, 워프등등... UI 닫기..
		s_pUIMgr->UserMoveHideUIs();

		if(s_pPlayer->m_bStun) return; // 기절해 있음 공격 못함..
		if(NULL == pTarget) return;
		s_pPlayer->RotateTo(pTarget); // 방향을 돌린다.
		if(pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation) return; // 국가가 같으면 넘어간다..

		//-------------------------------------------------------------------------
		/*
		// TODO(srmeier): need to use ZoneAbilityType here
		// NOTE(srmeier): using zoneability information to determine if target is attackable
		if (!ACT_WORLD->canAttackSameNation() && (pTarget->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation))
			return;
		if (!ACT_WORLD->canAttackOtherNation() && (s_pPlayer->m_InfoBase.eNation == NATION_ELMORAD && pTarget->m_InfoBase.eNation == NATION_KARUS))
			return;
		if (!ACT_WORLD->canAttackOtherNation() && (s_pPlayer->m_InfoBase.eNation == NATION_KARUS && pTarget->m_InfoBase.eNation == NATION_ELMORAD))
			return;
		*/
		//-------------------------------------------------------------------------
	}
	s_pPlayer->m_bAttackContinous = bEnable; // 상태를 기록하고..

	if(bEnable)
		SetGameCursor(s_hCursorAttack);
	else
	{
		e_Nation eNation = s_pPlayer->m_InfoBase.eNation;
		SetGameCursor(((NATION_ELMORAD == eNation) ? s_hCursorNormal1 : s_hCursorNormal));
	}

	if(m_pUICmd->m_pBtn_Act_Attack)
	{
		if(bEnable) m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_DOWN);
		else m_pUICmd->m_pBtn_Act_Attack->SetState(UI_STATE_BUTTON_NORMAL);
	}

	std::string szMsg;
	if(	bEnable ) // 자동 공격!
	{
		//::_LoadStringFromResource(IDS_MSG_ATTACK_START, szMsg);
		szMsg = "Beginning attack on " + pTarget->IDString();
		this->PlayBGM_Battle();
		
		if(s_pPlayer->IsAttackableTarget(pTarget))
			s_pPlayer->Action(PSA_BASIC, true, pTarget);
	}
	else // 자동 공격 아님.
	{
		szMsg = "Stop Attack";//::_LoadStringFromResource(IDS_MSG_ATTACK_STOP, szMsg);
		s_pPlayer->Action(PSA_BASIC, true, pTarget);
	}

	this->MsgOutput(szMsg, 0xff00ffff);


	if(	bEnable && false == s_pPlayer->IsAttackableTarget(pTarget)) // 국가, 거리 및 각도 체크해서 공격 불가능하면 돌아가기..
	{
		std::string szMsg = "Could not attack because you're facing the wrong direction or the target is too far.";
		//::_LoadStringFromResource(IDS_MSG_ATTACK_DISABLE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		return;
	}
}

bool CGameProcMain::CommandToggleUIState()
{
	bool bNeedOpen = !(m_pUIVar->IsVisible());
	if(bNeedOpen)
	{
		s_pUIMgr->SetFocusedUI(m_pUIVar);
		m_pUIVar->Open();
	}
	else m_pUIVar->Close();

	// 커맨드 버튼 업데이트..
//	if(m_pUICmd->m_pBtn_Character)
//	{
//		if(bNeedOpen) m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_DOWN); // 버튼 누름 해제..
//		else m_pUICmd->m_pBtn_Character->SetState(UI_STATE_BUTTON_NORMAL); // 버튼 누름
//	}

	return bNeedOpen;
}

bool CGameProcMain::CommandToggleUIInventory()
{
	bool bNeedOpen = false;

	// 개인 거래중이면..
	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (m_pUINpcEvent->IsVisible())
		return bNeedOpen;

	if( m_pUIInn->IsVisible()) return bNeedOpen;
	if( m_pUICreateClanName->IsVisible()) return bNeedOpen;

	if (m_pUIWareHouseDlg->IsVisible())
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close(true);
		return bNeedOpen;
	}

	if (m_pUITransactionDlg->IsVisible())
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close(true);
		return bNeedOpen;
	}
	
	if (m_pUIInventory->IsVisible())
	{
		m_pUIInventory->Close(true);
		return bNeedOpen;
	}
	else
	{
		bNeedOpen = true;
		if(m_pUISkillTreeDlg->IsVisible()) m_pUISkillTreeDlg->Close();
		
		s_pUIMgr->SetFocusedUI(m_pUIInventory);
		m_pUIInventory->Open();
	}

	return bNeedOpen;
}

bool CGameProcMain::CommandToggleUISkillTree()
{
	bool bNeedOpen = !(m_pUISkillTreeDlg->IsVisible());

	// 개인 거래중이면..
	if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
		return bNeedOpen;

	if (bNeedOpen)
	{
		if (m_pUIInventory->IsVisible())
			m_pUIInventory->Close();
		if (m_pUITransactionDlg->IsVisible())
			m_pUITransactionDlg->LeaveTransactionState();
		if (m_pUIWareHouseDlg->IsVisible())
			m_pUIWareHouseDlg->LeaveWareHouseState();

		s_pUIMgr->SetFocusedUI(m_pUISkillTreeDlg);
		m_pUISkillTreeDlg->Open();
	}
	else
	{
		m_pUISkillTreeDlg->Close();
	}

	return bNeedOpen;
}

bool CGameProcMain::CommandToggleUIMiniMap()
{
	return m_pUIStateBarAndMiniMap->ToggleMiniMap();
}

void CGameProcMain::CommandCameraChange() // 카메라 시점 바꾸기..
{
	if(VP_THIRD_PERSON == s_pEng->ViewPoint() && s_pPlayer->m_bTargetOrPosMove)
	{
		s_pPlayer->m_bMoveContinous = false;
		s_pPlayer->m_bTargetOrPosMove = false;
		CommandToggleMoveContinous();
	}

	s_pEng->ViewPointChange(VP_UNKNOWN); // 순서대로 시점을 바꾼다..
}

void CGameProcMain::MsgOutput(const std::string& szMsg, D3DCOLOR crMsg)
{
	m_pUIMsgDlg->AddMsg(szMsg, crMsg);
}

bool CGameProcMain::MsgRecv_ItemDroppedGetResult(DataPack* pDataPack, int& iOffset)	// 땅에 떨어진 아이템 먹기 결과..
{
	BYTE	bResult; 
	BYTE	bPos;
	int		iItemID = 0;
	int		iGoldID = 0;
	int		iStrLen = 0;
	short	sItemCount = 0;
	std::string szString = "";

	bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	if ( (bResult == 0x01) || (bResult == 0x02) || (bResult == 0x05) )
	{
		bPos	= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
			if ( (bResult == 0x01) || (bResult == 0x05) )
			{
				sItemCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			}
		iGoldID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
	}


	if (bResult == 0x03)
	{
		iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		iStrLen = (int)CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szString, iStrLen);
	}

	TRACE("받음 - Item Get %d %d\n", bResult, iGoldID);

	if ( m_pUIDroppedItemDlg )
		m_pUIDroppedItemDlg->GetItemByIDToInventory(bResult, iItemID, iGoldID, bPos, sItemCount, iStrLen, szString);
	return true;
}

void CGameProcMain::MsgRecv_ZoneChange(DataPack* pDataPack, int& iOffset)
{
	BYTE ZoneChangeFlag = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch (ZoneChangeFlag) {

		case ZoneChangeTeleport: {
			int iZone = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			float fX = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
			float fZ = (CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset)) / 10.0f;
			float fY = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset)) / 10.0f;
			int iVictoryNation = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			CGameProcedure::LoadingUIChange(iVictoryNation);


			__Vector3 vPosPlayer;
			vPosPlayer.x = fX;
			vPosPlayer.y = fY;
			vPosPlayer.z = fZ;
			this->InitPlayerPosition(vPosPlayer); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
			s_pPlayer->RegenerateCollisionMesh(); // 충돌 메시를 다시 만든다..
			s_pPlayer->m_iSendRegeneration = 0; // 한번 보내면 다시 죽을때까지 안보내는 플래그
			s_pPlayer->m_fTimeAfterDeath = 0; // 한번 보내면 다시 죽을때까지 안보내는 플래그

			if (s_pPlayer->IsDead())
			{
				TRACE("ZoneChange - 다시 살아나기(%.1f, %.1f)\n", fX, fZ);

				//마법 & 효과 초기화..
				if (m_pUIStateBarAndMiniMap) m_pUIStateBarAndMiniMap->ClearMagic();
				if (m_pMagicSkillMng) m_pMagicSkillMng->ClearDurationalMagic();
				if (CGameProcedure::s_pFX) s_pFX->StopMine();

				if (s_pPlayer->Nation() == NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(), -1);
				else if (s_pPlayer->Nation() == NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(), -1);
			}

			this->InitZone(iZone, __Vector3(fX, fY, fZ)); // Zone Update

			BYTE byBuff[4];
			int iOffset_send = 0;
			CAPISocket::MP_AddByte(byBuff, iOffset_send, N3_ZONE_CHANGE);
			CAPISocket::MP_AddByte(byBuff, iOffset_send, (Uint8)ZoneChangeLoading);
			s_pSocket->Send(byBuff, iOffset_send);
		} break;

		case ZoneChangeLoaded: {
			BYTE byBuff[4];
			int iOffset_send = 0;
			CAPISocket::MP_AddByte(byBuff, iOffset_send, N3_ZONE_CHANGE);
			CAPISocket::MP_AddByte(byBuff, iOffset_send, (Uint8)ZoneChangeLoaded);
			s_pSocket->Send(byBuff, iOffset_send);
		} break;

	}
}

void CGameProcMain::MsgRecv_UserState(DataPack* pDataPack, int& iOffset)
{
	int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	e_SubPacket_State eSP = (e_SubPacket_State)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset); // 0x01
	int iState = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);//int iState = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	CPlayerBase* pBPC = NULL;
	if ( s_pPlayer->IDNumber() == iID )
		pBPC = s_pPlayer;
	else
		pBPC = s_pOPMgr->UPCGetByID(iID, false); 
	
	if(NULL == pBPC) return;

	if(N3_SP_STATE_CHANGE_SITDOWN == eSP) // 앉기,서기,죽음...
	{
		if(pBPC != s_pPlayer) // 플레이어가 아닐때..
		{
			if(0x01 == iState) 
				pBPC->Action(PSA_BASIC, true);
			else if(0x02 == iState) 
				pBPC->Action(PSA_SITDOWN, true);
			else if(0x03 == iState) 
				pBPC->Action(PSA_DYING, false, NULL, true);
		}
	}
	else if(N3_SP_STATE_CHANGE_RECRUIT_PARTY == eSP) // 파티 구함..
	{
		if(0x01 == iState) 
		{
			pBPC->InfoStringSet("", 0);
		}
		else if(0x02 == iState) 
		{
			int iLevel = pBPC->m_InfoBase.iLevel;
			int iLMin = iLevel - 8;
			if(iLMin < 0) iLMin = 0;
			int iLMax = iLevel + 8;
			if(iLMax > 60) iLMax = 60;

			char szBuff[128];
			std::string szMsg;
			szMsg = "IDS_WANT_PARTY_MEMBER";//::_LoadStringFromResource(IDS_WANT_PARTY_MEMBER, szMsg);
			sprintf(szBuff, szMsg.c_str(), iLMin, iLMax);
			pBPC->InfoStringSet(szBuff, 0xff00ff00);
		}
	}
	else if(N3_SP_STATE_CHANGE_SIZE == eSP) // 크기 변함
	{
		if(0x01 == iState)
		{
			pBPC->ScaleSetGradually(1.0f); // 보통 크기..
			pBPC->FlickerFactorSet(1.0f); // 부활되서 반투명.. 깜박거린다..
		}
		else if(0x02 == iState) pBPC->ScaleSetGradually(2.0f); // 커졌다..
		else if(0x03 == iState) pBPC->ScaleSetGradually(0.5f);// 작아졌다.
		else if(0x04 == iState) pBPC->FlickerFactorSet(0.7f); // 부활되서 반투명.. 깜박거린다..
	}
	else if(N3_SP_STATE_CHANGE_ACTION == eSP) // 크기 변함
	{
		if(1 == iState) pBPC->AnimationAdd(ANI_GREETING0, true); // 인사
		else if(11 == iState) pBPC->AnimationAdd(ANI_WAR_CRY1, true); // 도발
	}
	else if (N3_SP_STATE_CHANGE_VISIBLE == eSP)
	{
		if (pBPC->m_InfoBase.iAuthority == AUTHORITY_MANAGER) {
			if (0 == iState)
				pBPC->m_bVisible = true;
			if (255 == iState && (s_pPlayer->m_InfoBase.iAuthority != AUTHORITY_MANAGER))
				pBPC->m_bVisible = false;
		}
	}
}

void CGameProcMain::MsgRecv_Notice(DataPack* pDataPack, int& iOffset)
{
	if(m_pUINotice) m_pUINotice->RemoveNotice();

	int iNoticeCount = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	for(int i = 0; i < iNoticeCount; i++)
	{
		int iStrLen = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		if(iStrLen <= 0) continue;

		std::string szNotice;
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szNotice, iStrLen);
		if(m_pUINotice) m_pUINotice->m_Texts.push_back(szNotice);
	}

	if(m_pUINotice && iNoticeCount > 0)
	{
		m_pUINotice->GenerateText();
		
		// NOTE(srmeier): set where on the screen the notice appears
		RECT rc = m_pUINotice->GetRegion();
		int x = CN3Base::s_CameraData.vp.Width - (rc.right - rc.left);//(CN3Base::s_CameraData.vp.Width/2) - (rc.right - rc.left)/2;
		int y = 10;//(CN3Base::s_CameraData.vp.Height/2) - (rc.bottom - rc.top)/2;
		m_pUINotice->SetPos(x, y);
		m_pUINotice->SetVisible(true);
	}
}

void CGameProcMain::MsgRecv_PartyOrForce(DataPack* pDataPack, int& iOffset)
{
//	int iPartyOrForce = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	int iSubCmd	= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	
	switch(iSubCmd)
	{
		case N3_SP_PARTY_OR_FORCE_PERMIT:			// 0x02	// Send - b1(YesNo) | Recv - s1(ID) 요청한 사람의 ID
		{
			int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iStrLen		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			std::string szID;
			CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iStrLen);
			
			if(iID >= 0)
			{
				std::string szMsg = " IDS_PARTY_PERMIT";
				//::_LoadStringFromResource(IDS_PARTY_PERMIT, szMsg);
				CGameProcedure::MessageBoxPost(szID + szMsg, "", MB_YESNO, BEHAVIOR_PARTY_PERMIT);
			}
		}
		break;

		case N3_SP_PARTY_OR_FORCE_INSERT:			// 0x02	// Send - s1(ID) | Recv - s3(ID, HPMax, HP) b2(Level, Class) - 문자열은 ID 로 알아낸다..
		{
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iErrorCode = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

			if(iErrorCode >= 0)
			{
				int iIDLength	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				std::string szID; CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iIDLength);
				int iHPMax		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				int iHP			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				int iLevel		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
				e_Class eClass	= (e_Class)(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset));

				// NOTE: these parts where added to this packet at some later point and will need to be
				// implemented...
				int iMPMax		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				int iMP			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				e_Nation eNation = (e_Nation)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

				m_pUIPartyOrForce->MemberAdd(iID, szID, iLevel, eClass, iHP, iHPMax); // 다른넘 파티에추가..
				if(iID != s_pPlayer->IDNumber()) // 자기 자신이 아닌 경우 메시지 출력.
				{
					std::string szMsg = " IDS_PARTY_INSERT"; //::_LoadStringFromResource(IDS_PARTY_INSERT, szMsg);
					this->MsgOutput(szID + szMsg, D3DCOLOR_ARGB(255,255,255,255));
				}
			}
			else // 파티에 들어올수 없다..
			{
				std::string szMsg;

				if (-1 == iErrorCode) szMsg = "IDS_PARTY_INSERT_ERR_REJECTED";//::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_REJECTED, szMsg); // 상대방이 파티에 들어오기를 거절 하였다..
				else if (-2 == iErrorCode) szMsg = "IDS_PARTY_INSERT_ERR_LEVEL_DIFFERENCE";//::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_LEVEL_DIFFERENCE, szMsg); // 레벨 차이가 너무 난다...
				else if (-3 == iErrorCode) szMsg = "IDS_PARTY_INSERT_ERR_INVALID_NATION";//::_LoadStringFromResource(IDS_PARTY_INSERT_ERR_INVALID_NATION, szMsg); // 파티를 맺을 수 없는 국가이다.
				else szMsg = "IDS_PARTY_INSERT_ERR";//::_LoadStringFromResource(IDS_PARTY_INSERT_ERR, szMsg); // 상대방이 파티에 들어오기를 거절 하였다..

				this->MsgOutput(szMsg, D3DCOLOR_ARGB(255,255,255,255));
				if(m_pUIPartyOrForce->MemberCount() == 1) m_pUIPartyOrForce->MemberDestroy(); // 멤버가 한명이면 내가 파티를 만든 경우다.
			}
			
			this->UpdateUI_PartyOrForceButtons(); // 커맨드 줄에 있는 파티 버튼을 상황에 따라 업데이트 해준다.
		}
		break;
	
		case N3_SP_PARTY_OR_FORCE_REMOVE:			// 0x03	// Send - s1(ID) | Recv - s1(ID) - 
		{
			int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

			if(iID == s_pPlayer->IDNumber())
			{
				std::string szMsg = "IDS_PARTY_DESTROY";
				//::_LoadStringFromResource(IDS_PARTY_DESTROY, szMsg); // 파티를 떠났다..
				this->MsgOutput(szMsg, D3DCOLOR_ARGB(255,255,255,255));  // 파티 해제 메시지
				m_pUIPartyOrForce->MemberDestroy(); // 자기 자신이면.. 파티를 뽀갠다..
			}
			else
			{
				int iMemberIndex = -1;
				const __InfoPartyOrForce* pInfo = m_pUIPartyOrForce->MemberInfoGetByID(iID, iMemberIndex);
				if(pInfo)
				{
					std::string szMsg = " IDS_PARTY_LEAVE";
					//::_LoadStringFromResource(IDS_PARTY_LEAVE, szMsg); // 파티를 떠났다..
					this->MsgOutput(pInfo->szID + szMsg, D3DCOLOR_ARGB(255,255,255,255)); // 누가 파티에서 떠났다는 메시지..
					m_pUIPartyOrForce->MemberRemove(iID); // 남이면..
				}
			}

			this->UpdateUI_PartyOrForceButtons(); // 커맨드 줄에 있는 파티 버튼을 상황에 따라 업데이트 해준다.
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_DESTROY:			// 0x04	// Send
		{
			m_pUIPartyOrForce->MemberDestroy(); // 파티 뽀갠다..
			std::string szMsg = "IDS_PARTY_DESTROY"; //::_LoadStringFromResource(IDS_PARTY_DESTROY, szMsg);
			this->MsgOutput(szMsg, D3DCOLOR_ARGB(255,255,255,255));

			this->UpdateUI_PartyOrForceButtons(); // 커맨드 줄에 있는 파티 버튼을 상황에 따라 업데이트 해준다.
		}
		break;

		case N3_SP_PARTY_OR_FORCE_HP_CHANGE:		// 0x05	// Recv - s3(ID, HPMax, HP) - 자기 자신이면 파티를 깨야 한다..
		{
			int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iHPMax		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iHP			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

			m_pUIPartyOrForce->MemberHPChange(iID, iHP, iHPMax);
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_LEVEL_CHANGE:		// 0x06	// Recv - s1(ID), b1(Level)
		{
			int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iLevel		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

			m_pUIPartyOrForce->MemberLevelChange(iID, iLevel);
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_CLASS_CHANGE:		// 0x07	// Recv - s1(ID), b1(Class)드물지만 전직할때...
		{
			int iID			= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			e_Class eClass	= (e_Class)(CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset));

			m_pUIPartyOrForce->MemberClassChange(iID, eClass);
		}
		break;
		
		case N3_SP_PARTY_OR_FORCE_STATUS_CHANGE:	// 0x08	// Recv - s1(ID), b1(Status)...독, 저주, 지속성마법, 축복
		{
			int iID	=			CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			e_PartyStatus ePS =	(e_PartyStatus)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			int iSuffer = 		CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			bool bSuffer = (iSuffer) ? true : false; 

			m_pUIPartyOrForce->MemberStatusChange(iID, ePS, bSuffer);
		}
		break;
	}
}

void CGameProcMain::CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately)
{
	if(bSitDown == s_pPlayer->m_bSitDown) return; // 상태가 같으면 돌아간다..

	// 앉기 서기 제한..
	if(bLimitInterval)
	{
		float fTime = CN3Base::TimeGet();
		static float fTimePrev = fTime - 4.0f;
		if(fTime - fTimePrev < 3.0f) return;
		fTimePrev = fTime;
	}

	e_StateAction eSA = s_pPlayer->State();
	e_StateMove eSM = s_pPlayer->StateMove();

	if(bSitDown)
	{
		if(eSM != PSM_STOP) // 뛰던 놈이면 멈춘다.
			return;

		if(this->m_pMagicSkillMng->IsCasting()) // 스킬 캐스팅 중이면 앉지 못한다..
			return;

		if(s_pPlayer->m_bAttackContinous) // 계속 공격하는 중이면..
			this->CommandEnableAttackContinous(false, NULL); // 계속 공격 취소..
	}

	bool bVBs[2] = { false, false };
	int iState = 0;
	if(bSitDown)
	{
		eSA = PSA_SITDOWN;
		bVBs[0] = false; bVBs[1] = true;
		iState = 0x02;
	} // 앉는 경우
	else
	{
		eSA = PSA_BASIC;
		bVBs[0] = true; bVBs[1] = false;
		iState = 0x01;
	} // 일어서는 경우
	
	s_pPlayer->m_bSitDown = bSitDown;
	s_pPlayer->Action(eSA, true, NULL, bImmediately);

	if(m_pUICmd->m_pBtn_Act_SitDown)
	{
		m_pUICmd->m_pBtn_Act_SitDown->SetVisible(bVBs[0]);
		m_pUICmd->m_pBtn_Act_SitDown->SetState(UI_STATE_BUTTON_DOWN); // 버튼 누름
	}
	if(m_pUICmd->m_pBtn_Act_StandUp)
	{
		m_pUICmd->m_pBtn_Act_StandUp->SetVisible(bVBs[1]);
		m_pUICmd->m_pBtn_Act_StandUp->SetState(UI_STATE_BUTTON_DOWN); // 버튼 누름
	}

	this->MsgSend_StateChange(N3_SP_STATE_CHANGE_SITDOWN, iState); // 앉았다.. 패킷..
}

void CGameProcMain::CommandTargetSelect_NearstEnemy() // 가장 가까운 적 타겟 잡기..
{
	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByNearstEnemy(s_pPlayer->m_InfoBase.eNation, s_pPlayer->Position());
	this->TargetSelect(pTarget);
	s_pPlayer->RotateTo(pTarget);
}

void CGameProcMain::CommandTargetSelect_NearstOurForce() // 가장 가까운 파티 타겟잡기..
{
	CPlayerOther* pTarget = m_pUIPartyOrForce->MemberGetByNearst(s_pPlayer->Position());
	this->TargetSelect(pTarget);
	s_pPlayer->RotateTo(pTarget);
}

void CGameProcMain::CloseUIs()
{
}

void CGameProcMain::MsgSend_StateChange(e_SubPacket_State eSP, int iState)
{
	BYTE byBuff[4];											// 패킷 버퍼..
	int iOffset=0;											// 패킷 오프셋..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_STATE_CHANGE);	// 상태 변화..
	CAPISocket::MP_AddByte(byBuff, iOffset, eSP);
	CAPISocket::MP_AddShort(byBuff, iOffset, iState);//CAPISocket::MP_AddByte(byBuff, iOffset, iState);

	s_pSocket->Send(byBuff, iOffset);	
}

void CGameProcMain::MsgSend_PerTradeReq(int iDestID, bool bNear)
{
	BYTE byBuff[4];											// 패킷 버퍼..
	int iOffset=0;											// 패킷 오프셋..

	CAPISocket::MP_AddByte(byBuff, iOffset, N3_PER_TRADE);			
	CAPISocket::MP_AddByte(byBuff, iOffset, N3_SP_PER_TRADE_REQ);		
	CAPISocket::MP_AddShort(byBuff, iOffset, (short)iDestID );			// 상대방 아이디..
	if(bNear)
		CAPISocket::MP_AddByte(byBuff, iOffset, 1);
	else
		CAPISocket::MP_AddByte(byBuff, iOffset, 2);
	//1:일반 거래
	//2:상거래 게시판 거래

	s_pSocket->Send(byBuff, iOffset);									// 보냄..

	TRACE("아이디: %d, 아이템 거래 신청 패킷 보냄.. \n", iDestID);
}

void CGameProcMain::MsgRecv_PerTrade(DataPack* pDataPack, int& iOffset)
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	short	sOtherID, sItemCount, sCount, sDurability;
	BYTE	bResult, bItemPos;
	int		iItemID, iCount, iTotalGold;

	switch ( bSubCom )
	{
		case N3_SP_PER_TRADE_REQ:
			TRACE("아이템 거래 신청 패킷 받음.. \n");
			sOtherID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	

			if (m_pUITransactionDlg->IsVisible()) 	/* 상인과 거래중이면.. */
			{
				// 거절한다..
				m_pUITransactionDlg->LeaveTransactionState();
				m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_DISAGREE);
				break;
			}

			if (m_pUIWareHouseDlg->IsVisible()) 	/* 보관함에 보관중이면.. */
			{
				// 거절한다..
				m_pUIWareHouseDlg->LeaveWareHouseState();
				m_pSubProcPerTrade->LeavePerTradeState(PER_TRADE_RESULT_MY_DISAGREE);
				break;
			}

			if ( m_pUIInventory->IsVisible() )
				m_pUIInventory->Close();

			if (m_pSubProcPerTrade->m_ePerTradeState != PER_TRADE_STATE_NONE)
			{
				std::string stdMsg = "IDS_PER_TRADEING_OTHER";
				//::_LoadStringFromResource(IDS_PER_TRADEING_OTHER, stdMsg);
				CGameProcedure::s_pProcMain->MsgOutput(stdMsg, 0xff9b9bff);
				break;
			}

			if (m_pUIQuestMenu->IsVisible())
				m_pUIQuestMenu->SetVisible(false);

			if (m_pUIQuestTalk->IsVisible())
				m_pUIQuestTalk->SetVisible(false);

			if (m_pUINpcEvent->IsVisible())
				m_pUINpcEvent->Close();

			if (m_pUITradeBBSSelector->IsVisible())
				m_pUITradeBBSSelector->SetVisible(false);

			if(m_pUITradeBBS->IsVisible())
				m_pUITradeBBS->SetVisible(false);

			m_pSubProcPerTrade->ReceiveMsgPerTradeReq(sOtherID);
			break;


		case N3_SP_PER_TRADE_AGREE:
			bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			m_pSubProcPerTrade->ReceiveMsgPerTradeAgree(bResult);
			break;

		case N3_SP_PER_TRADE_ADD:
			bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			m_pSubProcPerTrade->ReceiveMsgPerTradeAdd(bResult);
			break;

		case N3_SP_PER_TRADE_OTHER_ADD:
			iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
			iCount  = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
			sDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			m_pSubProcPerTrade->ReceiveMsgPerTradeOtherAdd(iItemID, iCount, (int)sDurability);			
			break;

		case N3_SP_PER_TRADE_OTHER_DECIDE:
			m_pSubProcPerTrade->ReceiveMsgPerTradeOtherDecide();
			break;

		case N3_SP_PER_TRADE_DONE:
			bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			if ( bResult == 0x01 )		// 성공이면..
			{	
				iTotalGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
				m_pSubProcPerTrade->ReceiveMsgPerTradeDoneSuccessBegin(iTotalGold);
				sItemCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				for( int i = 0; i < sItemCount; i++ )
				{
					bItemPos = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	
					iItemID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
					sCount  = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	
					sDurability = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
					m_pSubProcPerTrade->ReceiveMsgPerTradeDoneItemMove(bItemPos, iItemID, sCount, sDurability);					
				}
				m_pSubProcPerTrade->ReceiveMsgPerTradeDoneSuccessEnd();
			}
			else						// 실패이면..
				m_pSubProcPerTrade->ReceiveMsgPerTradeDoneFail();
			break;

		case N3_SP_PER_TRADE_CANCEL:
				m_pSubProcPerTrade->ReceiveMsgPerTradeCancel();
			break;
	}
}

void CGameProcMain::TargetSelect(int iID, bool bMustAlive)
{
	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(iID, bMustAlive);
	this->TargetSelect(pTarget);
}

void CGameProcMain::TargetSelect(CPlayerNPC* pTarget)
{
	if(PSA_SPELLMAGIC == s_pPlayer->State()) // 마법 캐스팅을 하는 중간에는 타겟을 못 바꾼다..
		return;

	int iIDTargetPrev = s_pPlayer->m_iIDTarget; // 전의 타겟 기억..
	s_pPlayer->m_iIDTarget = -1;
	s_pPlayer->m_pObjectTarget = NULL;

	if(pTarget && pTarget->m_InfoBase.iAuthority != AUTHORITY_MANAGER)
	{ //운영자는 포커스를 주지 않게 하기 위해서...
		s_pPlayer->m_iIDTarget = pTarget->IDNumber();
		if(iIDTargetPrev != s_pPlayer->m_iIDTarget) // 전의 타겟과 다르면
		{
			this->CommandEnableAttackContinous(false, NULL);	// 자동 공격 취소..

			D3DCOLOR crID = 0xffffffff;
			if(	pTarget->PlayerType() == PLAYER_OTHER) // User..
			{
				if(pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff4040; // 다른 국가이면
				else crID = 0xff6b9fff;
			}
			else // NPC
			{
				if(pTarget->m_InfoBase.eNation != s_pPlayer->m_InfoBase.eNation) crID = 0xffff6060; // 다른 국가이면
				else crID = 0xff1064ff;
			}
			
			if( pTarget->IsAlive() )//살아있는 상태만 target bar를 보여준다.
			{
				m_pUITargetBar->SetVisible(true);
				m_pUITargetBar->SetIDString(pTarget->IDString(), crID); // 이름을 넣고..
				this->MsgSend_RequestTargetHP(s_pPlayer->m_iIDTarget, 0x01);	// 서버에게 HP정보를 요청한다..
			}
			else
			{
				m_pUITargetBar->SetVisible(false); // 타겟 바 없애기..
			}
		}
	}
	else
	{
		s_pPlayer->m_iIDTarget = -1; // 타겟 아이디를 잡고..
		m_pUITargetBar->SetVisible(false); // 타겟 바 없애기..

		this->CommandEnableAttackContinous(false, NULL); // 자동 공격 취소..
	}

	if(	PSA_SITDOWN != s_pPlayer->State() && 
		PSM_STOP == s_pPlayer->StateMove() &&
		PSA_BASIC == s_pPlayer->State()) s_pPlayer->Action(PSA_BASIC, true); // 기본 자세 바꾸기..

	this->UpdateUI_PartyOrForceButtons(); // 커맨드 줄에 있는 파티 버튼을 상황에 따라 업데이트 해준다.
}

void CGameProcMain::MsgRecv_SkillChange(DataPack* pDataPack, int& iOffset)			// 스킬 변화..
{
	int iType	= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	int iValue	= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	m_pUISkillTreeDlg->m_iSkillInfo[iType] = iValue;
	m_pUISkillTreeDlg->m_iSkillInfo[0]++;
	m_pUISkillTreeDlg->InitIconUpdate();		// 스킬 포인트가 변화되었으므로 .. 스킬도 변화될 수 있다..
}

void CGameProcMain::MsgRecv_MagicProcess(DataPack* pDataPack, int& iOffset)
{
	e_SubPacket_Magic eSP = (e_SubPacket_Magic)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	
	switch ( eSP )
	{
	case N3_SP_MAGIC_CASTING:
		m_pMagicSkillMng->MsgRecv_Casting(pDataPack, iOffset);
		break;
	case N3_SP_MAGIC_FLYING:
		m_pMagicSkillMng->MsgRecv_Flying(pDataPack, iOffset);
		break;
	case N3_SP_MAGIC_EFFECTING:
		m_pMagicSkillMng->MsgRecv_Effecting(pDataPack, iOffset);
		break;
	case N3_SP_MAGIC_FAIL:
		m_pMagicSkillMng->MsgRecv_Fail(pDataPack, iOffset);
		break;
	case N3_SP_MAGIC_TYPE4BUFFTYPE:
		m_pMagicSkillMng->MsgRecv_BuffType(pDataPack, iOffset);
		break;
	}	
}

void CGameProcMain::MsgRecv_ClassChange(DataPack* pDataPack, int& iOffset)			// 직업 변화..
{
	e_SubPacket_ClassChange eSP = (e_SubPacket_ClassChange)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch ( eSP )
	{
		case N3_SP_CLASS_CHANGE_SUCCESS:		// Success..
			m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_SUCCESS);
			break;

		case N3_SP_CLASS_CHANGE_NOT_YET:		// Not Yet..
			m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_NOT_YET);
			break;

		case N3_SP_CLASS_CHANGE_ALREADY:		// Already..
			m_pUIClassChange->Open(N3_SP_CLASS_CHANGE_ALREADY);
			break;

		case N3_SP_CLASS_CHANGE_FAILURE:		// Failure..
			m_pUIClassChange->RestorePrevClass();
			break;
	}
}

void CGameProcMain::MsgRecv_ObjectEvent(DataPack* pDataPack, int& iOffset)
{
	int iType = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// Event Type
	int iResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 결과..

	if(OBJECT_TYPE_BINDPOINT == iType) // 바인드 포인트
	{
		std::string szMsg;
		if (0x01 == iResult) szMsg = "IDS_BIND_POINT_SUCCESS";//::_LoadStringFromResource(IDS_BIND_POINT_SUCCESS, szMsg);
		else szMsg = "IDS_BIND_POINT_FAILED";//::_LoadStringFromResource(IDS_BIND_POINT_FAILED, szMsg);
		this->MsgOutput(szMsg, 0xff00ff00);
	}
	else if(OBJECT_TYPE_DOOR_LEFTRIGHT == iType ||
			OBJECT_TYPE_DOOR_TOPDOWN == iType ||
			OBJECT_TYPE_LEVER_TOPDOWN == iType ||
			OBJECT_TYPE_FLAG == iType) // 오브젝트 다루기..
	{
		int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	// 열고 닫을 성문 ID
		int iActivate = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 열고 닫음..

		CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(iID, true);
		__ASSERT(pNPC, "Invalid NPC ID");
		if(pNPC)
		{
			__ASSERT(pNPC->m_pShapeExtraRef && (pNPC->m_pShapeExtraRef->Type() & OBJ_SHAPE_EXTRA), "Invalid object!!");
			std::string szMsg = "Object Event - invalid object!!";
			if(pNPC->m_pShapeExtraRef)
			{
				__Vector3 vAxis(0,1,0);
				float fRadian = D3DXToRadian(90);
				bool bShouldBeRotate = true; // 돌려야 하는지??
				if(OBJECT_TYPE_DOOR_LEFTRIGHT == iType) // 좌우열림 성문
				{
					if(0x01 == iActivate) {
						fRadian = D3DXToRadian(80);
						szMsg = "IDS_DOOR_OPENED";//::_LoadStringFromResource(IDS_DOOR_OPENED, szMsg);
					}
					else {
						fRadian = D3DXToRadian(0);
						szMsg = "IDS_DOOR_CLOSED";//::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
					}
					vAxis.Set(0,1,0);
				} 
				else if(OBJECT_TYPE_DOOR_TOPDOWN == iType) // 상하열림 성문
				{
					if(0x01 == iActivate) {
						fRadian = D3DXToRadian(90);
						szMsg = "IDS_DOOR_OPENED";//::_LoadStringFromResource(IDS_DOOR_OPENED, szMsg);
					}
					else  {
						D3DXToRadian(0);
						szMsg = "IDS_DOOR_CLOSED";//::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
					}
					vAxis.Set(0,0,1);
				}
				else if(OBJECT_TYPE_LEVER_TOPDOWN == iType) // 상하 레버
				{
					if(0x01 == iActivate) {
						fRadian = D3DXToRadian(-45);
						szMsg = "IDS_LEVER_ACTIVATE";//::_LoadStringFromResource(IDS_LEVER_ACTIVATE, szMsg);
					}
					else  {
						fRadian = D3DXToRadian(45);
						szMsg = "IDS_LEVER_DEACTIVATE";//::_LoadStringFromResource(IDS_LEVER_DEACTIVATE, szMsg);
					}
					vAxis.Set(1,0,0);
				}
				else if(OBJECT_TYPE_FLAG == iType)
				{
					bShouldBeRotate = false; // 돌려야 하는지??
				}

				if(0x01 == iActivate)
				{
				}
				else // if(0x00 == iActivate);
				{
					szMsg = "IDS_DOOR_CLOSED";//::_LoadStringFromResource(IDS_DOOR_CLOSED, szMsg);
				}

				CN3ShapeExtra* pSE = (CN3ShapeExtra*)pNPC->m_pShapeExtraRef;
				if(bShouldBeRotate)
				{
					pSE->RotateTo(0, vAxis, fRadian, 1);
					pSE->RotateTo(1, vAxis, -fRadian, 1);
				}
				else 
				{
					if(iActivate) pSE->m_bVisible = true;
					else pSE->m_bVisible = false;
				}
			}
			this->MsgOutput(szMsg, 0xff00ff00);
		}
	}
	else
	{
		__ASSERT(0, "Unknown Object Event");
	}
}

void CGameProcMain::ParseChattingCommand(const std::string& szCmd)
{
	static char szCmds[4][1024] = { "", "", "", "" };
	static BYTE byBuff[1024] = "";
	sscanf(szCmd.c_str(), "/%s %s %s %s", szCmds[0], szCmds[1], szCmds[2], szCmds[3]);

	if(0 == lstrcmp(szCmds[0], "goto"))
	{
		float fX = (float)atof(szCmds[1]);
		float fZ = (float)atof(szCmds[2]);
		
		int iOffset = 0;
		CAPISocket::MP_AddByte(byBuff, iOffset, N3_WARP);
		CAPISocket::MP_AddWord(byBuff, iOffset, (fX * 10));
		CAPISocket::MP_AddWord(byBuff, iOffset, (fZ * 10));

		s_pSocket->Send(byBuff, iOffset);
	}
	
	e_ChatCmd eCmd = CMD_UNKNOWN;
	for(int i = 0; i < CMD_COUNT; i++)
	{
		if(0 == lstrcmpi(szCmds[0], s_szCmdMsg[i].c_str()))
		{
			eCmd = (e_ChatCmd)i;
			break;
		}
	}

	switch(eCmd)
	{
		case CMD_WHISPER:
		{
			this->MsgSend_ChatSelectTarget(szCmds[1]); // 일대일 채팅 상대 정하기.
		}
		break;

		case CMD_TOWN:
		{
			if(s_pPlayer->m_bStun) return; // 기절해 있음 못함..
			if(s_pPlayer->m_InfoBase.iHP * 2 >= s_pPlayer->m_InfoBase.iHPMax) // HP가 반 이상 있어야 한다.
			{
				// NOTE(srmeier): currently there is an issue where OtherPlayers may get
				// duplicated in the player manager if they where there before the TP

				int iOffset = 0;
				CAPISocket::MP_AddWord(byBuff, iOffset, N3_HOME);		// 마을로 가기...
				s_pSocket->Send(byBuff, iOffset);
			}
			else // HP가 반 이상 있어야 한다.
			{
				std::string szMsg;
				szMsg = "IDS_ERR_GOTO_TOWN_OUT_OF_HP";//::_LoadStringFromResource(IDS_ERR_GOTO_TOWN_OUT_OF_HP, szMsg);
				this->MsgOutput(szMsg, 0xffff00ff);
			}
		}
		break;

		case CMD_TRADE:
		{
			CPlayerOther* pOPC = s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, true);
			if(	pOPC &&
				(pOPC->Position() - s_pPlayer->Position()).Magnitude() < (pOPC->Height() + 5.0f) && 
				!m_pUITransactionDlg->IsVisible() ) // 타겟으로 다른 플레이어가 잡혀있고..  가까이 있으면.. // 개인간 아이템 거래.. // 상거래 중이 아니면..
			{
				std::string szMsg = " IDS_PERSONAL_TRADE_REQUEST";
				//::_LoadStringFromResource(IDS_PERSONAL_TRADE_REQUEST, szMsg);
				MsgOutput(pOPC->IDString() + szMsg, 0xffffff00);

				MsgSend_PerTradeReq(pOPC->IDNumber());
				
				if (m_pUINpcEvent->IsVisible())
					m_pUINpcEvent->Close();

				if (m_pUIQuestTalk->IsVisible())
					m_pUIQuestTalk->SetVisible(false);

				if (m_pUIQuestMenu->IsVisible())
					m_pUIQuestMenu->SetVisible(false);

				m_pSubProcPerTrade->EnterWaitMsgFromServerStatePerTradeReq();
			}
		}
		break;

		case CMD_EXIT:
		{
			//PostQuitMessage(0);
			CGameBase::s_bRunning = false;
		}
		break;

		case CMD_PARTY:
		{
			CPlayerBase* pTarget = s_pOPMgr->UPCGetByID(s_pPlayer->m_iIDTarget, true);
			if(pTarget)
			{
				std::string szMsg;
				if (this->MsgSend_PartyOrForceCreate(0, pTarget->IDString()))
					szMsg = " IDS_PARTY_INVITE";//::_LoadStringFromResource(IDS_PARTY_INVITE, szMsg); // 파티 요청.. 
				else
					szMsg = " IDS_PARTY_INVITE_FAILED";//::_LoadStringFromResource(IDS_PARTY_INVITE_FAILED, szMsg); // 파티 초대 실패
				this->MsgOutput(pTarget->IDString() + szMsg, 0xffffff00);
			}
		}
		break;

		case CMD_LEAVEPARTY:
		{
			this->MsgSend_PartyOrForceLeave(0); // 파티 요청..
		}
		break;

		case CMD_RECRUITPARTY:
		{
			if(m_pUIPartyBBS)
			{
				if(s_pPlayer->m_bRecruitParty)
				{
					m_pUIPartyBBS->MsgSend_RegisterCancel();
				}
				else
				{
					m_pUIPartyBBS->MsgSend_Register();
				}
			}

//			if(m_pUIPartyBBS && !m_pUIPartyBBS->IsVisible())
//			m_pUIPartyBBS->MsgSend_RefreshData(0);
		}
		break;

		case CMD_JOINCLAN:
		{
			if(s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF ||
				s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_VICECHIEF)
			{			
				this->MsgSend_KnightsJoin(s_pPlayer->m_iIDTarget);
			}
		}
		break;

		case CMD_WITHDRAWCLAN:
		{
			this->MsgSend_KnightsWithdraw();
		}
		break;

		case CMD_FIRECLAN:
		{
			if(s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF)
			{
				std::string szName = szCmds[1];
				MsgSend_KnightsLeave(szName);			
			}
		}
		break;

		case CMD_APPOINTVICECHIEF:
		{
			if(s_pPlayer->m_InfoExt.eKnightsDuty == KNIGHTS_DUTY_CHIEF)
			{
				std::string szName = szCmds[1];
				MsgSend_KnightsAppointViceChief(szName);
			}
		}
		break;

		case CMD_GREETING:
		{
			if(	s_pPlayer->State() == PSA_BASIC && 
				s_pPlayer->StateMove() == PSM_STOP )
			{
				this->MsgSend_StateChange(N3_SP_STATE_CHANGE_ACTION, 1);
			}
		}
		break;

		case CMD_EXCITE:
		{
			if(	s_pPlayer->State() == PSA_BASIC && 
				s_pPlayer->StateMove() == PSM_STOP )
			{
				this->MsgSend_StateChange(N3_SP_STATE_CHANGE_ACTION, 11);
			}
		}
		break;

		case CMD_VISIBLE:
		{
			this->MsgSend_StateChange(N3_SP_STATE_CHANGE_VISIBLE, 0);
		}
		break;

		case CMD_INVISIBLE:
		{
			this->MsgSend_StateChange(N3_SP_STATE_CHANGE_VISIBLE, 255);
		}
		break;

		case CMD_CLEAN:
		{
			int iPercent = atoi(szCmds[1]);
			this->MsgSend_Weather(1, iPercent);
		}
		break;

		case CMD_RAINING:
		{
			int iPercent = atoi(szCmds[1]);
			this->MsgSend_Weather(2, iPercent);
		}
		break;

		case CMD_SNOWING:
		{
			int iPercent = atoi(szCmds[1]);
			this->MsgSend_Weather(3, iPercent);
		}
		break;

		case CMD_TIME:
		{
			int iHour = atoi(szCmds[1]);
			int iMin = atoi(szCmds[2]);
			this->MsgSend_Time(iHour, iMin);
		}
		break;

		case CMD_CU_COUNT:
		{
			int iOffset=0;
			CAPISocket::MP_AddByte(byBuff, iOffset, N3_CONCURRENT_USER_COUNT); 
			s_pSocket->Send(byBuff, iOffset);
		}
		break;

		case CMD_NOTICE:
		{
			if(szCmd.size() >= (s_szCmdMsg[CMD_NOTICE].size()+2))//7)
			{
				std::string szChat = szCmd.substr(s_szCmdMsg[CMD_NOTICE].size()+2); // "/공지 "를 제외한 나머지 문자열
				this->MsgSend_Chat(N3_CHAT_PUBLIC, szChat);
			}
		}
		break;

		case CMD_ARREST:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_ARREST, szCmds[1]); //추적		
		}
		break;

		case CMD_FORBIDCONNECT:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_FORBID_CONNECT, szCmds[1]); //접속금지		
		}
		break;
		
		case CMD_FORBIDCHAT:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_CHAT_FORBID, szCmds[1]); //채팅금지		
		}
		break;
		
		case CMD_PERMITCHAT:
		{
			this->MsgSend_Administrator(N3_SP_ADMINISTRATOR_CHAT_PERMIT, szCmds[1]); //채팅허가		
		}
		break;
		
		case CMD_GAME_SAVE:
		{
			if(m_fRequestGameSave > 300.0f)
			{
				BYTE byBuff[4];												// 버퍼.. 
				int iOffset=0;												// 옵셋..
				s_pSocket->MP_AddByte(byBuff, iOffset, N3_REQUEST_GAME_SAVE);	// 저장 요청 커멘드..
				s_pSocket->Send(byBuff, iOffset);				// 보냄..
				m_fRequestGameSave = 0.0f;

				std::string szMsg;
				szMsg = "IDS_REQUEST_GAME_SAVE";//::_LoadStringFromResource(IDS_REQUEST_GAME_SAVE, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);
			}
			else
			{
				char szBuf[256];
				std::string szMsg;
				szMsg = "IDS_DELAY_GAME_SAVE (%d)";//::_LoadStringFromResource(IDS_DELAY_GAME_SAVE, szMsg);
				sprintf(szBuf, szMsg.c_str(), 5);
				this->MsgOutput(szBuf, 0xffffff00);
			}
		}
		break;

		default:
			break;
	} // end of switch(eCmd)
}

void CGameProcMain::UpdateUI_PartyOrForceButtons()
{
	// 파티 버튼 상태 바꾸기..
	CPlayerBase* pTarget = NULL;
	bool bIAmLeader = false, bIAmMemberOfParty = false;
	int iMemberIndex = -1;
	this->PartyOrForceConditionGet(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
	m_pUIPartyOrForce->MemberSelect(iMemberIndex);

	m_pUICmd->UpdatePartyButtons(bIAmLeader, bIAmMemberOfParty, iMemberIndex, pTarget);
}

const __InfoPartyOrForce* CGameProcMain::PartyOrForceConditionGet(bool& bIAmLeader, bool& bIAmMember, int& iMemberIndex, class CPlayerBase*& pTarget)
{
	// 파티 버튼 상태 바꾸기..
	bIAmLeader = false;
	iMemberIndex = -1;
	bIAmMember = false;
	pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

	if(m_pUIPartyOrForce->MemberCount() >= 2)
	{
		bIAmMember = true;
		if(m_pUIPartyOrForce->MemberInfoGetByIndex(0)->iID == s_pPlayer->IDNumber()) bIAmLeader = true;
		return m_pUIPartyOrForce->MemberInfoGetByID(s_pPlayer->m_iIDTarget, iMemberIndex);
	}

	return NULL;
}

void CGameProcMain::UpdateUI_MiniMap()
{
	if(NULL == m_pUIStateBarAndMiniMap || !m_pUIStateBarAndMiniMap->IsVisible()) return;


	m_pUIStateBarAndMiniMap->PositionInfoClear();

	D3DCOLOR crType = 0xffffffff;
	e_Nation eNation = s_pPlayer->m_InfoBase.eNation;

	it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
	CPlayerNPC* pNPC = NULL;
	for(; it != itEnd; it++) // NPC
	{
		pNPC = it->second;

		if(eNation != pNPC->m_InfoBase.eNation) crType = 0xff800000; // 다른 국가 NPC 혹은 몬스터 주황색
		else crType = 0xff00a0ff; // 같은 국가 NPC 하늘색

		m_pUIStateBarAndMiniMap->PositionInfoAdd(pNPC->IDNumber(), pNPC->Position(), crType, false);
	}

	it_UPC it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
	CPlayerOther* pUPC = NULL;
	__TABLE_ZONE* pZoneInfo = s_pTbl_Zones->Find(s_pPlayer->m_InfoExt.iZoneCur);
	for(; it2 != itEnd2; it2++) // User
	{
		pUPC = it2->second;

		bool bDrawTop = false;
		if(eNation != pUPC->m_InfoBase.eNation) // 적국일경우
		{
			if(pUPC->State() == PSA_SITDOWN)
			{
				pUPC->m_InfoBase.bRenderID = false; // 아이디 표시하지 않음.
				continue; // 앉아있으면.. 지나간다..
			}
			else
			{
				pUPC->m_InfoBase.bRenderID = true; // 아이디 표시함.
			}

			if(pZoneInfo && FALSE == pZoneInfo->bIndicateEnemyPlayer) continue; // 적국이 표시 되지 않아야 한다면.. 지나간다..
			
			crType = 0xffff4040; // 다른 국가  - 밝은 빨간색
		}
		else // 같은 국가면..
		{
			int iMemberIndex = -1;
			const __InfoPartyOrForce* pPI = m_pUIPartyOrForce->MemberInfoGetByID(pUPC->IDNumber(), iMemberIndex);
			if(pPI)
			{
				bDrawTop = true; // 맨 위에 그린다.
				crType = 0xffffff00; // 파티 멤버면 // 노란색
			}
			else
			{
				crType = 0xff0040ff; // 파티 멤버가 아니면 밝은 파란색
			}
		}

		if(pUPC && pUPC->m_InfoBase.iAuthority != AUTHORITY_MANAGER)	// 운영자가 아닌경우만 미니맵에 포인트를 찍어준다.
			m_pUIStateBarAndMiniMap->PositionInfoAdd(pUPC->IDNumber(), pUPC->Position(), crType, bDrawTop);
	}
}

void CGameProcMain::UpdateUI_TargetBar()
{
	if(NULL == m_pUITargetBar || !m_pUITargetBar->IsVisible()) return;

	//죽은 캐릭터가 선택되었을때는 target bar를 그려주지 않는다.
	CPlayerNPC* t_pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, false);
	if( t_pTarget && t_pTarget->State() == PSA_DEATH )
	{
		m_pUITargetBar->SetVisible(false);
		return;
	}

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);
	if(pTarget) // 타겟이 유효하면..
	{
		float fTimeTmp = CN3Base::TimeGet();
		if(fTimeTmp > m_pUITargetBar->m_fTimeSendPacketLast + PACKET_INTERVAL_REQUEST_TARGET_HP) // 1초가 지나면 타겟 정보 요청
		{
			this->MsgSend_RequestTargetHP(s_pPlayer->m_iIDTarget, false);
			m_pUITargetBar->m_fTimeSendPacketLast = fTimeTmp;
		}
	}
	else // 타겟이 없으면..
	{
		s_pPlayer->m_iIDTarget = -1;
		m_pUITargetBar->SetVisible(false);
	}
}

void CGameProcMain::UpdateBGM()
{
	if(	NULL == m_pSnd_Battle || !m_pSnd_Battle->IsPlaying() ) return;

//	if(s_pPlayer->pTarget && s_pPlayer->pTarget->IsAlive()) 
	__Vector3 vPosPlayer = s_pPlayer->Position();
	e_Nation eNationPlayer = s_pPlayer->m_InfoBase.eNation;

	bool bStopBattleBgm = true;
	CPlayerBase* pBPC;
	it_NPC it = s_pOPMgr->m_NPCs.begin(), itEnd = s_pOPMgr->m_NPCs.end();
	for(; it != itEnd && bStopBattleBgm; it++)
	{
		pBPC = it->second;
		if(eNationPlayer == pBPC->m_InfoBase.eNation) continue;

		if((vPosPlayer - pBPC->Position()).Magnitude() < 12.0f)
			bStopBattleBgm = false;
	}

	CPlayerOther* pUPC;
	it_UPC it2 = s_pOPMgr->m_UPCs.begin(), itEnd2 = s_pOPMgr->m_UPCs.end();
	for(; it2 != itEnd2 && bStopBattleBgm; it2++)
	{
		pUPC = it2->second;
		if(eNationPlayer == pUPC->m_InfoBase.eNation) continue;

		if((vPosPlayer - pUPC->Position()).Magnitude() < 12.0f)
			bStopBattleBgm = false;
	}

	if(bStopBattleBgm) this->PlayBGM_Town();
}

void CGameProcMain::UpdateCameraAndLight()
{
	__Vector3 vPosPlayer = s_pPlayer->Position();
	if(s_pPlayer->State() == PSA_SITDOWN)
	{
		float fRootY = s_pPlayer->RootPosition().y;
		float fH = s_pPlayer->Height();
		vPosPlayer.y += fRootY - (fH / 2.0f);
	}

	D3DCOLOR crDiffuses[MAX_GAME_LIGHT];
	D3DCOLOR crAmbients[MAX_GAME_LIGHT];
	for(int i = 0; i < MAX_GAME_LIGHT; i++)
	{
		crDiffuses[i] = ACT_WORLD->GetLightDiffuseColorWithSky(i);
		crAmbients[i] = ACT_WORLD->GetLightAmbientColorWithSky(i);
	}
	
	s_pEng->Tick(	crDiffuses, crAmbients, ACT_WORLD->GetFogColorWithSky(), 
					vPosPlayer, s_pPlayer->Rotation(),
					s_pPlayer->Height(), ACT_WORLD->GetSunAngleByRadinWithSky() );		// 캐릭터 위치와 해의 각도를 넣어준다..
	s_pEng->ApplyCameraAndLight();	// 카메라와 라이트에 세팅된 값을 D3D Device 에 적용한다.
}

void CGameProcMain::MsgRecv_DurabilityChange(DataPack* pDataPack, int& iOffset)		// 내구력 변경..
{
	e_ItemSlot eSlot = (e_ItemSlot)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	
	int iCurValue  = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	

	s_pPlayer->DurabilitySet(eSlot, iCurValue);
	m_pUIInventory->DurabilityChange(eSlot, iCurValue);
}

void CGameProcMain::MsgRecv_NpcEvent(DataPack* pDataPack, int& iOffset)				// Npc Event(Exchange, Repair both).. 
{
	int iTradeID = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);		// Trade id
	CPlayerNPC* pNPC = s_pOPMgr->NPCGetByID(s_pPlayer->m_iIDTarget, true);
	if (!pNPC)	return;

	m_pUINpcEvent->Open(NPC_EVENT_TRADE_REPAIR, iTradeID, pNPC->GetNPCOriginID());
}


void CGameProcMain::MsgRecv_Knights(DataPack* pDataPack, int& iOffset)
{
	e_SubPacket_Knights eSP = (e_SubPacket_Knights)(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset));	// Sub Packet

	switch(eSP)
	{
	case N3_SP_KNIGHTS_CREATE: // 생성..
		this->MsgRecv_Knights_Create(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_WITHDRAW: //탈퇴
		this->MsgRecv_Knights_Withdraw(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_JOIN: //가입
		this->MsgRecv_Knights_Join(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_MEMBER_REMOVE: //멤버 삭제 - 
		this->MsgRecv_Knights_Leave(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_APPOINT_VICECHIEF: //부단장 임명 - 가입허가와 같음
		this->MsgRecv_Knights_AppointViceChief(pDataPack, iOffset);
 	break;
	case N3_SP_KNIGHTS_MEMBER_INFO_ALL:
		this->MsgRecv_Knights_MemberInfoAll(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_GRADE_CHANGE_ALL:
		this->MsgRecv_Knights_GradeChangeAll(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_DESTROY: // 뽀개기 Send - | Recv - b1(1:성공 0:실패)
		{
			BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			
			std::string szMsg;
			switch ( (e_SubPacket_KNights_Common)bSubCom )
			{
			case N3_SP_KNIGHTS_COMMON_SUCCESS: //클랜파괴 성공
				szMsg = "IDS_CLAN_WITHDRAW_SUCCESS";//::_LoadStringFromResource(IDS_CLAN_WITHDRAW_SUCCESS, szMsg);
				m_pUIKnightsOp->KnightsInfoDelete(s_pPlayer->m_InfoExt.iKnightsID);
				this->MsgOutput(szMsg, 0xffffff00);
				break;
			case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB검색 실패..
			case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//없는 유저..
				break;
			}
			s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_UNKNOWN;
			s_pPlayer->KnightsInfoSet(0, "", 0, 0);
			m_pUIVar->UpdateKnightsInfo();
		}
		break;
	case N3_SP_KNIGHTS_DUTY_CHANGE: // 직위 변경..
		MsgRecv_Knights_Duty_Change(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_JOIN_REQ:
		MsgRecv_Knigts_Join_Req(pDataPack, iOffset);
		break;

/*	case N3_SP_KNIGHTS_APPOINT_CHIEF: //단장 임명 - 가입허가와 같음
		{
			::_LoadStringFromResource(IDS_KNIGHTS_APPOINT_CHIEF_SUCCESS, szMsg); // 성공
		}
		break;
	case N3_SP_KNIGHTS_DESTROY: // 뽀개기 Send - | Recv - b1(1:성공 0:실패)
		{
			::_LoadStringFromResource(IDS_KNIGHTS_DESTROY_SUCCESS, szMsg); // 성공

			s_pPlayer->m_InfoExt.iKnightsID = 0;
			s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_UNKNOWN;

			// 기사단에서 뺀다..
			m_pUIVar->m_pPageKnights->UpdateKnightsName("");
			m_pUIVar->m_pPageKnights->UpdateKnightsChiefName("");
			m_pUIVar->m_pPageKnights->UpdateKnightsDuty(KNIGHTS_DUTY_UNKNOWN);

			m_pUIKnightsOp->MsgSend_KnightsList(0); // 다시 기사단 리스트를 요청한다..
		}
		break;

	case N3_SP_KNIGHTS_MEMBER_JOIN_ADMIT: //멤버 가입 허가 Send - s1(Knights ID) | Recv - b1(1:성공 0:실패)
		{
			::_LoadStringFromResource(IDS_KNIGHTS_ADMIT_SUCCESS, szMsg); // 성공
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_JOIN_REJECT: //멤버 가입 거절 - 가입허가와 같음
		{
			::_LoadStringFromResource(IDS_KNIGHTS_REJECT_SUCCESS, szMsg); // 성공
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_PUNISH: //멤버 징계 - 가입허가와 같음
		{
			::_LoadStringFromResource(IDS_KNIGHTS_PUNISH_SUCCESS, szMsg); // 성공
		}
		break;
	case N3_SP_KNIGHTS_APPOINT_OFFICER: // 장교임명 - 가입허가와 같음
		{
			::_LoadStringFromResource(IDS_KNIGHTS_APPOINT_OFFICER_SUCCESS, szMsg); // 성공
		}
		break;
	case N3_SP_KNIGHTS_LIST: //모든 리스트 요청 Send - | s1(Knights Count) Loop { s1(Knights ID) s1(Name Length) str1 (Name) }
		{
			if(false == m_pUIKnightsOp->IsVisible())
			{
				m_pUIKnightsOp->Open(s_pPlayer->m_InfoExt.eKnightsDuty); // UI 열고 리스트등 초기화..
			}
			else 
			{
				m_pUIKnightsOp->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty); // UI 열고 리스트등 초기화..
			}
			m_pUIKnightsOp->MsgRecv_KnightsList(pDataPack, iOffset);
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_INFO_ALL: // 전체 멤버 Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
		{
			m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pDataPack, iOffset);
		}
		break;
	case N3_SP_KNIGHTS_MEMBER_INFO_ONLINE: //현재 접속 리스트 Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
		{
			m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pDataPack, iOffset);
		}
		break;
	case N3_SP_KNIGHTS_STASH: //기사단 창고
		{
		}
		break;
	case N3_SP_KNIGHTS_DUTY_CHANGE: // 직위 변경..
		{
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

			if(iID == s_pPlayer->IDNumber()) // 내 직위 변경..
			{
				m_pUIVar->m_pPageKnights->UpdateKnightsDuty(eDuty);
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(iID, false);
				if(pUPC) // 다른넘 직위 변경..
				{
				}
			}
		}
		break;
	default:
		__ASSERT(0, "Invalid Knights SubPacket");
		break;
*/	}
}

void CGameProcMain::MsgRecv_KnightsListBasic(DataPack* pDataPack, int& iOffset) // 기사단 기본 정보 받기..
{
	e_SubPacket_KnightsList eSP = (e_SubPacket_KnightsList)(CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset));	// Sub Packet
	switch(eSP)
	{
	case N3_SP_KNIGHTS_LIST_BASIC_ALL: // Receive - s1(knights Count) { s21(id, 이름길이), str1(이름) }
		{
			int iCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	// 기사단 갯수
			for(int i = 0; i < iCount; i++)
			{
				std::string szID;
				int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// 기사단 ID
				int iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// ID 문자열 길이..
				CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);	// ID 문자열..

				m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // 기사단 정보 모든 걸 받는다..
			}
		}
		break;
	case N3_SP_KNIGHTS_LIST_BASIC_INSERT: // Receive - s2(id, 이름길이), str1(이름)
		{
			std::string szID;
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// 기사단 ID
			int iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// ID 문자열 길이..
			CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);	// ID 문자열..

			m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // 기사단 정보 추가..
		}
		break;
	case N3_SP_KNIGHTS_LIST_BASIC_REMOVE: // Receive - s1(id)
		{
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// 기사단 ID
			m_pUIKnightsOp->KnightsInfoDelete(iID); // 기사단 정보 지우기..
		}
		break;
	}
}

/*
void CGameProcMain::MsgRecv_CompressedPacket(DataPack* pDataPack, int& iOffset) // 압축된 데이터 이다... 한번 더 파싱해야 한다!!!
{
	//short sCompLen, sOrgLen;
	//DWORD dwCrcValue;
	//sCompLen =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);	// 압축된 데이타길이얻기... (Obtain compressed data length)
	//sOrgLen =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// 원래데이타길이얻기... (Getting the original data length)
	//dwCrcValue =	CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);	// CRC값 얻기... (Getting CRC value)

	// TEMP: just to easily get the packet from the watch window
	//char temp[0x0023];
	//memcpy(temp, (char*)(pDataPack->m_pData+iOffset), sCompLen);

	/// 압축 데이터 얻기 및 해제 (Obtaining and decompress data)	
	//CCompressMng Compressor;
	//Compressor.PreUncompressWork((char*)(pDataPack->m_pData+iOffset), sCompLen, sOrgLen);	// 압축 풀기... (Extracting)
	//iOffset += sCompLen;

	//if (Compressor.Extract() == false || 
//		Compressor.m_nErrorOccurred != 0 ||
		//dwCrcValue != Compressor.m_dwCrc )
	//{
//		return;
//	}

	// 압축 풀린 데이타 읽기 (Read loose data compression)
//	BYTE* pDecodeBuf = (BYTE*)(Compressor.m_pOutputBuffer);

	Uint16 compressedLength = CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset);
	Uint16 originalLength = CAPISocket::Parse_GetWord(pDataPack->m_pData, iOffset);
	Uint32 crc = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

	BYTE* pDecodeBuf = new BYTE[originalLength];

	Uint32 result = lzf_decompress((char*)(pDataPack->m_pData + iOffset), compressedLength, pDecodeBuf, originalLength);
	if (result
		!= originalLength)
	{
		delete[] pDecodeBuf;
		return;
	}

	// 임시로 데이터 팩 만들고.. (Create temporary data pack)
	DataPack DataPackTemp;
	DataPackTemp.m_Size = originalLength;
	DataPackTemp.m_pData = pDecodeBuf;
	int iOffset2 = 0;
	this->ProcessPacket(&DataPackTemp, iOffset2); // 바로 파싱... (Just parsing)
	DataPackTemp.m_Size = 0;
	DataPackTemp.m_pData = NULL;
}
*/

void CGameProcMain::MsgRecv_ContinousPacket(DataPack* pDataPack, int& iOffset) // 압축된 데이터 이다... 한번 더 파싱해야 한다!!!
{
	int iWholeSize = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// 원래데이타길이얻기...
	int iOffset2 = iOffset, iOffsetPrev = 0, iSizeThisPacket = 0;
	int i = 0;
	while(iOffset2 < iWholeSize)
	{
		iOffsetPrev = iOffset2;
		iSizeThisPacket = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset2);		// 원래데이타길이얻기...

		if(iSizeThisPacket <= 0 || iSizeThisPacket >= iWholeSize)
		{
			char szErr[256];
			sprintf(szErr, "연속 패킷 받기 오류 - 순서(%d) 크기(%d/%d)", i, iSizeThisPacket, iWholeSize);
			CGameProcedure::ReportDebugStringAndSendToServer(szErr);
			__ASSERT(0, szErr);
			break; // 멈춘다!!
		}
		i++;

		this->ProcessPacket(pDataPack, iOffset2);
		iOffset2 = iOffsetPrev + iSizeThisPacket + 2;
	}

	iOffset += iWholeSize;
}

void CGameProcMain::MsgRecv_WareHouse(DataPack* pDataPack, int& iOffset)			// 보관함 관련 패킷..
{
	BYTE	bResult, bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch ( (e_SubPacket_WareHouse)bSubCom )
	{
		case N3_SP_WARE_INN:
			m_pUIInn->SetVisible(true);
			break;

		case N3_SP_WARE_OPEN:
			MsgRecv_WareHouseOpen(pDataPack, iOffset);
			break;

		case N3_SP_WARE_GET_IN:
			bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultToWareMsg(bResult);
			break;

		case N3_SP_WARE_GET_OUT:
			bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultFromWareMsg(bResult);
			break;

		case N3_SP_WARE_WARE_MOVE:
			bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultWareToWareMsg(bResult);
			break;

		case N3_SP_WARE_INV_MOVE:
			bResult = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 0x01 : true, 0x00 : false..
			if (m_pUIWareHouseDlg) m_pUIWareHouseDlg->ReceiveResultInvToInvMsg(bResult);
			break;
	}
}

void CGameProcMain::MsgRecv_WareHouseOpen(DataPack* pDataPack, int& iOffset)		// 보관함 오픈..
{
	if (m_pUIWareHouseDlg->IsVisible())
		return;

	Uint8 idk = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	int iWareGold, iItemID, iItemDurability, iItemCount;
	iWareGold		= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
	m_pUIWareHouseDlg->EnterWareHouseStateStart(iWareGold);

	for ( int i = 0; i < MAX_ITEM_WARE_PAGE*MAX_ITEM_TRADE; i++ )				// 슬롯 갯수마큼..
	{
		iItemID			= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);
		iItemDurability	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		iItemCount		= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		m_pUIWareHouseDlg->AddItemInWare( iItemID, iItemDurability, iItemCount, i );
	}

	if (!m_pUIWareHouseDlg->IsVisible())
		m_pUIWareHouseDlg->SetVisible(true);

	if (m_pUIInventory->IsVisible())		// 인벤토리가 안열려 있으면..
		this->CommandToggleUIInventory();

	if (m_pUISkillTreeDlg->IsVisible())
		m_pUISkillTreeDlg->Close();

	m_pUIWareHouseDlg->EnterWareHouseStateEnd();
}

void CGameProcMain::PlayBGM_Town()
{
	if(m_pSnd_Battle) m_pSnd_Battle->Stop(3.0f);
	if(NULL == m_pSnd_Town || m_pSnd_Town->IsPlaying()) return;
	m_pSnd_Town->SetMaxVolume(60);
	m_pSnd_Town->Play(NULL, 3.0f); // 전투 음악 설정.. 해제는 주위에 몬스터가 없을때 한다..
}

void CGameProcMain::PlayBGM_Battle()
{
	if(m_pSnd_Town) m_pSnd_Town->Stop(3.0f);
	if(NULL == m_pSnd_Battle || m_pSnd_Battle->IsPlaying()) return;
	m_pSnd_Battle->SetMaxVolume(80);
	m_pSnd_Battle->Play(NULL, 3.0f); // 전투 음악 설정.. 해제는 주위에 몬스터가 없을때 한다..
}

void CGameProcMain::ReleaseSound()
{
	if(ACT_WORLD && ACT_WORLD->GetSkyRef()) ACT_WORLD->GetSkyRef()->ReleaseSound();
	CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Town);
	CN3Base::s_SndMgr.ReleaseStreamObj(&m_pSnd_Battle);
}

void CGameProcMain::MsgRecv_NpcChangeOpen(DataPack* pDataPack, int& iOffset)		// Class Change와 초기화..
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch (bSubCom)
	{
		case N3_SP_CLASS_CHANGE_PURE:
			m_pUINpcChange->Open();
			break;

		case N3_SP_CLASS_CHANGE_REQ:
			this->MsgRecv_ClassChange(pDataPack, iOffset);
			break;

		case N3_SP_CLASS_ALL_POINT:
			this->MsgRecv_AllPointInit(pDataPack, iOffset);
			break;

		case N3_SP_CLASS_SKILL_POINT:
			this->MsgRecv_SkillPointInit(pDataPack, iOffset);
			break;

		case N3_SP_CLASS_POINT_CHANGE_PRICE_QUERY:
			this->MsgRecv_PointChangePriceQueryRequest(pDataPack, iOffset);
			break;
	}
}

void CGameProcMain::MsgRecv_AllPointInit(DataPack* pDataPack, int& iOffset)			// All Point 초기화..
{
	BYTE	bType		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	
	DWORD	dwGold		= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);	

	char szBuf[256] = "";
	std::string szMsg; 

	switch (bType)
	{
		case 0x00:	// 돈이 부족..
			szMsg = "IDS_POINTINIT_NOT_ENOUGH_NOAH (%d)";//::_LoadStringFromResource(IDS_POINTINIT_NOT_ENOUGH_NOAH, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGold);
			CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
			break;

		case 0x01:	// 성공..
			s_pPlayer->m_InfoExt.iStrength = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			m_pUIVar->m_pPageState->UpdateStrength(s_pPlayer->m_InfoExt.iStrength, s_pPlayer->m_InfoExt.iStrength_Delta);

			s_pPlayer->m_InfoExt.iStamina = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			m_pUIVar->m_pPageState->UpdateStamina(s_pPlayer->m_InfoExt.iStamina, s_pPlayer->m_InfoExt.iStamina_Delta);

			s_pPlayer->m_InfoExt.iDexterity = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			m_pUIVar->m_pPageState->UpdateDexterity(s_pPlayer->m_InfoExt.iDexterity, s_pPlayer->m_InfoExt.iDexterity_Delta);

			s_pPlayer->m_InfoExt.iIntelligence = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			m_pUIVar->m_pPageState->UpdateIntelligence(s_pPlayer->m_InfoExt.iIntelligence, s_pPlayer->m_InfoExt.iIntelligence_Delta);

			s_pPlayer->m_InfoExt.iMagicAttak = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			m_pUIVar->m_pPageState->UpdateMagicAttak(s_pPlayer->m_InfoExt.iMagicAttak, s_pPlayer->m_InfoExt.iMagicAttak_Delta);

			s_pPlayer->m_InfoBase.iHPMax =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			s_pPlayer->m_InfoExt.iMSPMax =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			s_pPlayer->m_InfoExt.iAttack =		CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			s_pPlayer->m_InfoExt.iWeightMax =	CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

			m_pUIVar->m_pPageState->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax);
			m_pUIStateBarAndMiniMap->UpdateHP(s_pPlayer->m_InfoBase.iHP, s_pPlayer->m_InfoBase.iHPMax, false);
			
			m_pUIVar->m_pPageState->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax);
			m_pUIStateBarAndMiniMap->UpdateMSP(s_pPlayer->m_InfoExt.iMSP, s_pPlayer->m_InfoExt.iMSPMax, false);

			m_pUIVar->m_pPageState->UpdateAttackPoint(s_pPlayer->m_InfoExt.iAttack, s_pPlayer->m_InfoExt.iAttack_Delta);
			m_pUIVar->m_pPageState->UpdateWeight(s_pPlayer->m_InfoExt.iWeight, s_pPlayer->m_InfoExt.iWeightMax);

			s_pPlayer->m_InfoExt.iBonusPointRemain = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 남은 보너스 포인트..
			m_pUIVar->m_pPageState->UpdateBonusPointAndButtons(s_pPlayer->m_InfoExt.iBonusPointRemain); // 보너스 포인트 적용이 가능한가??

			// 돈 변경.. 인벤토리는 바꾸고 상거래.. 개인 거래와는 배타적..
			s_pPlayer->m_InfoExt.iGold = dwGold;
			if (m_pUIInventory->IsVisible())
				m_pUIInventory->GoldUpdate();
			if (m_pUITransactionDlg->IsVisible())
				m_pUITransactionDlg->GoldUpdate();
			if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
				m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();

			break;

		case 0x02:	// Already..
			szMsg = "IDS_POINTINIT_ALREADY";//::_LoadStringFromResource(IDS_POINTINIT_ALREADY, szMsg);
			sprintf(szBuf, szMsg.c_str());
			CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
			break;
	}
}

void CGameProcMain::MsgRecv_SkillPointInit(DataPack* pDataPack, int& iOffset)		// Skill Point 초기화..
{
	BYTE	bType		= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	
	DWORD	dwGold		= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);	
	int i;
	char szBuf[256] = "";
	std::string szMsg; 

	switch (bType)
	{
		case 0x00:	// 돈이 부족..
			szMsg = "IDS_POINTINIT_NOT_ENOUGH_NOAH (%d)";//::_LoadStringFromResource(IDS_POINTINIT_NOT_ENOUGH_NOAH, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGold);
			CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
			break;

		case 0x01:	// 성공..
			m_pUISkillTreeDlg->m_iSkillInfo[0] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			for ( i = 1; i < 9; i++ )
				m_pUISkillTreeDlg->m_iSkillInfo[i] = 0;
			m_pUISkillTreeDlg->InitIconUpdate();

			// 돈 변경.. 인벤토리는 바꾸고 상거래.. 개인 거래와는 배타적..
			s_pPlayer->m_InfoExt.iGold = dwGold;
			if (m_pUIInventory->IsVisible())
				m_pUIInventory->GoldUpdate();
			if (m_pUITransactionDlg->IsVisible())
				m_pUITransactionDlg->GoldUpdate();
			if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
				m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();

			if (m_pUIHotKeyDlg)
				m_pUIHotKeyDlg->AllFactorClear();
			break;

		case 0x02:	// Already..
			szMsg = "IDS_POINTINIT_ALREADY";//::_LoadStringFromResource(IDS_POINTINIT_ALREADY, szMsg);
			sprintf(szBuf, szMsg.c_str());
			CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
			break;
	}
}

void CGameProcMain::MsgRecv_PointChangePriceQueryRequest(DataPack* pDataPack, int& iOffset)		// 가격에 대한 응답 패킷..
{
	DWORD	dwGold		= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);	
	m_pUINpcChange->ReceivePriceFromServer(dwGold);
}

void CGameProcMain::MsgRecv_NoahChange(DataPack* pDataPack, int& iOffset)		// 노아 변경..
{
	BYTE	bType				= CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	
	DWORD	dwGoldOffset	= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);		
	DWORD	dwGold			= CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);	

	char szBuf[256] = "";
	std::string szMsg; 

	switch (bType)
	{
		case N3_SP_NOAH_GET:
			szMsg = "IDS_NOAH_CHANGE_GET (%d)";//::_LoadStringFromResource(IDS_NOAH_CHANGE_GET, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
			CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xff6565ff);
			break;

		case N3_SP_NOAH_LOST:
			szMsg = "IDS_NOAH_CHANGE_LOST (%d)";//::_LoadStringFromResource(IDS_NOAH_CHANGE_LOST, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
			CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
			break;

		case N3_SP_NOAH_SPEND:
			szMsg = "IDS_NOAH_CHANGE_LOST (%d)";//::_LoadStringFromResource(IDS_NOAH_CHANGE_LOST, szMsg);
			sprintf(szBuf, szMsg.c_str(), dwGoldOffset);
			CGameProcedure::s_pProcMain->MsgOutput(szBuf, 0xffff3b3b);
			break;
	}

	// 돈 변경.. 인벤토리는 바꾸고 상거래.. 개인 거래와는 배타적..
	s_pPlayer->m_InfoExt.iGold = dwGold;
	if (m_pUIInventory->IsVisible())
		m_pUIInventory->GoldUpdate();
	if (m_pUITransactionDlg->IsVisible())
		m_pUITransactionDlg->GoldUpdate();
	if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
		m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();
}

void CGameProcMain::MsgRecv_WarpList(DataPack* pDataPack, int& iOffset)		// 워프 리스트 - 존 체인지가 될 수도 있다..
{
	int iByte = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	m_pUIWarp->Reset();

	int iStrLen = 0;

	int iListCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	for(int i = 0; i < iListCount; i++)
	{
		__WarpInfo WI;
		
		WI.iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 워프 ID
		iStrLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 이름 길이
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, WI.szName, iStrLen); // 이름
		iStrLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 동의문 길이
		CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, WI.szAgreement, iStrLen); // 동의문
		WI.iZone = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);				// 존번호
		WI.iMaxUser = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);			// 최대 유저 카운트.
		WI.iGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);				// 돈
		WI.vPos.x = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;	// 좌표 
		WI.vPos.z = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;	//
		WI.vPos.y = (CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset))/10.0f;	// 

		m_pUIWarp->InfoAdd(WI);
	}

	m_pUIWarp->UpdateList();
	m_pUIWarp->SetVisible(true);
}

/*
void CGameProcMain::MsgRecv_ServerCheckAndRequestConcurrentUserCount(DataPack* pDataPack, int& iOffset)	// 서버 IP 와 포트를 받아 동접자를 체크해 본다..
{
	std::string szIP;
	int iStrLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // IP..
	CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szIP, iStrLen);
	DWORD dwPort = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // Port

	__WarpInfo WI;
	if(m_pUIWarp->InfoGetCur(WI) < 0) return;

	bool bNeedConnectSubSocket = (szIP != s_pSocket->GetCurrentIP() || dwPort != s_pSocket->GetCurrentPort()); // 접속해야 할 IP 와 포트가 똑같은지

	if(bNeedConnectSubSocket) // 서브 소켓으로 접속해야 하면..
	{
		int iErr = s_pSocketSub->Connect(s_hWndSubSocket, szIP.c_str(), dwPort); // 서브 소켓으로 접속해서..
		if(iErr)
		{
			this->ReportServerConnectionFailed(WI.szName, iErr, false);
			return;
		}
	}

	// 동접자 체크..
	int iOffsetSend = 0;
	BYTE byBuff[8];
	
	CAPISocket::MP_AddByte(byBuff, iOffsetSend, N3_SERVER_CONCURRENT_CONNECT);
	CAPISocket::MP_AddShort(byBuff, iOffsetSend, WI.iZone);
	CAPISocket::MP_AddByte(byBuff, iOffsetSend, s_pPlayer->m_InfoBase.eNation); // 국가별 동접수..

	if(bNeedConnectSubSocket) s_pSocketSub->Send(byBuff, iOffsetSend); // 서브 소켓으로 보내기.
	else s_pSocket->Send(byBuff, iOffsetSend); // 본 소켓으로 보내기..
}


void CGameProcMain::MsgRecv_ConcurrentUserCountAndSendServerCheck(DataPack* pDataPack, int& iOffset)			// 동접자를 받고 서버에 접속하겠다는 패킷을 보낸다.
{
	int iConcurrentUser = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // IP..
	if(s_pSocketSub->IsConnected()) s_pSocketSub->Disconnect();

	__WarpInfo WI;
	if(m_pUIWarp->InfoGetCur(WI) < 0) return;

	if(iConcurrentUser < WI.iMaxUser) // 동접 제한보다 적으면..
	{
		int iOffsetSend = 0;
		BYTE byBuff[8];
		
		CAPISocket::MP_AddByte(byBuff, iOffsetSend, N3_SERVER_CHECK);
		CAPISocket::MP_AddShort(byBuff, iOffsetSend, WI.iID);

		s_pSocket->Send(byBuff, iOffsetSend);
	}
	else
	{
		std::string szMsg; ::_LoadStringFromResource(IDS_MSG_CONCURRENT_USER_OVERFLOW, szMsg); // 동시 접속 제한 초과..
		this->MsgOutput(szMsg, 0xffff0000);
	}
}
*/

void CGameProcMain::MsgRecv_Knights_Create(DataPack* pDataPack, int& iOffset)
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch ( (e_SubPacket_KNights_Create)bSubCom )
	{
		case N3_SP_KNIGHTS_CREATE_SUCCESS:
			{
				int sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				std::string szID;
				int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// 기사단 ID
				int iLen = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);		// ID 문자열 길이..
				CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szID, iLen);	// ID 문자열..
				int iGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 등급
				int iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// 순위
				DWORD dwGold = CAPISocket::Parse_GetDword(pDataPack->m_pData, iOffset);

				if(s_pPlayer->IDNumber()==sid)
				{
					m_pUIInn->Message(IDS_CLAN_MAKE_SUCCESS);

					// 돈 변경.. 인벤토리는 바꾸고 상거래.. 개인 거래와는 배타적..
					s_pPlayer->m_InfoExt.iGold = dwGold;
					if (m_pUIInventory->IsVisible()) m_pUIInventory->GoldUpdate();
					if (m_pUITransactionDlg->IsVisible()) m_pUITransactionDlg->GoldUpdate();
					if (m_pSubProcPerTrade && m_pSubProcPerTrade->m_pUIPerTradeDlg->IsVisible())
						m_pSubProcPerTrade->m_pUIPerTradeDlg->GoldUpdate();

					//기사단(클랜)UI업데이트...해라...
					s_pPlayer->m_InfoExt.eKnightsDuty = KNIGHTS_DUTY_CHIEF;
					s_pPlayer->KnightsInfoSet(iID, szID, iGrade, iRank);
					m_pUIVar->UpdateKnightsInfo();

					if(m_pUIVar->m_pPageKnights->IsVisible())
					{
						m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
						m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
					}

					//m_pUIKnightsOp->KnightsInfoInsert(iID, szID); // 기사단 정보 추가..
				}
				else
				{
					CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);
					if(pUPC)
						pUPC->KnightsInfoSet(iID, szID, iGrade, iRank);
				}
			}
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_LOWLEVEL:
			m_pUIInn->Message(IDS_CLAN_DENY_LOWLEVEL);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_DUPLICATEDNAME:
			m_pUICreateClanName->Open(IDS_CLAN_REINPUT_NAME);
			break;

		case N3_SP_KNIGHTS_CREATE_FAIL_LOWMONEY:
			m_pUIInn->Message(IDS_CLAN_DENY_LOWGOLD);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_ALREADYJOINED:
			m_pUIInn->Message(IDS_CLAN_DENY_ALREADYJOINED);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_INVALIDDAY:
			m_pUIInn->Message(IDS_CLAN_DENY_INVALIDDAY);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_DBFAIL:
		case N3_SP_KNIGHTS_CREATE_FAIL_UNKNOWN:
			m_pUIInn->Message(IDS_CLAN_DENY_UNKNOWN);
			break;
		case N3_SP_KNIGHTS_CREATE_FAIL_INVALIDSERVER:
			m_pUIInn->Message(IDS_CLAN_DENY_INVALID_SERVER);
			break;
	}
}

void CGameProcMain::MsgRecv_Knights_Withdraw(DataPack* pDataPack, int& iOffset)
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL:
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		{
			int sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.iKnightsID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				s_pPlayer->m_InfoExt.eKnightsDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
				m_pUIVar->UpdateKnightsInfo();

				s_pPlayer->KnightsInfoSet(s_pPlayer->m_InfoExt.iKnightsID, "", 0, 0);
				szMsg = "IDS_CLAN_WITHDRAW_SUCCESS";//::_LoadStringFromResource(IDS_CLAN_WITHDRAW_SUCCESS, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);

				if(m_pUIVar->m_pPageKnights->IsVisible())
				{
					m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
					m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
				}
			}
			else
			{
				int iKnightsID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
				e_KnightsDuty eKnightsDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);
				if(pUPC)
				{
					//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(iKnightsID);
					//if(pKIB) pUPC->KnightsNameSet(pKIB->szName, 0xffff0000);
					//else pUPC->KnightsNameSet("", 0xffff0000);
					pUPC->KnightsInfoSet(iKnightsID, "", 0, 0);
				}
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER:
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT:
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		szMsg = "IDS_CLAN_WITHDRAW_FAIL";//::_LoadStringFromResource(IDS_CLAN_WITHDRAW_FAIL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		szMsg = "IDS_CLAN_COMMON_FAIL_BATTLEZONE";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}
}

void CGameProcMain::MsgRecv_Knights_Join(DataPack* pDataPack, int& iOffset)
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB검색 실패..
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS: //클랜가입 성공
		{
			int sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			int iL = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 소속 기사단 이름 길이.
			std::string szKnightsName;
			CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iL);
			int iGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 등급
			int iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// 순위

			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
				s_pPlayer->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
				m_pUIVar->UpdateKnightsInfo();

				szMsg = "IDS_CLAN_JOIN_SUCCESS";//::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);

				if(m_pUIVar->m_pPageKnights->IsVisible())
				{
					m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
					m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
				}
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);								
				if(pUPC)
					pUPC->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//없는 유저..
		szMsg = "IDS_CLAN_JOIN_FAIL_NONE_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:	//상대유저가 죽어 있음..
		szMsg = "IDS_CLAN_JOIN_FAIL_DEAD_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //상대유저의 국가가 다름..
		szMsg = "IDS_CLAN_JOIN_FAIL_ENEMY_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //상대유저가 이미 다른 클랜이나 기사단에 가입되어 있음..
		szMsg = "IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //권한이 없음..
		szMsg = "IDS_CLAN_JOIN_FAIL_INVALIDRIGHT";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:	//존재하지 않는 기사단..									
		szMsg = "IDS_CLAN_JOIN_FAIL_NONE_CLAN";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:	//인원이 풀..
		szMsg = "IDS_CLAN_JOIN_FAIL_CLAN_FULL";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
		szMsg = "IDS_CLAN_COMMON_FAIL_ME";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		szMsg = "IDS_CLAN_COMMON_FAIL_NOTJOINED";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_REJECT:
		szMsg = "IDS_CLAN_JOIN_REJECT";//::_LoadStringFromResource(IDS_CLAN_JOIN_REJECT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		szMsg = "IDS_CLAN_COMMON_FAIL_BATTLEZONE";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}	
}

void CGameProcMain::MsgRecv_Knights_Leave(DataPack* pDataPack, int& iOffset)
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB검색 실패..
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS: //클랜탈퇴 성공
		{
			int sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
			int iL = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 소속 기사단 이름 길이.
			std::string szKnightsName;
			CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iL);
			int iGrade = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);	// 등급
			int iRank = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);		// 순위

			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
				s_pPlayer->KnightsInfoSet(iID, szKnightsName, iGrade, iRank);
				m_pUIVar->UpdateKnightsInfo();
				
				szMsg = "IDS_CLAN_JOIN_SUCCESS";//::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
				this->MsgOutput(szMsg, 0xffffff00);

				if(m_pUIVar->m_pPageKnights->IsVisible())
				{
					m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
					m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
				}
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);								
				if(pUPC)
					pUPC->KnightsInfoSet(iID, "", 0, 0);
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//없는 유저..
		szMsg = "IDS_CLAN_JOIN_FAIL_NONE_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:	//상대유저가 죽어 있음..
		szMsg = "IDS_CLAN_JOIN_FAIL_DEAD_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //상대유저의 국가가 다름..
		szMsg = "IDS_CLAN_JOIN_FAIL_ENEMY_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //상대유저가 이미 다른 클랜이나 기사단에 가입되어 있음..
		szMsg = "IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //권한이 없음..
		szMsg = "IDS_CLAN_JOIN_FAIL_INVALIDRIGHT";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:	//존재하지 않는 기사단..									
		szMsg = "IDS_CLAN_JOIN_FAIL_NONE_CLAN";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:	//인원이 풀..
		szMsg = "IDS_CLAN_JOIN_FAIL_CLAN_FULL";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
		szMsg = "IDS_CLAN_COMMON_FAIL_ME";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		szMsg = "IDS_CLAN_COMMON_FAIL_NOTJOINED";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		szMsg = "IDS_CLAN_COMMON_FAIL_BATTLEZONE";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}	
}

void CGameProcMain::MsgRecv_Knights_AppointViceChief(DataPack* pDataPack, int& iOffset)
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB검색 실패..
		break;
	case N3_SP_KNIGHTS_COMMON_SUCCESS: //클랜가입 성공
		{
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

			s_pPlayer->m_InfoExt.iKnightsID = iID;
			s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
			m_pUIVar->UpdateKnightsInfo();

			szMsg = "IDS_CLAN_JOIN_SUCCESS";//::_LoadStringFromResource(IDS_CLAN_JOIN_SUCCESS, szMsg);
			this->MsgOutput(szMsg, 0xffffff00);

			if(m_pUIVar->m_pPageKnights->IsVisible())
			{
				m_pUIVar->m_pPageKnights->MsgSend_MemberInfoAll();
				m_pUIVar->m_pPageKnights->ChangeUIByDuty(s_pPlayer->m_InfoExt.eKnightsDuty);
			}
		}
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER:	//없는 유저..
		szMsg = "IDS_CLAN_JOIN_FAIL_NONE_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER:	//상대유저가 죽어 있음..
		szMsg = "IDS_CLAN_JOIN_FAIL_DEAD_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_DEAD_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER: //상대유저의 국가가 다름..
		szMsg = "IDS_CLAN_JOIN_FAIL_ENEMY_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_ENEMY_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER: //상대유저가 이미 다른 클랜이나 기사단에 가입되어 있음..
		szMsg = "IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_OTHER_CLAN_USER, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT: //권한이 없음..
		szMsg = "IDS_CLAN_JOIN_FAIL_INVALIDRIGHT";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_INVALIDRIGHT, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN:	//존재하지 않는 기사단..									
		szMsg = "IDS_CLAN_JOIN_FAIL_NONE_CLAN";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_NONE_CLAN, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_FULL:	//인원이 풀..
		szMsg = "IDS_CLAN_JOIN_FAIL_CLAN_FULL";//::_LoadStringFromResource(IDS_CLAN_JOIN_FAIL_CLAN_FULL, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_ME:
		szMsg = "IDS_CLAN_COMMON_FAIL_ME";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_ME, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED:
		szMsg = "IDS_CLAN_COMMON_FAIL_NOTJOINED";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_NOTJOINED, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	case N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE:
		szMsg = "IDS_CLAN_COMMON_FAIL_BATTLEZONE";//::_LoadStringFromResource(IDS_CLAN_COMMON_FAIL_BATTLEZONE, szMsg);
		this->MsgOutput(szMsg, 0xffffff00);
		break;
	}	
}

void CGameProcMain::MsgRecv_Knights_MemberInfoAll(DataPack* pDataPack, int& iOffset)
{
	BYTE	bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	std::string szMsg;
	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		m_pUIVar->m_pPageKnights->MsgRecv_MemberInfo(pDataPack, iOffset);
		break;
	case N3_SP_KNIGHTS_COMMON_DBFAIL: //DB검색 실패..
	default:
		break;		
	}
}

void CGameProcMain::MsgRecv_Knights_GradeChangeAll(DataPack* pDataPack, int& iOffset)
{
	int iCount = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
	if(iCount <= 0) return;
	
	std::vector<int> iIDs(iCount, 0);
	std::vector<int> iGrades(iCount, 0);
	std::vector<int> iRanks(iCount, 0);

	for(int i = 0; i < iCount; i++)
	{
		iIDs[i] = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
		iGrades[i] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
		iRanks[i] = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);
	}

	it_UPC it = s_pOPMgr->m_UPCs.begin(), itEnd = s_pOPMgr->m_UPCs.end();
	for(; it != itEnd; it++)
	{
		CPlayerOther* pUPC = it->second;
		if(NULL == pUPC) continue;

		int iIDTmp = pUPC->m_InfoExt.iKnightsID;
		if(iIDTmp <= 0) continue;

		for(int i = 0; i < iCount; i++)
		{
			if(iIDs[i] == iIDTmp)
			{
				pUPC->KnightsInfoSet(iIDTmp, pUPC->m_InfoExt.szKnights, iGrades[i], iRanks[i]);
				break;
			}
		}
	}

}

void CGameProcMain::MsgRecv_Knights_Duty_Change(DataPack* pDataPack, int& iOffset)
{
	BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		{
			int sid = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			int iID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			e_KnightsDuty eDuty = (e_KnightsDuty)CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

			if(s_pPlayer->IDNumber()==sid)
			{
				s_pPlayer->m_InfoExt.iKnightsID = iID;
				s_pPlayer->m_InfoExt.eKnightsDuty = eDuty;
				m_pUIVar->UpdateKnightsInfo();
				if(s_pPlayer->m_InfoExt.iKnightsID == 0)
					s_pPlayer->KnightsInfoSet(0, "", 0, 0);
				//std::string szName;
				//__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(s_pPlayer->m_InfoExt.iKnightsID);
				//if(pKIB) m_pUIVar->m_pPageKnights->UpdateKnightsName(pKIB->szName);
				//else m_pUIVar->m_pPageKnights->UpdateKnightsName("");
			}
			else
			{
				CPlayerOther* pUPC = s_pOPMgr->UPCGetByID(sid, true);								
				if(pUPC)
					if(iID == 0) pUPC->KnightsInfoSet(0, "", 0, 0);
			}
		}
		break;
	}
}

void CGameProcMain::MsgRecv_Knigts_Join_Req(DataPack* pDataPack, int& iOffset)
{
	BYTE bSubCom = CAPISocket::Parse_GetByte(pDataPack->m_pData, iOffset);

	switch ( (e_SubPacket_KNights_Common)bSubCom )
	{
	case N3_SP_KNIGHTS_COMMON_SUCCESS:
		{
			m_iJoinReqClanRequierID = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);
			m_iJoinReqClan = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);

			int iL = CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset); // 소속 기사단 이름 길이.
			std::string szKnightsName;
			CAPISocket::Parse_GetString(pDataPack->m_pData, iOffset, szKnightsName, iL);

//			std::string szName;
//			__KnightsInfoBase* pKIB = m_pUIKnightsOp->KnightsInfoFind(m_iJoinReqClan);
//			if(!pKIB)
//			{
//				MsgSend_KnightsJoinReq(false);
//				break;
//			}

			std::string szMsg;
			szMsg = "IDS_CLAN_JOIN_REQ"; //::_LoadStringFromResource(IDS_CLAN_JOIN_REQ, szMsg);

			char szTmp[256];
			sprintf(szTmp, szMsg.c_str(), szKnightsName.c_str());
			
			CGameProcedure::s_pProcMain->MessageBoxPost(szTmp, "", MB_YESNO, BEHAVIOR_CLAN_JOIN);			
		}
		break;
	}
}

int CGameProcMain::MsgRecv_VersionCheck(DataPack* pDataPack, int& iOffset) // virtual
{
	int iVersion = CGameProcedure::MsgRecv_VersionCheck(pDataPack, iOffset);
	this->MsgSend_CharacterSelect(); // virtual

	return iVersion;
}

bool CGameProcMain::MsgRecv_CharacterSelect(DataPack* pDataPack, int& iOffset) // virtual
{
	bool bSuccess = CGameProcedure::MsgRecv_CharacterSelect(pDataPack, iOffset);

	//전쟁존에서 죽어서 서버 체인지 하는 경우는 다시 값을 세팅해준다.
	if(s_pPlayer->IsDead())
	{
		this->InitPlayerPosition(s_pPlayer->Position()); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.
		s_pPlayer->RegenerateCollisionMesh(); // 충돌 메시를 다시 만든다..
		s_pPlayer->m_iSendRegeneration = 0; // 한번 보내면 다시 죽을때까지 안보내는 플래그
		s_pPlayer->m_fTimeAfterDeath = 0; // 한번 보내면 다시 죽을때까지 안보내는 플래그

		//
		//마법 & 효과 초기화..
		if(m_pUIStateBarAndMiniMap) m_pUIStateBarAndMiniMap->ClearMagic();
		if(m_pMagicSkillMng) m_pMagicSkillMng->ClearDurationalMagic();
		if(CGameProcedure::s_pFX) s_pFX->StopMine();

		CLogWriter::Write("Receive Regeneration");

		if(s_pPlayer->Nation()==NATION_KARUS) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_KARUS, s_pPlayer->IDNumber(), -1);
		else if(s_pPlayer->Nation()==NATION_ELMORAD) CGameProcedure::s_pFX->TriggerBundle(s_pPlayer->IDNumber(), -1, FXID_REGEN_ELMORAD, s_pPlayer->IDNumber(), -1);
	}
			

	this->InitZone(s_pPlayer->m_InfoExt.iZoneCur, s_pPlayer->Position()); // 존을 바꾼다..
	s_pProcMain->MsgSend_GameStart(); // 존로딩이 끝나면 게임 시작..

	return bSuccess;
}

void CGameProcMain::MsgRecv_Corpse(DataPack *pDataPack, int &iOffset)
{
	int iID	= CAPISocket::Parse_GetShort(pDataPack->m_pData, iOffset);//regen하고자 하는 유저의 아이디

	if( s_pPlayer->IDNumber() != iID )
	{
		s_pOPMgr->CorpseAdd( iID );

		if( s_pPlayer->m_iIDTarget == iID )
			s_pPlayer->m_iIDTarget = -1; //만약 내가 regen한 캐릭터에 포커스를 가지고 있다면 초기화 해준다.
	}
}

void CGameProcMain::MsgSend_PerTradeBBSReq(std::string szName, int iDestID)
{
	if(	!m_pUITransactionDlg->IsVisible() ) //// 개인간 아이템 거래.. // 상거래 중이 아니면..
	{
		std::string szMsg;
		szMsg = " IDS_PERSONAL_TRADE_REQUEST";//::_LoadStringFromResource(IDS_PERSONAL_TRADE_REQUEST, szMsg);
		MsgOutput(szName + szMsg, 0xffffff00);

		MsgSend_PerTradeReq(iDestID, false);

		if (m_pUINpcEvent->IsVisible())
			m_pUINpcEvent->Close();
		m_pSubProcPerTrade->EnterWaitMsgFromServerStatePerTradeReq(szName);
	}
}

void CGameProcMain::MsgSend_CharacterSelect() // virtual
{
	CGameProcedure::MsgSend_CharacterSelect();
	
	m_pMagicSkillMng->ClearDurationalMagic();
	s_pFX->ClearAll();
}

void CGameProcMain::ControlViewVDegree(short sValue)
{
	if(s_pEng->ViewPoint() == VP_THIRD_PERSON)
	{
		float fPitch = 0;
		if(sValue > 0)	fPitch = D3DXToRadian(45.0f)*3;
		else			fPitch = D3DXToRadian(-45.0f)*3;
		if(fPitch) s_pEng->CameraPitchAdd(fPitch);
	}
}
/*
bool CGameProcMain::OnMouseMove(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	if(ptCur.x != ptPrev.x || ptCur.y != ptPrev.y)
	{
		if(m_pMagicSkillMng->m_dwRegionMagicState==1)
		{
			__Vector3 vPick;
			CPlayerNPC*	 pTarget = NULL;
		
			int iID = -1;
			pTarget = s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &vPick); // 사방에 깔린넘들 픽킹..
			this->TargetSelect(iID, false); // 타겟을 잡는다..
			if(NULL == pTarget) // 타겟이 없으면..
			{
				s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &vPick); // 찍힌 위치를 저장한다..
				if(NULL == s_pPlayer->m_pObjectTarget) // 타겟도 없으면..
				{
					ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vPick); // 지형을 찍어본다..
				}
			}
			s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, vPick);
		}
	}

	return true;
}
*/
bool CGameProcMain::OnMouseMove(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	// NOTE: check if cursor position has changed
	if(ptCur.x != ptPrev.x || ptCur.y != ptPrev.y)
	{
		// NOTE: check if something has been selected by the cursor
		if(m_pMagicSkillMng->m_dwRegionMagicState==1)
		{
			__Vector3 vNormal, vMyPos, vGap, vDir;

			vMyPos = s_pPlayer->Position();
			vMyPos.y += s_pPlayer->Height() / 2;

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseSkillPos); // 지형을 찍어본다..

			vDir = m_vMouseSkillPos - vMyPos;
			vGap = vDir;
			vDir.Normalize();

			bool bColShape = ACT_WORLD->CheckCollisionWithShape(vMyPos, vDir, vGap.Magnitude(), &m_vMouseSkillPos, &vNormal);

			if(!bColShape) // 타겟도 없으면..
			{
				int iID = -1;
				s_pOPMgr->Pick(ptCur.x, ptCur.y, iID, &m_vMouseSkillPos); // 사방에 깔린넘들 픽킹..
				this->TargetSelect(iID, false); // 타겟을 잡는다..
			}
			else
			{
				m_vMouseSkillPos.y = ACT_WORLD->GetHeightWithTerrain(m_vMouseSkillPos.x, m_vMouseSkillPos.z);
			}
			s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseSkillPos);
		}
	}

	return true;
}

// 왼쪽 더블 클릭
bool CGameProcMain::OnMouseLDBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	CPlayerNPC* pTarget = s_pOPMgr->CharacterGetByID(s_pPlayer->m_iIDTarget, true);

	if(pTarget && pTarget->m_InfoBase.iAuthority == AUTHORITY_MANAGER)
	{
		s_pPlayer->m_iIDTarget = -1;
		pTarget = NULL;
	}

	if(VP_THIRD_PERSON == s_pEng->ViewPoint())
	{
		if(s_pPlayer->IsAttackableTarget(pTarget, false))
		{
			this->CommandMove(MD_STOP, true);
			this->CommandEnableAttackContinous(true, pTarget); // 자동 공격
		}
		else if(pTarget && VP_THIRD_PERSON == s_pEng->ViewPoint())
		{
			this->CommandMove(MD_FOWARD, true);
			s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
		}
	}
	else
	{
		s_pPlayer->m_bAttackContinous = false;
		CommandToggleAttackContinous();
	}
	return true;
}

// 왼쪽 클릭
bool CGameProcMain::OnMouseLBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	POINT ptPlayer = ::_Convert3D_To_2DCoordinate(	s_pPlayer->Position(), 
													CN3Base::s_CameraData.mtxView,
													CN3Base::s_CameraData.mtxProjection,
													CN3Base::s_CameraData.vp.Width,
													CN3Base::s_CameraData.vp.Height);
	
	__Vector3 vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
	__Matrix44 mtxTmp; mtxTmp.RotationY(s_pEng->CameraYaw());
	vDir *= mtxTmp;
	vDir.Normalize();
	float fYaw = ::_Yaw2D(vDir.x, vDir.z);

	CPlayerNPC*	 pTarget			= NULL;
	BOOL		 bFindCorpse		= false;

	int iID = -1;
	pTarget = s_pOPMgr->PickPrecisely(ptCur.x, ptCur.y, iID, &m_vMouseLBClickedPos); // 사방에 깔린넘들 픽킹..
	this->TargetSelect(iID, false); // 타겟을 잡는다..
	if(NULL == pTarget) // 타겟이 없으면..
	{
		if(s_pPlayer->m_bAttackContinous) // 계속 공격하는 중이면..
			this->CommandEnableAttackContinous(false, NULL); // 계속 공격 취소..

		s_pPlayer->m_pObjectTarget = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true, &m_vMouseLBClickedPos); // 찍힌 위치를 저장한다..
		if(NULL == s_pPlayer->m_pObjectTarget) // 타겟도 없으면..
		{
			// 시체 뒤저서 아이템 상자 열기..
			CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // 픽킹..
			if(false == this->MsgSend_RequestItemBundleOpen(pCorpse)) // 시체 뒤지기 요청..
			{
				ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, m_vMouseLBClickedPos); // 지형을 찍어본다..
			}
			else
				bFindCorpse = true;
		}
	}

	if(m_pMagicSkillMng->m_dwRegionMagicState==1)
	{
//		s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseLBClickedPos);
		s_pFX->SetBundlePos(m_pMagicSkillMng->m_iMyRegionTargetFXID, m_pMagicSkillMng->m_iMyRegionTargetFXID, m_vMouseSkillPos);
		m_pMagicSkillMng->m_dwRegionMagicState = 2;
	}

	if(!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint())
	{
		m_fLBClickTime = CN3Base::TimeGet();
		this->CommandSitDown(false, false); // 일단 일으켜 세운다..

		if(pTarget)
		{
			if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_LCTRL) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RCTRL))
			{
				if(s_pPlayer->IsAttackableTarget(pTarget, false))
				{
					if(s_pPlayer->m_bTargetOrPosMove)
						this->CommandMove(MD_STOP, true);

					this->CommandEnableAttackContinous(true, pTarget); // 자동 공격
				}
				else
				{
					this->CommandMove(MD_FOWARD, true);
					s_pPlayer->SetMoveTargetID(s_pPlayer->m_iIDTarget);
				}
			}
			else
			{
				if(s_pPlayer->m_bTargetOrPosMove && s_pPlayer->m_iMoveTarget != pTarget->IDNumber())
					this->CommandMove(MD_STOP, true);
			}
		}
		else if(!bFindCorpse)
		{
			s_pPlayer->RotateTo(fYaw, true);
			__Vector3 vMovePoint;
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // 지형을 찍어본다..

			float fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			if(!s_pPlayer->m_bTargetOrPosMove && fDist > 1.5f)
				this->CommandMove(MD_FOWARD, true);

			if(fDist > 1.5f)
			{
				s_pPlayer->SetMoveTargetPos(vMovePoint);
			}
			else if(s_pPlayer->m_bTargetOrPosMove)
			{
				this->CommandMove(MD_STOP, true);
			}
		}
	}
	else if(!s_pPlayer->IsDead())
	{
		if(pTarget && (s_pLocalInput->IsKeyDown(SDL_SCANCODE_LCTRL) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RCTRL)))
		{
			if(s_pPlayer->IsAttackableTarget(pTarget, false))
			{
				if(s_pPlayer->m_bTargetOrPosMove)
					this->CommandMove(MD_STOP, true);

				this->CommandEnableAttackContinous(true, pTarget); // 자동 공격
			}
			else
			{
				s_pPlayer->m_bAttackContinous = false;
				CommandToggleAttackContinous();
			}
		}
	}

	return true;
}

// 왼쪽 클릭업
bool CGameProcMain::OnMouseLBtnPressd(POINT ptCur, POINT ptPrev)
{
	if(!s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD))
	{
//		if(!s_pPlayer->m_bTargetOrPosMove)
//			this->CommandMove(MD_STOP, true);
	}
	return true;
}

// 왼쪽 눌리고 있을때
bool CGameProcMain::OnMouseLbtnDown(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	POINT ptPlayer = ::_Convert3D_To_2DCoordinate(	s_pPlayer->Position(), 
													CN3Base::s_CameraData.mtxView,
													CN3Base::s_CameraData.mtxProjection,
													CN3Base::s_CameraData.vp.Width,
													CN3Base::s_CameraData.vp.Height);
	
	__Vector3 vDir((float)(ptCur.x - ptPlayer.x), 0, (float)(ptPlayer.y - ptCur.y));
	__Matrix44 mtxTmp; mtxTmp.RotationY(s_pEng->CameraYaw());
	vDir *= mtxTmp;
	vDir.Normalize();
	float fYaw = ::_Yaw2D(vDir.x, vDir.z);

	if(!s_pPlayer->IsDead() && VP_THIRD_PERSON == s_pEng->ViewPoint() && !s_pLocalInput->IsKeyDown(KM_MOVE_FOWARD)) // 삼인칭 시점이면.. UI 를 건들지 않았으면..
	{
		__Vector3 vMovePoint;
		float fDist;
		float fNowTime = CN3Base::TimeGet();

		if( fNowTime - m_fLBClickTime > 0.1f && s_pPlayer->m_bTargetOrPosMove)
		{
			if(s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();


			this->CommandSitDown(false, false); // 일단 일으켜 세운다..
			s_pPlayer->RotateTo(fYaw, true);
			this->CommandMove(MD_FOWARD, false);

			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // 지형을 찍어본다..
			fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			s_pPlayer->SetMoveTargetPos(vMovePoint);
		}
		else if( fNowTime - m_fLBClickTime > 0.1f && !s_pPlayer->m_bTargetOrPosMove)
		{
			if(s_pPlayer->m_bAttackContinous)
				CommandToggleAttackContinous();

			s_pPlayer->RotateTo(fYaw, true);
			ACT_WORLD->PickWideWithTerrain(ptCur.x, ptCur.y, vMovePoint); // 지형을 찍어본다..

			fDist = (vMovePoint - s_pPlayer->Position()).Magnitude();

			if(fDist > 1.5f)
			{
				this->CommandMove(MD_FOWARD, true);
				s_pPlayer->SetMoveTargetPos(vMovePoint);
			}
		}
	}
	return true;
}

// 오른쪽 클릭
bool CGameProcMain::OnMouseRBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	if(s_pLocalInput->IsKeyDown(SDL_SCANCODE_LCTRL) || s_pLocalInput->IsKeyDown(SDL_SCANCODE_RCTRL))
	{
		m_pUIHotKeyDlg->EffectTriggerByMouse();
		return true;
	}

	int iID = -1;
	CPlayerNPC* pNPC = s_pOPMgr->PickNPC(ptCur.x, ptCur.y, iID); // 픽킹..

	if(NULL == pNPC)
	{
		// NOTE: sending the packet to pick up items
		CPlayerNPC* pCorpse = s_pOPMgr->PickCorpse(ptCur.x, ptCur.y, iID); // 픽킹..
		if(false == this->MsgSend_RequestItemBundleOpen(pCorpse)) // 시체 뒤저서 아이템 상자 열기..
		{
			// NOTE: if no corpse and item box then check if the player clicked on an iteractable shape
			CN3Shape* pShape = ACT_WORLD->PickWithShape(ptCur.x, ptCur.y, true);
			if(	pShape && pShape == s_pPlayer->m_pObjectTarget && pShape->m_iEventID) // Event 가 있으면..
			{
				float fD = (s_pPlayer->Position() - pShape->Pos()).Magnitude(); 
				float fDLimit = (s_pPlayer->Radius() + pShape->Radius()) * 2.0f;
				if(fD > fDLimit) // 거리가 멀면
				{
					std::string szMsg; 
					if (OBJECT_TYPE_BINDPOINT == pShape->m_iEventType)
						szMsg = "IDS_BIND_POINT_REQUEST_FAIL";//::_LoadStringFromResource(IDS_BIND_POINT_REQUEST_FAIL, szMsg);
					else
						szMsg = "IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR";//::_LoadStringFromResource(IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					if(OBJECT_TYPE_BINDPOINT == pShape->m_iEventType)
					{
						std::string szMsg = "IDS_REQUEST_BINDPOINT"; //::_LoadStringFromResource(IDS_REQUEST_BINDPOINT, szMsg);
						this->MessageBoxPost(szMsg, "", MB_YESNO, BEHAVIOR_REQUEST_BINDPOINT); // 바인팅 포인트 설정 메시지 박스
					}
					else if(OBJECT_TYPE_WARP_POINT == pShape->m_iEventType)
					{
						this->MsgSend_ObjectEvent(pShape->m_iEventID, pShape->m_iNPC_ID); // 오브젝트 이벤트 보내기..
					}
				}
			}
		}
	}
	else if(pNPC->IDNumber() == s_pPlayer->m_iIDTarget) // 타겟으로 찍은 캐릭터와 피킹한 캐릭터가 같고 ..) 
	{
		if(	pNPC->m_pShapeExtraRef ) // 오브젝트 형태의 NPC 이면.. 컨트롤 할 NPC의 ID 가 있으면..
		{
			if(pNPC->m_pShapeExtraRef->m_iNPC_ID > 0)
			{
				float fD = (s_pPlayer->Position() - pNPC->m_pShapeExtraRef->Pos()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->m_pShapeExtraRef->Radius()) * 2.0f;
				if(fD > fDLimit) // 거리가 멀면
				{
					std::string szMsg;
					szMsg = "IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR";//::_LoadStringFromResource(IDS_ERR_REQUEST_OBJECT_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					this->MsgSend_ObjectEvent(pNPC->m_pShapeExtraRef->m_iEventID, pNPC->IDNumber());
				}
			}
		}
		else // 보통 NPC 이면..
		{
			// NOTE: an NPC has been clicked on
			// TODO(srmeier): need to use ZoneAbilityType here
			// NOTE(srmeier): using the zone type to decide if you can talk with NPC
			if(ACT_WORLD->GetZoneType()==ZoneAbilityNeutral || (pNPC->m_InfoBase.eNation == s_pPlayer->m_InfoBase.eNation)) // 같은 국가 일때만..
			{
				float fD = (s_pPlayer->Position() - pNPC->Position()).Magnitude();
				float fDLimit = (s_pPlayer->Radius() + pNPC->Radius()) * 3.0f;
				if(fD > fDLimit) // 거리가 멀면
				{
					std::string szMsg;
					szMsg = "IDS_ERR_REQUEST_NPC_EVENT_SO_FAR"; //::_LoadStringFromResource(IDS_ERR_REQUEST_NPC_EVENT_SO_FAR, szMsg);
					this->MsgOutput(szMsg, 0xffff8080);
				}
				else
				{
					s_pPlayer->ActionMove(PSM_STOP); // 일단 멈추고..
					pNPC->RotateTo(s_pPlayer); // 방향을 플레이어 쪽으로 돌린다.
					this->MsgSend_NPCEvent(iID); // 이벤트 요청..
					if (m_pUITransactionDlg) m_pUITransactionDlg->m_iNpcID = pNPC->IDNumber();
				}
			}
		}
	}
	return true;
}

// 오른쪽 클릭업
bool CGameProcMain::OnMouseRBtnPressd(POINT ptCur, POINT ptPrev)
{
	return true;
}

// 오른쪽 눌리고 있을때
bool CGameProcMain::OnMouseRbtnDown(POINT ptCur, POINT ptPrev)
{
	float fMouseSensivity = 0.05f;//0.02f;
	
	float fRotY = D3DXToRadian(180.0f) * ((ptCur.x - ptPrev.x) * fMouseSensivity); // 회전할 양을 계산하고..
	float fRotX = D3DXToRadian(180.0f) * ((ptCur.y - ptPrev.y) * fMouseSensivity);
	if(fRotY && s_pPlayer->IsAlive())
	{
		if(VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraYawAdd(fRotY);
		else if(false == s_pPlayer->m_bStun) s_pPlayer->RotAdd(fRotY); // 기절해 있지 않을때만..
	}
	if(fRotX)
	{
		if(VP_THIRD_PERSON == s_pEng->ViewPoint()) s_pEng->CameraZoom(-fRotX); // 카메라 확대
		else s_pEng->CameraPitchAdd(fRotX); // 카메라 각도
	}

	if(fRotY || fRotX)
	{
		//SetGameCursor(NULL);
		//::SetCursorPos(ptPrev.x, ptPrev.y);
		//s_pLocalInput->MouseSetPos(ptPrev.x, ptPrev.y);
	}
	return true;
}

// 오른쪽 더블 클릭
bool CGameProcMain::OnMouseRDBtnPress(POINT ptCur, POINT ptPrev)
{
	if(s_pUIMgr->m_bDoneSomething) return false;

	//스킬 매직이 사용되었다면....
	m_pUIHotKeyDlg->EffectTriggerByMouse();

	return true;
}

void CGameProcMain::ProcessUIKeyInput(bool bEnable)
{
	if(m_pUIChatDlg && !m_pUIChatDlg->IsChatMode())
	{
		CGameProcedure::ProcessUIKeyInput();
		if(s_pLocalInput->IsKeyPress(SDL_SCANCODE_RETURN) && !s_bKeyPress)
		{
			m_pUIChatDlg->SetFocus();
		}
	}
	else if(m_pUIChatDlg && m_pUIChatDlg->IsChatMode())
	{
		s_bKeyPress = false;
		if(m_pUIChatDlg->GetEnableKillFocus())
		{
			m_pUIChatDlg->SetEnableKillFocus(false);
			m_pUIChatDlg->KillFocus();
		}
	}
}

void CGameProcMain::MsgSend_SpeedCheck(bool bInit)
{
	BYTE	byBuff[10];											// 버퍼.. 
	int		iOffset=0;											// 옵셋..
	float	fTime = CN3Base::TimeGet();							// 클라이언트 시간

	s_pSocket->MP_AddByte(byBuff, iOffset, N3_CHECK_SPEEDHACK);	// 스피드핵 체크 패킷..
	s_pSocket->MP_AddByte(byBuff, iOffset, bInit);				// 서버가 기준 시간으로 쓸 타입 true 이면 기준시간 false면 체크타입
	s_pSocket->MP_AddFloat(byBuff, iOffset, fTime);				// 클라이언트 시간
	s_pSocket->Send(byBuff, iOffset);							// 보냄..
}
