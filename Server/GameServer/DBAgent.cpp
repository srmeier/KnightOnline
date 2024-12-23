#include "stdafx.h"
#include "../shared/database/OdbcConnection.h"
#include "KnightsManager.h"
#include "DBAgent.h"
#include "../shared/DateTime.h"

CDBAgent g_DBAgent;

CDBAgent::CDBAgent()
{
	m_GameDB = new OdbcConnection();
	m_AccountDB = new OdbcConnection();
}

CDBAgent::~CDBAgent()
{
	delete m_GameDB;
	delete m_AccountDB;
}

bool CDBAgent::Startup(
	bool bMarsEnabled,
	const tstring& strAccountDSN,
	const tstring& strAccountUID,
	const tstring& strAccountPWD,
	const tstring& strGameDSN,
	const tstring& strGameUID,
	const tstring& strGamePWD)
{
	if (!Connect(
		bMarsEnabled,
		strAccountDSN,
		strAccountUID,
		strAccountPWD,
		strGameDSN,
		strGameUID,
		strGamePWD))
	{
		// we should probably be a little more specific (i.e. *which* database server)
		printf(_T("ERROR: Failed to connect to the database server."));
		return false;
	}

	DatabaseThread::Startup();

	return true;
}

bool CDBAgent::Connect(
	bool bMarsEnabled,
	const tstring& strAccountDSN,
	const tstring& strAccountUID,
	const tstring& strAccountPWD,
	const tstring& strGameDSN,
	const tstring& strGameUID,
	const tstring& strGamePWD)
{
	if (!m_AccountDB->Connect(
		strAccountDSN,
		strAccountUID,
		strAccountPWD,
		bMarsEnabled))
	{
		ReportSQLError(m_AccountDB->GetError());
		return false;
	}

	if (!m_GameDB->Connect(
		strGameDSN,
		strGameUID,
		strGamePWD,
		bMarsEnabled))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	return true;
}


void CDBAgent::ReportSQLError(
	OdbcError* pError)
{
	if (pError == nullptr)
		return;

	DateTime time;

	// This is *very* temporary.
	std::string errorMessage = string_format(_T("[ ODBC Error - %d.%d.%d %d:%d:%d ] ] Source: %s Error: %s Description: %s\n"),
		time.GetDay(),time.GetMonth(),time.GetYear(),time.GetHour(),time.GetMinute(),time.GetSecond(),
		pError->Source.c_str(), pError->ExtendedErrorMessage.c_str(), pError->ErrorMessage.c_str());

	Guard lock(m_lock);
	FILE *fp = fopen("./Logs/GameServer.log", "a");
	if (fp != nullptr)
	{
		fwrite(errorMessage.c_str(), errorMessage.length(), 1, fp);
		fclose(fp);
	}

	TRACE("Database error: %s\n", errorMessage.c_str());
	delete pError;
}

int8_t CDBAgent::AccountLogin(
	const std::string& strAccountID,
	const std::string& strPasswd)
{
	int8_t bRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strPasswd.c_str(), strPasswd.length());
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);

	if (!dbCommand->Execute(_T("{CALL ACCOUNT_LOGIN(?, ?, ?)}")))
		ReportSQLError(m_GameDB->GetError());

	/*
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strPasswd.c_str(), strPasswd.length());

	if (!dbCommand->Execute(_T("{? = CALL GAME_LOGIN(?, ?)}")))
		ReportSQLError(m_GameDB->GetError());
	*/

	return bRet;
}

uint8_t CDBAgent::NationSelect(const std::string& strAccountID, uint8_t bNation)
{
	uint8_t bRet = 0;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());

	if (!dbCommand->Execute(string_format(_T("{CALL NATION_SELECT(?, ?, %d)}"), bNation)))
		ReportSQLError(m_GameDB->GetError());

	return (bRet > 0 ? bNation : 0);
}

/*  @brief  Fetches all(max 3) characters that are on the account
 *
 *  @param  strAccountID    Account ID
 *  @param  strCharID1  Character one
 *  @param  strCharID2  Character two
 *  @param  strCharID3  Character three
 */

