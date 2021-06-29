#pragma once

#include "WinRT.h"
#include "Misc/Guid.h"

template <typename GuidSender>
void BreakGuid(GuidSender& Sender, const winrt::guid& InGuid)
{
	Sender(InGuid.Data1, InGuid.Data2, InGuid.Data3, InGuid.Data4);
}

template <typename GuidReceiver>
void MakeGuid(winrt::guid& InGuid, const GuidReceiver& Receiver)
{
	auto&& [D1, D2, D3, D4] = Receiver.Get(); // uint32_t, uint16_t, uint16_t, uint8_t[8]

	InGuid.Data1 = D1;
	InGuid.Data2 = D2;
	InGuid.Data3 = D3;
	for (size_t Idx = 0; Idx < 8; Idx++)
	{
		InGuid.Data4[Idx] = D4[Idx];
	}
}

template <typename GuidSender>
void BreakGuid(GuidSender& Sender, const GUID& InGuid)
{
	Sender(InGuid.Data1, InGuid.Data2, InGuid.Data3, InGuid.Data4);
}

template <typename GuidReceiver>
void MakeGuid(GUID& InGuid, const GuidReceiver& Receiver)
{
	auto&& [D1, D2, D3, D4] = Receiver.Get(); // uint32_t, uint16_t, uint16_t, uint8_t[8]

	InGuid.Data1 = D1;
	InGuid.Data2 = D2;
	InGuid.Data3 = D3;
	for (size_t Idx = 0; Idx < 8; Idx++)
	{
		InGuid.Data4[Idx] = D4[Idx];
	}
}

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
	auto&& [D1, D2, D3, D4] = Receiver.Get(); // uint32_t, uint16_t, uint16_t, uint8_t[8]

	InGuid.A = D1;
	InGuid.B = static_cast<uint32_t>(D2) | (static_cast<uint32_t>(D3) << 16);

	for (size_t Idx = 0; Idx < 4; Idx++)
	{
		InGuid.C |= static_cast<uint32_t>(D4[Idx]) << ((3 - Idx) * 8);
	}

	for (size_t Idx = 0; Idx < 4; Idx++)
	{
		InGuid.D |= static_cast<uint32_t>(D4[Idx + 4]) << ((3 - Idx) * 8);
	}
}
