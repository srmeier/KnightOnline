#include "stdafx.h"

void CUser::WarehouseProcess(Packet & pkt)
{
	Packet result(WIZ_WAREHOUSE);
	uint32_t nItemID, nCount;
	uint16_t reference_pos;
	uint8_t page, bSrcPos, bDstPos;
	CNpc * pNpc = nullptr;
	_ITEM_TABLE * pTable = nullptr;
	_ITEM_DATA * pSrcItem = nullptr, * pDstItem = nullptr;
	uint8_t opcode;
	bool bResult = false;

	if (isDead())
		return;

	if (isTrading())
		goto fail_return;

	pkt >> opcode;
	if (opcode == WAREHOUSE_OPEN)
	{
		result << opcode << uint8_t(1) << GetInnCoins();
		for (int i = 0; i < WAREHOUSE_MAX; i++)
		{
			_ITEM_DATA *pItem = &m_sWarehouseArray[i];

			if(pItem == nullptr)
				continue; 

			result << pItem->nNum
				<< pItem->sDuration
				<< pItem->sCount;
				//<< pItem->bFlag 
				//<< uint32_t(0)
				//<< pItem->nExpirationTime;
		}
		if (isInPKZone())
		{
			if (hasCoins(10000))
				GoldLose(10000);
			else
			{
				opcode = 1;
				goto fail_return;
			}
		}
		Send(&result);
		return;
	}

	pkt /*>> sNpcId*/ >> nItemID >> page  >> bSrcPos >> bDstPos;

	/*
	pNpc = g_pMain->GetNpcPtr(sNpcId);
	if (pNpc == nullptr
		|| pNpc->GetType() != NPC_WAREHOUSE
		|| !isInRange(pNpc, MAX_NPC_RANGE))
		goto fail_return;
	*/

	pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable == nullptr)
		goto fail_return;

	reference_pos = 24 * page;

	switch (opcode)
	{
		// Inventory -> inn
	case WAREHOUSE_INPUT:
		pkt >> nCount;

		// Handle coin input.
		if (nItemID == ITEM_GOLD)
		{
			if (!hasCoins(nCount)
				|| GetInnCoins() + nCount > COIN_MAX)
				goto fail_return;

			m_iBank += nCount;
			m_iGold -= nCount;
			break;
		}

		// Check for invalid slot IDs.
		if (bSrcPos > HAVE_MAX
			|| reference_pos + bDstPos > WAREHOUSE_MAX
			// Cannot be traded, sold or stored (note: don't check the race, as these items CAN be stored).
			|| nItemID >= ITEM_NO_TRADE	
			// Check that the source item we're moving is what the client says it is.
			|| (pSrcItem = GetItem(SLOT_MAX + bSrcPos))->nNum != nItemID
			// Rented items cannot be placed in the inn.
			|| pSrcItem->isRented()
			|| pSrcItem->isDuplicate())
			goto fail_return;

		pDstItem = &m_sWarehouseArray[reference_pos + bDstPos];
		// Forbid users from moving non-stackable items into a slot already occupied by an item.
		if ((!pTable->isStackable() && pDstItem->nNum != 0)
			// Forbid users from moving stackable items into a slot already occupied by a different item.
				|| (pTable->isStackable() 
				&& pDstItem->nNum != 0 // slot in use
				&& pDstItem->nNum != pSrcItem->nNum) // ... by a different item.
				// Ensure users have enough of the specified item to move.
				|| pSrcItem->sCount < nCount)
				goto fail_return;

		pDstItem->nNum = pSrcItem->nNum;
		pDstItem->sDuration = pSrcItem->sDuration;
		pDstItem->sCount += (uint16_t) nCount;
		pSrcItem->sCount -= nCount;
		pDstItem->bFlag = pSrcItem->bFlag;
		pDstItem->sRemainingRentalTime = pSrcItem->sRemainingRentalTime;
		pDstItem->nExpirationTime = pSrcItem->nExpirationTime;

		if (!pTable->isStackable() || nCount == pDstItem->sCount)
			pDstItem->nSerialNum = pSrcItem->nSerialNum;

		if (!pTable->isStackable() && pDstItem->nSerialNum == 0)
			pDstItem->nSerialNum = g_pMain->GenerateItemSerial();

		if (pSrcItem->sCount == 0)
			memset(pSrcItem, 0, sizeof(_ITEM_DATA));

		SetUserAbility(false);
		SendItemWeight();
		break;

		// Inn -> inventory
	case WAREHOUSE_OUTPUT:
		pkt >> nCount;

		if (nItemID == ITEM_GOLD)
		{
			if (!hasInnCoins(nCount)
				|| GetCoins() + nCount > COIN_MAX)
				goto fail_return;

			m_iGold += nCount;
			m_iBank -= nCount;
			break;
		}

		// Ensure we're not being given an invalid slot ID.
		if (reference_pos + bSrcPos > WAREHOUSE_MAX
			|| bDstPos > HAVE_MAX
			// Check that the source item we're moving is what the client says it is.
			|| (pSrcItem = &m_sWarehouseArray[reference_pos + bSrcPos])->nNum != nItemID
			// Does the player have enough room in their inventory?
			|| !CheckWeight(pTable, nItemID, (uint16_t) nCount))
			goto fail_return;

		pDstItem = GetItem(SLOT_MAX + bDstPos);
		// Forbid users from moving non-stackable items into a slot already occupied by an item.
		if ((!pTable->isStackable() && pDstItem->nNum != 0)
			// Forbid users from moving stackable items into a slot already occupied by a different item.
				|| (pTable->isStackable() 
				&& pDstItem->nNum != 0 // slot in use
				&& pDstItem->nNum != pSrcItem->nNum) // ... by a different item.
				// Ensure users have enough of the specified item to move.
				|| pSrcItem->sCount < nCount)
				goto fail_return;

		pDstItem->nNum = pSrcItem->nNum;
		pDstItem->sDuration = pSrcItem->sDuration;
		pDstItem->sCount += (uint16_t) nCount;
		pSrcItem->sCount -= nCount;
		pDstItem->bFlag = pSrcItem->bFlag;
		pDstItem->sRemainingRentalTime = pSrcItem->sRemainingRentalTime;
		pDstItem->nExpirationTime = pSrcItem->nExpirationTime;

		if (!pTable->isStackable() || nCount == pDstItem->sCount)
			pDstItem->nSerialNum = pSrcItem->nSerialNum;

		if (!pTable->isStackable() && pDstItem->nSerialNum == 0)
			pDstItem->nSerialNum = g_pMain->GenerateItemSerial();

		if (pSrcItem->sCount == 0)
			memset(pSrcItem, 0, sizeof(_ITEM_DATA));

		SetUserAbility(false);
		SendItemWeight();
		break;

		// Inn -> inn
	case WAREHOUSE_MOVE:
		// Ensure we're not being given an invalid slot ID.
		if (reference_pos + bSrcPos > WAREHOUSE_MAX
			|| reference_pos + bDstPos > WAREHOUSE_MAX)
			goto fail_return;

		pSrcItem = &m_sWarehouseArray[reference_pos + bSrcPos];
		pDstItem = &m_sWarehouseArray[reference_pos + bDstPos];

		// Check that the source item we're moving is what the client says it is.
		if (pSrcItem->nNum != nItemID
			// You can't move a partial stack in the inn (the whole stack is moved).
				|| pDstItem->nNum != 0)
				goto fail_return;

		memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA));
		memset(pSrcItem, 0, sizeof(_ITEM_DATA));
		break;

		// Inventory -> inventory (using the inn dialog)
	case WAREHOUSE_INVENMOVE:
		// Ensure we're not being given an invalid slot ID.
		if (bSrcPos > HAVE_MAX
			|| bDstPos > HAVE_MAX)
			goto fail_return;

		pSrcItem = GetItem(SLOT_MAX + bSrcPos);
		pDstItem = GetItem(SLOT_MAX + bDstPos);

		// Check that the source item we're moving is what the client says it is.
		if (pSrcItem->nNum != nItemID
			// You can't move a partial stack in the inventory (the whole stack is moved).
				|| pDstItem->nNum != 0)
				goto fail_return;

		memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA));
		memset(pSrcItem, 0, sizeof(_ITEM_DATA));
		break;
	}

	bResult = true;

