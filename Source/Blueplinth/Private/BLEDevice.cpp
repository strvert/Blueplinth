#include "BLEDevice.h"

void UBLEDevice::GetBluetoothAddress(FBluetoothAddress& Address) const
{
	Address = FBluetoothAddress::FromUint64(WinRTBLEDevice.BluetoothAddress());
}

void UBLEDevice::GetDeviceId(FString& DeviceId) const
{
	DeviceId = FString(WinRTBLEDevice.DeviceId().c_str());
}

void UBLEDevice::GetConnectionStatus(EBluetoothConnectionStatus& Status) const
{
	const auto WinRTStatus = WinRTBLEDevice.ConnectionStatus();
	Status = static_cast<EBluetoothConnectionStatus>(WinRTStatus);
}