#ifndef __ITEMINFO_H_
#define __ITEMINFO_H_

#include "defines.h"

#include "STLMap.h"
#include "N3TableBase.h"

//-----------------------------------------------------------------------------
class ItemInfo {
private:
	CSTLMap<_ITEM_TABLE> dbItemInfo;
	CN3TableBase<__TABLE_ITEM_BASIC>* tblItemInfo;
};

#endif
