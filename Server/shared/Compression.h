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

	uint8 * Compress(const uint8 * in_data, uint32 in_len, uint32 * out_len);
	uint8 * Decompress(const uint8 * in_data, uint32 in_len, uint32 original_len);
}
