#include "stdafx.h"
#include "Compression.h"

uint8 * Compression::Compress(const uint8 * in_data, uint32 in_len, uint32 * out_len)
{
	uint32 checksum;
	return CompressWithCRC32(in_data, in_len, out_len, &checksum);
}

uint8 * Compression::Decompress(const uint8 * in_data, uint32 in_len, uint32 original_len)
{
	uint32 checksum = 0;
	return DecompressWithCRC32(in_data, in_len, original_len, checksum);
}
