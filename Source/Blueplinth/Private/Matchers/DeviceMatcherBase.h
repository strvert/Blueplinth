#pragma once

#include "CoreMinimal.h"
#include "Windows/WinRT.h"
#include "DeviceMatcherBase.generated.h"

UCLASS(Abstract)
class UDeviceMatcherBase : public UObject
{
	GENERATED_BODY()
protected:
	using FAdvEventArgs = winrt::Windows::Devices::Bluetooth::Advertisement::BluetoothLEAdvertisementReceivedEventArgs;

public:	
	virtual bool Check(const FAdvEventArgs&) { return false; }
};

// struct FUuidMatcher final : FDeviceMatcherBase
// {
// 	explicit FUuidMatcher(const winrt::guid& InUuid) : ServiceUuid(InUuid) {}
// 
// 	winrt::guid ServiceUuid;
// 	virtual bool Check(const FAdvEventArgs& Args) override;
// };
// 
// struct FBluetoothAddressMatcher final : FDeviceMatcherBase
// {
// 	explicit FBluetoothAddressMatcher(const FBluetoothAddress& InAddress) : BluetoothAddress(InAddress) {}
// 
// 	FBluetoothAddress BluetoothAddress;
// 	virtual bool Check(const FAdvEventArgs& Args) override;
// };
// 