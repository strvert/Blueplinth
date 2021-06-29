#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintAsyncActionBase.h"
#include "BLEDevice.h"
#include "BluetoothAddress.h"
#include "WinRT.h"
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
	using FAdvEventArgs = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;

	struct FMatcherBase
	{
		virtual bool Check(const FAdvEventArgs&) = 0;
		virtual ~FMatcherBase() {};
	};

	struct FUuidMatcher final : FMatcherBase
	{
		explicit FUuidMatcher(const winrt::guid& InUuid) : ServiceUuid(InUuid) {}

		winrt::guid ServiceUuid;
		virtual bool Check(const FAdvEventArgs& Args) override;
	};

	struct FBluetoothAddressMatcher final : FMatcherBase
	{
		explicit FBluetoothAddressMatcher(const FBluetoothAddress& InAddress) : BluetoothAddress(InAddress) {}

		FBluetoothAddress BluetoothAddress;
		virtual bool Check(const FAdvEventArgs& Args) override;
	};
	
	float Timeout;
	winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementWatcher AdvWatcher;

	FTimerHandle TimeoutHandler;
	TUniquePtr<FMatcherBase> Matcher;
	
	UPROPERTY(BlueprintAssignable)
	FSucceededGetBLEDevice Succeeded;

	UPROPERTY(BlueprintAssignable)
	FFailedGetBLEDevice Failed;

public:
	static ULatentGetBLEDevice* ConstructNode(UObject* InWorldContextObject, const float InTimeout,
	                                          TUniquePtr<FMatcherBase> InMatcher)
	{
		ULatentGetBLEDevice* Node = NewObject<ULatentGetBLEDevice>();
		Node->RegisterWithGameInstance(InWorldContextObject);
		Node->Timeout = InTimeout;
		Node->Matcher = MoveTemp(InMatcher);
		return Node;
	}

	UFUNCTION(BlueprintCallable, Category="Blueplinth", DisplayName="GetBLEDeviceByServiceUuid",
		meta=(WorldContext="WorldContextObject", BlueprintInternalUseOnly="true"))
	static ULatentGetBLEDevice* LatentGetBLEDeviceByServiceUuid(UObject* WorldContextObject, const FGuid& InServiceUuid,
	                                                            const float InTimeout);

	UFUNCTION(BlueprintCallable, Category="Blueplinth", DisplayName="GetBLEDeviceByBluetoothAddress",
		meta=(WorldContext="WorldContextObject", BlueprintInternalUseOnly="true"))
	static ULatentGetBLEDevice* LatentGetBLEDeviceByBluetoothAddress(UObject* WorldContextObject,
	                                                                 const FBluetoothAddress& InBluetoothAddress,
	                                                                 const float InTimeout);

	virtual void Activate() override;
};
