// Copyright BanMing

#include "Grid/GridBase.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
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

void AGridBase::SpawnGrid(FVector InCenterLocation, FVector InTileSize, FIntPoint InTileCount, EGridShape InShape, bool bUseEnvironment)
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
	FVector2D HalfGrid = FVector2D(TileCount / 2) * FVector2D(TileSize);
	GridLocationLeftCornerLocation = CenterLocation - FVector(HalfGrid.X, HalfGrid.Y, 0.f);

	DestroyGrid();

	const int32 XCount = TileCount.X;
	const int32 YCount = TileCount.Y;
	for (int32 X = 0; X < XCount; X++)
	{
		for (int32 Y = 0; Y < YCount; Y++)
		{
			FTransform TileTransform;
			TileTransform.SetScale3D(TileSize / Data->MeshSize);
			FVector Location = GetTileLocationFromXY(X, Y) + FVector(0.f, 0.f, 2.f);
			if (bUseEnvironment)
			{
				if (TraceForGround(Location))
				{
					TileTransform.SetLocation(Location);
					InstancesMeshComp->AddInstance(TileTransform);
				}
			}
			else
			{
				TileTransform.SetLocation(Location);
				InstancesMeshComp->AddInstance(TileTransform);
			}
		}
	}
}

void AGridBase::SpawnGridByDefault()
{
	SpawnGrid(CenterLocation, TileSize, TileCount, Shape, true);
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
	InstancesMeshComp->ClearInstances();
}

bool AGridBase::TraceForGround(FVector& OutLocation)
{
	const FVector Start = OutLocation + FVector(0.f, 0.f, 1000.f);
	const FVector End = OutLocation - FVector(0.f, 0.f, 1000.f);
	const float Radius = TileSize.X / (Shape == EGridShape::Triangle ? 5.f : 3.f);
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> OutHits;
	const bool bHit = UKismetSystemLibrary::SphereTraceMulti(this, Start, End, Radius, UEngineTypes::ConvertToTraceType(ECC_Ground), false, ActorsToIgnore, DrawDebugTrace, OutHits, true);
	if (OutHits.Num() > 0)
	{
		OutLocation.Z = OutHits[0].Location.Z - Radius;
		// FMath::GridSnap(OutHits[0].Location.Z - Radius, TileSize.Z);

		return true;
	}
	return false;
}
