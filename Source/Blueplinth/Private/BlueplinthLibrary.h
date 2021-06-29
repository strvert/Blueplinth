#pragma once

#include "CoreMinimal.h"

#include "Blueplinth.h"
#include "WinRT.h"
#include "GuidHelper.h"
#include "BluetoothAddress.h"
#include "GuidOverloads.h"

#include "BlueplinthLibrary.generated.h"

using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Foundation;

UENUM(BlueprintType)
enum class ESeparatorType : uint8
{
	Hyphen,
	Colon,
};

UCLASS(BlueprintType)
class UBlueplinthLibrary final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, Category="Blueplinth")
	static void ParseStringToBluetoothAddress(const FString& AddressString, FBluetoothAddress& Address, bool& Success)
	{
		constexpr uint8 OctetCount = 6;
		constexpr uint8 OctetLenInStr = 3;

		if (AddressString.Len() != OctetCount * OctetLenInStr - 1)
		{
			Success = false;
			return;
		}

		constexpr uint8 SeparatorPos[] = {2, 5, 8, 11, 14};
		for (auto&& Pos : SeparatorPos)
		{
			if (!(AddressString[Pos] == TEXT('-') || AddressString[Pos] == TEXT(':')))
			{
				Success = false;
				return;
			}
		}

		FBluetoothAddress Ret;
		const FStringView AddrStrView(AddressString);
		for (int Idx = 0; Idx < OctetCount; Idx++)
		{
			const int Look = Idx * OctetLenInStr;
			uint8 Byte = 0;
			const auto ByteStrView = AddrStrView.Mid(Look, 2);
			for (auto&& Char : ByteStrView)
			{
				if (!CheckTCharIsHex(Char))
				{
					Success = false;
					return;
				}
			}
			if (UE::String::HexToBytes(ByteStrView, &Byte) == 1)
			{
				Ret.Address[Idx] = Byte;
			}
			else
			{
				Success = false;
				return;
			}
		}
		Success = true;
		Address = Ret;
	}

	UFUNCTION(BlueprintPure, Category="Blueplinth|ByteUtils")
	static void ByteToHexString(const uint8 Byte, FString& HexString)
	{
		ByteToHex(Byte, HexString);
	}

	UFUNCTION(BlueprintPure, Category="Blueplinth|ByteUtils")
	static void HexStringToByte(const FString& HexStr, uint8& Byte, bool& Success)
	{
		const FStringView HexStrView(HexStr);
		Success = UE::String::HexToBytes(HexStrView, &Byte) == 1;
	}

	UFUNCTION(BlueprintPure, Category="Blueplinth", DisplayName="ToString (BluetoothAddress)", meta=(AutoCreateRefTerm="SeparatorType"))
	static void BluetoothAddressToString(const FBluetoothAddress& Address, const ESeparatorType& SeparatorType, FString& AddressString)
	{
		constexpr uint8 OctetCount = 6;
		constexpr uint8 OctetLenInStr = 3;
		constexpr uint8 StrLen = OctetCount * OctetLenInStr;
		AddressString.Empty(StrLen);
		const char Separator = [&]
		{
			switch (SeparatorType)
			{
				case ESeparatorType::Hyphen:
					return '-';
				case ESeparatorType::Colon:
					return ':';
				default:
					return ':';
			}
		}();

		int Idx = 0;
		for (auto&& Part : Address.Address)
		{
			ByteToHex(Part, AddressString);
			if (Idx != OctetCount-1)
			{
				AddressString += Separator;
			}
			Idx++;
		}
	}
};
