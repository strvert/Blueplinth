#pragma once

#include "ByteUtils.h"

namespace BplGuidHelper
{
	namespace Impl
	{
		struct FIntermediateGuid
		{
			uint32 Data1;
			uint16 Data2;
			uint16 Data3;
			uint8 Data4[8];
		};

		constexpr bool operator==(FIntermediateGuid const& Lhs, FIntermediateGuid const& Rhs) noexcept
		{
			return Lhs.Data1 == Rhs.Data1 &&
				Lhs.Data2 == Rhs.Data2 &&
				Lhs.Data3 == Rhs.Data3 &&
				FMemory::Memcmp(Lhs.Data4, Rhs.Data4, 8) == 0;
		}

		struct FImGuidMaker
		{
			FIntermediateGuid ImGuid;

			void operator()(const uint32 D1, const uint16 D2, const uint16 D3, const uint8* const D4)
			{
				auto& [Data1, Data2, Data3, Data4] = ImGuid;
				Data1 = D1;
				Data2 = D2;
				Data3 = D3;
				FMemory::Memcpy(Data4, D4, 8);
			}

			FIntermediateGuid Get() const { return ImGuid; };
		};

		struct FImGuidBreaker
		{
			const FIntermediateGuid& ImGuid;

			explicit FImGuidBreaker(const FIntermediateGuid& ImGuid) : ImGuid(ImGuid) {}

			const FIntermediateGuid& Get() const { return ImGuid; }
		};
	}

	template <typename ToGuidType, typename FromGuidType>
	ToGuidType GuidCast(const FromGuidType& InGuid)
	{
		Impl::FImGuidMaker Maker;
		BreakGuid(Maker, InGuid);

		ToGuidType ToGuid;
		Impl::FImGuidBreaker Breaker(Maker.Get());
		MakeGuid(ToGuid, Breaker);
		return ToGuid;
	};

	template <typename GuidType>
	SIZE_T ToString(const GuidType& InGuid, char* Result, bool Upper = true, bool WithoutHyphen = false)
	{
		Impl::FImGuidMaker Maker;
		BreakGuid(Maker, InGuid);
		auto [Data1, Data2, Data3, Data4] = Maker.Get();

		SIZE_T Head = 0;
		Head += ByteUtils::ToHexString(Data1, Result, Upper);
		if (!WithoutHyphen)
		{
			Result[Head] = u8'-';
			Head += 1;
		}

		Head += ByteUtils::ToHexString(Data2, Result + Head, Upper);
		if (!WithoutHyphen)
		{
			Result[Head] = u8'-';
			Head += 1;
		}

		Head += ByteUtils::ToHexString(Data3, Result + Head, Upper);
		if (!WithoutHyphen)
		{
			Result[Head] = u8'-';
			Head += 1;
		}

		for (SIZE_T Idx = 0; Idx < 8; Idx++)
		{
			if (!WithoutHyphen && Idx == 2)
			{
				Result[Head] = u8'-';
				Head += 1;
			}
			Head += ByteUtils::ToHexString(Data4[Idx], Result + Head, Upper);
		}

		Result[Head] = '\0';
		Head += 1;
		return Head;
	}
};

/*
対応したいGUIDの型に対して、2つのオーバーロードを実装すると↑の実装が利用可能になる。以下はwinrt::guidにおける例。

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
*/
