#ifndef __PACKET_DEF_H_
#define __PACKET_DEF_H_

//const int SOCKET_PORT_GAME = 15001;//10000;//15001; // 소켓이 붙을 포트 번호..
//const int SOCKET_PORT_LOGIN = 15100; // 소켓이 붙을 포트 번호..

// NOTE: with this custom Ebenezer
const int SOCKET_PORT_GAME = 10000;//10000;//15001; // 소켓이 붙을 포트 번호..
const int SOCKET_PORT_LOGIN = 10000; // 소켓이 붙을 포트 번호..

#define N3_ACCOUNT_LOGIN			0xf3
#define N3_ACCOUNT_LOGIN_MGAME		0xf4	// !!! LOGIN_REQ !!! 와 동일
#define N3_GAMESERVER_GROUP_LIST	0xf5	// Recv - b1(ServerCount) loop(	s1(IP길이), str1(IP문자열), s1(서버이름길이), str1(서버이름문자열), s1(서버접속인원) )

#define	N3_GAME_SERVER_LOGIN	0x01	// Game server 에 Log in..
#define	N3_NEW_CHARACTER		0x02	// 캐릭터 새로 만들기..
#define	N3_DELETE_CHARACTER		0x03	// 만들어 놓은 캐릭터 지우기..
#define	N3_CHARACTER_SELECT		0x04	// Game start..
#define	N3_NATION_SELECT		0x05	// 국가 선택 - 0 이면 실패..
#define	N3_MOVE					0x06	// Move..
#define	N3_ROTATE				0x09	// Rotate..
#define	N3_USER_INOUT			0x07	// User inout..
#define	N3_NPC_INOUT			0x0A	// NPC inout..
#define	N3_ATTACK				0x08	// Attack..
#define	N3_NPC_MOVE				0x0B	// NPC move..
#define	N3_ALL_CHARACTER_INFO_REQUEST	0x0C	// 모든 캐릭터 정보 요청하기..
#define	N3_GAMESTART			0x0D	// Game Start..
#define	N3_MYINFO				0x0E	// NPC moveedge..
#define	N3_CHAT					0x10	// Chat..
	enum e_ChatMode {	N3_CHAT_NORMAL = 1,
						N3_CHAT_PRIVATE,
						N3_CHAT_PARTY,
						N3_CHAT_FORCE,
						N3_CHAT_SHOUT,
						N3_CHAT_CLAN,
						N3_CHAT_PUBLIC,
						N3_CHAT_WAR,
						N3_CHAT_TITLE,						//맨윗줄에 갈 공지
						N3_CHAT_TITLE_DELETE,				//맨윗줄에 있는 공지 삭제
						N3_CHAT_CONTINUE,					//지속 공지
						N3_CHAT_CONTINUE_DELETE,			//지속 공지 삭제
						N3_CHAT_UNKNOWN = 0xffffffff };

#define	N3_DEAD					0x11	// Player Dead..
#define	N3_REGENE				0x12	// Player Regene..
#define N3_TIME					0x13	// 게임상의 시간
#define N3_WEATHER				0x14	// 게임상의 날씨변화
#define	N3_UPDATE_REGION_UPC	0x15	// Player Regene..
#define	N3_UPDATE_REGION_NPC	0x1C	// NPC Region Update
#define	N3_REQUEST_NPC_IN		0x1D	// NPC 정보 요청
#define N3_WARP					0x1E	// Warp !!!
#define	N3_ITEM_MOVE			0x1F	// Item Move..
#define N3_HP_CHANGE			0x17	// Player stats Update
#define N3_MSP_CHANGE			0x18	// Player stats Update
#define N3_EXP_CHANGE			0x1A	// Player stats Update
#define N3_LEVEL_CHANGE			0x1B	// Player stats Update
#define N3_REALM_POINT_CHANGE	0x2A	// Player stats Update // 국가 기여도
#define N3_REQUEST_USER_IN		0x16	// Update Region 다음에 필요한 User In 요청
#define N3_NPC_EVENT			0x20	// NPC Event 예를 들면 상거래..
#define	N3_ITEM_TRADE_START		0x25	// 상거래 패킷.. 서버가 줌..
#define N3_TARGET_HP			0x22	// Target Character HP Percentage 요청
#define N3_ITEM_BUNDLE_DROP		0x23	// 죽으면서 Item 드롭
#define N3_ITEM_BUNDLE_OPEN_REQUEST 0x24	// 아이템상자를 열거나 시체를 뒤진다.. 요청과 데이터 받기 공통..
#define	N3_ITEM_TRADE			0x21	// 상거래 패킷.. 서버가 줌..
	enum e_SubPacket_Trade		{	N3_SP_TRADE_BUY  = 0x01,
									N3_SP_TRADE_SELL = 0x02,
									N3_SP_TRADE_MOVE = 0x03	};

