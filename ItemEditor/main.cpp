/*
*/

#include <windows.h>
#include <tchar.h>

#include <string>
#include <vector>

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>

#include "STLMap.h"
#include "N3TableBase.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "config.h"
#include "fl/fl.h"
#include "fl/fl_window.h"
#include "fl/fl_hor_slider.h"
#include "fl/fl_toggle_button.h"
#include "fl/math.h"
#include "fl/fl_draw.H"
#include "fl/gl.h"
#include "fl/fl_gl_window.h"
#include "fl/fl_table_row.H"

#define NAME_LENGTH 0xFFF

const int _gl_width = 380;
const int _gl_height = 1024/3;

//-----------------------------------------------------------------------------
struct _N3Material {
	unsigned char data[92];
};

struct _N3Matrix44 {
	unsigned char data[64];
};

struct _N3VertexT1: public glm::vec3 {
	glm::vec3 n;
	float tu, tv;
};

struct _N3TexHeader {
	char szID[4];
	int nWidth;
	int nHeight;
	int Format;
	bool bMipMap;
};

struct _N3EdgeCollapse {
	int NumIndicesToLose;
	int NumIndicesToChange;
	int NumVerticesToLose;
	int iIndexChanges;
	int CollapseTo;
	bool bShouldCollapse;
};

struct _N3LODCtrlValue {
	float fDist;
	int iNumVertices;
};

enum _D3DFORMAT {
	D3DFMT_UNKNOWN,
	D3DFMT_DXT1 = 827611204,
	D3DFMT_DXT3 = 861165636,
	D3DFMT_DXT5 = 894720068,
};

_N3TexHeader    HeaderOrg;
unsigned char*  compTexData;
int             compTexSize;
unsigned short* m_pIndices0;
_N3VertexT1*    m_pVertices0;
int             m_iMaxNumIndices0;
int             m_iMaxNumVertices0;

bool debugMode = true;

//-----------------------------------------------------------------------------
void N3LoadTexture(const char* szFN) {
	FILE* fpTexture = fopen(szFN, "rb");
	if(fpTexture == NULL) {
		fprintf(stderr, "ERROR: Missing texture %s\n", szFN);
		system("pause");
		exit(-1);
	}
	/*
	*/

	// NOTE: length of the texture name
	int nL0 = 0;
	fread(&nL0, sizeof(int), 1, fpTexture);

	// NOTE: if the texture has a name read it in
	char m_szName0[NAME_LENGTH] = {};

	if(nL0 > 0) {
		memset(&m_szName0, 0, (nL0+1));
		fread(&m_szName0, sizeof(char), nL0, fpTexture);
	}

	// NOTE: read in the header information
	fread(&HeaderOrg, sizeof(_N3TexHeader), 1, fpTexture);

	// NOTE: display debug info
	if(debugMode) {
		printf("\nTexName: %s\n", m_szName0);
		printf("H.szID    -> %c%c%c%hu\n",
			HeaderOrg.szID[0],
			HeaderOrg.szID[1],
			HeaderOrg.szID[2],
			(unsigned char) HeaderOrg.szID[3]
		);
		printf("H.nWidth  -> %d\n", HeaderOrg.nWidth);
		printf("H.nHeight -> %d\n", HeaderOrg.nHeight);
		printf("H.Format  -> %d\n", HeaderOrg.Format);
		printf("H.bMipMap -> %d\n\n", HeaderOrg.bMipMap);
	}

	// NOTE: the textures contain multiple mipmap data "blocks"
	switch(HeaderOrg.Format) {
		case D3DFMT_DXT1: {
			compTexSize = (HeaderOrg.nWidth*HeaderOrg.nHeight/2);
		} break;
		case D3DFMT_DXT3: {
			compTexSize = (HeaderOrg.nWidth*HeaderOrg.nHeight);
		} break;
		case D3DFMT_DXT5: {
			compTexSize = (HeaderOrg.nWidth*HeaderOrg.nHeight*2);
			printf("D3DFMT_DXT5 tex: need to verify size\n");
		} break;
	}

	if(compTexData) {
		delete compTexData;
		compTexData = NULL;
	}

	compTexData = new unsigned char[compTexSize];
	fread(compTexData, sizeof(unsigned char), compTexSize, fpTexture);

	int nMMC = 1;
	if(HeaderOrg.bMipMap) {
		// NOTE: calculate the number of MipMap which are possible with this
		// texture
		nMMC = 0;
		for(
			int nW=HeaderOrg.nWidth, nH=HeaderOrg.nHeight;
			nW>=4 && nH>=4;
			nW/=2, nH/=2
		) nMMC++;
	} else {
		// NOTE: not a mipmap
		printf("HeaderOrg.bMipMap tex: need to implement non-mipmap textures\n");
		system("pause");
		exit(-1);
	}

	/*
	*/
	fclose(fpTexture);
}

