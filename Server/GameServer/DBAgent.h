#pragma once

#include "../shared/database/OdbcConnection.h"

enum UserUpdateType
{
	UPDATE_LOGOUT,
	UPDATE_ALL_SAVE,
	UPDATE_PACKET_SAVE,
};

enum RentalType
{
	RENTAL_TYPE_IN_LIST		= 1,
	RENTAL_TYPE_LENDER		= 2,
	RENTAL_TYPE_BORROWER	= 3
};

struct _USER_RENTAL_ITEM
{
	std::string strUserID;
	uint64_t nSerialNum;
	uint32_t nRentalIndex, nItemID, nRentalMoney;
	uint16_t sDurability, sRentalTime;
	int16_t sMinutesRemaining;
	uint8_t byRentalType, byRegType;
	char szTimeRental[30];

	_USER_RENTAL_ITEM()
	{
		memset(&szTimeRental, 0, sizeof(szTimeRental));
	}
};

typedef std::map<uint64_t, _USER_RENTAL_ITEM *> UserRentalMap;

class Packet;
class CUser;
struct _ITEM_DATA;
class CDBAgent  
{
public:
	CDBAgent();

	bool Startup(
		bool bMarsEnabled,
		const tstring& strAccountDSN,
		const tstring& strAccountUID,
		const tstring& strAccountPWD,
		const tstring& strGameDSN,
		const tstring& strGameUID,
		const tstring& strGamePWD);

	bool Connect(
		bool bMarsEnabled,
		const tstring& strAccountDSN,
		const tstring& strAccountUID,
		const tstring& strAccountPWD,
		const tstring& strGameDSN,
		const tstring& strGameUID,
		const tstring& strGamePWD);

	void ReportSQLError(OdbcError* pError);

	int8_t AccountLogin(const std::string& strAccountID, const std::string& strPasswd);
	uint8_t NationSelect(const std::string& strAccountID, uint8_t bNation);
	bool GetAllCharID(const std::string& strAccountID, std::string& strCharID1, std::string& strCharID2, std::string& strCharID3);
	void LoadCharInfo(const std::string& strCharID, ByteBuffer& result);

	int8_t CreateNewChar(const std::string& strAccountID, int index, const std::string& strCharID, uint8_t bRace, uint16_t sClass, uint8_t nHair, uint8_t bFace, uint8_t bStr, uint8_t bSta, uint8_t bDex, uint8_t bInt, uint8_t bCha);
	int8_t DeleteChar(const std::string& strAccountID, int index, const std::string& strCharID, const std::string& strSocNo);

	void LoadRentalData(const std::string& strAccountID, const std::string& strCharID, UserRentalMap& rentalData);

	bool LoadUserData(const std::string& strAccountID, const std::string& strCharID, CUser* pUser);
	bool LoadWarehouseData(const std::string& strAccountID, CUser* pUser);
	bool LoadPremiumServiceUser(const std::string& strAccountID, CUser* pUser);
	bool LoadSavedMagic(CUser* pUser);
	bool SetLogInInfo(const std::string& strAccountID, const std::string& strCharID, const std::string& strServerIP, int16_t sServerNo, const std::string& strClientIP, uint8_t bInit);
	bool SavePremiumServiceUser(CUser* pUser);

	bool LoadWebItemMall(std::vector<_ITEM_DATA>& itemList, CUser* pUser);

	bool LoadSkillShortcut(Packet& result, CUser* pUser);
	void SaveSkillShortcut(int16_t sCount, char* buff, CUser* pUser);

	void RequestFriendList(std::vector<std::string>& friendList, CUser* pUser);
	FriendAddResult AddFriend(int16_t sid, int16_t tid);
	FriendRemoveResult RemoveFriend(const std::string& strCharID, CUser* pUser);

	bool UpdateUser(const std::string& strCharID, UserUpdateType type, CUser* pUser);
	bool UpdateWarehouseData(const std::string& strAccountID, UserUpdateType type, CUser* pUser);
	bool UpdateSavedMagic(CUser* pUser);

	int8_t CreateKnights(uint16_t sClanID, uint8_t bNation, const std::string& strKnightsName, const std::string& strChief, uint8_t bFlag = 1);
	int UpdateKnights(uint8_t bType, const std::string& strCharID, uint16_t sClanID, uint8_t bDomination);
	int DeleteKnights(uint16_t sClanID);
	uint16_t LoadKnightsAllMembers(uint16_t sClanID, Packet& result);
	bool LoadKnightsInfo(uint16_t sClanID, uint8_t& bNation, std::string& strKnightsName, uint16_t& sMembers, uint32_t& nPoints, uint8_t& bRank);
	int8_t CreateAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag);
	int8_t InsertAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag);
	int8_t RemoveAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag);
	int8_t DestoryAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag);
	void LoadKnightsAllList();
	bool UpdateClanSymbol(uint16_t sClanID, uint16_t sSymbolSize, char* clanSymbol);
	void UpdateUserAuthority(const std::string& strUserID, uint8_t sAuthority);
	NameChangeOpcode UpdateCharacterName(const std::string& strAccountID, const std::string& strUserID, const std::string& strNewUserID);
	void UpdateCape(uint16_t sClanID, uint16_t sCapeID);
	void UpdateClanGrade(uint16_t sClanID, uint8_t byFlag, uint16_t sCapeID);

	void UpdateBattleEvent(const std::string& strCharID, uint8_t bNation);
	void AccountLogout(const std::string& strAccountID);

	void UpdateConCurrentUserCount(int nServerNo, int nZoneNo, int nCount);

	void UpdateElectionStatus(uint8_t byType, uint8_t byNation);
	void UpdateElectionList(uint8_t byDBType, uint8_t byType, uint8_t byNation, uint16_t sKnights, uint32_t nAmount, const std::string& strNominee, CUser* pUser);
	int16_t UpdateElectionProc(CUser* pUser);
	int16_t UpdateCandidacyRecommend(const std::string& strNominator, const std::string& strNominee, uint8_t byNation);

	void SendUDP_ElectionStatus(uint8_t byType);

	void UpdateCandidacyNoticeBoard(const std::string& strCharID, uint8_t byNation, const std::string& strNotice);
	void UpdateNoahOrExpEvent(uint8_t byType, uint8_t byNation, uint8_t byAmount, uint8_t byDay, uint8_t byHour, uint8_t byMinute, uint16_t sDuration);
	void InsertPrizeEvent(uint8_t byType, uint8_t byNation, uint32_t nCoins, const std::string& strCharID);
	void InsertTaxEvent(uint32_t KingNationTax, uint8_t Nation, uint32_t TerritoryTax = 0);
	void InsertTaxUpEvent(uint8_t Nation, uint32_t TerritoryTax);

	void ResetLoyaltyMonthly();
	void ClearRemainUsers();
	void UpdateRanks();
	void UpdateSiege(int16_t sCastleIndex, int16_t sMasterKnights, int16_t bySiegeType, int16_t byWarDay, int16_t byWarTime, int16_t byWarMinute);
	void UpdateSiegeTax(uint8_t Zone, int16_t ZoneTarriff);

	~CDBAgent();

private:
	OdbcConnection* m_GameDB, * m_AccountDB;
	std::recursive_mutex m_lock;

	friend class CGameServerDlg;
};

extern CDBAgent g_DBAgent;