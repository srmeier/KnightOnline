#include "stdafx.h"

enum MerchantOpenResponseCodes
{
	MERCHANT_OPEN_SUCCESS = 1,
	MERCHANT_OPEN_NO_SESSION = -1,
	MERCHANT_OPEN_DEAD = -2,
	MERCHANT_OPEN_TRADING = -3,
	MERCHANT_OPEN_MERCHANTING = -4,
	MERCHANT_OPEN_INVALID_ZONE = -5,
	MERCHANT_OPEN_SHOPPING = -6,
	MERCHANT_OPEN_UNDERLEVELED = 30
};

void CUser::MerchantProcess(Packet & pkt)
{
	uint8_t opcode = pkt.read<uint8_t>();
	switch (opcode)
	{
	// Regular merchants
	case MERCHANT_OPEN: 
		MerchantOpen(); 
		break;

	case MERCHANT_CLOSE: 
		MerchantClose(); 
		break;

	case MERCHANT_ITEM_ADD: 
		MerchantItemAdd(pkt); 
		break;

	case MERCHANT_ITEM_CANCEL: 
		MerchantItemCancel(pkt); 
		break;

	case MERCHANT_ITEM_LIST: 
		MerchantItemList(pkt); 
		break;

	case MERCHANT_ITEM_BUY: 
		MerchantItemBuy(pkt); 
		break;

	case MERCHANT_INSERT: 
		MerchantInsert(pkt); 
		break;

	case MERCHANT_TRADE_CANCEL: 
		CancelMerchant(); 
		break;
	}
}

void CUser::MerchantOpen()
{
	int16_t errorCode = 0;
	if (isDead())
		errorCode = MERCHANT_OPEN_DEAD;
	else if (isStoreOpen())
		errorCode = MERCHANT_OPEN_SHOPPING;
	else if (isTrading())
		errorCode = MERCHANT_OPEN_TRADING;
	else if (GetZoneID() > ZONE_MORADON || GetZoneID() <= ELMORAD)
		errorCode = MERCHANT_OPEN_INVALID_ZONE;
	//else if (GetLevel() < 30)
	//	errorCode = MERCHANT_OPEN_UNDERLEVELED;
	else if (isMerchanting())
		errorCode = MERCHANT_OPEN_MERCHANTING;
	else 
		errorCode = MERCHANT_OPEN_SUCCESS;

	Packet result(WIZ_MERCHANT);
	result << uint8_t(MERCHANT_OPEN) << errorCode;
	Send(&result);

	// If we're already merchanting, user may be desynced
	// so we need to close our current merchant first.
	if (errorCode == MERCHANT_OPEN_MERCHANTING)
		MerchantClose();
}

void CUser::MerchantClose()
{
	//if (!isMerchanting())
	//	return;

	m_bMerchantState = MERCHANT_STATE_NONE;
	for (int i = SLOT_MAX; i < SLOT_MAX+HAVE_MAX; i++)
	{
		_ITEM_DATA *pItem = &m_sItemArray[i];
		pItem->IsSelling = false;
	}
	memset(&m_arMerchantItems, 0, sizeof(m_arMerchantItems));	
	//GiveMerchantItems(); // Give back to the user that which hasn't been sold, if any.
	Packet result(WIZ_MERCHANT);
	result << uint8_t(MERCHANT_CLOSE) << GetSocketID();
	SendToRegion(&result);
}

void CUser::MerchantItemAdd(Packet & pkt)
{
	Packet result(WIZ_MERCHANT);
	uint32_t nGold, nItemID;
	uint16_t sCount;
	uint8_t bSrcPos, // It sends the "actual" inventory slot (SLOT_MAX -> INVENTORY_MAX-SLOT_MAX), so need to allow for it. 
		bDstPos, 
		bMode; // Might be a flag for normal / "premium" merchant mode, once skills are implemented take another look at this.

	result << uint8_t(MERCHANT_ITEM_ADD);
	pkt >> nItemID >> sCount >> nGold >> bSrcPos >> bDstPos >> bMode;

	// TODO: Implement the possible error codes for these various error cases.
	if (bSrcPos >= HAVE_MAX
		|| bDstPos >= MAX_MERCH_ITEMS)
		return;

	_ITEM_TABLE * pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable == nullptr
		|| nItemID >= ITEM_NO_TRADE // Cannot be traded, sold or stored.
		|| pTable->m_bRace == RACE_UNTRADEABLE) // Cannot be traded or sold.
		goto fail_return;

	bSrcPos += SLOT_MAX;
	_ITEM_DATA *pSrcItem = GetItem(bSrcPos);
	if (pSrcItem == nullptr || pSrcItem->nNum != nItemID 
		|| pSrcItem->sCount < sCount
		|| pSrcItem->isRented()
		|| pSrcItem->isDuplicate())
		goto fail_return;

	_MERCH_DATA *pMerch = &m_arMerchantItems[bDstPos];

	if(pMerch == nullptr)
		return; 

	if (pSrcItem->IsSelling)
		goto fail_return;

	pMerch->nNum = nItemID;
	pMerch->nPrice = nGold;
	pMerch->sCount = sCount; // Selling Count
	pMerch->bCount = pSrcItem->sCount; // Original Count ( INVENTORY )
	pMerch->sDuration = pSrcItem->sDuration;
	pMerch->nSerialNum = pSrcItem->nSerialNum; // NOTE: Stackable items will have an issue with this.
	pMerch->bOriginalSlot = bSrcPos;
	pMerch->IsSoldOut = false;
	pSrcItem->IsSelling = true;

	// Take the user's item.
	//memset(pSrcItem, 0, sizeof(_ITEM_DATA));

	result	<< uint16_t(1)
		<< nItemID << sCount << pMerch->sDuration << nGold 
		<< bSrcPos << bDstPos;
	Send(&result);

