#pragma once

#include "ReferenceObject.h"

class CUser;

typedef CSTLMap <_OBJECT_EVENT>		ObjectEventArray;
typedef CSTLMap <_REGENE_EVENT>		ObjectRegeneArray;
typedef	CSTLMap <_WARP_INFO>		WarpArray;

class CN3ShapeMgr;
class SMDFile : public ReferenceObject
{
public:
	SMDFile();

	static SMDFile *Load(std::string mapName, bool bLoadWarpsAndRegeneEvents = false /* AI server doesn't need them */);

	void OnInvalidMap();
	bool LoadMap(FILE *fp, std::string & mapName, bool bLoadWarpsAndRegeneEvents /* AI server doesn't need them */);
	void LoadTerrain(FILE *fp);
	void LoadObjectEvent(FILE *fp);
	void LoadMapTile(FILE *fp);
	void LoadRegeneEvent(FILE *fp);
	void LoadWarpList(FILE *fp);

	bool IsValidPosition(float x, float z, float y);
	bool CheckEvent( float x, float z, CUser* pUser = nullptr );

	int GetEventID(int x, int z);

	INLINE int GetMapSize() { return m_nMapSize - 1; }
	INLINE float GetUnitDistance() { return m_fUnitDist; }
	INLINE int GetXRegionMax() { return m_nXRegion - 1; }
	INLINE int GetZRegionMax() { return m_nZRegion - 1; }

	INLINE short * GetEventIDs() { return m_ppnEvent; }

	INLINE ObjectEventArray	* GetObjectEventArray() { return &m_ObjectEventArray; }
	INLINE _OBJECT_EVENT * GetObjectEvent(int objectindex) { return m_ObjectEventArray.GetData(objectindex); }
	INLINE _REGENE_EVENT * GetRegeneEvent(int objectindex) { return m_ObjectRegeneArray.GetData(objectindex); }
	INLINE _WARP_INFO * GetWarp(int warpID) { return m_WarpArray.GetData(warpID); }

	void GetWarpList(int warpGroup, std::set<_WARP_INFO *> & warpEntries);

	virtual ~SMDFile();

private:
	std::string m_MapName;

	short*		m_ppnEvent;
	WarpArray	m_WarpArray;

	ObjectEventArray	m_ObjectEventArray;
	ObjectRegeneArray	m_ObjectRegeneArray;

	CN3ShapeMgr *m_N3ShapeMgr;

	float*		m_fHeight;

	int			m_nXRegion, m_nZRegion;

	int			m_nMapSize;		// Grid Unit ex) 4m
	float		m_fUnitDist;	// i Grid Distance

	typedef std::map<std::string, SMDFile *> SMDMap;
	static SMDMap s_loadedMaps;

#if defined(GAMESERVER)
	friend class C3DMap;
#elif defined(AI_SERVER)
	friend class MAP;
#endif
};