#define	N3_ITEM_DROPPED_GET		0x26	// 유저가 획득한 아이템에 대한 거..
#define N3_ZONE_CHANGE			0x27	// 존체인지.. Recv - b1 존번호 f3 좌표 X, Z, Y | Recv
#define N3_POINT_CHANGE			0x28	// 레벨 체인지 -	 보낼때 b1(1힘 2체력 3민첩 4지능 5마력) s(-1 +1)  // 받을때 b1(1힘 2체력 3민첩 4지능 5매력) s(절대수치)
#define N3_STATE_CHANGE			0x29	// 상태변화 Send - b2(Type, State) | Recv S1(ID) b2(Type, State) - // Type 1 앉기서기, 2 파티구함...
	enum e_SubPacket_State {	N3_SP_STATE_CHANGE_SITDOWN = 0x01,
								N3_SP_STATE_CHANGE_RECRUIT_PARTY = 0x02,
								N3_SP_STATE_CHANGE_SIZE = 0x03,
								N3_SP_STATE_CHANGE_ACTION = 0x04,			// 1 - 인사, 11 - 도발
								N3_SP_STATE_CHANGE_VISIBLE = 0x05 };		// 투명 0 ~ 255

#define N3_VERSION_CHECK		0x2B	// 버전 체크... s1 - Version
//#define N3_CRYPTION_PUBLIC_KEY	0x2C	// 암호화 64 비트 공개키 Recv 64i
#define N3_USER_LOOK_CHANGE		0x2D	// 플레이어의 복장이나 무기가 바뀐다..
#define N3_NOTICE				0x2E	// 공지사항..

#define N3_PARTY_OR_FORCE		0x2F	// Party Or Force Packet... Send - b1(Party:1, Force:2)
	// Sub Packet
	enum e_SubPacket_Party {	N3_SP_PARTY_OR_FORCE_CREATE			= 0x01,	// Send - s1(ID)	| Recv b1(YesNoErr)
							N3_SP_PARTY_OR_FORCE_PERMIT			= 0x02,	// Send - b1(YesNo) | Recv - s1(ID)
							N3_SP_PARTY_OR_FORCE_INSERT			= 0x03,	// Send - s1(ID) | Recv - s3(ID, HPMax, HP) b2(Level, Class) - 문자열은 ID 로 알아낸다.. ID 가 -1 이면.. 파티에 들어오는것을 상대방이 거절한거다..
							N3_SP_PARTY_OR_FORCE_REMOVE			= 0x04,	// Send - s1(ID) | Recv - s1(ID) - 자기 자신이면 파티를 깨야 한다..
							N3_SP_PARTY_OR_FORCE_DESTROY			= 0x05,	// Send
							N3_SP_PARTY_OR_FORCE_HP_CHANGE		= 0x06,	// Recv - s3(ID, HPMax, HP)
							N3_SP_PARTY_OR_FORCE_LEVEL_CHANGE	= 0x07,	// Recv - s1(ID), b1(Level)
							N3_SP_PARTY_OR_FORCE_CLASS_CHANGE	= 0x08,	// Recv - s1(ID), b1(Class)드물지만 전직할때...
							N3_SP_PARTY_OR_FORCE_STATUS_CHANGE	= 0x09 };	// Recv - s1(ID), b1(Status)독, 저주, 지속성마법, 축복