bool CDBAgent::GetAllCharID(
	const std::string& strAccountID,
	std::string& strCharID1,
	std::string& strCharID2,
	std::string& strCharID3)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());

	if (!dbCommand->Execute(_T("SELECT strCharID1, strCharID2, strCharID3 FROM ACCOUNT_CHAR WHERE strAccountID = ?")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	dbCommand->FetchString(1, strCharID1);
	dbCommand->FetchString(2, strCharID2);
	dbCommand->FetchString(3, strCharID3);

	return true;
}

void CDBAgent::LoadCharInfo(
	const std::string& strCharID,
	ByteBuffer& result)
{
	uint8_t nHair = 0;
	uint16_t sClass = 0;
	uint8_t bRace = 0, bLevel = 0, bFace = 0, bZone = 0; 
	char strItem[INVENTORY_TOTAL * 8] = {};
	ByteBuffer itemData;

	if (!strCharID.empty())
	{
		std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
		if (dbCommand.get() == nullptr)
			return;

		dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
		
		if (!dbCommand->Execute(_T("SELECT Race, Class, HairColor, [Level], Face, Zone, strItem FROM USERDATA WHERE strUserID = ?")))
			ReportSQLError(m_GameDB->GetError());

		if (dbCommand->hasData())
		{
			dbCommand->FetchByte(1, bRace);
			dbCommand->FetchUInt16(2, sClass);
			dbCommand->FetchByte(3, nHair);
			dbCommand->FetchByte(4, bLevel);
			dbCommand->FetchByte(5, bFace);
			dbCommand->FetchByte(6, bZone);
			dbCommand->FetchBinary(7, strItem, sizeof(strItem));
		}
	}

	itemData.append(strItem, sizeof(strItem));

	result	<< strCharID << bRace << sClass << bLevel << bFace << nHair << bZone;
	for (int i = 0; i < SLOT_MAX; i++) 
	{
		uint32_t nItemID;
		uint16_t sDurability, sCount;
		itemData >> nItemID >> sDurability >> sCount;

#if __VERSION <= 1264
		if (i == HEAD || i == BREAST || i == SHOULDER || i == LEG || i == GLOVE || i == FOOT)
#else
		if (i == HEAD || i == BREAST || i == SHOULDER || i == LEG || i == GLOVE || i == FOOT || i == RIGHTHAND || i == LEFTHAND)
#endif
			result << nItemID << sDurability;
	}
}

int8_t CDBAgent::CreateNewChar(
	const std::string& strAccountID,
	int index,
	const std::string& strCharID,
	uint8_t bRace,
	uint16_t sClass,
	uint8_t nHair,
	uint8_t bFace,
	uint8_t bStr,
	uint8_t bSta,
	uint8_t bDex,
	uint8_t bInt,
	uint8_t bCha)
{
	int8_t bRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());

	if (!dbCommand->Execute(string_format(_T("{CALL CREATE_NEW_CHAR(?, ?, %d, ?, %d, %d, %d, %d, %d, %d, %d, %d, %d)}"), 
		index, bRace, sClass, nHair, bFace, bStr, bSta, bDex, bInt, bCha)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}

int8_t CDBAgent::DeleteChar(
	const std::string& strAccountID,
	int index,
	const std::string& strCharID,
	const std::string& strSocNo)
{
	int8_t bRet = -2; // generic error
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strSocNo.c_str(), strSocNo.length());
	//TODO(onurcanbektas): implement the fuction.
	if (!dbCommand->Execute(string_format(_T("{? = CALL DELETE_CHAR(?, %d, ?, ?)}"), index)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}

void CDBAgent::LoadRentalData(
	const std::string& strAccountID,
	const std::string& strCharID,
	UserRentalMap& rentalData)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	if (!dbCommand->Execute(_T("{CALL LOAD_RENTAL_DATA(?)}")))
	{
		ReportSQLError(m_GameDB->GetError());
		return;
	}

	if (!dbCommand->hasData())
		return;

	do
	{
		_USER_RENTAL_ITEM *pItem = new _USER_RENTAL_ITEM();
		_RENTAL_ITEM *pRentalItem = nullptr;

		dbCommand->FetchString(1, pItem->strUserID);
		if (STRCASECMP(pItem->strUserID.c_str(), strCharID.c_str()) != 0)
		{
			delete pItem;
			continue;
		}

		dbCommand->FetchByte(2, pItem->byRentalType);
		dbCommand->FetchByte(3, pItem->byRegType);
		dbCommand->FetchUInt32(4, pItem->nRentalIndex);
		dbCommand->FetchUInt32(5, pItem->nItemID);
		dbCommand->FetchUInt16(6, pItem->sDurability);
		dbCommand->FetchUInt64(7, pItem->nSerialNum);
		dbCommand->FetchUInt32(8, pItem->nRentalMoney);
		dbCommand->FetchUInt16(9, pItem->sRentalTime);
		dbCommand->FetchInt16(10, pItem->sMinutesRemaining);
		dbCommand->FetchString(11, pItem->szTimeRental, sizeof(pItem->szTimeRental));

		pRentalItem = g_pMain->m_RentalItemArray.GetData(pItem->nRentalIndex);
		if (pRentalItem == nullptr
			|| rentalData.find(pItem->nSerialNum) != rentalData.end())
			delete pItem;
		else
			rentalData.insert(std::make_pair(pItem->nSerialNum, pItem));

	} while (dbCommand->MoveNext());
}

bool CDBAgent::LoadUserData(
	const std::string& strAccountID,
	const std::string& strCharID,
	CUser* pUser)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (pUser == nullptr 
		|| pUser->m_bLogout
		|| !pUser->GetName().empty()
		|| strCharID.length() > MAX_ID_SIZE
		/*|| pUser->m_dwTime != 0*/)
		return false;

	int8_t bRet = -1;
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);

	if (!dbCommand->Execute(_T("{CALL LOAD_USER_DATA(?, ?, ?)}")))
		ReportSQLError(m_GameDB->GetError());

	if (!dbCommand->hasData())
		return false;

	char	strItem[INVENTORY_TOTAL * 8] = {0}, strSerial[INVENTORY_TOTAL * 8] = {0},
		strQuest[QUEST_ARRAY_SIZE]; //, strItemTime[INVENTORY_TOTAL * 8];

	uint16_t sQuestCount = 0;

	int field = 1;
	dbCommand->FetchByte(field++, pUser->m_bNation);
	dbCommand->FetchByte(field++, pUser->m_bRace);
	dbCommand->FetchUInt16(field++, pUser->m_sClass);
	dbCommand->FetchByte(field++, pUser->m_byHair);
	dbCommand->FetchByte(field++, pUser->m_bRank);
	dbCommand->FetchByte(field++, pUser->m_bTitle);
	dbCommand->FetchByte(field++, pUser->m_bLevel);
	dbCommand->FetchInt64(field++, pUser->m_iExp);
	dbCommand->FetchUInt32(field++, pUser->m_iLoyalty);
	dbCommand->FetchByte(field++, pUser->m_bFace);
	dbCommand->FetchByte(field++, pUser->m_bCity);
	dbCommand->FetchInt16(field++, pUser->m_bKnights);
	dbCommand->FetchByte(field++, pUser->m_bFame);
	dbCommand->FetchInt16(field++, pUser->m_sHp);
	dbCommand->FetchInt16(field++, pUser->m_sMp);
	dbCommand->FetchInt16(field++, pUser->m_sSp);
	dbCommand->FetchByte(field++, pUser->m_bStats[STAT_STR]);
	dbCommand->FetchByte(field++, pUser->m_bStats[STAT_STA]);
	dbCommand->FetchByte(field++, pUser->m_bStats[STAT_DEX]);
	dbCommand->FetchByte(field++, pUser->m_bStats[STAT_INT]);
	dbCommand->FetchByte(field++, pUser->m_bStats[STAT_CHA]);
	dbCommand->FetchByte(field++, pUser->m_bAuthority);
	dbCommand->FetchInt16(field++, pUser->m_sPoints);
	dbCommand->FetchUInt32(field++, pUser->m_iGold);
	dbCommand->FetchByte(field++, pUser->m_bZone);
	dbCommand->FetchInt16(field++, pUser->m_sBind);
	pUser->m_curx = (float)(dbCommand->FetchInt32(field++) / 100.0f);
	pUser->m_curz = (float)(dbCommand->FetchInt32(field++) / 100.0f);
	pUser->m_cury = (float)(dbCommand->FetchInt32(field++) / 100.0f);
	pUser->m_oldx = pUser->m_curx;
	pUser->m_oldy = pUser->m_cury;
	pUser->m_oldz = pUser->m_curz;
	pUser->m_dwTime = dbCommand->FetchUInt32(field++) + 1;
	dbCommand->FetchString(field++, (char *)pUser->m_bstrSkill, sizeof(pUser->m_bstrSkill));
	dbCommand->FetchBinary(field++, strItem, sizeof(strItem));
	dbCommand->FetchBinary(field++, strSerial, sizeof(strSerial));
	dbCommand->FetchUInt16(field++, sQuestCount);
	dbCommand->FetchBinary(field++, strQuest, sizeof(strQuest));
	dbCommand->FetchUInt32(field++, pUser->m_iMannerPoint);
	dbCommand->FetchUInt32(field++, pUser->m_iLoyaltyMonthly);

	pUser->m_strUserID = strCharID;
	pUser->m_lastSaveTime = UNIXTIME;

	// Convert the old quest storage format to the new one.
	pUser->m_questMap.clear();
	if (sQuestCount > QUEST_LIMIT)
		sQuestCount = QUEST_LIMIT;

	for (int i = 0, index = 0; i < sQuestCount; i++, index += 3)
	{
		uint16_t	sQuestID	= *(uint16_t *)(strQuest + index);
		uint8_t	bQuestState	= *(uint8_t  *)(strQuest + index + 2);
		pUser->m_questMap.insert(std::make_pair(sQuestID, bQuestState));
	}

	ByteBuffer itemBuffer, serialBuffer, itemTimeBuffer;
	itemBuffer.append(strItem, sizeof(strItem));
	serialBuffer.append(strSerial, sizeof(strSerial));

	memset(pUser->m_sItemArray, 0x00, sizeof(pUser->m_sItemArray));

	UserRentalMap rentalData;

	// Until this statement is cleaned up, 
	// no other statements can be processed.
	delete dbCommand.release();

	LoadRentalData(strAccountID, strCharID, rentalData);

	for (int i = 0; i < INVENTORY_TOTAL; i++)
	{ 
		uint64_t nSerialNum;
		uint32_t nItemID;
		int16_t sDurability, sCount;
		uint32_t nItemTime;

		itemBuffer >> nItemID >> sDurability >> sCount;
		serialBuffer >> nSerialNum;
		itemTimeBuffer >> nItemTime;

		_ITEM_TABLE *pTable = g_pMain->GetItemPtr(nItemID);
		if (pTable == nullptr || sCount <= 0)
			continue;

		if (!pTable->m_bCountable && sCount > 1)
			sCount = 1;
		else if (sCount > ITEMCOUNT_MAX)
			sCount = ITEMCOUNT_MAX;

		if (nSerialNum == 0)
			nSerialNum = g_pMain->GenerateItemSerial();

		_ITEM_DATA *pItem = pUser->GetItem(i);
		pItem->nNum = nItemID;
		pItem->sDuration = pTable->isAccessory() ? pTable->m_sDuration : sDurability;
		pItem->sCount = sCount;
		pItem->nSerialNum = nSerialNum;
		pItem->nExpirationTime = nItemTime;

		// If the serial was found in the rental data, mark as rented.
		UserRentalMap::iterator itr = rentalData.find(nSerialNum);
		if (itr != rentalData.end())
		{
			pItem->bFlag = ITEM_FLAG_RENTED;
			pItem->sRemainingRentalTime = itr->second->sMinutesRemaining;
		}

		g_pMain->AddUserItem(nItemID, nSerialNum);

		if (g_pMain->IsDuplicateItem(nItemID,nSerialNum))
			pItem->bFlag = ITEM_FLAG_DUPLICATE;
	}

	// Clean up the rental data
	foreach (itr, rentalData)
		delete itr->second;
	rentalData.clear();

	// Starter items. This needs fixing eventually.
	if (pUser->GetLevel() == 1 && pUser->m_iExp == 0) // going back to their initial bugginess
	{
		uint32_t nItemID = 0;
		uint16_t sDurability = 0;

		switch (pUser->m_sClass)
		{
		case 101:
			nItemID = 120010000;
			sDurability = 5000;
			break;
		case 102:
			nItemID = 110010000;
			sDurability = 4000;
			break;
		case 103:
			nItemID = 180010000;
			sDurability = 5000;
			break;
		case 104:
			nItemID = 190010000;
			sDurability = 10000;
			break;
		case 201:
			nItemID = 120050000;
			sDurability = 5000;
			break;
		case 202:
			nItemID = 110050000;
			sDurability = 4000;
			break;
		case 203:
			nItemID = 180050000;
			sDurability = 5000;
			break;
		case 204:
			nItemID = 190050000;
			sDurability = 10000;
			break;
		}

		if (nItemID && !pUser->CheckExistItem(nItemID, 1))
		{
			int slot = pUser->GetEmptySlot();
			if (slot < 0)
				return true;

			_ITEM_DATA * pItem = pUser->GetItem(slot);
			pItem->nNum = nItemID;
			pItem->sDuration = sDurability;
			pItem->sCount = 1;
			pItem->nSerialNum = g_pMain->GenerateItemSerial();
		}
	}

	return true;
}

