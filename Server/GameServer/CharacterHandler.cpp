#include "stdafx.h"
#include "DBAgent.h"

#pragma region Nation Transfer

uint8 CUser::NationChange()
{
	if(!CheckExistItem(ITEM_NATION_TRANSFER))
		return 1; // Item does exist.

	uint8 nRet = g_DBAgent.NationTransfer(GetAccountName());

	if (nRet > 0)
		return nRet; // 2 = Character(s) in the clan / 3 = Database error.

	RobItem(ITEM_NATION_TRANSFER);

	// if database operation is successfull but inline code have a problem disconnect to user...
	if (GetNewRace() == 0)
		Disconnect();

	m_bRank = 0;
	m_bTitle = 0;
	m_bRace = GetNewRace();
	m_sClass = GetNation() == KARUS ? GetClass() + 100 : GetClass() - 100;	
	m_bNation = GetNation() == KARUS ? ELMORAD : KARUS;

	if (GetHealth() < (GetMaxHealth() / 2))
		HpChange(GetMaxHealth());

	SendMyInfo();

	UserInOut(INOUT_OUT);
	SetRegion(GetNewRegionX(), GetNewRegionZ());
	UserInOut(INOUT_WARP);

	g_pMain->UserInOutForMe(this);
	g_pMain->NpcInOutForMe(this);
	g_pMain->MerchantUserInOutForMe(this);

	ResetWindows();

	InitType4();
	RecastSavedMagic();

	return 0; // Successfull
}

uint8 CUser::GetNewRace()
{
	uint8 nNewRace = 0;

	if (GetNation() == KARUS)
	{
		if (m_bRace == KARUS_BIG)
			nNewRace = BABARIAN;
		else if (m_bRace == KARUS_MIDDLE)
			nNewRace = ELMORAD_MAN;
		else if (m_bRace == KARUS_SMALL)
			nNewRace = ELMORAD_MAN;
		else if (m_bRace == KARUS_WOMAN)
			nNewRace = ELMORAD_WOMAN;
	}
	else
	{
		if (m_bRace == BABARIAN)
			nNewRace = KARUS_BIG;
		// El Morad Male and El Morad Warriors
		else if (m_bRace == ELMORAD_MAN && isWarrior())
			nNewRace = KARUS_BIG;
		// El Morad Female and El Morad Warriors
		else if (m_bRace == ELMORAD_WOMAN && isWarrior())
			nNewRace = KARUS_BIG;
		// El Morad Male and El Morad Rogues
		else if (m_bRace == ELMORAD_MAN && isRogue())
			nNewRace = KARUS_MIDDLE;
		// El Morad Female and El Morad Rogues
		else if (m_bRace == ELMORAD_WOMAN && isRogue())
			nNewRace = KARUS_MIDDLE;
		// El Morad Male and El Morad Magicians
		else if (m_bRace == ELMORAD_MAN && isMage())
			nNewRace = KARUS_SMALL;
		// El Morad Female and El Morad Magicians
		else if (m_bRace == ELMORAD_WOMAN && isMage())
			nNewRace = KARUS_WOMAN;
		// El Morad Male and El Morad Priests
		else if (m_bRace == ELMORAD_MAN && isPriest())
			nNewRace = KARUS_MIDDLE;
		// El Morad Female and El Morad Priests
		else if (m_bRace == ELMORAD_WOMAN && isPriest())
			nNewRace = KARUS_WOMAN;
	}

	return nNewRace;
}

#pragma endregion

#pragma region Gender Change

bool CUser::GenderChange(uint8 Race /*= 0*/)
{
	if (Race == 0 || Race > 13)
		return false; 

	if(!CheckExistItem(ITEM_GENDER_CHANGE))
		return false; 

	RobItem(ITEM_GENDER_CHANGE);

	m_bRace = Race;

	g_DBAgent.UpdateUser(GetName(), UPDATE_PACKET_SAVE, this);

	if (GetHealth() < (GetMaxHealth() / 2))
		HpChange(GetMaxHealth());

	SendMyInfo();

	UserInOut(INOUT_OUT);
	SetRegion(GetNewRegionX(), GetNewRegionZ());
	UserInOut(INOUT_WARP);

	g_pMain->UserInOutForMe(this);
	g_pMain->NpcInOutForMe(this);
	g_pMain->MerchantUserInOutForMe(this);

	ResetWindows();

	InitType4();
	RecastSavedMagic();

	return true;
}