#define N3_PER_TRADE			0x30	// Personal Trade..
	// Sub Packet
	enum e_SubPacket_PerTrade {	N3_SP_PER_TRADE_REQ =			0x01, 
								N3_SP_PER_TRADE_AGREE =			0x02,
								N3_SP_PER_TRADE_ADD =			0x03,
								N3_SP_PER_TRADE_OTHER_ADD =		0x04,
								N3_SP_PER_TRADE_DECIDE	=		0x05,
								N3_SP_PER_TRADE_OTHER_DECIDE =	0x06,
								N3_SP_PER_TRADE_DONE =			0x07,
								N3_SP_PER_TRADE_CANCEL =		0x08 };	

//	마법효과 패킷 구조(모두 공통)
//	(BYTE)	N3_MAGIC
//	(BYTE)	N3_MAGIC_CASTING or flying or effecting or fail
//	(DWORD)	Magic ID
//	(short)	시전자 ID
//	(short) 타겟ID
//	(short) data1
//	(short) data2
//	(short) data3
//	(short) data4
//	(short) data5
//	(short) data6
#define N3_MAGIC				0x31	//마법효과..
	// Sub Packet
	enum e_SubPacket_Magic {	N3_SP_MAGIC_CASTING =	0x01,
								N3_SP_MAGIC_FLYING =	0x02,
								N3_SP_MAGIC_EFFECTING =	0x03,
								N3_SP_MAGIC_FAIL = 		0x04,
								N3_SP_MAGIC_TYPE4BUFFTYPE = 0x05,
								N3_SP_MAGIC_CANCEL = 0x06								
	};

#define N3_SKILL_CHANGE			0x32	// Skill Point Change..
#define N3_OBJECT_EVENT			0x33	// Send - s4(Object Event ID, Parameter, 조종할 NPC ID, 조종할 NPC Command) | Recv 

#define N3_CLASS_CHANGE			0x34	// Class Change..
	// Sub Packet..
	enum e_SubPacket_ClassChange_Main {
		N3_SP_CLASS_CHANGE_PURE = 0x01,		// Class Change Real..
		N3_SP_CLASS_CHANGE_REQ  = 0x02,		// Class Change Req..
		N3_SP_CLASS_ALL_POINT   = 0x03,		// Class Change All Point..
		N3_SP_CLASS_SKILL_POINT = 0x04,
		N3_SP_CLASS_POINT_CHANGE_PRICE_QUERY = 0x05	};	// Class Change Skill Point..

	enum e_SubPacket_ClassChange {	N3_SP_CLASS_CHANGE_SUCCESS	= 0x01,		// Success..
									N3_SP_CLASS_CHANGE_NOT_YET	= 0x02,		// Not yet..
									N3_SP_CLASS_CHANGE_ALREADY = 0x03,		// Already..
									N3_SP_CLASS_CHANGE_ITEM_IN_SLOT = 0x04,		// Item in Slot..
									N3_SP_CLASS_CHANGE_FAILURE	= 0x00 };	// Failure..

#define N3_CHAT_SELECT_TARGET		0x35	// Send - s1(ID String Length) str1(ID String) | Recv - s1(ID String Length) str1(ID String) 문자열 길이가 0이면 실패..
#define N3_CONCURRENT_USER_COUNT	0x36	// Send - ... | Recv - s1(현재 동시 접속자수)
#define N3_REQUEST_GAME_SAVE		0x37	// 서버에 저장 요청.. 받는건 없다..

#define N3_DURABILITY_CHANGE		0x38	// 내구력 변경..

