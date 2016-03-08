#pragma once

class CNpcTableSet : public OdbcRecordset
{
public:
	CNpcTableSet(OdbcConnection * dbConnection, NpcTableArray * pMap) 
		: OdbcRecordset(dbConnection), m_pMap(pMap), m_bMonster(false)
	{
	}

	virtual tstring GetTableName() { return _T("K_NPC"); }
	virtual tstring GetColumns() { return _T("sSid, strName, sPid, sSize, iWeapon1, iWeapon2, byGroup, byActType, byType, byFamily, byRank, byTitle, iSellingGroup, sLevel, iExp, iLoyalty, iHpPoint, sMpPoint, sAtk, sAc, sHitRate, sEvadeRate, sDamage, sAttackDelay, bySpeed1, bySpeed2, sStandtime, sItem, iMagic1, iMagic2, iMagic3, sFireR, sColdR, sLightningR, sMagicR, sDiseaseR, sPoisonR, sBulk, byAttackRange, bySearchRange, byTracingRange, iMoney, byDirectAttack, byMagicAttack"); }

	virtual bool Fetch()
	{
		CNpcTable *pData = new CNpcTable();
		uint16 sBulk;

		int i = 1;
		_dbCommand->FetchUInt16(i++, pData->m_sSid);
		_dbCommand->FetchString(i++, pData->m_strName);
		_dbCommand->FetchUInt16(i++, pData->m_sPid);
		_dbCommand->FetchUInt16(i++, pData->m_sSize);
		_dbCommand->FetchUInt32(i++, pData->m_iWeapon_1);
		_dbCommand->FetchUInt32(i++, pData->m_iWeapon_2);
		_dbCommand->FetchByte(i++, pData->m_byGroup);
		_dbCommand->FetchByte(i++, pData->m_byActType);
		_dbCommand->FetchByte(i++, pData->m_tNpcType);
		_dbCommand->FetchByte(i++, pData->m_byFamilyType);
		_dbCommand->FetchByte(i++, pData->m_byRank);
		_dbCommand->FetchByte(i++, pData->m_byTitle);
		_dbCommand->FetchUInt32(i++, pData->m_iSellingGroup);
		_dbCommand->FetchUInt16(i++, pData->m_sLevel);
		_dbCommand->FetchUInt32(i++, pData->m_iExp);
		_dbCommand->FetchUInt32(i++, pData->m_iLoyalty);
		_dbCommand->FetchUInt32(i++, pData->m_iMaxHP);
		_dbCommand->FetchUInt16(i++, pData->m_sMaxMP);
		_dbCommand->FetchUInt16(i++, pData->m_sAttack);
		_dbCommand->FetchUInt16(i++, pData->m_sDefense);
		_dbCommand->FetchUInt16(i++, pData->m_sHitRate);
		_dbCommand->FetchUInt16(i++, pData->m_sEvadeRate);
		_dbCommand->FetchUInt16(i++, pData->m_sDamage);
		_dbCommand->FetchUInt16(i++, pData->m_sAttackDelay);
		_dbCommand->FetchByte(i++, pData->m_bySpeed_1);
		_dbCommand->FetchByte(i++, pData->m_bySpeed_2);
		_dbCommand->FetchUInt16(i++, pData->m_sStandTime);
		_dbCommand->FetchUInt16(i++, pData->m_iItem);
		_dbCommand->FetchUInt32(i++, pData->m_iMagic1);
		_dbCommand->FetchUInt32(i++, pData->m_iMagic2);
		_dbCommand->FetchUInt32(i++, pData->m_iMagic3);
		_dbCommand->FetchUInt16(i++, pData->m_byFireR);
		_dbCommand->FetchUInt16(i++, pData->m_byColdR);
		_dbCommand->FetchUInt16(i++, pData->m_byLightningR);
		_dbCommand->FetchUInt16(i++, pData->m_byMagicR);
		_dbCommand->FetchUInt16(i++, pData->m_byDiseaseR);
		_dbCommand->FetchUInt16(i++, pData->m_byPoisonR);
		_dbCommand->FetchUInt16(i++, sBulk);
		_dbCommand->FetchByte(i++, pData->m_byAttackRange);
		_dbCommand->FetchByte(i++, pData->m_bySearchRange);
		_dbCommand->FetchByte(i++, pData->m_byTracingRange);
		_dbCommand->FetchUInt32(i++, pData->m_iMoney);
		_dbCommand->FetchByte(i++, pData->m_byDirectAttack);
		_dbCommand->FetchByte(i++, pData->m_byMagicAttack);
		
		pData->m_byGroupSpecial = 0; // Default value

		// Certain NPCs are defined in the database with a type of 0, which is the monster type.
		// This can potentially cause problems in the future, so fix it now.
		if (!m_bMonster && pData->m_tNpcType == NPC_MONSTER)
			pData->m_tNpcType = NPC_GENERAL;

		pData->m_fBulk =  (float)(((double)sBulk / 100) * ((double)pData->m_sSize / 100));

		if (!m_pMap->PutData(pData->m_sSid, pData))
			delete pData;

		return true;
	}

	NpcTableArray *m_pMap;
	bool m_bMonster;
};

class CMonTableSet : public CNpcTableSet
{
public:
	CMonTableSet(OdbcConnection * dbConnection, NpcTableArray * pMap) 
		: CNpcTableSet(dbConnection, pMap)
	{
		m_bMonster = true;
	}

	virtual tstring GetTableName() { return _T("K_MONSTER"); }
};