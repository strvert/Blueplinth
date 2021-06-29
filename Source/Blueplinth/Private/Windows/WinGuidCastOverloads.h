#pragma once
#include "Windows/WinRT.h"

template <typename GuidSender>
void BreakGuid(GuidSender& Sender, const winrt::guid& InGuid)
{
	Sender(InGuid.Data1, InGuid.Data2, InGuid.Data3, InGuid.Data4);
}

template <typename GuidReceiver>
void MakeGuid(winrt::guid& InGuid, const GuidReceiver& Receiver)
{
	auto&& [RD1, RD2, RD3, RD4] = Receiver.Get(); // uint32_t, uint16_t, uint16_t, uint8_t[8]
	auto& [LD1, LD2, LD3, LD4] = InGuid;

	LD1 = RD1;
	LD2 = RD2;
	LD3 = RD3;
	for (size_t Idx = 0; Idx < 8; Idx++)
	{
		LD4[Idx] = RD4[Idx];
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
	auto&& [RD1, RD2, RD3, RD4] = Receiver.Get(); // uint32_t, uint16_t, uint16_t, uint8_t[8]
	auto& [LD1, LD2, LD3, LD4] = InGuid;

	LD1 = RD1;
	LD2 = RD2;
	LD3 = RD3;
	for (size_t Idx = 0; Idx < 8; Idx++)
	{
		LD4[Idx] = RD4[Idx];
	}
}