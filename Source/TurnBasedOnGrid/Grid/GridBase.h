// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/GridShapes/GridShapeData.h"

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
	UFUNCTION(BlueprintPure)
	FGridShapeData* GetShapeData() const;

	UFUNCTION(BlueprintCallable)
	void SpawnGrid(FVector InCenterLocation, FVector InTileSize, FVector2D InTileCount, EGridShape InShape);

	UFUNCTION(BlueprintCallable)
	void DestroyGrid();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector CenterLocation;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector TileSize;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FVector2D TileCount;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGridShape Shape;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UInstancedStaticMeshComponent> InstancesMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> GridShapeDataTable;

protected:
	UPROPERTY(BlueprintReadOnly)
	FVector GridLocationLeftCornerLocation;
};