//-----------------------------------------------------------------------------
void N3LoadMesh(const char* szFN) {
	FILE* fpMesh = fopen(szFN, "rb");
	if(fpMesh == NULL) {
		fprintf(stderr, "\nERROR: Missing mesh %s\n", szFN);
		system("pause");
		exit(-1);
	}
	/*
	*/

	// NOTE: length of the name for the mesh
	int nL0 = 0;
	fread(&nL0, sizeof(int), 1, fpMesh);

	// NOTE: if the shape has a mesh name read it in
	char m_szName0[NAME_LENGTH] = {};

	if(nL0 > 0) {
		memset(&m_szName0, 0, (nL0+1));
		fread(&m_szName0, sizeof(char), nL0, fpMesh);
	}

	// NOTE: read in the number of "collapses"
	int m_iNumCollapses0;
	fread(&m_iNumCollapses0, sizeof(int), 1, fpMesh);

	// NOTE: read in the total index changes
	int m_iTotalIndexChanges0;
	fread(&m_iTotalIndexChanges0, sizeof(int), 1, fpMesh);

	// NOTE: read in the max num of vertices
	fread(&m_iMaxNumVertices0, sizeof(int), 1, fpMesh);

	// NOTE: read in the max num of indices
	fread(&m_iMaxNumIndices0, sizeof(int), 1, fpMesh);

	// NOTE: read in the min num of vertices
	int m_iMinNumVertices0;
	fread(&m_iMinNumVertices0, sizeof(int), 1, fpMesh);

	// NOTE: read in the min num of indices
	int m_iMinNumIndices0;
	fread(&m_iMinNumIndices0, sizeof(int), 1, fpMesh);

	// NOTE: free the previous vertex data
	if(m_pVertices0) {
		delete m_pVertices0;
		m_pVertices0 = NULL;
	}

	// NOTE: if there is a max vertex amount allocate space for it
	if(m_iMaxNumVertices0 > 0) {
		m_pVertices0 = new _N3VertexT1[m_iMaxNumVertices0];
		memset(m_pVertices0, 0, sizeof(_N3VertexT1)*m_iMaxNumVertices0);

		// NOTE: read in the vertex data
		fread(m_pVertices0, sizeof(_N3VertexT1), m_iMaxNumVertices0, fpMesh);
	}

	// NOTE: free the previous index data
	if(m_pIndices0) {
		delete m_pIndices0;
		m_pIndices0 = NULL;
	}

	// NOTE: if there is a max index amount allocate space for it
	if(m_iMaxNumIndices0 > 0) {
		m_pIndices0 = new unsigned short[m_iMaxNumIndices0];
		memset(m_pIndices0, 0, sizeof(unsigned short)*m_iMaxNumIndices0);

		// NOTE: read in the vertex data
		fread(m_pIndices0, sizeof(unsigned short), m_iMaxNumIndices0, fpMesh);
	}

	// NOTE: read in the "collapses" (I think this is used to set the vertices
	// based on how close the player is to the object)
	_N3EdgeCollapse* m_pCollapses = new _N3EdgeCollapse[(m_iNumCollapses0+1)];
	memset(&m_pCollapses[m_iNumCollapses0], 0, sizeof(_N3EdgeCollapse));

	if(m_iNumCollapses0 > 0) {
		fread(m_pCollapses, sizeof(_N3EdgeCollapse), m_iNumCollapses0, fpMesh);
		memset(&m_pCollapses[m_iNumCollapses0], 0, sizeof(_N3EdgeCollapse));
	}

	// NOTE: read in the index changes
	int* m_pAllIndexChanges = new int[m_iTotalIndexChanges0];

	if(m_iTotalIndexChanges0 > 0) {
		fread(m_pAllIndexChanges, sizeof(int), m_iTotalIndexChanges0, fpMesh);
	}

	// NOTE: read in m_iLODCtrlValueCount
	int m_iLODCtrlValueCount0;
	fread(&m_iLODCtrlValueCount0, sizeof(int), 1, fpMesh);

	// NOTE: read in the LODCtrls (current size seems to be 0)
	_N3LODCtrlValue* m_pLODCtrlValues = new _N3LODCtrlValue[m_iLODCtrlValueCount0];

	if(m_iLODCtrlValueCount0) {
		fread(m_pLODCtrlValues, sizeof(_N3LODCtrlValue), m_iLODCtrlValueCount0, fpMesh);
	}

	// NOTE: the actual number of indices and vertices for the specific
	// collapse
	if(m_pAllIndexChanges) {
		int m_iNumIndices = 0;
		int m_iNumVertices = 0;

		int c = 0;
		int LOD = 0;

		int iDiff = m_pLODCtrlValues[LOD].iNumVertices - m_iNumVertices;

		while(m_pLODCtrlValues[LOD].iNumVertices > m_iNumVertices) {
			if(c >= m_iNumCollapses0) break;
			if(m_pCollapses[c].NumVerticesToLose+m_iNumVertices > m_pLODCtrlValues[LOD].iNumVertices)
				break;

			m_iNumIndices += m_pCollapses[c].NumIndicesToLose;
			m_iNumVertices += m_pCollapses[c].NumVerticesToLose;
		
			int tmp0 = m_pCollapses[c].iIndexChanges;
			int tmp1 = tmp0+m_pCollapses[c].NumIndicesToChange;

			for(int i=tmp0; i<tmp1; i++) {
				m_pIndices0[m_pAllIndexChanges[i]] = m_iNumVertices-1;
			}

			c++;
		}

		// NOTE: if we break on a collapse that isn't intended to be one we
		// should collapse up to then keep collapsing until we find one
		while(m_pCollapses[c].bShouldCollapse) {
			/*
			- not sure if this is really needed
			*/
			if(c >= m_iNumCollapses0) break;

			m_iNumIndices += m_pCollapses[c].NumIndicesToLose;
			m_iNumVertices += m_pCollapses[c].NumVerticesToLose;
		
			int tmp0 = m_pCollapses[c].iIndexChanges;
			int tmp1 = tmp0+m_pCollapses[c].NumIndicesToChange;

			for(int i=tmp0; i<tmp1; i++) {
				m_pIndices0[m_pAllIndexChanges[i]] = m_iNumVertices-1;
			}

			c++;
		}
	}

	free(m_pLODCtrlValues);
	free(m_pAllIndexChanges);
	free(m_pCollapses);
	
	// NOTE: display debug info
	if(debugMode) {
		printf("\nMeshName: %s\n", m_szName0);
		printf("m_iNumCollapses      -> %d\n", m_iNumCollapses0);
		printf("m_iTotalIndexChanges -> %d\n", m_iTotalIndexChanges0);
		printf("m_iMaxNumVertices    -> %d\n", m_iMaxNumVertices0);
		printf("m_iMaxNumIndices     -> %d\n", m_iMaxNumIndices0);
		printf("m_iMinNumVertices    -> %d\n", m_iMinNumVertices0);
		printf("m_iMinNumIndices     -> %d\n", m_iMinNumIndices0);
		printf("m_iLODCtrlValueCount -> %d\n", m_iLODCtrlValueCount0);
	}

	/*
	*/
	fclose(fpMesh);
}

