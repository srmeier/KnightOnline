#include "stdafx.h"
#include "Compression.h"
#include "lzf.h"
#include "crc32.h"

namespace Compression
{
	namespace LZF
	{
		uint8_t * CompressWithCRC32(const uint8_t * in_data, uint32_t in_len, uint32_t * out_len, uint32_t * checksum)
		{
			uint32_t max_out_len = in_len + LZF_MARGIN;
			uint8_t * out_data = new uint8_t[max_out_len];

			*out_len = lzf_compress(in_data, in_len, out_data, max_out_len);

			return out_data;
		}

		uint8_t * DecompressWithCRC32(const uint8_t * in_data, uint32_t in_len, uint32_t original_len, uint32_t checksum)
		{
			uint8_t * out_data = new uint8_t[original_len];
			uint32_t out_len = lzf_decompress(in_data, in_len, out_data, original_len);
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
