// Catapult.h: interface for the CCatapult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CATAPULT_H__9D404701_4B6D_4DFE_9BDD_7125FA9B3CE8__INCLUDED_)
#define AFX_CATAPULT_H__9D404701_4B6D_4DFE_9BDD_7125FA9B3CE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MachineBase.h"

class CCatapult : public CMachineBase
{
public:
	CCatapult();
	virtual ~CCatapult();

// Structure
public:
	struct __Thrower
	{
		// 파일에서 읽어오는 정보
		CN3SPart*	pThrowerPart;			// Thrower의 파트 포인터
		float	fLimitRadian;			// 한계 회전 각도
		float	fRadianAccel;			// 회전 가속도
		float	fRadianSpeed2Reload;	// 재장전 회전 속도
		float	fReloadDelayTime;		// 완전 발사후 재장전 시작하기까지의 시간 delay
		float	fRecoilRadian;			// 발사후 반동하는 각도
		class CN3Shape*	pStone;			// 발사하는 돌덩이 shape 포인터
		__Vector3		vStoneOffset;	// 돌덩이 offset (thrower의 pivot 점에서 떨어진 거리)

		// 파일에서 읽은 정보를 토대로 만들어지는 정보
		float	fReleaseTime;			// 발사후 한계 회전 각도에 도달하기까지의 시간 [ sqrtf(fLimitRadian/fRadianAccel) ]

		// 게임 중에 사용되는 정보
		float	fTime;					// 발사 경과 시간(초) 발시 시간0을 기준으로 축적됨.
		BOOL	bFire;					// 발사중인가?
		float	fCurRadian;				// 현재 Thrower가 돌아간 회전각
		BOOL	bDontRenderStone;		// 발사하는 돌을 안그릴 것인가?
	};
// Attributes
public:
protected:
	__Thrower		m_Thrower;			// 발사대

// Operations
public:
	virtual void	Release();
	virtual void	Tick(float fFrm);
	virtual void	LoadMachine(FILE* stream);
	virtual void	Render();
protected:
	virtual void	ReCalcMatrix4AnimatedPart();
	void			ThrowStone(__Vector3& vPos, __Vector3& vVel, CN3Shape* pShape) const;

// 머신 동작 관련
public:
	virtual void	Fire();		// 발사
};

#endif // !defined(AFX_CATAPULT_H__9D404701_4B6D_4DFE_9BDD_7125FA9B3CE8__INCLUDED_)