bool CDBAgent::LoadWarehouseData(
	const std::string& strAccountID,
	CUser* pUser)
{
	char strItem[WAREHOUSE_MAX * 8] = {}, strSerial[WAREHOUSE_MAX * 8] = {};

	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (pUser == nullptr 
		|| pUser->m_bLogout)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());

	if (!dbCommand->Execute(_T("SELECT nMoney, WarehouseData, strSerial FROM WAREHOUSE WHERE strAccountID = ?")))
		ReportSQLError(m_GameDB->GetError());

	if (!dbCommand->hasData())
		return false;

	memset(strItem, 0x00, sizeof(strItem));
	memset(strSerial, 0x00, sizeof(strSerial));

	dbCommand->FetchUInt32(1, pUser->m_iBank);
	dbCommand->FetchBinary(2, strItem, sizeof(strItem));
	dbCommand->FetchBinary(3, strSerial, sizeof(strSerial));

	ByteBuffer itemBuffer, serialBuffer;
	itemBuffer.append(strItem, sizeof(strItem));
	serialBuffer.append(strSerial, sizeof(strSerial));

	memset(pUser->m_sWarehouseArray, 0x00, sizeof(pUser->m_sWarehouseArray));

	for (int i = 0; i < WAREHOUSE_MAX; i++) 
	{
		uint64_t nSerialNum;
		uint32_t nItemID;
		int16_t sDurability, sCount;

		itemBuffer >> nItemID >> sDurability >> sCount;
		serialBuffer >> nSerialNum;

		_ITEM_TABLE *pTable = g_pMain->GetItemPtr(nItemID);
		if (pTable == nullptr || sCount <= 0)
			continue;

		if (!pTable->m_bCountable && sCount > 1)
			sCount = 1;
		else if (sCount > ITEMCOUNT_MAX)
			sCount = ITEMCOUNT_MAX;

		pUser->m_sWarehouseArray[i].nNum = nItemID;
		pUser->m_sWarehouseArray[i].sDuration = sDurability;
		pUser->m_sWarehouseArray[i].sCount = sCount;
		pUser->m_sWarehouseArray[i].nSerialNum = nSerialNum;
		pUser->m_sWarehouseArray[i].nExpirationTime = 0;

		g_pMain->AddUserItem(nItemID, nSerialNum);

		if (g_pMain->IsDuplicateItem(nItemID,nSerialNum))
			pUser->m_sWarehouseArray[i].bFlag = ITEM_FLAG_DUPLICATE;
	}

	return true;
}

bool CDBAgent::LoadPremiumServiceUser(
	const std::string& strAccountID,
	CUser* pUser)
{
	if (pUser == nullptr)
		return false;

	std::unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &pUser->m_bPremiumType);
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &pUser->m_sPremiumTime);

	if (!dbCommand->Execute(_T("{CALL LOAD_PREMIUM_SERVICE_USER(?, ?, ?)}")))
		ReportSQLError(m_AccountDB->GetError());

	// this is hardcoded because we don't really care about the other mode
	if (pUser->m_bPremiumType != 0 && pUser->m_sPremiumTime != 0)
		pUser->m_bAccountStatus = 1; // normal premium with expiry time

	return true;
}

