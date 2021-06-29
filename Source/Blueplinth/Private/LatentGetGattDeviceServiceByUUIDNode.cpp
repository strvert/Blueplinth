#include "LatentGetGattDeviceServiceByUUIDNode.h"

#include "GuidHelper.h"
#include "GattDeviceService.h"
#include "Windows/WinRT.h"
#include "PlatformGuidCastOverloads.h"

ULatentGetGattDeviceServiceByUuid::ULatentGetGattDeviceServiceByUuid(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

ULatentGetGattDeviceServiceByUuid* ULatentGetGattDeviceServiceByUuid::LatentGetGattDeviceServiceByUuid(
	UObject* WorldContextObject, const FGuid& InServiceGuid, const float InTimeout)
{
	ULatentGetGattDeviceServiceByUuid* Node = NewObject<ULatentGetGattDeviceServiceByUuid>();
	Node->RegisterWithGameInstance(WorldContextObject);
	Node->ServiceUuid = BplGuidHelper::GuidCast<winrt::guid>(InServiceGuid);
	Node->Timeout = InTimeout;
	return Node;
}

void ULatentGetGattDeviceServiceByUuid::Activate()
{
	using namespace winrt::Windows::Devices;
	using namespace winrt::Windows::Foundation;
	using namespace Bluetooth::Advertisement;


	AdvWatcher = BluetoothLEAdvertisementWatcher();
	AdvWatcher.Received([this](const BluetoothLEAdvertisementWatcher& Sender,
	                           const BluetoothLEAdvertisementReceivedEventArgs& Args)
	{
		for (auto&& Uuid : Args.Advertisement().ServiceUuids())
		{
			if (ServiceUuid == Uuid)
			{
				Sender.Stop();
				const auto BleDeviceOp = Bluetooth::BluetoothLEDevice::FromBluetoothAddressAsync(
					Args.BluetoothAddress());
				BleDeviceOp.Completed([this](const IAsyncOperation<Bluetooth::BluetoothLEDevice>& BleDeviceSender,
				                             AsyncStatus)
				{
					if (const auto BleDevice = BleDeviceSender.get(); BleDevice)
					{
						const auto DeviceService = BleDevice.GetGattService(ServiceUuid);
						auto DeviceServiceObject = NewObject<UGattDeviceService>();
						DeviceServiceObject->DeviceService = DeviceService;
						DeviceServiceObject->DeviceName = FString(BleDevice.Name().c_str());
						DeviceServiceObject->BluetoothAddress = FBluetoothAddress::FromUint64(
							BleDevice.BluetoothAddress());
						Completed.Broadcast(DeviceServiceObject);
					}
					else
					{
						Failed.Broadcast();
					}
					SetReadyToDestroy();
				});
				break;
			}
		}
	});
	AdvWatcher.Start();

	if (const auto Deinit = [this]
	{
		if (const auto Status = AdvWatcher.Status(); Status != BluetoothLEAdvertisementWatcherStatus::Stopped)
		{
			AdvWatcher.Stop();
			Failed.Broadcast();
			SetReadyToDestroy();
			GWorld->GetTimerManager().ClearTimer(TimeoutHandler);
		}
	}; Timeout <= 0)
	{
		Deinit();
	}
	else
	{
		GWorld->GetTimerManager().SetTimer(TimeoutHandler, Deinit, Timeout, false);
	}
}
