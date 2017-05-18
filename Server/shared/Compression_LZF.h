#pragma once

namespace Compression
{
	namespace LZF
	{
		uint8_t * CompressWithCRC32(const uint8_t * in_data, uint32_t in_len, uint32_t * out_len, uint32_t * checksum);
		uint8_t * DecompressWithCRC32(const uint8_t * in_data, uint32_t in_len, uint32_t original_len, uint32_t checksum);
	}
}
