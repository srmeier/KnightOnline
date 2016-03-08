#pragma once

class CKingCandidacyNoticeBoardSet : public OdbcRecordset
{
public:
	CKingCandidacyNoticeBoardSet(OdbcConnection * dbConnection, KingSystemArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("KING_CANDIDACY_NOTICE_BOARD"); }
	virtual tstring GetColumns() { return _T("byNation, strUserID, strNotice"); }

	virtual bool Fetch()
	{
		CKingSystem * pData;
		uint8 byNation;

		// Pull the nation first so we can use it as an ID.
		_dbCommand->FetchByte(1, byNation);

		// Hi, I'll take a map for 2 please.
		if (byNation != KARUS && byNation != ELMORAD)
			return false;

		// Do we have an entry for this nation already?
		pData = m_pMap->GetData(byNation);

		// We don't? Nothing to do here.
		if (pData == nullptr)
			return true;

		std::string strUserID;
		char strNotice[1024];	// strNotice is always a 1024 byte binary field 
		// which we implicitly create a variable sized std::string from to save memory.

		_dbCommand->FetchString(2, strUserID);
		_dbCommand->FetchBinary(3, strNotice, sizeof(strNotice));

		// Is this user added to the map already?
		KingCandidacyNoticeBoardMap::iterator itr = pData->m_noticeBoardMap.find(strUserID);

		// Nope, let's add them.
		if (itr == pData->m_noticeBoardMap.end())
			pData->m_noticeBoardMap.insert(make_pair(strUserID, strNotice));
		// They do, replace the notice board entry (no need to clear for reloads).
		else
			itr->second = strNotice;

		return true;
	}

	KingSystemArray * m_pMap;
};