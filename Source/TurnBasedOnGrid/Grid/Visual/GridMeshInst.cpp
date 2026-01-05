// Copyright BanMing

#include "Grid/Visual/GridMeshInst.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurnBasedOnGrid.h"

AGridMeshInst::AGridMeshInst()
{
	PrimaryActorTick.bCanEverTick = false;
	InstancesMeshComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>("InstancesMeshComp");
	InstancesMeshComp->SetupAttachment(GetRootComponent());

	InstancesMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InstancesMeshComp->SetCollisionObjectType(ECC_WorldStatic);
	InstancesMeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	InstancesMeshComp->SetCollisionResponseToChannel(ECC_Grid, ECR_Block);

	// tile color -> rgb, isfilled
	InstancesMeshComp->NumCustomDataFloats = 4;
}

void AGridMeshInst::BeginPlay()
{
	Super::BeginPlay();
}

void AGridMeshInst::InitializeGridMeshInst(UStaticMesh* NewMesh, UMaterialInterface* Material, FLinearColor Color, ECollisionEnabled::Type Collision)
{
	InstancesMeshComp->SetStaticMesh(NewMesh);
	InstancesMeshComp->SetMaterial(0, Material);
	InstancesMeshComp->SetVectorParameterValueOnMaterials("Color", FVector(Color));
	InstancesMeshComp->SetCollisionEnabled(Collision);
}

void AGridMeshInst::AddInstance(FTransform InTransform, FIntPoint Index, TArray<ETileState> TileStates)
{
	RemoveInstance(Index);
	const int32 NewInstanceIndex = InstancesMeshComp->AddInstance(InTransform);
	InstanceIndexes.Add(Index);

	const FLinearColor Color = GetColorFromStates(TileStates);
	InstancesMeshComp->SetCustomDataValue(NewInstanceIndex, 0, Color.R);
	InstancesMeshComp->SetCustomDataValue(NewInstanceIndex, 1, Color.G);
	InstancesMeshComp->SetCustomDataValue(NewInstanceIndex, 2, Color.B);
	const float IsFilled = TileStates.Contains(ETileState::Selected) || TileStates.Contains(ETileState::IsNeighbor) || TileStates.Contains(ETileState::IsInPath) ? 1.f : 0.f;
	InstancesMeshComp->SetCustomDataValue(NewInstanceIndex, 3, IsFilled);
}

void AGridMeshInst::RemoveInstance(FIntPoint Index)
{
	if (InstanceIndexes.Contains(Index))
	{
		InstancesMeshComp->RemoveInstance(InstanceIndexes.Find(Index));
		InstanceIndexes.Remove(Index);
	}
}

void AGridMeshInst::ClearInstances()
{
	InstancesMeshComp->ClearInstances();
	InstanceIndexes.Empty();
}

FLinearColor AGridMeshInst::GetColorFromStates(TArray<ETileState> TileStates) const
{
	if (TileStates.Num() == 0)
	{
		return FLinearColor::Black;
	}

	if (TileStates.Contains(ETileState::Hovered))
	{
		return HoveredColor;
	}
	else if (TileStates.Contains(ETileState::Selected))
	{
		return SelectedColor;
	}
	else if (TileStates.Contains(ETileState::IsNeighbor))
	{
		return NeighborColor;
	}
	else if (TileStates.Contains(ETileState::IsInPath))
	{
		return InPathColor;
	}

	return FLinearColor::Black;
}