#define N3_TIME_NOTIFY				0x39	// 패킷에서 아무것도 안보낸지 2초 지나면.. 이걸 보낸다..

#define N3_ITEM_TRADE_REPAIR		0x3a	// 아이템 매매, 교환, 수리..
#define N3_ITEM_REPAIR_REQUEST		0x3b	// 아이템 수리 응답..


#define N3_KNIGHTS					0x3C	// Knights Related Packet..
	enum e_SubPacket_Knights {	N3_SP_KNIGHTS_CREATE =				0x01, // 생성 Send - s1(Name Length) str1 | Recv - b1(1:성공 0:실패)
								N3_SP_KNIGHTS_JOIN =				0x02, // 가입 Send - s1(Knights ID) | Recv - b1(1:성공 0:실패)
								N3_SP_KNIGHTS_WITHDRAW =			0x03, // 탈퇴 Send - | Recv - b1(1:성공 0:실패)
								N3_SP_KNIGHTS_MEMBER_REMOVE =		0x04, // 멤버 삭제 - 
								N3_SP_KNIGHTS_DESTROY	=			0x05, // 뽀개기 Send - | Recv - b1(1:성공 0:실패)
								N3_SP_KNIGHTS_MEMBER_JOIN_ADMIT =	0x06, // 멤버 가입 허가 Send - s1(Knights ID) | Recv - b1(1:성공 0:실패)
								N3_SP_KNIGHTS_MEMBER_JOIN_REJECT =	0x07, // 멤버 가입 거절 Send - s1(Knights ID) | Recv - b1(1:성공 0:실패)
								N3_SP_KNIGHTS_MEMBER_PUNISH =		0x08, // 멤버 징계 - 가입과 같음
								N3_SP_KNIGHTS_APPOINT_CHIEF =		0x09, // 단장 임명 - 가입과 같음
								N3_SP_KNIGHTS_APPOINT_VICECHIEF =	0x0A, // 부단장 임명 - 가입과 같음
								N3_SP_KNIGHTS_APPOINT_OFFICER =		0x0B, // 장교임명 - 가입과 같음
								N3_SP_KNIGHTS_GRADE_CHANGE_ALL =	0x0C, // 전 기사단의 등급 및 순위 변경 Recv - s1(count) Loop { s1(Knights ID) b2(Grade Rank) }
								N3_SP_KNIGHTS_MEMBER_INFO_ALL =		0x0D, // 모든 멤버 요청 Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
								N3_SP_KNIGHTS_MEMBER_INFO_ONLINE =	0x0E, // 현재 접속 리스트 Send - s1(page) | s1(Member Count) Loop { s1(Name Length) str1 (Name) }
								N3_SP_KNIGHTS_STASH =				0x0F, // 기사단 창고
								N3_SP_KNIGHTS_DUTY_CHANGE =			0x10, // 멤버의 직위 변경.. 해당 멤버에게 간다.. Recv - s1(Knights ID) b1(직위);
								N3_SP_KNIGHTS_JOIN_REQ =			0x11, // 기사단 인덱스
								N3_SP_KNIGHTS_UNKNOWN };

	enum e_SubPacket_KNights_Create {	N3_SP_KNIGHTS_CREATE_FAIL_DBFAIL =			0x00,
										N3_SP_KNIGHTS_CREATE_SUCCESS =				0x01,
										N3_SP_KNIGHTS_CREATE_FAIL_LOWLEVEL =		0x02,
										N3_SP_KNIGHTS_CREATE_FAIL_DUPLICATEDNAME =	0x03,
										N3_SP_KNIGHTS_CREATE_FAIL_LOWMONEY =		0x04,
										N3_SP_KNIGHTS_CREATE_FAIL_ALREADYJOINED =	0x05,
										N3_SP_KNIGHTS_CREATE_FAIL_UNKNOWN =			0x06,
										N3_SP_KNIGHTS_CREATE_FAIL_INVALIDDAY =		0x07,
										N3_SP_KNIGHTS_CREATE_FAIL_INVALIDSERVER =	0x08 };

	enum e_SubPacket_KNights_Common {	N3_SP_KNIGHTS_COMMON_DBFAIL =				0x00,	//DB검색 실패..
										N3_SP_KNIGHTS_COMMON_SUCCESS =				0x01,	//성공
										N3_SP_KNIGHTS_COMMON_FAIL_NONE_USER =		0x02,	//없는 유저..
										N3_SP_KNIGHTS_COMMON_FAIL_DEAD_USER =		0x03,	//상대유저가 죽어 있음..
										N3_SP_KNIGHTS_COMMON_FAIL_ENEMY_USER =		0x04,	//상대유저의 국가가 다름..
										N3_SP_KNIGHTS_COMMON_FAIL_OTHER_CLAN_USER =	0x05,	//상대유저가 이미 다른 클랜이나 기사단에 가입되어 있음..
										N3_SP_KNIGHTS_COMMON_FAIL_INVALIDRIGHT =	0x06,	//권한이 없음..
										N3_SP_KNIGHTS_COMMON_FAIL_NONE_CLAN =		0x07,	//존재하지 않는 기사단..									
										N3_SP_KNIGHTS_COMMON_FAIL_FULL =			0x08,	//인원이 풀..
										N3_SP_KNIGHTS_COMMON_FAIL_ME =				0x09,	//자기자신을 선택한 경우..
										N3_SP_KNIGHTS_COMMON_FAIL_NOT_JOINED =		0x0A,	//기사단이나 클랜에 가입되어 있지 않음...
										N3_SP_KNIGHTS_COMMON_FAIL_REJECT =			0x0B,	//상대편에서 거절...
										N3_SP_KNIGHTS_COMMON_FAIL_BATTLEZONE =		0x0C	// 전쟁존에서의 거부..
								};	// join, appoint, leave, withdraw 모두 쓴다.....
	
