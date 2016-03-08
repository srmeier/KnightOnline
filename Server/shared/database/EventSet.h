#pragma once

class CEventSet : public OdbcRecordset
{
public:
	CEventSet(OdbcConnection * dbConnection, ZoneArray *pZoneArray) 
		: OdbcRecordset(dbConnection), m_pZoneArray(pZoneArray) {}

	virtual tstring GetTableName() { return _T("EVENT"); }
	virtual tstring GetColumns() { return _T("ZoneNum, EventNum, Type, Cond1, Cond2, Cond3, Cond4, Cond5, Exec1, Exec2, Exec3, Exec4, Exec5"); }

	virtual bool Fetch()
	{
		CGameEvent *pData = new CGameEvent();
		uint8 bZone;
		int i = 1;

		_dbCommand->FetchByte(i++, bZone);
		_dbCommand->FetchUInt16(i++, pData->m_sIndex);
		_dbCommand->FetchByte(i++, pData->m_bType);

		// TODO: Get rid of this (need to tweak the database to just use int fields)
		for (int j = 0; j < 5; j++)
		{
			char tmp[16];
			_dbCommand->FetchString(i++, tmp, sizeof(tmp));
			pData->m_iCond[j] = atoi(tmp);
		}

		for (int j = 0; j < 5; j++)
		{
			char tmp[16];
			_dbCommand->FetchString(i++, tmp, sizeof(tmp));
			pData->m_iExec[j] = atoi(tmp);
		}

		C3DMap *pZone = m_pZoneArray->GetData(bZone);
		if (pZone == nullptr
			|| !pZone->m_EventArray.PutData(pData->m_sIndex, pData))
			delete pData;

		return true;
	}

	ZoneArray * m_pZoneArray;
};