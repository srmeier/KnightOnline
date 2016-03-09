#include "stdafx.h"
#include "Map.h"
#include "KnightsManager.h"
#include "KingSystem.h"
#include "MagicInstance.h"
#include "DBAgent.h"
#include <algorithm>
#include "../shared/DateTime.h"

using namespace std;

CUser::CUser(uint16 socketID, SocketMgr *mgr) : KOSocket(socketID, mgr, -1, 16384, 3172), Unit(UnitPlayer)
{
	m_bHasCheckedClientVersion = false;
}

/**
* @brief	Executes the connect action.
*/
void CUser::OnConnect()
{
	KOSocket::OnConnect();
	Initialize();
}

/**
* @brief	Initializes this object.
*/
void CUser::Initialize()
{
	Unit::Initialize();

	m_strUserID.clear();
	m_strAccountID.clear();
	m_bLogout = 0;

	m_bAuthority = 1;
	m_sBind = -1;

	m_state = GAME_STATE_CONNECTED;

	m_bSelectedCharacter = false;
	m_bStoreOpen = false;
	m_bPartyLeader = false;
	m_bIsChicken = false;
	m_bIsHidingHelmet = false;
	m_bMining = false;
	m_bPremiumMerchant = false;
	m_bInParty = false;

	m_tLastMiningAttempt = 0;

	m_bMerchantState = MERCHANT_STATE_NONE;
	m_bInvisibilityType = INVIS_NONE;

	m_sDirection = 0;

	m_sItemMaxHp = m_sItemMaxMp = 0;
	m_sItemWeight = 0;
	m_sItemAc = 0;

	m_sExpGainAmount = m_bNPGainAmount = m_bNoahGainAmount = 100;
	m_bItemExpGainAmount = m_bItemNoahGainAmount = 0;
	m_bItemNPBonus = m_bSkillNPBonus = 0;

	m_byAPBonusAmount = 0;
	memset(&m_byAPClassBonusAmount, 0, sizeof(m_byAPClassBonusAmount));
	memset(&m_byAcClassBonusAmount, 0, sizeof(m_byAcClassBonusAmount));

	memset(&m_bStats, 0, sizeof(m_bStats));
	memset(&m_sStatItemBonuses, 0, sizeof(m_sStatItemBonuses));
	memset(&m_bStatBuffs, 0, sizeof(m_bStatBuffs));
	memset(&m_bstrSkill, 0, sizeof(m_bstrSkill));

	m_bPlayerAttackAmount = 100;

	m_bAddWeaponDamage = 0;
	m_bPctArmourAc = 100;
	m_sAddArmourAc = 0;

	m_sItemHitrate = 100;
	m_sItemEvasionrate = 100;

	m_sSpeed = 0;

	m_bAuthority = AUTHORITY_PLAYER;
	m_bLevel = 1;
	m_iExp = 0;
	m_iBank = m_iGold = 0;
	m_iLoyalty = m_iLoyaltyMonthly = 0;
	m_iMannerPoint = 0;
	m_sHp = m_sMp = m_sSp = 0;

	m_iMaxHp = 0;
	m_iMaxMp = 1;
	m_iMaxExp = 0;
	m_sMaxWeight = 0;
	m_sMaxWeightBonus = 0;

	m_bResHpType = USER_STANDING;
	m_bWarp = false;

	m_sMerchantsSocketID = -1;
	m_sChallengeUser = -1;
	m_sPartyIndex = -1;		
	m_sExchangeUser = -1;
	m_bRequestingChallenge = 0;
	m_bChallengeRequested = 0;
	m_bExchangeOK = 0x00;
	m_bBlockPrivateChat = false;
	m_sPrivateChatUser = -1;
	m_bNeedParty = 0x01;

	m_tHPLastTimeNormal = 0;		// For Automatic HP recovery. 
	m_tHPStartTimeNormal = 0;
	m_bHPAmountNormal = 0;
	m_bHPDurationNormal = 0;
	m_bHPIntervalNormal = 5;

	m_tGameStartTimeSavedMagic = 0;

	m_fSpeedHackClientTime = 0;
	m_fSpeedHackServerTime = 0;
	m_bSpeedHackCheck = 0;

	m_tBlinkExpiryTime = 0;

	m_bAbnormalType = ABNORMAL_NORMAL;	// User starts out in normal size.
	m_nOldAbnormalType = m_bAbnormalType;

	m_sWhoKilledMe = -1;
	m_iLostExp = 0;

	m_tLastTrapAreaTime = 0;

	memset(m_iSelMsgEvent, -1,  MAX_MESSAGE_EVENT);

	m_sEventNid = m_sEventSid = -1;
	m_nQuestHelperID = 0;
	m_bZoneChangeFlag = false;
	m_bRegeneType = 0;
	m_tLastRegeneTime = 0;
	m_bZoneChangeSameZone = false;

	m_transformationType = TransformationNone;
	m_sTransformID = 0;
	m_tTransformationStartTime = 0;
	m_sTransformationDuration = 0;

	memset(&m_bKillCounts, 0, sizeof(m_bKillCounts));
	m_sEventDataIndex = 0;

	m_pKnightsUser = nullptr;

	m_sRivalID = -1;
	m_tRivalExpiryTime = 0;

	m_byAngerGauge = 0;

	m_bWeaponsDisabled = false;

	m_teamColour = TeamColourNone;
	m_iLoyaltyDaily = 0;
	m_iLoyaltyPremiumBonus = 0;
	m_KillCount = 0;
	m_DeathCount = 0;

	m_LastX = 0;
	m_LastZ = 0;
}

/**
* @brief	Executes the disconnect action.
*/
void CUser::OnDisconnect()
{
	KOSocket::OnDisconnect();

	g_pMain->RemoveSessionNames(this);

	if (isInGame())
	{
		UserInOut(INOUT_OUT);

		if (isInParty())
			if (isPartyLeader())
			{
			_PARTY_GROUP * pParty = g_pMain->GetPartyPtr(GetPartyID());
			PartyPromote(pParty->uid[1]);
			}
			PartyRemove(GetSocketID());

		if (isInClan())
		{
			CKnights *pKnights = g_pMain->GetClanPtr(GetClanID());
			_KNIGHTS_ALLIANCE *pAllianceKnights = g_pMain->GetAlliancePtr(pKnights->GetAllianceID());
			if (pAllianceKnights != nullptr)
				pKnights->OnLogoutAlliance(this);
			else if (pKnights != nullptr)
				pKnights->OnLogout(this);
		}

		ResetWindows();

		if (hasRival())
			RemoveRival();
	}
	LogOut();
}

/**
* @brief	Handles an incoming user packet.
*
* @param	pkt	The packet.
*
* @return	true if it succeeds, false if it fails.
*/
bool CUser::HandlePacket(Packet & pkt)
{
	uint8 command = pkt.GetOpcode();
	TRACE("[SID=%d] Packet: %X (len=%d)\n", GetSocketID(), command, pkt.size());

	// If crypto's not been enabled yet, force the version packet to be sent.
	//if (!isCryptoEnabled())
	if(!m_bHasCheckedClientVersion)
	{
		if (command == WIZ_VERSION_CHECK)
			VersionCheck(pkt);

		m_bHasCheckedClientVersion = true;
		return true;
	}
	// If we're not authed yet, forced us to before we can do anything else.
	// NOTE: We're checking the account ID store here because it's only set on successful auth,
	// at which time the other account ID will be cleared out (yes, it's messy -- need to clean it up).
	else if (m_strAccountID.empty())
	{
		if (command == WIZ_LOGIN)
			LoginProcess(pkt);

		return true;
	}
	// If we haven't logged in yet, don't let us hit in-game packets.
	// TODO: Make sure we support all packets in the loading stage (and rewrite this logic considerably better).
	else if (!m_bSelectedCharacter)
	{
		switch (command)
		{
		case WIZ_SEL_NATION:
			SelNationToAgent(pkt);
			break;
		case WIZ_ALLCHAR_INFO_REQ:
			AllCharInfoToAgent();
			break;
		case WIZ_CHANGE_HAIR:
			ChangeHair(pkt);
			break;
		case WIZ_NEW_CHAR:
			NewCharToAgent(pkt);
			break;
		case WIZ_DEL_CHAR:
			DelCharToAgent(pkt);
			break;
		case WIZ_SEL_CHAR:
			SelCharToAgent(pkt);
			break;
		case WIZ_SPEEDHACK_CHECK:
			SpeedHackTime(pkt);
			break;
		default:
			TRACE("[SID=%d] Unhandled packet (%X) prior to selecting character\n", GetSocketID(), command);
			break;
		}
		return true;
	}

	// Otherwise, assume we're authed & in-game.
	switch (command)
	{
	case WIZ_GAMESTART:
		GameStart(pkt);
		break;
	case WIZ_SERVER_INDEX:
		SendServerIndex();
		break;
	case WIZ_RENTAL:
		RentalSystem(pkt);
		break;
	case WIZ_SKILLDATA:
		SkillDataProcess(pkt);
		break;
	case WIZ_MOVE:
		MoveProcess(pkt);
		break;
	case WIZ_ROTATE:
		Rotate(pkt);
		break;
	case WIZ_ATTACK:
		Attack(pkt);
		break;
	case WIZ_CHAT:
		Chat(pkt);
		break;
	case WIZ_CHAT_TARGET:
		ChatTargetSelect(pkt);
		break;
	case WIZ_REGENE:	
		Regene(pkt.read<uint8>()); // respawn type
		break;
	case WIZ_REQ_USERIN:
		RequestUserIn(pkt);
		break;
	case WIZ_REQ_NPCIN:
		RequestNpcIn(pkt);
		break;
	case WIZ_WARP:
		if (isGM())
			RecvWarp(pkt);
		break;
	case WIZ_ITEM_MOVE:
		ItemMove(pkt);
		break;
	case WIZ_NPC_EVENT:
		NpcEvent(pkt);
		break;
	case WIZ_ITEM_TRADE:
		ItemTrade(pkt);
		break;
	case WIZ_TARGET_HP:
		{
			uint16 uid = pkt.read<uint16>();
			uint8 echo = pkt.read<uint8>();
			m_targetID = uid;
			SendTargetHP(echo, uid);
		}
		break;
	case WIZ_BUNDLE_OPEN_REQ:
		BundleOpenReq(pkt);
		break;
	case WIZ_ITEM_GET:
		ItemGet(pkt);
		break;
	case WIZ_ZONE_CHANGE:
		RecvZoneChange(pkt);
		break;
	case WIZ_POINT_CHANGE:
		PointChange(pkt);
		break;
	case WIZ_STATE_CHANGE:
		StateChange(pkt);
		break;
	case WIZ_PARTY:
		PartyProcess(pkt);
		break;
	case WIZ_EXCHANGE:
		ExchangeProcess(pkt);
		break;
	case WIZ_QUEST:
		QuestV2PacketProcess(pkt);
		break;
	case WIZ_MERCHANT:
		MerchantProcess(pkt);
		break;
	case WIZ_MAGIC_PROCESS:
		CMagicProcess::MagicPacket(pkt, this);
		break;
	case WIZ_SKILLPT_CHANGE:
		SkillPointChange(pkt);
		break;
	case WIZ_OBJECT_EVENT:
		ObjectEvent(pkt);
		break;
	case WIZ_WEATHER:
	case WIZ_TIME:
		UpdateGameWeather(pkt);
		break;
	case WIZ_CLASS_CHANGE:
		ClassChange(pkt);
		break;
	case WIZ_CONCURRENTUSER:
		CountConcurrentUser();
		break;
	case WIZ_DATASAVE:
		UserDataSaveToAgent();
		break;
	case WIZ_ITEM_REPAIR:
		ItemRepair(pkt);
		break;
	case WIZ_KNIGHTS_PROCESS:
		CKnightsManager::PacketProcess(this, pkt);
		break;
	case WIZ_ITEM_REMOVE:
		ItemRemove(pkt);
		break;
	case WIZ_OPERATOR:
		OperatorCommand(pkt);
		break;
	case WIZ_SPEEDHACK_CHECK:
		SpeedHackTime(pkt);
		break;
	case WIZ_WAREHOUSE:
		WarehouseProcess(pkt);
		break;
	case WIZ_HOME:
		Home();
		break; 
	case WIZ_FRIEND_PROCESS:
		FriendProcess(pkt);
		break;
	case WIZ_WARP_LIST:
		SelectWarpList(pkt);
		break;
	case WIZ_VIRTUAL_SERVER:
		ServerChangeOk(pkt);
		break;
	case WIZ_PARTY_BBS:
		PartyBBS(pkt);
		break;
	case WIZ_MAP_EVENT:
		break;
	case WIZ_CLIENT_EVENT:
		ClientEvent(pkt.read<uint16>());
		break;
	case WIZ_SELECT_MSG:
		RecvSelectMsg(pkt);
		break;
	case WIZ_ITEM_UPGRADE:
		ItemUpgradeProcess(pkt);
		break;
	case WIZ_EVENT:
		TempleProcess(pkt);
		break;
	case WIZ_SHOPPING_MALL: // letter system's used in here too
		ShoppingMall(pkt);
		break;
	case WIZ_NAME_CHANGE:
		HandleNameChange(pkt);
		break;
	case WIZ_KING:
		CKingSystem::PacketProcess(this, pkt);
		break;
	case WIZ_HELMET:
		HandleHelmet(pkt);
		break;
	case WIZ_CAPE:
		HandleCapeChange(pkt);
		break;
	case WIZ_CHALLENGE:
		HandleChallenge(pkt);
		break;
	case WIZ_RANK:
		HandlePlayerRankings(pkt);
		break;
	case WIZ_MINING:
		HandleMiningSystem(pkt);
		break;
	case WIZ_SIEGE:
		SiegeWarFareNpc(pkt);
		break;
	case WIZ_LOGOSSHOUT:
		LogosShout(pkt);

	default:
		TRACE("[SID=%d] Unknown packet %X\n", GetSocketID(), command);
		return false;
	}

	Update();
	return true;
}

/**
* @brief	Updates timed player data, e.g. skills & save requests.
*/
void CUser::Update()
{
	if (m_tGameStartTimeSavedMagic != 0 && (UNIXTIME - m_tGameStartTimeSavedMagic) >= 2)
	{
		m_tGameStartTimeSavedMagic = 0;
		// Restore scrolls...
		InitType4();
		RecastSavedMagic();
	}

	if (!isBlinking() && m_tHPLastTimeNormal != 0 && (UNIXTIME - m_tHPLastTimeNormal) > m_bHPIntervalNormal)
		HPTimeChange();	// For Sitdown/Standup HP restoration.

	// Handles DOT/HOT skills (not COLD skills though.)
	if (m_bType3Flag) 
		HPTimeChangeType3();	

	// Check for expired type 4 buffs
	Type4Duration();

	// Expire any timed out saved skills.
	CheckSavedMagic();

	if (isTransformed())
		CMagicProcess::CheckExpiredType6Skills(this);

	// Check for expired type 9/visibility skills
	CMagicProcess::CheckExpiredType9Skills(this);

	if (isBlinking())		// Should you stop blinking?
		BlinkTimeCheck();

	if (hasRival() && hasRivalryExpired())
		RemoveRival();

	if ((UNIXTIME - m_lastSaveTime) >= PLAYER_SAVE_INTERVAL)
	{
		m_lastSaveTime = UNIXTIME; // this is set by UpdateUser(), however it may result in multiple requests unless it's set first.
		UserDataSaveToAgent();
	}
		for (int i = 0; i < WAREHOUSE_MAX; i++)
		{
			_ITEM_DATA *pItem = &m_sWarehouseArray[i];

			if(pItem == nullptr)
			continue; 

			if (pItem->nExpirationTime < (uint32)UNIXTIME && pItem->nExpirationTime != 0)
				memset(pItem, 0, sizeof(_ITEM_DATA));
		}

		for (int i = 0; i < INVENTORY_TOTAL; i++)
		{
			_ITEM_DATA *pItem = &m_sItemArray[i];

			if(pItem == nullptr)
				continue; 

			if (pItem->nExpirationTime < (uint32)UNIXTIME && pItem->nExpirationTime != 0)
				memset(pItem, 0, sizeof(_ITEM_DATA));
		}
}

void CUser::SetRival(CUser * pRival)
{
	if (pRival == nullptr
		|| hasRival())
		return;

	Packet result(WIZ_PVP, uint8(PVPAssignRival));
	CKnights * pKnights = nullptr;

	result	<< pRival->GetID()
		<< GetCoins() << GetLoyalty();

	if (pRival->isInClan() 
		&& (pKnights = g_pMain->GetClanPtr(pRival->GetClanID())))
		result << pKnights->GetName();
	else
		result << uint16(0); // 0 length clan name;

	result << pRival->GetName();

	m_sRivalID = pRival->GetID();
	m_tRivalExpiryTime = UNIXTIME + RIVALRY_DURATION;

	Send(&result);
}

/**
* @brief	Removes our rivalry state.
*/
void CUser::RemoveRival()
{
	if (!hasRival())
		return;

	// Reset our rival data
	m_tRivalExpiryTime = 0;
	m_sRivalID = -1;

	// Send the packet to let the client know that our rivalry has ended
	Packet result(WIZ_PVP, uint8(PVPRemoveRival));
	Send(&result);
}

/**
* @brief	Adjusts a player's loyalty (NP) and sends the loyalty 
* 			change packet.
*
* @param	nChangeAmount	The amount to adjust the loyalty points by.
* @param	bIsKillReward	When set to true, enables the use of NP-modifying buffs
*							and includes monthly NP gains.
*/
void CUser::SendLoyaltyChange(int32 nChangeAmount /*= 0*/, bool bIsKillReward /* false */, bool bIsBonusReward /* false */, bool bIsAddLoyaltyMonthly /* true */)
{
	Packet result(WIZ_LOYALTY_CHANGE, uint8(LOYALTY_NATIONAL_POINTS));
	uint32 nClanLoyaltyAmount = 0;

	int32 nChangeAmountLoyaltyMonthly = nChangeAmount;

	// If we're taking NP, we need to prevent us from hitting values below 0.
	if (nChangeAmount < 0)
	{
		// Negate the value so it becomes positive (i.e. -50 -> 50)
		// so we can determine if we're trying to take more NP than we have.
		uint32 amt = -nChangeAmount; /* avoids unsigned/signed comparison warning */

		if (amt > m_iLoyalty)
			m_iLoyalty = 0;
		else
			m_iLoyalty += nChangeAmount;

		// We should only adjust monthly NP when NP was lost when killing a player.
		if (bIsKillReward)
		{
			if (GetZoneID() == ZONE_ARDREAM || GetZoneID() == ZONE_RONARK_LAND_BASE)
				bIsAddLoyaltyMonthly = false;

			if (bIsAddLoyaltyMonthly)
			{
				if (nChangeAmountLoyaltyMonthly > 40)
					nChangeAmountLoyaltyMonthly += 20;
				else if (nChangeAmountLoyaltyMonthly >= 20 && nChangeAmountLoyaltyMonthly < 40)
					nChangeAmountLoyaltyMonthly += 10;

				if (amt > m_iLoyaltyMonthly)
					m_iLoyaltyMonthly = 0;
				else 
					m_iLoyaltyMonthly += nChangeAmountLoyaltyMonthly;
			}
		}
	}
	// We're simply adding NP here.
	else
	{
		// If you're using an NP modifying buff then add the bonus
		nChangeAmount = m_bNPGainAmount * nChangeAmount / 100;

		// Add on any additional NP earned because of a global NP event.
		// NOTE: They officially check to see if the NP is <= 100,000.
		nChangeAmount = nChangeAmount * (100 + g_pMain->m_byNPEventAmount) / 100;

		// We should only apply NP bonuses when NP was gained as a reward for killing a player.
		if (bIsKillReward)
		{
			// Add on any additional NP gained from items/skills.
			nChangeAmount += m_bItemNPBonus + m_bSkillNPBonus;

			// Add monument bonus.
			if (isInPKZone() && GetPVPMonumentNation() == GetNation())
				nChangeAmount += PVP_MONUMENT_NP_BONUS;
		}

		if (m_iLoyalty + nChangeAmount > LOYALTY_MAX)
			m_iLoyalty = LOYALTY_MAX;
		else
			m_iLoyalty += nChangeAmount;

		if (isInPKZone() && !bIsBonusReward)
		{
			if (GetZoneID() == ZONE_ARDREAM || GetZoneID() == ZONE_RONARK_LAND_BASE)
				bIsAddLoyaltyMonthly = false;

			m_iLoyaltyDaily += nChangeAmount;
			UpdatePlayerRank();
		}

		//// We should only apply additional monthly NP when NP was gained as a reward for killing a player.
		if (!bIsBonusReward)
		{
			if (bIsAddLoyaltyMonthly)
			{
				if (nChangeAmountLoyaltyMonthly > 40)
					nChangeAmountLoyaltyMonthly -= 20;
				else if (nChangeAmountLoyaltyMonthly >= 20 && nChangeAmountLoyaltyMonthly < 40)
					nChangeAmountLoyaltyMonthly -= 10;

				if (m_iLoyaltyMonthly + nChangeAmountLoyaltyMonthly > LOYALTY_MAX)
					m_iLoyaltyMonthly = LOYALTY_MAX;
				else
					m_iLoyaltyMonthly += nChangeAmountLoyaltyMonthly;
			}
		}

		if (bIsKillReward)
		{
			if (GetPremiumProperty(PremiumBonusLoyalty) > 0)
			{
				m_iLoyalty += GetPremiumProperty(PremiumBonusLoyalty);

				if (bIsAddLoyaltyMonthly)
					m_iLoyaltyMonthly += GetPremiumProperty(PremiumBonusLoyalty);

				m_iLoyaltyPremiumBonus += GetPremiumProperty(PremiumBonusLoyalty);
			}
		}

		CKnights * pKnights = g_pMain->GetClanPtr(GetClanID());

		if (pKnights && pKnights->m_byFlag >= ClanTypeAccredited5 && pKnights->GetClanPointMethod() == 0 && !bIsBonusReward)
		{
			if (pKnights->m_sMembers > 0 && pKnights->m_sMembers <= MAX_CLAN_USERS)
			{
				if (pKnights->m_sMembers <= 5)
					nClanLoyaltyAmount = 1;
				else if (pKnights->m_sMembers <= 10)
					nClanLoyaltyAmount = 2;
				else if (pKnights->m_sMembers <= 15)
					nClanLoyaltyAmount = 3;
				else if (pKnights->m_sMembers <= 20)
					nClanLoyaltyAmount = 4;
				else if (pKnights->m_sMembers <= 25)
					nClanLoyaltyAmount = 5;
				else if (pKnights->m_sMembers <= 30)
					nClanLoyaltyAmount = 6;
				else if (pKnights->m_sMembers > 30)
					nClanLoyaltyAmount = 7;

				m_iLoyalty -= nClanLoyaltyAmount;
				CKnightsManager::AddUserDonatedNP(GetClanID(), m_strUserID, nClanLoyaltyAmount, true);
			}
		}
	}

	result << m_iLoyalty << m_iLoyaltyMonthly
		<< uint32(0) // Clan donations(? Donations made by this user? For the clan overall?)
		<< nClanLoyaltyAmount; // Premium NP(? Additional NP gained?)

	Send(&result);

	// Player is give first np, second exp and third meat dumpling etc.
	if (bIsKillReward && nChangeAmount > 0)
	{
		if (isInPKZone() || GetMap()->isWarZone())
			ExpChange(PVP_BONUS_EXP, true);

		// Additionally, we should receive a "Meat dumpling"
		if (isInPKZone())
			GiveItem(ITEM_MEAT_DUMPLING);
	}
}

/**
* @brief	Get a player loyalty reward.
*
* @param	isMonthly	Monthly reward.
*/
uint8 CUser::GetRankReward(bool isMonthly)
{
	enum RankErrorCodes
	{
		NoRank				= 0,
		RewardSuccessfull	= 1,
		RewardAlreadyTaken	= 2
	};

	int8 nRank = -1;
	int32 nGoldAmount = 0;

	Guard lock(g_pMain->m_userRankingsLock);

	string strUserID = GetName();
	STRTOUPPER(strUserID);

	UserNameRankMap::iterator itr;

	if (isMonthly)
	{
		itr = g_pMain->m_UserPersonalRankMap.find(strUserID);
		nRank = itr != g_pMain->m_UserPersonalRankMap.end() ? int8(itr->second->nRank) : -1;
	} else {
		itr = g_pMain->m_UserKnightsRankMap.find(strUserID);
		nRank = itr != g_pMain->m_UserKnightsRankMap.end() ? int8(itr->second->nRank) : -1;
	}

	nRank = 1;

	if (nRank > 0 && nRank <= 100)
	{
		if (nRank == 1)
			nGoldAmount = 1000000;
		else if (nRank >= 2 && nRank <= 4)
			nGoldAmount = 700000;
		else if (nRank >= 5 && nRank <= 10)
			nGoldAmount = 500000;
		else if (nRank >= 11 && nRank <= 40)
			nGoldAmount = 300000;
		else if (nRank >= 41 && nRank <= 100)
			nGoldAmount = 200000;
		else
			nGoldAmount = 0;

		if (nGoldAmount > 0) 
		{
			if (GetUserDailyOp(isMonthly ? DAILY_USER_PERSONAL_RANK_REWARD : DAILY_USER_RANK_REWARD) == 0)
				return RewardAlreadyTaken;

			GoldGain(nGoldAmount);
			return RewardSuccessfull;

		}
	}

	return NoRank;
}

