#pragma once

#include "CoreMinimal.h"

#include "BluetoothAddress.h"
#include "WinRT.h"
#include "BluetoothConnectionStatus.h"
#include "BLEDevice.generated.h"

UCLASS(BlueprintType)
class UBLEDevice final : public UObject
{
	GENERATED_BODY()

private:
	using FWinRTBLEDevice = winrt::Windows::Devices::Bluetooth::BluetoothLEDevice;
	FWinRTBLEDevice WinRTBLEDevice = FWinRTBLEDevice(nullptr);

	friend class ULatentGetBLEDevice;

public:
	explicit UBLEDevice(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){}

	UFUNCTION(BlueprintCallable, Category="Blueplinth|BLEDEvice")
	void GetBluetoothAddress(FBluetoothAddress& Address) const;
	
	UFUNCTION(BlueprintCallable, Category="Blueplinth|BLEDEvice")
	void GetDeviceId(FString& DeviceId) const;
	
	UFUNCTION(BlueprintCallable, Category="Blueplinth|BLEDEvice")
	void GetConnectionStatus(EBluetoothConnectionStatus& Status) const;
};
