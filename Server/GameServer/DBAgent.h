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


	bool Startup(bool bMarsEnabled,
		tstring & strAccountDSN, tstring & strAccountUID, tstring & strAccountPWD,
		tstring & strGameDSN, tstring & strGameUID, tstring & strGamePWD);

	bool Connect(bool bMarsEnabled,
		tstring & strAccountDSN, tstring & strAccountUID, tstring & strAccountPWD,
		tstring & strGameDSN, tstring & strGameUID, tstring & strGamePWD);

	void ReportSQLError(OdbcError *pError);

	int8_t AccountLogin(std::string & strAccountID, std::string & strPasswd);
	uint8_t NationSelect(std::string & strAccountID, uint8_t bNation);
	bool GetAllCharID(std::string & strAccountID, std::string & strCharID1, std::string & strCharID2, std::string & strCharID3);
	void LoadCharInfo(std::string & strCharID, ByteBuffer & result);

	int8_t ChangeHair(std::string & strAccountID, std::string & strCharID, uint8_t bOpcode, uint8_t bFace, uint8_t nHair);

	int8_t CreateNewChar(std::string & strAccountID, int index, std::string & strCharID, uint8_t bRace, uint16_t sClass, uint8_t nHair, uint8_t bFace, uint8_t bStr, uint8_t bSta, uint8_t bDex, uint8_t bInt, uint8_t bCha);
	int8_t DeleteChar(std::string & strAccountID, int index, std::string & strCharID, std::string & strSocNo);

	void LoadRentalData(std::string & strAccountID, std::string & strCharID, UserRentalMap & rentalData);
	void LoadItemSealData(std::string & strAccountID, std::string & strCharID, UserItemSealMap & itemSealData);

	bool LoadUserData(std::string & strAccountID, std::string & strCharID, CUser *pUser);
	bool LoadWarehouseData(std::string & strAccountID, CUser *pUser);
	bool LoadPremiumServiceUser(std::string & strAccountID, CUser *pUser);
	bool LoadSavedMagic(CUser *pUser);
	bool SetLogInInfo(std::string & strAccountID, std::string & strCharID, std::string & strServerIP, int16_t sServerNo, std::string & strClientIP, uint8_t bInit);
	bool SavePremiumServiceUser(CUser *pUser);

	bool LoadWebItemMall(std::vector<_ITEM_DATA> & itemList, CUser *pUser);

	bool LoadSkillShortcut(Packet & result, CUser *pUser);
	void SaveSkillShortcut(int16_t sCount, char *buff, CUser *pUser);

	void RequestFriendList(std::vector<std::string> & friendList, CUser *pUser);
	FriendAddResult AddFriend(int16_t sid, int16_t tid);
	FriendRemoveResult RemoveFriend(std::string & strCharID, CUser *pUser);

	bool UpdateUser(std::string & strCharID, UserUpdateType type, CUser *pUser);
	bool UpdateWarehouseData(std::string & strAccountID, UserUpdateType type, CUser *pUser);
	bool UpdateSavedMagic(CUser *pUser);
	uint8_t SealItem(std::string strSealPasswd, uint64_t nItemSerial, uint32_t nItemID, uint8_t bSealType, CUser *pUser);

	int8_t CreateKnights(uint16_t sClanID, uint8_t bNation, std::string & strKnightsName, std::string & strChief, uint8_t bFlag = 1);
	int UpdateKnights(uint8_t bType, std::string & strCharID, uint16_t sClanID, uint8_t bDomination);
	int DeleteKnights(uint16_t sClanID);
	uint16_t LoadKnightsAllMembers(uint16_t sClanID, Packet & result);
	bool LoadKnightsInfo(uint16_t sClanID, uint8_t & bNation, std::string & strKnightsName, uint16_t & sMembers, uint32_t & nPoints, uint8_t & bRank);
	int8_t CreateAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag); 
	int8_t InsertAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag); 
	int8_t RemoveAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag); 
	int8_t DestoryAlliance(uint8_t byType, uint16_t shAlliancIndex, uint16_t shKnightsIndex, uint8_t  byEmptyIndex, uint8_t bySiegeFlag); 
	void LoadKnightsAllList();
	bool UpdateClanSymbol(uint16_t sClanID, uint16_t sSymbolSize, char *clanSymbol);
	bool DonateClanPoints(CUser * pUser, uint32_t amountNP);
	void RefundNP(std::string & strUserID, uint32_t nRefundNP);
	void UpdateUserAuthority(std::string & strUserID, uint16_t nAuthority);
	void UpdateClanFund(uint16_t sClanID, uint32_t nClanPointFund);
	void UpdateClanNotice(uint16_t sClanID, std::string & strClanNotice);
	NameChangeOpcode UpdateCharacterName(std::string & strAccountID, std::string & strUserID, std::string & strNewUserID);
	void UpdateCape(uint16_t sClanID, uint16_t sCapeID, uint8_t r, uint8_t g, uint8_t b);
	void UpdateClanGrade(uint16_t sClanID, uint8_t byFlag, uint16_t sCapeID);
	void UpdateAccountKnightCash(std::string & strAccountID, uint32_t KnightCash = 0);

	void UpdateBattleEvent(std::string & strCharID, uint8_t bNation);
	void AccountLogout(std::string & strAccountID);

	void UpdateConCurrentUserCount(int nServerNo, int nZoneNo, int nCount);

	uint8_t GetUnreadLetterCount(std::string & strCharID);
	bool GetLetterList(std::string & strCharID, Packet & result, bool bNewLettersOnly = true);
	int8_t SendLetter(std::string & strSenderID, std::string & strRecipientID, std::string & strSubject, std::string & strMessage, uint8_t bType, _ITEM_DATA * pItem, int32_t nCoins);
	bool ReadLetter(std::string & strCharID, uint32_t nLetterID, std::string & strMessage);
	int8_t GetItemFromLetter(std::string & strCharID, uint32_t nLetterID, uint32_t & nItemID, uint16_t & sCount, uint16_t & sDurability, uint32_t & nCoins, uint64_t & nSerialNum);
	void DeleteLetter(std::string & strCharID, uint32_t nLetterID);

	void UpdateElectionStatus(uint8_t byType, uint8_t byNation);
	void UpdateElectionList(uint8_t byDBType, uint8_t byType, uint8_t byNation, uint16_t sKnights, uint32_t nAmount, std::string & strNominee, CUser * pUser);
	int16_t UpdateElectionProc(CUser * pUser);
	int16_t UpdateCandidacyRecommend(std::string & strNominator, std::string & strNominee, uint8_t byNation);

	void SendUDP_ElectionStatus(uint8_t m_byType);

	void UpdateCandidacyNoticeBoard(std::string & strCharID, uint8_t byNation, std::string & strNotice);
	void UpdateNoahOrExpEvent(uint8_t byType, uint8_t byNation, uint8_t byAmount, uint8_t byDay, uint8_t byHour, uint8_t byMinute, uint16_t sDuration);
	void InsertPrizeEvent(uint8_t byType, uint8_t byNation, uint32_t nCoins, std::string & strCharID);
	void InsertTaxEvent(uint8_t KingNationTax, uint8_t Nation, uint32_t TerritoryTax = 0);
	void InsertTaxUpEvent(uint8_t Nation, uint32_t TerritoryTax);
	
	void ResetLoyaltyMonthly();
	void ClearRemainUsers();
	void InsertUserDailyOp(_USER_DAILY_OP * pUserDailyOp);
	void UpdateUserDailyOp(std::string strUserId, uint8_t type, int32_t sUnixTime);
	void UpdateRanks();
	void UpdateSiege(int16_t m_sCastleIndex, int16_t m_sMasterKnights, int16_t m_bySiegeType, int16_t m_byWarDay, int16_t m_byWarTime, int16_t m_byWarMinute);
	void UpdateSiegeTax(uint8_t Zone, int16_t ZoneTarrif);

	int8_t NationTransfer(std::string strAccountID);

	~CDBAgent();

private:
	OdbcConnection *m_GameDB, *m_AccountDB;
	std::recursive_mutex m_lock;

	friend class CGameServerDlg;
};

extern CDBAgent g_DBAgent;