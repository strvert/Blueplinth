#pragma once

#include "CoreMinimal.h"

#include "BlueplinthLibrary.h"
#include "WinRT.h"
#include "DeviceMatcherBase.h"
#include "BluetoothAddress.h"
#include "BluetoothAddressDeviceMatcher.generated.h"

UCLASS(BlueprintType)
class UBluetoothAddressDeviceMatcher : public UDeviceMatcherBase
{
	GENERATED_BODY()
private:
	FBluetoothAddress BLEAddress;

public:
	explicit UBluetoothAddressDeviceMatcher(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual bool Check(const FAdvEventArgs& Args) override
	{
		return Args.BluetoothAddress() == FBluetoothAddress::ToUint64(BLEAddress);
	}

	UFUNCTION(BlueprintCallable, Category="Blueplinth|BLEDevice|Matcher", DisplayName="Create Device Matcher (by Bluetooth Address)")
	static UDeviceMatcherBase* CreateDeviceMatcherByBluetoothAddress(const FBluetoothAddress& Address)
	{
		auto NewObj = NewObject<UBluetoothAddressDeviceMatcher>();
		
		NewObj->BLEAddress = Address;
		return NewObj;
	}
};
