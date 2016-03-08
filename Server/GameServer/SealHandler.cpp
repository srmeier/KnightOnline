#include "stdafx.h"

using std::string;

#define ITEM_SEAL_PRICE 1000000

enum
{
	SEAL_TYPE_SEAL		= 1,
	SEAL_TYPE_UNSEAL	= 2,
	SEAL_TYPE_KROWAZ	= 3
};

enum SealErrorCodes
{
	SealErrorNone			= 0, // no error, success!
	SealErrorFailed			= 2, // "Seal Failed."
	SealErrorNeedCoins		= 3, // "Not enough coins."
	SealErrorInvalidCode	= 4, // "Invalid Citizen Registry Number" (i.e. invalid code/password)
	SealErrorPremiumOnly	= 5, // "Only available to premium users"
	SealErrorFailed2		= 6, // "Seal Failed."
	SealErrorTooSoon		= 7, // "Please try again. You may not repeat this function instantly."
};

/**
* @brief	Packet handler for the item sealing system.
*
* @param	pkt	The packet.
*/
void CUser::ItemSealProcess(Packet & pkt)
{
	// Seal type
	uint8 opcode = pkt.read<uint8>();

	Packet result(WIZ_ITEM_UPGRADE, uint8(ITEM_SEAL));
	result << opcode;

	switch (opcode)
	{
		// Used when sealing an item.
	case SEAL_TYPE_SEAL:
		{
			string strPasswd;
			uint32 nItemID; 
			int16 unk0; // set to -1 in this case
			uint8 bSrcPos, bResponse = SealErrorNone;
			pkt >> unk0 >> nItemID >> bSrcPos >> strPasswd;

			/* 
			Most of these checks are handled client-side, so we shouldn't need to provide error messages.
			Also, item sealing requires certain premium types (gold, platinum, etc) - need to double-check 
			these before implementing this check.
			*/

			// is this a valid position? (need to check if it can be taken from new slots)
			if (bSrcPos >= HAVE_MAX 
				// does the item exist where the client says it does?
					|| GetItem(SLOT_MAX + bSrcPos)->nNum != nItemID
					// i ain't be allowin' no stealth items to be sealed!
					|| GetItem(SLOT_MAX + bSrcPos)->nSerialNum == 0)
					bResponse = SealErrorFailed;
			// is the password valid by client limits?
			else if (strPasswd.empty() || strPasswd.length() > 8)
				bResponse = SealErrorInvalidCode;
			// do we have enough coins?
			else if (!hasCoins(ITEM_SEAL_PRICE))
				bResponse = SealErrorNeedCoins;

			_ITEM_TABLE* pItem = g_pMain->m_ItemtableArray.GetData(nItemID);

			if(pItem == nullptr)
				return; 

#if 0 // this doesn't look right
			// If the item's not equippable we not be lettin' you seal no moar!
			if (pItem->m_bSlot >= SLOT_MAX)
				bResponse = SealErrorFailed;
#endif

			// If no error, pass it along to the database.
			if (bResponse == SealErrorNone)
			{
				result << nItemID << bSrcPos << strPasswd << bResponse;
				g_pMain->AddDatabaseRequest(result, this);
			}
			// If there's an error, tell the client.
			// From memory though, there was no need -- it handled all of these conditions itself
			// so there was no need to differentiate (just ignore the packet). Need to check this.
			else 
			{
				result << bResponse;
				Send(&result);
			}
		} break;

		// Used when unsealing an item.
	case SEAL_TYPE_UNSEAL:
		{
			string strPasswd;
			uint32 nItemID; 
			int16 unk0; // set to -1 in this case
			uint8 bSrcPos, bResponse = SealErrorNone;
			pkt >> unk0 >> nItemID >> bSrcPos >> strPasswd;

			if (bSrcPos >= HAVE_MAX
				|| GetItem(SLOT_MAX+bSrcPos)->bFlag != ITEM_FLAG_SEALED
				|| GetItem(SLOT_MAX+bSrcPos)->nNum != nItemID)
				bResponse = SealErrorFailed;
			else if (strPasswd.empty() || strPasswd.length() > 8)
				bResponse = SealErrorInvalidCode;

			// If no error, pass it along to the database.
			if (bResponse == SealErrorNone)
			{
				result << nItemID << bSrcPos << strPasswd << bResponse;
				g_pMain->AddDatabaseRequest(result, this);
			}
			// If there's an error, tell the client.
			// From memory though, there was no need -- it handled all of these conditions itself
			// so there was no need to differentiate (just ignore the packet). Need to check this.
			else
			{
				result << bResponse;
				Send(&result);
			}
		} break;

		// Used when binding a Krowaz item (used to take it from not bound -> bound)
	case SEAL_TYPE_KROWAZ:
		{
			string strPasswd = "0"; //Dummy, not actually used.
			uint32 nItemID;
			uint8 bSrcPos = 0 , unk3, bResponse = SealErrorNone;
			uint16 unk1, unk2;
			pkt >> unk1 >> nItemID >> bSrcPos >> unk3 >> unk2;

			if (bSrcPos >= HAVE_MAX
				|| GetItem(SLOT_MAX+bSrcPos)->bFlag != ITEM_FLAG_NONE
				|| GetItem(SLOT_MAX+bSrcPos)->nNum != nItemID)
				bResponse = SealErrorFailed;

			if (bResponse == SealErrorNone)
			{
				result << nItemID << bSrcPos << strPasswd << bResponse;
				g_pMain->AddDatabaseRequest(result, this);
			}
		} break;
	}
}

void CUser::SealItem(uint8 bSealType, uint8 bSrcPos)
{
	_ITEM_DATA * pItem = GetItem(SLOT_MAX + bSrcPos);
	if (pItem == nullptr)
		return;

	switch (bSealType)
	{
	case SEAL_TYPE_SEAL:
		pItem->bFlag = ITEM_FLAG_SEALED;
		GoldLose(ITEM_SEAL_PRICE);
		break;

	case SEAL_TYPE_UNSEAL:
		pItem->bFlag = 0;
		break;

	case SEAL_TYPE_KROWAZ:
		pItem->bFlag = ITEM_FLAG_BOUND;
		break;
	}
}

/**
* @brief	Packet handler for the character sealing system.
*
* @param	pkt	The packet.
*/
void CUser::CharacterSealProcess(Packet & pkt)
{
}