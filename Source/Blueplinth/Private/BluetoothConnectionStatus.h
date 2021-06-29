#pragma once

#include "CoreMinimal.h"
#include "BluetoothConnectionStatus.generated.h"

UENUM(BlueprintType)
enum class EBluetoothConnectionStatus : uint8
{
	Disconnected = 0,
	Connected = 1,
};

// template <typename Enum1, typename Enum2>
// std::map<Enum1, Enum2> EnumMapTable = {
// 	
// };