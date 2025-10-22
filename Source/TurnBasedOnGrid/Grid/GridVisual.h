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
	void UpdateTileVisual();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TObjectPtr<class UChildActorComponent> ChildActorComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridBase> Grid;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridMeshInst> GridMeshInst;

	UPROPERTY(BlueprintReadOnly)
	float OffsetfromGround;
};
