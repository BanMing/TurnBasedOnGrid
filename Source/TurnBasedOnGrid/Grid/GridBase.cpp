// Copyright BanMing

#include "Grid/GridBase.h"

#include "Components/ChildActorComponent.h"
#include "FunctionLibrary/GridShapeLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Grid/GridModifier.h"
#include "Grid/GridVisual.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
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
	// GridVisual = Cast<AGridVisual>(ChildActorComponent->GetChildActor());
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

FVector AGridBase::GetCursorLocationOnGrid(int32 PlayerIndex) const
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, PlayerIndex);
	FHitResult HitResult;
	if (PC->GetHitResultUnderCursor(ECC_Grid, false, HitResult))
	{
		return HitResult.Location;
	}

	const FPlane Plane = FPlane(CenterLocation, FVector::UpVector);
	FVector WorldLocation;
	FVector WorldDirection;
	PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	const FVector LineEnd = WorldLocation + WorldDirection * 9999.f;
	float T;
	FVector Intersection;
	if (UKismetMathLibrary::LinePlaneIntersection(WorldLocation, LineEnd, Plane, T, Intersection))
	{
		return Intersection;
	}

	return FVector(-999.f, -999.f, -999.f);
}

FIntPoint AGridBase::GetTileIndexfromWorldLocation(FVector Location) const
{
	FVector LocationOnGrid = Location - GridLocationLeftCornerLocation;
	if (Shape == EGridShape::Square)
	{
		const FVector SnapVector = UGridShapeLibrary::SnapVectorToVector(LocationOnGrid, TileSize);
		return FIntPoint(SnapVector.X / TileSize.X, SnapVector.Y / TileSize.Y);
	}
	else if (Shape == EGridShape::Hexagon)
	{
		const FVector SnapVector = UGridShapeLibrary::SnapVectorToVector(LocationOnGrid * FVector(1.f, 2.f, 1.f), TileSize * FVector(0.75f, 0.25f, 1.f));
		FIntPoint TempIndex = FIntPoint(SnapVector.X / TileSize.X, SnapVector.Y / TileSize.Y) * FIntPoint(0.75f, 1.f);
		if (TempIndex.X % 2 == 0)
		{
			return FIntPoint(TempIndex.X, FMath::RoundToInt(TempIndex.Y / 2.f) * 2);
		}
		else
		{
			return FIntPoint(TempIndex.X, FMath::FloorToInt(TempIndex.Y / 2.f) * 2 + 1);
		}
	}
	else if (Shape == EGridShape::Triangle)
	{
		const FVector SnapVector = UGridShapeLibrary::SnapVectorToVector(LocationOnGrid, TileSize / FVector(1.f, 2.f, 1.f));
		return FIntPoint(SnapVector.X / TileSize.X, SnapVector.Y / TileSize.Y) * FIntPoint(1, 2);
	}
	return FIntPoint(-999, -999);
}

FIntPoint AGridBase::GetTileIndexUnderCursor(int32 PlayerIndex) const
{
	return GetTileIndexfromWorldLocation(GetCursorLocationOnGrid(PlayerIndex));
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
			FVector Location = GetTileLocationFromXY(X, Y);
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
	SpawnGrid(GetActorLocation(), TileSize, TileCount, Shape, bUseEnvironment);
}

void AGridBase::DrawDebugInfo() const
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

	if (bDrawMouseLocation)
	{
		FVector CursorLocationOnGrid = GetCursorLocationOnGrid();
		DrawDebugSphere(GetWorld(), CursorLocationOnGrid, 15.f, 5, FColor::Yellow, false, 0.1f, 5.f);
		DrawDebugString(GetWorld(), CursorLocationOnGrid, CursorLocationOnGrid.ToCompactString(), 0, FColor::Yellow, 0.1f);
	}

	if (bDrawHoveredTile)
	{
		FIntPoint TileIndex = GetTileIndexUnderCursor(0);
		const FTileData* TileData = GridTiles.Find(TileIndex);
		if (TileData)
		{
			DrawDebugBox(GetWorld(), TileData->Transform.GetLocation(), FVector(35.f, 35.f, 5.f), FQuat::Identity, FColor::Green, false, 0.1f, 0.f, 5.f);
			DrawDebugString(GetWorld(), TileData->Transform.GetLocation(), TileData->Transform.GetLocation().ToCompactString(), 0, FColor::Green, 0.1f);
		}
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
	ETileType Res = bHit ? ETileType::Normal : ETileType::None;
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
