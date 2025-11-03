// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "GridVisual.generated.h"

class AGridBase;
class AGridMeshInst;

UCLASS()
class TURNBASEDONGRID_API AGridVisual : public AActor
{
	GENERATED_BODY()

public:
	AGridVisual();

	UFUNCTION(BlueprintCallable)
	void InitializeGridVisual(AGridBase* InGrid);

	UFUNCTION(BlueprintCallable)
	void SetOffsetfromGround(float Offset);

	UFUNCTION(BlueprintCallable)
	void DestroyGridVisual();

	UFUNCTION(BlueprintCallable)
	void UpdateTileVisual(FTileData TileData);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UChildActorComponent> ChildActorComponent;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	float OffsetfromGround = 2.f;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridBase> Grid;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridMeshInst> GridMeshInst;
};
