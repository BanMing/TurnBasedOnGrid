// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/GridShapes/GridShapeData.h"
#include "Grid/TileType.h"

#include "GridModifier.generated.h"

UCLASS()
class TURNBASEDONGRID_API AGridModifier : public AActor
{
	GENERATED_BODY()

public:
	AGridModifier(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void MakeModifier();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EGridShape Shape;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	ETileType TileType;
};