//-----------------------------------------------------------------------------
enum e_PlugType{
	PLUGTYPE_NORMAL=0,
	PLUGTYPE_CLOAK,
	PLUGTYPE_MAX=10,
	PLUGTYPE_UNDEFINED=0xffffffff
};

//-----------------------------------------------------------------------------
bool CN3BaseFileAccess_Load(FILE* hFile) {
	std::string m_szName = "";

	DWORD dwRWC = 0;
	int nL = 0;
	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0)  {
		std::vector<char> buffer(nL+1, NULL);
		ReadFile(hFile, &buffer[0], nL, &dwRWC, NULL);
		m_szName = &buffer[0];
	}

	return true;
}

//-----------------------------------------------------------------------------
bool CN3CPlugBase_Load(FILE* hFile) {
	CN3BaseFileAccess_Load(hFile);

	DWORD dwRWC = 0;
	int nL = 0;
	char szFN[512] = "";

	e_PlugType m_ePlugType;
	ReadFile(hFile, &m_ePlugType, 4, &dwRWC, NULL);
	if(m_ePlugType > PLUGTYPE_MAX) {
		m_ePlugType = PLUGTYPE_NORMAL;
	}
	int m_nJointIndex;
	ReadFile(hFile, &m_nJointIndex, 4, &dwRWC, NULL);

	glm::vec3 m_vPosition;
	ReadFile(hFile, &m_vPosition, sizeof(m_vPosition), &dwRWC, NULL);
	_N3Matrix44 m_MtxRot;
	ReadFile(hFile, &m_MtxRot, sizeof(m_MtxRot), &dwRWC, NULL);
	glm::vec3 m_vScale;
	ReadFile(hFile, &m_vScale, sizeof(m_vScale), &dwRWC, NULL);

	_N3Material m_Mtl;
	ReadFile(hFile, &m_Mtl, sizeof(_N3Material), &dwRWC, NULL);

	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) {
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
		N3LoadMesh(szFN); //this->PMeshSet(szFN);
	}

	ReadFile(hFile, &nL, 4, &dwRWC, NULL);
	if(nL > 0) {
		ReadFile(hFile, szFN, nL, &dwRWC, NULL); szFN[nL] = NULL;
		N3LoadTexture(szFN); //this->TexSet(szFN);
	}	

	//this->ReCalcMatrix();

	return 0;
}

