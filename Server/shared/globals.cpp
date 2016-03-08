#include "stdafx.h"

static std::mt19937 s_randomNumberGenerator;
static std::recursive_mutex s_rngLock;
static bool s_rngSeeded = false;

INLINE void SeedRNG()
{
	if (!s_rngSeeded)
	{
		s_randomNumberGenerator.seed((unsigned long)getMSTime());
		s_rngSeeded = true;
	}
}

int32 myrand(int32 min, int32 max)
{
	Guard lock(s_rngLock);
	SeedRNG();
	if (min > max) std::swap(min, max);
	std::uniform_int_distribution<int32> dist(min, max);
	return dist(s_randomNumberGenerator);
}

uint64 RandUInt64()
{
	Guard lock(s_rngLock);
	SeedRNG();
	std::uniform_int_distribution<uint64> dist;
	return dist(s_randomNumberGenerator);
}