#include "stdafx.h"
#include "Compression.h"
#include "lzf.h"
#include "crc32.h"

namespace Compression
{
	namespace LZF
	{
		uint8 * CompressWithCRC32(const uint8 * in_data, uint32 in_len, uint32 * out_len, uint32 * checksum)
		{
			uint32 max_out_len = in_len + LZF_MARGIN;
			uint8 * out_data = new uint8[max_out_len];

			*out_len = lzf_compress(in_data, in_len, out_data, max_out_len);

			return out_data;
		}

		uint8 * DecompressWithCRC32(const uint8 * in_data, uint32 in_len, uint32 original_len, uint32 checksum)
		{
			uint8 * out_data = new uint8[original_len];
			uint32 out_len = lzf_decompress(in_data, in_len, out_data, original_len);
			if (out_len != original_len)
			{
				delete[] out_data;
				return nullptr;
			}

			if (checksum != 0
				&& crc32(out_data, out_len) != checksum)
			{
				delete[] out_data;
				return nullptr;
			}

			return out_data;
		}
	}
}
