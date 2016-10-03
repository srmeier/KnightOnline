// N3GlobalEffectMng.h: interface for the CN3GlobalEffectMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_N3GLOBALEFFECTMNG_H__E95C3268_DD68_45AE_AA24_DC64B11A1B08__INCLUDED_)
#define AFX_N3GLOBALEFFECTMNG_H__E95C3268_DD68_45AE_AA24_DC64B11A1B08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "N3Base.h"

enum {	GEW_CLEAR=0,			// ¸¼À½
		GEW_DRIZZLE_RAIN=1,		// °¡¶ûºñ
		GEW_RAINY=2,			// Àû´ç·®ÀÇ ºñ
		GEW_HEAVY_RAIN=3,		// Æø¿ì
		GEW_SNOW1=11,			// Æ÷±ÙÇÑ ´«
		GEW_SNOW2=12,			// Àû´çÇÑ ´«
		GEW_HEAVY_SNOW=13		// Æø¼³
};

class CN3GERain;
class CN3GESnow;
class CN3GlobalEffectMng : public CN3Base  
{
public:
	CN3GlobalEffectMng();
	virtual ~CN3GlobalEffectMng();

// Attributes
public:
protected:
	float			m_fCellSize;	// ³»ºÎ ¼¿ Å©±â
	POINT			m_CurCellPos;	// ÇöÀç ³»ºÎ¼¿ ÁÂÇ¥
	__Vector3		m_vPos[9];		// ÁÖº¯ ¼¿ 9Ä­ÀÇ Áß½ÉÁÂÇ¥µé

	// È¿°úµé..
	CN3GERain*		m_pGERain;		// ºñ
	CN3GESnow*		m_pGESnow;		// ´«

// Operations
public:
	virtual void	Release();
	void			Tick();
	void			Render();
//	void			SetWeather(int iWeather);	// ³¯¾¾ ÁöÁ¤ÇÏ±â
	void			WeatherSetRainy(int iPercent);
	void			WeatherSetSnow(int iPercent);
	void			WeatherSetClean();

protected:

};

#endif // !defined(AFX_N3GLOBALEFFECTMNG_H__E95C3268_DD68_45AE_AA24_DC64B11A1B08__INCLUDED_)
