#ifndef __ITEMINFO_H_
#define __ITEMINFO_H_

#include "defines.h"

#include "STLMap.h"
#include "N3TableBase.h"

/*
- goal is to be able to just plug one of these guys into the "Fl_Tabs" and have
	all their information be displayed
- this can house all the items and create a copy of the selected one which the
	user can modify until they go to save their changes (just need to keep
	track of the index/itorator)
*/

//-----------------------------------------------------------------------------
class ItemInfo {
private:
	CSTLMap<_ITEM_TABLE> dbItemInfo;
	CN3TableBase<__TABLE_ITEM_BASIC>* tblItemInfo;
};

#endif
