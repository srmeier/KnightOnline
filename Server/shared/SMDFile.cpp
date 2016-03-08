#include "stdafx.h"
#include "../N3BASE/N3ShapeMgr.h"
#include "STLMap.h"
#include "database/structs.h"
#include <set>
#include "SMDFile.h"

SMDFile::SMDMap SMDFile::s_loadedMaps;

SMDFile::SMDFile() : m_ppnEvent(nullptr), m_fHeight(nullptr),
	m_nXRegion(0), m_nZRegion(0), m_nMapSize(0), m_fUnitDist(0.0f),
	m_N3ShapeMgr(new CN3ShapeMgr())
{
}

SMDFile *SMDFile::Load(std::string mapName, bool bLoadWarpsAndRegeneEvents /*= false*/)
{
	// case insensitive filenames, allowing for database inconsistency...
	STRTOLOWER(mapName);

	// Look to see if that SMD file has been loaded already
	SMDMap::iterator itr = s_loadedMaps.find(mapName);

	// If it's been loaded already, we don't need to do anything.
	if (itr != s_loadedMaps.end())
	{
		// Add another reference.
		itr->second->IncRef();
		return itr->second;
	}

	// Map hasn't already been loaded
	std::string filename = string_format(MAP_DIR "%s", mapName.c_str());

	// Does this file exist/can it be opened?
	FILE *fp = fopen(filename.c_str(), "rb");
	if (fp == nullptr)
	{
		printf("ERROR: %s does not exist or no permission to access.\n", filename.c_str());
		return nullptr;
	}

	// Try to load the file now.
	SMDFile *smd = new SMDFile();
	if (!smd->LoadMap(fp, mapName, bLoadWarpsAndRegeneEvents))
	{
		// Problem? Make sure we clean up after ourselves.
		smd->DecRef(); // it's the only reference anyway
		smd = nullptr;
	}
	else
	{
		// Loaded fine, so now add it to the map.
		s_loadedMaps.insert(std::make_pair(mapName, smd));
	}

	fclose(fp);
	return smd;
}

void SMDFile::OnInvalidMap()
{
	printf("\n ** An error has occurred **\n\n");
	printf("ERROR: %s is not a valid map file.\n\n", m_MapName.c_str());
	printf("Previously, we ignored all invalid map behaviour, however this only hides\n");
	printf("very real problems - especially with things like AI pathfinding.\n\n");
	printf("This problem is most likely occur with maps tweaked to use a different\n");
	printf("map size. Unfortunately, doing this means data after that (almost everything)\n");
	printf("becomes corrupt, which is known to cause extremely 'unusual' buggy behaviour.\n\n");
	printf("It is recommended you use a map built for this zone, or at the very least,\n");
	printf("you should use a map originally built for the same zone size.\n\n");
	ASSERT(0);
}

bool SMDFile::LoadMap(FILE *fp, std::string & mapName, bool bLoadWarpsAndRegeneEvents)
{
	m_MapName = mapName;

	LoadTerrain(fp);

	m_N3ShapeMgr->Create((m_nMapSize - 1)*m_fUnitDist, (m_nMapSize-1)*m_fUnitDist);
	if (!m_N3ShapeMgr->LoadCollisionData(fp)
		|| (m_nMapSize - 1) * m_fUnitDist != m_N3ShapeMgr->Width() 
		|| (m_nMapSize - 1) * m_fUnitDist != m_N3ShapeMgr->Height())
		return false;

	int mapwidth = (int)m_N3ShapeMgr->Width();

	m_nXRegion = (int)(mapwidth / VIEW_DISTANCE) + 1;
	m_nZRegion = (int)(mapwidth / VIEW_DISTANCE) + 1;

	LoadObjectEvent(fp);
	LoadMapTile(fp);

	if (bLoadWarpsAndRegeneEvents)
	{
		LoadRegeneEvent(fp);
		LoadWarpList(fp);
	}

	return true;
}

