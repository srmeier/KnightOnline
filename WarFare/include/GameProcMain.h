// GameProcMain.h: interface for the CGameProcMain class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCMAIN_H__E1C4F2CC_5AF3_4417_8917_A52CD5523DB3__INCLUDED_)
#define AFX_GAMEPROCMAIN_H__E1C4F2CC_5AF3_4417_8917_A52CD5523DB3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameProcedure.h"
#include <set>

typedef std::set<int>::iterator it_ID;
typedef std::pair<it_ID, bool> pair_ID;

class CGameProcMain : public CGameProcedure  
{
	friend class CGameProcedure;
protected:
	std::set<int> m_SetNPCID;
	std::set<int> m_SetUPCID;

public:
#ifdef _N3_64GRID_
	CServerMesh		*m_pSMesh;									// 서버에게 필요한 메쉬 클래스..
#endif
	class CUIInventory*			m_pUIInventory;			// 인벤토리
	class CUIVarious*			m_pUIVar;				// 캐릭터 상태창, 기사단 관리 등이 페이지로 들어간 다용도 UI
	class CUIChat*				m_pUIChatDlg;			// 채팅 입출력 대화상자..
	class CUIMessageWnd*		m_pUIMsgDlg;			// 게임 메시지 출력 상자.
	class CUIStateBar*			m_pUIStateBarAndMiniMap;	// mp,hp,exp, minimap....
	class CUICmd*				m_pUICmd;				// 왼쪽 하단의 명령버튼 창..
	class CUITargetBar*			m_pUITargetBar;			// 타겟 상태창..
	class CUITransactionDlg*	m_pUITransactionDlg;
	class CUIDroppedItemDlg*	m_pUIDroppedItemDlg;
	class CSubProcPerTrade*		m_pSubProcPerTrade;
	class CUIHelp*				m_pUIHelp;
	class CUIPartyOrForce*		m_pUIPartyOrForce;
	class CUISkillTreeDlg*		m_pUISkillTreeDlg;
	class CUIHotKeyDlg*			m_pUIHotKeyDlg;
	class CUINotice*			m_pUINotice;
	class CUIClassChange*		m_pUIClassChange;
	class CUINPCEvent*			m_pUINpcEvent;
	class CUIItemExchange*		m_pUIItemREDlg;
	class CUIRepairTooltipDlg*	m_pUIRepairTooltip;
	class CUIWareHouseDlg*		m_pUIWareHouseDlg;
	class CUIInn*				m_pUIInn;
	class CUICreateClanName*	m_pUICreateClanName;
	


	class CUIKnightsOperation*	m_pUIKnightsOp;					// 기사단 리스트 보기, 가입, 등...
	class CUIPartyBBS*			m_pUIPartyBBS;					// 파티 지원 게시판.
	class CUITradeSellBBS*		m_pUITradeBBS;					// 상거래 게시판
	class CUIQuestMenu*			m_pUIQuestMenu;					// Quest Menu
	class CUIQuestTalk*			m_pUIQuestTalk;					// Quest Talk
	class CUIDead*				m_pUIDead;						// Dead UI
	class CUITradeBBSSelector*	m_pUITradeBBSSelector;			// 상거래 게시판 종류 선택
	class CUITradeBBSEditDlg*	m_pUITradeBBSEdit;				// 상거래 게시물 설명

	class CN3Shape*				m_pTargetSymbol;				// 플레이어가 타겟으로 잡은 캐릭터의 위치위에 그리면 된다..

	class CN3SndObjStream*		m_pSnd_Town, *m_pSnd_Battle;	//마을음악, 전투음악 포인터..
	class CMagicSkillMng*		m_pMagicSkillMng;

	class CUINpcTalk*			m_pUINpcTalk;	
	class CUINPCChangeEvent*	m_pUINpcChange;
	class CUIWarp*				m_pUIWarp;
	class CWarMessage*			m_pWarMessage;					// 전쟁관련 메시지
	class CLightMgr*			m_pLightMgr;

	   
	//..
	BOOL		m_bLoadComplete;							// 로딩이 완료되었나??
	
	float		m_fMsgSendTimeMove;							// 최근 이동 메시지 보낸 시간..
	float		m_fMsgSendTimeRot;							// 최근 회전 메시지 보낸 시간..
	float		m_fPlayerYawSended; // 최근에 메시지를 보낸 시점의 플레이어 y 축 회전값.
	float		m_fRequestGameSave;	// 최근에 게임 데이터 저장을 요청한 시간
	__Vector3	m_vPlayerPosSended; // 최근에 메시지를 보낸 시점의 플레이어 위치.

	__Vector3	m_vMouseLBClickedPos;
	__Vector3	m_vMouseSkillPos;

	float		m_fLBClickTime;

	int			m_iJoinReqClan;
	int			m_iJoinReqClanRequierID;

