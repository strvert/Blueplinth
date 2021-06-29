#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "GattDeviceService.h"
#include "LatentGetGattDeviceServiceByUUIDNode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAsyncFoundGetGattDeviceService, class UGattDeviceService*, DeviceService);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAsyncNotFoundGetGattDeviceService);

UCLASS()
class ULatentGetGattDeviceServiceByUuid final : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

private:
	UPROPERTY(BlueprintAssignable)
	FAsyncFoundGetGattDeviceService Completed;
	
	UPROPERTY(BlueprintAssignable)
	FAsyncNotFoundGetGattDeviceService Failed;

	winrt::guid ServiceUuid;
	float Timeout;
	FTimerHandle TimeoutHandler;

	winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher AdvWatcher;

public:
	explicit ULatentGetGattDeviceServiceByUuid(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category="Blueplinth", DisplayName="GetGattDeviceServiceByUuid",
		meta=(WorldContext="WorldContextObject", BlueprintInternalUseOnly="true"))
	static ULatentGetGattDeviceServiceByUuid* LatentGetGattDeviceServiceByUuid(
		UObject* WorldContextObject, const FGuid& InServiceGuid, const float InTimeout);

	virtual void Activate() override;
};
