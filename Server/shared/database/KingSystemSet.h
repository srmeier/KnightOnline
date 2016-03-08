#pragma once

class CKingSystemSet : public OdbcRecordset
{
public:
	CKingSystemSet(OdbcConnection * dbConnection, KingSystemArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("KING_SYSTEM"); }
	virtual tstring GetColumns() { return _T("byNation, byType, sYear, byMonth, byDay, byHour, byMinute, byImType, sImYear, byImMonth, byImDay, byImHour, byImMinute, byNoahEvent, byNoahEvent_Day, byNoahEvent_Hour, byNoahEvent_Minute, sNoahEvent_Duration, byExpEvent, byExpEvent_Day, byExpEvent_Hour, byExpEvent_Minute, sExpEvent_Duration, nTribute, byTerritoryTariff, nTerritoryTax, nNationalTreasury, strKingName, strImRequestID"); }

	virtual bool Fetch()
	{
		CKingSystem * pData;
		uint8 byNation;
		uint32 i = 1;

		// Pull the nation first so we can use it as an ID.
		_dbCommand->FetchByte(i++, byNation);

		// Hi, I'll take a map for 2 please.
		if (byNation != KARUS && byNation != ELMORAD)
			return true;

		// Do we have an entry for this nation already?
		pData = m_pMap->GetData(byNation);

		// We don't? Create one.
		if (pData == nullptr)
		{
			pData = new CKingSystem();

			// We don't need to check if it exists, because if it did
			// we wouldn't be here...
			m_pMap->PutData(byNation, pData);
		}

		// Let's set the nation then read the rest of the data from the table.
		pData->m_byNation = byNation;

		/* Election */
		_dbCommand->FetchByte(i++, pData->m_byType);
		_dbCommand->FetchUInt16(i++, pData->m_sYear);
		_dbCommand->FetchByte(i++, pData->m_byMonth);
		_dbCommand->FetchByte(i++, pData->m_byDay);
		_dbCommand->FetchByte(i++, pData->m_byHour);
		_dbCommand->FetchByte(i++, pData->m_byMinute);

		/* Impeachment */
		_dbCommand->FetchByte(i++, pData->m_byImType);
		_dbCommand->FetchUInt16(i++, pData->m_sImYear);
		_dbCommand->FetchByte(i++, pData->m_byImMonth);
		_dbCommand->FetchByte(i++, pData->m_byImDay);
		_dbCommand->FetchByte(i++, pData->m_byImHour);
		_dbCommand->FetchByte(i++, pData->m_byImMinute);

		/* King events */

		// Noah
		_dbCommand->FetchByte(i++, pData->m_byNoahEvent);
		_dbCommand->FetchByte(i++, pData->m_byNoahEvent_Day);
		_dbCommand->FetchByte(i++, pData->m_byNoahEvent_Hour);
		_dbCommand->FetchByte(i++, pData->m_byNoahEvent_Minute);
		_dbCommand->FetchUInt16(i++, pData->m_sNoahEvent_Duration);

		// Experience
		_dbCommand->FetchByte(i++, pData->m_byExpEvent);
		_dbCommand->FetchByte(i++, pData->m_byExpEvent_Day);
		_dbCommand->FetchByte(i++, pData->m_byExpEvent_Hour);
		_dbCommand->FetchByte(i++, pData->m_byExpEvent_Minute);
		_dbCommand->FetchUInt16(i++, pData->m_sExpEvent_Duration);

		/* Money, money, money. */
		_dbCommand->FetchUInt32(i++, pData->m_nTribute);
		_dbCommand->FetchByte(i++, pData->m_nTerritoryTariff);
		_dbCommand->FetchUInt32(i++, pData->m_nTerritoryTax);
		_dbCommand->FetchUInt32(i++, pData->m_nNationalTreasury);

		C3DMap * pMap = g_pMain->GetZoneByID(pData->m_byNation);
		if (pMap != nullptr)
			pMap->SetTariff(pData->m_nTerritoryTariff);

		/* Names are so hard to remember. */
		_dbCommand->FetchString(i++, pData->m_strKingName);
		_dbCommand->FetchString(i++, pData->m_strImRequestID);

		pData->ResetElectionLists();

		return true;
	}

	KingSystemArray * m_pMap;
};