/**
* @brief	Changes a player's fame.
*
* @param	bFame	The fame.
*/
void CUser::ChangeFame(uint8 bFame)
{
	Packet result(WIZ_AUTHORITY_CHANGE, uint8(COMMAND_AUTHORITY));

	m_bFame = bFame;
	result << GetSocketID() << GetFame();
	SendToRegion(&result);
}

/**
* @brief	Sends the server index.
*/
void CUser::SendServerIndex()
{
	Packet result(WIZ_SERVER_INDEX);
	result << uint16(1) << uint16(g_pMain->m_nServerNo);
	Send(&result);
}

/**
* @brief	Packet handler for skillbar requests.
*
* @param	pkt	The packet.
*/
void CUser::SkillDataProcess(Packet & pkt)
{
	uint8 opcode = pkt.read<uint8>();
	switch (opcode)
	{
	case SKILL_DATA_SAVE:
		SkillDataSave(pkt);
		break;

	case SKILL_DATA_LOAD:
		SkillDataLoad();
		break;
	}
}

/**
* @brief	Packet handler for saving a skillbar.
*
* @param	pkt	The packet.
*/
void CUser::SkillDataSave(Packet & pkt)
{
	Packet result(WIZ_SKILLDATA, uint8(SKILL_DATA_SAVE));
	uint16 sCount = pkt.read<uint16>();
	if (sCount == 0 || sCount > 64)
		return;

	result	<< sCount;
	for (int i = 0; i < sCount; i++)
		result << pkt.read<uint32>();

	g_pMain->AddDatabaseRequest(result, this);
}

/**
* @brief	Packet handler for loading a skillbar.
*/
void CUser::SkillDataLoad()
{
	Packet result(WIZ_SKILLDATA, uint8(SKILL_DATA_LOAD));
	g_pMain->AddDatabaseRequest(result, this);
}

/**
* @brief	Initiates a database request to save the character's information.
*/
void CUser::UserDataSaveToAgent()
{
	if (!isInGame())
		return;

	Packet result(WIZ_DATASAVE);
	result << GetAccountName() << GetName();
	g_pMain->AddDatabaseRequest(result, this);
}

/**
* @brief	Logs a player out.
*/
void CUser::LogOut()
{
	if (m_strUserID.empty()) 
		return; 

	Packet result(AG_USER_LOG_OUT);
	result << GetID() << GetName();
	Send_AIServer(&result);

	result.Initialize(WIZ_LOGOUT);
	m_deleted = true; // make this session unusable until the logout is complete
	g_pMain->AddDatabaseRequest(result, this);
}

/**
* @brief	Sends the player's information on initial login.
*/
void CUser::SendMyInfo()
{
	C3DMap* pMap = GetMap();
	CKnights* pKnights = nullptr;

	if (!pMap->IsValidPosition(GetX(), GetZ(), 0.0f))
	{
		short x = 0, z = 0;
		GetStartPosition(x, z); 

		m_curx = (float)x;
		m_curz = (float)z;
	}

	QuestDataRequest();

	Packet result(WIZ_MYINFO);

	// Load up our user rankings (for our NP symbols).
	g_pMain->GetUserRank(this);

	// Are we the King? Let's see, shall we?
	CKingSystem * pData = g_pMain->m_KingSystemArray.GetData(GetNation());
	if (pData != nullptr
		&& STRCASECMP(pData->m_strKingName.c_str(), m_strUserID.c_str()) == 0)
		m_bRank = 1; // We're da King, man.
	else
		m_bRank = 0; // totally not da King.

	//result.SByte(); // character name has a single byte length
	result.DByte(); // character name has a single byte length
	result	<< GetSocketID()
		<< GetName()
		<< GetSPosX() << GetSPosZ() << GetSPosY()
		<< GetNation() 
		<< m_bRace << m_sClass << m_bFace
		<< m_nHair
		<< m_bRank << m_bTitle
		<< GetLevel()
		<< int8(m_sPoints) // NOTE: int16 to int8
		<< uint32(m_iMaxExp) << uint32(m_iExp)
		<< GetLoyalty() << GetMonthlyLoyalty()
		<< m_bCity << GetClanID() ;

	if (isInClan())
		pKnights = g_pMain->GetClanPtr(GetClanID());

	if (pKnights == nullptr)
	{
		result	<< uint64(0) << uint8(0) << uint16(-1);
	}
	else 
	{
		if (pKnights->m_sAlliance != 0)
		{
		pKnights->OnLoginAlliance(this);
		}
		else
		pKnights->OnLogin(this);

		CKnights *aKnights = g_pMain->GetClanPtr(pKnights->GetAllianceID());

		result	
			<< GetFame()
			<< pKnights->GetAllianceID()
			<< pKnights->m_byFlag
			<< pKnights->m_strName
			<< pKnights->m_byGrade << pKnights->m_byRanking
			<< uint16(pKnights->m_sMarkVersion)
			<< pKnights->GetCapeID(aKnights);
	}

	result	
		<< m_iMaxHp << m_sHp
		<< m_iMaxMp << m_sMp
		<< MaxWeight(m_sMaxWeight) << m_sItemWeight
		<< GetStat(STAT_STR) << uint8(GetStatItemBonus(STAT_STR))
		<< GetStat(STAT_STA) << uint8(GetStatItemBonus(STAT_STA))
		<< GetStat(STAT_DEX) << uint8(GetStatItemBonus(STAT_DEX))
		<< GetStat(STAT_INT) << uint8(GetStatItemBonus(STAT_INT))
		<< GetStat(STAT_CHA) << uint8(GetStatItemBonus(STAT_CHA))
		<< m_sTotalHit << m_sTotalAc
		<< uint8(m_sFireR) << uint8(m_sColdR) << uint8(m_sLightningR)
		<< uint8(m_sMagicR) << uint8(m_sDiseaseR) << uint8(m_sPoisonR)
		<< m_iGold
		<< m_bAuthority
		<< m_bKnightsRank << m_bPersonalRank; // national rank, leader rank

	result.append(m_bstrSkill, 9);

	for (int i = 0; i < SLOT_MAX; i++)
	{
		_ITEM_DATA *pItem = GetItem(i); 
		result	<< pItem->nNum
			<< pItem->sDuration << pItem->sCount
			<< pItem->bFlag	// item type flag (e.g. rented)
			<< pItem->sRemainingRentalTime;	// remaining time

			// NOTE: gone from 1298
			//<< uint32(0) // unknown
			//<< pItem->nExpirationTime; // expiration date in unix time
	}
	
	/*
	for (int i = 43; i <= 46; i+=3)
	{
		_ITEM_DATA * pItem = GetItem(i);
		result	
		<< pItem->nNum
		<< pItem->sDuration
		<< pItem->sCount
		<< pItem->bFlag // item type flag (e.g. rented)
		<< pItem->sRemainingRentalTime // remaining time
		<< pItem->nExpirationTime; // expiration date in unix time
	}
	*/
	
	for (int i = 0; i < HAVE_MAX; i++)
	{
		_ITEM_DATA *pItem = GetItem(i+SLOT_MAX); 
		result	<< pItem->nNum
			<< pItem->sDuration << pItem->sCount
			<< pItem->bFlag	// item type flag (e.g. rented)
			<< pItem->sRemainingRentalTime;	// remaining time

			// NOTE: gone from 1298
			//<< uint32(0) // unknown
			//<< pItem->nExpirationTime; // expiration date in unix time
	}

	m_bIsChicken = CheckExistEvent(50, 1);
	result
		//<< m_bAccountStatus	// account status (0 = none, 1 = normal prem with expiry in hours, 2 = pc room)
		//<< m_bPremiumType		// premium type (7 = platinum premium)
		//<< m_sPremiumTime		// premium time

		<< uint8(0x00) << uint8(0x00) << uint16(0x00)
		<< m_bIsChicken						// chicken/beginner flag
		<< m_iMannerPoint;
		//<< uint8(0x00); // extra byte?


	Send(&result);

	g_pMain->AddCharacterName(this);

	SetZoneAbilityChange(GetZoneID());
	//SendPremiumInfo(); // NOTE: 1298 thing - seen with packet sniff. and may not be within WIZ_MYINFO
	Send2AI_UserUpdateInfo(true); 
}

uint16 CUser::MaxWeight (uint16 MaxWeight)
{
	if(MaxWeight >= 32767)
		return m_sMaxWeight = 32767;
	else
		return MaxWeight;
} 

/**
* @brief	Calculates & sets a player's maximum HP.
*
* @param	iFlag	If set to 1, additionally resets the HP to max. 
* 					If set to 2, additionally resets the max HP to 100 (i.e. Snow war). 
*/
void CUser::SetMaxHp(int iFlag)
{
	_CLASS_COEFFICIENT* p_TableCoefficient = nullptr;
	p_TableCoefficient = g_pMain->m_CoefficientArray.GetData( m_sClass );
	if( !p_TableCoefficient ) return;

	int temp_sta = getStatTotal(STAT_STA);

	if (GetZoneID() == ZONE_SNOW_BATTLE && iFlag == 0)
		if(GetFame() == COMMAND_CAPTAIN || isKing())
		m_iMaxHp = 300;
		else
		m_iMaxHp = 100;

	else if (GetZoneID() == ZONE_CHAOS_DUNGEON && iFlag == 0)
		m_iMaxHp = 10000 / 10;
	else	
	{
		m_iMaxHp = (short)(((p_TableCoefficient->HP * GetLevel() * GetLevel() * temp_sta ) 
			+ 0.1 * (GetLevel() * temp_sta) + (temp_sta / 5)) + m_sMaxHPAmount + m_sItemMaxHp + 20);

		// A player's max HP should be capped at (currently) 14,000 HP.
		if (m_iMaxHp > MAX_PLAYER_HP && !isGM()) 
			m_iMaxHp = MAX_PLAYER_HP;

		if (iFlag == 1)
			m_sHp = m_iMaxHp;
		else if (iFlag == 2)
			m_iMaxHp = 100;
	}

	if(m_iMaxHp < m_sHp) {
		m_sHp = m_iMaxHp;
		HpChange( m_sHp );
	}
}

/**
* @brief	Calculates & sets a player's maximum MP.
*/
void CUser::SetMaxMp()
{
	_CLASS_COEFFICIENT* p_TableCoefficient = nullptr;
	p_TableCoefficient = g_pMain->m_CoefficientArray.GetData( m_sClass );
	if( !p_TableCoefficient ) return;

	int temp_intel = 0, temp_sta = 0;
	temp_intel = getStatTotal(STAT_INT) + 30;
	//	if( temp_intel > 255 ) temp_intel = 255;
	temp_sta = getStatTotal(STAT_STA);
	//	if( temp_sta > 255 ) temp_sta = 255;

	if( p_TableCoefficient->MP != 0)
	{
		m_iMaxMp = (short)((p_TableCoefficient->MP * GetLevel() * GetLevel() * temp_intel)
			+ (0.1f * GetLevel() * 2 * temp_intel) + (temp_intel / 5) + m_sMaxMPAmount + m_sItemMaxMp + 20);
	}
	else if( p_TableCoefficient->SP != 0)
	{
		m_iMaxMp = (short)((p_TableCoefficient->SP * GetLevel() * GetLevel() * temp_sta )
			+ (0.1f * GetLevel() * temp_sta) + (temp_sta / 5) + m_sMaxMPAmount + m_sItemMaxMp);
	}

	if(m_iMaxMp < m_sMp) {
		m_sMp = m_iMaxMp;
		MSpChange( m_sMp );
	}
}

/**
* @brief	Sends the server time.
*/
void CUser::SendTime()
{
	Packet result(WIZ_TIME);
	result	<< uint16(g_pMain->m_sYear) << uint16(g_pMain->m_sMonth) << uint16(g_pMain->m_sDate)
		<< uint16(g_pMain->m_sHour) << uint16(g_pMain->m_sMin);
	Send(&result);
}

/**
* @brief	Sends the weather status.
*/
void CUser::SendWeather()
{
	Packet result(WIZ_WEATHER);
	result << g_pMain->m_byWeather << g_pMain->m_sWeatherAmount;
	Send(&result);
}

/**
* @brief	Sets various zone flags to control how
* 			the client handles other players/NPCs.
* 			Also sends the zone's current tax rate.
*/
void CUser::SetZoneAbilityChange(uint16 sNewZone)
{
	C3DMap * pMap = g_pMain->GetZoneByID(sNewZone);
	_KNIGHTS_SIEGE_WARFARE *pSiegeWar = g_pMain->GetSiegeMasterKnightsPtr(1);
	CKingSystem *pKingSystem = g_pMain->m_KingSystemArray.GetData(GetNation());
	if (pMap == nullptr)
		return;

	if (pSiegeWar != nullptr && pKingSystem != nullptr)
	{
		switch (sNewZone)
		{
		case ZONE_KARUS:
		pMap->SetTariff(pKingSystem->m_nTerritoryTariff);
			break;
		case ZONE_ELMORAD:
		pMap->SetTariff(pKingSystem->m_nTerritoryTariff);
			break;
		case ZONE_MORADON:
		pMap->SetTariff((uint8)pSiegeWar->sMoradonTariff);
			break;
		case ZONE_DELOS:
		pMap->SetTariff((uint8)pSiegeWar->sDellosTariff);
			break;

		default:
			break;
		}
	}


	Packet result(WIZ_ZONEABILITY, uint8(1));

	result	<< pMap->canTradeWithOtherNation()
		<< pMap->GetZoneType()
		<< pMap->canTalkToOtherNation()
		<< uint16(pMap->GetTariff());

	Send(&result);

	if (!isGM())
		PlayerRankingProcess(sNewZone,false);

	g_pMain->KillNpc(GetSocketID());

	if (sNewZone == ZONE_BIFROST || sNewZone == ZONE_BATTLE4  || sNewZone ==  ZONE_RONARK_LAND)
		g_pMain->SendEventRemainingTime(false, this, (uint8)sNewZone);

	// Clear skill cooldowns...
	m_RHitRepeatList.clear();
	m_CoolDownList.clear();
	m_MagicTypeCooldownList.clear();
}

/**
* @brief	Sends the user's premium state.
*/
void CUser::SendPremiumInfo()
{
	Packet result(WIZ_PREMIUM, m_bAccountStatus);
	result << m_bPremiumType << uint32(m_sPremiumTime); 
	Send(&result);
}

/**
* @brief	Requests user info for the specified session IDs.
*
* @param	pkt	The packet.
*/
void CUser::RequestUserIn(Packet & pkt)
{
	Packet result(WIZ_REQ_USERIN);
	short user_count = pkt.read<uint16>(), online_count = 0;
	if (user_count > 1000)
		user_count = 1000;

	result << uint16(0); // placeholder for user count

	for (int i = 0; i < user_count; i++)
	{
		CUser *pUser = g_pMain->GetUserPtr(pkt.read<uint16>());
		if (pUser == nullptr || !pUser->isInGame())
			continue;

		result << uint8(0) << pUser->GetSocketID();
		pUser->GetUserInfo(result);

		online_count++;
	}

	result.put(0, online_count); // substitute count in
	SendCompressed(&result);
}

/**
* @brief	Request NPC info for the specified NPC IDs.
*
* @param	pkt	The packet.
*/
void CUser::RequestNpcIn(Packet & pkt)
{
	if (g_pMain->m_bPointCheckFlag == false)
		return;

	Packet result(WIZ_REQ_NPCIN);
	uint16 npc_count = pkt.read<uint16>();
	if (npc_count > 1000)
		npc_count = 1000;

	result << uint16(0); // NPC count placeholder

	CKnights *pKnights = g_pMain->GetClanPtr(m_bKnights);
	_KNIGHTS_SIEGE_WARFARE *pSiegeWars = g_pMain->GetSiegeMasterKnightsPtr(1);

	for (int i = 0; i < npc_count; i++)
	{
		uint16 nid = pkt.read<uint16>();
		if (nid < 0 || nid > NPC_BAND+NPC_BAND)
			continue;

		CNpc *pNpc = g_pMain->GetNpcPtr(nid);
		if (pNpc == nullptr || pNpc->isDead())
			continue;

		if (pNpc->m_sSid == 541 && pNpc->GetType() == NPC_DESTROYED_ARTIFACT && pNpc->m_bZone == ZONE_DELOS)
		if ( pKnights != nullptr && pSiegeWars != nullptr)
		{
			if (pSiegeWars->sMasterKnights == pKnights->m_sAlliance || pSiegeWars->sMasterKnights == pKnights->m_sIndex)
					pNpc->m_bNation = 3;
			else
					pNpc->m_bNation = 0;
		}
		result << pNpc->GetID();
		pNpc->GetNpcInfo(result);
	}

	result.put(0, npc_count);
	SendCompressed(&result);
}

/**
* @brief	Calculates & resets item stats/bonuses.
*/
void CUser::SetSlotItemValue()
{
	_ITEM_TABLE* pTable = nullptr;
	int item_hit = 0, item_ac = 0;

	m_sItemMaxHp = m_sItemMaxMp = 0;
	m_sItemAc = 0; 
	m_sItemWeight = m_sMaxWeightBonus = 0;	
	m_sItemHitrate = m_sItemEvasionrate = 100; 

	memset(m_sStatItemBonuses, 0, sizeof(uint16) * STAT_COUNT);
	m_sFireR = m_sColdR = m_sLightningR = m_sMagicR = m_sDiseaseR = m_sPoisonR = 0;
	m_sDaggerR = m_sSwordR = m_sAxeR = m_sMaceR = m_sSpearR = m_sBowR = 0;

	m_byAPBonusAmount = 0;
	memset(&m_byAPClassBonusAmount, 0, sizeof(m_byAPClassBonusAmount));
	memset(&m_byAcClassBonusAmount, 0, sizeof(m_byAcClassBonusAmount));

	m_bItemExpGainAmount = m_bItemNPBonus = m_bItemNoahGainAmount = 0;

	Guard lock(m_equippedItemBonusLock);
	m_equippedItemBonuses.clear();

	map<uint16, uint32> setItems;

	// Apply stat bonuses from all equipped & cospre items.
	// Total up the weight of all items.
	for (int i = 0; i < INVENTORY_TOTAL; i++)
	{
		_ITEM_DATA * pItem = nullptr;
		pTable = GetItemPrototype(i, pItem);
		if (pTable == nullptr)
			continue;

		// Bags increase max weight, they do not weigh anything.
		if (i == INVENTORY_COSP + COSP_BAG1
			|| i == INVENTORY_COSP + COSP_BAG2)
		{
			m_sMaxWeightBonus += pTable->m_sDuration;
		}
		// All other items are attributed to the total weight of items in our inventory.
		else
		{
			// Non-stackable items should have a count of 1. If not, something's broken.
			m_sItemWeight += pTable->m_sWeight * pItem->sCount;
		}

		// Do not apply stats to unequipped items
		if ((i >= SLOT_MAX && i < INVENTORY_COSP)
			// or disabled weapons.
				|| (isWeaponsDisabled() 
				&& (i == RIGHTHAND || i == LEFTHAND) 
				&& !pTable->isShield())
				// or items in magic bags.
				|| i >= INVENTORY_MBAG
				|| pItem->isDuplicate())
				continue;

		item_ac = pTable->m_sAc;
		if (pItem->sDuration == 0) 
			item_ac /= 10;

		m_sItemMaxHp += pTable->m_MaxHpB;
		m_sItemMaxMp += pTable->m_MaxMpB;
		m_sItemAc += item_ac;
		m_sStatItemBonuses[STAT_STR] += pTable->m_sStrB;
		m_sStatItemBonuses[STAT_STA] += pTable->m_sStaB;
		m_sStatItemBonuses[STAT_DEX] += pTable->m_sDexB;
		m_sStatItemBonuses[STAT_INT] += pTable->m_sIntelB;
		m_sStatItemBonuses[STAT_CHA] += pTable->m_sChaB;
		m_sItemHitrate += pTable->m_sHitrate;
		m_sItemEvasionrate += pTable->m_sEvarate;

		m_sFireR += pTable->m_bFireR;
		m_sColdR += pTable->m_bColdR;
		m_sLightningR += pTable->m_bLightningR;
		m_sMagicR += pTable->m_bMagicR;
		m_sDiseaseR += pTable->m_bCurseR;
		m_sPoisonR += pTable->m_bPoisonR;

		m_sDaggerR += pTable->m_sDaggerAc;
		m_sSwordR += pTable->m_sSwordAc;
		m_sAxeR += pTable->m_sAxeAc;
		m_sMaceR += pTable->m_sMaceAc;
		m_sSpearR += pTable->m_sSpearAc;
		m_sBowR += pTable->m_sBowAc;

		ItemBonusMap bonusMap;
		if (pTable->m_bFireDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_FIRE, pTable->m_bFireDamage));

		if (pTable->m_bIceDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_COLD, pTable->m_bIceDamage));

		if (pTable->m_bLightningDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_LIGHTNING, pTable->m_bLightningDamage));

		if (pTable->m_bPoisonDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_POISON, pTable->m_bPoisonDamage));

		if (pTable->m_bHPDrain)
			bonusMap.insert(std::make_pair(ITEM_TYPE_HP_DRAIN, pTable->m_bHPDrain));

		if (pTable->m_bMPDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_MP_DAMAGE, pTable->m_bMPDamage));

		if (pTable->m_bMPDrain)
			bonusMap.insert(std::make_pair(ITEM_TYPE_MP_DRAIN, pTable->m_bMPDrain));

		if (pTable->m_bMirrorDamage)
			bonusMap.insert(std::make_pair(ITEM_TYPE_MIRROR_DAMAGE, pTable->m_bMirrorDamage));

		// If we have bonuses to apply, store them.
		if (!bonusMap.empty())
			m_equippedItemBonuses[i] = bonusMap;

		// Apply cospre item stats
		if (pTable->GetKind() == ITEM_KIND_COSPRE)
		{
			/*
			// If this item exists in the set table, it has bonuses to be applied.
			_SET_ITEM * pSetItem = g_pMain->m_SetItemArray.GetData(pTable->m_iNum);
			if (pSetItem != nullptr)
				ApplySetItemBonuses(pSetItem);
			*/
		}

		// All set items start with race over 100
		if (pTable->m_bRace < 100)
			continue;

		// Each set is uniquely identified by item's race
		auto itr = setItems.find(pTable->m_bRace);

		// If the item doesn't exist in our map yet...
		if (itr == setItems.end())
		{
			// Generate the base set ID and insert it into our map
			setItems.insert(make_pair(pTable->m_bRace, pTable->m_bRace * 10000));
			itr = setItems.find(pTable->m_bRace);
		}

		// Update the final set ID depending on the equipped set item 
		switch (pTable->m_bSlot)
		{
		case ItemSlotHelmet:
			itr->second += 2;
			break;
		case ItemSlotPauldron:
			itr->second += 16;
			break;
		case ItemSlotPads:
			itr->second += 512;
			break;
		case ItemSlotGloves:
			itr->second += 2048;
			break;
		case ItemSlotBoots:
			itr->second += 4096;
			break;
		}
	}
	/*
	// Now we can add up all the set bonuses, if any.
	foreach (itr, setItems)
	{
		// Test if this set item exists (if we're not using at least 2 items from the set, this will fail)
		_SET_ITEM * pItem = g_pMain->m_SetItemArray.GetData(itr->second);
		if (pItem == nullptr)
			continue;

		ApplySetItemBonuses(pItem);
	}
	*/
	if (m_sAddArmourAc > 0)
		m_sItemAc += m_sAddArmourAc;
	else
		m_sItemAc = m_sItemAc * m_bPctArmourAc / 100;
}

void CUser::ApplySetItemBonuses(_SET_ITEM * pItem)
{
	return;

	m_sItemAc += pItem->ACBonus;
	m_sItemMaxHp += pItem->HPBonus;
	m_sItemMaxMp += pItem->MPBonus;

	m_sStatItemBonuses[STAT_STR] += pItem->StrengthBonus;
	m_sStatItemBonuses[STAT_STA] += pItem->StaminaBonus;
	m_sStatItemBonuses[STAT_DEX] += pItem->DexterityBonus;
	m_sStatItemBonuses[STAT_INT] += pItem->IntelBonus;
	m_sStatItemBonuses[STAT_CHA] += pItem->CharismaBonus;

	m_sFireR += pItem->FlameResistance;
	m_sColdR += pItem->GlacierResistance;
	m_sLightningR += pItem->LightningResistance;
	m_sMagicR += pItem->MagicResistance;
	m_sDiseaseR += pItem->CurseResistance;
	m_sPoisonR += pItem->PoisonResistance;

	m_bItemExpGainAmount += pItem->XPBonusPercent;
	m_bItemNoahGainAmount += pItem->CoinBonusPercent;
	m_bItemNPBonus += pItem->NPBonus;

	m_sMaxWeightBonus += pItem->MaxWeightBonus;

	// NOTE: The following percentages use values such as 3 to indicate +3% (not the typical 103%).
	// Also note that at this time, there are no negative values used, so we can assume it's always a bonus.
	m_byAPBonusAmount += pItem->APBonusPercent; 
	if (pItem->APBonusClassType >= 1 && pItem->APBonusClassType <= 4)
		m_byAPClassBonusAmount[pItem->APBonusClassType - 1] += pItem->APBonusClassPercent;

	if (pItem->ACBonusClassType >= 1 && pItem->ACBonusClassType <= 4)
		m_byAcClassBonusAmount[pItem->ACBonusClassType - 1] += pItem->ACBonusClassPercent;
}

