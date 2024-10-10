#pragma once

#include <set>
#include <map>

struct _KING_ELECTION_LIST
{
	uint16_t sKnights;
	uint32_t nVotes;
};

struct ci_compare
{
	bool operator() (const std::string & str1, const std::string & str2) const {
		return STRCASECMP(str1.c_str(), str2.c_str()) < 0;
	}
};

typedef std::set<uint16_t> ClanIDSet;
typedef std::map<std::string, std::string, ci_compare> KingCandidacyNoticeBoardMap; 
typedef std::map<std::string, _KING_ELECTION_LIST *, ci_compare> KingElectionList; 
typedef std::set<std::string, ci_compare> ResignedCandidateList;

enum ElectionType
{
	ELECTION_TYPE_NO_TERM		= 0,
	ELECTION_TYPE_NOMINATION	= 1,
	ELECTION_TYPE_PRE_ELECTION	= 2, // not sure if this is accurately named
	ELECTION_TYPE_ELECTION		= 3,

	ELECTION_TYPE_TERM_STARTED	= 6, // these two probably aren't accurately named at all
	ELECTION_TYPE_TERM_ENDED	= 7,

};

enum ImpeachmentType
{
};

class CUser;
class CKingSystem
{
public:
	CKingSystem();

	// Handles timed events.
	void CheckKingTimer();

	// Updates the election status
	void UpdateElectionStatus(uint8_t byElectionState);
	void UpdateElectionList(uint8_t byElectionListType, bool bDeleteList, uint16_t sClanID, std::string & strUserID, CUser * pUser = nullptr);

	// Checks to see if a special (coin/XP) event should end.
	void CheckSpecialEvent();

	// Generates a list of the top 10 clan leaders eligible to nominate a King.
	void LoadRecommendList();

	void KingNotifyMessage(uint32_t nResourceID, int byNation, ChatType byChatType);

	// Wrapper to lookup the appropriate King system instance
	static void PacketProcess(CUser * pUser, Packet & pkt);

	// The real method to handle packets from the client.
	void KingPacketProcess(CUser * pUser, Packet & pkt);

	/* Election system */
	void ElectionSystem(CUser * pUser, Packet & pkt);
	void ElectionScheduleConfirmation(CUser * pUser, Packet & pkt);
	void CandidacyRecommend(CUser * pUser, Packet & pkt);
	void InsertNominee(std::string & strNominee);
	void CandidacyNoticeBoard(CUser * pUser, Packet & pkt);
	void ElectionPoll(CUser * pUser, Packet & pkt);
	void CandidacyResign(CUser * pUser, Packet & pkt);

	/* Impeachment system */
	void ImpeachmentSystem(CUser * pUser, Packet & pkt);
	void ImpeachmentRequest(CUser * pUser, Packet & pkt);
	void ImpeachmentRequestElect(CUser * pUser, Packet & pkt);
	void ImpeachmentList(CUser * pUser, Packet & pkt);
	void ImpeachmentElect(CUser * pUser, Packet & pkt);
	void ImpeachmentRequestUiOpen(CUser * pUser, Packet & pkt);
	void ImpeachmentElectionUiOpen(CUser * pUser, Packet & pkt);

	void KingTaxSystem(CUser * pUser, Packet & pkt);
	void KingSpecialEvent(CUser * pUser, Packet & pkt);

	void SendUDP_ElectionStatus(uint8_t m_byType);
	void GetElectionResult(uint8_t Nation);
	static void HandleDatabaseRequest(CUser * pUser, Packet & pkt);
	static void HandleDatabaseRequest_Election(CUser * pUser, Packet & pkt);
	static void HandleDatabaseRequest_Event(CUser * pUser, Packet & pkt);
	static void HandleDatabaseRequest_Tax(uint8_t TerritoryTariff, uint8_t Nation, uint32_t TerritoryTax = 0);

	void ResetElectionLists();
	~CKingSystem();

	uint8_t	m_byNation;

	uint8_t	m_byType;
	uint16_t	m_sYear;
	uint8_t	m_byMonth;
	uint8_t	m_byDay;
	uint8_t	m_byHour;
	uint8_t	m_byMinute;

	uint8_t	m_byImType;
	uint16_t	m_sImYear;
	uint8_t	m_byImMonth;
	uint8_t	m_byImDay;
	uint8_t	m_byImHour;
	uint8_t	m_byImMinute;

	uint8_t	m_byNoahEvent;
	uint8_t	m_byNoahEvent_Day;
	uint8_t	m_byNoahEvent_Hour;
	uint8_t	m_byNoahEvent_Minute;
	uint16_t	m_sNoahEvent_Duration;

	uint8_t	m_byExpEvent;
	uint8_t	m_byExpEvent_Day;
	uint8_t	m_byExpEvent_Hour;
	uint8_t	m_byExpEvent_Minute;
	uint16_t	m_sExpEvent_Duration;

	uint32_t	m_nTribute;
	uint8_t  m_nTerritoryTariff;
	uint32_t	m_nTerritoryTax;
	uint32_t	m_nNationalTreasury;

	std::string m_strKingName;
	std::string m_strImRequestID;

	std::recursive_mutex m_lock;
	
	ClanIDSet m_top10ClanSet;

	KingCandidacyNoticeBoardMap m_noticeBoardMap;

	/**
	* @brief	List of senators.
	* 			Senators are the top 10 clan leaders and are
	* 			able to nominate a King or propose impeachment.
	*/
	KingElectionList m_senatorList;

	/**
	* @brief	List of candidates still in the running for King.
	* 			Candidates for election must be senators,
	* 			so this data just refers to the above senator list.
	*/
	KingElectionList m_candidateList;

	/**
	* @brief	List of resigned candidates.
	*/
	ResignedCandidateList m_resignedCandidateList;

	// TODO: Give this a more appropriate name.
	bool m_bSentFirstMessage;
};