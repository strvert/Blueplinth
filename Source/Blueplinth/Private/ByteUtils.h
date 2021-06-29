#pragma once

namespace ByteUtils
{
	inline size_t ToHexString(const uint8 Value, char* HexStr, bool Upper = true)
	{
		constexpr uint8 Mask = 0x0F;

		const uint8 Low = (Value & Mask);
		const uint8 High = (Value & (Mask << 4)) >> 4;

		const auto Conv = [&](const uint8 V) { return V <= 9 ? '0' + V : (Upper ? 'A' : 'a') + (V - 10); };
		HexStr[0] = Conv(High);
		HexStr[1] = Conv(Low);
		return 2;
	}

	template <typename T, size_t TSize = sizeof(T)>
	size_t ToHexString(const T Value, char* HexStr, bool Upper = true)
	{
		for (size_t Idx = 0; Idx < TSize; Idx++)
		{
			ToHexString(static_cast<uint8>((Value >> Idx * 8) & 0xFF), HexStr + (TSize - Idx - 1) * 2, Upper);		}
		return TSize * 2;
	}
}