//-----------------------------------------------------------------------------
bool CN3CPlug_Load(FILE* hFile) {
	CN3CPlugBase_Load(hFile);
	/*
	DWORD dwRWC = 0;

	ReadFile(hFile, &m_nTraceStep, 4, &dwRWC, NULL);
	if(m_nTraceStep > 0) {
		ReadFile(hFile, &m_crTrace, 4, &dwRWC, NULL);
		ReadFile(hFile, &m_fTrace0, 4, &dwRWC, NULL);
		ReadFile(hFile, &m_fTrace1, 4, &dwRWC, NULL);
	} else m_nTraceStep = 0;

	int iUseVMesh = 0;
	ReadFile(hFile, &iUseVMesh, 4, &dwRWC, NULL);
	if(iUseVMesh) {
		CN3PMesh* pPMesh = new CN3PMesh();

		pPMesh->Load(hFile);
		static int iSN = 0;
		char szFNTmp[256]; sprintf(szFNTmp, "Temp_Plug_%d.N3PMesh", iSN++);
		pPMesh->FileNameSet(szFNTmp);
		s_MngPMesh.Add(pPMesh);
		m_PMeshInstFX.Create(pPMesh);
	}

	m_strFXMainName = "";
	m_strFXTailName = "";
	InitFX(m_strFXMainName, m_strFXTailName, 0xffffffff);
	*/

	return true;
}