#define N3_ITEM_COUNT_CHANGE		0x3d
#define N3_KNIGHTS_LIST_BASIC		0x3e // 기사단 정보 요청
	enum e_SubPacket_KnightsList {	N3_SP_KNIGHTS_LIST_BASIC_ALL = 0x01,  // Receive - s1(knights Count) { s21(id, 이름길이), str1(이름) }
									N3_SP_KNIGHTS_LIST_BASIC_INSERT = 0x02, // Receive - s2(id, 이름길이), str1(이름)
									N3_SP_KNIGHTS_LIST_BASIC_REMOVE = 0x03 }; // Receive - s1(id)
#define N3_ITEM_DESTROY				0x3f
#define N3_ADMINISTRATOR			0x40 // 관리자 전용 패킷..
	enum e_SubPacket_Administrator { N3_SP_ADMINISTRATOR_ARREST = 0x01, // 그 유저 있는 곳으로 존체인지.. Send b1(Type) s1(유저이름길이), str1(유저이름)
									N3_SP_ADMINISTRATOR_FORBID_CONNECT = 0x02, // 접속금지 및 쫓아내기.. Send b1(Type) s1(유저이름길이), str1(유저이름)
									N3_SP_ADMINISTRATOR_CHAT_FORBID = 0x03, // 채팅금지 Send b1(Type) s1(유저이름길이), str1(유저이름)
									N3_SP_ADMINISTRATOR_CHAT_PERMIT = 0x04 }; // 채팅허가 Send b1(Type) s1(유저이름길이), str1(유저이름)
									
#define N3_CHECK_SPEEDHACK			0x41 // 스피드핵 체크용 패킷..

#define N3_COMPRESSED_PACKET		0x42 // 압축된 패킷,.. 풀어서 한번더 파싱해야 한다..
#define N3_CONTINOUS_PACKET			0x44 // 붙어서 오는 패킷.. 루프를 돌면서 파싱해야 한다..

