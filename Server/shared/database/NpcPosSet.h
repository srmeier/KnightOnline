#pragma once

class CNpcPosSet : public OdbcRecordset
{
public:
	CNpcPosSet(OdbcConnection * dbConnection, void * dummy) 
		: OdbcRecordset(dbConnection) {}

	virtual tstring GetTableName() { return _T("K_NPCPOS"); }
	virtual tstring GetColumns() { return _T("ZoneID, NpcID, ActType, RegenType, DungeonFamily, SpecialType, TrapNumber, LeftX, TopZ, RightX, BottomZ, LimitMinX, LimitMinZ, LimitMaxX, LimitMaxZ, NumNPC, RegTime, byDirection, DotCnt, path"); }

	virtual bool Fetch() { return g_pMain->LoadSpawnCallback(_dbCommand); }
};