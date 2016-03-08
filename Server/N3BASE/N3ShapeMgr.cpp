#include "stdafx.h"

CN3ShapeMgr::CN3ShapeMgr()
{
	m_fMapWidth = 0.0f;
	m_fMapLength = 0.0f;
	m_nCollisionFaceCount = 0;
	m_pvCollisions = nullptr;
	memset(m_pCells, 0, sizeof(m_pCells));
}

CN3ShapeMgr::~CN3ShapeMgr()
{
	if (m_pvCollisions != nullptr)
	{
		delete [] m_pvCollisions; 
		m_pvCollisions = nullptr;
	}

	for(int z = 0; z < MAX_CELL_MAIN; z++)
	{
		for(int x = 0; x < MAX_CELL_MAIN; x++)
		{
			delete m_pCells[x][z];
		}
	}
}

void CN3ShapeMgr::unlock()
{
	m_fMapWidth = 0.0f;
	m_fMapLength = 0.0f;
	m_nCollisionFaceCount = 0;

	if (m_pvCollisions != nullptr)
	{
		delete [] m_pvCollisions; 
		m_pvCollisions = nullptr;
	}

	for(int z = 0; z < MAX_CELL_MAIN; z++)
	{
		for(int x = 0; x < MAX_CELL_MAIN; x++)
		{
			delete m_pCells[x][z];
			m_pCells[x][z] = nullptr;
		}
	}
	memset(m_pCells, 0, sizeof(MAX_CELL_MAIN));
}

bool CN3ShapeMgr::LoadCollisionData(FILE *fp)
{
	if (fread(&m_fMapWidth, 4, 1, fp) != 1
		|| fread(&m_fMapLength, 4, 1, fp) != 1)
	{
		ASSERT(0);
		return false;
	}

	Create(m_fMapWidth, m_fMapLength);

	if (fread(&m_nCollisionFaceCount, 4, 1, fp) != 1)
	{
		ASSERT(0);
		return false;
	}

	if (m_pvCollisions != nullptr)
	{
		delete [] m_pvCollisions; 
		m_pvCollisions = nullptr;
	}

	if (m_nCollisionFaceCount > 0)
	{
		m_pvCollisions = new __Vector3[m_nCollisionFaceCount * 3];
		if (fread(m_pvCollisions, sizeof(__Vector3) * m_nCollisionFaceCount * 3, 1, fp) != 1)
		{
			ASSERT(0);
			return false;
		}
	}

	// Cell data
	int z = 0;
	for(float fZ = 0.0f; fZ < m_fMapLength; fZ += CELL_MAIN_SIZE, z++)
	{
		int x = 0;
		for(float fX = 0.0f; fX < m_fMapWidth;  fX += CELL_MAIN_SIZE, x++)
		{
			if (m_pCells[x][z] != nullptr)
			{
				delete m_pCells[x][z]; 
				m_pCells[x][z] = nullptr;
			}

			uint32 bExist;
			if (fread(&bExist, 4, 1, fp) != 1)
			{
				ASSERT(0);
				return false;
			}

			if (!bExist) 
				continue;

			m_pCells[x][z] = new __CellMain;
			m_pCells[x][z]->Load(fp);
		}
	}

	return true;
}

bool CN3ShapeMgr::Create(float fMapWidth, float fMapLength) // 맵의 너비와 높이를 미터 단위로 넣는다..
{
	if(	fMapWidth <= 0.0f || fMapWidth > MAX_CELL_MAIN * CELL_MAIN_SIZE ||
		fMapLength <= 0.0f || fMapLength > MAX_CELL_MAIN * CELL_MAIN_SIZE )
	{
		return false;
	}

	m_fMapWidth = fMapWidth;
	m_fMapLength = fMapLength;

	return true;
}