#pragma endregion

#pragma region Job Change

uint8 CUser::JobChange(uint8 NewJob /*= 0*/)
{
	uint8 bNewClass = 0, bNewRace = 0;
	uint8 bResult = 0;

	if (NewJob < 1 || NewJob > 4)
		return 5; // Unknown job is selected...

	if(!CheckExistItem(ITEM_JOB_CHANGE))
		return 6; // Job Change scroll is not found...

	for (int i = 0; i < SLOT_MAX; i++)
	{
		if (m_sItemArray[i].nNum) 
		{
			bResult = 7;
			break;
		}
	}

	if (bResult == 7)
	{
		Packet result(WIZ_CLASS_CHANGE, uint8(ALL_POINT_CHANGE));
		result << uint8(4) << int(0);
		Send(&result);
		return bResult; // While there are items equipped on you.
	}

	// If bResult between 1 and 4 character already selected job...

	// If selected a new job Warrior
	if (NewJob == 1)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Rogue, Magician, Priest
			if (isBeginnerRogue() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = KARUWARRIOR;
				bNewRace = KARUS_BIG;
			}
			// Skilled Karus Rogue, Magician, Priest
			else if (isNoviceRogue() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = BERSERKER;
				bNewRace = KARUS_BIG;
			}
			// Mastered Karus Rogue, Magician, Priest
			else if (isMasteredRogue() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = GUARDIAN;
				bNewRace = KARUS_BIG;
			}
		}
		else
		{
			// Beginner El Morad Rogue, Magician, Priest
			if (isBeginnerRogue() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = ELMORWARRRIOR;
				bNewRace = GetRace();
			}
			// Skilled El Morad Rogue, Magician, Priest
			else if (isNoviceRogue() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = BLADE;
				bNewRace = GetRace();
			}
			// Mastered El Morad Rogue, Magician, Priest
			else if (isMasteredRogue() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = PROTECTOR;
				bNewRace = GetRace();
			}
		}

		// Character already Warrior.
		if (bNewClass == 0 || bNewRace == 0)		
			bResult = NewJob;
	}

	// If selected a new job Rogue
	if (NewJob == 2)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Warrior, Magician, Priest
			if (isBeginnerWarrior() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = KARUROGUE;
				bNewRace = KARUS_MIDDLE;
			}
			// Skilled Karus Warrior, Magician, Priest
			else if (isNoviceWarrior() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = HUNTER;
				bNewRace = KARUS_MIDDLE;
			}
			// Mastered Karus Warrior, Magician, Priest
			else if (isMasteredWarrior() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = PENETRATOR;
				bNewRace = KARUS_MIDDLE;
			}
		}
		else
		{
			// Beginner El Morad Warrior, Magician, Priest
			if (isBeginnerWarrior() || isBeginnerMage() || isBeginnerPriest())
			{
				bNewClass = ELMOROGUE;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Skilled El Morad Warrior, Magician, Priest
			else if (isNoviceWarrior() || isNoviceMage() || isNovicePriest())
			{
				bNewClass = RANGER;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Mastered El Morad Warrior, Magician, Priest
			else if (isMasteredWarrior() || isMasteredMage() || isMasteredPriest())
			{
				bNewClass = ASSASSIN;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
		}

		// Character already Rogue.
		if (bNewClass == 0 || bNewRace == 0)
			bResult = NewJob;
	}

	// If selected a new job Magician
	if (NewJob == 3)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Warrior, Rogue, Priest
			if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerPriest())
			{
				bNewClass = KARUWIZARD;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE)
					bNewRace = KARUS_SMALL;
				else
					bNewRace = GetRace();
			}
			// Skilled Karus Warrior, Rogue, Priest
			else if (isNoviceWarrior() || isNoviceRogue() || isNovicePriest())
			{
				bNewClass = SORSERER;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE)
					bNewRace = KARUS_SMALL;
				else
					bNewRace = GetRace();
			}
			// Mastered Karus Warrior, Rogue, Priest
			else if (isMasteredWarrior() || isMasteredRogue() || isMasteredPriest())
			{
				bNewClass = NECROMANCER;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_MIDDLE)
					bNewRace = KARUS_SMALL;
				else
					bNewRace = GetRace();
			}
		}
		else
		{
			// Beginner El Morad Warrior, Rogue, Priest
			if (isBeginnerRogue() || isBeginnerRogue() || isBeginnerPriest())
			{
				bNewClass = ELMOWIZARD;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Skilled El Morad Warrior, Rogue, Priest
			else if (isNoviceWarrior() || isNoviceRogue() || isNovicePriest())
			{
				bNewClass = MAGE;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Mastered El Morad Warrior, Rogue, Priest
			else if (isMasteredWarrior() || isMasteredRogue() || isMasteredPriest())
			{
				bNewClass = ENCHANTER;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
		}

		// Character already Magician.
		if (bNewClass == 0 || bNewRace == 0)
			bResult = NewJob;
	}

	// If selected a new job Priest
	if (NewJob == 4)
	{
		if (GetNation() == KARUS)
		{
			// Beginner Karus Warrior, Rogue, Magician
			if (isBeginnerWarrior() || isBeginnerRogue() || isBeginnerMage())
			{
				bNewClass = KARUPRIEST;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
					bNewRace = KARUS_MIDDLE;
				else
					bNewRace = GetRace();
			}
			// Skilled Karus Warrior, Rogue, Magician
			else if (isNoviceWarrior() || isNoviceRogue() || isNoviceMage())
			{
				bNewClass = SHAMAN;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
					bNewRace = KARUS_MIDDLE;
				else
					bNewRace = GetRace();
			}
			// Mastered Karus Warrior, Rogue, Magician
			else if (isMasteredRogue() || isMasteredRogue() || isMasteredMage())
			{
				bNewClass = DARKPRIEST;

				if (GetRace() == KARUS_BIG || GetRace() == KARUS_SMALL)
					bNewRace = KARUS_MIDDLE;
				else
					bNewRace = GetRace();
			}
		}
		else
		{
			// Beginner El Morad Warrior, Rogue, Magician
			if (isBeginnerRogue() || isBeginnerRogue() || isBeginnerMage())
			{
				bNewClass = ELMOPRIEST;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Skilled El Morad Warrior, Rogue, Magician
			else if (isNoviceRogue() || isNoviceRogue() || isNoviceMage())
			{
				bNewClass = CLERIC;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
			// Mastered El Morad Warrior, Rogue, Magician
			else if (isMasteredRogue() || isMasteredRogue() || isMasteredMage())
			{
				bNewClass = DRUID;

				if (GetRace() == BABARIAN)
					bNewRace = ELMORAD_MAN;
				else
					bNewRace = GetRace();
			}
		}

		// Character already Priest.
		if (bNewClass == 0 || bNewRace == 0)
			bResult = NewJob;
	}

	if (bResult == 0)
	{
		RobItem(ITEM_JOB_CHANGE);

		m_sClass = bNewClass;
		m_bRace = bNewRace;

		// Reset stat and skill points...
		AllPointChange(true);
		AllSkillPointChange(true);

		g_DBAgent.UpdateUser(GetName(), UPDATE_PACKET_SAVE, this);

		if (GetHealth() < (GetMaxHealth() / 2))
			HpChange(GetMaxHealth());

		SendMyInfo();

		UserInOut(INOUT_OUT);
		SetRegion(GetNewRegionX(), GetNewRegionZ());
		UserInOut(INOUT_WARP);

		g_pMain->UserInOutForMe(this);
		g_pMain->NpcInOutForMe(this);
		g_pMain->MerchantUserInOutForMe(this);

		ResetWindows();

		InitType4();
		RecastSavedMagic();
	}

	return bResult;
}

#pragma endregion