void CUser::RecvUserExp(Packet & pkt)
{
	CNpc * pNpc;
	_PARTY_GROUP * pParty;
	uint16 sNpcID;
	int32 iDamage, iTotalDamage, iNpcExp, iNpcLoyalty;
	uint32 nFinalExp, nFinalLoyalty;
	double TempValue = 0;

	pkt >> sNpcID >> iDamage >> iTotalDamage >> iNpcExp >> iNpcLoyalty;

	pNpc = g_pMain->GetNpcPtr(sNpcID);
	if (pNpc == nullptr
		|| !isInRangeSlow(pNpc, RANGE_50M)
		|| (iNpcExp <= 0 && iNpcLoyalty <= 0))
		return;

	// Calculate base XP earned for the damage dealt.
	if (iNpcExp > 0)
	{
		TempValue = iNpcExp * ((double)iDamage / (double)iTotalDamage);
		nFinalExp = (int) TempValue;
		if (TempValue > nFinalExp)
			nFinalExp++;
	}

	// Calculate base NP earned for the damage dealt.
	if (iNpcLoyalty > 0)
	{
		TempValue = iNpcLoyalty * ((double)iDamage / (double)iTotalDamage);
		nFinalLoyalty = (int) TempValue;
		if (TempValue > nFinalLoyalty)
			nFinalLoyalty++;
	}

	// Handle solo XP/NP gain
	if (!isInParty()
		|| (pParty = g_pMain->GetPartyPtr(GetPartyID())) == nullptr)
	{
		if (isDead())
			return;

		// Calculate the amount to adjust the XP/NP based on level difference.
		float fModifier = pNpc->GetRewardModifier(GetLevel());

		// Give solo XP
		if (iNpcExp > 0)
		{
			TempValue = nFinalExp * fModifier;
			nFinalExp = (int) TempValue;
			if (TempValue > nFinalExp)
				nFinalExp++;

			ExpChange(nFinalExp);
		}

		// Give solo NP
		if (iNpcLoyalty > 0)
		{
			bool UseModifier = false;

			if (UseModifier)
			{
				TempValue = nFinalLoyalty * fModifier;
				nFinalLoyalty = (int) TempValue;
				if (TempValue > nFinalLoyalty)
					nFinalLoyalty++;
			}

			SendLoyaltyChange(nFinalLoyalty);
		}

		return;
	}

	// Handle party XP/NP gain
	std::vector<CUser *> partyUsers;
	uint32 nTotalLevel = 0;
	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser * pUser = g_pMain->GetUserPtr(pParty->uid[i]);
		if (pUser == nullptr)
			continue;

		partyUsers.push_back(pUser);
		nTotalLevel += pUser->GetLevel();
	}

	const float fPartyModifierXP = 0.3f;
	const float fPartyModifierNP = 0.2f;

	uint32 nPartyMembers = (uint32) partyUsers.size();

	// Calculate the amount to adjust the XP/NP based on level difference.
	float fModifier = pNpc->GetPartyRewardModifier(nTotalLevel, nPartyMembers);

	if (iNpcExp > 0)
	{
		TempValue = nFinalExp * fModifier;
		nFinalExp = (int) TempValue;
		if (TempValue > nFinalExp)
			nFinalExp++;
	}

	if (iNpcLoyalty > 0)
	{
		TempValue = nFinalLoyalty * fModifier;
		nFinalLoyalty = (int) TempValue;
		if (TempValue > nFinalLoyalty)
			nFinalLoyalty++;
	}

	// Hand out kill rewards to all users in the party and still in range.
	foreach (itr, partyUsers)
	{
		CUser * pUser = (*itr);
		if (pUser->isDead()
			|| !pUser->isInRange(pNpc, RANGE_50M))
			continue;

		if (iNpcExp > 0)
		{
			TempValue = (nFinalExp * (1 + fPartyModifierXP * (nPartyMembers - 1))) * (double)pUser->GetLevel() / (double)nTotalLevel;
			int iExp = (int) TempValue;
			if (TempValue > iExp)
				iExp++;

			pUser->ExpChange(iExp);
		}

		if (iNpcLoyalty > 0)
		{
			TempValue = (nFinalLoyalty * (1 + fPartyModifierNP * (nPartyMembers - 1))) * (double)pUser->GetLevel() / (double)nTotalLevel;
			int iLoyalty = (int) TempValue;
			if (TempValue > iLoyalty)
				iLoyalty++;

			pUser->SendLoyaltyChange(iLoyalty);
		}
	}
}

/**
* @brief	Changes the player's experience points by iExp.
*
* @param	iExp	The amount of experience points to adjust by.
*/
void CUser::ExpChange(int64 iExp, bool bIsBonusReward)
{	
	// Stop players level 5 or under from losing XP on death.
	if ((GetLevel() < 6 && iExp < 0)
		// Stop players in the war zone (TODO: Add other war zones) from losing XP on death.
			|| (GetMap()->isWarZone() && iExp < 0))
			return;

	// Despite being signed, we don't want m_iExp ever going below 0.
	// If this happens, we need to investigate why -- not sweep it under the rug.
	ASSERT(m_iExp >= 0);

	if (iExp > 0)
	{
		if (!bIsBonusReward)
		{
			// Adjust the exp gained based on the percent set by the buff
			iExp = iExp * (m_sExpGainAmount + m_bItemExpGainAmount) / 100;

			// Add on any additional XP earned because of a global XP event.
			// NOTE: They officially check to see if the XP is <= 100,000.
			iExp = iExp * (100 + g_pMain->m_byExpEventAmount) / 100;

			if (GetPremiumProperty(PremiumExpPercent) > 0 && !isDead())
				iExp = iExp * (100 + GetPremiumProperty(PremiumExpPercent)) / 100;
		}
	}

	bool bLevel = true;
	if (iExp < 0 
		&& (m_iExp + iExp) < 0)
		bLevel = false;
	else
		m_iExp += iExp;

	// If we need to delevel...
	if (!bLevel)
	{
		// Drop us back a level.
		m_bLevel--;

		// Get the excess XP (i.e. below 0), so that we can take it off the max XP of the previous level
		// Remember: we're deleveling, not necessarily starting from scratch at the previous level
		int64 diffXP = m_iExp + iExp;

		// Now reset our XP to max for the former level.
		m_iExp = g_pMain->GetExpByLevel(GetLevel());

		// Get new stats etc.
		LevelChange(GetLevel(), false);

		// Take the remainder of the XP off (and delevel again if necessary).
		ExpChange(diffXP);
		return;
	}
	// If we've exceeded our XP requirement, we've leveled.
	else if (m_iExp >= m_iMaxExp)
	{
		if (GetLevel() < MAX_LEVEL)
		{
			// Reset our XP, level us up.
			m_iExp -= m_iMaxExp;
			LevelChange(++m_bLevel);
			return;
		}

		// Hit the max level? Can't level any further. Cap the XP.
		m_iExp = m_iMaxExp;
	}

	// Tell the client our new XP
	Packet result(WIZ_EXP_CHANGE);
	result << uint8(0) << m_iExp; // NOTE: Use proper flag
	Send(&result);

	// If we've lost XP, save it for possible refund later.
	if (iExp < 0)
		m_iLostExp = -iExp;
}

/**
* @brief	Get premium properties.
*/
uint16 CUser::GetPremiumProperty(PremiumPropertyOpCodes type)
{
	if (m_bPremiumType <= 0)
		return 0;

	_PREMIUM_ITEM * pPremiumItem = g_pMain->m_PremiumItemArray.GetData(m_bPremiumType);
	if (pPremiumItem == nullptr)
		return 0;

	switch (type)
	{
	case PremiumExpRestorePercent:
		return pPremiumItem->ExpRestorePercent;
	case PremiumNoahPercent:
		return pPremiumItem->NoahPercent;
	case PremiumDropPercent:
		return pPremiumItem->DropPercent;
	case PremiumBonusLoyalty:
		return pPremiumItem->BonusLoyalty;
	case PremiumRepairDiscountPercent:
		return pPremiumItem->RepairDiscountPercent;
	case PremiumItemSellPercent:
		return pPremiumItem->ItemSellPercent;
	case PremiumExpPercent:
		{
			foreach_stlmap (itr, g_pMain->m_PremiumItemExpArray)
			{
				_PREMIUM_ITEM_EXP *pPremiumItemExp = g_pMain->m_PremiumItemExpArray.GetData(itr->first);

				if (pPremiumItemExp == nullptr)
					continue;

				if (m_bPremiumType == pPremiumItemExp->Type && GetLevel() >= pPremiumItemExp->MinLevel && GetLevel() <= pPremiumItemExp->MaxLevel)
					return pPremiumItemExp->sPercent;
			}
		}
	}

	return 0;
}

/**
* @brief	Handles stat updates after a level change. 
* 			It does not change the level.
*
* @param	level   	The level we've changed to.
* @param	bLevelUp	true to level up, false for deleveling.
*/
void CUser::LevelChange(uint8 level, bool bLevelUp /*= true*/)
{
	if (level < 1 || level > MAX_LEVEL)
		return;

	if (bLevelUp && level > GetLevel() + 1)
	{
		int16 nStatTotal = 300 + (level - 1) * 3;
		uint8 nSkillTotal = (level - 9) * 2;

		if (level > 60)
			nStatTotal += 2 * (level - 60);

		m_sPoints += nStatTotal - GetStatTotal();
		m_bstrSkill[SkillPointFree] += nSkillTotal - GetTotalSkillPoints();
		m_bLevel = level;
	}
	else if (bLevelUp)
	{
		// On each level up, we should give 3 stat points for levels 1-60.
		// For each level above that, we give an additional 2 stat points (so 5 stat points per level).
		int levelsAfter60 = (level > 60 ? level - 60 : 0);
		if ((m_sPoints + GetStatTotal()) < int32(297 + (3 * level) + (2 * levelsAfter60)))
			m_sPoints += (levelsAfter60 == 0 ? 3 : 5);

		if (level >= 10 && GetTotalSkillPoints() < 2 * (level - 9))
			m_bstrSkill[SkillPointFree] += 2;
	}

	m_iMaxExp = g_pMain->GetExpByLevel(level);
	SetUserAbility();

	m_sMp = m_iMaxMp;
	HpChange(GetMaxHealth()); 

	Send2AI_UserUpdateInfo();

	Packet result(WIZ_LEVEL_CHANGE);
	result	<< GetSocketID()
		<< GetLevel() << m_sPoints << m_bstrSkill[SkillPointFree]
		<< uint32(m_iMaxExp) << uint32(m_iExp)
		<< m_iMaxHp << m_sHp 
		<< m_iMaxMp << m_sMp
		<< MaxWeight(m_sMaxWeight) << m_sItemWeight;

	g_pMain->Send_Region(&result, GetMap(), GetRegionX(), GetRegionZ());
	if (isInParty())
	{
		// TODO: Move this to party specific code
		result.Initialize(WIZ_PARTY);
		result << uint8(PARTY_LEVELCHANGE) << GetSocketID() << GetLevel();
		g_pMain->Send_PartyMember(GetPartyID(), &result);

		if (m_bIsChicken)
			GrantChickenManner();
	}

	// We should kick players out of the zone if their level no longer matches the requirements for this zone.
	if (GetLevel() < GetMap()->GetMinLevelReq() 
		|| GetLevel() > GetMap()->GetMaxLevelReq())
		KickOutZoneUser();
}

/**
* @brief	Handles player stat assignment.
*
* @param	pkt	The packet.
*/
void CUser::PointChange(Packet & pkt)
{
	uint8 type = pkt.read<uint8>();
	StatType statType = (StatType)(type - 1);

	if (statType < STAT_STR || statType >= STAT_COUNT 
		|| m_sPoints < 1
		|| GetStat(statType) >= STAT_MAX) 
		return;

	Packet result(WIZ_POINT_CHANGE, type);

	m_sPoints--; // remove a free point
	result << uint16(++m_bStats[statType]); // assign the free point to a stat
	SetUserAbility();
	result << m_iMaxHp << m_iMaxMp << m_sTotalHit << MaxWeight(m_sMaxWeight);
	Send(&result);
	SendItemMove(1);
}

/**
* @brief	Changes a user's HP.
*
* @param	amount   	The amount to change by.
* @param	pAttacker	The attacker.
* @param	bSendToAI	true to update the AI server.
*/
void CUser::HpChange(int amount, Unit *pAttacker /*= nullptr*/, bool bSendToAI /*= true*/) 
{
	Packet result(WIZ_HP_CHANGE);
	uint16 tid = (pAttacker != nullptr ? pAttacker->GetID() : -1);
	int16 oldHP = m_sHp;
	int originalAmount = amount;
	int mirrorDamage = 0;

	// No cheats allowed
	if (pAttacker && pAttacker->GetZoneID() != GetZoneID())
		return; 

	// Implement damage/HP cap.
	if (amount < -MAX_DAMAGE)
		amount = -MAX_DAMAGE;
	else if (amount > MAX_DAMAGE)
		amount = MAX_DAMAGE;

	// If we're taking damage...
	if (amount < 0)
	{
		if (isGM())
			return;

		RemoveStealth();

		// Handle the mirroring of damage.
		if (m_bMirrorDamage && isInParty() && GetZoneID() != ZONE_CHAOS_DUNGEON)
		{
			_PARTY_GROUP *pParty = nullptr;
			CUser *pUser = nullptr;
			mirrorDamage = (m_byMirrorAmount * amount) / 100;
			amount -= mirrorDamage;
			pParty = g_pMain->GetPartyPtr(GetPartyID());
			if (pParty != nullptr)
			{
				mirrorDamage = mirrorDamage / (GetPartyMemberAmount(pParty) - 1);
				for (int i = 0; i < MAX_PARTY_USERS; i++)
				{
					pUser = g_pMain->GetUserPtr(pParty->uid[i]);
					if(pUser == nullptr || pUser == this)
						continue;

					pUser->HpChange(mirrorDamage);
				}
			}
		}

		// Handle mana absorb skills
		if (m_bManaAbsorb > 0 && GetZoneID() != ZONE_CHAOS_DUNGEON)
		{
			int toBeAbsorbed = 0;
			toBeAbsorbed = (originalAmount*m_bManaAbsorb) / 100;
			amount -= toBeAbsorbed;

			if (m_bManaAbsorb == 15)
				toBeAbsorbed *= 4;

			MSpChange(toBeAbsorbed);
		}

		// Handle mastery passives
		if (isMastered() && GetZoneID() != ZONE_CHAOS_DUNGEON)
		{
			// Matchless: [Passive]Decreases all damages received by 15%
			if (CheckSkillPoint(SkillPointMaster, 10, MAX_LEVEL))
				amount = (85 * amount) / 100;
			// Absoluteness: [Passive]Decrease 10 % demage of all attacks
			else if (CheckSkillPoint(SkillPointMaster, 5, 9))
				amount = (90 * amount) / 100;
		}
	}
	// If we're receiving HP and we're undead, all healing must become damage.
	else if (m_bIsUndead)
	{
		amount = -amount;
		originalAmount = amount;
	}

	if (amount < 0 && -amount >= m_sHp)
		m_sHp = 0;
	else if (amount >= 0 && m_sHp + amount > m_iMaxHp)
		m_sHp = m_iMaxHp;
	else
		m_sHp += amount;

	result << m_iMaxHp << m_sHp << tid;

	if (GetHealth() > 0
		&& isMastered()
		&& !isMage() && GetZoneID() != ZONE_CHAOS_DUNGEON)
	{
		const uint16 hp30Percent = (30 * GetMaxHealth()) / 100;
		if ((oldHP >= hp30Percent && m_sHp < hp30Percent)
			|| (m_sHp > hp30Percent))
		{
			SetUserAbility();

			if (m_sHp < hp30Percent)
				ShowEffect(106800); // skill ID for "Boldness", shown when a player takes damage.
		}
	}

	Send(&result);

	if (bSendToAI)
	{
		result.Initialize(AG_USER_SET_HP);
		result << GetSocketID() << m_sHp << tid;
		Send_AIServer(&result);
	}

	if (isInParty() && GetZoneID() != ZONE_CHAOS_DUNGEON)
		SendPartyHPUpdate();

	// Ensure we send the original damage (prior to passives) amount to the attacker 
	// as it appears to behave that way officially.
	if (pAttacker != nullptr
		&& pAttacker->isPlayer())
		TO_USER(pAttacker)->SendTargetHP(0, GetID(), originalAmount);

	if (m_sHp == 0)
		OnDeath(pAttacker);
}

/**
* @brief	Changes a user's mana points.
*
* @param	amount	The amount to adjust by.
*/
void CUser::MSpChange(int amount)
{
	Packet result(WIZ_MSP_CHANGE);
	int16 oldMP = m_sMp;

	if (isGM() && amount < 0)
		return;

	// TODO: Make this behave unsigned.
	m_sMp += amount;
	if (m_sMp < 0)
		m_sMp = 0;
	else if (m_sMp > m_iMaxMp)
		m_sMp = m_iMaxMp;

	if (isMasteredMage())
	{
		const uint16 mp30Percent = (30 * GetMaxMana()) / 100;
		if (oldMP >= mp30Percent
			&& GetMana() < mp30Percent)
			ShowEffect(106800); // skill ID for "Boldness", shown when a player loses mana.
	}

	result << m_iMaxMp << m_sMp;
	Send(&result);

	if (isInParty())
		SendPartyHPUpdate(); // handles MP too
}

/**
* @brief	Sends a HP update to the user's party.
*/
void CUser::SendPartyHPUpdate()
{
	Packet result(WIZ_PARTY);
	result	<< uint8(PARTY_HPCHANGE)
		<< GetSocketID()
		<< m_iMaxHp << m_sHp
		<< m_iMaxMp << m_sMp;
	g_pMain->Send_PartyMember(GetPartyID(), &result);
}

/**
* @brief	Shows the specified skill's effect 
* 			to the surrounding regions.
*
* @param	nSkillID	Skill identifier.
*/
void CUser::ShowEffect(uint32 nSkillID)
{
	Packet result(WIZ_EFFECT);
	result << GetID() << nSkillID;
	SendToRegion(&result);
}

/**
* @brief	Shows an effect on the NPC currently 
* 			being interacted with.
*
* @param	nEffectID	Identifier for the effect.
*/
void CUser::ShowNpcEffect(uint32 nEffectID, bool bSendToRegion)
{
	Packet result(WIZ_OBJECT_EVENT, uint8(OBJECT_NPC));
	result << uint8(3) << m_sEventNid << nEffectID;
	if (bSendToRegion)
		SendToRegion(&result);
	else
		Send(&result);
}

/**
* @brief	Sends a player's base information to the AI server.
*
* @param	initialInfo	true when initially sending a player's information
* 						to the server.
*/
void CUser::Send2AI_UserUpdateInfo(bool initialInfo /*= false*/)
{
	Packet result(initialInfo ? AG_USER_INFO : AG_USER_UPDATE);
	GetUserInfoForAI(result);
	Send_AIServer(&result);
}

/**
* @brief	Calculates and resets the player's stats/resistances.
*
* @param	bSendPacket	true to send a subsequent item movement packet
* 						which is almost always required in addition to
* 						using this method.
*/
void CUser::SetUserAbility(bool bSendPacket /*= true*/)
{
	bool bHaveBow = false;
	_CLASS_COEFFICIENT* p_TableCoefficient = g_pMain->m_CoefficientArray.GetData(GetClass());
	uint16 sItemDamage = 0;
	if (p_TableCoefficient == nullptr)
		return;

	float hitcoefficient = 0.0f;

	if (!isWeaponsDisabled())
	{
		_ITEM_TABLE * pRightHand = GetItemPrototype(RIGHTHAND);
		_ITEM_DATA * pRightData = GetItem(RIGHTHAND);
		if (pRightHand != nullptr)
		{
			switch (pRightHand->m_bKind/10)
			{
			case WEAPON_DAGGER:
				hitcoefficient = p_TableCoefficient->ShortSword;
				break;
			case WEAPON_SWORD:
				hitcoefficient = p_TableCoefficient->Sword;
				break;
			case WEAPON_AXE:
				hitcoefficient = p_TableCoefficient->Axe;
				break;
			case WEAPON_MACE:
			case WEAPON_MACE2:
				hitcoefficient = p_TableCoefficient->Club;
				break;
			case WEAPON_SPEAR:
				hitcoefficient = p_TableCoefficient->Spear;
				break;
			case WEAPON_BOW:
			case WEAPON_LONGBOW:
			case WEAPON_LAUNCHER:
				hitcoefficient = p_TableCoefficient->Bow;
				bHaveBow = true;
				break;
			case WEAPON_STAFF:
				hitcoefficient = p_TableCoefficient->Staff;
				break;
			}
			if (pRightData->sDuration == 0)
			sItemDamage += (pRightHand->m_sDamage + m_bAddWeaponDamage)/2;
			else
			sItemDamage += pRightHand->m_sDamage + m_bAddWeaponDamage;
		}

		_ITEM_TABLE *pLeftHand = GetItemPrototype(LEFTHAND);
		_ITEM_DATA * pLeftData = GetItem(LEFTHAND);
		if (pLeftHand != nullptr)
		{
			if (pLeftHand->isBow())
			{
				hitcoefficient = p_TableCoefficient->Bow;
				bHaveBow = true;
				if (pLeftData->sDuration == 0)
				sItemDamage = (pLeftHand->m_sDamage + m_bAddWeaponDamage)/2;
				else
				sItemDamage = pLeftHand->m_sDamage + m_bAddWeaponDamage;
			}
			else
			{
				if (pLeftData->sDuration == 0)
				sItemDamage += ((pLeftHand->m_sDamage + m_bAddWeaponDamage) / 2) /2;
				else
				sItemDamage += (pLeftHand->m_sDamage + m_bAddWeaponDamage) / 2;
			}
		}
	}

	if (m_sACAmount < 0)
		m_sACAmount = 0;

	m_sTotalHit = 0;

	if (sItemDamage < 3)
		sItemDamage = 3;

	// Update stats based on item data
	SetSlotItemValue();

	int temp_str = GetStat(STAT_STR), temp_dex = getStatTotal(STAT_DEX);
	//	if( temp_str > 255 ) temp_str = 255;
	//	if( temp_dex > 255 ) temp_dex = 255;

	uint32 baseAP = 0, ap_stat = 0, additionalAP = 3;
	if (temp_str > 150)
		baseAP = temp_str - 150;

	if (temp_str == 160)
		baseAP--;

	temp_str += GetStatBonusTotal(STAT_STR);

	m_sMaxWeight = (((GetStatWithItemBonus(STAT_STR) + GetLevel()) * 50) + m_sMaxWeightBonus)  * (m_bMaxWeightAmount <= 0 ? 1 :  m_bMaxWeightAmount / 100);

	if (isRogue()) 
	{
		ap_stat = temp_dex;
	}
	else
	{
		ap_stat = temp_str;
		additionalAP += baseAP;
	}	
	
	if (isWarrior() || isPriest())
	{
	m_sTotalHit = (uint16)((0.010f * sItemDamage * (ap_stat + 40)) + (hitcoefficient * sItemDamage * GetLevel() * ap_stat));
	m_sTotalHit = (m_sTotalHit + additionalAP) * (100 + m_byAPBonusAmount) / 100;
	}
	if(isRogue())
	{
	m_sTotalHit = (uint16)((0.007f * sItemDamage * (ap_stat + 40)) + (hitcoefficient * sItemDamage * GetLevel() * ap_stat));
	m_sTotalHit = (m_sTotalHit + additionalAP) * (100 + m_byAPBonusAmount) / 100;
	}
	else if(isMage())
	{
	m_sTotalHit = (uint16)((0.005f * sItemDamage * (ap_stat + 40)) + (hitcoefficient * sItemDamage * GetLevel()));
	m_sTotalHit = (m_sTotalHit + additionalAP) * (100 + m_byAPBonusAmount) / 100;
	}

	m_sTotalAc = (short)(p_TableCoefficient->AC * (GetLevel() + m_sItemAc));
	if (m_sACPercent <= 0)
		m_sACPercent = 100;

		m_sTotalAc = m_sTotalAc * m_sACPercent / 100;

	m_fTotalHitrate = ((1 + p_TableCoefficient->Hitrate * GetLevel() *  temp_dex ) * m_sItemHitrate/100 ) * (m_bHitRateAmount/100);

	m_fTotalEvasionrate = ((1 + p_TableCoefficient->Evasionrate * GetLevel() * temp_dex ) * m_sItemEvasionrate/100) * (m_sAvoidRateAmount/100);

	SetMaxHp();
	SetMaxMp();

	uint8 bDefenseBonus = 0, bResistanceBonus = 0;

	// Reset resistance bonus
	m_bResistanceBonus = 0;

	// Apply passive skill bonuses
	// NOTE: This is how it's done officially (we should really clean this up)
	// Passive bonuses do NOT stack.
	if (isWarrior())
	{
		// NOTE: These may need updating (they're based on 1.298 stats)
		if (CheckSkillPoint(PRO_SKILL2, 5, 14))
			bDefenseBonus = 20;
		else if (CheckSkillPoint(PRO_SKILL2, 15, 34))
			bDefenseBonus = 30;
		else if (CheckSkillPoint(PRO_SKILL2, 35, 54))
			bDefenseBonus = 40;
		else if (CheckSkillPoint(PRO_SKILL2, 55, 69))
			bDefenseBonus = 50;
		else if (CheckSkillPoint(PRO_SKILL2, 70, MAX_LEVEL))
		{
			// Level 70 skill quest
			if (CheckExistEvent(51, 2))
				bDefenseBonus = 60;
			else
				bDefenseBonus = 50;
		}

		// Resist: [Passive]Increase all resistance by 30. If a shield is not equipped, the effect will decrease by half.
		if (CheckSkillPoint(PRO_SKILL2, 10, 19))
			bResistanceBonus = 30;
		// Endure: [Passive]Increase all resistance by 60. If a shield is not equipped, the effect will decrease by half.
		else if (CheckSkillPoint(PRO_SKILL2, 20, 39))
			bResistanceBonus = 60;
		// Immunity: [Passive]Increase all resistance by 90. If a shield is not equipped, the effect will decrease by half.
		else if (CheckSkillPoint(PRO_SKILL2, 40, MAX_LEVEL))
			bResistanceBonus = 90;

		// If a shield's not equipped, bonuses are decreased by half.
		_ITEM_TABLE *pLeftHand = GetItemPrototype(LEFTHAND);
		if (pLeftHand == nullptr || !pLeftHand->isShield())
		{
			bResistanceBonus /= 2;
			bDefenseBonus /= 2;
		}

		m_bResistanceBonus = bResistanceBonus;
		m_sTotalAc += bDefenseBonus * m_sTotalAc / 100;
		// m_sTotalAcUnk += bDefenseBonus * m_sTotalAcUnk / 100;
	}

	// Mastered warriors / mastered priests
	if (CheckClass(6, 12))
	{
		// Boldness/Daring: [Passive]Increase your defense by 20% when your HP is down to 30% or lower.
		if (m_sHp < 30 * m_iMaxHp / 100)
		{
			m_sTotalAc += 20 * m_sTotalAc / 100;
			// m_sTotalAcUnk += 20 * m_sTotalAcUnk / 100;
		}
	}
	else if (isRogue())
	{
		// Valor: [Passive]Increase your resistance by 50 when your HP is down to 30% or below.
		if (m_sHp < 30 * m_iMaxHp / 100)
			m_bResistanceBonus += 50;
	}

	if (m_bAddWeaponDamage > 0)
		++m_sTotalHit;

	if (m_sAddArmourAc > 0 || m_bPctArmourAc > 100)
		++m_sTotalAc;

	uint8 bSta = GetStat(STAT_STA);
	if (bSta > 100)
	{
		m_sTotalAc += bSta - 100;
		// m_sTotalAcUnk += (bSta - 100) / 3;
	}

	uint8 bInt = GetStat(STAT_INT);
	if (bInt > 100)
		m_bResistanceBonus += (bInt - 100) / 2;

	// TODO: Transformation stats need to be applied here

	if (bSendPacket)
		SendItemMove(2);

	// Update the AI server
	Send2AI_UserUpdateInfo();
}

