// MachineBase.cpp: implementation of the CMachineBase class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "MachineBase.h"
#include "N3Terrain.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMachineBase::CMachineBase()
{
	m_fRotateSpeed = 0.15f;
	m_fSpeed = 1.0f;
	m_bSkipCalcPartMtx = NULL;
	Release();
}

CMachineBase::~CMachineBase()
{
	Release();
}

void CMachineBase::Release()
{
	CN3Shape::Release();
	ZeroMemory( m_Wheel, sizeof(m_Wheel) );
	m_dwMachineState = MS_STOP;
	m_fDirRadian = 0.0f;
	m_szID = "";
	if (m_bSkipCalcPartMtx) {delete [] m_bSkipCalcPartMtx; m_bSkipCalcPartMtx = NULL;}

	m_fFireRadian = 0.0f;
	m_fFireSpeed = 0.0f;
}

void CMachineBase::ReCalcMatrix4AnimatedPart()
{
	// 바퀴 파트의 매트릭스를 다시 계산해 준다..
	int iPC = m_Parts.size();
	for(int i = 0; i < iPC; i++)
	{
		for (int j=0; j<NUM_WHEEL; ++j)
		{
			if (m_Parts[i] == m_Wheel[j].pPart)
			{
				m_Parts[i]->m_Matrix.RotationX(m_Wheel[j].fRadian);
				m_Parts[i]->m_Matrix.PosSet(m_Parts[i]->m_vPivot);
				m_Parts[i]->m_Matrix *= m_Matrix;
				m_bSkipCalcPartMtx[j] = TRUE;
				break;
			}
		}
	}

}

void CMachineBase::ReCalcMatrix()
{
//	CN3Transform::ReCalcMatrix(); // Transfomr Matrix 를 계산 해주고..
/*
	// 균형잡기 위한 계산
	static __Matrix44 mtx1;	static __Matrix44 mtx2;
	mtx1.Scale(m_vScale);	mtx2.Rotation(0, m_fDirRadian, 0);
	mtx1 *= mtx2;	mtx1.PosSet(m_vPos);

	float fPitch=0, fRoll=0;
	CN3Terrain* pTerrain =  CN3GameBase::s_pArith->m_pTerrain;
	__Vector3 vPos1 = m_vBalancePoint[0]*mtx1;
	__Vector3 vPos2 = m_vBalancePoint[1]*mtx1;
	vPos1.y = pTerrain->GetHeight(vPos1.x, vPos1.z);
	vPos2.y = pTerrain->GetHeight(vPos2.x, vPos2.z);
	if (vPos1.y != -FLT_MAX && vPos2.y != -FLT_MAX) fPitch = asinf((vPos2.y - vPos1.y)/(vPos1-vPos2).Magnitude());	// 지형범위 안일때 계산
	vPos1 = m_vBalancePoint[2]*mtx1;
	vPos2 = m_vBalancePoint[3]*mtx1;
	vPos1.y = pTerrain->GetHeight(vPos1.x, vPos1.z);
	vPos2.y = pTerrain->GetHeight(vPos2.x, vPos2.z);
	if (vPos1.y != -FLT_MAX && vPos2.y != -FLT_MAX) fRoll = asinf((vPos2.y - vPos1.y)/(vPos1-vPos2).Magnitude());	// 지형범위 안일때만 계산

	// machine의 스케일, 회전, 위치 계산
	D3DXMatrixRotationYawPitchRoll(&mtx1, -m_fDirRadian, fPitch, fRoll);
	// -m_fDirRadian : -를 붙여주는 이유는 N3Base에 정의된 matrix연산과 d3d의 연산이 반대로 되어 있어서.. 

	m_Matrix.Scale(m_vScale);
	m_Matrix *= mtx1;
	m_Matrix.PosSet(m_vPos);

	ReCalcMatrix4AnimatedPart();

	// Animate 되지 않는 나머지 파트의 매트릭스를 다시 계산.
	int iPC = m_Parts.size();
	for(int i = 0; i < iPC; i++)
	{
		if (m_bSkipCalcPartMtx[count] == FALSE)	m_Parts[i]->RecalcMatrix(m_Matrix);
		++count;
	}
*/
}

