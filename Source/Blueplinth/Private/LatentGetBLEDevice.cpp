#include "LatentGetBLEDevice.h"
#include "GuidHelper.h"
#include "GuidOverloads.h"
#include "WinRT.h"

using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Foundation;
using namespace Bluetooth::Advertisement;

bool ULatentGetBLEDevice::FUuidMatcher::Check(const FAdvEventArgs& Args)
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

bool ULatentGetBLEDevice::FBluetoothAddressMatcher::Check(const FAdvEventArgs& Args)
{
	return Args.BluetoothAddress() == FBluetoothAddress::ToUint64(BluetoothAddress);
}

ULatentGetBLEDevice* ULatentGetBLEDevice::LatentGetBLEDeviceByServiceUuid(UObject* WorldContextObject,
                                                                          const FGuid& InServiceUuid,
                                                                          const float InTimeout)
{
	return ConstructNode(WorldContextObject, InTimeout,
	                     MakeUnique<FUuidMatcher>(BPLGuidHelper::GuidCast<winrt::guid>(InServiceUuid)));
}

ULatentGetBLEDevice* ULatentGetBLEDevice::LatentGetBLEDeviceByBluetoothAddress(UObject* WorldContextObject,
                                                                               const FBluetoothAddress&
                                                                               InBluetoothAddress,
                                                                               const float InTimeout)
{
	return ConstructNode(WorldContextObject, InTimeout,
	                     MakeUnique<FBluetoothAddressMatcher>(InBluetoothAddress));
}

void ULatentGetBLEDevice::Activate()
{
	AdvWatcher = BluetoothLEAdvertisementWatcher();
	AdvWatcher.Received([this](const BluetoothLEAdvertisementWatcher& Sender,
	                           const BluetoothLEAdvertisementReceivedEventArgs& Args)
	{
		if (Matcher->Check(Args))
		{
			Sender.Stop();
			const auto BLEDeviceOp = Bluetooth::BluetoothLEDevice::FromBluetoothAddressAsync(
				Args.BluetoothAddress());
			BLEDeviceOp.Completed([this](const IAsyncOperation<Bluetooth::BluetoothLEDevice>& BleDeviceSender,
			                             AsyncStatus)
			{
				if (const auto BLEDevice = BleDeviceSender.get(); BLEDevice)
				{
					const auto BLEDeviceObj = NewObject<UBLEDevice>();
					Succeeded.Broadcast(BLEDeviceObj);
				}
				else
				{
					Failed.Broadcast();
				}
				SetReadyToDestroy();
			});
		}
	});
	AdvWatcher.Start();

	if (const auto DeinitProc = [this]
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
		DeinitProc();
	}
	else
	{
		GWorld->GetTimerManager().SetTimer(TimeoutHandler, DeinitProc, Timeout, false);
	}
}
