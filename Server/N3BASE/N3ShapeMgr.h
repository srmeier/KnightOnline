#pragma once

#include "My_3DStruct.h"
#include <cstdio>

const int CELL_MAIN_DEVIDE = 4;
const int CELL_SUB_SIZE = 4; // 4m
const int CELL_MAIN_SIZE = CELL_MAIN_DEVIDE * CELL_SUB_SIZE;
const int MAX_CELL_MAIN = 4096 / CELL_MAIN_SIZE;
const int MAX_CELL_SUB = MAX_CELL_MAIN * CELL_MAIN_DEVIDE;

class CN3ShapeMgr
{
public:
	struct __CellSub
	{
		int 	nCCPolyCount; // Collision Check Polygon Count
		uint32*	pdwCCVertIndices; // Collision Check Polygon Vertex Indices - wCCPolyCount * 3

		void Load(FILE *fp)
		{
			if (fread(&nCCPolyCount, sizeof(int), 1, fp) != 1)
			{
				ASSERT(0);
				return;
			}

			if (nCCPolyCount != 0)
			{
				if (pdwCCVertIndices) 
					delete [] pdwCCVertIndices;

				pdwCCVertIndices = new uint32[nCCPolyCount * 3];
				if (fread(pdwCCVertIndices, nCCPolyCount * 3 * 4, 1, fp) != 1)
				{
					ASSERT(0);
					return;
				}
			}
		}

		__CellSub() { memset(this, 0, sizeof(__CellSub)); }
		~__CellSub() { delete [] pdwCCVertIndices; }
	};

	struct __CellMain
	{
		int		nShapeCount; // Shape Count;
		WORD*	pwShapeIndices; // Shape Indices
		__CellSub SubCells[CELL_MAIN_DEVIDE][CELL_MAIN_DEVIDE];

		void Load(FILE *fp)
		{
			if (fread(&nShapeCount, sizeof(int), 1, fp) != 1)
			{
				ASSERT(0);
				return;
			}

			if (nShapeCount != 0)
			{
				if (pwShapeIndices) 
					delete [] pwShapeIndices;

				pwShapeIndices = new WORD[nShapeCount];
				if (fread(pwShapeIndices, nShapeCount * 2, 1, fp) != 1)
				{
					ASSERT(0);
					return;
				}
			}

			for (int z = 0; z < CELL_MAIN_DEVIDE; z++)
			{
				for (int x = 0; x < CELL_MAIN_DEVIDE; x++)
					SubCells[x][z].Load(fp);
			}
		}

		__CellMain() { nShapeCount = 0; pwShapeIndices = nullptr; }
		~__CellMain() { delete [] pwShapeIndices; }
	};

	__Vector3* 				m_pvCollisions;

protected:
	float					m_fMapWidth;
	float					m_fMapLength;
	int					m_nCollisionFaceCount;
	__CellMain*				m_pCells[MAX_CELL_MAIN][MAX_CELL_MAIN];

public:
	void SubCell(const __Vector3& vPos, __CellSub** ppSubCell);
	__CellSub* SubCell(float fX, float fZ)
	{
		int x = (int)(fX / CELL_MAIN_SIZE);
		int z = (int)(fZ / CELL_MAIN_SIZE);

		// _ASSERT(x >= 0 && x < MAX_CELL_MAIN && z >= 0 && z < MAX_CELL_MAIN);
		if(nullptr == m_pCells[x][z]) return nullptr;

		int xx = (((int)fX)%CELL_MAIN_SIZE)/CELL_SUB_SIZE;
		int zz = (((int)fZ)%CELL_MAIN_SIZE)/CELL_SUB_SIZE;

		return &(m_pCells[x][z]->SubCells[xx][zz]);
	}
	float		Width() { return m_fMapWidth; }
	float		Height() { return m_fMapWidth; }

	bool		Create(float fMapWidth, float fMapLength);
	bool		LoadCollisionData(FILE *fp);

	void unlock();
	CN3ShapeMgr();
	virtual ~CN3ShapeMgr();
};
