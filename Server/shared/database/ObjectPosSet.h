#pragma once

class CObjectPosSet : public OdbcRecordset
{
public:
	CObjectPosSet(OdbcConnection * dbConnection, ObjectEventArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("K_OBJECTPOS"); }
	virtual tstring GetColumns() { return _T("ZoneID, Belong, sIndex, Type, ControlNPCID, Status, PosX, PosY, PosZ, byLife"); }

	virtual bool Fetch()
	{
		_OBJECT_EVENT *pData = new _OBJECT_EVENT;

		int i = 1;
		_dbCommand->FetchUInt16(i++, pData->sZoneID);
		_dbCommand->FetchInt32(i++, pData->sBelong);
		_dbCommand->FetchInt16(i++, pData->sIndex);
		_dbCommand->FetchInt16(i++, pData->sType);
		_dbCommand->FetchInt16(i++, pData->sControlNpcID);
		_dbCommand->FetchInt16(i++, pData->sStatus);
		_dbCommand->FetchSingle(i++, pData->fPosX);
		_dbCommand->FetchSingle(i++, pData->fPosY);
		_dbCommand->FetchSingle(i++, pData->fPosZ);
		_dbCommand->FetchByte(i++, pData->byLife);

		pData->nIndex = m_pMap->GetSize();

		if (!m_pMap->PutData(pData->nIndex, pData))
			delete pData;

		return true;
	}

	ObjectEventArray *m_pMap;
};