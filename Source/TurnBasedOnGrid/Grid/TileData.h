// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Grid/TileType.h"

#include "TileData.generated.h"

USTRUCT(BlueprintType)
struct FTileData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FIntPoint Index;

	UPROPERTY(BlueprintReadWrite)
	ETileType Type;

	UPROPERTY(BlueprintReadWrite)
	TArray<ETileState> States;

	UPROPERTY(BlueprintReadWrite)
	FTransform Transform;
};