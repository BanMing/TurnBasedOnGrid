// Copyright BanMing

#include "Grid/GridBase.h"

#include "Components/ChildActorComponent.h"
#include "FunctionLibrary/GridShapeLibrary.h"
#include "Grid/GridModifier.h"
#include "Grid/GridVisual.h"
#include "TurnBasedOnGrid.h"

AGridBase::AGridBase()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	ChildActorComponent = CreateDefaultSubobject<UChildActorComponent>("ChildActor_GridMeshVisual");
	SetRootComponent(SceneComponent);
}

void AGridBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//GridVisual = Cast<AGridVisual>(ChildActorComponent->GetChildActor());
}

void AGridBase::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

}

void AGridBase::BeginPlay()
{
	Super::BeginPlay();
}

FGridShapeData AGridBase::GetShapeData() const
{
	if (!GridShapeDataTable)
	{
		UE_LOG(LogGrid, Error, TEXT("GridShapeDataTable is NULL in %s"), *GetName());
		return FGridShapeData();
	}

	FString ShapeName = StaticEnum<EGridShape>()->GetNameStringByValue((int64) Shape);
	FGridShapeData* Data = GridShapeDataTable->FindRow<FGridShapeData>(FName(ShapeName), TEXT(""));
	if (!Data)
	{
		UE_LOG(LogGrid, Error, TEXT("Cant find shape %s in %s data table"), *ShapeName, *GridShapeDataTable->GetName());
		return FGridShapeData();
	}
	return *Data;
}

void AGridBase::SpawnGrid(FVector InCenterLocation, FVector InTileSize, FIntPoint InTileCount, EGridShape InShape, bool bInUseEnvironment)
{
	CenterLocation = InCenterLocation;
	TileSize = InTileSize;
	TileCount = InTileCount;
	Shape = InShape;
	bUseEnvironment = bInUseEnvironment;

	// TODO: load async
	FGridShapeData Data = GetShapeData();
	if (GridVisual == nullptr)
	{
		return;
	}
 	GridVisual->InitializeGridVisual(this);
	FVector2D HalfGrid = FVector2D(TileCount / 2) * FVector2D(TileSize);
	GridLocationLeftCornerLocation = CenterLocation - FVector(HalfGrid.X, HalfGrid.Y, 0.f);

	DestroyGrid();

	const int32 XCount = TileCount.X;
	const int32 YCount = TileCount.Y;
	for (int32 X = 0; X < XCount; X++)
	{
		for (int32 Y = 0; Y < YCount; Y++)
		{
			FTileData TileData;
			TileData.Index = FIntPoint(X, Y);
			FTransform TileTransform;
			TileTransform.SetScale3D(TileSize / Data.MeshSize);
			FVector Location = GetTileLocationFromXY(X, Y) + FVector(0.f, 0.f, 2.f);
			TileTransform.SetLocation(Location);

			if (bInUseEnvironment)
			{
				TileData.Type = TraceForGround(Location);
				TileTransform.SetLocation(Location);
			}
			else
			{
				TileData.Type = ETileType::Normal;
			}

			TileData.Transform = TileTransform;
			AddGridTile(TileData);
		}
	}
}

void AGridBase::SpawnGridByDefault()
{
	GridVisual = Cast<AGridVisual>(ChildActorComponent->GetChildActor());
	SpawnGrid(CenterLocation, TileSize, TileCount, Shape, bUseEnvironment);
}

void AGridBase::DrawDebugInfo()
{
	if (bDrawCenter)
	{
		DrawDebugSphere(GetWorld(), CenterLocation - FVector(TileSize.X / 2, TileSize.Y / 2, 0.f), 100.f, 3, FColor::Red, false, 0.1f, 0, 10.f);
	}

	if (bDrawBottomLeft)
	{
		DrawDebugSphere(GetWorld(), GridLocationLeftCornerLocation - FVector(TileSize.X / 2, TileSize.Y / 2, 0.f), 100.f, 3, FColor::Green, false, 0.1f, 0, 10.f);
	}

	if (bDrawBounds)
	{
		DrawDebugBox(GetWorld(), CenterLocation - FVector(TileSize.X / 2, TileSize.Y / 2, 0.f), CenterLocation - GridLocationLeftCornerLocation, FColor::Yellow, false, 0.1f, 0, 10.f);
	}
}

void AGridBase::DestroyGrid()
{
	GridTiles.Empty();
	GridVisual->DestroyGridVisual();
}

ETileType AGridBase::TraceForGround(FVector& OutLocation)
{
	const FVector Start = OutLocation + FVector(0.f, 0.f, 1000.f);
	const FVector End = OutLocation - FVector(0.f, 0.f, 1000.f);
	const float Radius = TileSize.X / (Shape == EGridShape::Triangle ? 5.f : 3.f);
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	const bool bHit = UKismetSystemLibrary::SphereTraceMulti(this, Start, End, Radius, UEngineTypes::ConvertToTraceType(ECC_Ground), false, ActorsToIgnore, DrawDebugTrace, OutHits, true);
	ETileType Res = ETileType::Normal;
	for (const FHitResult& HitResult : OutHits)
	{
		if (AGridModifier* GridModifier = Cast<AGridModifier>(HitResult.GetActor()))
		{
			return GridModifier->TileType;
		}
		else
		{
			OutLocation.Z = OutHits[0].Location.Z - Radius;
		}
	}
	return Res;
}

bool AGridBase::IsTileTypeWalkable(ETileType TileType)
{
	return UGridShapeLibrary::IsTileTypeWalkable(TileType);
}

void AGridBase::AddGridTile(FTileData TileData)
{
	GridTiles.Add(TileData.Index, TileData);
	GridVisual->UpdateTileVisual(TileData);
}
