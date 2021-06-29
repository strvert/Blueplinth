#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintAsyncActionBase.h"
#include "BLEDevice.h"
#include "Matchers/DeviceMatcherBase.h"
#include "Windows/WinRT.h"
#include "LatentGetBleDevice.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSucceededGetBLEDevice, class UBLEDevice*, BLEDevice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFailedGetBLEDevice);

UCLASS()
class ULatentGetBLEDevice final : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	explicit ULatentGetBLEDevice(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

private:
	float Timeout;
	winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher AdvWatcher;

	FTimerHandle TimeoutHandler;

	UPROPERTY()
	UDeviceMatcherBase* Matcher;
	
	UPROPERTY(BlueprintAssignable)
	FSucceededGetBLEDevice Succeeded;

	UPROPERTY(BlueprintAssignable)
	FFailedGetBLEDevice Failed;

public:
	UFUNCTION(BlueprintCallable, Category="Blueplinth", DisplayName="GetBLEDevice",
		meta=(WorldContext="WorldContextObject", BlueprintInternalUseOnly="true"))
	static ULatentGetBLEDevice* LatentGetBLEDevice(UObject* WorldContextObject,
												   UDeviceMatcherBase* DeviceMatcher,
												   const float Timeout);

	virtual void Activate() override;
};