void CMachineBase::Render()
{
	CN3Shape::Render();
	if (m_bSkipCalcPartMtx) ZeroMemory(m_bSkipCalcPartMtx, sizeof(m_bSkipCalcPartMtx[0])*PartCount());		// 그린후 part matrix계산 플래그 초기화
}

void CMachineBase::Tick(float fFrm)
{
/*	CN3Shape::Tick(fFrm);
	if (m_bDontRender) return;

	// 회전 계산
	if (!((m_dwMachineState & MS_TURNRIGHT) && (m_dwMachineState & MS_TURNLEFT)))	// 좌우 동시가 아니면
	{
		float fAddRadian = 0.0f;
		if (m_dwMachineState & MS_TURNRIGHT)
		{
			fAddRadian = -m_fRotateSpeed*s_fSecPerFrm;
			m_fDirRadian += fAddRadian;
			if (m_fDirRadian < 0) m_fDirRadian += (2 * D3DX_PI);
		}
		else if (m_dwMachineState & MS_TURNLEFT)
		{
			fAddRadian = m_fRotateSpeed*s_fSecPerFrm;
			m_fDirRadian += fAddRadian;
			if (m_fDirRadian > (2 * D3DX_PI)) m_fDirRadian -= (2 * D3DX_PI);
		}
		// 바퀴 회전각 계산.
		int i;
		for (i=0; i<NUM_WHEEL; ++i)
		{
			m_Wheel[i].fRadian += fAddRadian*m_Wheel[i].fRotateRatio;
		}
	}

	float f90d = D3DXToRadian(90.0f);
	__Vector3 vDir;	vDir.Set(cosf(f90d+m_fDirRadian), 0, sinf(f90d+m_fDirRadian));
	__Vector3 vPosDiff;	vPosDiff.Set(0,0,0);

	// 전후 이동 계산.
	if (!((m_dwMachineState & MS_FORWARD) && (m_dwMachineState & MS_BACKWARD)))	// 앞뒤 동시가 아니면
	{
		float fDistance = 0.0f;
		if (m_dwMachineState & MS_FORWARD) fDistance = (m_fSpeed*s_fSecPerFrm);
		else if (m_dwMachineState & MS_BACKWARD) fDistance = (-m_fSpeed*s_fSecPerFrm);

		if (fDistance != 0.0f)
		{
			vPosDiff = vDir*fDistance;

			// 지형에 따른 y좌표 계산
			__Vector3 vPos = m_vPos + vPosDiff;
			vPos.y = CN3GameBase::s_pArith->m_pTerrain->GetHeight(vPos.x, vPos.z);
			m_vPos = vPos;

			// 바퀴 회전각 계산.
			int i;
			for (i=0; i<NUM_WHEEL; ++i)
			{
				m_Wheel[i].fRadian -= (fDistance/m_Wheel[i].fRadius);
			}
		}
	}

	// 바퀴 회전각 0~2pi 사이로 조정
	int i;
	for (i=0; i<NUM_WHEEL; ++i)
	{
		if (m_Wheel[i].fRadian > D3DX_PI*2)	m_Wheel[i].fRadian -= (D3DX_PI*2);
		else if (m_Wheel[i].fRadian < 0.0f)	m_Wheel[i].fRadian += (D3DX_PI*2);
	}

	// Rotate 계산
//	m_vRot.y = m_fDirRadian;

	// 이동 후 matrix계산.
	ReCalcMatrix();
*/
}

CN3SPart* CMachineBase::GetPartByPMeshName(const std::string& szName)
{
	if(szName.empty()) return NULL;

	int iPC = m_Parts.size();
	for(int i = 0; i < iPC; i++)
	{
		CN3PMesh* pPMesh = m_Parts[i]->Mesh();
		if (pPMesh == NULL) continue;
		if (pPMesh->m_szName == szName) return m_Parts[i];
	}
	
	return NULL;
}

