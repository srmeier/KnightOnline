// Catapult.cpp: implementation of the CCatapult class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Catapult.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCatapult::CCatapult()
{
	m_Thrower.pStone = NULL;
	Release();
}

CCatapult::~CCatapult()
{
	Release();
}

void CCatapult::Release()
{
	CMachineBase::Release();
	if (m_Thrower.pStone) { delete m_Thrower.pStone; m_Thrower.pStone = NULL;}
	ZeroMemory(&m_Thrower, sizeof(m_Thrower));
}

void CCatapult::ReCalcMatrix4AnimatedPart()
{
	CMachineBase::ReCalcMatrix4AnimatedPart();

	if (m_Thrower.bFire == TRUE)	// 발사 상태이면
	{
		int iSize = m_Parts.size();
		for(int i = 0; i < iSize; i++)
		{
			if (m_Parts[i] == m_Thrower.pThrowerPart)
			{
				m_Parts[i]->m_Matrix.RotationX(m_Thrower.fCurRadian);
				m_Parts[i]->m_Matrix.PosSet(m_Parts[i]->m_vPivot);
				m_Parts[i]->m_Matrix *= m_Matrix;
				m_bSkipCalcPartMtx[i] = TRUE;
				break;
			}
		}
	}
}

void CCatapult::Render()
{
	CMachineBase::Render();

	__ASSERT(m_Thrower.pStone, "투석기에 돌맹이 없음");
	if (m_Thrower.bDontRenderStone == FALSE ) m_Thrower.pStone->Render();
}

void CCatapult::Tick(float fFrm)
{
/*	if (m_bDontRender)
	{
		CN3Shape::Tick(fFrm);
		if (m_bDontRender) return;
	}

	BOOL IsStoneThrown = FALSE;
	if (m_Thrower.bFire == TRUE)	// 발사 상태이면
	{
		m_Thrower.fTime += s_fSecPerFrm;
		float fReloadTime = m_Thrower.fTime - (m_Thrower.fReleaseTime + m_Thrower.fReloadDelayTime);
		if (fReloadTime<0)	// 발사중이거나 delay상태중..
		{
			float fVibrationTime = m_Thrower.fTime - m_Thrower.fReleaseTime;
			if (fVibrationTime<0)
			{
				m_Thrower.fCurRadian = -m_Thrower.fRadianAccel*(m_Thrower.fTime*m_Thrower.fTime);
			}
			else // 발사후 반동으로 인해 진동중 
			{
				if (m_Thrower.bDontRenderStone == FALSE)
				{
					m_Thrower.bDontRenderStone = TRUE;	// 돌덩이 안그리기

					// 돌 날리는 메세지 발생 해야 한다.(한번만)
					IsStoneThrown = TRUE;
				}

				float fFactor = fVibrationTime*20.0f;
				// 진동 5번만..
				if ( fFactor < D3DX_PI*8) m_Thrower.fCurRadian = -m_Thrower.fLimitRadian - sinf(fFactor) * (m_Thrower.fRecoilRadian/(10*(1.0f+fVibrationTime)*(1.0f+fVibrationTime)));
				else m_Thrower.fCurRadian = -m_Thrower.fLimitRadian;
			}
		}
		else
		{
			m_Thrower.bDontRenderStone = TRUE;	// 돌덩이 안그리기

			m_Thrower.fCurRadian = -(m_Thrower.fLimitRadian - fReloadTime*m_Thrower.fRadianSpeed2Reload);
			if (m_Thrower.fCurRadian > 0)
			{
				m_Thrower.bFire = FALSE;
				m_Thrower.fCurRadian = 0;
				m_Thrower.fTime = 0;
				m_Thrower.bDontRenderStone = FALSE;
			}
		}
	}
	CMachineBase::Tick(fFrm);

	// 돌덩이 위치 계산
	if (m_Thrower.bDontRenderStone == FALSE)
	{
		__Matrix44& mtx = m_Thrower.pStone->m_Matrix;
		mtx.Identity();
		mtx.PosSet(m_Thrower.vStoneOffset);
		mtx *= m_Thrower.pThrowerPart->m_Matrix;
		m_Thrower.pStone->ReCalcPartMatrix();
		m_Thrower.pStone->PosSet(mtx._41, mtx._42, mtx._43);
		m_Thrower.pStone->Tick(fFrm);
	}

	if (IsStoneThrown)
	{
		__Matrix44 mtx; // 회전 행렬 구하기..
		mtx.Identity();
		mtx.RotationX(m_Thrower.fLimitRadian);
		mtx.PosSet(m_Thrower.pThrowerPart->m_vPivot);
		mtx *= m_Matrix;

		// 돌 날리는 메세지 발생
		// tigger - 08.25 직접 메시지를 만들어서 날린다..
		LocalMsg Msg;
		Msg.s_MagicNum			= MAGIC_NUM;
		Msg.s_iAddressLen		= CN3GameBase::s_pMsgRouter->GetThisClassLevel(ID_FX_MGR);
		Msg.s_iAddIndex			= 0;
		Msg.s_ipAddress[0]		= ID_ARITH;
		Msg.s_ipAddress[1]		= ID_FX_MGR;

		Msg.s_iGameID			= ID_THROW_STONE;
		Msg.s_vPos				= m_Thrower.vStoneOffset * mtx; // 위치
		__Vector3 vVel; vVel.Set(0, sinf(m_fFireRadian), cosf(m_fFireRadian)); // 방향 계산.
		mtx = m_Matrix;	mtx.PosSet(0,0,0);
		Msg.s_vDir= vVel*mtx; Msg.s_vDir.Normalize();// 방향
		Msg.s_fVelocity			= m_fFireSpeed; // 날아가는 스피드..
		Msg.s_pShape			= m_Thrower.pStone; // 돌 Shape
		
		CN3GameBase::s_pMsgRouter->m_cMsgQueue.RouteLocalMsg(Msg); // Message 보냄..
	}
*/
}