/**
* @brief	Sends the target's HP to the player.
*
* @param	echo  	Client-based flag that we must echo back to the client. 
* 					Set to 0 if not responding to the client.
* @param	tid   	The target's ID.
* @param	damage	The amount of damage taken on this request, 0 if it does not apply.
*/
void CUser::SendTargetHP( uint8 echo, int tid, int damage )
{
	int hp = 0, maxhp = 0;

	Unit *pTarget = nullptr;

	if (tid >= NPC_BAND)
	{
		if (g_pMain->m_bPointCheckFlag == false) return;
		CNpc *pNpc = g_pMain->GetNpcPtr(tid);
		if (pNpc == nullptr)
			return;

		hp = pNpc->m_iHP;	
		maxhp = pNpc->m_iMaxHP;
		pTarget = pNpc;
	}
	else 
	{
		CUser *pUser = g_pMain->GetUserPtr(tid);
		if (pUser == nullptr || pUser->isDead()) 
			return;

		hp = pUser->m_sHp;	
		maxhp = pUser->m_iMaxHp;
		pTarget = pUser;
	}

	Packet result(WIZ_TARGET_HP);
	result << uint16(tid) << echo << maxhp << hp << uint16(damage);
	Send(&result);
}

/**
* @brief	Handler for opening a loot box.
*
* @param	pkt	The packet.
*/
void CUser::BundleOpenReq(Packet & pkt)
{
	Packet result(WIZ_BUNDLE_OPEN_REQ);
	uint32 bundle_index = pkt.read<uint32>();
	C3DMap* pMap = GetMap();
	CRegion * pRegion = GetRegion();

	if (pMap == nullptr
		|| bundle_index < 1 
		|| pRegion == nullptr
		|| isDead()) // yeah, we know people abuse this. We do not care!
		return;

	Guard lock(pRegion->m_RegionItemArray.m_lock);
	_LOOT_BUNDLE *pBundle = pRegion->m_RegionItemArray.GetData(bundle_index);
	if (pBundle == nullptr
		|| !isInRange(pBundle->x, pBundle->z, MAX_LOOT_RANGE))
		return;

	// Send all items in the bundle to the player
	foreach (itr, pBundle->Items)
		result << itr->nItemID << itr->sCount;

	// The client expects all n items, so if there's any excess...
	// send placeholder data for them.
	for (uint32 i = pBundle->Items.size(); i < LOOT_ITEMS; i++)
		result << uint32(0) << uint16(0);

	Send(&result);
}

/**
* @brief	Handler for looting an item from a loot box.
*
* @param	pkt	The packet.
*/
void CUser::ItemGet(Packet & pkt)
{
	enum LootErrorCodes
	{
		LootError					= 0,
		LootSolo					= 1,
		LootPartyCoinDistribution	= 2,
		LootPartyNotification		= 3,
		LootPartyItemGivenAway		= 4,
		LootPartyItemGivenToUs		= 5,
		LootNoRoom					= 6
	};

	Packet result(WIZ_ITEM_GET);
	uint32 nBundleID = pkt.read<uint32>(), nItemID = pkt.read<uint32>();
	_LOOT_BUNDLE * pBundle = nullptr;
	_LOOT_ITEM * pItem = nullptr;
	CRegion* pRegion = GetRegion();
	CUser * pReceiver = nullptr;

	// Lock the array while we process this request
	// to prevent any race conditions between getting/removing the items...
	Guard lock(pRegion->m_RegionItemArray.m_lock);

	// Are we in any region?
	if (pRegion == nullptr
		|| isTrading()
		|| isDead()
		// Does the bundle exist in this region's list?
		|| (pBundle = pRegion->m_RegionItemArray.GetData(nBundleID)) == nullptr
		// Are we close enough to the bundle?
		|| !isInRange(pBundle->x, pBundle->z, MAX_LOOT_RANGE))
		goto fail_return;

	// Does the specified item we're looting exist in the bundle?
	foreach (itr, pBundle->Items)
	{
		if (itr->nItemID == nItemID)
		{
			pItem = &(*itr);
			break;
		}
	}

	// Attempt to loot the specified item.
	// If looting is possible, we can then give the receiver the item.
	if (pItem == nullptr
		|| pItem->sCount == 0
		|| (pReceiver = GetLootUser(pBundle, pItem)) == nullptr)
		goto fail_return;

	// If we're dealing with coins, either:
	//  - we're not in a party, in which case the coins go to us. 
	//  - we're in a party, in which case we need to distribute the coins (proportionately, by their level). 
	// Error handling should already have occurred in GetLootUser().
	if (nItemID == ITEM_GOLD)
	{
		_PARTY_GROUP * pParty;
		uint32 pGold = 0;
		// Not in a party, so all the coins go to us.
		if (!isInParty()
			|| (pParty = g_pMain->GetPartyPtr(GetPartyID())) == nullptr)
		{
			// NOTE: Coins have been checked already.
			if (GetPremiumProperty(PremiumNoahPercent) > 0)
				pGold = pItem->sCount * (100 + GetPremiumProperty(PremiumNoahPercent)) / 100;
			else
				pGold = pItem->sCount;

			GoldGain(pGold, false, true);
			result << uint8(LootSolo) /*<< nBundleID */<< int8(-1) << nItemID << pItem->sCount << GetCoins();
			pReceiver->Send(&result);
		}
		// In a party, so distribute the coins relative to their level.
		else
		{
			uint16 sumOfLevels = 0;
			vector<CUser *> partyUsers;
			for (int i = 0; i < MAX_PARTY_USERS; i++)
			{
				CUser * pUser = g_pMain->GetUserPtr(pParty->uid[i]);
				if (pUser == nullptr)
					continue;

				sumOfLevels += pUser->GetLevel();
				partyUsers.push_back(pUser);
			}

			if (partyUsers.empty())
				goto fail_return;

			foreach (itr, partyUsers)
			{
				if ((*itr)->isDead() || 
					!(*itr)->isInRange(pBundle->x, pBundle->z, MAX_LOOT_RANGE))
					continue;

				// Calculate the number of coins to give the player
				// Give each party member coins relative to their level.
				int coins = (int)(pItem->sCount * (float)((*itr)->GetLevel() / (float)sumOfLevels));

				if ((*itr)->GetPremiumProperty(PremiumNoahPercent) > 0)
					pGold = coins * (100 + (*itr)->GetPremiumProperty(PremiumNoahPercent)) / 100;
				else
					pGold = coins;

				(*itr)->GoldGain(pGold, false, true);

				// Let each player know they received coins.
				result.clear();
				result << uint8(LootSolo) /*<< nBundleID */<< int8(-1) << nItemID << pItem->sCount << (*itr)->GetCoins();
				//result << uint8(LootPartyCoinDistribution) << nBundleID << uint8(-1) << nItemID << (*itr)->GetCoins(); old
				(*itr)->Send(&result);
			}
		}
	} // end of coin distribution
	// If the item selected is actually an item...
	else
	{
		if (pReceiver->isDead() || 
			!pReceiver->isInRange(pBundle->x, pBundle->z, MAX_LOOT_RANGE))
			(pReceiver = GetLootUser(pBundle, pItem));

		// Retrieve the position for this item.
		int8 bDstPos = pReceiver->FindSlotForItem(pItem->nItemID, pItem->sCount);

		// This should NOT happen unless their inventory changed after the check.
		// The item won't be removed until after processing's complete, so it's OK to error out here.
		if (bDstPos < 0)
			goto fail_return;

		// Ensure there's enough room in this user's inventory.
		if (!pReceiver->CheckWeight(pItem->nItemID, pItem->sCount))
		{
			result << uint8(LootNoRoom);
			pReceiver->Send(&result);
			return; // don't need to remove the item, so stop here.
		}

		// Add item to receiver's inventory
		_ITEM_TABLE * pTable = g_pMain->GetItemPtr(nItemID); // note: already looked up in GetLootUser() so it definitely exists
		_ITEM_DATA * pDstItem = &pReceiver->m_sItemArray[bDstPos];

		pDstItem->nNum    = pItem->nItemID;
		pDstItem->sCount += pItem->sCount;

		if (pDstItem->sCount == pItem->sCount)
		{
			pDstItem->nSerialNum = g_pMain->GenerateItemSerial();

			// NOTE: Obscure special items that act as if their durations are their stack sizes
			// will be broken here, but usual cases are typically only given in the PUS.
			// Will need to revise this logic (rather, shift it out into its own method).
			pDstItem->sDuration  = pTable->m_sDuration; 
		}

		if (pDstItem->sCount > MAX_ITEM_COUNT)
			pDstItem->sCount = MAX_ITEM_COUNT;
		if(!isInParty())
		{
		result	<< uint8(pReceiver == this ? LootSolo : LootPartyItemGivenToUs)
			//<< nBundleID 
			<< uint8(bDstPos - SLOT_MAX) 
			<< pDstItem->nNum << pDstItem->sCount
			<< pReceiver->GetCoins();
		
		pReceiver->Send(&result);
		}
		else
		{		if(pReceiver->GetName() == GetName())
				{
				result	<< uint8(pReceiver == this ? LootSolo : LootPartyItemGivenToUs)
				/*<< nBundleID */
				<< uint8(bDstPos - SLOT_MAX) 
				<< pDstItem->nNum << pDstItem->sCount
				<< pReceiver->GetCoins();
				}
			else{
				result	<< uint8(pReceiver == this ? LootSolo : LootPartyItemGivenToUs)
				<< nBundleID 
				<< uint8(bDstPos - SLOT_MAX) 
				<< pDstItem->nNum << pDstItem->sCount
				<< pReceiver->GetCoins();
				}

		pReceiver->Send(&result);
		}
		pReceiver->SetUserAbility(false);
		pReceiver->SendItemWeight();

		// Now notify the party that we've looted, if applicable.
		if (isInParty())
		{
			result.clear();
			result << uint8(LootPartyNotification) << nBundleID << nItemID << pReceiver->GetName();
			g_pMain->Send_PartyMember(GetPartyID(), &result);

			// If we're not the receiver, i.e. round-robin gave it to someone else
			// we should let us know that this was done (otherwise we'll be like, "GM!!? WHERE'S MY ITEM?!?")
			if (pReceiver != this)
			{
				result.clear();
				result << uint8(LootPartyItemGivenAway);
				Send(&result);
			}
		}
	}

	// Everything is OK, we have a target. Now remove the item from the bundle.
	// If there's nothing else in the bundle, remove the bundle from the region.
	GetMap()->RegionItemRemove(pRegion, pBundle, pItem);
	return;

fail_return:
	// Generic error
	result << uint8(LootError);
	Send(&result);
}

/**
* @brief	Gets the user to give the loot to.
*
* @param	pBundle	The loot bundle.
* @param	pItem  	The item being looted.
*
* @return	null if it fails, else the loot user.
*/
CUser * CUser::GetLootUser(_LOOT_BUNDLE * pBundle, _LOOT_ITEM * pItem)
{
	CUser * pReceiver = nullptr;

	if (pBundle == nullptr
		|| pItem == nullptr)
		return nullptr;

	// If we're dealing with coins, either:
	//  - we're in a party, in which case we need to distribute the coins (proportionately, by their level). 
	//	  No checks are necessary here (the coins will be miniscule, so if there's no room we can safely ignore them)
	//  - we're not in a party, in which case the coins go to us. 
	//	  In this case, we MUST check to be sure we have room for the coins.
	if (pItem->nItemID == ITEM_GOLD)
	{
		// NOTE: No checks are necessary if we're in a party.
		if (!isInParty())
		{
			// We're not in a party, so we must check to be 
			// sure we have enough room for the coins.
			if ((GetCoins() + pItem->sCount) > COIN_MAX)
				return nullptr;
		}

		// The caller will perform the distribution.
		return this; 
	}

	// If we're dealing with items:
	//	- if we're in a party: 
	//		distribute the item to the next player in the party in round-robin fashion, 
	//		whilst ensuring that user can actually hold the item.
	//  - if not in a party: 
	//		simply ensure that we can hold the item.
	if (isInParty())
	{
		// This ensures the user can hold the item.
		return GetItemRoutingUser(pItem->nItemID, pItem->sCount);
	}
	else
	{
		// NOTE: We check to see if they can hold this item in the caller.
		pReceiver = this;
	}

	return pReceiver;
}

/**
* @brief	Packet handler for various player state changes.
*
* @param	pkt	The packet.
*/
void CUser::StateChange(Packet & pkt)
{
	if (isDead())
		return;

	uint8 bType = pkt.read<uint8>(), buff;
	uint16 nBuff = pkt.read<uint16>();
	buff = *(uint8 *)&nBuff; // don't ask

	switch (bType)
	{
	case 1:
		if (buff != USER_STANDING && buff != USER_SITDOWN)
			return;
		break;

	case 3:
		// /unview | /view
		if ((buff == 1 || buff == 5)
			&& !isGM())
			return;
		break;

	case 4: // emotions
		switch (buff)
		{
		case 1: // Greeting 1-3
		case 2:
		case 3:
		case 11: // Provoke 1-3
		case 12:
		case 13:
		case 14: // additional animations randomly used when hitting spacebar
		case 15:
			break; // don't do anything with them (this can be handled neater, but just for testing purposes), just make sure they're allowed

		default:
			TRACE("[SID=%d] StateChange: %s tripped (bType=%d, buff=%d, nBuff=%d) somehow, HOW!?\n", 
				GetSocketID(), GetName().c_str(), bType, buff, nBuff);
			break;
		}
		break;

	case 5:
		if (!isGM())
			return;
		break;

	case 7: // invisibility flag, we don't want users overriding server behaviour.
		return;

	default:
		TRACE("[SID=%d] StateChange: %s tripped (bType=%d, buff=%d, nBuff=%d) somehow, HOW!?\n", 
			GetSocketID(), GetName().c_str(), bType, buff, nBuff);
		return;
	}

	StateChangeServerDirect(bType, nBuff);
}

/**
* @brief	Changes a player's state directly from the server
* 			without any checks.
*
* @param	bType	State type.
* @param	nBuff	The buff/flag (depending on the state type).
*/
void CUser::StateChangeServerDirect(uint8 bType, uint32 nBuff)
{
	uint8 buff = *(uint8 *)&nBuff; // don't ask
	switch (bType)
	{
	case 1:
		m_bResHpType = buff;
		break;

	case 2:
		m_bNeedParty = buff;
		break;

	case 3:
		m_nOldAbnormalType = m_bAbnormalType;

		// If we're a GM, we need to show ourselves before transforming.
		// Otherwise the visibility state is completely desynced.
		if (isGM())
			StateChangeServerDirect(5, 1);

		m_bAbnormalType = nBuff;
		break;

	case 5:
		m_bAbnormalType = nBuff;
		break;

	case 6:
		nBuff = m_bPartyLeader; // we don't set this here.
		break;

	case 7:
		UpdateVisibility((InvisibilityType)buff);
		break;

	case 8: // beginner quest
		break;
	}

	Packet result(WIZ_STATE_CHANGE);
	result << GetSocketID() << bType << nBuff; 
	SendToRegion(&result);
}

/**
* @brief	Takes a target's loyalty points (NP)
* 			and rewards some/all to the killer (current user).
*
* @param	tid		The target's ID.
* @param	bonusNP Bonus NP to be awarded to the killer as-is.
*/
void CUser::LoyaltyChange(int16 tid, uint16 bonusNP /*= 0*/)
{
	short loyalty_source = 0, loyalty_target = 0;

	// TODO: Rewrite this out, it shouldn't handle all cases so generally like this
	if (!GetMap()->isNationPVPZone() && !g_pMain->m_byBattleSiegeWarOpen
		|| GetZoneID() == ZONE_DESPERATION_ABYSS 
		|| GetZoneID() == ZONE_HELL_ABYSS 
		|| GetZoneID() == ZONE_DRAGON_CAVE 
		|| GetZoneID() == ZONE_CAITHAROS_ARENA)
		return;

	CUser* pTUser = g_pMain->GetUserPtr(tid);  
	if (pTUser == nullptr) 
		return;

	if (pTUser->GetNation() != GetNation() || g_pMain->m_byBattleSiegeWarOpen) 
	{
		if (pTUser->GetLoyalty() == 0) 
		{
			loyalty_source = 0;
			loyalty_target = 0;
		}
		// Ardream
		else if (pTUser->GetZoneID() == ZONE_ARDREAM)
		{
			loyalty_source = g_pMain->m_Loyalty_Ardream_Source; 
			loyalty_target = g_pMain->m_Loyalty_Ardream_Target;
		}
		// Ronark Land Base
		else if (pTUser->GetZoneID() == ZONE_RONARK_LAND_BASE)
		{
			loyalty_source = g_pMain->m_Loyalty_Ronark_Land_Base_Source; 
			loyalty_target = g_pMain->m_Loyalty_Ronark_Land_Base_Target;
		}
		else if (pTUser->GetZoneID() == ZONE_RONARK_LAND) 
		{
			loyalty_source = g_pMain->m_Loyalty_Ronark_Land_Source;
			loyalty_target = g_pMain->m_Loyalty_Ronark_Land_Target;
		}
		// Other zones
		else 
		{
			loyalty_source = g_pMain->m_Loyalty_Other_Zone_Source;
			loyalty_target = g_pMain->m_Loyalty_Other_Zone_Target;
		}
	}

	// Include any bonus NP (e.g. rival NP bonus)
	loyalty_source += bonusNP;
	loyalty_target -= bonusNP;

	SendLoyaltyChange(loyalty_source, true, false, pTUser->GetMonthlyLoyalty() > 0 ? true : false);
	pTUser->SendLoyaltyChange(loyalty_target, true, false, pTUser->GetMonthlyLoyalty() > 0 ? true : false);

	// TODO: Move this to a better place (death handler, preferrably)
	// If a war's running, and we died/killed in a war zone... (this method should NOT be so tied up in specifics( 
	if (g_pMain->m_byBattleOpen && GetMap()->isWarZone()) 
	{
		// Update the casualty count
		if (pTUser->GetNation() == KARUS)
			g_pMain->m_sKarusDead++;
		else 
			g_pMain->m_sElmoradDead++;
	}
}

void CUser::SpeedHackUser()
{
	if (!isInGame() || isGM())
		return;

	int16 nMaxSpeed = 45;

	if (GetFame() == COMMAND_CAPTAIN || isRogue())
		nMaxSpeed = 90;
	else if (isWarrior() || isMage() || isPriest())
		nMaxSpeed = 67;

	if (m_sSpeed > nMaxSpeed || m_sSpeed < -nMaxSpeed)
	{
		DateTime time;
		Disconnect();
		g_pMain->SendFormattedNotice("%s is currently disconnect for speed hack.",Nation::ALL,GetName().c_str());
		g_pMain->WriteCheatLogFile(string_format("[ SpeedHack - %d:%d:%d ] %s is Disconnected.\n", time.GetHour(),time.GetMinute(),time.GetSecond(),GetName().c_str()));
	}
}

void CUser::UserLookChange(int pos, int itemid, int durability)
{
	if (pos >= SLOT_MAX) // let's leave it at this for the moment, the updated check needs considerable reworking
		return;

	Packet result(WIZ_USERLOOK_CHANGE);
	result << GetSocketID() << uint8(pos) << itemid << uint16(durability);
	SendToRegion(&result, this, GetEventRoom());
}

void CUser::SendNotice()
{
	Packet result(WIZ_NOTICE);
	uint8 count = 0;

	result << uint8(2); // new-style notices (top-right of screen)
	result << count; // placeholder the count

	// Use first line for header, 2nd line for data, 3rd line for header... etc.
	// It's most likely what they do officially (as usual, | is their line separator)
	for (int i = 0; i < 10; i += 2)
		AppendNoticeEntry(result, count, g_pMain->m_ppNotice[i + 1], g_pMain->m_ppNotice[i]);

	AppendExtraNoticeData(result, count);
	result.put(1, count); // replace the placeholdered line count

	Send(&result);
}

void CUser::AppendNoticeEntry(Packet & pkt, uint8 & elementCount, const char * message, const char * title)
{
	if (message == nullptr || *message == '\0'
		|| title == nullptr || *title == '\0')
		return;

	pkt << title << message;
	elementCount++;
}

void CUser::AppendExtraNoticeData(Packet & pkt, uint8 & elementCount)
{
	string message;
	if (g_pMain->m_byExpEventAmount > 0)
	{
		g_pMain->GetServerResource(IDS_EXP_REPAY_EVENT, &message, g_pMain->m_byExpEventAmount);
		AppendNoticeEntry(pkt, elementCount, message.c_str(), "EXP Event"); 
	}

	if (g_pMain->m_byCoinEventAmount > 0)
	{
		g_pMain->GetServerResource(IDS_MONEY_REPAY_EVENT, &message, g_pMain->m_byCoinEventAmount);
		AppendNoticeEntry(pkt, elementCount, message.c_str(), "Noah Event"); 
	}

	if (g_pMain->m_byNPEventAmount > 0)
	{
		g_pMain->GetServerResource(IDS_NP_REPAY_EVENT, &message, g_pMain->m_byNPEventAmount);
		AppendNoticeEntry(pkt, elementCount, message.c_str(), "NP Event"); 
	}
}

void CUser::SkillPointChange(Packet & pkt)
{
	uint8 type = pkt.read<uint8>();
	Packet result(WIZ_SKILLPT_CHANGE, type);
	// invalid type
	if (type < SkillPointCat1 || type > SkillPointMaster 
		// not enough free skill points to allocate
			|| m_bstrSkill[0] < 1 
			// restrict skill points per category to your level
			|| m_bstrSkill[type] + 1 > GetLevel()
			// we need our first job change to assign skill points
			|| (GetClass() % 100) <= 4
			// to set points in the mastery category, we need to be mastered.
			|| (type == SkillPointMaster
			&& ((GetClass() % 2) != 0 || (GetClass() % 100) < 6
			// force a limit of MAX_LEVEL - 60 (the level you can do the mastery quest)
			// on the master skill category, so the limit's 23 skill points with a level 83 cap.
			|| m_bstrSkill[type] >= (MAX_LEVEL  - 60) 
			// allow only 1 point in the master category for every level above 60.
			|| m_bstrSkill[type] >= (GetLevel() - 60))))

	{
		result << m_bstrSkill[type]; // only send the packet on failure
		Send(&result);
		return;
	}

	--m_bstrSkill[0];
	++m_bstrSkill[type];
}

