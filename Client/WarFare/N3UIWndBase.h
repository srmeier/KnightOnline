﻿// N3UIWndBase.h: interface for the CN3UIWndBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3UIWNDBASE_H__A30E8AD0_2EB8_4F27_8E0D_3E0979560761__INCLUDED_)
#define AFX_N3UIWNDBASE_H__A30E8AD0_2EB8_4F27_8E0D_3E0979560761__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3UIBase.h"
#include "N3UIArea.h"
#include "N3UIIcon.h"
#include "GameProcedure.h"
#include "GameDef.h"

#include <string>

//////////////////////////////////////////////////////////////////////

// Wnd Info..
enum e_UIWND				{ 
								UIWND_INVENTORY = 0,		// Inventory Wnd..
								UIWND_TRANSACTION, 			// Transaction Wnd..
								UIWND_DROPITEM,				// Drop Item Wnd..
								UIWND_PER_TRADE,			// 개인간 거래..
								UIWND_SKILL_TREE,			// Skill Icon Info Wnd..
								UIWND_HOTKEY,				// Hot Key Wnd..
								UIWND_PER_TRADE_EDIT,		// Per Trade Edit Wnd..
								UIWND_EXCHANGE_REPAIR,		// Exchange Repair Wnd..
								UIWND_WARE_HOUSE,			// 보관함..
								UIWND_UNKNOWN,				// Wnd Unknown..
							};

// District Info..
enum e_UIWND_DISTRICT		{	
								UIWND_DISTRICT_INVENTORY_SLOT = 0,	// Slot district of Inventory Wnd..
								UIWND_DISTRICT_INVENTORY_INV,		// Inv district of Inventory Wnd..
								UIWND_DISTRICT_TRADE_NPC,			// Transaction district of Transaction Wnd of Npc..
								UIWND_DISTRICT_PER_TRADE_MY,		// My Transaction district of Per Transaction Wnd..
								UIWND_DISTRICT_PER_TRADE_OTHER,		// Other Transaction district of Per Transaction Wnd..
								UIWND_DISTRICT_DROPITEM,			// Dropitem district of Drop item wnd..
								UIWND_DISTRICT_SKILL_TREE,			// Skillicon district of Skill icon wnd..
								UIWND_DISTRICT_SKILL_HOTKEY,		// Skillicon district of Hotkey icon wnd..
								UIWND_DISTRICT_EX_RE_NPC,			// Exchange Repair Wnd Npc District..			
								UIWND_DISTRICT_EX_RE_INV,			// Exchange Repair Wnd Inv District..
								UIWND_DISTRICT_TRADE_MY,			// Npc 와의 거래에서 내 영역..
								UIWND_DISTRICT_PER_TRADE_INV,		// Inv District of Per Trade Wnd ..
								UIWND_DISTRICT_UNKNOWN,				// District Unknown..
							};

enum e_UIIconState			{
								UIICON_SELECTED = 0,				// Icon Selected..
								UIICON_NOT_SELECTED_BUT_HIGHLIGHT,	// Icon Not Selected But Highlight..
							};

// Total Wnd Info.. 
struct __UIWndIconInfo		{
								e_UIWND				UIWnd;
								e_UIWND_DISTRICT	UIWndDistrict;
								int					iOrder;
							};

// enum Icon Type Info..//
enum e_UIIconType			{						
								UIICON_TYPE_ITEM = 0,				// Icon type item..
								UIICON_TYPE_SKILL,					// Icon type skill..
							};

// Item Info..
struct	__IconItemSkill		{
//								e_UIIconType			eIconType;
								CN3UIIcon*				pUIIcon;
								std::string				szIconFN;

								union {
									struct {
										__TABLE_ITEM_BASIC*	pItemBasic;		// Item.. ^^
										__TABLE_ITEM_EXT*	pItemExt;
										int					iCount;
										int					iDurability;	// 내구력
										};

									__TABLE_UPC_SKILL*		pSkill;			// Skill.. ^^
								};
							};

// Select Icon Info..
struct __InfoSelectedIcon	{
								__UIWndIconInfo		UIWndSelect;
								__IconItemSkill*	pItemSelect;					
								