fail_return: // hmm...
	result << opcode << bResult;
	Send(&result);
}

bool CUser::CheckWeight(uint32_t nItemID, uint32_t sCount)
{
	_ITEM_TABLE * pTable = g_pMain->GetItemPtr(nItemID);

	if(pTable == nullptr)
		return false; 

	return CheckWeight(pTable, nItemID, sCount);
}

bool CUser::CheckWeight(_ITEM_TABLE * pTable, uint32_t nItemID, uint32_t sCount)
{
	return (pTable != nullptr // Make sure the item exists
		// and that the weight doesn't exceed our limit
		&& (m_sItemWeight + (pTable->m_sWeight * sCount)) <= m_sMaxWeight
		// and we have room for the item.
		&& FindSlotForItem(nItemID, sCount) >= 0);
}

bool CUser::CheckExistItem(int itemid, int16_t count /*= 1*/)
{
	// Search for the existance of all items in the player's inventory storage and onwards
	for (int i = 0; i < INVENTORY_TOTAL; i++)
	{
		// This implementation fixes a bug where it ignored the possibility for multiple stacks.
		if (m_sItemArray[i].nNum == itemid
			&& m_sItemArray[i].sCount >= count)
			return true;
	}

	return false;
}

uint32_t CUser::GetItemCount(uint32_t nItemID)
{
	uint32_t result = 0;
	// Search for the existance of all items in the player's inventory storage and onwards
	for (int i = 0; i < INVENTORY_TOTAL; i++)
	{
		// This implementation fixes a bug where it ignored the possibility for multiple stacks.
		if (m_sItemArray[i].nNum == nItemID)
			result += m_sItemArray[i].sCount;
	}

	return result;
}

