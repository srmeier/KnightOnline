#pragma once

class CKnightsAllianceSet : public OdbcRecordset
{
public:
	CKnightsAllianceSet(OdbcConnection * dbConnection, KnightsAllianceArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("KNIGHTS_ALLIANCE"); }
	virtual tstring GetColumns() { return _T("sMainAllianceKnights, sSubAllianceKnights, sMercenaryClan_1, sMercenaryClan_2"); }

	virtual bool Fetch()
	{
		_KNIGHTS_ALLIANCE *pData = new _KNIGHTS_ALLIANCE;

		_dbCommand->FetchUInt16(1, pData->sMainAllianceKnights);
		_dbCommand->FetchUInt16(2, pData->sSubAllianceKnights);
		_dbCommand->FetchUInt16(3, pData->sMercenaryClan_1);
		_dbCommand->FetchUInt16(4, pData->sMercenaryClan_2);

		CKnights * pKnights = g_pMain->GetClanPtr(pData->sMainAllianceKnights);
		if (pKnights == nullptr
			|| !m_pMap->PutData(pData->sMainAllianceKnights, pData))
			delete pData;

		return true;
	}

	KnightsAllianceArray *m_pMap;
};