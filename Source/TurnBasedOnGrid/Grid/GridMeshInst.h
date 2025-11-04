// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Grid/TileType.h"

#include "GridMeshInst.generated.h"

UCLASS()
class TURNBASEDONGRID_API AGridMeshInst : public AActor
{
	GENERATED_BODY()

public:
	AGridMeshInst();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void InitializeGridMeshInst(UStaticMesh* NewMesh, UMaterialInterface* Material, FLinearColor Color, ECollisionEnabled::Type Collision);

	UFUNCTION(BlueprintCallable)
	void AddInstance(FTransform InTransform, FIntPoint Index, TArray<ETileState> TileStates);

	UFUNCTION(BlueprintCallable)
	void RemoveInstance(FIntPoint Index);

	UFUNCTION(BlueprintCallable)
	void ClearInstances();

	UFUNCTION(BlueprintPure)
	FLinearColor GetColorFromStates(TArray<ETileState> TileStates) const;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FLinearColor HoveredColor;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FLinearColor SelectedColor;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<class UInstancedStaticMeshComponent> InstancesMeshComp;

	UPROPERTY(BlueprintReadOnly)
	TArray<FIntPoint> InstanceIndexes;
};