void CCatapult::LoadMachine(FILE* stream)
{
	if (stream == NULL) return;
	Release();
	CMachineBase::LoadMachine(stream);

	char szThrowerName[_MAX_PATH];	// Thrower pmesh파일 이름
	char szStoneShapeName[_MAX_PATH];	// stone shape의 이름

	int result;
	float x, y, z;
	result = fscanf(stream, "Thrower_Name = %s\n", &szThrowerName);						__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Thrower_LimitRadian = %f\n", &(m_Thrower.fLimitRadian));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Thrower_RadianAccel = %f\n", &(m_Thrower.fRadianAccel));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Thrower_RadianSpeed2Reload = %f\n", &(m_Thrower.fRadianSpeed2Reload));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Thrower_ReloadDelayTime = %f\n", &(m_Thrower.fReloadDelayTime));		__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Thrower_RecoilRadian = %f\n", &(m_Thrower.fRecoilRadian));				__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Thrower_StoneShapeName = %s\n", szStoneShapeName);						__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Thrower_StoneOffset = %f %f %f\n", &x, &y, &z);						__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");

	__ASSERT(m_Thrower.pStone == NULL, "catapult memory leak 가능성 존재");
	m_Thrower.pStone = new CN3Shape;
	m_Thrower.pStone->Load(szStoneShapeName);
	m_Thrower.vStoneOffset.Set(x, y, z);

	// 발사에 걸리는 시간 계산
	m_Thrower.fReleaseTime = sqrtf(m_Thrower.fLimitRadian/m_Thrower.fRadianAccel);

	// Thrower 찾기
	m_Thrower.pThrowerPart = GetPartByPMeshName(szThrowerName);
}

void CCatapult::Fire()
{
	if (m_Thrower.bFire == TRUE) return;	// 이미 발사상태이다.

	m_Thrower.bFire = TRUE;
	m_Thrower.fTime = 0;
	m_Thrower.fCurRadian = 0;
	m_Thrower.bDontRenderStone = FALSE;	// 돌덩이 그리기
}
