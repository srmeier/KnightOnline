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
#include "fl/fl_float_input.h"
#include "fl/fl_int_input.h"
#include "fl/fl_menu_bar.h"

#include "fl/filename.h"

#include "base.h"
#include "ItemInfo.h"
#include "GLItemViewer.h"

const int _gl_width = 380-60;
const int _gl_height = 1024/4;

//-----------------------------------------------------------------------------
extern e_ItemType eType;

//-----------------------------------------------------------------------------
class GLItemViewer* m_sw;

int num_disp_files;
char** disp_files;

Fl_Int_Input* tbl_id;
Fl_Int_Input* tbl_ext_index;
Fl_Input*     tbl_name;
Fl_Input*     tbl_remark;

//-----------------------------------------------------------------------------
CSTLMap<_ITEM_TABLE> ItemTableMap;
CN3TableBase<__TABLE_ITEM_BASIC>* s_pTbl_Items_Basic;

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

	char* filename = disp_files[r];
	int len_fn = strlen(filename);
	char* exten = &filename[len_fn-7];

	if(!strcmp(exten, "n3cplug")) {

		eType = ITEM_TYPE_PLUG;

		char tmp[0xFF] = "";
		sprintf(tmp, "./item/%s", filename);
		FILE* pFile = fopen(tmp, "rb");
		if(pFile == NULL) {
			fprintf(stderr, "ERROR: Missing N3Plug %s\n", tmp);
			return;
		}

		CN3CPlug_Load(pFile);
		fclose(pFile);
	} else if(!strcmp(exten, "n3cpart")) {

		eType = ITEM_TYPE_PART;

		char tmp[0xFF] = "";
		sprintf(tmp, "./item/%s", filename);
		FILE* pFile = fopen(tmp, "rb");
		if(pFile == NULL) {
			fprintf(stderr, "ERROR: Missing N3Part %s\n", tmp);
			return;
		}

		CN3CPart_Load(pFile);
		fclose(pFile);
	}

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
				case 0: strcpy(s, "Filename"); break;
				//case 0: strcpy(s, "Num");     break;
				//case 1: strcpy(s, "strName"); break;
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
			switch(c) {
				case 0: sprintf(s, "%s", disp_files[r]); break;
			}
			/*
			item = ItemTableMap.GetData(r);
			switch(c) {
				case 0: sprintf(s, "  %d", item->m_iNum);  break;
				case 1: sprintf(s, "  %s", item->m_sName); break;
			}
			*/

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

	SQLHANDLE hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hConn, &hStmt);
	if(SQLExecDirect(hStmt, _T("SELECT TOP(5000) Num, strName FROM ITEM;"), SQL_NTS) == SQL_ERROR) {//TOP(10000)
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
	
	SQLDisconnect(hConn);
	SQLFreeHandle(SQL_HANDLE_DBC, hConn);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	//----

	dirent** dir_list;
	int num_files = fl_filename_list("./item", &dir_list);

	num_disp_files = 0;
	for(int i=0; i<num_files; ++i) {
		int len_fn = strlen(dir_list[i]->d_name);
		if(!strcmp(&dir_list[i]->d_name[len_fn-7], "n3cplug") || !strcmp(&dir_list[i]->d_name[len_fn-7], "n3cpart")) {
			disp_files = (char**)realloc(disp_files, ++num_disp_files*sizeof(char*));
			disp_files[num_disp_files-1] = (char*)calloc(len_fn+1, sizeof(char));
			memcpy(disp_files[num_disp_files-1], dir_list[i]->d_name, len_fn);

			//char* filename = disp_files[num_disp_files-1];
			//N3MeshConverter::Convert(filename);
		}
	}

	fl_filename_free_list(&dir_list, num_files);

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
	demo_table.rows(num_disp_files /*ItemTableMap.GetSize()*/);
	demo_table.row_height_all(20);

	demo_table.col_header(true);
	demo_table.col_header_height(25);
	demo_table.col_resize(true);
	demo_table.cols(1 /*2*/);
	demo_table.col_width_all(150);

	Fl_Menu_Bar menubar(0, 0, window.w(), 30);
	menubar.menu(menu_table);
	menubar.callback(test_cb);

	// TODO: need to add the positions and widths/heights as variables
	Fl_Tabs info_tabs(0, _gl_height+40, window.w(), window.h()-_gl_height-30-10);
		Fl_Group group(0, _gl_height+40+35, window.w(), window.h()-_gl_height-30-10, "Table Info");
			tbl_id        = new Fl_Int_Input(80, _gl_height+30+35+20+5, 240, 30, "ID:");
			tbl_ext_index = new Fl_Int_Input(80, _gl_height+30+35+20+40+5, 240, 30, "Ext Index:");
			tbl_name      = new Fl_Input    (80, _gl_height+30+35+20+80+5, 240, 30, "Name:");
			tbl_remark    = new Fl_Input    (80, _gl_height+30+35+20+120+5, 240, 30, "Remark:");
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
