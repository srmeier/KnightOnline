#include "stdafx.h"

static std::mt19937 s_randomNumberGenerator;
static std::recursive_mutex s_rngLock;
static bool s_rngSeeded = false;

INLINE void SeedRNG()
{
	if (!s_rngSeeded)
	{
		s_randomNumberGenerator.seed((uint32_t)getMSTime());
		s_rngSeeded = true;
	}
}

int32_t myrand(int32_t min, int32_t max)
{
	Guard lock(s_rngLock);
	SeedRNG();
	if (min > max) std::swap(min, max);
	std::uniform_int_distribution<int32_t> dist(min, max);
	return dist(s_randomNumberGenerator);
}

uint64_t RandUInt64()
{
	Guard lock(s_rngLock);
	SeedRNG();
	std::uniform_int_distribution<uint64_t> dist;
	return dist(s_randomNumberGenerator);
}