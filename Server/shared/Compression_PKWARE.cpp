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
			const uint8_t * Input;
			uint32_t InputSize;
			uint32_t InputOffset;

			uint8_t * Output;
			uint32_t OutputSize;
			uint32_t OutputOffset;

			TCmpStruct State;

			bool Compressing;		// true when compressing, false when decompressing
			BufferError Error;

			uint32_t Checksum;	// crc32 checksum recalculated every block
		};

		static const uint32_t DefaultCompressionType = CMP_BINARY;
		static const uint32_t DefaultDictionarySize = 4096;

		uint32_t ReadCallback(char * buffer, uint32_t * bytesRemaining_, void * param)
		{
			UserContext * ctx = (UserContext *)param;
			if (ctx == nullptr)
				return 0;

			uint32_t bytesRemaining = *bytesRemaining_; // we don't want to touch the original
			uint32_t bytesRead = 0;

			if (ctx->InputOffset < ctx->InputSize)
			{
				uint32_t bytesLeft = ctx->InputSize - ctx->InputOffset;
				if (bytesLeft < bytesRemaining)
					bytesRemaining = bytesLeft;

				memcpy(buffer, &ctx->Input[ctx->InputOffset], bytesRemaining);
				ctx->InputOffset += bytesRemaining;
				bytesRead = bytesRemaining;
			}

			if (ctx->Compressing)
			{
				uint32_t old = ctx->Checksum;
				ctx->Checksum = crc32((const uint8_t *)buffer, bytesRead, ctx->Checksum);
			}

			return bytesRead;
		}

		void WriteCallback(char * buffer, uint32_t * size, void * param)
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
				ctx->Checksum = crc32((const uint8_t *)buffer, *size, ctx->Checksum);
		}

		uint8_t * CompressWithCRC32(const uint8_t * in_data, uint32_t in_len, uint32_t * out_len, uint32_t * checksum)
		{
			if (in_len == 0)
				return nullptr;

			uint32_t max_out_len = in_len;
			if (max_out_len < 1024)
				max_out_len *= 2;

			uint8_t * out_data = new uint8_t[max_out_len];
			UserContext ctx = { 0 };

			ctx.Input = in_data;
			ctx.InputSize = in_len;
			ctx.Output = out_data;
			ctx.OutputSize = max_out_len;

			ctx.Compressing = true;
			ctx.Checksum = ~((uint32_t)0);

			uint32_t compressionType = DefaultCompressionType;
			uint32_t dictionarySize = DefaultDictionarySize;

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

		uint8_t * DecompressWithCRC32(const uint8_t * in_data, uint32_t in_len, uint32_t original_len, uint32_t checksum)
		{
			if (original_len == 0)
				return nullptr;

			uint8_t * out_data = new uint8_t[original_len];
			UserContext ctx = { 0 };

			ctx.Input = in_data;
			ctx.InputSize = in_len;
			ctx.Output = out_data;
			ctx.OutputSize = original_len;

			ctx.Compressing = false;
			ctx.Checksum = ~((uint32_t)0);

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
