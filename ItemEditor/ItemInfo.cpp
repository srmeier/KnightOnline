/*
*/

#include "base.h"
#include "ItemInfo.h"

using namespace std;

//-----------------------------------------------------------------------------
vector<class ItemInfo> ItemInfo::_items;
vector<string> ItemInfo::_mesh_files_in_dir;
CSTLMap<_ITEM_TABLE> ItemInfo::_db_item_info;
CN3TableBase<__TABLE_ITEM_BASIC>* ItemInfo::_tbl_item_info; // TODO: clean up

//-----------------------------------------------------------------------------
ItemInfo::ItemInfo(void) {
	m_tbl_ind = -1;
	mesh_file_ind_for_race.clear();
}

//-----------------------------------------------------------------------------
ItemInfo::~ItemInfo(void) {

}

//-----------------------------------------------------------------------------
bool ItemInfo::LoadInformation(void) {
	// NOTE: load the Item_Org TBL
	_tbl_item_info = new CN3TableBase<__TABLE_ITEM_BASIC>;

	std::string szFN = "Item_Org.tbl";
	if(_tbl_item_info->LoadFromFile(szFN.c_str()) == false) {
		printf("Failed to load Item_Org.tbl\n");
		system("pause");
		return false;
	}

	// NOTE: grab what we need from the DB
	SQLHANDLE hEnv, hConn;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hConn);
	if(SQLConnect(hConn, _T("KN_online"), SQL_NTS, _T("knight"), SQL_NTS, _T("knight"), SQL_NTS) == SQL_ERROR) {
		printf("SQLConnect\n");
		system("pause");
		return false;
	}

	SQLHANDLE hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hConn, &hStmt);
	if(SQLExecDirect(hStmt, _T("SELECT TOP(5000) Num, strName FROM ITEM;"), SQL_NTS) == SQL_ERROR) {//TOP(10000)
		printf("SQLExecDirect\n");
		system("pause");
		return false;
	}

	long count = 0;
	SQLINTEGER cbData;
	while(SQLFetch(hStmt) == SQL_SUCCESS) {

		_ITEM_TABLE* item = new _ITEM_TABLE();

		SQLGetData(hStmt, 1, SQL_C_ULONG, &(item->m_iNum), sizeof(SQLUINTEGER), &cbData);
		SQLGetData(hStmt, 2, SQL_C_CHAR, item->m_sName, NAME_LENGTH, &cbData);

		_db_item_info.PutData(count++, item);

		__TABLE_ITEM_BASIC* pItem = _tbl_item_info->Find(item->m_iNum/1000*1000);
		if(!pItem) printf("Item \"%s\" is missing from the TBL!\n", item->m_sName);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	
	SQLDisconnect(hConn);
	SQLFreeHandle(SQL_HANDLE_DBC, hConn);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	// NOTE: grab the mesh file information
	dirent** dir_list;
	int num_files = fl_filename_list("./item", &dir_list);

	for(int i=0; i<num_files; ++i) {
		int len_fn = strlen(dir_list[i]->d_name);
		if(strlen(dir_list[i]->d_name)>7 && (
			!strcmp(&dir_list[i]->d_name[len_fn-7], "n3cplug") || 
			!strcmp(&dir_list[i]->d_name[len_fn-7], "n3cpart")
		)) {
			_mesh_files_in_dir.push_back(dir_list[i]->d_name);
		}
	}

	fl_filename_free_list(&dir_list, num_files);

	return true;
}

//-----------------------------------------------------------------------------
/*
- precedence goes to the TBLs here because that is what the client is most
	conserned with
- note that if another race gets added we have to manually add it here
*/
void ItemInfo::CreateItemsFromInfo(void) {

	for(int i=0; i<_tbl_item_info->GetSize(); ++i) {
		__TABLE_ITEM_BASIC* item = _tbl_item_info->GetIndexedData(i);

		ItemInfo item_info;
		item_info.m_tbl_ind = i;

		pair<map<e_Race, int>::iterator, bool> ret;
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_ALL, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_KA_ARKTUAREK, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_KA_TUAREK, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_KA_WRINKLETUAREK, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_KA_PURITUAREK, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_EL_BABARIAN, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_EL_MAN, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_EL_WOMEN, -1));
		ret = item_info.mesh_file_ind_for_race.insert(make_pair(RACE_NPC, -1));

		_items.push_back(item_info);
	}
}

//-----------------------------------------------------------------------------
void ItemInfo::setMeshFileForRace(e_Race race, bool check_type) {
	e_ItemType     type;
	std::string    szResrcFN;
	std::string    szIconFN;
	e_PartPosition ePartPosition;
	e_PlugPosition ePlugPosition;

	type = MakeResrcFileNameForUPC(_tbl_item_info->GetIndexedData(m_tbl_ind),
		&szResrcFN, &szIconFN, ePartPosition, ePlugPosition, race
	);

	if(check_type && type!=m_type) {
		printf("ERROR: item has changed type.\n");
		exit(-1);
	} else {
		m_type = type;
	}

	string filename = "";
	if(type==ITEM_TYPE_PLUG || type==ITEM_TYPE_PART) {
		filename = szIconFN;
	} else {
		filename = szResrcFN;
	}

	int filename_ind = -1;
	for(int i=0; i<_mesh_files_in_dir.size(); ++i) {
		if(_mesh_files_in_dir[i] == filename) {
			filename_ind = i;
		}
	}

	mesh_file_ind_for_race[race] = filename_ind;
}

//-----------------------------------------------------------------------------
ItemInfo* ItemInfo::getItem(int i) {
	if(i>_items.size() || i<0) return NULL;
	return &_items[i];
}

//-----------------------------------------------------------------------------
e_ItemType ItemInfo::getItemType(void) {
	return m_type;
}

//-----------------------------------------------------------------------------
string ItemInfo::getItemMeshFileForRace(e_Race race) {
	if(mesh_file_ind_for_race[race] == -1) {
		setMeshFileForRace(race, false);
	}

	if(mesh_file_ind_for_race[race] == -1) return "";
	return _mesh_files_in_dir[mesh_file_ind_for_race[race]];
}