								__InfoSelectedIcon() { memset(this, 0, sizeof(__InfoSelectedIcon)); }
							};								

// Recovery Job Info..
struct __RecoveryJobInfo	{ 
								bool				m_bWaitFromServer;
								__IconItemSkill*	pItemSource;					
								__UIWndIconInfo		UIWndSourceStart;
								__UIWndIconInfo		UIWndSourceEnd;
								__IconItemSkill*	pItemTarget;
								__UIWndIconInfo		UIWndTargetStart;
								__UIWndIconInfo		UIWndTargetEnd;
								int					m_iPage;
							};

struct __SkillSelectInfo	{
								e_UIWND				UIWnd;
								int					iOrder;
								__IconItemSkill*	pSkillDoneInfo;
							};

const int UIITEM_TYPE_ONLYONE = 0;
const int UIITEM_TYPE_COUNTABLE = 1;
const int UITEIM_TYPE_GOLD = 2;
const int UIITEM_TYPE_COUNTABLE_SMALL = 3;
const int UIITEM_TYPE_SOMOONE = 4;

const int UIITEM_TYPE_SONGPYUN_ID_MIN = 450000;

const int UIITEM_COUNT_MANY = 9999;
const int UIITEM_COUNT_FEW = 500;

//////////////////////////////////////////////////////////////////////

class CUIImageTooltipDlg;
class CCountableItemEditDlg;

// Class ^^
class CN3UIWndBase  : public CN3UIBase		// 가상 함수로 자식의 Area 갯수를 파악할 수 있는 함수가 있어야 하지 않을 까???
											// int GetChildCountByAreaType(eAreatype .. ) ^^
{
	void				PlayItemEtcSound();
	void				PlayItemWeaponSound();
	void				PlayItemArmorSound();

public:
	static __InfoSelectedIcon		m_sSelectedIconInfo;
	static __RecoveryJobInfo		m_sRecoveryJobInfo;
	static __SkillSelectInfo		m_sSkillSelectInfo;
	static CN3UIImage*				m_pSelectionImage;
	static CCountableItemEditDlg*	m_pCountableItemEdit;

protected:
	e_UIWND						m_eUIWnd;

	static int					s_iRefCount; // 참조 카운트...
	static CN3SndObj*			s_pSnd_Item_Etc;
	static CN3SndObj*			s_pSnd_Item_Weapon;
	static CN3SndObj*			s_pSnd_Item_Armor;
	static CN3SndObj*			s_pSnd_Gold;
	static CN3SndObj*			s_pSnd_Repair;

protected:
	virtual void				InitIconWnd(e_UIWND eWnd);
	virtual void				InitIconUpdate() = 0;

public:
	CN3UIWndBase();
	virtual ~CN3UIWndBase();

	e_UIWND						GetUIWnd() { return m_eUIWnd; }

	virtual CN3UIArea*			GetChildAreaByiOrder(eUI_AREA_TYPE eUAT, int iOrder);
	virtual CN3UIString*		GetChildStringByiOrder(int iOrder);

	virtual uint32_t				MouseProc(uint32_t dwFlags, const POINT& ptCur, const POINT& ptOld);
	virtual void				AllHighLightIconFree();

	virtual __IconItemSkill*	GetHighlightIconItem(CN3UIIcon* pUIIcon) = 0;
	virtual void				IconRestore() {}	
	virtual bool				CheckIconDropFromOtherWnd(__IconItemSkill* spItem) { return false; }
	virtual bool				ReceiveIconDrop(__IconItemSkill* spItem, POINT ptCur) { return false; }
	virtual void				CancelIconDrop(__IconItemSkill* spItem) {}
	virtual void				AcceptIconDrop(__IconItemSkill* spItem) {}

	virtual void				PlayItemSound(__TABLE_ITEM_BASIC* pBasic);
	virtual void				PlayGoldSound();
	virtual void				PlayRepairSound();
};

#endif // !defined(AFX_N3UIWNDBASE_H__A30E8AD0_2EB8_4F27_8E0D_3E0979560761__INCLUDED_)
