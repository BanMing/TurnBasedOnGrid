// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridPathfinding.generated.h"

class AGridBase;

UCLASS()
class TURNBASEDONGRID_API AGridPathfinding : public AActor
{
	GENERATED_BODY()

public:
	AGridPathfinding();
	void InitializeGridPathfinding(AGridBase* InGrid);
	void FinalizeGridPathfinding();

public:
	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetValidTileNeihbors(FIntPoint Index, bool bIncludeDiagonals) const;
	
	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetTileNeihbors(FIntPoint Index, bool bIncludeDiagonals) const;

protected:
	TArray<FIntPoint> GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals) const;
	TArray<FIntPoint> GetNeighborIndexesForHexagon(FIntPoint Index) const;
	TArray<FIntPoint> GetNeighborIndexesForTriangle(FIntPoint Index, bool bIncludeDiagonals) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridBase> Grid;
};
