#ifndef __DEFINES_H_
#define __DEFINES_H_

#include <windows.h>
#include <tchar.h>

#include <string>
#include <vector>

#include <float.h>

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "config.h"
#include "fl/fl.h"

#include "fl/fl_hor_slider.h"
#include "fl/fl_toggle_button.h"
#include "fl/fl_draw.h"
#include "fl/fl_table_row.h"

#include "fl/fl_tabs.h"
#include "fl/fl_group.h"
#include "fl/fl_input.h"
#include "fl/fl_float_input.h"
#include "fl/fl_int_input.h"
#include "fl/fl_menu_bar.h"
#include "fl/fl_choice.h"

#include "fl/filename.h"

#include "fl/fl_window.h"
#include "fl/gl.h"
#include "fl/fl_gl_window.h"
#include "fl/math.h"

//-----------------------------------------------------------------------------
#define NAME_LENGTH 0xFFF

//-----------------------------------------------------------------------------
typedef struct {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
} Vertex;

//-----------------------------------------------------------------------------
enum e_ItemType {
	ITEM_TYPE_PLUG = 1,
	ITEM_TYPE_PART,
	ITEM_TYPE_ICONONLY,
	ITEM_TYPE_GOLD = 9,
	ITEM_TYPE_SONGPYUN = 10,
	ITEM_TYPE_UNKNOWN = 0xFFFFFFFF
};

//-----------------------------------------------------------------------------
enum e_PlugType{
	PLUGTYPE_NORMAL=0,
	PLUGTYPE_CLOAK,
	PLUGTYPE_MAX=10,
	PLUGTYPE_UNDEFINED=0xffffffff
};

//-----------------------------------------------------------------------------
enum e_Race {
	RACE_ALL = 0,
	RACE_KA_ARKTUAREK = 1,
	RACE_KA_TUAREK = 2,
	RACE_KA_WRINKLETUAREK = 3,
	RACE_KA_PURITUAREK = 4,
	RACE_EL_BABARIAN = 11,
	RACE_EL_MAN = 12,
	RACE_EL_WOMEN = 13,
	RACE_NPC = 100,
	RACE_UNKNOWN = 0xffffffff
};

//-----------------------------------------------------------------------------
enum e_PartPosition	{
	PART_POS_UPPER = 0,
	PART_POS_LOWER,
	PART_POS_FACE,
	PART_POS_HANDS,
	PART_POS_FEET,
	PART_POS_HAIR_HELMET,
	PART_POS_COUNT,
	PART_POS_UNKNOWN = 0xffffffff
};

//-----------------------------------------------------------------------------
enum e_PlugPosition {
	PLUG_POS_RIGHTHAND = 0,
	PLUG_POS_LEFTHAND,
	PLUG_POS_BACK,
	PLUG_POS_KNIGHTS_GRADE,
	PLUG_POS_COUNT,
	PLUG_POS_UNKNOWN = 0xffffffff
};

//-----------------------------------------------------------------------------
enum e_ItemPosition {
	ITEM_POS_DUAL = 0,
	ITEM_POS_RIGHTHAND,
	ITEM_POS_LEFTHAND,
	ITEM_POS_TWOHANDRIGHT,
	ITEM_POS_TWOHANDLEFT,
	ITEM_POS_UPPER = 5,
	ITEM_POS_LOWER,
	ITEM_POS_HEAD,
	ITEM_POS_GLOVES,
	ITEM_POS_SHOES,
	ITEM_POS_EAR = 10,
	ITEM_POS_NECK,
	ITEM_POS_FINGER,
	ITEM_POS_SHOULDER,
	ITEM_POS_BELT,
	ITEM_POS_INVENTORY = 15,
	ITEM_POS_GOLD = 16,
	ITEM_POS_SONGPYUN = 17,
	ITEM_POS_UNKNOWN = 0xffffffff
};

//-----------------------------------------------------------------------------
struct __VertexSkinned {
	glm::vec3 vOrigin;
	int nAffect;
	int* pnJoints;
	float* pfWeights;

	__VertexSkinned() {memset(this, 0, sizeof(__VertexSkinned));}
	~__VertexSkinned() {delete [] pnJoints; delete [] pfWeights;}
};

//-----------------------------------------------------------------------------
struct __VertexXyzNormal : public glm::vec3 {
public:
	glm::vec3 n;
public:
	void Set(const glm::vec3& p, const glm::vec3& sn) {x = p.x; y = p.y; z = p.z; n = sn;}
	void Set(float xx, float yy, float zz, float nxx, float nyy, float nzz) {
		x = xx; y = yy; z = zz; n.x = nxx; n.y = nyy; n.z = nzz;
	}

	const __VertexXyzNormal& operator = (const glm::vec3& vec) {
		x = vec.x; y = vec.y; z = vec.z;
		return *this;
	}

	__VertexXyzNormal() {}
	__VertexXyzNormal(const glm::vec3& p, const glm::vec3& n) {this->Set(p, n);}
	__VertexXyzNormal(float sx, float sy, float sz, float xx, float yy, float zz) {
		this->Set(sx, sy, sz, xx, yy, zz);
	}
};

//-----------------------------------------------------------------------------
struct _N3Material {
	unsigned char data[92];
};

//-----------------------------------------------------------------------------
struct _N3Matrix44 {
	unsigned char data[64];
};

