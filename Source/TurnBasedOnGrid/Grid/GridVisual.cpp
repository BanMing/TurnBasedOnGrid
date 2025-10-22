// Copyright BanMing
#include "Grid/GridVisual.h"

#include "Components/ChildActorComponent.h"
#include "Grid/GridBase.h"
#include "Grid/GridMeshInst.h"
#include "Grid/GridShapes/GridShapeData.h"

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
	FGridShapeData GridShapeData = Grid->GetShapeData();
	GridMeshInst->InitializeGridMeshInst(GridShapeData.FlatMesh.LoadSynchronous(), GridShapeData.FlatBorderMaterial.LoadSynchronous(), FLinearColor::Black, ECollisionEnabled::QueryOnly);
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
	GridMeshInst->ClearInstances();
}

void AGridVisual::BeginPlay()
{
	Super::BeginPlay();
}
