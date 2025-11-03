// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/GridShapes/GridShapeData.h"
#include "Grid/TileData.h"
#include "Grid/TileType.h"
#include "Kismet/KismetSystemLibrary.h"

#include "GridBase.generated.h"

class UDataTable;
class AGridVisual;

UCLASS()
class TURNBASEDONGRID_API AGridBase : public AActor
{
	GENERATED_BODY()

public:
	AGridBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostRegisterAllComponents() override;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnGrid(FVector InCenterLocation, FVector InTileSize, FIntPoint InTileCount, EGridShape InShape, bool bInUseEnvironment);

	UFUNCTION(BlueprintCallable)
	void SpawnGridByDefault();

	UFUNCTION(BlueprintCallable)
	void DrawDebugInfo() const;

	UFUNCTION(BlueprintCallable)
	void DestroyGrid();

	UFUNCTION(BlueprintCallable)
	ETileType TraceForGround(FVector& OutLocation);

	UFUNCTION(BlueprintPure)
	bool IsTileTypeWalkable(ETileType TileType);

	UFUNCTION(BlueprintCallable)
	void AddGridTile(FTileData TileData);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetTileLocationFromXY(int32 X, int32 Y) const
	{
		return GridLocationLeftCornerLocation + TileSize * FVector(X, Y, 0.f);
	}

	UFUNCTION(BlueprintPure)
	FGridShapeData GetShapeData() const;

	UFUNCTION(BlueprintPure)
	FVector GetCursorLocationOnGrid(int32 PlayerIndex = 0) const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetTileIndexfromWorldLocation(FVector Location) const;

	UFUNCTION(BlueprintPure)
	FIntPoint GetTileIndexUnderCursor(int32 PlayerIndex = 0) const;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector CenterLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector TileSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FIntPoint TileCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGridShape Shape;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bUseEnvironment = true;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTrace;

public:
	// Debug
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawBounds = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawCenter = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawBottomLeft = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawMouseLocation = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawHoveredTile = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UDataTable> GridShapeDataTable;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<class UChildActorComponent> ChildActorComponent;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridVisual> GridVisual;

	UPROPERTY(BlueprintReadOnly)
	FVector GridLocationLeftCornerLocation;

	UPROPERTY(BlueprintReadOnly)
	TMap<FIntPoint, FTileData> GridTiles;
};
