// Copyright BanMing

#pragma once

#include "CoreMinimal.h"

#include "GridShapeData.generated.h"

class UStaticMesh;
class UMaterialInstance;

UENUM(BlueprintType)
enum class EGridShape : uint8
{
	None,
	Triangle,
	Square,
	Hexagon
};

USTRUCT(BlueprintType)
struct FGridShapeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	EGridShape GridShapeType = EGridShape::None;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FVector MeshSize = FVector(100.f, 100.f, 100.f);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UMaterialInstance> MeshMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> FlatMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UMaterialInstance> FlatBorderMaterial;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UMaterialInstance> FlatFilledMaterial;
};