// Pretend you didn't see me. This really needs to go (just copying official)
bool CUser::CheckExistItemAnd(
	int32_t nItemID1, int32_t sCount1,
	int32_t nItemID2, int32_t sCount2,
	int32_t nItemID3, int32_t sCount3,
	int32_t nItemID4, int32_t sCount4,
	int32_t nItemID5, int32_t sCount5)
{
	if (nItemID1
		&& !CheckExistItem(nItemID1, sCount1))
		if (nItemID1!=900000000 || nItemID1!=900001000 || nItemID1!=900002000 || nItemID1!=900003000 || nItemID1!=900004000 || nItemID1!=900005000 || nItemID1!=900006000 || nItemID1!=900007000 || nItemID1!=900008000 || nItemID1!=900009000 || nItemID1!=900010000 || nItemID1!=900011000)
		return false;

	if (nItemID2
		&& !CheckExistItem(nItemID2, sCount2))
		if (nItemID2!=900000000 || nItemID2!=900001000 || nItemID2!=900002000 || nItemID2!=900003000 || nItemID2!=900004000 || nItemID2!=900005000 || nItemID2!=900006000 || nItemID2!=900007000 || nItemID2!=900008000 || nItemID2!=900009000 || nItemID2!=900010000 || nItemID2!=900011000)
		return false;

	if (nItemID3
		&& !CheckExistItem(nItemID3, sCount3))
		if (nItemID3!=900000000 || nItemID3!=900001000 || nItemID3!=900002000 || nItemID3!=900003000 || nItemID3!=900004000 || nItemID3!=900005000 || nItemID3!=900006000 || nItemID3!=900007000 || nItemID3!=900008000 || nItemID3!=900009000 || nItemID3!=900010000 || nItemID3!=900011000)
		return false;

	if (nItemID4
		&& !CheckExistItem(nItemID4, sCount4))
		if (nItemID4!=900000000 || nItemID4!=900001000 || nItemID4!=900002000 || nItemID4!=900003000 || nItemID4!=900004000 || nItemID4!=900005000 || nItemID4!=900006000 || nItemID4!=900007000 || nItemID4!=900008000 || nItemID4!=900009000 || nItemID4!=900010000 || nItemID4!=900011000)
		return false;

	if (nItemID5
		&& !CheckExistItem(nItemID5, sCount5))
		if (nItemID5!=900000000 || nItemID5!=900001000 || nItemID5!=900002000 || nItemID5!=900003000 || nItemID5!=900004000 || nItemID5!=900005000 || nItemID5!=900006000 || nItemID5!=900007000 || nItemID5!=900008000 || nItemID5!=900009000 || nItemID5!=900010000 || nItemID5!=900011000)
		return false;

	return true;
}

bool CUser::RobItem(uint32_t nItemID, uint32_t sCount /*= 1*/)
{
	// Allow unused exchanges.
	 if (sCount == 0 || nItemID == 0)
	  return false;

	_ITEM_TABLE * pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable == nullptr)
		return false;
	if (nItemID==900000000) //	Noah
	{
		GoldLose(sCount,true);
			return true;
	}
	else if (nItemID==900001000) //	EXP
	{
		ExpChange(-(int64_t)sCount,true);
			return true;
	}	
	else if (nItemID==900002000 || nItemID==900003000) //CountryCONT and LadderPoint
	{
		SendLoyaltyChange(-(int32_t)sCount,true);
			return true;
	}
	else if (	nItemID==900004000 ||	//	Random
				nItemID==900005000 ||	//	Hunt
				nItemID==900006000 ||	//	Jobchange
				nItemID==900007000 ||	//	Skill
				nItemID==900008000 ||	//	Killopponentcountry
				nItemID==900009000 ||	//	Transport
				nItemID==900010000 ||	//	LevelUp
				nItemID==900011000 )	//	War
			return true;

	// Search for the existance of all items in the player's inventory storage and onwards (includes magic bags)
	for (int i = SLOT_MAX; i < INVENTORY_TOTAL; i++)
	{
		if (RobItem(i, pTable, sCount))
			return true;
	}

	return false;
}

bool CUser::RobItem(uint8_t bPos, _ITEM_TABLE * pTable, uint32_t sCount /*= 1*/)
{
	// Allow unused exchanges.
	if (sCount == 0)
		return false;

	if (pTable == nullptr)
		return false;

	_ITEM_DATA *pItem = GetItem(bPos);

	if (pItem == nullptr)
		return false;

	if (pItem->nNum != pTable->m_iNum
		|| pItem->sCount < sCount)
		return false;

	// Consumable "scrolls" (with some exceptions) use the duration/durability as a usage count
	// instead of the stack size. Interestingly, the client shows this instead of the stack size in this case.
	bool bIsConsumableScroll = (pTable->m_bKind == 255); /* include 97? not sure how accurate this check is... */
	if (bIsConsumableScroll)
		pItem->sDuration -= sCount;
	else
		pItem->sCount -= sCount;

	// Delete the item if the stack's now 0
	// or if the item is a consumable scroll and its "duration"/use count is now 0.
	if (pItem->sCount == 0 
		|| (bIsConsumableScroll && pItem->sDuration == 0))
		memset(pItem, 0, sizeof(_ITEM_DATA));

	SendStackChange(pTable->m_iNum, pItem->sCount, pItem->sDuration, bPos - SLOT_MAX);
	return true;
}

/**
* @brief	Checks if all players in the party have sCount of item nItemID
* 			and if so, removes it.
*
* @param	nItemID	Identifier for the item.
* @param	sCount 	Stack size.
*
* @return	true if the required items were taken, false if not.
*/
bool CUser::RobAllItemParty(uint32_t nItemID, uint16_t sCount /*= 1*/)
{
	// Allow unused exchanges.
	if (sCount == 0)
		return false;

	_PARTY_GROUP * pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr)
		return RobItem(nItemID, sCount);

	// First check to see if all users in the party have enough of the specified item.
	std::vector<CUser *> partyUsers;
	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser * pUser = g_pMain->GetUserPtr(pParty->uid[i]);
		if (pUser != nullptr
			&& !pUser->CheckExistItem(nItemID, sCount))
			return false;

		partyUsers.push_back(pUser);
	}

	// Since all users have the required item, we can now remove them. 
	foreach (itr, partyUsers)
		(*itr)->RobItem(nItemID, sCount);

	return true;
}