fail_return:
	result  << uint16_t(0)
		<< nItemID << sCount << pMerch->sDuration << nGold 
		<< bSrcPos << bDstPos;
	Send(&result);
}

void CUser::MerchantItemCancel(Packet & pkt)
{
	Packet result(WIZ_MERCHANT);
	_MERCH_DATA *pMerch = nullptr;
	_ITEM_DATA *pItem = nullptr;

	result << uint8_t(MERCHANT_ITEM_CANCEL);

	uint8_t bSrcPos = pkt.read<uint8_t>();

	/*	if (this == nullptr)
	result << int16_t(-1);*/
	// Invalid source position
	if (bSrcPos >= MAX_MERCH_ITEMS)
		result << int16_t(-2);
	// There's no item in that list..?
	else if ((pMerch = &m_arMerchantItems[bSrcPos])->nNum == 0)
		result << int16_t(-3);
	// Check to make sure we've got a valid stack
	else if (pMerch->sCount + (pItem = &m_sItemArray[pMerch->bOriginalSlot])->sCount > HAVE_MAX) 
		result << int16_t(-3); // custom error
	else
	{
		pItem->nNum = pMerch->nNum;
		pItem->sCount = pMerch->sCount;
		pItem->sDuration = pMerch->sDuration;
		pItem->nSerialNum = pMerch->nSerialNum; // NOTE: Stackable items will have an issue with this.
		memset(pMerch, 0, sizeof(_MERCH_DATA));
		result << int16_t(1) << bSrcPos;
	}

	Send(&result);
}

void CUser::MerchantItemList(Packet & pkt)
{
	if (m_sMerchantsSocketID >= 0)
		RemoveFromMerchantLookers(); //This check should never be hit...

	uint16_t uid = pkt.read<uint16_t>();

	CUser *pMerchant = g_pMain->GetUserPtr(uid);
	if (pMerchant == nullptr
		|| !pMerchant->isMerchanting())
		return;

	m_sMerchantsSocketID = uid;
	pMerchant->m_arMerchantLookers.push_front(GetSocketID());

	Packet result(WIZ_MERCHANT);
	result << uint8_t(MERCHANT_ITEM_LIST) << uint16_t(1) << uint16_t(uid);
	for (int i = 0; i < MAX_MERCH_ITEMS; i++)
	{
		_MERCH_DATA *pMerch = &pMerchant->m_arMerchantItems[i];
		result	<< pMerch->nNum << pMerch->sCount
			<< pMerch->sDuration << pMerch->nPrice
			<< uint32_t(0); // Not sure what this one is
	}
	Send(&result);
}

