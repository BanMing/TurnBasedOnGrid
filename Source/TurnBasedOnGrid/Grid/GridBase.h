// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/GridShapes/GridShapeData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Grid/TileType.h"

#include "GridBase.generated.h"

class UInstancedStaticMeshComponent;
class UDataTable;
UCLASS()
class TURNBASEDONGRID_API AGridBase : public AActor
{
	GENERATED_BODY()

public:
	AGridBase();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void SpawnGrid(FVector InCenterLocation, FVector InTileSize, FIntPoint InTileCount, EGridShape InShape, bool bInUseEnvironment);

	UFUNCTION(BlueprintCallable)
	void SpawnGridByDefault();

	UFUNCTION(BlueprintCallable)
	void DrawDebugInfo();

	UFUNCTION(BlueprintCallable)
	void DestroyGrid();

	UFUNCTION(BlueprintCallable)
	ETileType TraceForGround(FVector& OutLocation);

	UFUNCTION(BlueprintPure)
	bool IsTileTypeWalkable(ETileType TileType);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetTileLocationFromXY(int32 X, int32 Y) const
	{
		return GridLocationLeftCornerLocation + TileSize * FVector(X, Y, 0.f);
	}

public:
	FGridShapeData* GetShapeData() const;

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

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float OffsetFromGround = 2.f;

public:
	// Debug
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawBounds = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawCenter = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bDrawBottomLeft = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UInstancedStaticMeshComponent> InstancesMeshComp;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<UDataTable> GridShapeDataTable;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector GridLocationLeftCornerLocation;
};