#define N3_WAREHOUSE				0X45 // 보관함 메인 패킷..
	enum e_SubPacket_WareHouse {	N3_SP_WARE_OPEN = 				0x01,	// Open..
									N3_SP_WARE_GET_IN =				0x02,	// 녛기..
									N3_SP_WARE_GET_OUT =			0X03,	// 빼기..
									N3_SP_WARE_WARE_MOVE =			0x04,	// 이동..
									N3_SP_WARE_INV_MOVE =			0x05,	// 이동..
									N3_SP_WARE_INN =				0x10,	// 맨처음..여관주인임을 받음..
									N3_SP_WARE_UNKNOWN };

#define N3_SERVER_CHANGE			0x46
#define N3_REPORT_DEBUG_STRING		0x47
#define N3_HOME						0x48	// 마을로 가기.. 응답으로 워프를 받는다.
#define N3_FRIEND_INFO				0x49	// 친구 정보.. Send s1(유저카운트) s1(이름길이), str1(유저이름) | Receive s1(유저카운트) ㅣLoop { s1(이름길이), str1(유저이름), b1(접속, 파티 or 연산) }
#define N3_NOAH_CHANGE	0x4a	// 노아 Change..
	enum e_SubPacket_NoahChange {	N3_SP_NOAH_GET = 0x01,	// 노아 습득,
									N3_SP_NOAH_LOST	= 0x02,	// 노아 분실..
									N3_SP_NOAH_SPEND = 0x03	// 노아 소비..
								};		
#define	N3_WARP_LIST					0x4b	// 워프 리스트 Recv -
//#define	N3_SERVER_CHECK					0x4c	// 가상 임시 서버 체크... Recv - s1, str1(IP), s1(dwPort) | Send s1(Zone ID), s3(x*10,z*10,y*10)
//#define	N3_SERVER_CONCURRENT_CONNECT	0x4d	// 동시접속자수...  Send - b1(국가) | Recv - s1(동접자수)
#define N3_CORPSE_CHAR					0x4e	// 캐릭터가 다시 regen을 하여 시체로 남긴다.
#define N3_PARTY_BBS					0x4f	// 파티 게시판에 등록하기
	enum e_Party_BBS {	N3_SP_PARTY_REGISTER						= 0x01,		// 파티 게시판에 등록
						N3_SP_PARTY_REGISTER_CANCEL					= 0x02,		// 파티 게시판에서 등록 취소
						N3_SP_PARTY_BBS_DATA						= 0x03};	// 파티 게시 정보 요구

#define N3_TRADE_BBS					0x50	// 상거래 게시판
	enum e_Trade_BBS_Kind { N3_SP_TRADE_BBS_BUY						= 0x01,		// 사는 물건 목록
							N3_SP_TRADE_BBS_SELL					= 0x02};		// 파는 물건 목록

	enum e_Trade_BBS_Sub  { N3_SP_TYPE_REGISTER						= 0x01,		// 물건 등록하기
							N3_SP_TYPE_REGISTER_CANCEL				= 0x02,		// 등록 해제하기
							N3_SP_TYPE_BBS_DATA						= 0x03,		// 게시판 정보 요구
							N3_SP_TYPE_BBS_OPEN						= 0x04,		// 상거래 게시판 열기
							N3_SP_TYPE_BBS_TRADE					= 0X05};	// 게시판에서 거래 신청하기
#define N3_KICK_OUT						0x51	// Recv s1, str1(IP) s1(port) | Send s1, str1(ID)
#define N3_NPC_QUEST					0X52	// npc 퀘스트
#define N3_ALIVE_CHECK					0x53	// 단순 에코
#define N3_ITEM_WEIGHT_CHANGE			0x54
#define N3_QUEST_SELECT					0x55	// 퀘스트 메뉴 선택
#define N3_QUEST_TALK					0x56	// 퀘스트 대화









#define N3_TEMP_TEST 0xff // 임시 테스트..

#endif // end of #ifndef __PACKET_DEF_H_

