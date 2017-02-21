/*
*/

#include "defines.h"

#include "fl/fl_hor_slider.h"
#include "fl/fl_toggle_button.h"
#include "fl/fl_draw.H"
#include "fl/fl_table_row.H"

#include "fl/fl_tabs.h"
#include "fl/fl_group.h"
#include "fl/fl_input.h"
#include "fl/fl_menu_bar.h"

#include "ItemInfo.h"
#include "GLItemViewer.h"

const int _gl_width = 380-60;
const int _gl_height = 1024/4;

//-----------------------------------------------------------------------------
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

//-----------------------------------------------------------------------------
e_ItemType eType;
class GLItemViewer* m_sw;

//-----------------------------------------------------------------------------
_N3TexHeader    HeaderOrg;
unsigned char*  compTexData;
int             compTexSize;
int             iPixelSize;
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
		//system("pause");
		return;//exit(-1);
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
	iPixelSize = 0;
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
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A4R4G4B4: {
			iPixelSize = 2;
		} break;
		case D3DFMT_R8G8B8: {
			iPixelSize = 3;
		} break;
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8: {
			iPixelSize = 4;
		} break;
		default: {
			fprintf(stderr,
				"\nERROR: Unknown texture format %d. (need to implement this)\n", HeaderOrg.Format
			);
			//system("pause");
			return; //exit(-1);
		} break;
	}

	if(iPixelSize == 0) {
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
			//system("pause");
			return; //exit(-1);
		}
	} else {
		if(compTexData) {
			delete compTexData;
			compTexData = NULL;
		}

		compTexSize = (HeaderOrg.nWidth*HeaderOrg.nHeight*iPixelSize);
		compTexData = new unsigned char[compTexSize];
		fread(compTexData, sizeof(unsigned char), compTexSize, fpTexture);
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
		//system("pause");
		return; //exit(-1);
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
bool CN3BaseFileAccess_Load(FILE* hFile) {
	std::string m_szName = "";

	int nL = 0;
	fread(&nL, sizeof(int), 1, hFile);
	if(nL > 0)  {
		std::vector<char> buffer(nL+1, NULL);
		fread(&buffer[0], sizeof(char), nL, hFile);
		m_szName = &buffer[0];
	}

	return true;
}

//-----------------------------------------------------------------------------
bool CN3CPlugBase_Load(FILE* hFile) {
	CN3BaseFileAccess_Load(hFile);

	int nL = 0;
	char szFN[512] = "";

	e_PlugType m_ePlugType;
	fread(&m_ePlugType, sizeof(e_PlugType), 1, hFile);
	if(m_ePlugType > PLUGTYPE_MAX) {
		m_ePlugType = PLUGTYPE_NORMAL;
	}
	int m_nJointIndex;
	fread(&m_nJointIndex, sizeof(int), 1, hFile);

	glm::vec3 m_vPosition;
	fread(&m_vPosition, sizeof(glm::vec3), 1, hFile);
	_N3Matrix44 m_MtxRot;
	fread(&m_MtxRot, sizeof(_N3Matrix44), 1, hFile);
	glm::vec3 m_vScale;
	fread(&m_vScale, sizeof(glm::vec3), 1, hFile);

	_N3Material m_Mtl;
	fread(&m_Mtl, sizeof(_N3Material), 1, hFile);

	fread(&nL, sizeof(int), 1, hFile);
	if(nL > 0) {
		fread(szFN, sizeof(char), nL, hFile);
		N3LoadMesh(szFN); //this->PMeshSet(szFN);
	}

	fread(&nL, sizeof(int), 1, hFile);
	if(nL > 0) {
		fread(szFN, sizeof(char), nL, hFile); szFN[nL] = '\0';
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
int m_nVC;
int m_nFC;
int m_nUVC;

__VertexSkinned* m_pSkinVertices;
__VertexXyzNormal* m_pVertices;
WORD* m_pwVtxIndices;
float* m_pfUVs;
WORD* m_pwUVsIndices;

bool CN3IMesh_Create(int nFC, int nVC, int nUVC) {
	if(nFC<=0 || nVC<=0) return false;

	m_nFC = nFC;
	m_nVC = nVC;

	if(m_pVertices) delete m_pVertices;
	if(m_pwVtxIndices) delete m_pwVtxIndices;
	if(m_pfUVs) delete m_pfUVs;
	if(m_pwUVsIndices) delete m_pwUVsIndices;

	m_pVertices = new __VertexXyzNormal[nVC]; memset(m_pVertices, 0, sizeof(__VertexXyzNormal) * nVC);
	m_pwVtxIndices = new WORD[nFC*3]; memset(m_pwVtxIndices, 0, 2 * nFC * 3); // unsigned short

	if(nUVC > 0) {
		m_nUVC = nUVC;
		m_pfUVs = new float[nUVC*2]; memset(m_pfUVs, 0, 8 * nUVC);
		m_pwUVsIndices = new WORD[nFC*3]; memset(m_pwUVsIndices, 0, 2 * nFC * 3); // unsigned short
	}

	return true;
}

bool CN3Skin_Create(int nFC, int nVC, int nUVC) {
	if(false == CN3IMesh_Create(nFC, nVC, nUVC)) return false;

	delete [] m_pSkinVertices;
	m_pSkinVertices = new __VertexSkinned[nVC];
	memset(m_pSkinVertices, 0, sizeof(__VertexSkinned)*nVC);

	return true;
}

bool CN3IMesh_Load(FILE* hFile) {
	CN3BaseFileAccess_Load(hFile);

	int nFC = 0, nVC = 0, nUVC = 0;

	fread(&nFC, sizeof(int), 1, hFile);
	fread(&nVC, sizeof(int), 1, hFile);
	fread(&nUVC, sizeof(int), 1, hFile);

	if(nFC > 0 && nVC > 0) {
		CN3Skin_Create(nFC, nVC, nUVC);

		fread(m_pVertices, sizeof(__VertexXyzNormal), nVC, hFile);
		fread(m_pwVtxIndices, 2*3, nFC, hFile);
	}

	if(m_nUVC > 0) {
		fread(m_pfUVs, 8, nUVC, hFile);
		fread(m_pwUVsIndices, 2*3, nFC, hFile);
	}

	//this->FindMinMax();

	return true;
}

//-----------------------------------------------------------------------------
bool CN3Skin_Load(FILE* hFile) {
	/*
	for(int i = 0; i < m_nVC; i++) {
		if(m_pSkinVertices[i].pnJoints) delete m_pSkinVertices[i].pnJoints;
		if(m_pSkinVertices[i].pfWeights) delete m_pSkinVertices[i].pfWeights;
	}
	*/

	CN3IMesh_Load(hFile);

	// NOTE: will need this if planning to go to other LODs because I have to get past this info...
	/*
	for(int i = 0; i < m_nVC; i++) {
		fread(&m_pSkinVertices[i], sizeof(__VertexSkinned), 1, hFile);
		m_pSkinVertices[i].pnJoints = NULL;
		m_pSkinVertices[i].pfWeights = NULL;

		int nAffect = m_pSkinVertices[i].nAffect;
		if(nAffect > 1) {
			m_pSkinVertices[i].pnJoints = new int[nAffect];
			m_pSkinVertices[i].pfWeights = new float[nAffect];

			ReadFile(hFile, m_pSkinVertices[i].pnJoints, 4 * nAffect, &dwRWC, NULL);
			ReadFile(hFile, m_pSkinVertices[i].pfWeights, 4 * nAffect, &dwRWC, NULL);
		} else if(nAffect == 1) {
			m_pSkinVertices[i].pnJoints = new int[1];
			ReadFile(hFile, m_pSkinVertices[i].pnJoints, 4, &dwRWC, NULL);
		}
	}
	*/

	return true;
}

//-----------------------------------------------------------------------------
bool CN3CPart_Load(FILE* hFile) {
	CN3BaseFileAccess_Load(hFile);

	int nL = 0;
	char szFN[256] = "";

	int m_dwReserved;
	fread(&m_dwReserved, sizeof(int), 1, hFile);
	_N3Material m_MtlOrg;
	fread(&m_MtlOrg, sizeof(_N3Material), 1, hFile);

	fread(&nL, sizeof(int), 1, hFile);
	if(nL > 0) {
		fread(szFN, sizeof(char), nL, hFile);
		N3LoadTexture(szFN); //CN3CPart_TexSet(szFN);
	}

	fread(&nL, sizeof(int), 1, hFile);
	if(nL > 0) {
		fread(szFN, sizeof(char), nL, hFile); szFN[nL] = '\0';
		//s_MngSkins.Delete(&m_pSkinsRef);
		//m_pSkinsRef = s_MngSkins.Get(szFN);

		FILE* fpSkin = fopen(szFN, "rb");
		if(fpSkin == NULL) {
			fprintf(stderr, "\nERROR: Missing skin %s\n", szFN);
			//system("pause");
			return false; //exit(-1);
		}

		CN3BaseFileAccess_Load(fpSkin);
		//for(int i=0; i<4; ++i) {
			// NOTE: just load the first LOD
			CN3Skin_Load(fpSkin);
		//}
	}

	return true;
}

//-----------------------------------------------------------------------------
CSTLMap<_ITEM_TABLE> ItemTableMap;
CN3TableBase<__TABLE_ITEM_BASIC>* s_pTbl_Items_Basic;

//-----------------------------------------------------------------------------
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
	std::string pszIconFN;
	e_PartPosition ePartPosTmp = PART_POS_UNKNOWN;
	e_PlugPosition ePlugPosTmp = PLUG_POS_UNKNOWN;

	eType = MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_UNKNOWN);
	printf("%s\n", szResrcFN.c_str());
	printf("--------------------\n");

	if(eType == ITEM_TYPE_PLUG) {
		FILE* pFile = fopen(szResrcFN.c_str(), "rb");
		if(pFile == NULL) {
			fprintf(stderr, "ERROR: Missing N3Plug %s\n", szResrcFN.c_str());
			//system("pause");
			return; //exit(-1);
		}

		CN3CPlug_Load(pFile);
		fclose(pFile);
	} else if(eType == ITEM_TYPE_PART) {
		FILE* pFile = fopen(szResrcFN.c_str(), "rb");
		if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_ARKTUAREK);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_EL_WOMEN);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_EL_MAN);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_EL_BABARIAN);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_PURITUAREK);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_WRINKLETUAREK);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_TUAREK);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
			MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_NPC);
			printf("%s\n", szResrcFN.c_str());
			pFile = fopen(szResrcFN.c_str(), "rb");
			if(pFile == NULL) {
				fprintf(stderr, "ERROR: Missing N3Part %s\n", szResrcFN.c_str());
				//system("pause");
				return; //exit(-1);
			}
			}}}}}}}
		}

		CN3CPart_Load(pFile);
		fclose(pFile);
	} else if(eType==ITEM_TYPE_ICONONLY || eType==ITEM_TYPE_GOLD || eType==ITEM_TYPE_SONGPYUN) {
		
		N3LoadTexture(pszIconFN.c_str());
		
	} else {
		printf("Unknown item type!\n");
		return;
	}

	printf("--------------------\n");

	/*
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_ARKTUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_TUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_WRINKLETUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_KA_PURITUAREK);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_EL_BABARIAN);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_EL_MAN);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_EL_WOMEN);
	printf("%s\n", szResrcFN.c_str());
	MakeResrcFileNameForUPC(pItem, &szResrcFN, &pszIconFN, ePartPosTmp, ePlugPosTmp, RACE_NPC);
	printf("%s\n", szResrcFN.c_str());
	*/

	// ========================================================================

	m_sw->PushDataToGPU();
	m_sw->redraw();
}

