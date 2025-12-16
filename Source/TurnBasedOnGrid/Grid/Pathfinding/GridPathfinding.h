// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/Pathfinding/GridPathfindingData.h"

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
	TArray<FPathfindingData> GetValidTileNeighbors(FIntPoint Index, bool bIncludeDiagonals) const;

	UFUNCTION(BlueprintPure)
	TArray<FIntPoint> GetTileNeighbors(FIntPoint Index, bool bIncludeDiagonals) const;

	UFUNCTION(BlueprintCallable)
	TArray<FIntPoint> FindPath(FIntPoint Start, FIntPoint Target, bool bIncludeDiagonals);

protected:
	TArray<FIntPoint> GetNeighborIndexesForSquare(FIntPoint Index, bool bIncludeDiagonals) const;
	TArray<FIntPoint> GetNeighborIndexesForHexagon(FIntPoint Index) const;
	TArray<FIntPoint> GetNeighborIndexesForTriangle(FIntPoint Index, bool bIncludeDiagonals) const;

protected:
	bool IsInputDataValid() const;
	void DiscoverTile(FPathfindingData TilePathData);
	int32 GetMinCostBetweenTwoTiles(FIntPoint Index1, FIntPoint Index2, bool bIncludeDiagonals) const;
	bool AnalyseNextDiscoveredTile();
	TArray<FIntPoint> GeneratePath();
	FPathfindingData PullCheapestTileOutOfDiscoveredList();
	bool DiscoverNextNeighbor();
	void InsertTileInDiscoveredArray(FPathfindingData TileData);
	void ResetPathfindingInfo();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridBase> Grid;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint StartIndex;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint TargetIndex;

	UPROPERTY(BlueprintReadOnly)
	bool bFindingIncludeDiagonals;

	// 按照Cost排序，越大靠前
	UPROPERTY(BlueprintReadOnly)
	TArray<FIntPoint> DiscoveredTileIndexes;

	// 越大靠前
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> DiscoveredTileSortingCosts;

	UPROPERTY(BlueprintReadOnly)
	TArray<FIntPoint> AnalysedTileIndexes;

	UPROPERTY(BlueprintReadOnly)
	FPathfindingData CurrentDiscoveredTile;

	UPROPERTY(BlueprintReadOnly)
	TArray<FPathfindingData> CurrentNeighbors;

	UPROPERTY(BlueprintReadOnly)
	FPathfindingData CurrentNeighbor;

	UPROPERTY(BlueprintReadOnly)
	TMap<FIntPoint, FPathfindingData> PathfindingData;
};