	int			KM_COUNT;

protected:
	bool	ProcessPacket(DataPack* pDataPack, int& iOffset);

	bool	MsgRecv_CharacterSelect(DataPack* pDataPack, int& iOffset); // virtual
	int		MsgRecv_VersionCheck(DataPack* pDataPack, int& iOffset); // virtual

	bool	MsgRecv_MyInfo_All(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_MyInfo_HP(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_MyInfo_MSP(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_MyInfo_EXP(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_MyInfo_LevelChange(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_MyInfo_RealmPoint(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_MyInfo_PointChange(DataPack* pDataPack, int& iOffset);

	bool	MsgRecv_Chat(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_UserMove(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_Rotation(DataPack* pDataPack, int& iOffset);
//	bool	MsgRecv_Dead(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_Regen(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Corpse(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_Time(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_Weather(DataPack* pDataPack, int& iOffset);
	
	bool	MsgRecv_UserInAndRequest(DataPack* pDataPack, int& iOffset);		// 주위 영역의 모든 User 아이디를 카운트만큼 받는다... 글구.. 업데이트가 필요한 것만 서버에게 요청..
	bool	MsgRecv_UserInRequested(DataPack* pDataPack, int& iOffset);					// 서버에게 요청한 유저의 아이디들을 받아서 User 처리..
	bool	MsgRecv_UserInOut(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_UserIn(DataPack* pDataPack, int& iOffset, bool bWithFX=false);
	bool	MsgRecv_UserOut(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_UserState(DataPack* pDataPack, int& iOffset);

	bool	MsgRecv_NPCInAndRequest(DataPack* pDataPack, int& iOffset);		// 주위 영역의 모든 NPC 아이디를 카운트만큼 받는다... 글구.. 업데이트가 필요한 것만 서버에게 요청..
	bool	MsgRecv_NPCInRequested(DataPack* pDataPack, int& iOffset);				// 서버에게 요청한 NPC 아이디들을 받아서 User 처리..
	bool	MsgRecv_NPCInOut(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_NPCIn(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_NPCOut(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_NPCMove(DataPack* pDataPack, int& iOffset);

	bool	MsgRecv_Attack(DataPack* pDataPack, int& iOffset);
	bool	MsgRecv_Dead(DataPack* pDataPack, int& iOffset);

	bool	MsgRecv_ItemMove(DataPack* pDataPack, int& iOffset);					// Item Move에 대한 응답..
	bool	MsgRecv_ItemBundleDrop(DataPack* pDataPack, int& iOffset);			// Item 이 필드에 나타나는데에 대한 응답
	bool	MsgRecv_ItemBundleOpen(DataPack* pDataPack, int& iOffset);			// 아이템 상자를 열거나 시체를 뒤진다..
	bool	MsgRecv_ItemTradeStart(DataPack* pDataPack, int& iOffset);			// 아이템 상거래..
	bool	MsgRecv_ItemTradeResult(DataPack* pDataPack, int& iOffset);			// 아이템 상거래 결과..
	bool	MsgRecv_ItemDroppedGetResult(DataPack* pDataPack, int& iOffset);	// 땅에 떨어진 아이템 먹기 결과..
	bool	MsgRecv_ItemWeightChange(DataPack* pDataPack, int& iOffset);		// 아이템 무게 변화..

	bool	MsgRecv_UserLookChange(DataPack* pDataPack, int& iOffset);			// 겉모습이 바뀐다.. 아이템 착용등

	void	MsgRecv_TargetHP(DataPack* pDataPack, int& iOffset);

	void	MsgRecv_ZoneChange(DataPack* pDataPack, int& iOffset);			// 걍 단순한 존 체인지.

	void	MsgRecv_Notice(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_PartyOrForce(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_PerTrade(DataPack* pDataPack, int& iOffset);

	void	MsgRecv_SkillChange(DataPack* pDataPack, int& iOffset);			// 스킬 변화..
	void	MsgRecv_MagicProcess(DataPack* pDataPack, int& iOffset);			// 스킬 변화..
	void	MsgRecv_ClassChange(DataPack* pDataPack, int& iOffset);			// 직업 변화..
	void	MsgRecv_ObjectEvent(DataPack* pDataPack, int& iOffset);			// 오브젝트 이벤트 신청에 대한 응답

	void	MsgRecv_DurabilityChange(DataPack* pDataPack, int& iOffset);		// 내구력 변경..
	void	MsgRecv_NpcEvent(DataPack* pDataPack, int& iOffset);				// Npc Event(Exchange, Repair both).. 

	void	MsgRecv_Knights(DataPack* pDataPack, int& iOffset);				// 기사단 조작 관련 패킷..
	void	MsgRecv_KnightsListBasic(DataPack* pDataPack, int& iOffset);		// 기사단 기본 정보들..
	
	void	MsgRecv_CompressedPacket(DataPack* pDataPack, int& iOffset);		// 압축된 패킷이다... 압축 풀고 루프를 돌면서 한번더 파싱해야 한다!!!
	void	MsgRecv_ContinousPacket(DataPack* pDataPack, int& iOffset);		// 붙어서 오는 패킷이다.. 루프를 돌면서 한번더 파싱해야 한다!!!

	void	MsgRecv_ItemRepair(DataPack* pDataPack, int& iOffset);			// Item Repair Result.. 
	void	MsgRecv_ItemCountChange(DataPack* pDataPack, int& iOffset);		// Item Count Change..
	void	MsgRecv_ItemDestroy(DataPack* pDataPack, int& iOffset);			// Item Count Change..

	void	MsgRecv_WareHouse(DataPack* pDataPack, int& iOffset);			// 보관함 관련 패킷..
	void	MsgRecv_WareHouseOpen(DataPack* pDataPack, int& iOffset);		// 보관함 오픈..

	void	MsgRecv_NpcChangeOpen(DataPack* pDataPack, int& iOffset);		// Class Change와 초기화..
	void	MsgRecv_AllPointInit(DataPack* pDataPack, int& iOffset);			// All Point 초기화..
	void	MsgRecv_SkillPointInit(DataPack* pDataPack, int& iOffset);		// Skill Point 초기화..
	void	MsgRecv_PointChangePriceQueryRequest(DataPack* pDataPack, int& iOffset);		// 가격에 대한 응답 패킷..

	void	MsgRecv_NoahChange(DataPack* pDataPack, int& iOffset);			// 노아 변경..
	void	MsgRecv_WarpList(DataPack* pDataPack, int& iOffset);				// 워프 리스트..
//	void	MsgRecv_ServerCheckAndRequestConcurrentUserCount(DataPack* pDataPack, int& iOffset);			// 서버 IP 와 포트를 받아 동접자를 체크해 본다..
//	void	MsgRecv_ConcurrentUserCountAndSendServerCheck(DataPack* pDataPack, int& iOffset);
	
	//knights...
	void	MsgRecv_Knights_Create(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knights_Withdraw(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knights_Join(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knights_Leave(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knights_AppointViceChief(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knights_MemberInfoAll(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knights_GradeChangeAll(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knights_Duty_Change(DataPack* pDataPack, int& iOffset);
	void	MsgRecv_Knigts_Join_Req(DataPack* pDataPack, int& iOffset);

public:
	void	ProcessUIKeyInput(bool bEnable = true);
	bool	OnMouseMove(POINT ptCur, POINT ptPrev);
	bool	OnMouseLbtnDown(POINT ptCur, POINT ptPrev);
	bool	OnMouseLBtnPressd(POINT ptCur, POINT ptPrev);
	bool	OnMouseLBtnPress(POINT ptCur, POINT ptPrev);
	bool	OnMouseLDBtnPress(POINT ptCur, POINT ptPrev);
	bool	OnMouseRbtnDown(POINT ptCur, POINT ptPrev);
	bool	OnMouseRBtnPressd(POINT ptCur, POINT ptPrev);
	bool	OnMouseRBtnPress(POINT ptCur, POINT ptPrev);
	bool	OnMouseRDBtnPress(POINT ptCur, POINT ptPrev);
	void	ControlViewVDegree(short sValue);
	void	PlayBGM_Town();
	void	PlayBGM_Battle();

	void	DoCommercialTransaction(int iTradeID);								// 상거래..

	const __InfoPartyOrForce*	PartyOrForceConditionGet(bool& bIAmLeader, bool& bIAmMember, int& iMemberIndex, class CPlayerBase*& pTarget);
	void						TargetSelect(int iID, bool bMustAlive);
	void						TargetSelect(class CPlayerNPC* pTarget);

	bool	CommandToggleUIInventory();
	bool	CommandToggleUIState();
	bool	CommandToggleAttackContinous();
	bool	CommandToggleMoveContinous();
	bool	CommandToggleWalkRun();
	bool	CommandToggleUISkillTree();
	bool	CommandToggleUIMiniMap();

	void	CommandMove(e_MoveDirection eMD, bool bStartOrEnd); // 움직이는 방향(전후진, 멈춤), 움직이기 시작하는가?
	void	CommandEnableAttackContinous(bool bEnable, CPlayerBase* pTarget);
	void	CommandCameraChange(); // 카메라 시점 바꾸기..
	void	CommandSitDown(bool bLimitInterval, bool bSitDown, bool bImmediately = false);

	void	CommandTargetSelect_NearstEnemy(); // 가장 가까운 적 타겟 잡기..
	void	CommandTargetSelect_NearstOurForce(); // 가장 가까운 파티 타겟잡기..

	void	CloseUIs(); // 각종 상거래, 워프등등... UI 닫기..

	void	MsgOutput(const std::string& szMsg, D3DCOLOR crMsg);

	void	InitZone(int iZone, const __Vector3& vPosPlayer);
	void	InitUI();
	void	InitPlayerPosition(const __Vector3& vPos); // 플레이어 위치 초기화.. 일으켜 세우고, 기본동작을 취하게 한다.

	void	MsgSend_Continous();												// 특정 조건(?)하에서 서버에게 정기적으로 메시지를 보냄..
	void	MsgSend_Attack(int iTargetID, float fInterval, float fDistance);	// 공격 패킷 날리기 - 테이블의 공격 주기를 같이 줘서 해킹을 막는다.
	void	MsgSend_Move(bool bMove, bool bContinous);							// 서버에게 움직임 패킷을 날린다.. // 움직이는가 ? 주기적으로 움직이는 건가?
	void	MsgSend_Rotation();													// 서버에게 회전 패킷을 날린다..
	void	MsgSend_Chat(enum e_ChatMode eMode, const std::string& szChat);		// 서버에게 채팅 메시지를 날린다..
	void	MsgSend_ChatSelectTarget(const std::string& szTargetID); // 일대일 채팅 상대 정하기.
	void	MsgSend_Regen();
	bool	MsgSend_RequestItemBundleOpen(CPlayerNPC* pCorpse); // 아이템 상자를 열거나 시체를 뒤진다..
	void	MsgSend_RequestTargetHP(short siIDTarget, BYTE byUpdateImmediately); // 0x00 - 점차 늘어나게끔.. 0x01 - 즉시 업데이트..
	void	MsgSend_GameStart();
	bool	MsgSend_NPCEvent(short siIDTarget);
	void	MsgSend_NPCInRequest(int iID); // NPC 정보가 없을 경우 요청한다..
	void	MsgSend_UserInRequest(int iID); // User 정보가 없을 경우 요청한다..
	void	MsgSend_Warp(); // 워프?? - 존체인지가 될수도 있다..
	void	MsgSend_ZoneChangeComplete(); // 존 체인지 완료.. (맵 로딩 끝..)
	void	MsgSend_StateChange(enum e_SubPacket_State eSP, int iState);
	void	MsgSend_PerTradeReq(int iDestID, bool bNear = true);
	void	MsgSend_SpeedCheck(bool bInit = false);

	void	MsgSend_PartyOrForcePermit(int iPartyOrForce, bool bYesNo); // iPartyOrForce 1 : Party, 2:Force
	void	MsgSend_PartyOrForceLeave(int iPartyOrForce); // iPartyOrForce 1 : Party, 2:Force
	bool	MsgSend_PartyOrForceCreate(int iPartyOrForce, const std::string& szID); // iPartyOrForce 1 : Party, 2:Force

	void	MsgSend_ObjectEvent(int iEventID, int iNPCID);		// 오브젝트에 설정되어 있는 이벤트 요청..
	void	MsgSend_Weather(int iWeather, int iPercent);
	void	MsgSend_Time(int iHour, int iMin);
	void	MsgSend_Administrator(enum e_SubPacket_Administrator eSP, const std::string& szID);
	
	void	MsgSend_KnightsJoin(int iTargetID);
	void	MsgSend_KnightsLeave(std::string& szName);
	void	MsgSend_KnightsWithdraw();
	void	MsgSend_KnightsAppointViceChief(std::string& szName);
	void	MsgSend_KnightsJoinReq(bool bJoin);	
	void	MsgSend_PerTradeBBSReq(std::string szName, int iDestID);
	void	MsgSend_CharacterSelect(); // virtual


	void	ProcessPlayerInclination();				// 경사 처리..(가만히 있어도 경사가 급하면 미끄러짐..).
	void	ProcessLocalInput(DWORD dwMouseFlags);	// 키보드 눌린것을 처리한다..
	void	ParseChattingCommand(const std::string& szCmd);
	

	void	UpdateUI_PartyOrForceButtons(); // 커맨드 줄에 있는 파티 버튼을 상황에 따라 업데이트 해준다.
	void	UpdateUI_MiniMap();
	void	UpdateUI_TargetBar();
	void	UpdateBGM();
	void	UpdateCameraAndLight();

	void	RenderTarget();
	
	void	Init();									// UI 와 UI 리소스등을 읽는다.
	void	Release();								// Release..	
	void	ReleaseUIs();
	void	ReleaseSound();
	
	void	Tick();								// 잡다한 계산..
	void	Render();							// 렌더링..

	CGameProcMain();									// 생성자.
	virtual ~CGameProcMain();							// 소멸자.
};

#endif // !defined(AFX_GAMEPROCMAIN_H__E1C4F2CC_5AF3_4417_8917_A52CD5523DB3__INCLUDED_)
