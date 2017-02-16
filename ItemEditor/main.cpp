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

const int _gl_width = 380;
const int _gl_height = 1024/3;

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
	}
};

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

			// TODO: set the click callback, don't do this for every draw call
			if(row_selected(r) && (c==0)) {
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