void ItemTableView::draw_cell(TableContext context,
	int r, int c, int x, int y, int w, int h
) {
	static char s[(NAME_LENGTH+1)];
	memset(s, 0x00, (NAME_LENGTH+1)*sizeof(char));

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
Fl_Menu_Item menu_table[] = {
	{"&File",0,0,0,FL_SUBMENU},
		{"&Open",	FL_ALT+'o', 0, 0, FL_MENU_INACTIVE},
		{"&Close",	0,	0},
		{"&Quit",	FL_ALT+'q', 0, 0, FL_MENU_DIVIDER},
		{"shortcut",'a'},
		{"shortcut",FL_SHIFT+'a'},
		{"shortcut",FL_CTRL+'a'},
		{"shortcut",FL_CTRL+FL_SHIFT+'a'},
		{"shortcut",FL_ALT+'a'},
		{"shortcut",FL_ALT+FL_SHIFT+'a'},
		{"shortcut",FL_ALT+FL_CTRL+'a'},
		{"shortcut",FL_ALT+FL_SHIFT+FL_CTRL+'a', 0,0, FL_MENU_DIVIDER},
		{"shortcut",'\r'/*FL_Enter*/},
		{"shortcut",FL_CTRL+FL_Enter, 0,0, FL_MENU_DIVIDER},
		{"shortcut",FL_F+1},
		{"shortcut",FL_SHIFT+FL_F+1},
		{"shortcut",FL_CTRL+FL_F+1},
		{"shortcut",FL_SHIFT+FL_CTRL+FL_F+1},
		{"shortcut",FL_ALT+FL_F+1},
		{"shortcut",FL_ALT+FL_SHIFT+FL_F+1},
		{"shortcut",FL_ALT+FL_CTRL+FL_F+1},
		{"shortcut",FL_ALT+FL_SHIFT+FL_CTRL+FL_F+1, 0,0, FL_MENU_DIVIDER},
		{"&Submenus", FL_ALT+'S',	0, (void*)"Submenu1", FL_SUBMENU},
			{"A very long menu item"},
			{"&submenu",FL_CTRL+'S',	0, (void*)"submenu2", FL_SUBMENU},
				{"item 1"},
				{"item 2"},
				{"item 3"},
				{"item 4"},
			{0},
			{"after submenu"},
		{0},
	{0},
	{"&Edit",FL_F+2,0,0,FL_SUBMENU},
		{"Undo",	FL_ALT+'z',	0},
		{"Redo",	FL_ALT+'r',	0, 0, FL_MENU_DIVIDER},
		{"Cut",	FL_ALT+'x',	0},
		{"Copy",	FL_ALT+'c',	0},
		{"Paste",	FL_ALT+'v',	0},
		{"Inactive",FL_ALT+'d',	0, 0, FL_MENU_INACTIVE},
		{"Clear",	0,	0, 0, FL_MENU_DIVIDER},
		{"Invisible",FL_ALT+'e',	0, 0, FL_MENU_INVISIBLE},
		{"Preferences",0,	0},
		{"Size",	0,	0},
	{0},
	{"&Checkbox",FL_F+3,0,0,FL_SUBMENU},
		{"&Alpha",	FL_F+2,	0, (void *)1, FL_MENU_TOGGLE},
		{"&Beta",	0,	0, (void *)2, FL_MENU_TOGGLE},
		{"&Gamma",	0,	0, (void *)3, FL_MENU_TOGGLE},
		{"&Delta",	0,	0, (void *)4, FL_MENU_TOGGLE|FL_MENU_VALUE},
		{"&Epsilon",0,	0, (void *)5, FL_MENU_TOGGLE},
		{"&Pi",	0,	0, (void *)6, FL_MENU_TOGGLE},
		{"&Mu",	0,	0, (void *)7, FL_MENU_TOGGLE|FL_MENU_DIVIDER},
		{"Red",	0,	0, (void *)1, FL_MENU_TOGGLE, 0, 0, 0, 1},
		{"Black",	0,	0, (void *)1, FL_MENU_TOGGLE|FL_MENU_DIVIDER},
		{"00",	0,	0, (void *)1, FL_MENU_TOGGLE},
		{"000",	0,	0, (void *)1, FL_MENU_TOGGLE},
	{0},
	{"&Radio",0,0,0,FL_SUBMENU},
		{"&Alpha",	0,	0, (void *)1, FL_MENU_RADIO},
		{"&Beta",	0,	0, (void *)2, FL_MENU_RADIO},
		{"&Gamma",	0,	0, (void *)3, FL_MENU_RADIO},
		{"&Delta",	0,	0, (void *)4, FL_MENU_RADIO|FL_MENU_VALUE},
		{"&Epsilon",0,	0, (void *)5, FL_MENU_RADIO},
		{"&Pi",	0,	0, (void *)6, FL_MENU_RADIO},
		{"&Mu",	0,	0, (void *)7, FL_MENU_RADIO|FL_MENU_DIVIDER},
		{"Red",	0,	0, (void *)1, FL_MENU_RADIO},
		{"Black",	0,	0, (void *)1, FL_MENU_RADIO|FL_MENU_DIVIDER},
		{"00",	0,	0, (void *)1, FL_MENU_RADIO},
		{"000",	0,	0, (void *)1, FL_MENU_RADIO},
	{0},
	{"&Font",0,0,0,FL_SUBMENU /*, 0, FL_BOLD, 20*/},
		{"Normal",	0, 0, 0, 0, 0, 0, 14},
		{"Bold",	0, 0, 0, 0, 0, FL_BOLD, 14},
		{"Italic",	0, 0, 0, 0, 0, FL_ITALIC, 14},
		{"BoldItalic",0,0,0, 0, 0, FL_BOLD+FL_ITALIC, 14},
		{"Small",	0, 0, 0, 0, 0, FL_BOLD+FL_ITALIC, 10},
		{"Emboss",	0, 0, 0, 0, (uchar)FL_EMBOSSED_LABEL},
		{"Engrave",	0, 0, 0, 0, (uchar)FL_ENGRAVED_LABEL},
		{"Shadow",	0, 0, 0, 0, (uchar)FL_SHADOW_LABEL},
		{"@->",	0, 0, 0, 0, (uchar)FL_SYMBOL_LABEL},
	{0},
	{"&International",0,0,0,FL_SUBMENU},
		{"Sharp Ess",0x0000df},
		{"A Umlaut",0x0000c4},
		{"a Umlaut",0x0000e4},
		{"Euro currency",FL_COMMAND+0x0020ac},
		{"the &\xc3\xbc Umlaut"},  // &uuml;
		{"the capital &\xc3\x9c"}, // &Uuml;
		{"convert \xc2\xa5 to &\xc2\xa3"}, // Yen to GBP
		{"convert \xc2\xa5 to &\xe2\x82\xac"}, // Yen to Euro
		{"Hangul character Sios &\xe3\x85\x85"},
		{"Hangul character Cieuc", 0x003148},
	{0},
	{"E&mpty",0,0,0,FL_SUBMENU},
	{0},
	{"&Inactive", 0,	0, 0, FL_MENU_INACTIVE|FL_SUBMENU},
		{"A very long menu item"},
		{"A very long menu item"},
	{0},
	{"Invisible",0,	0, 0, FL_MENU_INVISIBLE|FL_SUBMENU},
		{"A very long menu item"},
		{"A very long menu item"},
	{0},
	{"button",FL_F+4, 0, 0, FL_MENU_TOGGLE},
	{0}
};

//-----------------------------------------------------------------------------
void test_cb(Fl_Widget* w, void*) {
	Fl_Menu_* mw = (Fl_Menu_*)w;
	const Fl_Menu_Item* m = mw->mvalue();
	if (!m)
		printf("NULL\n");
	else if (m->shortcut())
		printf("%s - %s\n", m->label(), fl_shortcut_label(m->shortcut()));
	else
		printf("%s\n", m->label());
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
	if(SQLExecDirect(hStmt, _T("SELECT Num, strName FROM ITEM;"), SQL_NTS) == SQL_ERROR) {//TOP(10000)
		printf("SQLExecDirect\n");
		system("pause");
		return -1;
	}

	long count = 0;
	SQLINTEGER cbData;
	while(SQLFetch(hStmt) == SQL_SUCCESS) {

		_ITEM_TABLE* item = new _ITEM_TABLE();

		SQLGetData(hStmt, 1, SQL_C_ULONG, &(item->m_iNum), sizeof(SQLUINTEGER), &cbData);
		SQLGetData(hStmt, 2, SQL_C_CHAR, item->m_sName, NAME_LENGTH, &cbData);

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
	Fl_Window window(1024, 720, "KO Item Editor");

	GLItemViewer sw(window.w()-_gl_width, 30, _gl_width, _gl_height);
	m_sw = &sw;

	ItemTableView demo_table(0, 30, window.w()-(_gl_width+0), _gl_height);
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

	Fl_Menu_Bar menubar(0, 0, window.w(), 30);
	menubar.menu(menu_table);
	menubar.callback(test_cb);

	Fl_Tabs info_tabs(0, _gl_height+40, window.w(), window.h()-_gl_height-30-10);
		Fl_Group group(0, _gl_height+40+35, window.w(), window.h()-_gl_height-30-10, "Table Info");
			Fl_Input input(10, _gl_height+30+35+20, 240, 30);
		group.end();
		Fl_Group group2(0, _gl_height+40+35, window.w(), window.h()-_gl_height-30-10, "Database Info");
			Fl_Input input2(10, _gl_height+30+35+20, 240, 30);
		group2.end();
	info_tabs.end();

	window.end();
	window.show(argc, argv);

	sw.show();
	sw.redraw_overlay();

	return Fl::run();
}