//-----------------------------------------------------------------------------
struct _N3VertexT1: public glm::vec3 {
	glm::vec3 n;
	float tu, tv;
};

//-----------------------------------------------------------------------------
struct _N3TexHeader {
	char szID[4];
	int nWidth;
	int nHeight;
	int Format;
	bool bMipMap;
};

//-----------------------------------------------------------------------------
struct _N3EdgeCollapse {
	int NumIndicesToLose;
	int NumIndicesToChange;
	int NumVerticesToLose;
	int iIndexChanges;
	int CollapseTo;
	bool bShouldCollapse;
};

//-----------------------------------------------------------------------------
struct _N3LODCtrlValue {
	float fDist;
	int iNumVertices;
};

//-----------------------------------------------------------------------------
enum _D3DFORMAT {
	D3DFMT_UNKNOWN,
	D3DFMT_DXT1 = 827611204,
	D3DFMT_DXT3 = 861165636,
	D3DFMT_DXT5 = 894720068,
	D3DFMT_A1R5G5B5 = 25,
	D3DFMT_A4R4G4B4 = 26,
	D3DFMT_R8G8B8 = 20,
	D3DFMT_A8R8G8B8 = 21,
	D3DFMT_X8R8G8B8 = 22
};

//-----------------------------------------------------------------------------
struct _ITEM_TABLE {
	SQLUINTEGER m_iNum;
	SQLCHAR m_sName[(NAME_LENGTH+1)];
	SQLCHAR m_bKind;
	SQLCHAR m_bSlot;
	SQLCHAR m_bRace;
	SQLCHAR m_bClass;
	SQLUSMALLINT m_sDamage;
	SQLUSMALLINT m_sDelay;
	SQLUSMALLINT m_sRange;
	SQLUSMALLINT m_sWeight;
	SQLUSMALLINT m_sDuration;
	SQLUINTEGER m_iBuyPrice;
	SQLUINTEGER m_iSellPrice;
	SQLSMALLINT m_sAc;
	SQLCHAR m_bCountable;
	SQLUINTEGER m_iEffect1;
	SQLUINTEGER m_iEffect2;
	SQLCHAR m_bReqLevel;
	SQLCHAR m_bReqLevelMax;
	SQLCHAR m_bReqRank;
	SQLCHAR m_bReqTitle;
	SQLCHAR m_bReqStr;
	SQLCHAR m_bReqSta;
	SQLCHAR m_bReqDex;
	SQLCHAR m_bReqIntel;
	SQLCHAR m_bReqCha;
	SQLCHAR m_bSellingGroup;
	SQLCHAR m_ItemType;
	SQLUSMALLINT m_sHitrate;
	SQLUSMALLINT m_sEvarate;
	SQLUSMALLINT m_sDaggerAc;
	SQLUSMALLINT m_sSwordAc;
	SQLUSMALLINT m_sMaceAc;
	SQLUSMALLINT m_sAxeAc;
	SQLUSMALLINT m_sSpearAc;
	SQLUSMALLINT m_sBowAc;
	SQLCHAR m_bFireDamage;
	SQLCHAR m_bIceDamage;
	SQLCHAR m_bLightningDamage;
	SQLCHAR m_bPoisonDamage;
	SQLCHAR m_bHPDrain;
	SQLCHAR m_bMPDamage;
	SQLCHAR m_bMPDrain;
	SQLCHAR m_bMirrorDamage;
	SQLSMALLINT m_sStrB;
	SQLSMALLINT m_sStaB;
	SQLSMALLINT m_sDexB;
	SQLSMALLINT m_sIntelB;
	SQLSMALLINT m_sChaB;
	SQLSMALLINT m_MaxHpB;
	SQLSMALLINT m_MaxMpB;
	SQLSMALLINT m_bFireR;
	SQLSMALLINT m_bColdR;
	SQLSMALLINT m_bLightningR;
	SQLSMALLINT m_bMagicR;
	SQLSMALLINT m_bPoisonR;
	SQLSMALLINT m_bCurseR;

	_ITEM_TABLE(void) {
		memset(this, 0x00, sizeof(_ITEM_TABLE));
	}
};

//-----------------------------------------------------------------------------
typedef struct __TABLE_ITEM_BASIC {
	DWORD dwID;
	BYTE byExtIndex;
	std::string szName;
	std::string szRemark;
	DWORD dwIDK0;
	BYTE byIDK1;
	DWORD dwIDResrc;
	DWORD dwIDIcon;
	DWORD dwSoundID0;
	DWORD dwSoundID1;
	BYTE byClass;
	BYTE byIsRobeType;
	BYTE byAttachPoint;
	BYTE byNeedRace;
	BYTE byNeedClass;
	short siDamage;
	short siAttackInterval;
	short siAttackRange;
	short siWeight;
	short siMaxDurability;
	int iPrice;
	int iPriceSale;
	short siDefense;
	BYTE byContable;
	DWORD dwEffectID1;
	DWORD dwEffectID2;
	char cNeedLevel;
	char cIDK2;
	BYTE byNeedRank;
	BYTE byNeedTitle;
	BYTE byNeedStrength;
	BYTE byNeedStamina;
	BYTE byNeedDexterity;
	BYTE byNeedInteli;
	BYTE byNeedMagicAttack;
	BYTE bySellGroup;
	BYTE byIDK3;
} TABLE_ITEM_BASIC;

#endif
