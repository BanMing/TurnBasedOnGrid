// Copyright BanMing

#include "Grid/GridModifier.h"

#include "FunctionLibrary/GridShapeLibrary.h"
#include "TurnBasedOnGrid.h"

AGridModifier::AGridModifier(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMesh);
}

void AGridModifier::MakeModifier()
{
	FGridShapeData GridShapeData = UGridShapeLibrary::GetShapeData(Shape);
	StaticMesh->SetStaticMesh(GridShapeData.Mesh.LoadSynchronous());

	FVector ColorParameter = FVector::ZeroVector;
	if (TileType == ETileType::Normal)
	{
		ColorParameter = FVector::OneVector;
	}
	else if (TileType == ETileType::Obstacle)
	{
		ColorParameter = FVector::ForwardVector;
	}
	StaticMesh->SetVectorParameterValueOnMaterials("Color", ColorParameter);

	StaticMesh->SetCollisionResponseToChannel(ECC_Ground, ECR_Overlap);

	SetActorHiddenInGame(true);
}

void AGridModifier::BeginPlay()
{
	Super::BeginPlay();
}
