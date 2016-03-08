#pragma once

class CLevelUpTableSet : public OdbcRecordset
{
public:
	CLevelUpTableSet(OdbcConnection * dbConnection, LevelUpArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("LEVEL_UP"); }
	virtual tstring GetColumns() { return _T("[Level], [Exp]"); }

	virtual bool Fetch()
	{
		std::pair<uint8, int64> pData;

		_dbCommand->FetchByte(1, pData.first);
		_dbCommand->FetchInt64(2, pData.second);

		m_pMap->insert(pData);

		return true;
	}

	LevelUpArray *m_pMap;
};