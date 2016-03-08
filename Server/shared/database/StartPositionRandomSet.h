 #pragma once

class CStartPositionRandomSet : public OdbcRecordset
{
public:
	CStartPositionRandomSet(OdbcConnection * dbConnection, StartPositionRandomArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("START_POSITION_RANDOM"); }
	virtual tstring GetColumns() { return _T("ZoneID, PosX, PosZ, Radius"); }

	virtual bool Fetch()
	{
		_START_POSITION_RANDOM * pData = new _START_POSITION_RANDOM;

		int i = 1;
		_dbCommand->FetchByte(i++, pData->ZoneID);
		_dbCommand->FetchUInt16(i++, pData->PosX);
		_dbCommand->FetchUInt16(i++, pData->PosZ);
		_dbCommand->FetchByte(i++, pData->Radius);

		pData->sIndex = m_pMap->GetSize();

		if (!m_pMap->PutData(pData->sIndex, pData))
			delete pData;

		return true;
	}

	StartPositionRandomArray *m_pMap;
};