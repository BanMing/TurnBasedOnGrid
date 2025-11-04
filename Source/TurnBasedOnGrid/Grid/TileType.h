// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	None,
	Normal,
	Obstacle
};

UENUM(BlueprintType)
enum class ETileState : uint8
{
	None,
	Hovered,
	Selected
};