// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Grid/TileType.h"

#include "TileData.generated.h"

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FIntPoint Index;

	UPROPERTY(BlueprintReadOnly)
	ETileType Type;

	UPROPERTY(BlueprintReadOnly)
	FTransform Transform;
};