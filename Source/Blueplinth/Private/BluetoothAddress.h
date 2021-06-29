#pragma once

#include "BluetoothAddress.generated.h"

USTRUCT(BlueprintType)
struct FBluetoothAddress
{
	GENERATED_BODY()
	FBluetoothAddress()
	{
		Address.SetNum(6);
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Blueplinth|BluetoothAddress")
	TArray<uint8> Address;


	static FBluetoothAddress FromUint64(const uint64 Data) 
	{
		FBluetoothAddress New;
		uint64 WorkingData = Data;
		if (FGenericPlatformProperties::IsLittleEndian())
		{
			WorkingData = ByteSwap(WorkingData);
		}
		WorkingData >>= 16;
		FMemory::Memcpy(New.Address.GetData(), &WorkingData, 6);
		return New;
	}

	static uint64 ToUint64(const FBluetoothAddress& Address)
	{
		uint64 New;
		FMemory::Memcpy(&New, Address.Address.GetData(), 6);
		New <<= 16;
		if (FGenericPlatformProperties::IsLittleEndian())
		{
			New = ByteSwap(New);	
		}
		return New;
	}
};