bool CDBAgent::LoadSavedMagic(
	CUser* pUser)
{
	if (pUser == nullptr)
		return false;

	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->m_strUserID.c_str(), pUser->m_strUserID.length());
	if (!dbCommand->Execute(_T("SELECT "
		"nSkill1, nDuring1, nSkill2, nDuring2, nSkill3, nDuring3, nSkill4, nDuring4, nSkill5, nDuring5, "
		"nSkill6, nDuring6, nSkill7, nDuring7, nSkill8, nDuring8, nSkill9, nDuring9, nSkill10, nDuring10 "
		"FROM USER_SAVED_MAGIC WHERE strCharID = ?")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	Guard lock(pUser->m_savedMagicLock);
	pUser->m_savedMagicMap.clear();
	if (!dbCommand->hasData())
		return true;

	for (int i = 1; i <= 20; i += 2)
	{
		uint32_t nSkillID, nExpiry;
		dbCommand->FetchUInt32(i, nSkillID);
		dbCommand->FetchUInt32(i + 1, nExpiry);

		if (nSkillID != 0)
			pUser->m_savedMagicMap[nSkillID] = (nExpiry + UNIXTIME);
	}

	return true;
}

bool CDBAgent::UpdateSavedMagic(
	CUser* pUser)
{
	if (pUser == nullptr)
		return false;

	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	Guard lock(pUser->m_savedMagicLock);
	uint32_t nSkillID[10] = {0};
	uint32_t tExpiryTime[10] = {0};
	uint32_t i = 0;
	foreach (itr, pUser->m_savedMagicMap)
	{
		nSkillID[i]		= itr->first;
		if (itr->first != 0)
			tExpiryTime[i]	= (uint32_t)(itr->second - UNIXTIME);

		if (++i == 10)
			break;
	}

	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->m_strUserID.c_str(), pUser->m_strUserID.length());
	if (!dbCommand->Execute(
		string_format(_T("{CALL UPDATE_SAVED_MAGIC(?, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)}"), 
		nSkillID[0], tExpiryTime[0],
		nSkillID[1], tExpiryTime[1],
		nSkillID[2], tExpiryTime[2],
		nSkillID[3], tExpiryTime[3],
		nSkillID[4], tExpiryTime[4],
		nSkillID[5], tExpiryTime[5],
		nSkillID[6], tExpiryTime[6],
		nSkillID[7], tExpiryTime[7],
		nSkillID[8], tExpiryTime[8],
		nSkillID[9], tExpiryTime[9])))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	return true;
}

bool CDBAgent::SetLogInInfo(
	const std::string& strAccountID,
	const std::string& strCharID,
	const std::string& strServerIP,
	int16_t sServerNo,
	const std::string& strClientIP,
	uint8_t bInit)
{
	uint8_t result = 0;
	std::unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	/*
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &result);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strServerIP.c_str(), strServerIP.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strClientIP.c_str(), strClientIP.length());
	if (!dbCommand->Execute(string_format(_T("{? = CALL SET_LOGIN_INFO(?, ?, %d, ?, ?, %d)}"), sServerNo, bInit)))
	ReportSQLError(m_AccountDB->GetError());
	return (bool)(result == 0 ? false : true);
	*/

	if (bInit == 0x01)
	{
		if (!dbCommand->Execute(string_format(_T(
			"INSERT INTO CURRENTUSER (strAccountID, strCharID, nServerNo, strServerIP, strClientIP) VALUES (\'%s\',\'%s\',%d,\'%s\',\'%s\')"),
			strAccountID.c_str(), strCharID.c_str(), sServerNo, strServerIP.c_str(), strClientIP.c_str())))
		{
			ReportSQLError(m_AccountDB->GetError());
			return false;
		}
	}
	else if (bInit == 0x02)
	{
		if (!dbCommand->Execute(string_format(_T(
			"UPDATE CURRENTUSER SET nServerNo=%d, strServerIP=\'%s\' WHERE strAccountID = \'%s\'"),
			sServerNo, strServerIP.c_str(), strAccountID.c_str())))
		{
			ReportSQLError(m_AccountDB->GetError());
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CDBAgent::LoadWebItemMall(
	std::vector<_ITEM_DATA>& itemList,
	CUser* pUser)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->m_strUserID.c_str(), pUser->m_strUserID.length());

	// TODO: Add an arg for the free slot count so we only need to pull/delete what we can hold.
	if (!dbCommand->Execute(_T("{CALL LOAD_WEB_ITEMMALL(?)}")))
		ReportSQLError(m_AccountDB->GetError());

	if (dbCommand->hasData())
	{
		do
		{
			_ITEM_DATA item;
			dbCommand->FetchUInt32(2, item.nNum); // 1 is the account name, which we don't need to use unless we're logging	
			dbCommand->FetchUInt16(3, item.sCount);
			itemList.push_back(item);
		}
		while (dbCommand->MoveNext());
	}

	return true;
}

bool CDBAgent::LoadSkillShortcut(
	Packet& result,
	CUser* pUser)
{
	if (pUser == nullptr)
		return false;

	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	uint16_t sCount;
	char strSkillData[260];

	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetName().c_str(), pUser->GetName().length());
	if (!dbCommand->Execute(_T("SELECT nCount, strSkillData FROM USERDATA_SKILLSHORTCUT WHERE strCharID = ?")))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	if (!dbCommand->hasData())
		return false;

	dbCommand->FetchUInt16(1, sCount);
	dbCommand->FetchString(2, strSkillData, sizeof(strSkillData));

	result << sCount;
	for (uint32_t i = 0; i < sCount; i++)
		result << *(uint32_t *)(strSkillData + (i * sizeof(uint32_t)));

	return true;
}

void CDBAgent::SaveSkillShortcut(
	int16_t sCount,
	char* buff,
	CUser* pUser)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetName().c_str(), pUser->GetName().length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, buff, 260);

	if (!dbCommand->Execute(string_format(_T("{CALL SKILLSHORTCUT_SAVE(?, %d, ?)}"), sCount)))
		ReportSQLError(m_GameDB->GetError());
}

bool CDBAgent::SavePremiumServiceUser(
	CUser* pUser)
{
	if (pUser == nullptr)
		return false;

	if (pUser->m_bPremiumType == 0 
		&& pUser->m_sPremiumTime == 0)
		return false;

	std::unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetAccountName().c_str(), pUser->GetAccountName().length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetName().c_str(), pUser->GetName().length());

	if (!dbCommand->Execute(string_format(_T("{CALL SAVE_PREMIUM_SERVICE_USER(?, ?, %d, %d)}"), 
		pUser->m_bPremiumType, pUser->m_sPremiumTime)))
		ReportSQLError(m_GameDB->GetError());

	return true;
}

void CDBAgent::RequestFriendList(
	std::vector<std::string>& friendList,
	CUser* pUser)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetName().c_str(), pUser->GetName().length());
	if (!dbCommand->Execute(_T("SELECT * FROM FRIEND_LIST WHERE strUserID = ?")))
		ReportSQLError(m_GameDB->GetError());

	if (!dbCommand->hasData())
		return;

	std::string strCharID;
	for (int i = 2; i <= 25; i++)
	{
		if (dbCommand->FetchString(i, strCharID)
			&& strCharID.length())
			friendList.push_back(strCharID);
	}
}

