#pragma once

class CKnightsSet : public OdbcRecordset
{
public:
	CKnightsSet(OdbcConnection * dbConnection, KnightsArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("KNIGHTS"); }
	virtual tstring GetColumns() {
		return _T(
			"IDNum, "\
			"Flag, "\
			"Nation, "\
			"Ranking, "\
			"IDName, "\
			"Members, "\
			"Chief, "\
			"ViceChief_1, "\
			"ViceChief_2, "\
			"ViceChief_3, "\
			"Gold, "\
			"Domination, "\
			"Points, "\
			"Mark, "\
			"sMarkVersion, "\
			"sMarkLen, "\
			"sCape, "\
			"sAllianceKnights, "\
			"bySiegeFlag, "\
			"nLose, "\
			"nVictory"
		);
	}

	virtual bool Fetch()
	{
		CKnights* pData = new CKnights();

		_dbCommand->FetchUInt16(1, pData->m_sIndex);
		_dbCommand->FetchByte(2, pData->m_byFlag);
		_dbCommand->FetchByte(3, pData->m_byNation);
		_dbCommand->FetchByte(4, pData->m_byRanking);
		_dbCommand->FetchString(5, pData->m_strName);
		_dbCommand->FetchUInt16(6, pData->m_sMembers);
		_dbCommand->FetchString(7, pData->m_strChief);
		_dbCommand->FetchString(8, pData->m_strViceChief_1);
		_dbCommand->FetchString(9, pData->m_strViceChief_2);
		_dbCommand->FetchString(10, pData->m_strViceChief_3);
		_dbCommand->FetchUInt64(11, pData->m_nMoney);
		_dbCommand->FetchUInt16(12, pData->m_sDomination);
		_dbCommand->FetchUInt32(13, (uint32_t&) pData->m_nPoints);
		_dbCommand->FetchBinary(14, pData->m_Image, sizeof(pData->m_Image));
		_dbCommand->FetchUInt16(15, pData->m_sMarkVersion);
		_dbCommand->FetchUInt16(16, pData->m_sMarkLen);
		_dbCommand->FetchUInt16(17, pData->m_sCape);
		_dbCommand->FetchUInt16(18, pData->m_sAlliance);
		_dbCommand->FetchByte(19, pData->bySiegeFlag);
		_dbCommand->FetchUInt16(20, pData->nLose);
		_dbCommand->FetchUInt16(21, pData->nVictory);

		pData->m_byGrade = g_pMain->GetKnightsGrade(pData->m_nPoints);

		if (!m_pMap->PutData(pData->m_sIndex, pData))
			delete pData;

		return true;
	}

	KnightsArray *m_pMap;
};