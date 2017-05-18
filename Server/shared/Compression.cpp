#include "stdafx.h"
#include "Compression.h"

uint8_t * Compression::Compress(const uint8_t * in_data, uint32_t in_len, uint32_t * out_len)
{
	uint32_t checksum;
	return CompressWithCRC32(in_data, in_len, out_len, &checksum);
}

uint8_t * Compression::Decompress(const uint8_t * in_data, uint32_t in_len, uint32_t original_len)
{
	uint32_t checksum = 0;
	return DecompressWithCRC32(in_data, in_len, original_len, checksum);
}
