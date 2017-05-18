#pragma once

#define MAX_CLAN_USERS			36
#define MIN_NATIONAL_POINTS		500
#define MIN_NP_TO_DONATE		1000
#define MAX_CLAN_NOTICE_LENGTH	128

class CUser;
struct _KNIGHTS_USER
{
	uint8_t    byUsed;
	std::string strUserName;
	uint32_t	nDonatedNP;
	CUser	*pSession;

	INLINE _KNIGHTS_USER() { Initialise(); }
	INLINE void Initialise()
	{
		byUsed = 0;
		strUserName.clear();
		nDonatedNP = 0;
		pSession = nullptr;
	}
};

enum ClanTypeFlag
{
	ClanTypeNone		= 0,
	ClanTypeTraining	= 1,
	ClanTypePromoted	= 2,
	ClanTypeAccredited5	= 3,
	ClanTypeAccredited4	= 4,
	ClanTypeAccredited2	= 5,
	ClanTypeAccredited3	= 6,
	ClanTypeAccredited1	= 7,
	ClanTypeRoyal5		= 8,
	ClanTypeRoyal4		= 9,
	ClanTypeRoyal3		= 10,
	ClanTypeRoyal2		= 11,
	ClanTypeRoyal1		= 12
};

class CKnights  
{
public:
	uint16_t	m_sIndex;
	uint8_t	m_byFlag;			// 1 : Clan, 2 : Knights
	uint8_t	m_byNation;			// nation
	uint8_t	m_byGrade;
	uint8_t	m_byRanking;
	uint16_t	m_sMembers;

	std::string m_strName;
	std::string m_strChief, m_strViceChief_1, m_strViceChief_2, m_strViceChief_3;

	uint64_t	m_nMoney;
	uint16_t	m_sDomination;
	uint32_t	m_nPoints;
	Atomic<uint32_t> m_nClanPointFund; // stored in national point form
	uint16_t	m_sMarkVersion, m_sMarkLen;
	char	m_Image[MAX_KNIGHTS_MARK];
	uint16_t	m_sCape;
	uint8_t	m_bCapeR, m_bCapeG, m_bCapeB;
	uint16_t	m_sAlliance;
	uint8_t	m_sClanPointMethod;
	std::string m_strClanNotice;
	uint8_t	bySiegeFlag;
	uint16_t	nLose,nVictory;


	_KNIGHTS_USER m_arKnightsUser[MAX_CLAN_USERS];

	INLINE uint16_t GetID() { return m_sIndex; }
	INLINE uint16_t GetAllianceID() { return m_sAlliance; }
	INLINE uint16_t GetCapeID(CKnights *pKnights) 
	{
		if (isInAlliance())
			return m_sCape = pKnights->m_sCape;
		else
			return m_sCape;
	}
	INLINE std::string & GetName() { return m_strName; }
	INLINE uint8_t GetClanPointMethod() { return m_sClanPointMethod; }

	INLINE bool isPromoted() { return m_byFlag >= ClanTypePromoted; }
	INLINE bool isInAlliance() { return m_sAlliance > 0; }
	INLINE bool isAllianceLeader() { return GetAllianceID() == GetID(); }

	CKnights();

	// Attach our session to the clan's list & tell clannies we logged in.
	void OnLogin(CUser *pUser);
	void OnLoginAlliance(CUser *pUser);

	void ConstructClanNoticePacket(Packet *result);
	void UpdateClanNotice(std::string & clanNotice);

	void UpdateClanFund();

	// Detach our session from the clan's list & tell clannies we logged off.
	void OnLogout(CUser *pUser);
	void OnLogoutAlliance(CUser *pUser);

	bool AddUser(std::string & strUserID);
	bool AddUser(CUser *pUser);

	bool RemoveUser(std::string & strUserID);
	bool RemoveUser(CUser *pUser);

	void RefundDonatedNP(uint32_t nDonatedNP, CUser * pUser = nullptr, const char * strUserID = nullptr);

	void Disband(CUser *pLeader = nullptr);

	void SendUpdate();
	void SendChat(const char * format, ...);
	void SendChatAlliance(const char * format, ...);
	void Send(Packet *pkt);

	virtual ~CKnights();
};
