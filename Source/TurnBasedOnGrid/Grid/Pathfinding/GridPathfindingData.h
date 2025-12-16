// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

#include "GridPathfindingData.generated.h"

USTRUCT(BlueprintType)
struct FPathfindingData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FIntPoint Index = {-999, -999};

	UPROPERTY(BlueprintReadWrite)
	int32 CostToEnterTile = 1;

	UPROPERTY(BlueprintReadWrite)
	int32 CostFromStart = 999999;

	UPROPERTY(BlueprintReadWrite)
	int32 MinCostToTarget = 999999;

	UPROPERTY(BlueprintReadWrite)
	FIntPoint PrevIndex = {-999, -999};
};