// NOTE(srmeier): testing this debug string functionality
void CUser::SendDebugString(const char* pString) {
	Packet result(WIZ_DEBUG_STRING_PACKET);

	std::string tempStr(pString);
	result << tempStr;

	Send(&result);
}

bool CUser::GiveItem(uint32_t itemid, uint16_t count, bool send_packet /*= true*/, uint32_t Time)
{
	int8_t pos;
	bool bNewItem = true;
	_ITEM_TABLE* pTable = g_pMain->GetItemPtr( itemid );
	if (pTable == nullptr)
		return false;	

	pos = FindSlotForItem(itemid, count);
	if (pos < 0)
		return false;

	_ITEM_DATA *pItem = GetItem(pos);
	if (pItem->nNum != 0 || pItem == nullptr) 
		bNewItem = false;

	if (bNewItem)
		pItem->nSerialNum = g_pMain->GenerateItemSerial();

	pItem->nNum = itemid;
	pItem->sCount += count;
	if (pItem->sCount > MAX_ITEM_COUNT)
		pItem->sCount = MAX_ITEM_COUNT;

	/*if (pItem->nNum == CHAOS_MAP)
	pItem->nExpirationTime = int32_t(UNIXTIME) + 86400; // 1 day */

	pItem->sDuration = pTable->m_sDuration;
	if (Time != 0)
	{
		pItem->nExpirationTime = int32_t(UNIXTIME) + ((60 * 60 * 24) * Time);
	}
	else
	{
		pItem->nExpirationTime = 0;
	}


	// This is really silly, but match the count up with the duration
	// for this special items that behave this way.
	if (pTable->m_bKind == 255)
		pItem->sCount = pItem->sDuration;

	if (send_packet)
	{
		SendStackChange(itemid, m_sItemArray[pos].sCount, m_sItemArray[pos].sDuration, pos - SLOT_MAX, true, pItem->nExpirationTime);
	}
	else
	{
		SetUserAbility(false);
		SendItemWeight();
	}

	return true;
}

void CUser::SendItemWeight()
{
	Packet result(WIZ_WEIGHT_CHANGE);
	result << m_sItemWeight;
	Send(&result);
}

bool CUser::ItemEquipAvailable(_ITEM_TABLE *pTable)
{
	// NOTE(srmeier): temp fix, see Issue #13 on Github
	return (pTable != nullptr
		//&& GetLevel() >= pTable->m_bReqLevel 
		//&& GetLevel() <= pTable->m_bReqLevelMax
		&& m_bRank >= pTable->m_bReqRank // this needs to be verified
		&& m_bTitle >= pTable->m_bReqTitle // this is unused
		&& GetStat(STAT_STR) >= pTable->m_bReqStr 
		&& GetStat(STAT_STA) >= pTable->m_bReqSta 
		&& GetStat(STAT_DEX) >= pTable->m_bReqDex 
		&& GetStat(STAT_INT) >= pTable->m_bReqIntel 
		&& GetStat(STAT_CHA) >= pTable->m_bReqCha);
}

