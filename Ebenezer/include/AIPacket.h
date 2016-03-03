#ifndef _AIPACKET_H_
#define _AIPACKET_H_

// ---------------------------------------------------------------------
// AI Server와 게임서버간의 Npc에 관련된 패킷은 1번~49번 
// ---------------------------------------------------------------------
const BYTE AI_SERVER_CONNECT	= 1;
const BYTE	NPC_INFO_ALL		= 2;
const BYTE	MOVE_REQ			= 3;
const BYTE	MOVE_RESULT			= 4;
const BYTE	MOVE_END_REQ		= 5;
const BYTE	MOVE_END_RESULT		= 6;
const BYTE	AG_NPC_INFO			= 7;	
const BYTE	AG_NPC_GIVE_ITEM	= 8;
const BYTE	AG_NPC_GATE_OPEN	= 9;	
const BYTE	AG_NPC_GATE_DESTORY	= 10;	
const BYTE	AG_NPC_INOUT		= 11;	
const BYTE	AG_NPC_EVENT_ITEM	= 12;	
const BYTE	AG_NPC_HP_REQ		= 13;	// sungyong tw

// ---------------------------------------------------------------------
// AI Server와 게임서버간의 User, Npc 공통 관련된 패킷은 50번~100번 
// ---------------------------------------------------------------------
const BYTE	AG_SERVER_INFO			= 50;	// 
const BYTE	AG_ATTACK_REQ			= 51;	// Attck Packet
const BYTE	AG_ATTACK_RESULT		= 52;	// Attck Packet
const BYTE	AG_DEAD					= 53;	// Dead Packet
const BYTE	AG_SYSTEM_MSG			= 54;	// System message Packet
const BYTE	AG_CHECK_ALIVE_REQ		= 55;	// Server alive check
const BYTE	AG_COMPRESSED_DATA		= 56;	// Packet Data compressed
const BYTE	AG_ZONE_CHANGE			= 57;	// Zone change
const BYTE	AG_MAGIC_ATTACK_REQ		= 58;	// Magic Attck Packet
const BYTE	AG_MAGIC_ATTACK_RESULT	= 59;	// Magic Attck Packet
const BYTE	AG_USER_INFO_ALL		= 60;	// User의 모든 정보 전송
const BYTE	AG_LONG_MAGIC_ATTACK	= 61;	// Magic Attck Packet
const BYTE	AG_PARTY_INFO_ALL		= 62;	// Party의 모든 정보 전송
const BYTE	AG_HEAL_MAGIC			= 63;	// Healing magic
const BYTE	AG_TIME_WEATHER			= 64;	// time and whether info
const BYTE	AG_BATTLE_EVENT			= 65;	// battle event
		// ---------------------------------------------------------------------
		// Battle Event Sub Packet
		// ---------------------------------------------------------------------
		const BYTE	BATTLE_EVENT_OPEN   = 1;	// battle event open
		const BYTE	BATTLE_MAP_EVENT_RESULT = 2;	// battle zone map event result
		const BYTE	BATTLE_EVENT_RESULT		= 3;	// battle event result ( victory nation )
		const BYTE	BATTLE_EVENT_MAX_USER	= 4;	// battle event result ( user name )
		const BYTE	BATTLE_EVENT_KILL_USER	= 5;	// battle event result ( user kill count )

// ---------------------------------------------------------------------
// AI Server와 게임서버간의 User에 관련된 패킷은 101번 부터 시작
// ---------------------------------------------------------------------
const BYTE	AG_USER_INFO		= 101;	// User의 정보
const BYTE	AG_USER_INOUT		= 102;	// User의 In,Out 정보
const BYTE	AG_USER_MOVE		= 103;	// User의 move 정보
const BYTE	AG_USER_MOVEEDGE	= 104;	// User의 move end 정보
const BYTE	AG_USER_SET_HP		= 105;	// User의 HP
const BYTE	AG_USER_LOG_OUT		= 106;	// User의 LogOut
const BYTE	AG_USER_REGENE		= 107;	// User의 Regene
const BYTE	AG_USER_EXP			= 108;	// User의 경험치
const BYTE	AG_USER_UPDATE		= 109;	// User의 Update Info
const BYTE	AG_USER_FAIL		= 110;	// 잘못된 유저 처리...
const BYTE	AG_USER_PARTY		= 111;	// 파티처리 담당

#endif