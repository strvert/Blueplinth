#pragma once

#include "CoreMinimal.h"

#include "BluetoothAddress.h"
#include "WinRT.h"
#include "DeviceMatcherBase.h"
#include "GuidHelper.h"
#include "GuidOverloads.h"
#include "UuidDeviceMatcher.generated.h"

UCLASS(BlueprintType)
class UUuidDeviceMatcher : public UDeviceMatcherBase
{
	GENERATED_BODY()
private:
	winrt::guid ServiceUuid;

public:
	explicit UUuidDeviceMatcher(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}

	virtual bool Check(const FAdvEventArgs& Args) override
	{
		for (auto&& Uuid : Args.Advertisement().ServiceUuids())
		{
			if (ServiceUuid == Uuid)
			{
				return true;
			}
		}
		return false;
	}

	UFUNCTION(BlueprintCallable, Category="Blueplinth|BLEDevice|Matcher", DisplayName="Create Device Matcher (by Uuid)")
	static UDeviceMatcherBase* CreateDeviceMatcherByUuid(const FGuid& Uuid)
	{
		auto NewObj = NewObject<UUuidDeviceMatcher>();
		NewObj->ServiceUuid = BplGuidHelper::GuidCast<winrt::guid>(Uuid);
		return NewObj;
	}
};
