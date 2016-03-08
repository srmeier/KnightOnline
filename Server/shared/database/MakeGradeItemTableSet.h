#pragma once

class CMakeGradeItemTableSet : public OdbcRecordset
{
public:
	CMakeGradeItemTableSet(OdbcConnection * dbConnection, MakeGradeItemTableArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap) {}

	virtual tstring GetTableName() { return _T("MAKE_ITEM_GRADECODE"); }
	virtual tstring GetColumns() { return _T("byItemIndex, byGrade_1, byGrade_2, byGrade_3, byGrade_4, byGrade_5, byGrade_6, byGrade_7, byGrade_8, byGrade_9"); }

	virtual bool Fetch()
	{
		_MAKE_ITEM_GRADE_CODE *pData = new _MAKE_ITEM_GRADE_CODE;

		_dbCommand->FetchByte(1, pData->byItemIndex);

		for (int i = 1; i <= 9; i++)
			_dbCommand->FetchUInt16(i+1, pData->sGrade[i-1]);

		if (!m_pMap->PutData(pData->byItemIndex, pData))
			delete pData;

		return true;
	}

	MakeGradeItemTableArray *m_pMap;
};