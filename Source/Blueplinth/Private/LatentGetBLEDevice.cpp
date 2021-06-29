#include "LatentGetBLEDevice.h"
#include "Windows/WinRT.h"

using namespace winrt::Windows::Devices;
using namespace winrt::Windows::Foundation;
using namespace Bluetooth::Advertisement;

ULatentGetBLEDevice* ULatentGetBLEDevice::LatentGetBLEDevice(UObject* InWorldContextObject,
                                                             UDeviceMatcherBase* InDeviceMatcher,
                                                             const float InTimeout)
{
		ULatentGetBLEDevice* Node = NewObject<ULatentGetBLEDevice>();
		Node->RegisterWithGameInstance(InWorldContextObject);
		Node->Timeout = InTimeout;
		check(InDeviceMatcher);
		Node->Matcher = InDeviceMatcher;
		return Node;
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
					BLEDeviceObj->WinRTBLEDevice = BLEDevice;
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