//-----------------------------------------------------------------------------
struct _ITEM_TABLE {
	SQLUINTEGER m_iNum;
	SQLCHAR m_sName[(25+1)];
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

CSTLMap<_ITEM_TABLE> ItemTableMap;

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

CN3TableBase<__TABLE_ITEM_BASIC>* s_pTbl_Items_Basic;

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

enum e_ItemType {
	ITEM_TYPE_PLUG = 1,
	ITEM_TYPE_PART,
	ITEM_TYPE_ICONONLY,
	ITEM_TYPE_GOLD = 9,
	ITEM_TYPE_SONGPYUN = 10,
	ITEM_TYPE_UNKNOWN = 0xffffffff
};

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

enum e_PlugPosition {
	PLUG_POS_RIGHTHAND = 0,
	PLUG_POS_LEFTHAND,
	PLUG_POS_BACK,
	PLUG_POS_KNIGHTS_GRADE,
	PLUG_POS_COUNT,
	PLUG_POS_UNKNOWN = 0xffffffff
};

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

e_ItemType MakeResrcFileNameForUPC(
	__TABLE_ITEM_BASIC* pItem, std::string* pszResrcFN,
	std::string* pszIconFN, e_PartPosition& ePartPosition,
	e_PlugPosition& ePlugPosition, e_Race eRace
) {	
	ePartPosition = PART_POS_UNKNOWN;
	ePlugPosition = PLUG_POS_UNKNOWN;

	if(pszResrcFN) *pszResrcFN = "";
	if(pszIconFN) *pszIconFN = "";

	if(NULL == pItem) return ITEM_TYPE_UNKNOWN;

	e_ItemType eType = ITEM_TYPE_UNKNOWN;
	e_ItemPosition ePos = (e_ItemPosition)pItem->byAttachPoint;

	int iPos = 0;
	std::string szExt;

	if(ePos>=ITEM_POS_DUAL && ePos<=ITEM_POS_TWOHANDLEFT) {
		if(ITEM_POS_DUAL==ePos||ITEM_POS_RIGHTHAND==ePos||ITEM_POS_TWOHANDRIGHT==ePos)
			ePlugPosition = PLUG_POS_RIGHTHAND;
		else if(ITEM_POS_LEFTHAND==ePos||ITEM_POS_TWOHANDLEFT==ePos)
			ePlugPosition = PLUG_POS_LEFTHAND;
		eType = ITEM_TYPE_PLUG;
		szExt = ".n3cplug";
	} else if(ePos>=ITEM_POS_UPPER && ePos<=ITEM_POS_SHOES) {
		if(ITEM_POS_UPPER == ePos)
			ePartPosition = PART_POS_UPPER;
		else if(ITEM_POS_LOWER == ePos)
			ePartPosition = PART_POS_LOWER;
		else if(ITEM_POS_HEAD == ePos)
			ePartPosition = PART_POS_HAIR_HELMET;
		else if(ITEM_POS_GLOVES == ePos)
			ePartPosition = PART_POS_HANDS;
		else if(ITEM_POS_SHOES == ePos)
			ePartPosition = PART_POS_FEET;
		eType = ITEM_TYPE_PART;
		szExt = ".n3cpart";
		iPos = ePartPosition + 1;
	} else if(ePos>=ITEM_POS_EAR && ePos<=ITEM_POS_INVENTORY) {
		eType = ITEM_TYPE_ICONONLY;
		szExt = ".dxt";
	} else if(ePos == ITEM_POS_GOLD) {
		eType = ITEM_TYPE_GOLD;
		szExt = ".dxt";
	} else if(ePos == ITEM_POS_SONGPYUN) {
		eType = ITEM_TYPE_SONGPYUN;
		szExt = ".dxt";
	} else printf("Invalid Item Position\n");

	std::vector<char> buffer(256, NULL);
	if(pszResrcFN) {
		if(pItem->dwIDResrc) {
			if(eRace!=RACE_UNKNOWN && ePos>=/*ITEM_POS_DUAL*/ITEM_POS_UPPER && ePos<=ITEM_POS_SHOES) {
				// NOTE: no idea but perhaps this will work for now
				sprintf(&buffer[0], "Item\\%.1d_%.4d_%.2d_%.1d%s",
					(pItem->dwIDResrc / 10000000),
					((pItem->dwIDResrc / 1000) % 10000) + eRace,
					(pItem->dwIDResrc / 10) % 100,
					pItem->dwIDResrc % 10,
					szExt.c_str()
				);
			} else {
				sprintf(&buffer[0], "Item\\%.1d_%.4d_%.2d_%.1d%s",
					(pItem->dwIDResrc / 10000000),
					(pItem->dwIDResrc / 1000) % 10000,
					(pItem->dwIDResrc / 10) % 100,
					pItem->dwIDResrc % 10,
					szExt.c_str()
				);
			}

			*pszResrcFN = &buffer[0];
		} else {
			*pszResrcFN = "";
		}
	}

	if(pszIconFN) {
		sprintf(&buffer[0],	"UI\\ItemIcon_%.1d_%.4d_%.2d_%.1d.dxt",
			(pItem->dwIDIcon / 10000000), 
			(pItem->dwIDIcon / 1000) % 10000, 
			(pItem->dwIDIcon / 10) % 100, 
			pItem->dwIDIcon % 10
		);

		*pszIconFN = &buffer[0];
	}

	return eType;
}

//-----------------------------------------------------------------------------
class shape_window: public Fl_Gl_Window {
private:
	void draw(void);
	void draw_overlay(void);

public:
	int sides;
	int overlay_sides;

public:
	shape_window(int x, int y, int w, int h, const char* l = NULL);
};

shape_window::shape_window(int x, int y, int w, int h, const char* l):
Fl_Gl_Window(x, y, w, h, l) {
	sides = overlay_sides = 6;
}

void shape_window::draw(void) {
	if(!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0, 0, pixel_w(), pixel_h());
	}

	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
	for(int i=0; i<sides; ++i) {
		double ang = i*2*M_PI/sides;
		glColor3f(float(i)/sides, float(i)/sides, float(i)/sides);
		glVertex3f(cos(ang), sin(ang), 0.0f);
	}
	glEnd();
}

