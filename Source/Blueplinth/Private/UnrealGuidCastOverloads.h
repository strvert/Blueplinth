#pragma once

template <typename GuidSender>
void BreakGuid(GuidSender& Sender, const FGuid& InGuid)
{
	uint16_t Data2 = (InGuid.B >> 16) & 0xFFFF'FFFF;
	uint16_t Data3 = InGuid.B & 0xFFFF'FFFF;

	uint8_t Data4[8] = {};
	for (size_t Idx = 0; Idx < 4; Idx++)
	{
		Data4[Idx] = (InGuid.C >> (3 - Idx) * 8) & 0xFF;
	}
	for (size_t Idx = 0; Idx < 4; Idx++)
	{
		Data4[Idx + 4] = (InGuid.D >> (3 - Idx) * 8) & 0xFF;
	}

	Sender(InGuid.A, Data2, Data3, Data4);
}

template <typename GuidReceiver>
void MakeGuid(FGuid& InGuid, const GuidReceiver& Receiver)
{
	auto&& [RD1, RD2, RD3, RD4] = Receiver.Get(); // uint32_t, uint16_t, uint16_t, uint8_t[8]
	auto& [LD1, LD2, LD3, LD4] = InGuid;

	LD1 = RD1;
	LD2 = static_cast<uint32_t>(RD2) | (static_cast<uint32_t>(RD3) << 16);

	for (size_t Idx = 0; Idx < 4; Idx++)
	{
		LD3 |= static_cast<uint32_t>(RD4[Idx]) << ((3 - Idx) * 8);
	}

	for (size_t Idx = 0; Idx < 4; Idx++)
	{
		LD4 |= static_cast<uint32_t>(RD4[Idx + 4]) << ((3 - Idx) * 8);
	}
}