FriendAddResult CDBAgent::AddFriend(
	int16_t sid,
	int16_t tid)
{
	CUser* pSrcUser = g_pMain->GetUserPtr(sid);
	if (pSrcUser == nullptr)
		return FRIEND_ADD_ERROR;

	CUser* pTargetUser = g_pMain->GetUserPtr(tid);
	if (pTargetUser == nullptr)
		return FRIEND_ADD_ERROR;

	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return FRIEND_ADD_ERROR;

	int16_t sRet = FRIEND_ADD_ERROR;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, pSrcUser->GetName().c_str(), pSrcUser->GetName().length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, pTargetUser->GetName().c_str(), pTargetUser->GetName().length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL INSERT_FRIEND_LIST(%d, ?, ?)}"), sRet)))
		ReportSQLError(m_GameDB->GetError());

	return (FriendAddResult) sRet;
}

FriendRemoveResult CDBAgent::RemoveFriend(
	const std::string& strCharID,
	CUser* pUser)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return FRIEND_REMOVE_ERROR;

	int16_t sRet = FRIEND_REMOVE_ERROR;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->m_strUserID.c_str(), pUser->m_strUserID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL DELETE_FRIEND_LIST(%d, ?, ?)}"), sRet)))
		ReportSQLError(m_GameDB->GetError());

	return (FriendRemoveResult) sRet;
}

bool CDBAgent::UpdateUser(
	const std::string& strCharID,
	UserUpdateType type,
	CUser* pUser)
{
	if (strCharID != pUser->GetName())
		return false;

	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (type == UPDATE_PACKET_SAVE)
		pUser->m_dwTime++;
	else if (type == UPDATE_LOGOUT || type == UPDATE_ALL_SAVE)
		pUser->m_dwTime = 0;

	char strQuest[QUEST_ARRAY_SIZE] = {};
	int index = 0;
	foreach (itr, pUser->m_questMap)
	{
		*(uint16_t *)(strQuest + index) = itr->first;
		*(uint8_t  *)(strQuest + index + 2) = itr->second;
		index += 3;
	}

	ByteBuffer itemBuffer, serialBuffer;
	for (int i = 0; i < SLOT_MAX+HAVE_MAX; i++)
	{
		_ITEM_DATA *pItem = &pUser->m_sItemArray[i];
		itemBuffer << pItem->nNum << pItem->sDuration << pItem->sCount;
		serialBuffer << pItem->nSerialNum;
	}

	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, (char *)pUser->m_bstrSkill, sizeof(pUser->m_bstrSkill));
	dbCommand->AddParameter(SQL_PARAM_INPUT, (char *)itemBuffer.contents(), itemBuffer.size(), SQL_BINARY);
	dbCommand->AddParameter(SQL_PARAM_INPUT, (char *)serialBuffer.contents(), serialBuffer.size(), SQL_BINARY);
	dbCommand->AddParameter(SQL_PARAM_INPUT, (char *)strQuest, sizeof(strQuest), SQL_BINARY);

	//,? NOTE: 1298
	if (!dbCommand->Execute(string_format(
		_T("{CALL UPDATE_USER_DATA("
		"?, "								// strCharID 
		"%d, %d, %d, %d, %d, "				// nation, race, class, hair, rank
		"%d, %d, " I64FMTD ", %d, %d, "		// title, level, exp, loyalty, face
		"%d, %d, %d, "						// city, knights, fame
		"%d, %d, %d, "						// hp, mp, sp
		"%d, %d, %d, %d, %d, "				// str, sta, dex, int, cha
		"%d, %d, %d, %d, %d, "				// authority, free points, gold, zone, bind
		"%d, %d, %d, %d, %d, "				// x, z, y, dwTime, sQuestCount
		"?, ?, ?, ?, "						// strSkill, strItem, strSerial, strQuest
		"%d, %d)}"),						// manner points, monthly NP, strItemTime
		pUser->m_bNation, pUser->m_bRace, pUser->m_sClass, pUser->m_byHair, pUser->m_bRank,
		pUser->m_bTitle, pUser->m_bLevel, pUser->m_iExp /* temp hack, database needs to support it */, pUser->m_iLoyalty, pUser->m_bFace,
		pUser->m_bCity, pUser->m_bKnights, pUser->m_bFame,
		pUser->m_sHp, pUser->m_sMp, pUser->m_sSp,
		pUser->m_bStats[STAT_STR], pUser->m_bStats[STAT_STA], pUser->m_bStats[STAT_DEX], pUser->m_bStats[STAT_INT], pUser->m_bStats[STAT_CHA],
		pUser->m_bAuthority, pUser->m_sPoints, pUser->m_iGold, pUser->m_bZone, pUser->m_sBind,
		(int) (pUser->m_curx * 100), (int) (pUser->m_curz * 100), (int) (pUser->m_cury * 100), pUser->m_dwTime, (uint16_t) pUser->m_questMap.size(),
		pUser->m_iMannerPoint, pUser->m_iLoyaltyMonthly)))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	pUser->m_lastSaveTime = UNIXTIME;
	return true;
}

bool CDBAgent::UpdateWarehouseData(
	const std::string& strAccountID,
	UserUpdateType type,
	CUser* pUser)
{
	if (strAccountID.empty())
		return false;

	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (type == UPDATE_LOGOUT || type == UPDATE_ALL_SAVE)
		pUser->m_dwTime = 0;

	ByteBuffer itemBuffer, serialBuffer;
	for (int i = 0; i < WAREHOUSE_MAX; i++)
	{
		_ITEM_DATA* pItem = &pUser->m_sWarehouseArray[i];
		itemBuffer << pItem->nNum << pItem->sDuration << pItem->sCount;
		serialBuffer << pItem->nSerialNum;
	}

	dbCommand->AddParameter(SQL_PARAM_INPUT, (char *)itemBuffer.contents(), itemBuffer.size(), SQL_BINARY);
	dbCommand->AddParameter(SQL_PARAM_INPUT, (char *)serialBuffer.contents(), serialBuffer.size(), SQL_BINARY);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());

	if (!dbCommand->Execute(string_format(
		_T("UPDATE WAREHOUSE SET nMoney=%d, dwTime=%d, WarehouseData=?, strSerial=? WHERE strAccountID=?"),
		pUser->m_iBank,
		pUser->m_dwTime)))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	return true;
}