void CUser::ItemMove(Packet & pkt)
{
	_ITEM_TABLE *pTable;
	_ITEM_DATA *pSrcItem, *pDstItem, tmpItem;
	uint32_t nItemID;
	uint8_t dir, bSrcPos, bDstPos;

	pkt >> dir >> nItemID >> bSrcPos >> bDstPos;

	if (isTrading()
		|| isMerchanting()
		|| GetZoneID() == ZONE_CHAOS_DUNGEON)
		goto fail_return;

	pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable == nullptr
		//  || dir == ITEM_INVEN_SLOT && ((pTable->m_sWeight + m_sItemWeight) > m_sMaxWeight))
			//  || dir > ITEM_MBAG_TO_MBAG || bSrcPos >= SLOT_MAX+HAVE_MAX+COSP_MAX+MBAG_MAX || bDstPos >= SLOT_MAX+HAVE_MAX+COSP_MAX+MBAG_MAX
				|| ((dir == ITEM_INVEN_SLOT || dir == ITEM_SLOT_SLOT) && (bDstPos > SLOT_MAX || !ItemEquipAvailable(pTable)))
				|| (dir == ITEM_SLOT_INVEN && bSrcPos > SLOT_MAX)
				|| ((dir == ITEM_INVEN_SLOT || dir == ITEM_SLOT_SLOT) && bDstPos == RESERVED))
				goto fail_return;

	switch (dir)
	{
	case ITEM_INVEN_SLOT:
		if (bDstPos >= SLOT_MAX || bSrcPos >= HAVE_MAX
			// Make sure that the item actually exists there.
				|| nItemID != m_sItemArray[INVENTORY_INVENT + bSrcPos].nNum
				// Disable duplicate item moving to slot.
				|| m_sItemArray[INVENTORY_INVENT + bSrcPos].isDuplicate()
				// Ensure the item is able to be equipped in that slot
				|| !IsValidSlotPos(pTable, bDstPos))
				goto fail_return;

		pSrcItem = &m_sItemArray[INVENTORY_INVENT + bSrcPos];
		pDstItem = &m_sItemArray[bDstPos];
		break;

	case ITEM_SLOT_INVEN:
		if (bDstPos >= HAVE_MAX || bSrcPos >= SLOT_MAX
			// Make sure that the item actually exists there.
				|| m_sItemArray[SLOT_MAX+bDstPos].nNum != 0
				|| nItemID != m_sItemArray[bSrcPos].nNum)
				goto fail_return;

		pSrcItem = &m_sItemArray[bSrcPos];
		pDstItem = &m_sItemArray[INVENTORY_INVENT + bDstPos];
		break;

	case ITEM_INVEN_INVEN:
		if (bDstPos >= HAVE_MAX || bSrcPos >= HAVE_MAX
			// Make sure that the item actually exists there.
				|| nItemID != m_sItemArray[INVENTORY_INVENT + bSrcPos].nNum)
				goto fail_return;

		pSrcItem = &m_sItemArray[INVENTORY_INVENT + bSrcPos];
		pDstItem = &m_sItemArray[INVENTORY_INVENT + bDstPos];
		break;

	case ITEM_SLOT_SLOT:
		if (bDstPos >= SLOT_MAX || bSrcPos >= SLOT_MAX
			// Make sure that the item actually exists there.
				|| nItemID != m_sItemArray[bSrcPos].nNum
				// Ensure the item is able to be equipped in that slot
				|| !IsValidSlotPos(pTable, bDstPos))
				goto fail_return;

		pSrcItem = &m_sItemArray[bSrcPos];
		pDstItem = &m_sItemArray[bDstPos];
		break;

	default:
		return;
	}

	// If there's an item already in the target slot already, we need to just swap the items
	if (pDstItem->nNum != 0)
	{
		memcpy(&tmpItem, pDstItem, sizeof(_ITEM_DATA)); // Temporarily store the target item
		memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA)); // Replace the target item with the source
		memcpy(pSrcItem, &tmpItem, sizeof(_ITEM_DATA)); // Now replace the source with the old target (swapping them)
	}
	// Since there's no way to move a partial stack using this handler, just overwrite the destination.
	else
	{
		memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA)); // Shift the item over
		memset(pSrcItem, 0, sizeof(_ITEM_DATA)); // Clear out the source item's data
	}

	// If equipping/de-equipping an item
	if (dir == ITEM_INVEN_SLOT
		|| dir == ITEM_SLOT_INVEN
		|| dir == ITEM_SLOT_SLOT)
	{
		// Re-update item stats
		SetUserAbility(false);
	}

	SendItemMove(1);	
	SendItemWeight();

	// Update everyone else, so that they can see your shiny new items (you didn't take them off did you!? DID YOU!?)
	switch (dir)
	{
	case ITEM_INVEN_SLOT:
		UserLookChange(bDstPos, nItemID, pDstItem->sDuration);	
		break;

	case ITEM_SLOT_INVEN:
		UserLookChange(bSrcPos, 0, 0);
		break;

	case ITEM_SLOT_SLOT:
		UserLookChange(bSrcPos, pSrcItem->nNum, pSrcItem->sDuration);
		UserLookChange(bDstPos, pDstItem->nNum, pDstItem->sDuration);
		break;
	}

	Send2AI_UserUpdateInfo();
	return;

fail_return:
	SendItemMove(0);
}

bool CUser::CheckExchange(int nExchangeID)
{
	// Does the exchange exist?
	_ITEM_EXCHANGE * pExchange = g_pMain->m_ItemExchangeArray.GetData(nExchangeID);
	if (pExchange == nullptr)
		return false;

	// Find free slots in the inventory, so that we can check against this later.
	uint8_t bFreeSlots = 0;
	for (int i = SLOT_MAX; i < SLOT_MAX+HAVE_MAX; i++)
	{
		if (GetItem(i)->nNum == 0
			&& ++bFreeSlots >= ITEMS_IN_EXCHANGE_GROUP)
			break;
	}

	// Add up the rates for this exchange to obtain a total percentage
	int nTotalPercent = 0;
	for (int i = 0; i < ITEMS_IN_EXCHANGE_GROUP; i++)
		nTotalPercent += pExchange->sExchangeItemCount[i];

	if (nTotalPercent > 9000)
		return (bFreeSlots > 0);

	// Can we hold all of these items? If we can't, we have a problem.
	uint8_t bReqSlots = 0;
	uint32_t nReqWeight = 0;
	for (int i = 0; i < ITEMS_IN_EXCHANGE_GROUP; i++)
	{
		uint32_t nItemID = pExchange->nExchangeItemNum[i];

		// Does the item exist? If not, we'll ignore it (NOTE: not official behaviour).
		_ITEM_TABLE * pTable = nullptr;
		if (nItemID == 0
			|| (pTable = g_pMain->GetItemPtr(nItemID)) == nullptr)
			continue;

		// Try to find a slot for the item.
		// If we can't find an applicable slot with our inventory as-is,
		// there's no point even checking further.
		int pos;
		if ((pos = FindSlotForItem(nItemID, 1)) < 0)
			return false;

		// Now that we have our slot, see if it's in use (i.e. if adding a stackable item)
		// If it's in use, then we don't have to worry about requiring an extra slot for this item.
		// The only caveat here is with having multiple of the same stackable item: 
		// theoretically we could give them OK, but when it comes time to adding them, we'll find that
		// there's too many of them and they can't fit in the same slot. 
		// As this isn't an issue with real use cases, we can ignore it.
		_ITEM_DATA *pItem = GetItem(pos);
		if (pItem->nNum == 0)
			bReqSlots++; // new item? new required slot.

		// Also take into account weight (not official behaviour)
		nReqWeight += pTable->m_sWeight;
	}

	// Holding too much already?
	if (m_sItemWeight + nReqWeight > m_sMaxWeight)
		return false;

	if (isTrading()
		|| isMerchanting())
		return false;

	// Do we have enough slots?
	return (bFreeSlots >= bReqSlots);
}