void CUser::UpdateGameWeather(Packet & pkt)
{
	if (!isGM())	// is this user a GM?
		return;

	if (pkt.GetOpcode() == WIZ_WEATHER)
	{
		pkt >> g_pMain->m_byWeather >> g_pMain->m_sWeatherAmount;
	}
	else
	{
		uint16 y, m, d;
		pkt >> y >> m >> d >> g_pMain->m_sHour >> g_pMain->m_sMin;
	}
	Send(&pkt); // pass the packet straight on
}

void CUser::GetUserInfoForAI(Packet & result)
{
	Guard lock(m_equippedItemBonusLock);

	result.SByte(); 
	result	<< GetSocketID()
		<< GetName() << GetZoneID() << GetNation() << GetLevel()
		<< m_sHp << m_sMp 
		<< m_sTotalHit << m_bAttackAmount
		<< m_sTotalAc << m_sACAmount
		<< m_fTotalHitrate << m_fTotalEvasionrate
		<< m_sItemAc
		<< GetPartyID() << GetAuthority()
		<< m_bInvisibilityType
		<< uint32(m_equippedItemBonuses.size());

	foreach (itr, m_equippedItemBonuses)
	{
		result << itr->first << uint32(itr->second.size()); // slot ID & number of bonuses
		foreach (bonusItr, itr->second)
			result << bonusItr->first << bonusItr->second; // bonus type, bonus amount
	}
}

void CUser::CountConcurrentUser()
{
	if (!isGM())
		return;

	uint16 count = 0;
	SessionMap sessMap = g_pMain->m_socketMgr.GetActiveSessionMap();
	foreach (itr, sessMap)
	{
		if (TO_USER(itr->second)->isInGame())
			count++;
	}

	Packet result(WIZ_CONCURRENTUSER);
	result << count;
	Send(&result);
}

/**
* @brief	Takes a target's loyalty points (NP)
* 			and rewards some/all to the killer's party (current user).
*
* @param	tid		The target's ID.
* @param	bonusNP Bonus NP to be awarded to the killer's party as-is.
*/
void CUser::LoyaltyDivide(int16 tid, uint16 bonusNP /*= 0*/)
{
	if (m_bZone == ZONE_SNOW_BATTLE
		|| m_bZone == ZONE_DESPERATION_ABYSS 
		|| m_bZone == ZONE_HELL_ABYSS 
		|| m_bZone == ZONE_DRAGON_CAVE 
		|| m_bZone == ZONE_CAITHAROS_ARENA)
		return;

	int16 loyalty_source = 0, loyalty_target = 0;
	uint8 total_member = 0;

	if (!isInParty())
		return;

	_PARTY_GROUP *pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr)
		return;

	CUser* pTUser = g_pMain->GetUserPtr(tid);
	if (pTUser == nullptr) 
		return;

	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser *pUser = g_pMain->GetUserPtr(pParty->uid[i]);
		if (pUser == nullptr)
			continue;

		total_member++;
	}

	if (total_member <= 0)
		return;

	//	This is for the Event Battle on Wednesday :(
	if (g_pMain->m_byBattleOpen
		&& GetZoneID() == (ZONE_BATTLE_BASE + g_pMain->m_byBattleZone))
	{
		if (pTUser->GetNation() == KARUS)
			g_pMain->m_sKarusDead++;
		else
			g_pMain->m_sElmoradDead++;
	}

	if (pTUser->GetNation() != GetNation()) // Different nations!!!
	{
		if (pTUser->GetLoyalty() == 0) // No cheats allowed...
		{
			loyalty_source = 0;
			loyalty_target = 0;
		}
		else
		{
			loyalty_source = GetLoyaltyDivideSource(total_member);
			loyalty_target = GetLoyaltyDivideTarget();

			if (loyalty_source == 0)
			{
				loyalty_source = 0;
				loyalty_target = 0;
			}
		}
	}
	else 
		return;

	for (int j = 0; j < MAX_PARTY_USERS; j++) // Distribute loyalty amongst party members.
	{
		bonusNP = 0;
		CUser *pUser = g_pMain->GetUserPtr(pParty->uid[j]);
		if (pUser == nullptr)
			continue;
		if (pUser->hasRival()
			&& !pUser->hasRivalryExpired()
			&& (pUser->GetRivalID() == pTUser->GetID())
			|| (pUser->GetRivalID() == pTUser->GetID()
			&& pUser->isPriest()))
		{
			bonusNP = RIVALRY_NP_BONUS;
			pUser->RemoveRival();
		}

		if (pUser->isAlive())
			pUser->SendLoyaltyChange(loyalty_source + bonusNP, true, false, pTUser->GetMonthlyLoyalty() > 0 ? true : false);
	}

	pTUser->SendLoyaltyChange(loyalty_target, true, false, pTUser->GetMonthlyLoyalty() > 0 ? true : false);
}

int16 CUser::GetLoyaltyDivideSource(uint8 totalmember)
{
	int16 nBaseLoyalty = 0;

	if (GetZoneID() == ZONE_ARDREAM)
		nBaseLoyalty = g_pMain->m_Loyalty_Ardream_Source;
	else if (GetZoneID() == ZONE_RONARK_LAND_BASE)
		nBaseLoyalty = g_pMain->m_Loyalty_Ronark_Land_Base_Source;
	else if (GetZoneID() == ZONE_RONARK_LAND)
		nBaseLoyalty = g_pMain->m_Loyalty_Ronark_Land_Source;
	else if (GetZoneID() == ZONE_KROWAZ_DOMINION)
		nBaseLoyalty = (g_pMain->m_Loyalty_Other_Zone_Source / 100) * 20;
	else
		nBaseLoyalty = g_pMain->m_Loyalty_Other_Zone_Source;

	int16 nMaxLoyalty = (nBaseLoyalty * 3) - 2;
	int16 nMinLoyalty = nMaxLoyalty / MAX_PARTY_USERS;
	int16 nLoyaltySource = nMinLoyalty;

	if (nLoyaltySource > 0)
	{
		for (int i = 0; i < (MAX_PARTY_USERS - totalmember); i++)
			nLoyaltySource += 2;
	}

	return nLoyaltySource -1;
}

int16 CUser::GetLoyaltyDivideTarget()
{
	if (GetZoneID() == ZONE_ARDREAM)
		return g_pMain->m_Loyalty_Ardream_Target;
	else if (GetZoneID() == ZONE_RONARK_LAND_BASE)
		return g_pMain->m_Loyalty_Ronark_Land_Base_Target;
	else if (GetZoneID() == ZONE_RONARK_LAND)
		return g_pMain->m_Loyalty_Ronark_Land_Target;
	else if (GetZoneID() == ZONE_KROWAZ_DOMINION)
		return (g_pMain->m_Loyalty_Other_Zone_Target / 100) * 20;
	else
		return g_pMain->m_Loyalty_Other_Zone_Target;

	return 0;
}

void CUser::ItemWoreOut(int type, int damage)
{
	static uint8 armourTypes[] = {HEAD, BREAST, LEG, GLOVE, FOOT };
	static uint8 weaponsTypes[] = {RIGHTHAND, LEFTHAND};
	uint8 totalSlots;
	int worerate;

	if (type == ACID_ALL)
	worerate = damage;
	else
	worerate = (int)sqrt(damage / 10.0f);

	if (worerate == 0) return;

	ASSERT(type == ATTACK || type == DEFENCE || type == REPAIR_ALL || type == ACID_ALL);

	// Inflict damage on equipped weapons.
	if (type == ATTACK)
		totalSlots = sizeof(weaponsTypes) / sizeof(*weaponsTypes); // use all the slots.
	// Inflict damage on equipped armour.
	else if (type == DEFENCE)
		totalSlots = sizeof(armourTypes) / sizeof(*armourTypes); // use all the slots.
	// Item repair all slots.
	else if (type == REPAIR_ALL)
		totalSlots = sizeof(armourTypes) / sizeof(*armourTypes); 
	// Item acid all slots.
	else if (type == ACID_ALL)
		totalSlots = sizeof(armourTypes) / sizeof(*armourTypes); 

	for (uint8 i = 0; i < totalSlots; i++) 
	{
		uint8 slot;
		if (type == DEFENCE)
		slot = armourTypes[i];
		else if (type == ATTACK)
		slot = weaponsTypes[i];
		else
		slot = armourTypes[i];

		_ITEM_DATA * pItem = GetItem(slot);
		_ITEM_TABLE * pTable = nullptr;

		// Is a non-broken item equipped?
		if (pItem == nullptr 
			|| (pItem->sDuration <= 0 && type != REPAIR_ALL) 
			// Does the item exist?
			|| (pTable = g_pMain->GetItemPtr(pItem->nNum)) == nullptr
			// If it's in the left or righthand slot, is it a shield? (this doesn't apply to weapons)
			|| (type == ATTACK 
			&& ((slot == LEFTHAND || slot == RIGHTHAND) && pTable->m_bSlot == ItemSlot1HLeftHand))) 
			continue;

		if (type == REPAIR_ALL)
		{
			pItem->sDuration = pTable->m_sDuration;
			SendDurability(slot, pItem->sDuration); 
			UserLookChange(slot, pItem->nNum, pItem->sDuration);
			continue;
		}

		int beforepercent = (int)((pItem->sDuration / (double)pTable->m_sDuration) * 100);
		int curpercent;

		if (worerate > pItem->sDuration)
			pItem->sDuration = 0;
		else 
			pItem->sDuration -= worerate;

		if (m_sItemArray[slot].sDuration == 0)
		{
			SendDurability(slot, 0);
			SetUserAbility(false);
			SendItemMove(1);
			continue;
		}

		SendDurability(slot, pItem->sDuration); 

		curpercent = (int)((pItem->sDuration / (double)pTable->m_sDuration) * 100);

		if ((curpercent / 5) != (beforepercent / 5)) 
		{
			if (curpercent >= 65 && curpercent < 70
				|| curpercent >= 25 && curpercent < 30)
				UserLookChange(slot, pItem->nNum, pItem->sDuration);
		}
	}
}

void CUser::SendDurability(uint8 slot, uint16 durability)
{
	Packet result(WIZ_DURATION, slot);
	result << durability;
	Send(&result);
}

void CUser::SendItemMove(uint8 subcommand)
{
	Packet result(WIZ_ITEM_MOVE, subcommand);

	if (m_bAttackAmount == 0)
		m_bAttackAmount = 100;

	// If the subcommand is not error, send the stats.
	if (subcommand != 0)
	{
		result	<< uint16(m_sTotalHit * m_bAttackAmount / 100) 
			<< uint16(m_sTotalAc + m_sACAmount)
			<< MaxWeight(m_sMaxWeight)
			<< m_iMaxHp << m_iMaxMp
			<< GetStatBonusTotal(STAT_STR) << GetStatBonusTotal(STAT_STA)
			<< GetStatBonusTotal(STAT_DEX) << GetStatBonusTotal(STAT_INT)
			<< GetStatBonusTotal(STAT_CHA)
			<< uint16(((m_sFireR + m_bAddFireR) * m_bPctFireR / 100) + m_bResistanceBonus) 
			<< uint16(((m_sColdR + m_bAddColdR) * m_bPctColdR / 100) + m_bResistanceBonus) 
			<< uint16(((m_sLightningR + m_bAddLightningR) * m_bPctLightningR / 100) + m_bResistanceBonus) 
			<< uint16(((m_sMagicR + m_bAddMagicR) * m_bPctMagicR / 100) + m_bResistanceBonus) 
			<< uint16(((m_sDiseaseR + m_bAddDiseaseR) * m_bPctDiseaseR / 100) + m_bResistanceBonus) 
			<< uint16(((m_sPoisonR + m_bAddPoisonR) * m_bPctPoisonR / 100) + m_bResistanceBonus);
	}
	Send(&result);
}

void CUser::HPTimeChange()
{
	m_tHPLastTimeNormal = UNIXTIME;

	if (isDead())
		return;

	int mpPercent = 100;

	if (GetZoneID() == ZONE_SNOW_BATTLE 
		&& g_pMain->m_byBattleOpen == SNOW_BATTLE)
	{
		HpChange(5);
		return;
	}

	// For mages with under 30% of HP
	if (CheckClass(110, 210)
		&& m_sMp < (30 * m_iMaxMp / 100))
		mpPercent = 120;

	if (m_bResHpType == USER_STANDING
		/* // unknown types
		|| m_bResHpType == 4 
		|| m_bResHpType == 5*/)
	{
		if (isTransformed())
		{
			// TODO: Apply transformation rates
		}
		else if (m_iMaxMp != m_sMp)
		{
			MSpChange((int)(((GetLevel() * (1 + GetLevel() / 60.0) + 1) * 0.2) + 3) * mpPercent / 100);
		}
	}
	else if (m_bResHpType == USER_SITDOWN)
	{
		if (isGM())
		{
			HpChange(m_iMaxHp);
			MSpChange(m_iMaxMp);
		}
		else
		{
			if (m_iMaxHp != m_sHp)
				HpChange((int)(GetLevel() * (1 + GetLevel() / 30.0)) + 3);

			if (m_iMaxMp != m_sMp)
				MSpChange((int)(((m_iMaxMp * 5) / ((GetLevel() - 1) + 30 )) + 3) * mpPercent / 100);
		}
	}
}

void CUser::HPTimeChangeType3()
{
	if (isDead()
		|| !m_bType3Flag)
		return;

	uint16	totalActiveDurationalSkills = 0, 
		totalActiveDOTSkills = 0;
	bool bIsDOT = false;
	for (int i = 0; i < MAX_TYPE3_REPEAT; i++)
	{
		MagicType3 * pEffect = &m_durationalSkills[i];
		if (!pEffect->m_byUsed)
			continue;

		// Has the required interval elapsed before using this skill?
		if ((UNIXTIME - pEffect->m_tHPLastTime) >= pEffect->m_bHPInterval)
		{
			Unit * pUnit = g_pMain->GetUnitPtr(pEffect->m_sSourceID);

			// Reduce the HP 
			HpChange(pEffect->m_sHPAmount, pUnit); // do we need to specify the source of the DOT?
			pEffect->m_tHPLastTime = UNIXTIME;

			if (pEffect->m_sHPAmount < 0)
				bIsDOT = true;

			// Has the skill expired yet?
			if (++pEffect->m_bTickCount == pEffect->m_bTickLimit)
			{
				Packet result(WIZ_MAGIC_PROCESS, uint8(MAGIC_DURATION_EXPIRED));

				// Healing-over-time skills require the type 100
				if (pEffect->m_sHPAmount > 0)
					result << uint8(100);
				else // Damage-over-time requires 200.
					result << uint8(200);

				Send(&result);

				// If the skill inflicts poison damage, remember to remove the poison indicator!
				if (pEffect->m_byAttribute == POISON_R)
					SendUserStatusUpdate(USER_STATUS_POISON, USER_STATUS_CURE);

				pEffect->Reset();
			}
		}

		if (pEffect->m_byUsed)
		{
			totalActiveDurationalSkills++;
			if (pEffect->m_sHPAmount < 0)
				totalActiveDOTSkills++;
		}
	}

	// Have all the skills expired?
	if (totalActiveDurationalSkills == 0)
		m_bType3Flag = false;

	// If there was DOT skills when we started, but none anymore... revert the HP bar.
	if (bIsDOT && totalActiveDOTSkills == 0)
		SendUserStatusUpdate(USER_STATUS_DOT, USER_STATUS_CURE);
}

void CUser::Type4Duration()
{
	Guard lock(m_buffLock);
	if (m_buffMap.empty())
		return;

	foreach (itr, m_buffMap)
	{
		if (itr->second.m_tEndTime > UNIXTIME)
			continue;

		CMagicProcess::RemoveType4Buff(itr->first, this, true, isLockableScroll(itr->second.m_bBuffType));
		break; // only ever handle one at a time with the current logic
	}

	if (!isDebuffed())
		SendUserStatusUpdate(USER_STATUS_POISON, USER_STATUS_CURE);
}

void CUser::SendAllKnightsID()
{
	Packet result(WIZ_KNIGHTS_LIST, uint8(1));
	uint16 count = 0;

	foreach_stlmap (itr, g_pMain->m_KnightsArray)
	{
		CKnights *pKnights = itr->second;
		if (pKnights == nullptr)
			continue;
		result << pKnights->m_sIndex << pKnights->m_strName;
		count++;
	}

	result.put(0, count);
	SendCompressed(&result);
}

void CUser::OperatorCommand(Packet & pkt)
{
	if (!isGM())
		return;

	std::string strUserID;
	uint8 opcode;
	bool bIsOnline = false;
	std::string sNoticeMessage, sOperatorCommandType;
	pkt >> opcode >> strUserID;

	if (strUserID.empty() || strUserID.size() > MAX_ID_SIZE)
		return;

	CUser *pUser = g_pMain->GetUserPtr(strUserID, TYPE_CHARACTER);
	if (pUser == nullptr)
		bIsOnline = false;
	else
		bIsOnline = true;

	switch (opcode)
	{
	case OPERATOR_ARREST:
		if (bIsOnline)
		{
			ZoneChange(pUser->GetZoneID(), pUser->m_curx, pUser->m_curz);
			sOperatorCommandType = "OPERATOR_ARREST";
		}
		break;
	case OPERATOR_SUMMON:
		if (bIsOnline)
		{
			pUser->ZoneChange(GetZoneID(), m_curx, m_curz);
			sOperatorCommandType = "OPERATOR_SUMMON";
		}
		break;
	case OPERATOR_CUTOFF:
		if (bIsOnline)
		{
			pUser->Disconnect();
			sOperatorCommandType = "OPERATOR_CUTOFF";
		}
		break;
	case OPERATOR_BAN:
	case OPERATOR_BAN_ACCOUNT: // ban account is meant to call a proc to do so
		if (bIsOnline)
		{
			pUser->m_bAuthority = AUTHORITY_BANNED;
			pUser->Disconnect();
		}
		else 
			g_DBAgent.UpdateUserAuthority(strUserID,AUTHORITY_BANNED);

		sOperatorCommandType = "OPERATOR_BAN_ACCOUNT";
		sNoticeMessage = string_format("%s is currently blocked for illegal activity.", strUserID.c_str());
		break;
	case OPERATOR_MUTE:
		if (bIsOnline)
			pUser->m_bAuthority = AUTHORITY_MUTED;
		else
			g_DBAgent.UpdateUserAuthority(strUserID,AUTHORITY_MUTED);

		sOperatorCommandType = "OPERATOR_MUTE";
		sNoticeMessage = string_format("%s is currently muted.", strUserID.c_str());
		break;
	case OPERATOR_DISABLE_ATTACK:
		if (bIsOnline) 
			pUser->m_bAuthority = AUTHORITY_ATTACK_DISABLED;
		else
			g_DBAgent.UpdateUserAuthority(strUserID,AUTHORITY_ATTACK_DISABLED);
		sOperatorCommandType = "OPERATOR_DISABLE_ATTACK";
		sNoticeMessage = string_format("%s is currently attack disabled.", strUserID.c_str());
		break;
	case OPERATOR_ENABLE_ATTACK:
		if (bIsOnline)
			pUser->m_bAuthority = AUTHORITY_PLAYER;
		else
			g_DBAgent.UpdateUserAuthority(strUserID,AUTHORITY_PLAYER);
		sOperatorCommandType = "OPERATOR_ENABLE_ATTACK";
		sNoticeMessage = string_format("%s is currently attack enabled.", strUserID.c_str());
		break;
	case OPERATOR_UNMUTE:
		if (bIsOnline)
			pUser->m_bAuthority = AUTHORITY_PLAYER;
		else
			g_DBAgent.UpdateUserAuthority(strUserID,AUTHORITY_PLAYER);
		sOperatorCommandType = "OPERATOR_UNMUTE";
		sNoticeMessage = string_format("%s is currently unmuted.", strUserID.c_str());
		break;
	}

	if (!sNoticeMessage.empty())
		g_pMain->SendNotice(sNoticeMessage.c_str(),Nation::ALL);

	if (!sOperatorCommandType.empty())
	{
		DateTime time;
		g_pMain->WriteChatLogFile(string_format("[ GAME MASTER - %d:%d:%d ] %s : %s %s ( Zone=%d, X=%d, Z=%d )\n",time.GetHour(),time.GetMinute(),time.GetSecond(),GetName().c_str(),sOperatorCommandType.c_str(),strUserID.c_str(),GetZoneID(),uint16(GetX()),uint16(GetZ())));
	}
}

void CUser::SpeedHackTime(Packet & pkt)
{
	if (!isInGame() || isGM())
		return;

	float nSpeed = 45.0f;

	if (GetFame() == COMMAND_CAPTAIN || isRogue())
		nSpeed = 90.0f;
	else if (isWarrior() || isMage() || isPriest())
		nSpeed = 67.0f;

	nSpeed += 10.0f; // Tolerance...

	float nRange = (pow(GetX() - m_LastX, 2.0f) + pow(GetZ() - m_LastZ, 2.0f)) / 100.0f;

	if (nRange >= nSpeed)
	{
		DateTime time;
		g_pMain->WriteCheatLogFile(string_format("[ SpeedHack - %d:%d:%d ] %s is Warp to Last Position.\n", time.GetHour(),time.GetMinute(),time.GetSecond(),GetName().c_str()));
		Warp(uint16(m_LastX) * 10, uint16(m_LastZ) * 10);
	}
	else
	{
		m_LastX = GetX();
		m_LastZ = GetZ();
	}

#if 0 // temporarily disabled
	uint8 b_first;
	float servertime = 0.0f, clienttime = 0.0f, client_gap = 0.0f, server_gap = 0.0f;

	pkt >> b_first >> clienttime;

	if( b_first ) {
		m_fSpeedHackClientTime = clienttime;
		m_fSpeedHackServerTime = TimeGet();
	}
	else {
		servertime = TimeGet();

		server_gap = servertime - m_fSpeedHackServerTime;
		client_gap = clienttime - m_fSpeedHackClientTime;

		if( client_gap - server_gap > 10.0f ) {
			TRACE("%s SpeedHack User Checked By Server Time\n", m_id);
			Close();
		}
		else if( client_gap - server_gap < 0.0f ) {
			m_fSpeedHackClientTime = clienttime;
			m_fSpeedHackServerTime = TimeGet();
		}
	}
#endif
}

int CUser::FindSlotForItem(uint32 nItemID, uint16 sCount /*= 1*/)
{
	int result = -1;
	_ITEM_TABLE *pTable = g_pMain->GetItemPtr(nItemID);
	if (pTable == nullptr)
		return result;

	// If the item's stackable, try to find it a home.
	// We could do this in the same logic, but I'd prefer one initial check
	// over the additional logic hit each loop iteration.
	if (pTable->m_bCountable)
	{
		for (int i = SLOT_MAX; i < SLOT_MAX+HAVE_MAX; i++)
		{
			_ITEM_DATA *pItem = GetItem(i);

			if(pItem == nullptr)
				continue; 

			// If it's the item we're after, and there will be room to store it...
			if (pItem
				&& pItem->nNum == nItemID 
				&& pItem->sCount + sCount <= ITEMCOUNT_MAX)
				return i;

			// Found a free slot, we'd prefer to stack it though
			// so store the first free slot, and ignore it.
			if (pItem->nNum == 0
				&& result < 0)
				result = i;
		}

		// If we didn't find a slot countaining our stackable item, it's possible we found
		// an empty slot. So return that (or -1 if it none was found; no point searching again).
		return result;
	}

	// If it's not stackable, don't need any additional logic.
	// Just find the first free slot.
	return GetEmptySlot();
}

int CUser::GetEmptySlot()
{
	for (int i = SLOT_MAX; i < SLOT_MAX+HAVE_MAX; i++)
	{
		_ITEM_DATA *pItem = GetItem(i);

		if (!pItem) 
			continue; 

		if (pItem->nNum == 0)
			return i;
	}

	return -1;
}

void CUser::Home()
{
	if (isDead()
		// When transformed into a "Kaul", you are unable to /town or attack.
			|| isKaul()
			/* No cheats allowed */
			|| GetHealth() < (GetMaxHealth() / 2)
			|| GetZoneID() == ZONE_CHAOS_DUNGEON
			|| GetZoneID() == ZONE_BORDER_DEFENSE_WAR
			|| hasBuff(BUFF_TYPE_FREEZE))
			return;

	// The point where you will be warped to.
	short x = 0, z = 0;

	// Forgotten Temple
	if (GetZoneID() == ZONE_FORGOTTEN_TEMPLE)
	{
		KickOutZoneUser(true);
		return;
	}
	// Prevent /town'ing in quest arenas
	else if ((GetZoneID() / 10) == 5
		|| !GetStartPosition(x, z))
		return;

	Warp(x * 10, z * 10);
}

