// Copyright BanMing
#include "Grid/Visual/GridVisual.h"

#include "Components/ChildActorComponent.h"
#include "FunctionLibrary/GridShapeLibrary.h"
#include "Grid/GridBase.h"
#include "Grid/Visual/GridMeshInst.h"
#include "Grid/GridShapes/GridShapeData.h"
#include "Grid/TileData.h"

AGridVisual::AGridVisual()
{
	PrimaryActorTick.bCanEverTick = false;
	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("ChildActor_GridMeshInst");
	SetRootComponent(ChildActorComponent);
}

void AGridVisual::InitializeGridVisual(AGridBase* InGrid)
{
	Grid = InGrid;
	GridMeshInst = Cast<AGridMeshInst>(ChildActorComponent->GetChildActor());
	if (!GridMeshInst)
	{
		return;
	}
	FGridShapeData GridShapeData = Grid->GetShapeData();
	GridMeshInst->InitializeGridMeshInst(GridShapeData.FlatMesh.LoadSynchronous(), GridShapeData.FlatMaterial.LoadSynchronous(), FLinearColor::Black, ECollisionEnabled::QueryOnly);
	SetActorLocation(FVector::ZeroVector);
	SetOffsetfromGround(OffsetfromGround);
}

void AGridVisual::SetOffsetfromGround(float Offset)
{
	OffsetfromGround = Offset;
	FVector ActorLocation = GetActorLocation();
	SetActorLocation(FVector(ActorLocation.X, ActorLocation.Y, OffsetfromGround));
}

void AGridVisual::DestroyGridVisual()
{
	if (GridMeshInst)
	{
		GridMeshInst->ClearInstances();
	}
	Grid = nullptr;
}

void AGridVisual::UpdateTileVisual(FTileData TileData)
{
	if (!GridMeshInst)
	{
		return;
	}
	GridMeshInst->RemoveInstance(TileData.Index);
	if (UGridShapeLibrary::IsTileTypeWalkable(TileData.Type))
	{
		GridMeshInst->AddInstance(TileData.Transform, TileData.Index, TileData.States);
	}
}

void AGridVisual::BeginPlay()
{
	Super::BeginPlay();
}