void CMachineBase::LoadMachine(FILE* stream)
{
	if (stream == NULL) return;

	Release();
	char szSrcName[_MAX_PATH];	// shape source파일 이름
	char szWheel[NUM_WHEEL][_MAX_PATH];	// 바퀴 pmesh이름

	int result;
	result = fscanf(stream, "Speed = %f\n", &m_fSpeed);	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "RotateSpeed = %f\n", &m_fRotateSpeed);	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Shape_Name = %s\n", szSrcName);					__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Wheel_FL = %s\n", szWheel[WHEEL_FL]);			__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Wheel_FR = %s\n", szWheel[WHEEL_FR]);			__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Wheel_BL = %s\n", szWheel[WHEEL_BL]);			__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
	result = fscanf(stream, "Wheel_BR = %s\n", szWheel[WHEEL_BR]);			__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
//	result = fscanf(stream, "WheelRadius_FL = %f\n", &(m_Wheel[WHEEL_FL].fRadius));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
//	result = fscanf(stream, "WheelRadius_FR = %f\n", &(m_Wheel[WHEEL_FR].fRadius));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
//	result = fscanf(stream, "WheelRadius_BL = %f\n", &(m_Wheel[WHEEL_BL].fRadius));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");
//	result = fscanf(stream, "WheelRadius_BR = %f\n", &(m_Wheel[WHEEL_BR].fRadius));	__ASSERT(result != EOF, "잘못된 Machine 세팅 파일");

	// shape load하기
	this->Load(szSrcName);

	__ASSERT(m_bSkipCalcPartMtx == NULL, "Machine에서 메모리 릭 가능성");
	int iPartCount = PartCount();
	if (iPartCount>0) m_bSkipCalcPartMtx = new BOOL[iPartCount];
	ZeroMemory(m_bSkipCalcPartMtx, sizeof(m_bSkipCalcPartMtx[0])*iPartCount);

	// 각각의 바퀴 CN3SPart 포인터 찾기
	int i;
	for (i=0; i<NUM_WHEEL; ++i)
	{
		m_Wheel[i].pPart = GetPartByPMeshName(szWheel[i]);
		__ASSERT(m_Wheel[i].pPart, "Machine의 바퀴 파트가 NULL입니다.");
		// 바퀴 반지름 구하기
		CN3PMesh* pPMesh = m_Wheel[i].pPart->Mesh();
		__ASSERT(pPMesh, "machine 바퀴의 PMesh가 없어요.");
		m_Wheel[i].fRadius = (pPMesh->Max().y - pPMesh->Min().y)/2.0f;
	}

	// machine이 1.0f(rad)회전할때 바퀴가 돌아가는 각도(rad) 정도 계산하기
	for (i=0; i<NUM_WHEEL; ++i)
	{
		if (i == WHEEL_FL || i == WHEEL_BL)
			m_Wheel[i].fRotateRatio = m_Wheel[i].pPart->m_vPivot.Magnitude() / m_Wheel[i].fRadius;
		else
			m_Wheel[i].fRotateRatio = -m_Wheel[i].pPart->m_vPivot.Magnitude() / m_Wheel[i].fRadius;
	}

	// 균형을 잡기위한 점 계산하기
	m_vBalancePoint[0] =	m_Wheel[WHEEL_FL].pPart->m_vPivot +				// 전
							0.5*(m_Wheel[WHEEL_FR].pPart->m_vPivot - m_Wheel[WHEEL_FL].pPart->m_vPivot);
	m_vBalancePoint[1] =	m_Wheel[WHEEL_BL].pPart->m_vPivot +				// 후
							0.5*(m_Wheel[WHEEL_BR].pPart->m_vPivot - m_Wheel[WHEEL_BL].pPart->m_vPivot);
	m_vBalancePoint[2] =	m_Wheel[WHEEL_FL].pPart->m_vPivot +				// 좌
							0.5*(m_Wheel[WHEEL_BL].pPart->m_vPivot - m_Wheel[WHEEL_FL].pPart->m_vPivot);
	m_vBalancePoint[3] =	m_Wheel[WHEEL_FR].pPart->m_vPivot +				// 우
							0.5*(m_Wheel[WHEEL_BR].pPart->m_vPivot - m_Wheel[WHEEL_FR].pPart->m_vPivot);
}

void CMachineBase::Fire()
{
}

