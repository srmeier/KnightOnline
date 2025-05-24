// CUIStatSkillReset.h: interface for the CUIStatSkillReset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UICmd_H__CA4F5382_D9A9_447C_B717_7A0A38724715__INCLUDED_)
#define AFX_UICmd_H__CA4F5382_D9A9_447C_B717_7A0A38724715__INCLUDED_
#endif // !defined(AFX_UICmd_H__CA4F5382_D9A9_447C_B717_7A0A38724715__INCLUDED_)

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameDef.h"

#include "N3UIBase.h"
#include "N3UIButton.h"
#include "N3UIString.h"
#include "N3UIImage.h"
#include "PlayerMySelf.h"


class CUIStatSkillReset : public CN3UIBase
{
private:
	__InfoPlayerBase* m_pInfo;
	__InfoPlayerMySelf* m_pInfoExt;
public:
	CN3UIButton* m_pBtn_WalkAway;
	CN3UIButton* m_pBtn_ResetStats;
	CN3UIButton* m_pBtn_ResetSkills;

	CN3UIString* m_pStr_TextWalkAway;
	CN3UIString* m_pStr_TextResetStats;
	CN3UIString* m_pStr_TextResetSkills;
	CN3UIString* m_pStr_TextTitle;


public:
	int m_iRequiredCoinsStatReset = 0;
	int m_iRequiredCoinsSkillReset = 0;
	uint8_t m_iStatResetLevelMin = 10;
	uint8_t m_iSkillResetLevelMin = 10;
	std::string m_sRequiredCoinsStatReset;
	std::string m_sRequiredCoinsSkillReset;

	bool OnKeyPress(int iKey);
	void Release();
	void Open();
	void SetVisible(bool bVisible);
	bool	Load(HANDLE hFile);
	bool	ReceiveMessage(CN3UIBase* pSender, uint32_t dwMsg);
	void MsgSend_ResetStats();
	void MsgSend_ResetSkills();
	bool CanChangeStats();
	bool CanChangeSkills();
	void ShowStatErrorLevel();
	void ShowStatErrorCoins();
	void ShowStatErrorEquipedItem();
	void ShowSkillErrorLevel();
	void ShowSkillErrorCoins();
	void ShowStatErrorUnusedPoints();
	void ShowSkillErrorUnusedPoints();
	void ShowStatResetSuccess();
	void ShowSkillResetSuccess();
	void UpdateRequiredCoinValues();
	std::string FormatCoin(int iCoin);
	CUIStatSkillReset();
	virtual ~CUIStatSkillReset();
};
