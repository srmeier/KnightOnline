#include "stdafx.h"
#include "pkware/pklib.h"
#include "crc32.h"
#include "Compression.h"

namespace Compression
{
	namespace PKWARE
	{
		enum class BufferError
		{
			None = 0,
			OutputTooSmall,
			OutputTooSmallForBlock
		};

		struct UserContext
		{
			const uint8 * Input;
			uint32 InputSize;
			uint32 InputOffset;

			uint8 * Output;
			uint32 OutputSize;
			uint32 OutputOffset;

			TCmpStruct State;

			bool Compressing;		// true when compressing, false when decompressing
			BufferError Error;

			uint32 Checksum;	// crc32 checksum recalculated every block
		};

		static const uint32 DefaultCompressionType = CMP_BINARY;
		static const uint32 DefaultDictionarySize = 4096;

		uint32 ReadCallback(char * buffer, uint32 * bytesRemaining_, void * param)
		{
			UserContext * ctx = (UserContext *)param;
			if (ctx == nullptr)
				return 0;

			uint32 bytesRemaining = *bytesRemaining_; // we don't want to touch the original
			uint32 bytesRead = 0;

			if (ctx->InputOffset < ctx->InputSize)
			{
				uint32 bytesLeft = ctx->InputSize - ctx->InputOffset;
				if (bytesLeft < bytesRemaining)
					bytesRemaining = bytesLeft;

				memcpy(buffer, &ctx->Input[ctx->InputOffset], bytesRemaining);
				ctx->InputOffset += bytesRemaining;
				bytesRead = bytesRemaining;
			}

			if (ctx->Compressing)
			{
				uint32 old = ctx->Checksum;
				ctx->Checksum = crc32((const uint8 *)buffer, bytesRead, ctx->Checksum);
			}

			return bytesRead;
		}

		void WriteCallback(char * buffer, uint32 * size, void * param)
		{
			UserContext * ctx = (UserContext *)param;
			if (ctx == nullptr)
				return;

			if (ctx->OutputOffset >= ctx->OutputSize)
			{
				ctx->Error = BufferError::OutputTooSmall;
				return;
			}

			if ((ctx->OutputSize - ctx->OutputOffset) < *size)
			{
				ctx->Error = BufferError::OutputTooSmallForBlock;
				return;
			}

			memcpy(&ctx->Output[ctx->OutputOffset], buffer, *size);
			ctx->OutputOffset += *size;

			if (!ctx->Compressing)
				ctx->Checksum = crc32((const uint8 *)buffer, *size, ctx->Checksum);
		}

		uint8_t * CompressWithCRC32(const uint8 * in_data, uint32 in_len, uint32 * out_len, uint32 * checksum)
		{
			if (in_len == 0)
				return nullptr;

			uint32 max_out_len = in_len;
			if (max_out_len < 1024)
				max_out_len *= 2;

			uint8 * out_data = new uint8[max_out_len];
			UserContext ctx = { 0 };

			ctx.Input = in_data;
			ctx.InputSize = in_len;
			ctx.Output = out_data;
			ctx.OutputSize = max_out_len;

			ctx.Compressing = true;
			ctx.Checksum = ~((uint32)0);

			uint32 compressionType = DefaultCompressionType;
			uint32 dictionarySize = DefaultDictionarySize;

			int r = implode(ReadCallback, WriteCallback, (char *)&ctx.State, &ctx, &compressionType, &dictionarySize);
			if (r != 0
				|| ctx.Error != BufferError::None)
			{
				delete[] out_data;
				return nullptr;
			}

			*out_len = ctx.OutputOffset;
			*checksum = ctx.Checksum;

			return out_data;
		}

		uint8 * DecompressWithCRC32(const uint8 * in_data, uint32 in_len, uint32 original_len, uint32 checksum)
		{
			if (original_len == 0)
				return nullptr;

			uint8 * out_data = new uint8[original_len];
			UserContext ctx = { 0 };

			ctx.Input = in_data;
			ctx.InputSize = in_len;
			ctx.Output = out_data;
			ctx.OutputSize = original_len;

			ctx.Compressing = false;
			ctx.Checksum = ~((uint32)0);

			int r = explode(ReadCallback, WriteCallback, (char *)&ctx.State, &ctx);
			if (r != 0
				|| ctx.Error != BufferError::None)
			{
				delete[] out_data;
				return nullptr;
			}

			if (checksum != 0
				&& ctx.Checksum != checksum)
			{
				delete[] out_data;
				return nullptr;
			}

			return out_data;
		}
	}
}
