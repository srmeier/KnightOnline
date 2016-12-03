// N3ShapeEx.cpp: implementation of the CN3ShapeEx class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfxBase.h"
#include "N3ShapeEx.h"
//////////////////////////////////////////////////////////////////////
// CN3SPartEx
//////////////////////////////////////////////////////////////////////
CN3SPartEx::CN3SPartEx()
{
	m_ActionInfo.dwActionFlag = 0;
}

CN3SPartEx::~CN3SPartEx()
{
}

bool CN3SPartEx::Load(HANDLE hFile)
{
	if (false == CN3SPart::Load(hFile)) return false;

	DWORD dwRWC;

	ReadFile(hFile, &(m_ActionInfo.dwActionFlag), sizeof(m_ActionInfo.dwActionFlag), &dwRWC, NULL);
	int iStateCount = 0;
	if (m_ActionInfo.dwActionFlag) ReadFile(hFile, &iStateCount, sizeof(iStateCount), &dwRWC, NULL);	// 상태 정보의 갯수
	if (AF_POS & m_ActionInfo.dwActionFlag)
	{
		m_ActionInfo.ActionStateInfos_Pos.assign(iStateCount);
		for (int i=0; i<iStateCount; ++i)
			ReadFile(hFile, &(m_ActionInfo.ActionStateInfos_Pos[i]), sizeof(__Vector3), &dwRWC, NULL);	// 상태 정보의 갯수
	}
	if (AF_SCALE & m_ActionInfo.dwActionFlag)
	{
		m_ActionInfo.ActionStateInfos_Scale.assign(iStateCount);
		for (int i=0; i<iStateCount; ++i)
			ReadFile(hFile, &(m_ActionInfo.ActionStateInfos_Scale[i]), sizeof(__Vector3), &dwRWC, NULL);	// 상태 정보의 갯수
	}
	if (AF_ROTATION & m_ActionInfo.dwActionFlag)
	{
		m_ActionInfo.ActionStateInfos_Rot.assign(iStateCount);
		for (int i=0; i<iStateCount; ++i)
			ReadFile(hFile, &(m_ActionInfo.ActionStateInfos_Rot[i]), sizeof(__Quaternion), &dwRWC, NULL);	// 상태 정보의 갯수
	}

	return true;
}
#ifdef _N3TOOL
bool CN3SPartEx::Save(HANDLE hFile)
{
	return true;
}
#endif

void CN3SPartEx::Release()
{
	CN3SPart::Release();
	m_ActionInfo.dwActionFlag = 0;
	m_ActionInfo.ActionStateInfos_Pos.clear();
	m_ActionInfo.ActionStateInfos_Scale.clear();
	m_ActionInfo.ActionStateInfos_Rot.clear();
}

void CN3SPartEx::Interpolate(const __Matrix44& mtxParent, int iCurActionState, int iNextActionState, float fPercentage)
{
	if (m_bOutOfCameraRange) return;
	if (0 == m_ActionInfo.dwActionFlag) return;
}

//////////////////////////////////////////////////////////////////////
// CN3ShapeEx
//////////////////////////////////////////////////////////////////////

CN3ShapeEx::CN3ShapeEx()
{
	m_iActionStateCount = 0;
	m_iCurActionState = 0;	m_iPrevActionState = 0;
	m_fActionTimeChanged = 0.0f;
	m_fActionTimeChanging = 0.0f;

	m_ActionInfo.dwActionFlag = 0;
}

CN3ShapeEx::~CN3ShapeEx()
{
}

void CN3ShapeEx::Release()
{
	CN3Shape::Release();

	m_iActionStateCount = 0;
	m_iCurActionState = 0;	m_iPrevActionState = 0;
	m_fActionTimeChanged = 0.0f;
	m_fActionTimeChanging = 0.0f;

	m_ActionInfo.dwActionFlag = 0;
	m_ActionInfo.ActionStateInfos_Pos.clear();
	m_ActionInfo.ActionStateInfos_Scale.clear();
	m_ActionInfo.ActionStateInfos_Rot.clear();
}