void shape_window::draw_overlay(void) {
	if(!valid()) {
		valid(1);
		glLoadIdentity();
		glViewport(0, 0, pixel_w(), pixel_h());
	}

	gl_color(FL_RED);
	glBegin(GL_LINE_LOOP);
	for(int i=0; i<overlay_sides; ++i) {
		double ang = i*2*M_PI/overlay_sides;
		glVertex3f(cos(ang), sin(ang), 0.0f);
	}
	glEnd();
}

//-----------------------------------------------------------------------------
void sides_cb(Fl_Widget* widget, void* data) {
	shape_window* sw = (shape_window*)data;
	sw->sides = int(((Fl_Slider*)widget)->value());
	sw->redraw();
}

//-----------------------------------------------------------------------------
void overlay_sides_cb(Fl_Widget* widget, void* data) {
	shape_window* sw = (shape_window*)data;
	sw->overlay_sides = int(((Fl_Slider*)widget)->value());
	sw->redraw_overlay();
}

//-----------------------------------------------------------------------------
class ItemTableView: public Fl_Table_Row {
private:
	Fl_Color cell_bgcolor;
	Fl_Color cell_fgcolor;

protected:
	void event_callback_update_opengl(void);
	static void event_callback(Fl_Widget* widget, void* data);
	void draw_cell(TableContext context,
		int r = 0, int c = 0, int x = 0, int y = 0, int w = 0, int h = 0
	);

public:
	Fl_Color GetCellBGColor(void) const {return cell_bgcolor;}
	Fl_Color GetCellFGColor(void) const {return cell_fgcolor;}

	void SetCellBGColor(Fl_Color color) {cell_bgcolor = color;}
	void SetCellFGColor(Fl_Color color) {cell_fgcolor = color;}

public:
	ItemTableView(int x, int y, int w, int h, const char* l = NULL):
	Fl_Table_Row(x, y, w, h, l) {
		cell_bgcolor = FL_WHITE;
		cell_fgcolor = FL_BLACK;
		callback(&event_callback, (void*)this);
		end();
	}
};

void ItemTableView::event_callback(Fl_Widget* widget, void* data) {
	ItemTableView* item_table = (ItemTableView*)data;
	item_table->event_callback_update_opengl();
}

void ItemTableView::event_callback_update_opengl(void) {
	int r = callback_row();
	int c = callback_col();
	TableContext context = callback_context();
	if(r==-1 || c==-1) return;
	if((int)Fl::event() != 1) return;

	_ITEM_TABLE* item = ItemTableMap.GetData(r);
	__TABLE_ITEM_BASIC* pItem = s_pTbl_Items_Basic->Find(item->m_iNum/1000*1000);

	if(pItem) {
		printf("--------------------\n");
		printf("%s:\n", pItem->szName.c_str());
		printf("\tdwID:\t\t%d\n", pItem->dwID);
		printf("\tbyExtIndex:\t%d\n", pItem->byExtIndex);
		printf("\tszRemark:\t%s\n", pItem->szRemark.c_str());
	} else {
		printf("Item is missing from TBL!\n");
	}

	std::string szResrcFN;
	e_PartPosition ePartPosTmp = PART_POS_UNKNOWN;
	e_PlugPosition ePlugPosTmp = PLUG_POS_UNKNOWN;

	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_ALL);
	printf("%s\n", szResrcFN.c_str());
	printf("--------------------\n");
	/*
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_KA_ARKTUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_KA_TUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_KA_WRINKLETUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_KA_PURITUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_EL_BABARIAN);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_EL_MAN);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_EL_WOMEN);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, NULL, ePartPosTmp, ePlugPosTmp, RACE_NPC);
	printf("%s\n", szResrcFN.c_str());
	*/
}

