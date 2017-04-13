#ifndef __ITEMINFO_H_
#define __ITEMINFO_H_

#include "defines.h"

#include "STLMap.h"
#include "N3TableBase.h"

using namespace std;

//-----------------------------------------------------------------------------
class ItemInfo {
public: //private:
	static vector<class ItemInfo> _items;
	static vector<string> _mesh_files_in_dir;
	static CSTLMap<_ITEM_TABLE> _db_item_info;
	static CN3TableBase<__TABLE_ITEM_BASIC>* _tbl_item_info;

public:
	static bool LoadInformation(void);
	static void CreateItemsFromInfo(void);

private:
	int m_tbl_ind;
	e_ItemType m_type;
	map<e_Race, int> mesh_file_ind_for_race;

public:
	e_ItemType getItemType(void);
	string getItemMeshFileForRace(e_Race race);

public:
	ItemInfo(void);
	~ItemInfo(void);
};

#endif