bool CUser::GetStartPosition(short & x, short & z, uint8 bZone /*= 0 */)
{
	// Get start position data for current zone (unless we specified a zone).
	int nZoneID = (bZone == 0 ? GetZoneID() : bZone);
	_START_POSITION *pData = g_pMain->GetStartPosition(nZoneID);
	if (pData == nullptr)
		return false;

	_KNIGHTS_SIEGE_WARFARE *pKnightSiege = g_pMain->GetSiegeMasterKnightsPtr(1);

	// TODO: Allow for Delos/CSW.
	if ((pKnightSiege!=nullptr) && (pKnightSiege->sMasterKnights == GetClanID() && GetZoneID() == ZONE_DELOS && GetClanID() != 0))
	{
		x = 505 + myrand(0, pData->bRangeX);
		z = 840 + myrand(0, pData->bRangeZ);
	}
	else
	{
		// NOTE: This is how mgame does it.
		// This only allows for positive randomisation; we should really allow for the full range...
		if (GetNation() == KARUS) {
			x = pData->sKarusX + myrand(0, pData->bRangeX);
			z = pData->sKarusZ + myrand(0, pData->bRangeZ);
		} else {
			x = pData->sElmoradX + myrand(0, pData->bRangeX);
			z = pData->sElmoradZ + myrand(0, pData->bRangeZ);
		}
	}

	return true;
}

bool CUser::GetStartPositionRandom(short & x, short & z, uint8 bZone)
{
	int nRandom = myrand(0, g_pMain->m_StartPositionRandomArray.GetSize() - 1);
	goto GetPosition;

GetPosition:
	{
		if (g_pMain->m_StartPositionRandomArray.GetData(nRandom)->ZoneID == (bZone == 0 ? GetZoneID() : bZone))
		{
			x = g_pMain->m_StartPositionRandomArray.GetData(nRandom)->PosX + myrand(0, g_pMain->m_StartPositionRandomArray.GetData(nRandom)->Radius);
			z = g_pMain->m_StartPositionRandomArray.GetData(nRandom)->PosZ + myrand(0, g_pMain->m_StartPositionRandomArray.GetData(nRandom)->Radius);
			return true;
		}

		nRandom = myrand(0, g_pMain->m_StartPositionRandomArray.GetSize() - 1);
		goto GetPosition;
	}

	return GetStartPosition(x, z);
}

void CUser::ResetWindows()
{
	if (isTrading())
		ExchangeCancel();

	if (m_bRequestingChallenge)
		HandleChallengeCancelled(m_bRequestingChallenge);

	if (m_bChallengeRequested)
		HandleChallengeRejected(m_bChallengeRequested);

	// If we're a vendor, close the stall
	if (isMerchanting())
		MerchantClose();

	// If we're just browsing, free up our spot so others can browse the vendor.
	if (m_sMerchantsSocketID >= 0)
		CancelMerchant();

	if(isMining())
		HandleMiningStop((Packet)(WIZ_MINING, MiningStop));

	/*	if (isUsingBuyingMerchant())
	BuyingMerchantClose();

	if (isUsingStore())
	m_bStoreOpen = false;*/
}

CUser * CUser::GetItemRoutingUser(uint32 nItemID, uint32 sCount)
{
	if (!isInParty())
		return this;

	_ITEM_TABLE * pTable;
	_PARTY_GROUP * pParty = g_pMain->GetPartyPtr(GetPartyID());
	if (pParty == nullptr
		|| (pTable = g_pMain->GetItemPtr(nItemID)) == nullptr
		|| pParty->bItemRouting >= MAX_PARTY_USERS)
		return nullptr;

	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser * pUser = g_pMain->GetUserPtr(pParty->uid[pParty->bItemRouting]);

		if (pParty->bItemRouting > 6)
			pParty->bItemRouting = 0;
		else
			pParty->bItemRouting++;

		if (pUser != nullptr 
			&& pUser->CheckWeight(pTable, nItemID, sCount))
			return pUser;
	}

	return nullptr;
}

void CUser::ClassChangeReq()
{
	Packet result(WIZ_CLASS_CHANGE, uint8(CLASS_CHANGE_RESULT));
	if (GetLevel() < 10) // if we haven't got our first job change
		result << uint8(2);
	else if ((m_sClass % 100) > 4) // if we've already got our job change
		result << uint8(3);
	else // otherwise
		result << uint8(1);
	Send(&result);
}

// Dialog
void CUser::SendStatSkillDistribute()
{
	Packet result(WIZ_CLASS_CHANGE,uint8(CLASS_CHANGE_REQ));
	Send(&result); 
}

void CUser::AllSkillPointChange(bool bIsFree)
{
	Packet result(WIZ_CLASS_CHANGE, uint8(ALL_SKILLPT_CHANGE));
	int index = 0, skill_point = 0, money = 0, temp_value = 0, old_money = 0;
	uint8 type = 0;

	temp_value = (int)pow((GetLevel() * 2.0f), 3.4f);
	if (GetLevel() < 30)		
		temp_value = (int)(temp_value * 0.4f);
	else if (GetLevel() >= 60)
		temp_value = (int)(temp_value * 1.5f);

	//temp_value = (int)(temp_value * 1.5f);

	// If global discounts are enabled 
	if (g_pMain->m_sDiscount == 2 // or war discounts are enabled
		|| (g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == m_bNation))
		temp_value /= 2;

	// Level too low.
	if (GetLevel() < 10)
		goto fail_return;

	// Get total skill points
	for (int i = 1; i < 9; i++)
		skill_point += m_bstrSkill[i];

	// If we don't have any skill points, there's no point resetting now is there.
	if (skill_point <= 0)
	{
		type = 2;
		goto fail_return;
	}

	// Not enough money.
	if (!bIsFree & !GoldLose(temp_value, false))
		goto fail_return;

	// Reset skill points.
	m_bstrSkill[0] = (GetLevel() - 9) * 2;
	for (int i = 1; i < 9; i++)	
		m_bstrSkill[i] = 0;

	result << uint8(1) << GetCoins() << m_bstrSkill[0];
	Send(&result);
	return;

fail_return:
	result << type << temp_value;
	Send(&result);
}

void CUser::AllPointChange(bool bIsFree)
{
	Packet result(WIZ_CLASS_CHANGE, uint8(ALL_POINT_CHANGE));
	int temp_money;
	uint16 statTotal;

	uint16 byStr, bySta, byDex, byInt, byCha;
	uint8 bResult = 0;

	if (GetLevel() > MAX_LEVEL)
		goto fail_return;

	temp_money = (int)pow((GetLevel() * 2.0f ), 3.4f);
	if (GetLevel() < 30)
		temp_money = (int)(temp_money * 0.4f);
	else if (GetLevel() >= 60) 
		temp_money = (int)(temp_money * 1.5f);

	if ((g_pMain->m_sDiscount == 1 && g_pMain->m_byOldVictory == GetNation())
		|| g_pMain->m_sDiscount == 2)
		temp_money /= 2;

	for (int i = 0; i < SLOT_MAX; i++)
	{
		if (m_sItemArray[i].nNum) {
			bResult = 4;
			goto fail_return;
		}
	}

	// It's 300-10 for clarity (the 10 being the stat points assigned on char creation)
	if (GetStatTotal() == 290)
	{
		bResult = 2; // don't need to reallocate stats, it has been done already...
		goto fail_return;
	}

	// Not enough coins
	if (!bIsFree & !GoldLose(temp_money, false))
		goto fail_return;

	// TODO: Pull this from the database.
	switch (m_bRace)
	{
	case KARUS_BIG:	
		if(isWarrior())
		{
		SetStat(STAT_STR, 65);
		SetStat(STAT_STA, 65);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		break;
	case KARUS_MIDDLE:
		if(isWarrior())
		{
		SetStat(STAT_STR, 65);
		SetStat(STAT_STA, 65);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else
		{
		SetStat(STAT_STR, 60);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		break;
	case KARUS_SMALL:
		if (isWarrior())
		{
		SetStat(STAT_STR, 65);
		SetStat(STAT_STA, 65);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isRogue())
		{
		SetStat(STAT_STR, 60);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isPriest())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 50);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		else  if(isMage())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		break;
	case KARUS_WOMAN:
		if (isWarrior())
		{
		SetStat(STAT_STR, 65);
		SetStat(STAT_STA, 65);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isRogue())
		{
		SetStat(STAT_STR, 60);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isPriest())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 50);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		else  if(isMage())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		break;
	case BABARIAN:
		if (isWarrior())
		{
		SetStat(STAT_STR, 65);
		SetStat(STAT_STA, 65);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		break;
	case ELMORAD_MAN:
		if (isWarrior())
		{
		SetStat(STAT_STR, 65);
		SetStat(STAT_STA, 65);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isRogue())
		{
		SetStat(STAT_STR, 60);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isPriest())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 50);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		else  if(isMage())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		break;
	case ELMORAD_WOMAN:	
		if (isWarrior())
		{
		SetStat(STAT_STR, 65);
		SetStat(STAT_STA, 65);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isRogue())
		{
		SetStat(STAT_STR, 60);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 50);
		SetStat(STAT_CHA, 50);
		}
		else if(isPriest())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 50);
		SetStat(STAT_DEX, 70);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		else  if(isMage())
		{
		SetStat(STAT_STR, 50);
		SetStat(STAT_STA, 60);
		SetStat(STAT_DEX, 60);
		SetStat(STAT_INT, 70);
		SetStat(STAT_CHA, 50);
		}
		break;
	}

	// Players gain 3 stats points for each level up to and including 60.
	// They also received 10 free stat points on creation. 
	m_sPoints = 10 + (GetLevel() - 1) * 3;

	// For every level after 60, we add an additional two points.
	if (GetLevel() > 60)
		m_sPoints += 2 * (GetLevel() - 60);

	statTotal = GetStatTotal();
	ASSERT(statTotal == 290);

	SetUserAbility();
	Send2AI_UserUpdateInfo();

	byStr = GetStat(STAT_STR);
	bySta = GetStat(STAT_STA), 
	byDex = GetStat(STAT_DEX);
	byInt = GetStat(STAT_INT),
	byCha = GetStat(STAT_CHA);

	result << uint8(1) // result (success)
		<< GetCoins()
		<< byStr << bySta << byDex << byInt << byCha 
		<< m_iMaxHp << m_iMaxMp << m_sTotalHit << MaxWeight(m_sMaxWeight) << m_sPoints;
	Send(&result);
	return;

fail_return:
	result << bResult << temp_money;
	Send(&result);
}

void CUser::GoldChange(short tid, int gold)
{
	if (m_bZone < 3) return;	// Money only changes in Frontier zone and Battle zone!!!
	if (m_bZone == ZONE_SNOW_BATTLE
		|| m_bZone == ZONE_DESPERATION_ABYSS 
		|| m_bZone == ZONE_HELL_ABYSS 
		|| m_bZone == ZONE_DRAGON_CAVE 
		|| m_bZone == ZONE_CAITHAROS_ARENA)
		return;

	CUser* pTUser = g_pMain->GetUserPtr(tid);
	if (pTUser == nullptr || pTUser->m_iGold <= 0)
		return;

	// Reward money in war zone
	if (gold == 0)
	{
		// If we're not in a party, we can distribute cleanly.
		if (!isInParty())
		{
			GoldGain((pTUser->m_iGold * 4) / 10);
			pTUser->GoldLose(pTUser->m_iGold / 2);
			return;
		}

		// Otherwise, if we're in a party, we need to divide it up.
		_PARTY_GROUP* pParty = g_pMain->GetPartyPtr(GetPartyID());
		if (pParty == nullptr)
			return;

		int userCount = 0, levelSum = 0, temp_gold = (pTUser->m_iGold * 4) / 10;
		pTUser->GoldLose(pTUser->m_iGold / 2);

		// TODO: Clean up the party system. 
		for (int i = 0; i < MAX_PARTY_USERS; i++)
		{
			CUser *pUser = g_pMain->GetUserPtr(pParty->uid[i]);
			if (pUser == nullptr)
				continue;

			userCount++;
			levelSum += pUser->GetLevel();
		}

		// No users (this should never happen! Needs to be cleaned up...), don't bother with the below loop.
		if (userCount == 0) 
			return;

		for (int i = 0; i < MAX_PARTY_USERS; i++)
		{		
			CUser * pUser = g_pMain->GetUserPtr(pParty->uid[i]);
			if (pUser == nullptr)
				continue;

			pUser->GoldGain((int)(temp_gold * (float)(pUser->GetLevel() / (float)levelSum)));
		}			
		return;
	}

	// Otherwise, use the coin amount provided.

	// Source gains money
	if (gold > 0)
	{
		GoldGain(gold);
		pTUser->GoldLose(gold);
	}
	// Source loses money
	else
	{
		GoldLose(gold);
		pTUser->GoldGain(gold);
	}
}

void CUser::SelectWarpList(Packet & pkt)
{
	if (isDead())
		return;

	uint16 /*npcid,*/ warpid;
	pkt /*>> npcid*/ >> warpid;

	_WARP_INFO *pWarp = GetMap()->GetWarp(warpid);
	if (pWarp == nullptr
		|| (pWarp->sNation != 0 && pWarp->sNation != GetNation()))
		return;

	C3DMap *pMap = g_pMain->GetZoneByID(pWarp->sZone);
	if (pMap == nullptr)
		return;

	_ZONE_SERVERINFO *pInfo = g_pMain->m_ServerArray.GetData(pMap->m_nServerNo);
	if (pInfo == nullptr)
		return;

	float rx = 0.0f, rz = 0.0f;
	rx = (float)myrand(0, (int)pWarp->fR * 2);
	if (rx < pWarp->fR)
		rx = -rx;

	rz = (float)myrand(0, (int)pWarp->fR * 2);
	if (rz < pWarp->fR)
		rz = -rz;

	if (m_bZone == pWarp->sZone) 
	{
		m_bZoneChangeSameZone = true;

		Packet result(WIZ_WARP_LIST, uint8(2));
		result << uint8(1);
		Send(&result);
	}

	ZoneChange(pWarp->sZone, pWarp->fX + rx, pWarp->fZ + rz);

	if (GetZoneID() == pWarp->sZone && pWarp->dwPay > 0 && hasCoins(pWarp->dwPay))
		GoldLose(pWarp->dwPay);
}

void CUser::ServerChangeOk(Packet & pkt)
{
	C3DMap* pMap = GetMap();
	if (pMap == nullptr)
		return;

	uint16 warpid = pkt.read<uint16>();
	_WARP_INFO* pWarp = pMap->GetWarp(warpid);
	if (pWarp == nullptr)
		return;

	float rx = 0.0f, rz = 0.0f;
	rx = (float)myrand(0, (int)pWarp->fR * 2);
	if (rx < pWarp->fR)
		rx = -rx;

	rz = (float)myrand(0, (int)pWarp->fR * 2);
	if (rz < pWarp->fR)
		rz = -rz;

	ZoneChange(pWarp->sZone, pWarp->fX + rx, pWarp->fZ + rz);
}

bool CUser::GetWarpList(int warp_group)
{
	Packet result(WIZ_WARP_LIST, uint8(1));
	C3DMap* pMap = GetMap();
	set<_WARP_INFO*> warpList;

	if(pMap == nullptr)
		return false; 

	pMap->GetWarpList(warp_group, warpList);

	result << uint16(warpList.size());
	foreach (itr, warpList)
	{
		C3DMap *pDstMap = g_pMain->GetZoneByID((*itr)->sZone);
		if (pDstMap == nullptr)
			continue;

		if (g_pMain->isWarOpen() 
			&& ((g_pMain->m_byBattleZoneType != ZONE_ARDREAM 
			&& ((*itr)->sZone == ZONE_ARDREAM 
			|| (*itr)->sZone == ZONE_RONARK_LAND_BASE
			|| (*itr)->sZone == ZONE_RONARK_LAND))
			|| (g_pMain->m_byBattleZoneType == ZONE_ARDREAM 
			&& (*itr)->sZone == ZONE_ARDREAM)))
			continue;

		result	<< (*itr)->sWarpID 
			<< (*itr)->strWarpName << (*itr)->strAnnounce
			<< (*itr)->sZone
			<< pDstMap->m_sMaxUser
			<< uint32((*itr)->dwPay);
	}

	Send(&result);
	return true;
}

bool CUser::BindObjectEvent(_OBJECT_EVENT *pEvent)
{
	if (pEvent->sBelong != 0 && pEvent->sBelong != GetNation())
		return false;

	Packet result(WIZ_OBJECT_EVENT, uint8(pEvent->sType));

	m_sBind = pEvent->sIndex;

	result << uint8(1);
	Send(&result);
	return true;
}

bool CUser::GateLeverObjectEvent(_OBJECT_EVENT *pEvent, int nid)
{
	_OBJECT_EVENT *pGateEvent;
	CNpc* pNpc, *pGateNpc;
	_KNIGHTS_SIEGE_WARFARE *pKnightSiegewars = g_pMain->GetSiegeMasterKnightsPtr(1);
	CKnights *pKnight = g_pMain->GetClanPtr(GetClanID());

	// Does the lever (object) NPC exist?
	if ((pNpc = g_pMain->GetNpcPtr(nid)) == nullptr
		// Does the corresponding gate object event exist?
			|| (pGateEvent = GetMap()->GetObjectEvent(pEvent->sControlNpcID)) == nullptr
			// Does the corresponding gate (object) NPC exist?
			|| (pGateNpc = g_pMain->FindNpcInZone(pEvent->sControlNpcID,GetZoneID())) == nullptr
			// Is it even a gate?
			|| !pGateNpc->isGate()
			// If the gate's closed (i.e. the lever is down), we can't open it unless the lever isn't nation-specific
			// or we're the correct nation. Seems the other nation cannot close them.
			|| (pNpc->isGateClosed() && pNpc->GetNation() != 0 && pNpc->GetNation() != GetNation()))
			return false;
	if ((GetZoneID() == ZONE_DELOS && pKnightSiegewars->sMasterKnights != 0 && pKnight->m_sIndex == pKnightSiegewars->sMasterKnights && isClanLeader())
		|| ((GetFame() == COMMAND_CAPTAIN || isKing()) &&  GetZoneID() != ZONE_DELOS) && pNpc->GetNation() == GetNation())
	{
	// Move the lever (up/down).
	pNpc->SendGateFlag(!pNpc->m_byGateOpen);
	
	// Open/close the gate.
	pGateNpc->SendGateFlag(!pGateNpc->m_byGateOpen);
	return true;
	}
	else
	return false;
}

/***
* Not sure what this is used for, so keeping logic the same just in case.
***/
bool CUser::FlagObjectEvent(_OBJECT_EVENT *pEvent, int nid)
{
	_OBJECT_EVENT *pFlagEvent;
	CNpc *pNpc, *pFlagNpc;

	// Does the flag object NPC exist?
	if ((pNpc = g_pMain->GetNpcPtr(nid)) == nullptr
		// Does the corresponding flag event exist?
			|| (pFlagEvent = GetMap()->GetObjectEvent(pEvent->sControlNpcID)) == nullptr
			// Does the corresponding flag object NPC exist?
			|| (pFlagNpc = g_pMain->FindNpcInZone(pEvent->sControlNpcID, GetZoneID())) == nullptr
			// Is this marked a gate? (i.e. can control)
			|| !pFlagNpc->isGate()
			// Is the war over or the gate closed?
			|| g_pMain->m_bVictory > 0 || pNpc->isGateClosed())
			return false;

	// Reset objects
	pNpc->SendGateFlag(0);
	pFlagNpc->SendGateFlag(0);

	// Add flag score (not sure what this is, is this even used anymore?)
	if (GetNation() == KARUS) 
		g_pMain->m_bKarusFlag++;
	else
		g_pMain->m_bElmoradFlag++;

	// Did one of the teams win?
	g_pMain->BattleZoneVictoryCheck();	
	return true;
}

bool CUser::WarpListObjectEvent(_OBJECT_EVENT *pEvent)
{
	// If the warp gate belongs to a nation, which isn't us...
	if (pEvent->sBelong != 0 && pEvent->sBelong != GetNation()
		// or we're in the opposing nation's zone...
			|| (GetZoneID() != GetNation() && GetZoneID() <= ELMORAD)
			// or we're unable to retrieve the warp list...
			|| !GetWarpList(pEvent->sControlNpcID)) 
			return false;

	return true;
}

void CUser::ObjectEvent(Packet & pkt)
{
	if (g_pMain->m_bPointCheckFlag == false
		|| isDead())
		return;

	bool bSuccess = false;
	uint16 objectindex, nid;
	pkt >> objectindex >> nid;

	_OBJECT_EVENT * pEvent = GetMap()->GetObjectEvent(objectindex);
	if (pEvent != nullptr
		&& isInRange(pEvent->fPosX, pEvent->fPosZ, MAX_OBJECT_RANGE))
	{
		switch (pEvent->sType)
		{
		case OBJECT_GATE:
		case OBJECT_BIND:
		case OBJECT_REMOVE_BIND:
			bSuccess = BindObjectEvent(pEvent);
			break;

		case OBJECT_GATE_LEVER:
			bSuccess = GateLeverObjectEvent(pEvent, nid);
			break;

		case OBJECT_FLAG_LEVER:
			bSuccess = FlagObjectEvent(pEvent, nid);
			break;

		case OBJECT_WARP_GATE:
			bSuccess = WarpListObjectEvent(pEvent);
			if (bSuccess)
				return;
			break;

		case OBJECT_ANVIL:
			SendAnvilRequest(nid);
			return;
		}

	}

	if (!bSuccess)
	{
		Packet result(WIZ_OBJECT_EVENT, uint8(pEvent == nullptr ? 0 : pEvent->sType));
		result << uint8(0);
		Send(&result);
	}
}

void CUser::SendAnvilRequest(uint16 sNpcID, uint8 bType /*= ITEM_UPGRADE_REQ*/)
{
	Packet result(WIZ_ITEM_UPGRADE, uint8(bType));
	result << sNpcID;
	Send(&result);
}

void CUser::UpdateVisibility(InvisibilityType bNewType)
{
	Packet result(AG_USER_VISIBILITY);
	m_bInvisibilityType = (uint8)(bNewType);
	result << GetID() << m_bInvisibilityType;
	Send_AIServer(&result);
}

/**
* @brief	Forces a reset of the GM's persistent visibility 
* 			state.
*/
void CUser::ResetGMVisibility()
{
	if (!isGM()
		|| isTransformed())
		return;

	// Force the client to reset
	if (m_bAbnormalType != ABNORMAL_INVISIBLE)
	{
		// Only send this packet to the GM as other users 
		// will already see the GM as invisible.
		Packet result(WIZ_STATE_CHANGE);
		result << GetID() << uint8(5) << uint32(0);
		Send(&result);
	}

	m_bAbnormalType = ABNORMAL_INVISIBLE;
}

void CUser::BlinkStart()
{
#if !defined(DISABLE_PLAYER_BLINKING)
	// Don't blink in zones where we can attack the other nation (note: this includes the arena!)
	if (GetMap()->canAttackOtherNation())
		return;

	m_bAbnormalType = ABNORMAL_BLINKING;
	m_tBlinkExpiryTime = UNIXTIME + BLINK_TIME;
	m_bRegeneType = REGENE_ZONECHANGE;

	UpdateVisibility(INVIS_DISPEL_ON_ATTACK); // AI shouldn't see us
	m_bInvisibilityType = INVIS_NONE; // but players should. 

	StateChangeServerDirect(3, ABNORMAL_BLINKING);
#endif
}

void CUser::BlinkTimeCheck()
{
	if (UNIXTIME < m_tBlinkExpiryTime)
		return;

	m_bRegeneType = REGENE_NORMAL;

	StateChangeServerDirect(3, ABNORMAL_NORMAL);

	Packet result(AG_USER_REGENE);
	result	<< GetSocketID() << m_sHp;
	Send_AIServer(&result);

	result.Initialize(AG_USER_INOUT);
	result.SByte(); // TODO: Remove this redundant uselessness that is mgame
	result	<< uint8(INOUT_RESPAWN) << GetSocketID()
		<< GetName()
		<< GetX() << GetZ();
	Send_AIServer(&result);

	UpdateVisibility(INVIS_NONE);
}

void CUser::GoldGain(uint32 gold, bool bSendPacket /*= true*/, bool bApplyBonus /*= false*/)
{
	// Assuming it works like this, although this affects (probably) all gold gained (including kills in PvP zones)
	// If this is wrong and it should ONLY affect gold gained from monsters, let us know!
	if (bApplyBonus)
		gold = gold * (m_bNoahGainAmount + m_bItemNoahGainAmount) / 100;

	if (m_iGold + gold > COIN_MAX)
		m_iGold = COIN_MAX;
	else
		m_iGold += gold;

	if (bSendPacket)
	{
		Packet result(WIZ_GOLD_CHANGE, uint8(CoinGain));
		result << gold << GetCoins();
		Send(&result);	
	}
}

bool CUser::GoldLose(uint32 gold, bool bSendPacket /*= true*/)
{
	if (!hasCoins(gold)) 
		return false;

	m_iGold -= gold;

	if (bSendPacket)
	{
		Packet result(WIZ_GOLD_CHANGE, uint8(CoinLoss));
		result << gold << GetCoins();
		Send(&result);	
	}
	return true;
}

bool CUser::CheckSkillPoint(uint8 skillnum, uint8 min, uint8 max)
{
	if (skillnum < 5 || skillnum > 8) 
		return false;

	return (m_bstrSkill[skillnum] >= min && m_bstrSkill[skillnum] <= max);
}