int8_t CDBAgent::CreateAlliance(
	uint8_t byType,
	uint16_t shAlliancIndex,
	uint16_t shKnightsIndex,
	uint8_t byEmptyIndex,
	uint8_t bySiegeFlag)
{
	int8_t bRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;
	
	//TODO(onurcanbektas): implement (Create, Insert, Remove, Destroy) Alliance
	if (!dbCommand->Execute(string_format(
		_T("{CALL UPDATE_KNIGHTS_ALLIANCE(%d, %d, %d, %d, %d)}"),
		byType,
		shAlliancIndex,
		shKnightsIndex,
		byEmptyIndex,
		bySiegeFlag)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}

int8_t CDBAgent::InsertAlliance(
	uint8_t byType,
	uint16_t shAlliancIndex,
	uint16_t shKnightsIndex,
	uint8_t byEmptyIndex,
	uint8_t bySiegeFlag)
{
	int8_t bRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;
	
	if (!dbCommand->Execute(string_format(
		_T("{CALL UPDATE_KNIGHTS_ALLIANCE(%d, %d, %d, %d, %d)}"),
		byType,
		shAlliancIndex,
		shKnightsIndex,
		byEmptyIndex,
		bySiegeFlag)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}

int8_t CDBAgent::RemoveAlliance(
	uint8_t byType,
	uint16_t shAlliancIndex,
	uint16_t shKnightsIndex,
	uint8_t byEmptyIndex,
	uint8_t bySiegeFlag)
{
	int8_t bRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;
	
	if (!dbCommand->Execute(string_format(
		_T("{CALL UPDATE_KNIGHTS_ALLIANCE(%d, %d, %d, %d, %d)}"),
		byType,
		shAlliancIndex,
		shKnightsIndex,
		byEmptyIndex,
		bySiegeFlag)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}

int8_t CDBAgent::DestoryAlliance(
	uint8_t byType,
	uint16_t shAlliancIndex,
	uint16_t shKnightsIndex,
	uint8_t  byEmptyIndex,
	uint8_t bySiegeFlag)
{
	int8_t bRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;
	
	if (!dbCommand->Execute(string_format(
		_T("{CALL UPDATE_KNIGHTS_ALLIANCE(%d, %d, %d, %d, %d)}"), byType, shAlliancIndex, shKnightsIndex, byEmptyIndex, bySiegeFlag)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}

int8_t CDBAgent::CreateKnights(
	uint16_t sClanID,
	uint8_t bNation,
	const std::string& strKnightsName,
	const std::string& strChief,
	uint8_t bFlag)
{
	int8_t bRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return bRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strKnightsName.c_str(), strKnightsName.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strChief.c_str(), strChief.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL CREATE_KNIGHTS (%d, %d, %d, %d, ?, ?)}"), bRet, sClanID, bNation, bFlag)))
		ReportSQLError(m_GameDB->GetError());

	return bRet;
}

int CDBAgent::UpdateKnights(
	uint8_t bType,
	const std::string& strCharID,
	uint16_t sClanID,
	uint8_t bDomination)
{
	int16_t sRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return sRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());

	if (!dbCommand->Execute(string_format(
		_T("{? = CALL UPDATE_KNIGHTS(%d, %d, ?, %d, %d)}"),
		sRet,
		bType + 0x10,
		sClanID,
		bDomination)))
		ReportSQLError(m_GameDB->GetError());

	if (bType == KNIGHTS_VICECHIEF
		&& sRet == 0)
	{
		CKnights * pKnights = g_pMain->GetClanPtr(sClanID);
		if (pKnights != nullptr)
		{
			if (pKnights->m_strViceChief_1.empty())
				pKnights->m_strViceChief_1 = strCharID;
			else if (pKnights->m_strViceChief_2.empty())
				pKnights->m_strViceChief_2 = strCharID;
			else if (pKnights->m_strViceChief_3.empty())
				pKnights->m_strViceChief_3 = strCharID;
		}
	}

	return sRet;
}

int CDBAgent::DeleteKnights(
	uint16_t sClanID)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	
	int16_t sRet = -1;
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	if (!dbCommand->Execute(string_format(_T("{? = CALL DELETE_KNIGHTS (%d, %d)}"), sRet, sClanID)))
		ReportSQLError(m_GameDB->GetError());

	return sRet;
}

uint16_t CDBAgent::LoadKnightsAllMembers(
	uint16_t sClanID,
	Packet& result)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return 0;

	if (!dbCommand->Execute(string_format(_T("SELECT strUserID, Fame, [Level], Class FROM USERDATA WHERE Knights = %d"), sClanID)))
		ReportSQLError(m_GameDB->GetError());

	if (!dbCommand->hasData())
		return 0;

	uint16_t count = 0;
	do
	{
		std::string strCharID;
		uint16_t sClass;
		uint8_t byFame, byLevel;
		dbCommand->FetchString(1, strCharID);
		dbCommand->FetchByte(2, byFame);
		dbCommand->FetchByte(3, byLevel);
		dbCommand->FetchUInt16(4, sClass);

		result
			<< strCharID
			<< uint8_t(byFame)
			<< uint8_t(byLevel)
			<< uint16_t(sClass )
			// check if user's logged in (i.e. grab logged in state)
			<< uint8_t(g_pMain->GetUserPtr(strCharID, TYPE_CHARACTER) == nullptr ? 0 : 1);
		count++;
	}
	while (dbCommand->MoveNext());

	return count;
}

bool CDBAgent::LoadKnightsInfo(
	uint16_t sClanID,
	uint8_t& bNation,
	std::string& strKnightsName,
	uint16_t& sMembers,
	uint32_t& nPoints,
	uint8_t& bRank)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	if (!dbCommand->Execute(string_format(
		_T("SELECT Nation, IDName, Members, Points, Ranking FROM KNIGHTS WHERE IDNum=%d" ), sClanID)))
		ReportSQLError(m_GameDB->GetError());

	if (!dbCommand->hasData())
		return false;

	dbCommand->FetchByte(1, bNation);
	dbCommand->FetchString(2, strKnightsName);
	dbCommand->FetchUInt16(3, sMembers);
	dbCommand->FetchUInt32(4, nPoints);
	dbCommand->FetchByte(5, bRank);

	return true;
}

void CDBAgent::LoadKnightsAllList()
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	const tstring szSQL = _T("SELECT IDNum, Points, Ranking FROM KNIGHTS WHERE Points != 0 ORDER BY Points DESC"); 

	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(szSQL))
		ReportSQLError(m_GameDB->GetError());

	if (!dbCommand->hasData())
		return;

	Packet result(WIZ_KNIGHTS_PROCESS);
	uint8_t bCount = 0;
	size_t offset = 0;

	do
	{
		if (bCount == 0)
		{
			result.Initialize(WIZ_KNIGHTS_PROCESS);
			offset = result.wpos();
			result << uint8_t(0);
		}

		uint32_t nPoints;
		uint16_t sClanID;
		uint8_t bRanking;
		dbCommand->FetchUInt16(1, sClanID);
		dbCommand->FetchUInt32(2, nPoints);
		dbCommand->FetchByte(3, bRanking);

		result << sClanID << nPoints << bRanking;

		// only send 100 clans at a time (no shared memory limit, yay!)
		if (++bCount >= 100)
		{
			// overwrite the count
			result.put(offset, bCount);

			CKnightsManager::RecvKnightsAllList(result);
			bCount = 0;
		}
	}
	while (dbCommand->MoveNext());

	// didn't quite make it in the last batch (if any)? send the remainder.
	if (bCount < 100)
	{
		result.put(offset, bCount);
		CKnightsManager::RecvKnightsAllList(result);
	}
}

bool CDBAgent::UpdateClanSymbol(
	uint16_t sClanID,
	uint16_t sSymbolSize,
	char* clanSymbol)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return false;

	dbCommand->AddParameter(SQL_PARAM_INPUT, clanSymbol, MAX_KNIGHTS_MARK, SQL_BINARY);
	if (!dbCommand->Execute(string_format(
		_T("UPDATE KNIGHTS SET Mark=?, sMarkVersion += 1, sMarkLen=%d WHERE IDNum=%d"),
		sSymbolSize,
		sClanID)))
	{
		ReportSQLError(m_GameDB->GetError());
		return false;
	}

	return true;
}

