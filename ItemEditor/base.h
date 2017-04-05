#ifndef __BASE_H_
#define __BASE_H_

#include "defines.h"

#include "assimp/scene.h"
#include "assimp/exporter.hpp"
#include "assimp/importer.hpp"
#include "assimp/postprocess.h"

//-----------------------------------------------------------------------------
std::wstring s2ws(const std::string& s);
void N3LoadTexture(const char* szFN);
void N3LoadMesh(const char* szFN);
bool CN3BaseFileAccess_Load(FILE* hFile);
bool CN3CPlugBase_Load(FILE* hFile);
bool CN3CPlug_Load(FILE* hFile);
bool CN3IMesh_Create(int nFC, int nVC, int nUVC);
bool CN3Skin_Create(int nFC, int nVC, int nUVC);
bool CN3IMesh_Load(FILE* hFile);
bool CN3Skin_Load(FILE* hFile);
bool CN3CPart_Load(FILE* hFile);
e_ItemType MakeResrcFileNameForUPC(
	__TABLE_ITEM_BASIC* pItem, std::string* pszResrcFN,
	std::string* pszIconFN, e_PartPosition& ePartPosition,
	e_PlugPosition& ePlugPosition, e_Race eRace
);
void GenerateScene(void);

//-----------------------------------------------------------------------------
class N3MeshConverter {
public:
	static bool Convert(char* filename);
};

#endif