bool CUser::CheckClass(short class1, short class2, short class3, short class4, short class5, short class6)
{
	return (JobGroupCheck(class1) || JobGroupCheck(class2) || JobGroupCheck(class3) || JobGroupCheck(class4) || JobGroupCheck(class5) || JobGroupCheck(class6));
}

bool CUser::JobGroupCheck(short jobgroupid)
{
	if (jobgroupid > 100) 
		return GetClass() == jobgroupid;

	ClassType subClass = GetBaseClassType();
	switch (jobgroupid) 
	{
	case GROUP_WARRIOR:
		return (subClass == ClassWarrior || subClass == ClassWarriorNovice || subClass == ClassWarriorMaster);

	case GROUP_ROGUE:
		return (subClass == ClassRogue || subClass == ClassRogueNovice || subClass == ClassRogueMaster);

	case GROUP_MAGE:
		return (subClass == ClassMage || subClass == ClassMageNovice || subClass == ClassMageMaster);

	case GROUP_CLERIC:	
		return (subClass == ClassPriest || subClass == ClassPriestNovice || subClass == ClassPriestMaster);
	}

	return (subClass == jobgroupid);
}

void CUser::TrapProcess()
{
	// If the time interval has passed
	if ((UNIXTIME - m_tLastTrapAreaTime) >= ZONE_TRAP_INTERVAL)
	{
		if(GetZoneID() == ZONE_BIFROST)
			SendUserStatusUpdate(USER_STATUS_BLIND,USER_STATUS_INFLICT);

		HpChange(-ZONE_TRAP_DAMAGE, this);
		m_tLastTrapAreaTime = UNIXTIME;
	}
}

void CUser::KickOutZoneUser(bool home, uint8 nZoneID)
{
	if (home)
	{
		C3DMap* pMap = g_pMain->GetZoneByID(nZoneID);
		if (pMap == nullptr) 
			return;

		int eventID = 0;
		int random = myrand(0, 9000);
		if (random >= 0 && random < 3000)			eventID = 0;
		else if (random >= 3000 && random < 6000)	eventID = 1;
		else if (random >= 6000 && random < 9001)	eventID = 2;

		_REGENE_EVENT* pRegene = pMap->GetRegeneEvent(eventID);
		if (pRegene == nullptr) 
		{
			KickOutZoneUser();
			return;
		}

		ZoneChange(pMap->m_nZoneNumber, 
			pRegene->fRegenePosX + (float)myrand(0, (int)pRegene->fRegeneAreaX), 
			pRegene->fRegenePosZ + (float)myrand(0, (int)pRegene->fRegeneAreaZ));
		return;
	}

	// Teleport the player to their native zone.
	short x, z;

	if (GetStartPosition(x,z,GetNation()))
		ZoneChange(GetNation(), x, z);
	else
	{
		TRACE("### KickOutZoneUser - StartPosition not found : Nation=%d",GetNation());
	}
}

void CUser::NativeZoneReturn()
{
	_START_POSITION *pStartPosition = g_pMain->m_StartPositionArray.GetData(m_bNation);
	if (pStartPosition == nullptr) 
		return; 

	m_bZone = m_bNation;

	m_curx = (float)((m_bNation == KARUS ? pStartPosition->sKarusX : pStartPosition->sElmoradX) + myrand(0, pStartPosition->bRangeX));
	m_curz = (float)((m_bNation == KARUS ? pStartPosition->sKarusZ : pStartPosition->sElmoradZ) + myrand(0, pStartPosition->bRangeZ)); 
}

/**
* @brief	Sends a packet to all players within the 
* 			user's current region and surrounding regions 
* 			(i.e. visible area)
*
* @param	pkt		   	The packet.
* @param	pExceptUser	User to except. If specified, will ignore this user.
*/
void CUser::SendToRegion(Packet *pkt, CUser *pExceptUser /*= nullptr*/, uint16 nEventRoom /*-1*/)
{
	g_pMain->Send_Region(pkt, GetMap(), GetRegionX(), GetRegionZ(), pExceptUser, nEventRoom);
}

/**
* @brief	Sends a packet to all players within the 
* 			user's current zone.
*
* @param	pkt		   	The packet.
* @param	pExceptUser	User to except. If specified, will ignore this user.
*/
void CUser::SendToZone(Packet *pkt, CUser *pExceptUser /*= nullptr*/, uint16 nEventRoom /*-1*/, float fRange)
{
	g_pMain->Send_Zone(pkt, GetZoneID(), pExceptUser, 0, nEventRoom, fRange);
}

void CUser::OnDeath(Unit *pKiller)
{
	if (m_bResHpType == USER_DEAD)
		return;

	m_bResHpType = USER_DEAD;

	// Player is dead stop other process.
	ResetWindows();

	if (GetFame() == COMMAND_CAPTAIN)
	{
		if (GetNation() == KARUS)
			g_pMain->Announcement(KARUS_CAPTAIN_DEPRIVE_NOTIFY, KARUS, 8, this);
		else
			g_pMain->Announcement(ELMORAD_CAPTAIN_DEPRIVE_NOTIFY, ELMORAD, 8, this);
	}

	InitType3();
	InitType4();

	DateTime time;

	if (pKiller != nullptr)
	{
		DeathNoticeType noticeType = DeathNoticeNone;

		if (pKiller->isNPC())
		{
			CNpc *pNpc = TO_NPC(pKiller);

			int64 nExpLost = 0;

			if (pNpc->GetType() == NPC_PATROL_GUARD || (GetZoneID() != GetNation() && GetZoneID() <= ELMORAD))
				nExpLost = m_iMaxExp / 100;
			else
				nExpLost = m_iMaxExp / 20;

			if ((pNpc->GetType() == NPC_GUARD_TOWER1 || pNpc->GetType() == NPC_GUARD_TOWER2) && isInPKZone())
				noticeType = DeathNotice;

			if (GetPremiumProperty(PremiumExpRestorePercent) > 0)
				nExpLost = nExpLost * (GetPremiumProperty(PremiumExpRestorePercent)) / 100;

			g_pMain->WriteDeathUserLogFile(string_format("[ NPC/MONSTER - %d:%d:%d ] SID=%d,Killer=%s,Target=%s,Zone=%d,X=%d,Z=%d,TargetExp=%d,LostExp=%d\n",time.GetHour(),time.GetMinute(),time.GetSecond(),pNpc->GetProtoID(),pKiller->GetName().c_str(),GetName().c_str(),GetZoneID(),uint16(GetX()),uint16(GetZ()),m_iExp, nExpLost));
			
			if (GetZoneID() != ZONE_FORGOTTEN_TEMPLE)
			ExpChange(-nExpLost);	

			if (GetZoneID() == ZONE_FORGOTTEN_TEMPLE)
			{
				KickOutZoneUser(true);
				return;
			}
		}
		else
		{
			CUser *pUser = TO_USER(pKiller);

			// Looks like we died of "natural causes!" (probably residual DOT)
			if (pUser == this)
			{
				m_sWhoKilledMe = -1;
			}
			// Someone else killed us? Need to clean this up.
			else
			{
				if (GetZoneID() == ZONE_CHAOS_DUNGEON)
				{
					noticeType = DeathNoticeCoordinates;
					RobChaosSkillItems();
					m_DeathCount++;
					UpdatePlayerRank();

					pUser->m_KillCount++;
					pUser->UpdatePlayerRank();
				}
				else
				{
					// Did we get killed in the snow war? Handle appropriately.
					if (GetZoneID() == ZONE_SNOW_BATTLE
						&& g_pMain->m_byBattleOpen == SNOW_BATTLE)
					{
						pUser->GoldGain(SNOW_EVENT_MONEY);
						pUser->LoyaltyChange(GetID());

						if (GetNation() == KARUS)
							g_pMain->m_sKarusDead++;
						else
							g_pMain->m_sElmoradDead++;
					}
					// All zones other than the snow war.
					else if (GetZoneID() == ZONE_DELOS
						&& g_pMain->m_byBattleOpen == CLAN_BATTLE)
					{
						pUser->LoyaltyChange(GetID());
					}
					else
					{
						if (isInArena())
						{
							// Show death notices in the arena
							noticeType = DeathNoticeCoordinates;
						}
						else
						{
							uint16 bonusNP = 0;
							bool bKilledByRival = false;

							if (!GetMap()->isWarZone() && g_pMain->m_byBattleOpen != NATION_BATTLE)
							{
								// Show death notices in PVP zones
								noticeType = DeathNoticeCoordinates;

								// If the killer has us set as their rival, reward them & remove the rivalry.
								bKilledByRival = (!pUser->hasRivalryExpired() && pUser->GetRivalID() == GetID());
								if (bKilledByRival)
								{
									// If we are our killer's rival, use the rival notice instead.
									noticeType = DeathNoticeRival;

									// Apply bonus NP for rival kills
									bonusNP += RIVALRY_NP_BONUS;

									// This player is no longer our rival
									pUser->RemoveRival();
								}

								// The anger gauge is increased on each death.
								// When your anger gauge is full (5 deaths), you can use the "Anger Explosion" skill.
								if (!hasFullAngerGauge())
									UpdateAngerGauge(++m_byAngerGauge);

							}

							// Loyalty should be awarded on kill.
							if (!pUser->isInParty())
								pUser->LoyaltyChange(GetID(), bonusNP);
							// In parties, the loyalty should be divided up across the party.
							// Each party member in range should also receive a "Meat Dumpling".
							else
								pUser->LoyaltyDivide(GetID(), bonusNP);

							if (!pUser->GetMap()->isWarZone())
								pUser->GoldChange(GetID(), 0);

							if (GetZoneID() != GetNation() && GetZoneID() <= ELMORAD)
							{
								int64 nExpLost = m_iMaxExp / 100;

								if (GetPremiumProperty(PremiumExpRestorePercent) > 0)
									nExpLost = nExpLost * (GetPremiumProperty(PremiumExpRestorePercent)) / 100;

								ExpChange(-nExpLost);
							}

							// If we don't have a rival, this player is now our rival for 3 minutes.
							if (isInPKZone()
								&& !hasRival())
								SetRival(pUser);

							if (GetNation() == KARUS)
							pUser->QuestV2MonsterCountAdd(KARUS);
							else if(GetNation() == ELMORAD)
							pUser->QuestV2MonsterCountAdd(ELMORAD);
						}
					}
				}

				m_sWhoKilledMe = pUser->GetID();
			}

			string pKillerPartyUsers;
			string pTargetPartyUsers;

			if (GetZoneID() != ZONE_CHAOS_DUNGEON && (pUser->isInParty() || isInParty()))
			{
				CUser *pPartyUser;
				_PARTY_GROUP *pParty = g_pMain->GetPartyPtr(pUser->GetPartyID());
				if (pParty)
				{
					for (int i = 0; i < MAX_PARTY_USERS; i++)
					{
						pPartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
						if (pPartyUser)
							pKillerPartyUsers += string_format("%s,",pPartyUser->GetName().c_str());
					}
				}

				pParty = g_pMain->GetPartyPtr(GetPartyID());
				if (pParty)
				{
					for (int i = 0; i < MAX_PARTY_USERS; i++)
					{
						pPartyUser = g_pMain->GetUserPtr(pParty->uid[i]);
						if (pPartyUser)
							pTargetPartyUsers += string_format("%s,",pPartyUser->GetName().c_str());
					}
				}

				if (!pKillerPartyUsers.empty())
					pKillerPartyUsers = pKillerPartyUsers.substr(0,pKillerPartyUsers.length() - 1);

				if (!pTargetPartyUsers.empty())
					pTargetPartyUsers = pTargetPartyUsers.substr(0,pTargetPartyUsers.length() - 1);
			}

			if (pKillerPartyUsers.empty() && pTargetPartyUsers.empty())
				g_pMain->WriteDeathUserLogFile(string_format("[ USER - %d:%d:%d ] Killer=%s,Target=%s,Zone=%d,X=%d,Z=%d,LoyaltyKiller=%d,LoyaltyMonthlyKiller=%d,LoyaltyTarget=%d,LoyaltyMonthlyTarget=%d\n",time.GetHour(),time.GetMinute(),time.GetSecond(),pKiller->GetName().c_str(),GetName().c_str(),GetZoneID(),uint16(GetX()),uint16(GetZ()),TO_USER(pKiller)->GetLoyalty(),TO_USER(pKiller)->GetMonthlyLoyalty(),GetLoyalty(),GetMonthlyLoyalty()));
			else if (pKillerPartyUsers.empty() && !pTargetPartyUsers.empty())
				g_pMain->WriteDeathUserLogFile(string_format("[ USER - %d:%d:%d ] Killer=%s,Target=%s,TargetParty=%s,Zone=%d,X=%d,Z=%d,LoyaltyKiller=%d,LoyaltyMonthlyKiller=%d,LoyaltyTarget=%d,LoyaltyMonthlyTarget=%d\n",time.GetHour(),time.GetMinute(),time.GetSecond(),pKiller->GetName().c_str(),GetName().c_str(), pTargetPartyUsers.c_str(),GetZoneID(),uint16(GetX()),uint16(GetZ()),TO_USER(pKiller)->GetLoyalty(),TO_USER(pKiller)->GetMonthlyLoyalty(),GetLoyalty(),GetMonthlyLoyalty()));
			else if (!pKillerPartyUsers.empty() && pTargetPartyUsers.empty())
				g_pMain->WriteDeathUserLogFile(string_format("[ USER - %d:%d:%d ] Killer=%s,KillerParty=%s,Target=%s,Zone=%d,X=%d,Z=%d,LoyaltyKiller=%d,LoyaltyMonthlyKiller=%d,LoyaltyTarget=%d,LoyaltyMonthlyTarget=%d\n",time.GetHour(),time.GetMinute(),time.GetSecond(),pKiller->GetName().c_str(),pKillerPartyUsers.c_str(),GetName().c_str(),GetZoneID(),uint16(GetX()),uint16(GetZ()),TO_USER(pKiller)->GetLoyalty(),TO_USER(pKiller)->GetMonthlyLoyalty(),GetLoyalty(),GetMonthlyLoyalty()));
			else if (!pKillerPartyUsers.empty() && !pTargetPartyUsers.empty())
				g_pMain->WriteDeathUserLogFile(string_format("[ USER - %d:%d:%d ] Killer=%s,KillerParty=%s,Target=%s,TargetParty=%s,Zone=%d,X=%d,Z=%d,LoyaltyKiller=%d,LoyaltyMonthlyKiller=%d,LoyaltyTarget=%d,LoyaltyMonthlyTarget=%d\n",time.GetHour(),time.GetMinute(),time.GetSecond(),pKiller->GetName().c_str(),pKillerPartyUsers.c_str(),GetName().c_str(), pTargetPartyUsers.c_str(),GetZoneID(),uint16(GetX()),uint16(GetZ()),TO_USER(pKiller)->GetLoyalty(),TO_USER(pKiller)->GetMonthlyLoyalty(),GetLoyalty(),GetMonthlyLoyalty()));
		}

		if (noticeType != DeathNoticeNone)
			SendDeathNotice(pKiller,noticeType); 
	}

	Unit::OnDeath(pKiller);
}

/**
* @brief	Updates the player's anger gauge level, setting it to
* 			byAngerGauge.
*
* @param	byAngerGauge	The anger gauge level.
*/
void CUser::UpdateAngerGauge(uint8 byAngerGauge)
{
	Packet result(WIZ_PVP, uint8(byAngerGauge == 0 ? PVPResetHelmet : PVPUpdateHelmet));

	if (byAngerGauge > MAX_ANGER_GAUGE)
		byAngerGauge = MAX_ANGER_GAUGE;

	m_byAngerGauge = byAngerGauge;
	if (byAngerGauge > 0)
		result << byAngerGauge << hasFullAngerGauge();

	Send(&result);
}

// We have no clan handler, we probably won't need to implement it (but we'll see).
void CUser::SendClanUserStatusUpdate(bool bToRegion /*= true*/)
{
	Packet result(WIZ_KNIGHTS_PROCESS, uint8(KNIGHTS_MODIFY_FAME));
	result	<< uint8(1) << GetSocketID() 
		<< GetClanID() << GetFame();

	// TODO: Make this region code user-specific to perform faster.
	if (bToRegion)
		SendToRegion(&result);
	else
		Send(&result);
}

void CUser::SendPartyStatusUpdate(uint8 bStatus, uint8 bResult /*= 0*/)
{
	if (!isInParty())
		return;

	Packet result(WIZ_PARTY, uint8(PARTY_STATUSCHANGE));
	result << GetSocketID() << bStatus << bResult;
	g_pMain->Send_PartyMember(GetPartyID(), &result);
}

void CUser::HandleHelmet(Packet & pkt)
{
	if (isDead())
		return;

	Packet result(WIZ_HELMET);
	pkt >> m_bIsHidingHelmet;
#if __VERSION >= 1900
	// pkt >> cospre flag
#endif
	result	<< m_bIsHidingHelmet 
#if __VERSION >= 1900
		//			<< cospre flag
#endif
		<< uint32(GetSocketID());
	SendToRegion(&result);
}

bool Unit::isInAttackRange(Unit * pTarget, _MAGIC_TABLE * pSkill /*= nullptr*/)
{
	if (pTarget == nullptr)
		return false;

	if (pTarget == this
		|| !isPlayer())
		return true;

	const float fBaseMeleeRange		= 15.0f; // far too generous
	const float fBaseRangedRange	= 65.0f;

	float fRange = fBaseMeleeRange, fWeaponRange = 0.0f;

	_ITEM_DATA * pItem = nullptr;
	_ITEM_TABLE * pTable = TO_USER(this)->GetItemPrototype(RIGHTHAND, pItem);

	if (pTable != nullptr
		&& pItem->sDuration > 0)
	{
		fWeaponRange = pTable->m_sRange;
	}
	else
	{
		pTable = TO_USER(this)->GetItemPrototype(LEFTHAND, pItem);
		if (pTable != nullptr
			&& pItem->sDuration != 0)
			fWeaponRange = pTable->m_sRange;
	}

	if (pSkill != nullptr)
	{
		// Not an attack skill, don't need to enforce these restrictions.
		if (pSkill->bMoral != MORAL_ENEMY && pSkill->bMoral > MORAL_PARTY)
			return true;

		// For physical attack skills (type 1 - melee, type 2 - ranged), we'll need take into account 
		// the weapon's range.
		if (pSkill->bType[0] != 3)
			fRange = fWeaponRange;

		// For physical melee & magic skills, try to use the skill's range if it's set.
		// Need to allow more for lag, and poorly thought out skill ranges.
		// If not, resort to using the weapon range -- or predefined 15m range in the case of type 3 skills.
		if (pSkill->bType[0] != 2)
		{
			return isInRangeSlow(pTarget, fBaseMeleeRange + (pSkill->sRange == 0 ? fRange : pSkill->sRange) + (pSkill->bType[0] == 1 ? fWeaponRange : 0)); 
		}
		// Ranged skills (type 2) don't typically have the main skill range set to anything useful, so
		// we need to allow for the: bow's range, flying skill-specific range, and an extra 50m for the
		// also extremely poorly thought out ranges.
		else
		{
			_MAGIC_TYPE2 * pType2 = g_pMain->m_Magictype2Array.GetData(pSkill->iNum);
			return pType2 != nullptr && isInRangeSlow(pTarget, fRange + pType2->sAddRange + fBaseRangedRange);
		}
	}

	// Regular attack range.
	if (fWeaponRange != 0.0f)
		fRange = fBaseMeleeRange + fWeaponRange;

	return isInRangeSlow(pTarget, fRange);
}

/**
* @brief	Determine if we can use the specified item
* 			via the magic/skill system.
*
* @param	itemid	The ID of the item.
* @param	count 	Stack (can probably disregard, as it's always 1).
*
* @return	true if we can use item, false if not.
*/
bool CUser::CanUseItem(uint32 nItemID, uint32 sCount /*= 1*/)
{
	_ITEM_TABLE* pItem = pItem = g_pMain->GetItemPtr(nItemID);
	if (pItem == nullptr)
		return false;

	// Disable scroll usage while transformed.
	if (isTransformed())
	{
		// Various NPC transformations ("Transform Scrolls") are exempt from this rule -- it's just monsters.
		// Also, siege transformations can use their own buff scrolls.
		if (isNPCTransformation() && isSiegeTransformation())
			return false;
	}

	// If the item's class specific, ensure it can be used by this user.
	if ((pItem->m_bClass != 0 && !JobGroupCheck(pItem->m_bClass))
		// Check the item's level requirement
			|| (GetLevel() < pItem->m_bReqLevel || GetLevel() > pItem->m_bReqLevelMax)
			// Ensure the item exists.
			|| !CheckExistItem(nItemID, sCount))
			return false;

	return true;
}

void CUser::SendUserStatusUpdate(UserStatus type, UserStatusBehaviour status)
{
	Packet result(WIZ_ZONEABILITY, uint8(2));
	result << uint8(type) << uint8(status);
	/*
	1				, 0 = Cure damage over time
	1				, 1 = Damage over time
	2				, 0 = Cure poison
	2				, 1 = poison (purple)
	3				, 0 = Cure disease
	3				, 1 = disease (green)
	4				, 1 = blind
	5				, 0 = Cure grey HP
	5				, 1 = HP is grey (not sure what this is)
	*/
	Send(&result);

	if (isInParty())
		SendPartyStatusUpdate(type, status);
}

/**
* @brief	Gets an item's prototype from a slot in a player's inventory.
*
* @param	pos	The position of the item in the player's inventory.
* @returns	nullptr if an invalid position is specified, or if there's no item at that position.
* 			The item's prototype (_ITEM_TABLE *) otherwise.
*/
_ITEM_TABLE* CUser::GetItemPrototype(uint8 pos, _ITEM_DATA *& pItem)
{
	if (pos >= INVENTORY_TOTAL)
		return nullptr;

	pItem = GetItem(pos);
	return pItem->nNum == 0 ? nullptr : g_pMain->GetItemPtr(pItem->nNum);
}

/**
* @brief	Checks & removes any expired skills from
* 			the saved magic list.
*/
void CUser::CheckSavedMagic()
{
	Guard lock(m_savedMagicLock);
	if (m_savedMagicMap.empty())
		return;

	set<uint32> deleteSet;
	foreach (itr, m_savedMagicMap)
	{
		if (itr->second <= UNIXTIME)
			deleteSet.insert(itr->first);
	}
	foreach (itr, deleteSet)
		m_savedMagicMap.erase(*itr);
}

/**
* @brief	Inserts a skill to the saved magic list
* 			to persist across zone changes/logouts.
*
* @param	nSkillID 	Identifier for the skill.
* @param	sDuration	The duration.
*/
void CUser::InsertSavedMagic(uint32 nSkillID, uint16 sDuration)
{
	Guard lock(m_savedMagicLock);
	UserSavedMagicMap::iterator itr = m_savedMagicMap.find(nSkillID);

	// If the buff is already in the savedBuffMap there's no need to add it again!
	if (itr != m_savedMagicMap.end())
		return;

	m_savedMagicMap.insert(make_pair(nSkillID, UNIXTIME + sDuration));
}

/**
* @brief	Removes the specified skill from the saved magic list.
*
* @param	nSkillID	Identifier for the skill.
*/
void CUser::RemoveSavedMagic(uint32 nSkillID)
{
	Guard lock(m_savedMagicLock);
	m_savedMagicMap.erase(nSkillID);
}

/**
* @brief	Checks if the given skill ID is already in our 
* 			saved magic list.
*
* @param	nSkillID	Identifier for the skill.
*
* @return	true if the skill exists in the saved magic list, false if not.
*/
bool CUser::HasSavedMagic(uint32 nSkillID)
{
	Guard lock(m_savedMagicLock);
	return m_savedMagicMap.find(nSkillID) != m_savedMagicMap.end();
}

/**
* @brief	Gets the duration for a saved skill, 
* 			if applicable.
*
* @param	nSkillID	Identifier for the skill.
*
* @return	The saved magic duration.
*/
int16 CUser::GetSavedMagicDuration(uint32 nSkillID)
{
	Guard lock(m_savedMagicLock);
	auto itr = m_savedMagicMap.find(nSkillID);
	if (itr == m_savedMagicMap.end())
		return 0;

	return int16(itr->second - UNIXTIME);
}

/**
* @brief	Recasts any saved skills on login/zone change.
*/
void CUser::RecastSavedMagic(uint8 buffType /* = 0*/)
{
	Guard lock(m_savedMagicLock);
	UserSavedMagicMap castSet;
	foreach (itr, m_savedMagicMap)
	{
		if (itr->first != 0 || itr->second != 0) 
			castSet.insert(make_pair(itr->first, itr->second));
	}

	if (castSet.empty())
		return;

	foreach (itr, castSet)
	{
		if (buffType > 0)
		{
			_MAGIC_TYPE4 * pType = g_pMain->m_Magictype4Array.GetData(itr->first);

			if (pType == nullptr)
				continue;

			if (pType->bBuffType != buffType)
				continue;
		}

		MagicInstance instance;
		instance.sCasterID = GetID();
		instance.sTargetID = GetID();
		instance.nSkillID = itr->first;
		instance.bIsRecastingSavedMagic = true;

		instance.Run();
	}
}

/**
* @brief	Recasts any lockable scrolls on debuff.
*/
void CUser::RecastLockableScrolls(uint8 buffType)
{
	InitType4(false, buffType);
	RecastSavedMagic(buffType);
}