/**
* @brief	Change authority is logged out character.
*
* @param	strUserID	Character's name.
* @param	byAuthority	New user authority.
*/
void CDBAgent::UpdateUserAuthority(
	const std::string& strUserID,
	uint8_t byAuthority)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strUserID.c_str(), strUserID.length());
	if (!dbCommand->Execute(string_format(_T("UPDATE USERDATA SET Authority = %d WHERE strUserID = ?"), byAuthority)))
		ReportSQLError(m_GameDB->GetError());
}

NameChangeOpcode CDBAgent::UpdateCharacterName(
	const std::string& strAccountID,
	const std::string& strUserID,
	const std::string& strNewUserID)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return NameChangeInvalidName;

	int16_t sRet = 1;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strUserID.c_str(), strUserID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strNewUserID.c_str(), strNewUserID.length());

	if (!dbCommand->Execute(string_format(_T("{? = CALL CHANGE_NEW_ID(%d, ?, ?, ?, 1)}"), g_pMain->m_nServerNo)))
		ReportSQLError(m_GameDB->GetError());

	return NameChangeOpcode(sRet);
}

/**
* @brief	Handles clan cape database updates.
*
* @param	sClanID	Identifier for the clan.
* @param	sCapeID	Identifier for the cape.
*
* @return	true if it succeeds, false if it fails.
*/
void CDBAgent::UpdateCape(
	uint16_t sClanID,
	uint16_t sCapeID)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(string_format(
		_T("UPDATE KNIGHTS SET sCape=%d WHERE IDNum=%d"),
		sCapeID,
		sClanID)))
		ReportSQLError(m_GameDB->GetError());
}

/**
* @brief	Updates the clan grade.
*
* @param	sClanID	Identifier for the clan.
* @param	byFlag 	The clan type (training, promoted, etc).
* @param	sCapeID	Identifier for the cape.
*/
void CDBAgent::UpdateClanGrade(
	uint16_t sClanID,
	uint8_t byFlag,
	uint16_t sCapeID)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(string_format(_T("UPDATE KNIGHTS SET sCape=%d, Flag=%d WHERE IDNum=%d"), 
		sCapeID, byFlag, sClanID)))
		ReportSQLError(m_GameDB->GetError());
}

void CDBAgent::UpdateBattleEvent(
	const std::string& strCharID,
	uint8_t bNation)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	if (!dbCommand->Execute(string_format(
		_T("UPDATE BATTLE SET byNation=%d, strUserName=? WHERE sIndex=%d"),
		bNation,
		g_pMain->m_nServerNo)))
		ReportSQLError(m_GameDB->GetError());
}

void CDBAgent::AccountLogout(
	const std::string& strAccountID)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	int iCode = 0;
	uint8_t bRet = 0;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strAccountID.c_str(), strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &bRet);

	if (!dbCommand->Execute(string_format(
		_T("{CALL ACCOUNT_LOGOUT(?, %d, ?)}"),
		iCode)))
		ReportSQLError(m_AccountDB->GetError());
}

void CDBAgent::UpdateConCurrentUserCount(
	int nServerNo,
	int nZoneNo,
	int nCount)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(string_format(
		_T("UPDATE CONCURRENT SET zone%d_count = %d WHERE serverid = %d"),
		nZoneNo,
		nCount,
		nServerNo)))
		ReportSQLError(m_AccountDB->GetError());
}

/**
* @brief	Updates the election status.
*
* @param	byType  	Election status.
* @param	byNation	Electoral nation.
*/
void CDBAgent::UpdateElectionStatus(
	uint8_t byType,
	uint8_t byNation)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(string_format(
		_T("{CALL KING_UPDATE_ELECTION_STATUS(%d, %d)}"),
		byType,
		byNation)))
		ReportSQLError(m_GameDB->GetError());
}

/**
* @brief	Updates the election list.
*
* @param	byDBType  	Procedure-specific database action.
* 						If 1, insert. If 2, delete.
* @param	byType	  	Flag to specify what the user's in the election list for (election, impeachment, and thereof).
* @param	byNation  	Electoral nation.
* @param	sKnights  	The nominee's clan ID.
* @param	nAmount		Vote count.
* @param	strNominee	The nominee's name.
*/
void CDBAgent::UpdateElectionList(
	uint8_t byDBType,
	uint8_t byType,
	uint8_t byNation,
	uint16_t sKnights,
	uint32_t nAmount,
	const std::string& strNominee,
	CUser* pUser)
{
	int16_t sRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr
		|| pUser == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->m_strAccountID.c_str(), pUser->m_strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetName().c_str(), pUser->GetName().length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strNominee.c_str(), strNominee.length());

	if (!dbCommand->Execute(string_format(
		_T("{? = CALL KING_ELECTION_PROC(?, ?, %d, ?, %d)}"),
		pUser->GetNation(),
		sRet)))
		ReportSQLError(m_GameDB->GetError());

	Packet result(WIZ_KING);
	result
		<< uint8_t(1)
		<< uint8_t(byType)
		<< uint8_t(2)
		<< int16_t(sRet);
	pUser->Send(&result);
}

int16_t CDBAgent::UpdateElectionProc(
	CUser* pUser)
{
	int16_t sRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr || pUser == nullptr)
		return sRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->m_strAccountID.c_str(), pUser->m_strAccountID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetName().c_str(), pUser->GetName().length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, pUser->GetName().c_str(), pUser->GetName().length());

	if (!dbCommand->Execute(string_format(
		_T("{? = CALL KING_ELECTION_PROC(?, ?, %d, ?, 0)}"),
		pUser->GetNation())))
		ReportSQLError(m_GameDB->GetError());

	return sRet;
}

void CDBAgent::SendUDP_ElectionStatus(
	uint8_t byType)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;
	
	if (!dbCommand->Execute(string_format(
		_T("UPDATE KING_SYSTEM SET byType = %d"),
		byType)))
		ReportSQLError(m_GameDB->GetError());
}

/**
* @brief	Nominates/recommends strNominee as King.
*
* @param	strNominator	The nominator.
* @param	strNominee  	The nominee.
* @param	byNation		Their nation.
*
* @return	.
*/
int16_t CDBAgent::UpdateCandidacyRecommend(
	const std::string& strNominator,
	const std::string& strNominee,
	uint8_t byNation)
{
	int16_t sRet = -1;
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return sRet;

	dbCommand->AddParameter(SQL_PARAM_OUTPUT, &sRet);
	dbCommand->AddParameter(SQL_PARAM_INPUT, strNominee.c_str(), strNominee.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strNominator.c_str(), strNominator.length());

	if (!dbCommand->Execute(string_format(
		_T("{? = CALL KING_CANDIDACY_RECOMMEND(?, ?, %d, 0)}"),
		byNation)))
		ReportSQLError(m_GameDB->GetError());

	return sRet;
}

