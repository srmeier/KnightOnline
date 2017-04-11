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
	map<e_Race, vector<int>> mesh_file_inds_per_race;

public:
	string getItemMeshForRace(e_Race race);
};

#endif