void CUser::MerchantItemBuy(Packet & pkt)
{
	uint32_t itemid, req_gold;
	uint16_t item_count, leftover_count;
	uint8_t item_slot, dest_slot, rob_slot;

	CUser *pMerchant = g_pMain->GetUserPtr(m_sMerchantsSocketID),
		*pUser = g_pMain->GetUserPtr(GetID());
	if (pMerchant == nullptr)
		return;

	pkt >> itemid >> item_count >> item_slot >> dest_slot >> rob_slot;

	// Make sure the slots are correct and that we're actually buying at least 1 item.
	if (item_slot >= MAX_MERCH_ITEMS
		|| dest_slot >= HAVE_MAX
		|| item_count == 0)
		return;

	// Grab pointers to the items.
	_MERCH_DATA *pMerch = &pMerchant->m_arMerchantItems[item_slot];
	_ITEM_DATA *pItem = GetItem(SLOT_MAX + dest_slot);
	_ITEM_DATA *pItemRob = &pMerchant->m_sItemArray[pMerch->bOriginalSlot];

	// Make sure the merchant actually has that item in that slot
	// and that they have enough
	if (pMerch->nNum != itemid
		|| pMerch->sCount < item_count)
		return;

	// If it's not stackable, and we're specifying something other than 1
	// we really don't care to handle this request...
	_ITEM_TABLE *proto = g_pMain->GetItemPtr(itemid);
	if (proto == nullptr
		|| !proto->m_bCountable && item_count != 1)
		return;

	// Do we have enough coins?
	req_gold = pMerch->nPrice * item_count;
	if (m_iGold < req_gold)
		return;

	// If the slot's not empty
	if (pItem->nNum != 0 
		// and we already have an item that isn't the same item
			// or it's the same item but the item's not stackable...
				&& (pItem->nNum != itemid || !proto->m_bCountable))
				return;

	leftover_count = pMerch->sCount - item_count;
	pMerchant->GoldChange(GetSocketID(), req_gold);
	pItem->nNum = itemid;
	pItem->sCount += item_count;
	pItem->sDuration = pMerch->sDuration;
	pItem->nSerialNum = pMerch->nSerialNum;

	pMerch->sCount -= item_count;
	pMerch->bCount -= item_count;
	
	if (pMerch->sCount == 0 && pMerch->bCount == 0)
	memset(pItemRob, 0, sizeof(_ITEM_DATA));
	else
		pItemRob->sCount = pMerch->sCount;

	SendStackChange(itemid, pItem->sCount, pItem->sDuration, dest_slot, 
		(pItem->sCount == item_count)); // is it a new item?

	pMerchant->SendStackChange(itemid, pMerch->bCount, pMerch->sDuration,
		pMerch->bOriginalSlot- SLOT_MAX);

	if (pMerch->sCount == 0 && pMerch->bCount == 0)
		memset(pMerch, 0, sizeof(_MERCH_DATA));
	else if (pMerch->bCount == 0 && pMerch->bCount != 0) // Countable item protect.
		pMerch->IsSoldOut = true;

	Packet result(WIZ_MERCHANT);
	result << uint8_t(MERCHANT_ITEM_PURCHASED) << itemid << GetName();
	pMerchant->Send(&result);

	result.Initialize(WIZ_MERCHANT);
	result	<< uint8_t(MERCHANT_ITEM_BUY) << uint16_t(1)
		<< itemid << leftover_count
		<< item_slot << dest_slot;
	Send(&result);

	if (item_slot < 4 && leftover_count == 0)
	{
		result.Initialize(WIZ_MERCHANT_INOUT);
		result << uint8_t(2) << m_sMerchantsSocketID << uint8_t(1) << uint8_t(0) << item_slot;
		pMerchant->SendToRegion(&result);
	}

	g_pMain->MerchantUserInOutForMe(pMerchant);
	g_pMain->MerchantUserInOutForMe(pUser);

	int nItemsRemaining = 0;
	for (int i = 0; i < MAX_MERCH_ITEMS; i++)
	{
		if (pMerchant->m_arMerchantItems[i].nNum != 0 && !pMerchant->m_arMerchantItems[i].IsSoldOut)
			nItemsRemaining++;
	}

	if (nItemsRemaining == 0)
		pMerchant->MerchantClose();
}			

void CUser::MerchantInsert(Packet & pkt)
{
	std::string advertMessage; // check here maybe to make sure they're not using it otherwise?
	pkt >> advertMessage;
	if (advertMessage.size() > MAX_MERCH_MESSAGE)
		return;

	m_bMerchantState = MERCHANT_STATE_SELLING;

	Packet result(WIZ_MERCHANT);
	result << uint8_t(MERCHANT_INSERT) << uint16_t(1) << advertMessage << GetSocketID();
		//<< m_bPremiumMerchant; 

	for (int i = 0; i < MAX_MERCH_ITEMS; i++)
		result << m_arMerchantItems[i].nNum;

	SendToRegion(&result);
}

void CUser::GiveMerchantItems()
{
	for (int i = 0; i < MAX_MERCH_ITEMS; i++)
	{
		_MERCH_DATA *pMerch = &m_arMerchantItems[i];  
		// Prevent küpe bug :P
		if (pMerch->nNum == 0) continue;
		_ITEM_DATA *pItem = &m_sItemArray[pMerch->bOriginalSlot];

		pItem->nNum = pMerch->nNum;
		pItem->nSerialNum = pMerch->nSerialNum;
		pItem->sCount = pMerch->bCount;
		pItem->sDuration = pMerch->sDuration;

		// NOTE: Don't need to update the client, the client doesn't see any change.
	}

	// remove the items from the array now that they've been restored to the user
	memset(&m_arMerchantItems, 0, sizeof(m_arMerchantItems));
}

void CUser::CancelMerchant()
{
	if (m_sMerchantsSocketID < 0)
		return;

	RemoveFromMerchantLookers();
	Packet result(WIZ_MERCHANT);
	result << uint8_t(MERCHANT_TRADE_CANCEL) << uint16_t(1);
	Send(&result);
}

void CUser::RemoveFromMerchantLookers()
{
	CUser *pPreviousMerchantUser = g_pMain->GetUserPtr(m_sMerchantsSocketID);
	if (pPreviousMerchantUser == nullptr)
		return;

	pPreviousMerchantUser->m_arMerchantLookers.remove(GetSocketID());
	m_sMerchantsSocketID = -1;
}
