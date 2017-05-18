#pragma once

#include "Compression_LZF.h"
#include "Compression_PKWARE.h"

namespace Compression
{
	static const size_t MinBytes = 500;
	static const size_t DefaultBufferSize = 16384;
	static const size_t DefaultGrowthSize = 8192;

#if __VERSION <= 1264
	using PKWARE::CompressWithCRC32;
	using PKWARE::DecompressWithCRC32;
#else
	using LZF::CompressWithCRC32;
	using LZF::DecompressWithCRC32;
#endif

	uint8_t * Compress(const uint8_t * in_data, uint32_t in_len, uint32_t * out_len);
	uint8_t * Decompress(const uint8_t * in_data, uint32_t in_len, uint32_t original_len);
}