/**
* @brief	Updates the candidacy notice board.
*
* @param	strCharID	Candidate's name.
* @param	byNation 	Candidate's nation.
* @param	strNotice	The notice.
*/
void CDBAgent::UpdateCandidacyNoticeBoard(
	const std::string& strCharID,
	uint8_t byNation,
	const std::string& strNotice)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	// Field is 1024 bytes in the database.
	uint8_t strBinaryNotice[1024] = {0};
	uint16_t sNoticeLen = strNotice.length();

	// This shouldn't happen, but... in the horribly unlikely event it does... we're ready.
	if (sNoticeLen > sizeof(strBinaryNotice))
		sNoticeLen = sizeof(strBinaryNotice);

	memcpy(strBinaryNotice, strNotice.c_str(), sNoticeLen);

	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	dbCommand->AddParameter(SQL_PARAM_INPUT, strBinaryNotice, sizeof(strBinaryNotice));

	if (!dbCommand->Execute(string_format(
		_T("{CALL KING_CANDIDACY_NOTICE_BOARD_PROC(?, %d, %d, ?)}"), 
		sNoticeLen,
		byNation)))
		ReportSQLError(m_GameDB->GetError());
}

void CDBAgent::UpdateNoahOrExpEvent(
	uint8_t byType,
	uint8_t byNation,
	uint8_t byAmount,
	uint8_t byDay,
	uint8_t byHour,
	uint8_t byMinute,
	uint16_t sDuration)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(string_format(
		_T("{CALL KING_UPDATE_NOAH_OR_EXP_EVENT(%d, %d, %d, %d, %d, %d, %d)}"), 
		byType,
		byNation,
		byAmount,
		byDay,
		byHour,
		byMinute,
		sDuration)))
		ReportSQLError(m_GameDB->GetError());
}

void CDBAgent::InsertPrizeEvent(
	uint8_t byType,
	uint8_t byNation,
	uint32_t nCoins,
	const std::string& strCharID)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_INPUT, strCharID.c_str(), strCharID.length());
	if (!dbCommand->Execute(string_format(_T("{CALL KING_INSERT_PRIZE_EVENT(%d, %d, %d, ?)}"), 
		byType,
		byNation,
		nCoins)))
		ReportSQLError(m_GameDB->GetError());
}

void CDBAgent::InsertTaxEvent(
	uint32_t KingNationTax,
	uint8_t Nation,
	uint32_t TerritoryTax)
{	
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (TerritoryTax > 0)
	{
		if (!dbCommand->Execute(string_format(
			_T("UPDATE KING_SYSTEM SET nTerritoryTax -= %d WHERE byNation = %d"),
			TerritoryTax,
			Nation)))
			ReportSQLError(m_GameDB->GetError());
	}
	else
	{
		if (!dbCommand->Execute(string_format(
			_T("UPDATE KING_SYSTEM SET byTerritoryTariff = %d WHERE byNation = %d"),
			KingNationTax,
			Nation)))
			ReportSQLError(m_GameDB->GetError());
	}
}

void CDBAgent::InsertTaxUpEvent(
	uint8_t Nation,
	uint32_t TerritoryTax)
{	
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (Nation == ZONE_ELMORAD
		|| Nation == ZONE_KARUS)
	{
		if (!dbCommand->Execute(string_format(
			_T("UPDATE KING_SYSTEM SET nTerritoryTax += %d WHERE byNation = %d"),
			TerritoryTax,
			Nation)))
			ReportSQLError(m_GameDB->GetError());

		if (!dbCommand->Execute(string_format(
			_T("UPDATE KING_SYSTEM SET nNationalTreasury += %d WHERE byNation = %d"),
			TerritoryTax,
			Nation)))
			ReportSQLError(m_GameDB->GetError());
	}
	else if (Nation == ZONE_MORADON)
	{
		if (!dbCommand->Execute(string_format(
			_T("UPDATE KNIGHTS_SIEGE_WARFARE SET nMoradonTax += %d WHERE sCastleIndex = 1"),
			TerritoryTax)))
			ReportSQLError(m_GameDB->GetError());
	}
	else if (Nation == ZONE_DELOS)
	{
		if (!dbCommand->Execute(string_format(
			_T("UPDATE KNIGHTS_SIEGE_WARFARE SET nDellosTax += %d WHERE sCastleIndex = 1"),
			TerritoryTax)))
			ReportSQLError(m_GameDB->GetError());
	}
}

/**
* @brief	Resets the monthly NP total accumulated in the last month.
*/
void CDBAgent::ResetLoyaltyMonthly()
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(_T("{CALL RESET_LOYALTY_MONTHLY}")))
		ReportSQLError(m_GameDB->GetError());
}

/**
* @brief	Clears the remaining users who were connected to this server
  			from the logged in user list that may still be there as the 
  			result of an improper shutdown.
*/
void CDBAgent::ClearRemainUsers()
{
	_ZONE_SERVERINFO* pInfo = g_pMain->m_ServerArray.GetData(g_pMain->m_nServerNo);
	if (pInfo == nullptr)
		return;

	std::unique_ptr<OdbcCommand> dbCommand(m_AccountDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	dbCommand->AddParameter(SQL_PARAM_INPUT, pInfo->strServerIP.c_str(), pInfo->strServerIP.length());
	if (!dbCommand->Execute(_T("{CALL CLEAR_REMAIN_USERS(?)}")))
		ReportSQLError(m_AccountDB->GetError());
}

void CDBAgent::UpdateRanks()
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(_T("{CALL UPDATE_PERSONAL_RANK}")))
		ReportSQLError(m_GameDB->GetError());
}

void CDBAgent::UpdateSiege(
	int16_t sCastleIndex,
	int16_t sMasterKnights,
	int16_t bySiegeType,
	int16_t byWarDay,
	int16_t byWarTime,
	int16_t byWarMinute)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (!dbCommand->Execute(string_format(
		_T("{CALL UPDATE_SIEGE (%d, %d, %d, %d, %d, %d)}"),
		sCastleIndex,
		sMasterKnights,
		bySiegeType,
		byWarDay,
		byWarTime,
		byWarMinute)))
		ReportSQLError(m_GameDB->GetError());	
}

void CDBAgent::UpdateSiegeTax(
	uint8_t Zone,
	int16_t ZoneTarriff)
{
	std::unique_ptr<OdbcCommand> dbCommand(m_GameDB->CreateCommand());
	if (dbCommand.get() == nullptr)
		return;

	if (Zone == ZONE_DELOS)
	{
		if (!dbCommand->Execute(string_format(
			_T("UPDATE KNIGHTS_SIEGE_WARFARE SET sMoradonTariff = %d"),
			ZoneTarriff)))
			ReportSQLError(m_GameDB->GetError());
	}
	else if (Zone == ZONE_MORADON)
	{
		if (!dbCommand->Execute(string_format(
			_T("UPDATE KNIGHTS_SIEGE_WARFARE SET sDellosTariff = %d"),
			ZoneTarriff)))
			ReportSQLError(m_GameDB->GetError());
	}
	else if (Zone == 0)
	{
		if (!dbCommand->Execute(
			_T("UPDATE KNIGHTS_SIEGE_WARFARE SET nDungeonCharge = 0, nMoradonTax = 0, nDellosTax = 0")))
			ReportSQLError(m_GameDB->GetError());
	}
}