bool CUser::RunExchange(int nExchangeID, uint32_t count)
{
	_ITEM_EXCHANGE * pExchange = g_pMain->m_ItemExchangeArray.GetData(nExchangeID);

	uint32_t temp_sOriginItemCount0 = 0;
	uint32_t temp_sOriginItemCount1 = 0;
	uint32_t temp_sOriginItemCount2 = 0;
	uint32_t temp_sOriginItemCount3 = 0;
	uint32_t temp_sOriginItemCount4 = 0;
	uint32_t temp_sOriginItemCount5 = 0;
	uint32_t temp_sOriginItemCount6 = 0;
	uint32_t temp_sOriginItemCount7 = 0;
	uint32_t temp_sOriginItemCount8 = 0;
	uint32_t temp_sOriginItemCount9 = 0;
	uint32_t temp_sOriginItemCount10 = 0;

	uint32_t temp_sCount = 0;

	if (pExchange != nullptr)
	{
		uint32_t sItemCount[11];
		sItemCount[0] = GetItemCount(pExchange->nOriginItemNum[0]);
		sItemCount[1] = GetItemCount(pExchange->nOriginItemNum[1]);
		sItemCount[2] = GetItemCount(pExchange->nOriginItemNum[2]);
		sItemCount[3] = GetItemCount(pExchange->nOriginItemNum[3]);
		sItemCount[4] = GetItemCount(pExchange->nOriginItemNum[4]);
		sItemCount[5] = GetItemCount(pExchange->nOriginItemNum[5]);
		sItemCount[6] = GetItemCount(pExchange->nOriginItemNum[6]);
		sItemCount[7] = GetItemCount(pExchange->nOriginItemNum[7]);
		sItemCount[8] = GetItemCount(pExchange->nOriginItemNum[8]);
		sItemCount[9] = GetItemCount(pExchange->nOriginItemNum[9]);
		sItemCount[10] = GetItemCount(pExchange->nOriginItemNum[10]);
		temp_sCount = sItemCount[0];

		for (int i = 1; i < 11; i++){
			if (sItemCount[i] < temp_sCount && sItemCount[i] != 0)
				temp_sCount = sItemCount[i];
		}

		if (temp_sCount >= count)
			temp_sCount = count;

		temp_sOriginItemCount0 = pExchange->nOriginItemNum[0] == 0 ? 0 : (count == 0 ? pExchange->sOriginItemCount[0] : temp_sCount);
		temp_sOriginItemCount1 = pExchange->nOriginItemNum[1] == 0 ? 0 : (count == 0 ? pExchange->sOriginItemCount[1] : temp_sCount);
		temp_sOriginItemCount2 = pExchange->nOriginItemNum[2] == 0 ? 0 : (count == 0 ? pExchange->sOriginItemCount[2] : temp_sCount);
		temp_sOriginItemCount3 = pExchange->nOriginItemNum[3] == 0 ? 0 : (count == 0 ? pExchange->sOriginItemCount[3] : temp_sCount);
		temp_sOriginItemCount4 = pExchange->nOriginItemNum[4] == 0 ? 0 : (count == 0 ? pExchange->sOriginItemCount[4] : temp_sCount);
	}
	
	if (isTrading()
		|| isMerchanting())
		return false;

	// no empty space in inventory
	if (!CheckExchange(nExchangeID)) 
	{
		Packet result(WIZ_ITEM_GET);
		result << uint8_t(0); //error code : loot no room = 0
		Send(&result);
		return false;
	}

	if (pExchange == nullptr
			// We handle flags from 0-101 only. Anything else is broken.
			|| pExchange->bRandomFlag > 101)
			return false;

			// Do we have all of the required items?
			CheckExistItemAnd(
				pExchange->nOriginItemNum[0], temp_sOriginItemCount0, 
				pExchange->nOriginItemNum[1], temp_sOriginItemCount1, 
				pExchange->nOriginItemNum[2], temp_sOriginItemCount2, 
				pExchange->nOriginItemNum[3], temp_sOriginItemCount3, 
				pExchange->nOriginItemNum[4], temp_sOriginItemCount4);
			// These checks are a little pointless, but remove the required items as well.
			RobItem(pExchange->nOriginItemNum[0], temp_sOriginItemCount0);
			RobItem(pExchange->nOriginItemNum[1], temp_sOriginItemCount1);
			RobItem(pExchange->nOriginItemNum[2], temp_sOriginItemCount2);
			RobItem(pExchange->nOriginItemNum[3], temp_sOriginItemCount3);
			RobItem(pExchange->nOriginItemNum[4], temp_sOriginItemCount4);

	// No random element? We're just exchanging x items for y items.
	if (pExchange->bRandomFlag == 0
		|| pExchange->bRandomFlag == 10
		|| pExchange->bRandomFlag == 11
		|| pExchange->bRandomFlag == 12)
	{
		for (int i = 0; i < ITEMS_IN_EXCHANGE_GROUP; i++)
		{
			bool item=false;
			if (count == 0)
			{
				if (pExchange->nExchangeItemNum[i]==900000000)//noah
				{
					GoldGain(pExchange->sExchangeItemCount[i],true,false);
					item =true;
				}
				else if (pExchange->nExchangeItemNum[i]==900001000)//exp
				{
					ExpChange(pExchange->sExchangeItemCount[i],false);
					item =true;
				}
				else if (pExchange->nExchangeItemNum[i]==900002000||pExchange->nExchangeItemNum[i]==900003000)//Country CONT Ladder Point
				{
					SendLoyaltyChange(pExchange->sExchangeItemCount[i],false);
					item =true;
				}
				else if (pExchange->nExchangeItemNum[i]==900004000   //random
						|| pExchange->nExchangeItemNum[i]==900005000 //hunt
						|| pExchange->nExchangeItemNum[i]==900006000 //Jobchange
						|| pExchange->nExchangeItemNum[i]==900007000 //Skill
						|| pExchange->nExchangeItemNum[i]==900008000 //Killopponentcountry
						|| pExchange->nExchangeItemNum[i]==900009000 //Transport
						|| pExchange->nExchangeItemNum[i]==900010000 //LevelUp
						|| pExchange->nExchangeItemNum[i]==900011000 //War
						)
				{
					item =true;
				}

				else if(!item)
				{
				GiveItem(pExchange->nExchangeItemNum[i], pExchange->sExchangeItemCount[i]);
				}
			}
			else if (pExchange->nExchangeItemNum[i]==900000000)//noah
				{
					GoldGain(pExchange->sExchangeItemCount[i]*count,true,false);
					item =true;
				}
				else if (pExchange->nExchangeItemNum[i]==900001000)//exp
				{
					ExpChange(pExchange->sExchangeItemCount[i]*count,false);
					item =true;
				}
				else if (pExchange->nExchangeItemNum[i]==900002000||pExchange->nExchangeItemNum[i]==900003000)//Country CONT Ladder Point
				{
					SendLoyaltyChange(pExchange->sExchangeItemCount[i]*count,false);
					item =true;
				}
			else
				GiveItem(pExchange->nExchangeItemNum[i], temp_sCount);
		}
	}
	// For these items the rate set by bRandomFlag.
	else if (pExchange->bRandomFlag <= 100)
	{
		int rand = myrand(0, 1000 * pExchange->bRandomFlag) / 1000;
		if (rand >= 5)
			rand = 4;

		if (rand <= 4)
			GiveItem(pExchange->nExchangeItemNum[rand], pExchange->sExchangeItemCount[rand]);
	}
	// For 101, the rates are determined by sExchangeItemCount.
	else if (pExchange->bRandomFlag == 101)
	{
		uint32_t nTotalPercent = 0;
		for (int i = 0; i < ITEMS_IN_EXCHANGE_GROUP; i++)
			nTotalPercent += pExchange->sExchangeItemCount[i];

		// If they add up to more than 100%, 
		if (nTotalPercent > 10000)
		{
			TRACE("Exchange %d is invalid. Rates add up to more than 100%% (%d%%)", nExchangeID, nTotalPercent / 100);
			return false;
		}

		// Holy stack batman! We're just going ahead and copying official for now.
		// NOTE: Officially they even use 2 bytes per element. Yikes.
		uint8_t bRandArray[10000];
		memset(&bRandArray, 0, sizeof(bRandArray)); // default to 0 in case it's lower than 100% (in which case, first item's rate increases)

		// Copy the counts, as we're going to adjust them locally.
		uint32_t sExchangeCount[ITEMS_IN_EXCHANGE_GROUP];

		memcpy(&sExchangeCount, &pExchange->sExchangeItemCount, sizeof(pExchange->sExchangeItemCount));

		// Build array of exchange item slots (0-4)
		int offset = 0;
		for (int n = 0, i = 0; n < ITEMS_IN_EXCHANGE_GROUP; n++)
		{
			if (sExchangeCount[n] > 0)
			{
				memset(&bRandArray[offset], n, sExchangeCount[n]);
				offset += sExchangeCount[n];
			}
		}

		// Pull our exchange item slot out of our hat (the array we generated).
		uint8_t bRandSlot = bRandArray[myrand(0, 9999)];
		uint32_t nItemID = pExchange->nExchangeItemNum[bRandSlot];

		// Finally, give our item.
		GiveItem(nItemID, 1);
	}

	return true;
}

