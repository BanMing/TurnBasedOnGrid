// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Grid/GridShapes/GridShapeData.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GridShapeLibrary.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDONGRID_API UGridShapeLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	static FGridShapeData GetShapeData(EGridShape GridShape);

	UFUNCTION(BlueprintPure)
	static bool IsTileTypeWalkable(ETileType TileType);

	UFUNCTION(BlueprintCallable)
	static FVector SnapVectorToVector(FVector V1, FVector V2);
};
