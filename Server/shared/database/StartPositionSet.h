#pragma once

class CStartPositionSet : public OdbcRecordset
{
public:
	CStartPositionSet(OdbcConnection * dbConnection, StartPositionArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("START_POSITION"); }
	virtual tstring GetColumns() { return _T("ZoneID, sKarusX, sKarusZ, sElmoradX, sElmoradZ, sKarusGateX, sKarusGateZ, sElmoGateX, sElmoGateZ, bRangeX, bRangeZ"); }

	virtual bool Fetch()
	{
		_START_POSITION *pData = new _START_POSITION;

		_dbCommand->FetchUInt16(1, pData->ZoneID);
		_dbCommand->FetchUInt16(2, pData->sKarusX);
		_dbCommand->FetchUInt16(3, pData->sKarusZ);
		_dbCommand->FetchUInt16(4, pData->sElmoradX);
		_dbCommand->FetchUInt16(5, pData->sElmoradZ);
		_dbCommand->FetchUInt16(6, pData->sKarusGateX);
		_dbCommand->FetchUInt16(7, pData->sKarusGateZ);
		_dbCommand->FetchUInt16(8, pData->sElmoradGateX);
		_dbCommand->FetchUInt16(9, pData->sElmoradGateZ);
		_dbCommand->FetchByte (10, pData->bRangeX);
		_dbCommand->FetchByte (11, pData->bRangeZ);

		if (!m_pMap->PutData(pData->ZoneID, pData))
			delete pData;

		return true;
	}

	StartPositionArray *m_pMap;
};