bool CN3ShapeEx::Load(HANDLE hFile)
{
	if (false == CN3Shape::Load(hFile)) return false;

	DWORD dwRWC;
	ReadFile(hFile, &m_iActionStateCount, sizeof(m_iActionStateCount), &dwRWC, NULL);	// 상태 정보의 갯수

	ReadFile(hFile, &(m_ActionInfo.dwActionFlag), sizeof(m_ActionInfo.dwActionFlag), &dwRWC, NULL);
	int iStateCount = 0;
	if (m_ActionInfo.dwActionFlag) ReadFile(hFile, &iStateCount, sizeof(iStateCount), &dwRWC, NULL);	// 상태 정보의 갯수
	if (AF_POS & m_ActionInfo.dwActionFlag)
	{
		m_ActionInfo.ActionStateInfos_Pos.assign(iStateCount);
		for (int i=0; i<iStateCount; ++i)
			ReadFile(hFile, &(m_ActionInfo.ActionStateInfos_Pos[i]), sizeof(__Vector3), &dwRWC, NULL);	// 상태 정보의 갯수
	}
	if (AF_SCALE & m_ActionInfo.dwActionFlag)
	{
		m_ActionInfo.ActionStateInfos_Scale.assign(iStateCount);
		for (int i=0; i<iStateCount; ++i)
			ReadFile(hFile, &(m_ActionInfo.ActionStateInfos_Scale[i]), sizeof(__Vector3), &dwRWC, NULL);	// 상태 정보의 갯수
	}
	if (AF_ROTATION & m_ActionInfo.dwActionFlag)
	{
		m_ActionInfo.ActionStateInfos_Rot.assign(iStateCount);
		for (int i=0; i<iStateCount; ++i)
			ReadFile(hFile, &(m_ActionInfo.ActionStateInfos_Rot[i]), sizeof(__Quaternion), &dwRWC, NULL);	// 상태 정보의 갯수
	}

	return true;
}

void CN3ShapeEx::ReCalcMatrix()
{
/*
	// m_Matrix 다시 계산..
	CN3Transform::ReCalcMatrix(); // Transform Matrix 를 계산 해주고..

	// 각 파트의 매트릭스를 다시 계산.
	int iPC = m_Parts.size();
	for(int i = 0; i < iPC; i++)
	{
		if (m_pMatchPart2ActionPart[i])
		{
			__ActionPart* pModPart = m_pMatchPart2ActionPart[i];
			// Scale
			m_Parts[i]->m_Matrix.Scale(pModPart->CurStateInfo.vScale);
			// 회전
			static __Matrix44 mtxRot;
			D3DXMatrixRotationQuaternion(&mtxRot, &(pModPart->CurStateInfo.qRot));
			m_Parts[i]->m_Matrix *= mtxRot;

			// 위치
			m_Parts[i]->m_Matrix.PosSet(m_Parts[i]->m_vPivot + pModPart->CurStateInfo.vPos);
			m_Parts[i]->m_Matrix *= m_Matrix;
		}
		else
		{
			m_Parts[i]->ReCalcMatrix(m_Matrix);
		}
	}
	*/
}

void CN3ShapeEx::Tick(float fFrm)
{
	/*
	CN3Shape::Tick(fFrm);
	if (m_bDontRender) return;

	int i;
	if (m_fActionTimeChanged > 0.0f)
	{
		m_fActionTimeChanging += s_fSecPerFrm;
		if (m_fActionTimeChanging > m_fActionTimeChanged)
		{
			m_fActionTimeChanged = 0.0f;
			m_fActionTimeChanging = 0.0f;
			m_iPrevActionState = m_iCurActionState;

			for (i=0; i<m_iActionPartCount; ++i)
			{
				m_pActionParts[i].CurStateInfo = m_pActionParts[i].pStateInfos[m_iCurActionState];
			}
		}
		else
		{
			__ActionPart* pMP = NULL;
			for (i=0; i<m_iActionPartCount; ++i)
			{
				pMP = m_pActionParts + i;
				if (pMP->bPos)
				{
					pMP->CurStateInfo.vPos = pMP->pStateInfos[m_iPrevActionState].vPos + 
											(pMP->pStateInfos[m_iCurActionState].vPos - pMP->pStateInfos[m_iPrevActionState].vPos)*m_fActionTimeChanging/m_fActionTimeChanged;
				}
				if (pMP->bRot)
				{
					pMP->CurStateInfo.qRot.Slerp(pMP->pStateInfos[m_iPrevActionState].qRot, pMP->pStateInfos[m_iCurActionState].qRot, m_fActionTimeChanging/m_fActionTimeChanged);
				}
				if (pMP->bScale)
				{
					pMP->CurStateInfo.vScale = pMP->pStateInfos[m_iPrevActionState].vScale + 
											(pMP->pStateInfos[m_iCurActionState].vScale - pMP->pStateInfos[m_iPrevActionState].vScale)*m_fActionTimeChanging/m_fActionTimeChanged;
				}
			}
		}
	}

	ReCalcMatrix();
	*/
}

BOOL CN3ShapeEx::SetState(int iState, float fSec)
{
	if (iState<0 || iState>= m_iActionStateCount) return FALSE;
	if (m_iCurActionState == iState) return FALSE;
	m_fActionTimeChanged = fSec;
	m_fActionTimeChanging = 0.0f;

	m_iPrevActionState = m_iCurActionState;
	m_iCurActionState = iState;
	return TRUE;
}
