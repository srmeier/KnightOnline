#pragma once

class CBattleSet : public OdbcRecordset
{
public:
	CBattleSet(OdbcConnection * dbConnection, uint8 * byOldVictory) 
		: OdbcRecordset(dbConnection), m_byOldVictory(byOldVictory) {}

	virtual tstring GetTableName() { return _T("BATTLE"); }
	virtual tstring GetColumns() { return _T("byNation"); }

	virtual bool Fetch() 
	{
		_dbCommand->FetchByte(1, *m_byOldVictory); 
		return true;
	}

	uint8 *m_byOldVictory;
};