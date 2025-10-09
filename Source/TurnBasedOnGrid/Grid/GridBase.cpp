// Copyright BanMing

#include "Grid/GridBase.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "TurnBasedOnGrid.h"

AGridBase::AGridBase()
{
	PrimaryActorTick.bCanEverTick = false;
	InstancesMeshComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancesMeshComp");
	InstancesMeshComp->SetupAttachment(GetRootComponent());
}

void AGridBase::BeginPlay()
{
	Super::BeginPlay();
}

FGridShapeData* AGridBase::GetShapeData() const
{
	if (!GridShapeDataTable)
	{
		UE_LOG(LogGrid, Error, TEXT("GridShapeDataTable is NULL in %s"), *GetName());
		return nullptr;
	}

	FString ShapeName = StaticEnum<EGridShape>()->GetNameStringByValue((int64) Shape);
	FGridShapeData* Data = GridShapeDataTable->FindRow<FGridShapeData>(FName(ShapeName), TEXT(""));
	if (!Data)
	{
		UE_LOG(LogGrid, Error, TEXT("Cant find shape %s in %s data table"), *ShapeName, *GridShapeDataTable->GetName());
		return nullptr;
	}
	return Data;
}

void AGridBase::SpawnGrid(FVector InCenterLocation, FVector InTileSize, FVector2D InTileCount, EGridShape InShape)
{
	CenterLocation = InCenterLocation;
	TileSize = InTileSize;
	TileCount = InTileCount;
	Shape = InShape;

	// TODO: load async
	FGridShapeData* Data = GetShapeData();
	if (!Data)
	{
		return;
	}

	InstancesMeshComp->SetStaticMesh(Data->FlatMesh.LoadSynchronous());
	InstancesMeshComp->SetMaterial(0, Data->FlatBorderMaterial.LoadSynchronous());
	FVector2D HalfGrid = TileCount / 2 * FVector2D(TileSize);
	GridLocationLeftCornerLocation = CenterLocation - FVector(HalfGrid.X, HalfGrid.Y, 0.f);

	DestroyGrid();

	const int32 XCount = FMath::RoundToInt(TileCount.X);
	const int32 YCount = FMath::RoundToInt(TileCount.Y);
	for (int32 X = 0; X < XCount; X++)
	{
		for (int32 Y = 0; Y < YCount; Y++)
		{
			FTransform TileTransform;
			TileTransform.SetScale3D(TileSize / Data->MeshSize);
			TileTransform.SetLocation(GetTileLocationFromXY(X, Y) + FVector(0.f, 0.f, 2.f));
			InstancesMeshComp->AddInstance(TileTransform);
		}
	}
}

void AGridBase::SpawnGridByDefault()
{
	SpawnGrid(CenterLocation, TileSize, TileCount, Shape);
}

void AGridBase::DestroyGrid()
{
	InstancesMeshComp->ClearInstances();
}