/**
* @brief	Displays the player rankings board in PK zones, 
* 			when left-ALT is held.
*
* @param	pkt	The packet.
*/
void CUser::HandlePlayerRankings(Packet & pkt)
{
	if (g_pMain->m_IsPlayerRankingUpdateProcess)
		return;

	uint8 nRankType = 0;
	pkt >> nRankType;

	Packet result(WIZ_RANK, nRankType);

	uint16 nMyRank = 0;
	uint16 sCount = 0;
	size_t wpos = 0;

	std::vector<_USER_RANKING> UserRankingSorted[NONE]; // 0 = Karus, 1 = Human and 2 = Both Nations

	for (int nation = KARUS_ARRAY; nation <= ELMORAD_ARRAY; nation++)
	{
		foreach_stlmap (itr, g_pMain->m_UserRankingArray[nation])
			UserRankingSorted[nRankType == RANK_TYPE_CHAOS_DUNGEON ? NONE - 1 : nation].push_back(*itr->second);

		if (nRankType == RANK_TYPE_PK_ZONE
			|| nRankType == RANK_TYPE_ZONE_BORDER_DEFENSE_WAR)
		{
			sCount = 0;
			wpos = result.wpos();
			result << sCount;

			std::sort(UserRankingSorted[nation].begin(), UserRankingSorted[nation].end(),
				[] (_USER_RANKING const &a, _USER_RANKING const &b ){ return a.m_iLoyaltyDaily > b.m_iLoyaltyDaily; });

			if ((uint32)UserRankingSorted[nation].size() > 0)
			{
				// Get my rank...
				if ((nation + 1) == GetNation())
				{
					for (int i = 0; i < (int32)UserRankingSorted[nation].size(); i++)
					{
						if (GetZoneID() != UserRankingSorted[nation][i].m_bZone)
							continue;

						nMyRank++;

						if (UserRankingSorted[nation][i].m_socketID == GetSocketID())
							break;
					}
				}

				for (int i = 0; i < (int32)UserRankingSorted[nation].size(); i++)
				{
					if ((nRankType == RANK_TYPE_PK_ZONE && sCount > 9) 
						|| (nRankType == RANK_TYPE_ZONE_BORDER_DEFENSE_WAR && sCount > 7))
						break;

					_USER_RANKING * pRankInfo = &UserRankingSorted[nation][i];

					if (pRankInfo == nullptr)
						continue;

					if (GetZoneID() == pRankInfo->m_bZone 
						&& GetEventRoom() == pRankInfo->m_bEventRoom)
					{
						CUser *pUser = g_pMain->GetUserPtr(pRankInfo->m_socketID);

						if (pUser == nullptr)
							continue;

						if (!pUser->isInGame())
							continue;

						result << pUser->GetName() << true;

						CKnights * pKnights = g_pMain->GetClanPtr(pUser->GetClanID());

						if (pKnights == nullptr)
							result	<< uint16(0) << uint16(0) << (std::string)"";
						else
							result	<< pKnights->GetID() << pKnights->m_sMarkVersion << pKnights->GetName();

						result << pRankInfo->m_iLoyaltyDaily;

						if(nRankType == RANK_TYPE_PK_ZONE)
							result << pRankInfo->m_iLoyaltyPremiumBonus;

						sCount++;
					}
				}
			}

			result.put(wpos, sCount);
			wpos = result.wpos();
		}
	}

	if (nRankType == RANK_TYPE_CHAOS_DUNGEON && (uint32)UserRankingSorted[NONE-1].size() > 0)
	{
		std::sort(UserRankingSorted[NONE-1].begin(), UserRankingSorted[NONE-1].end(),
			[]( _USER_RANKING const &a, _USER_RANKING const &b ){ return a.m_KillCount > b.m_KillCount; });

		// Get Event Room Users count
		result << uint8(g_pMain->TempleEventGetRoomUsers(GetEventRoom()));

		for (int i = 0; i < (int32)UserRankingSorted[NONE-1].size(); i++)
		{
			_USER_RANKING * pRankInfo = &UserRankingSorted[NONE-1][i];

			if (pRankInfo == nullptr)
				continue;

			if (GetSocketID() == pRankInfo->m_socketID)
				continue;

			if (GetZoneID() == pRankInfo->m_bZone 
				&& GetEventRoom() == pRankInfo->m_bEventRoom)
			{
				CUser *pUser = g_pMain->GetUserPtr(pRankInfo->m_socketID);

				if (pUser == nullptr)
					continue;

				if (!pUser->isInGame())
					continue;

				result << pUser->GetName()
					<< pRankInfo->m_KillCount << pRankInfo->m_DeathCount;
			}
		}
	}

	if (nRankType == RANK_TYPE_PK_ZONE)
		result  << nMyRank << m_iLoyaltyDaily << m_iLoyaltyPremiumBonus;
	else if (nRankType == RANK_TYPE_ZONE_BORDER_DEFENSE_WAR)
		result << int32(100000) << int32(50000);
	else if (nRankType == RANK_TYPE_CHAOS_DUNGEON)
	{
		int64 nGainedExp = int64(pow(GetLevel(),3) * 0.15 * (5 * m_KillCount - m_DeathCount));
		int64 nPremiumGainedExp = nGainedExp * 2;

		if (nGainedExp > 8000000)
			nGainedExp = 8000000;

		if (nPremiumGainedExp > 8000000)
			nPremiumGainedExp = 8000000;

		result << GetName()
			<< m_KillCount << m_DeathCount
			<< nGainedExp << nPremiumGainedExp;
	}

	Send(&result);
}

uint16 CUser::GetPlayerRank(uint8 nRankType)
{
	uint16 nMyRank = 0;
	uint8 nRankArrayIndex = (nRankType == RANK_TYPE_PK_ZONE 
		|| nRankType == RANK_TYPE_ZONE_BORDER_DEFENSE_WAR 
		? GetNation() -1
		:  NONE-1);

	std::vector<_USER_RANKING> UserRankingSorted[NONE]; // 0 = Karus, 1 = Human and 2 = Both Nations

	for (int nation = KARUS_ARRAY; nation <= ELMORAD_ARRAY; nation++)
	{
		foreach_stlmap (itr, g_pMain->m_UserRankingArray[nation])
			UserRankingSorted[nRankType == RANK_TYPE_CHAOS_DUNGEON ? NONE -1 : nation].push_back(*itr->second);
	}

	if (nRankArrayIndex < ELMORAD)
	{
		std::sort(UserRankingSorted[nRankArrayIndex].begin(), UserRankingSorted[nRankArrayIndex].end(),
			[] (_USER_RANKING const &a, _USER_RANKING const &b ){ return a.m_iLoyaltyDaily > b.m_iLoyaltyDaily; });
	}
	else if (nRankArrayIndex == ELMORAD)
	{
		std::sort(UserRankingSorted[nRankArrayIndex].begin(), UserRankingSorted[nRankArrayIndex].end(),
			[]( _USER_RANKING const &a, _USER_RANKING const &b ){ return a.m_KillCount > b.m_KillCount; });
	}

	for (int i = 0; i < (int32)UserRankingSorted[nRankArrayIndex].size(); i++)
	{
		_USER_RANKING * pRankInfo = &UserRankingSorted[nRankArrayIndex][i];

		if (pRankInfo)
		{
			if  (GetZoneID() == pRankInfo->m_bZone
				&& GetEventRoom() == pRankInfo->m_bEventRoom)
			{

				nMyRank++;

				if (GetSocketID() == pRankInfo->m_socketID)
					break;
			}
		}
	}

	return nMyRank;
}

/**
* @brief	Handles packets related to the mining system.
* 			Also handles soccer-related packets (yuck).
*
* @param	pkt	The packet.
*/
void CUser::HandleMiningSystem(Packet & pkt)
{
	uint8 opcode;
	pkt >> opcode;

	switch (opcode)
	{
	case MiningStart:
		HandleMiningStart(pkt);
		break;

	case MiningAttempt:
		HandleMiningAttempt(pkt);
		break;

	case MiningStop:
		HandleMiningStop(pkt);
		break;

	case MiningSoccer:
		HandleSoccer(pkt);
		break;
	}
}

/**
* @brief	Handles users requesting to start mining.
* 			NOTE: This is a mock-up, so be warned that it does not 
* 			handle checks such as identifying if the user is allowed
* 			to mine in this area.
*
* @param	pkt	The packet.
*/
void CUser::HandleMiningStart(Packet & pkt)
{
	Packet result(WIZ_MINING, uint8(MiningStart));
	uint16 resultCode = MiningResultSuccess;

	// Are we mining already?
	if (isMining())
		resultCode = MiningResultMiningAlready;

	// Do we have a pickaxe? Is it worn?
	_ITEM_DATA * pItem;
	_ITEM_TABLE * pTable = GetItemPrototype(RIGHTHAND, pItem);
	if (pItem == nullptr || pTable == nullptr
		|| pItem->sDuration <= 0
		|| !pTable->isPickaxe())
		resultCode = MiningResultNotPickaxe;

	result << resultCode;

	// If nothing went wrong, allow the user to start mining.
	// Be sure to let everyone know we're mining.
	if (resultCode == MiningResultSuccess)
	{
		m_bMining = true;
		result << GetID();
		SendToRegion(&result);
	}
	else
	{
		Send(&result);
	}
}

/**
* @brief	Handles a user's mining attempt by finding a random reward (or none at all).
* 			This is sent automatically by the client every MINING_DELAY (5) seconds.
*
* @param	pkt	The packet.
*/
void CUser::HandleMiningAttempt(Packet & pkt)
{
	if (!isMining())
		return;

	Packet result(WIZ_MINING, uint8(MiningAttempt));
	uint16 resultCode = MiningResultSuccess;

	// Do we have a pickaxe? Is it worn?
	_ITEM_DATA * pItem;
	_ITEM_TABLE * pTable = GetItemPrototype(RIGHTHAND, pItem);
	if (pItem == nullptr || pTable == nullptr
		|| pItem->sDuration <= 0 // are we supposed to wear the pickaxe on use? Need to verify.
		|| !pTable->isPickaxe())
		resultCode = MiningResultNotPickaxe;

	// Check to make sure we're not spamming the packet...
	if ((UNIXTIME - m_tLastMiningAttempt) < MINING_DELAY)
		resultCode = MiningResultMiningAlready; // as close an error as we're going to get...

	// Effect to show to clients
	uint16 sEffect = 0;

	// This is just a mock-up based on another codebase's implementation.
	// Need to log official data to get a proper idea of how it behaves, rate-wise,
	// so that we can then implement it more dynamically.
	if (resultCode == MiningResultSuccess)
	{
		int rate = myrand(1, 100), random = myrand(1, 10000);

		if (GetPremiumProperty(PremiumDropPercent) > 0)
		{
			rate += (rate / 100) * GetPremiumProperty(PremiumDropPercent);
			random += (rate / 100) * GetPremiumProperty(PremiumDropPercent);
		}

		if (pTable->m_iNum == GOLDEN_MATTOCK)
		{
			rate += (rate / 100) * 10;
			random += (random / 100) * 10;
		}

		if (rate > 100)
			rate = 100;
		if (random > 10000)
			random = 10000;

		if (rate <= 50 && random <= 5000)
		{
			ExpChange(1);
			sEffect = 13082; // "XP" effect
		}
		else if (rate >= 50 && rate <= 75 && random <= 7500)
		{
			GiveItem(SLING);
			sEffect = 13081; // "Item" effect
		}
		else if (rate >= 75 && rate <= 100 && random <= 10000)
		{
			if (pTable->m_iNum == MATTOCK)
				GiveItem(MYSTERIOUS_ORE);
			else if(pTable->m_iNum == GOLDEN_MATTOCK)
				GiveItem(MYSTERIOUS_GOLD_ORE);

			sEffect = 13081; // "Item" effect
		}
		else
		{
			resultCode = MiningResultNothingFound;
		}
		m_tLastMiningAttempt = UNIXTIME;
	}

	result << resultCode << GetID() << sEffect;

	ItemWoreOut(ATTACK,100); 

	if (resultCode != MiningResultSuccess
		&& resultCode != MiningResultNothingFound)
	{
		// Tell us the error first
		Send(&result);

		// and then tell the client to stop mining
		HandleMiningStop(pkt);
		return;
	}

	if(resultCode != MiningResultNothingFound)
		SendToRegion(&result);
	else if(resultCode == MiningResultNothingFound)
		Send(&result);
}

/**
* @brief	Handles when a user stops mining.
*
* @param	pkt	The packet.
*/
void CUser::HandleMiningStop(Packet & pkt)
{
	if (!isMining())
		return;

	Packet result(WIZ_MINING, uint8(MiningStop));
	result << uint16(1) << GetID();
	m_bMining = false;
	SendToRegion(&result);
}

void CUser::HandleSoccer(Packet & pkt)
{
}

void CUser::InitializeStealth()
{
	Packet pkt(WIZ_STEALTH);
	pkt << uint8(0) << uint16(0);
	Send(&pkt);
}

void CUser::GrantChickenManner()
{
	uint8 bLevel = GetLevel(), bManner = 0;
	// No manner points if you're not a chicken anymore nor when you're not in a party.
	if (!m_bIsChicken || !isInParty())
		return;

	_PARTY_GROUP *pParty = nullptr;
	pParty = g_pMain->GetPartyPtr(GetPartyID());

	if(pParty == nullptr)
		return;

	for (int i = 0; i < MAX_PARTY_USERS; i++)
	{
		CUser *pTargetUser = nullptr;
		if (pParty->uid[i] != GetSocketID())
			pTargetUser = g_pMain->GetUserPtr(pParty->uid[i]);

		if (pTargetUser == nullptr 
			|| pTargetUser->isDead() 
			|| pTargetUser->m_bIsChicken)
			continue;

		if (!isInRange(pTargetUser, RANGE_50M))
			continue;

		if (pTargetUser->GetLevel() > 20 && pTargetUser->GetLevel() < 40)
			bManner = pTargetUser->GetLevel() / 10;
		else
			bManner = 1;

		pTargetUser->SendMannerChange(bManner);
	}
}

void CUser::SendMannerChange(int32 iMannerPoints)
{
	//Make sure we don't have too many or too little manner points!
	if(m_iMannerPoint + iMannerPoints > LOYALTY_MAX)
		m_iMannerPoint = LOYALTY_MAX;
	else if (m_iMannerPoint + iMannerPoints < 0)
		m_iMannerPoint = 0;
	else
		m_iMannerPoint += iMannerPoints;

	Packet pkt(WIZ_LOYALTY_CHANGE, uint8(LOYALTY_MANNER_POINTS));
	pkt << m_iMannerPoint;
	Send(&pkt);
}

uint8 CUser::GetUserDailyOp(uint8 type)
{
	if (type == 0)
		return 0;

	int32 nUnixTime = -1;

	UserDailyOpMap::iterator itr = g_pMain->m_UserDailyOpMap.find(GetName());

	if (itr != g_pMain->m_UserDailyOpMap.end())
	{
		if (type == DAILY_CHAOS_MAP)
			nUnixTime = int(itr->second->ChaosMapTime);
		else if (type == DAILY_USER_RANK_REWARD)
			nUnixTime = int(itr->second->UserRankRewardTime);
		else if (type == DAILY_USER_PERSONAL_RANK_REWARD)
			nUnixTime = int(itr->second->PersonalRankRewardTime);
		else if (type == DAILY_KING_WING)
			nUnixTime = int(itr->second->KingWingTime);
		else if (type == DAILY_WARDER_KILLER_WING1)
			nUnixTime = int(itr->second->WarderKillerTime1);
		else if (type == DAILY_WARDER_KILLER_WING2)
			nUnixTime = int(itr->second->WarderKillerTime2);
		else if (type == DAILY_KEEPER_KILLER_WING)
			nUnixTime = int(itr->second->KeeperKillerTime);
		else if (type == DAILY_USER_LOYALTY_WING_REWARD)
			nUnixTime = int(itr->second->UserLoyaltyWingRewardTime);

		if (nUnixTime == -1)
			SetUserDailyOp(type);
		else
		{
			if (((int32(UNIXTIME) - nUnixTime) / 60) > DAILY_OPERATIONS_MINUTE)
				SetUserDailyOp(type);
			else
				return 0;
		}
	}
	else
		SetUserDailyOp(type, true);

	return 1;
}

void CUser::SetUserDailyOp(uint8 type, bool isInsert)
{
	if (type == 0)
		return;

	int32 nUnixTime = int32(UNIXTIME);

	if (isInsert)
	{
		_USER_DAILY_OP * pData = new _USER_DAILY_OP;

		pData->strUserId = GetName();
		pData->ChaosMapTime = -1;
		pData->UserRankRewardTime = -1;
		pData->PersonalRankRewardTime = -1;
		pData->KingWingTime = -1;
		pData->WarderKillerTime1 = -1;
		pData->WarderKillerTime2 = -1;
		pData->KeeperKillerTime = -1;
		pData->UserLoyaltyWingRewardTime = -1;

		if (type ==  DAILY_CHAOS_MAP)
			pData->ChaosMapTime = nUnixTime;
		else if (type == DAILY_USER_RANK_REWARD)
			pData->UserRankRewardTime = nUnixTime;
		else if (type == DAILY_USER_PERSONAL_RANK_REWARD)
			pData->PersonalRankRewardTime = nUnixTime;
		else if (type == DAILY_KING_WING)
			pData->KingWingTime = nUnixTime;
		else if (type == DAILY_WARDER_KILLER_WING1)
			pData->WarderKillerTime1 = nUnixTime;
		else if (type == DAILY_WARDER_KILLER_WING2)
			pData->WarderKillerTime2 = nUnixTime;
		else if (type == DAILY_KEEPER_KILLER_WING)
			pData->KeeperKillerTime = nUnixTime;
		else if (type == DAILY_USER_LOYALTY_WING_REWARD)
			pData->UserLoyaltyWingRewardTime = nUnixTime;

		g_pMain->m_UserDailyOpMap.insert(make_pair(pData->strUserId, pData));
		g_DBAgent.InsertUserDailyOp(pData);
	}
	else
	{
		UserDailyOpMap::iterator itr = g_pMain->m_UserDailyOpMap.find(GetName());
		if (itr != g_pMain->m_UserDailyOpMap.end())
		{
			if (type == DAILY_CHAOS_MAP)
				itr->second->ChaosMapTime = nUnixTime;
			else if (type == DAILY_USER_RANK_REWARD)
				itr->second->UserRankRewardTime = nUnixTime;
			else if (type == DAILY_USER_PERSONAL_RANK_REWARD)
				itr->second->PersonalRankRewardTime = nUnixTime;
			else if (type == DAILY_KING_WING)
				itr->second->KingWingTime = nUnixTime;
			else if (type == DAILY_WARDER_KILLER_WING1)
				itr->second->WarderKillerTime1 = nUnixTime;
			else if (type == DAILY_WARDER_KILLER_WING2)
				itr->second->WarderKillerTime2 = nUnixTime;
			else if (type == DAILY_KEEPER_KILLER_WING)
				itr->second->KeeperKillerTime = nUnixTime;
			else if (type == DAILY_USER_LOYALTY_WING_REWARD)
				itr->second->UserLoyaltyWingRewardTime = nUnixTime;

			g_DBAgent.UpdateUserDailyOp(GetName(), type, nUnixTime);
		}
	}
}

uint32 CUser::GetEventTrigger()
{
	CNpc *pNpc = g_pMain->GetNpcPtr(GetTargetID());
	if (pNpc == nullptr)
		return 0;

	foreach_stlmap (itr, g_pMain->m_EventTriggerArray) {
		_EVENT_TRIGGER *pEventTrigger = g_pMain->m_EventTriggerArray.GetData(itr->first);

		if (pEventTrigger == nullptr)
			continue;

		if (pNpc->m_tNpcType != pEventTrigger->bNpcType)
			continue;

		if (pNpc->m_byTrapNumber == pEventTrigger->sNpcID)
			return pEventTrigger->nTriggerNum;
	}

	return 0;
}

void CUser::RemoveStealth()
{
	if (this->m_bInvisibilityType != INVIS_NONE)
	{
		CMagicProcess::RemoveStealth(this, INVIS_DISPEL_ON_MOVE);
		CMagicProcess::RemoveStealth(this, INVIS_DISPEL_ON_ATTACK);
	}
}

void CUser::GivePremium(uint8 bPremiumType, uint16 sPremiumTime)
{
	if(GetPremium() > 0 
		|| bPremiumType <= 0 
		|| sPremiumTime <= 0)
		return;

	m_bPremiumType = bPremiumType;
	m_sPremiumTime = sPremiumTime * 24;
	m_bAccountStatus = 1;

	g_DBAgent.SavePremiumServiceUser(this);
	SendPremiumInfo();
}

void CUser::RobChaosSkillItems()
{
	if (GetItemCount(ITEM_LIGHT_PIT) > 0)
		RobItem(ITEM_LIGHT_PIT, GetItemCount(ITEM_LIGHT_PIT));
	if (GetItemCount(ITEM_DRAIN_RESTORE) > 0)
		RobItem(ITEM_DRAIN_RESTORE, GetItemCount(ITEM_DRAIN_RESTORE));
	if (GetItemCount(ITEM_KILLING_BLADE) > 0)
		RobItem(ITEM_KILLING_BLADE, GetItemCount(ITEM_KILLING_BLADE));
}

void CUser::SiegeWarFareNpc(Packet & pkt)
{
		uint8 opcode , type ;
		uint16 tarrif;
		pkt >> opcode >> type >> tarrif;
		_KNIGHTS_SIEGE_WARFARE *pKnightSiegeWarFare = g_pMain->GetSiegeMasterKnightsPtr(1);
		CKnights *pKnight = g_pMain->GetClanPtr(pKnightSiegeWarFare->sMasterKnights);
			
		Packet result(WIZ_SIEGE);
		switch (opcode)
		{
		case 3: //moradon npc
			{
				result << opcode << type;
					switch (type)
				{
					case 2:
						result << pKnightSiegeWarFare->sCastleIndex 
						<< uint16(pKnightSiegeWarFare->bySiegeType)
						<< pKnightSiegeWarFare->byWarDay
						<< pKnightSiegeWarFare->byWarTime 
						<< pKnightSiegeWarFare->byWarMinute;
						Send(&result);
						break;

					case 4:
						result.SByte();
						result 
						<< pKnightSiegeWarFare->sCastleIndex
						<< uint8(1)
						<< pKnight->GetName()
						<< pKnight->m_byNation 
						<< pKnight->m_sMembers
						<< pKnightSiegeWarFare->byWarRequestDay
						<< pKnightSiegeWarFare->byWarRequestTime 
						<< pKnightSiegeWarFare->byWarRequestMinute;
						Send(&result);
						break;

					case 5:
						result.SByte();
					result 
						<< pKnightSiegeWarFare->sCastleIndex 
						<< pKnightSiegeWarFare->bySiegeType
						<< pKnight->GetName()
						<< pKnight->m_byNation 
						<< pKnight->m_sMembers;
						Send(&result);
						break;

					default:
						break;
				}
			}break;

		case 4: //delos npc
			{
				result << opcode << type;
					switch (type)
					{
					case 2:
						pKnightSiegeWarFare->nDungeonCharge += pKnightSiegeWarFare->nDellosTax + pKnightSiegeWarFare->nMoradonTax;
						pKnightSiegeWarFare->nDellosTax = 0;
						pKnightSiegeWarFare->nMoradonTax = 0;
						GoldGain(pKnightSiegeWarFare->nDungeonCharge,true);
						pKnightSiegeWarFare->nDungeonCharge = 0;
						g_pMain->UpdateSiegeTax(0 , 0);
						break;
					case 3:
						result << pKnightSiegeWarFare->sCastleIndex
						<< pKnightSiegeWarFare->sMoradonTariff
						<< pKnightSiegeWarFare->sDellosTariff
						<< (pKnightSiegeWarFare->nDungeonCharge += pKnightSiegeWarFare->nDellosTax + pKnightSiegeWarFare->nMoradonTax);
						pKnightSiegeWarFare->nDellosTax = 0;
						pKnightSiegeWarFare->nMoradonTax = 0;
						Send(&result);
						break;
					case 4:
						pKnightSiegeWarFare->sMoradonTariff = tarrif;
						g_pMain->UpdateSiegeTax(ZONE_MORADON , tarrif);
						result << uint16(1) << tarrif << uint8(ZONE_MORADON);
						g_pMain->Send_All(&result);
						g_pMain->m_KnightsSiegeWarfareArray.GetData(pKnightSiegeWarFare->sMasterKnights);
						break;
					case 5:
						pKnightSiegeWarFare->sDellosTariff = tarrif;
						g_pMain->UpdateSiegeTax(ZONE_DELOS , tarrif);
						result << uint16(1) << tarrif << uint8(ZONE_DELOS);
						g_pMain->Send_All(&result);
						g_pMain->m_KnightsSiegeWarfareArray.GetData(pKnightSiegeWarFare->sMasterKnights);
						break;
					default:
						break;
					}
			}break;
		default:
			break;
		}
}

void CUser::LogosShout(Packet & pkt)
{
	uint8 opcode;
	string Notice;

	pkt >> opcode >> Notice;

	Packet result(WIZ_LOGOSSHOUT);
	result << opcode << Notice;
	Send(&result);
}