bool CUser::RunSelectExchange(int nExchangeID)
{
	Packet result(WIZ_EXCHANGE);
	result << uint8_t(EXCHANGE_REQ);
	Send(&result);
	return true;
}

uint32_t CUser::GetMaxExchange(int nExchangeID)
{
	uint32_t sResult = 0;
	_ITEM_TABLE * pTable;
	uint32_t temp_sCount = 0;

	_ITEM_EXCHANGE * pExchange = g_pMain->m_ItemExchangeArray.GetData(nExchangeID);

	if (pExchange != nullptr)
	{
		for (int i = 0; i < ITEMS_IN_EXCHANGE_GROUP; i++)
		{
			pTable = g_pMain->GetItemPtr(pExchange->nOriginItemNum[i]);
			if (pTable != nullptr)
				temp_sCount += GetItemCount(pExchange->nOriginItemNum[i]);
		}

		sResult = temp_sCount;
	}

	return sResult;
}

bool CUser::IsValidSlotPos(_ITEM_TABLE* pTable, int destpos)
{
	if (pTable == nullptr)
		return false;

	bool bOneHandedItem = false;
	switch (pTable->m_bSlot)
	{
	case ItemSlot1HEitherHand:
		if (destpos != RIGHTHAND && destpos != LEFTHAND)
			return false;

		bOneHandedItem = true;
		break;

	case ItemSlot1HRightHand:
		if (destpos != RIGHTHAND)
			return false;

		bOneHandedItem = true;
		break;

		// If we're equipping a 2H item in our right hand, there must
		// be no item in our left hand.
	case ItemSlot2HRightHand: //
		if (destpos != RIGHTHAND || GetItem(LEFTHAND)->nNum != 0)
	{
	_ITEM_DATA *pSrcItem = GetItem(LEFTHAND), *pDstItem = GetItem(RIGHTHAND), tmpItem ;
		memcpy(&tmpItem, pDstItem, sizeof(_ITEM_DATA)); // Temporarily store the target item
		memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA)); // Replace the target item with the source
		memcpy(pSrcItem, &tmpItem, sizeof(_ITEM_DATA)); // Now replace the source with the old target (swapping them)
	}
		break;

	case ItemSlot1HLeftHand:
		if (destpos != LEFTHAND)
			return false;

		bOneHandedItem = true;
		break;

		// If we're equipping a 2H item in our left hand, there must
		// be no item in our right hand.
	case ItemSlot2HLeftHand://
		if (destpos != LEFTHAND || GetItem(RIGHTHAND)->nNum != 0)	
	{
	_ITEM_DATA *pSrcItem = GetItem(RIGHTHAND), *pDstItem = GetItem(LEFTHAND), tmpItem ;
		memcpy(&tmpItem, pDstItem, sizeof(_ITEM_DATA)); // Temporarily store the target item
		memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA)); // Replace the target item with the source
		memcpy(pSrcItem, &tmpItem, sizeof(_ITEM_DATA)); // Now replace the source with the old target (swapping them)
	}
		break;

	case ItemSlotPauldron:
		if (destpos != BREAST)
			return false;
		break;

	case ItemSlotPads:
		if (destpos != LEG)
			return false;
		break;

	case ItemSlotHelmet:
		if (destpos != HEAD)
			return false;
		break;

	case ItemSlotGloves:
		if (destpos != GLOVE)
			return false;
		break;

	case ItemSlotBoots:
		if (destpos != FOOT)
			return false;
		break;

	case ItemSlotEarring:
		if (destpos != RIGHTEAR && destpos != LEFTEAR)
			return false;
		break;

	case ItemSlotNecklace:
		if (destpos != NECK)
			return false;
		break;

	case ItemSlotRing:
		if (destpos != RIGHTRING && destpos != LEFTRING)
			return false;
		break;

	case ItemSlotShoulder:
		if (destpos != SHOULDER)
			return false;
		break;

	case ItemSlotBelt:
		if (destpos != WAIST)
			return false;
		break;

	default:
		return false;
	}

	// 1H items can only be equipped when a 2H item isn't equipped.
	if (bOneHandedItem)
	{
		_ITEM_DATA* pItem;
		_ITEM_TABLE* pTable2 = GetItemPrototype(destpos == LEFTHAND ? RIGHTHAND : LEFTHAND, pItem);
		if (pTable2 != nullptr && pTable2->is2Handed())
		{
			_ITEM_DATA* pSrcItem = GetItem(RIGHTHAND);
			_ITEM_DATA* pDstItem = GetItem(LEFTHAND);
			_ITEM_DATA tmpItem;
			memcpy(&tmpItem, pDstItem, sizeof(_ITEM_DATA)); // Temporarily store the target item
			memcpy(pDstItem, pSrcItem, sizeof(_ITEM_DATA)); // Replace the target item with the source
			memcpy(pSrcItem, &tmpItem, sizeof(_ITEM_DATA)); // Now replace the source with the old target (swapping them)
		}
	}

	return true;
}

