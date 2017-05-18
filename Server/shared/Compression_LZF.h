#pragma once

namespace Compression
{
	namespace LZF
	{
		uint8 * CompressWithCRC32(const uint8 * in_data, uint32 in_len, uint32 * out_len, uint32 * checksum);
		uint8 * DecompressWithCRC32(const uint8 * in_data, uint32 in_len, uint32 original_len, uint32 checksum);
	}
}