void ItemTableView::draw_cell(TableContext context,
	int r, int c, int x, int y, int w, int h
) {
	static char s[(25+1)];
	memset(s, 0x00, (25+1)*sizeof(char));

	_ITEM_TABLE* item = NULL;

	switch(context) {
		case CONTEXT_STARTPAGE:
			fl_font(FL_HELVETICA, 16);
			return;
		case CONTEXT_COL_HEADER:
			switch(c) {
				case 0: strcpy(s, "Num");     break;
				case 1: strcpy(s, "strName"); break;
			}

			fl_push_clip(x, y, w, h);
			{
				fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, col_header_color());
				fl_color(FL_BLACK);
				fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);
			}
			fl_pop_clip();
			return;
		case CONTEXT_ROW_HEADER:
			sprintf(s, "%d", r+1);

			fl_push_clip(x, y, w, h);
			{
				fl_draw_box(FL_THIN_UP_BOX, x, y, w, h, row_header_color());
				fl_color(FL_BLACK);
				fl_draw(s, x, y, w, h, FL_ALIGN_CENTER);
			}
			fl_pop_clip();
			return;
		case CONTEXT_CELL:
			item = ItemTableMap.GetData(r);
			switch(c) {
				case 0: sprintf(s, "  %d", item->m_iNum);  break;
				case 1: sprintf(s, "  %s", item->m_sName); break;
			}

			fl_push_clip(x, y, w, h);
			{
				fl_color(row_selected(r) ? selection_color() : cell_bgcolor);
				fl_rectf(x, y, w, h);
				fl_color(cell_fgcolor);
				fl_draw(s, x, y, w, h, FL_ALIGN_LEFT);
				fl_color(color());
				fl_rect(x, y, w, h);
			}
			fl_pop_clip();
			return;
		case CONTEXT_TABLE:
			printf("Table Context Called?\n");
			return;
		case CONTEXT_ENDPAGE:
		case CONTEXT_RC_RESIZE:
		case CONTEXT_NONE:
			return;
	}
}

//-----------------------------------------------------------------------------
int main(int argc, char** argv) {

	//----

	s_pTbl_Items_Basic = new CN3TableBase<__TABLE_ITEM_BASIC>;

	std::string szFN = "Item_Org.tbl";
	if(s_pTbl_Items_Basic->LoadFromFile(szFN.c_str()) == false) {
		printf("Failed to load Item_Org.tbl\n");
		system("pause");
		return -1;
	}

	//----
	SQLHANDLE hEnv, hConn;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hConn);
	if(SQLConnect(hConn, _T("KN_online"), SQL_NTS, _T("knight"), SQL_NTS, _T("knight"), SQL_NTS) == SQL_ERROR) {
		printf("SQLConnect\n");
		system("pause");
		return -1;
	}

	//----
	SQLHANDLE hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hConn, &hStmt);
	if(SQLExecDirect(hStmt, _T("SELECT TOP(5000) Num, strName FROM ITEM;"), SQL_NTS) == SQL_ERROR) {
		printf("SQLExecDirect\n");
		system("pause");
		return -1;
	}

	long count = 0;
	SQLINTEGER cbData;
	while(SQLFetch(hStmt) == SQL_SUCCESS) {

		_ITEM_TABLE* item = new _ITEM_TABLE();

		SQLGetData(hStmt, 1, SQL_C_ULONG, &(item->m_iNum), sizeof(SQLUINTEGER), &cbData);
		SQLGetData(hStmt, 2, SQL_C_CHAR, item->m_sName, 25, &cbData);

		ItemTableMap.PutData(count++, item);

		__TABLE_ITEM_BASIC* pItem = s_pTbl_Items_Basic->Find(item->m_iNum/1000*1000);
		if(!pItem) printf("Item \"%s\" is missing from the TBL!\n", item->m_sName);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	//----

	SQLDisconnect(hConn);
	SQLFreeHandle(SQL_HANDLE_DBC, hConn);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	//----



	Fl::use_high_res_GL(true);
	Fl_Window window(1024, 720);

	shape_window sw(window.w()-(_gl_width+10), 10, _gl_width, _gl_height);

	ItemTableView demo_table(10, 10, window.w()-(_gl_width+30), _gl_height);
	demo_table.selection_color(FL_YELLOW);
	demo_table.when(FL_WHEN_RELEASE|FL_WHEN_CHANGED);
	demo_table.table_box(FL_NO_BOX);
	demo_table.col_resize_min(4);
	demo_table.row_resize_min(4);

	demo_table.row_header(true);
	demo_table.row_header_width(60);
	demo_table.row_resize(true);
	demo_table.rows(ItemTableMap.GetSize());
	demo_table.row_height_all(20);

	demo_table.col_header(true);
	demo_table.col_header_height(25);
	demo_table.col_resize(true);
	demo_table.cols(2);
	demo_table.col_width_all(150);

	window.end();
	window.show(argc, argv);

	sw.show();
	sw.redraw_overlay();

	//system("pause");
	return Fl::run();
}