void CUser::SendStackChange(uint32_t nItemID, uint32_t nCount /* needs to be 4 bytes, not a bug */, uint16_t sDurability, uint8_t bPos, bool bNewItem /* = false */, uint32_t Time)
{
	Packet result(WIZ_ITEM_COUNT_CHANGE);

	result << uint16_t(1);
	result << uint8_t(1);
	result << uint8_t(bPos);
	result << nItemID << nCount;
	result << uint8_t(bNewItem ? 100 : 0);
	result << sDurability;
	if (Time != 0)
	{
		result << Time << Time;
	}

	SetUserAbility(false);
	SendItemWeight();

	Send(&result);
}

void CUser::ItemRemove(Packet & pkt)
{
	Packet result(WIZ_ITEM_REMOVE);
	_ITEM_DATA * pItem;
	uint8_t bType, bPos;
	uint32_t nItemID;

	pkt >> bType >> bPos >> nItemID;

	// Inventory
	if (bType == 0)
	{
		if (bPos >= HAVE_MAX)
			goto fail_return;

		bPos += SLOT_MAX;
	}
	// Equipped items
	else if (bType == 1)
	{
		if (bPos >= SLOT_MAX)
			goto fail_return;
	}
	else if (bType == 2)
	{
		if (bPos >= HAVE_MAX)
			goto fail_return;
		bPos += SLOT_MAX;
	}

	pItem = GetItem(bPos);

	// Make sure the item matches what the client says it is
	if (pItem == nullptr
		|| pItem->nNum != nItemID
		|| pItem->isRented()
		|| GetZoneID() == ZONE_CHAOS_DUNGEON) 
		goto fail_return;

	memset(pItem, 0, sizeof(_ITEM_DATA));

	SetUserAbility();
	SendItemWeight();

	result << uint8_t(1);
	Send(&result);
	return;

fail_return:
	result << uint8_t(0);
	Send(&result);
}
