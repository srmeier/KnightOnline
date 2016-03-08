#pragma once

class CUser;
class CGameServerDlg;
class CKnights;
class Packet;

class CKnightsManager  
{
public:
	static void CreateKnights(CUser* pUser, Packet & pkt);
	static void JoinKnights(CUser* pUser, Packet & pkt);
	static void WithdrawKnights(CUser* pUser, Packet & pkt);
	static void ModifyKnightsMember(CUser* pUser, Packet & pkt, uint8 opcode);
	static void ModifyKnightsLeader(CUser* pUser, Packet & pkt, uint8 opcode);
	static void ModifyKnightsPointMethod(CUser* pUser, Packet & pkt);
	static void DestroyKnights(CUser* pUser);
	static void AllKnightsList(CUser* pUser, Packet & pkt);
	static void AllKnightsMember(CUser* pUser);
	static void CurrentKnightsMember(CUser* pUser, Packet & pkt);
	static void JoinKnightsReq(CUser* pUser, Packet & pkt);
	static void RegisterClanSymbol(CUser* pUser, Packet & pkt);
	static void RequestClanSymbolVersion(CUser* pUser, Packet & pkt);
	static void RequestClanSymbols(CUser* pUser, Packet & pkt);
	static void GetClanSymbol(CUser* pUser, uint16 sClanID);
	static bool CheckAlliance(CKnights * pMainClan, CKnights * pTargetClan);
	static void KnightsAllianceDestroy(CUser* pUser, Packet & pkt);
	static void KnightsAllianceCreate(CUser* pUser, Packet & pkt);
	static void KnightsAllianceRequest(CUser* pUser, Packet & pkt);
	static void KnightsAllianceInsert(CUser* pUser, Packet & pkt);
	static void KnightsAllianceRemove(CUser* pUser, Packet & pkt);
	static void KnightsAlliancePunish(CUser* pUser, Packet & pkt);
	static void KnightsAllianceList(CUser* pUser, Packet & pkt);
	static void ListTop10Clans(CUser *pUser);
	static void DonateNPReq(CUser *pUser, Packet & pkt);
	static void DonateNP(CUser *pUser, Packet & pkt);
	static void DonationList(CUser *pUser, Packet & pkt);

	static bool AddKnightsUser(int index, std::string & strUserID);
	static void AddUserDonatedNP(int index, std::string & strUserID, uint32 nDonatedNP, bool bIsKillReward = false);
	static bool RemoveKnightsUser(int index, std::string & strUserID);
	static bool LoadKnightsIndex(int index);
	static bool LoadAllKnights();
	static void UpdateKnightsGrade(uint16 sClanID, uint8 byFlag);
	static void UpdateClanPoint(uint16 sClanID, int32 nChangeAmount);

	// database requests go here
	static void ReqKnightsPacket(CUser* pUser, Packet & pkt);
	static void ReqCreateKnights(CUser* pUser, Packet & pkt);
	static void ReqUpdateKnights(CUser* pUser, Packet & pkt, uint8 opcode);
	static void ReqModifyKnightsMember(CUser* pUser, Packet & pkt, uint8 command);
	static void ReqDestroyKnights(CUser* pUser, Packet & pkt);
	static void ReqAllKnightsMember(CUser *pUser, Packet & pkt);
	static void ReqKnightsList(Packet & pkt);
	static void ReqRegisterClanSymbol(CUser *pUser, Packet & pkt);
	static void ReqKnightsAllianceCreate(CUser *pUser, Packet & pkt);
	static void ReqKnightsAllianceRequest(CUser *pUser, Packet & pkt);
	static void ReqKnightsAllianceRemove(CUser *pUser, Packet & pkt);
	static void ReqKnightsAlliancePunish(CUser *pUser, Packet & pkt);
	static void ReqUpdateGrade(Packet & pkt);
	static void ReqDonateNP(CUser *pUser, Packet & pkt);
	static void ReqRefundNP(Packet & pkt);
	static void ReqUpdateNP(Packet & pkt);
	static void ReqUpdateClanNotice(Packet & pkt);

	static void RecvUpdateKnights(CUser* pUser, Packet & pkt, uint8 command);
	static void RecvModifyFame(CUser* pUser, Packet & pkt, uint8 command);
	static void RecvKnightsAllList(Packet & pkt);

	static int GetKnightsIndex( int nation );
	static bool IsAvailableName( const char* strname);	
	static void PacketProcess(CUser* pUser, Packet & pkt);	

};