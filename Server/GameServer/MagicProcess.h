#pragma once

class Packet;
class Unit;
struct _MAGIC_TABLE;
struct _MAGIC_TYPE4;

class CMagicProcess  
{
public:
	static void MagicPacket(Packet & pkt, Unit * pCaster = nullptr);
	static void UpdateAIServer(uint32 nSkillID, AISkillOpcode opcode, Unit * pTarget, Unit * pCaster = nullptr, bool bIsRecastingSavedMagic = false);
	static void CheckExpiredType6Skills(Unit * pTarget);
	static void CheckExpiredType9Skills(Unit * pTarget, bool bForceExpiration = false);
	static void RemoveStealth(Unit * pTarget, InvisibilityType bInvisibilityType);
	static bool UserRegionCheck(Unit * pSkillCaster, Unit * pSkillTarget, _MAGIC_TABLE * pSkill, int radius, short mousex = 0, short mousez = 0);
	static bool GrantType4Buff(_MAGIC_TABLE * pSkill, _MAGIC_TYPE4 *pType, Unit * pCaster, Unit *pTarget, bool bIsRecastingSavedMagic = false);
	static bool RemoveType4Buff(uint8 byBuffType, Unit *pTarget, bool bRemoveSavedMagic = true, bool bRecastSavedMagic = false);
	static bool IsBuff(_MAGIC_TYPE4 * pType);
};