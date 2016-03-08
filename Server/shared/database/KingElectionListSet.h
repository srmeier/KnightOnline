#pragma once

class CKingElectionListSet : public OdbcRecordset
{
public:
	CKingElectionListSet(OdbcConnection * dbConnection, KingSystemArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("KING_ELECTION_LIST"); }
	virtual tstring GetColumns() { return _T("byNation, byType, strName, nKnights, nMoney"); }

	virtual bool Fetch()
	{
		CKingSystem * pData;
		uint8 byNation, byType;

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
		/**
		* byType list:
		* 1 = ???
		* 2 = elected senator to replace King?
		* 3 = ???
		* 4 = candidate for King
		**/

		_dbCommand->FetchByte(2, byType);

		// Only support senators / nominated/elected Kings for now.
		if (byType != 3 && byType != 4)
			return true;

		std::string strUserID;
		_dbCommand->FetchString(3, strUserID);

		Guard lock(pData->m_lock);
		KingElectionList * pList = (byType == 3 ? &pData->m_senatorList : &pData->m_candidateList);

		// Is this user added to the list already?
		KingElectionList::iterator itr = pList->find(strUserID);

		// Nope, let's add them.
		if (itr == pList->end())
		{
			_KING_ELECTION_LIST * pEntry = new _KING_ELECTION_LIST;

			_dbCommand->FetchUInt16(4, pEntry->sKnights);
			_dbCommand->FetchUInt32(5, pEntry->nVotes); // probably not needed as this is all handled in the procs as far as I know

			pList->insert(make_pair(strUserID, pEntry));
		}

		return true;
	}

	KingSystemArray * m_pMap;
};