void SMDFile::LoadTerrain(FILE *fp)
{
	if (fread(&m_nMapSize, sizeof(m_nMapSize), 1, fp) != 1
		|| fread(&m_fUnitDist, sizeof(m_fUnitDist), 1, fp) != 1)
		return OnInvalidMap();

	m_fHeight = new float[m_nMapSize * m_nMapSize];
	if (fread(m_fHeight, sizeof(float) * m_nMapSize * m_nMapSize, 1, fp) != 1)
		OnInvalidMap();
}

void SMDFile::LoadObjectEvent(FILE *fp)
{
	int iEventObjectCount = 0;
	if (fread(&iEventObjectCount, sizeof(int), 1, fp) != 1)
		return OnInvalidMap();

	// Load on K_OBJECTPOS table, this is set fd to last pointer...
	for (int i = 0; i < iEventObjectCount; i++)
	{
		if (fread((new _OBJECT_EVENT), 24, 1, fp) != 1)
			return OnInvalidMap();
	}
}

void SMDFile::LoadMapTile(FILE *fp)
{
	m_ppnEvent = new short[m_nMapSize * m_nMapSize];
	if (fread(m_ppnEvent, sizeof(short) * m_nMapSize * m_nMapSize, 1, fp) != 1)
		return OnInvalidMap();
}

void SMDFile::LoadRegeneEvent(FILE *fp)	
{
	int iEventObjectCount = 0;
	if (fread(&iEventObjectCount, sizeof(iEventObjectCount), 1, fp) != 1)
		return OnInvalidMap();

	for (int i = 0; i < iEventObjectCount; i++)
	{
		_REGENE_EVENT *pEvent = new _REGENE_EVENT;
		if (fread(pEvent, sizeof(_REGENE_EVENT) - sizeof(pEvent->sRegenePoint), 1, fp) != 1)
			return OnInvalidMap();

		pEvent->sRegenePoint = i;

		if (pEvent->sRegenePoint < 0
			|| !m_ObjectRegeneArray.PutData(pEvent->sRegenePoint, pEvent))
			delete pEvent;
	}
}

void SMDFile::LoadWarpList(FILE *fp)
{
	int WarpCount = 0;

	if (fread(&WarpCount, sizeof(WarpCount), 1, fp) != 1)
		return OnInvalidMap();

	for (int i = 0; i < WarpCount; i++)
	{
		_WARP_INFO *pWarp = new _WARP_INFO;
		if (fread(pWarp, sizeof(_WARP_INFO), 1, fp) != 1)
		{
			// NOTE: Some SMDs are so horribly broken warps are incomplete.
			// This will stop this (reasonably) normal use case from behaving any differently.
			if (feof(fp))
				return;

			return OnInvalidMap();
		}

		if (pWarp->sWarpID == 0
			|| !m_WarpArray.PutData(pWarp->sWarpID, pWarp))
			delete pWarp;
	}
}

void SMDFile::GetWarpList(int warpGroup, std::set<_WARP_INFO *> & warpEntries)
{
	foreach_stlmap (itr, m_WarpArray)
	{
		_WARP_INFO *pWarp = itr->second;
		if (pWarp == nullptr || (pWarp->sWarpID / 10) != warpGroup)
			continue;

		warpEntries.insert(pWarp);
	}
}

bool SMDFile::IsValidPosition(float x, float z, float y)
{
	// TODO: Implement more thorough check
	return (x < m_N3ShapeMgr->Width() && z < m_N3ShapeMgr->Height());
}

int SMDFile::GetEventID(int x, int z)
{
	if (x < 0 || x >= m_nMapSize || z < 0 || z >= m_nMapSize)
		return -1;

	return m_ppnEvent[x * m_nMapSize + z];
}

SMDFile::~SMDFile()
{
	if (m_ppnEvent != nullptr)
	{
		delete [] m_ppnEvent;
		m_ppnEvent = nullptr;
	}

	if (m_fHeight != nullptr)
	{
		delete[] m_fHeight;
		m_fHeight = nullptr;
	}

	delete m_N3ShapeMgr;
}