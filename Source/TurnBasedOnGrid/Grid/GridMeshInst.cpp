// Copyright BanMing

#include "Grid/GridMeshInst.h"

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

void AGridMeshInst::AddInstance(FTransform InTransform, FIntPoint Index)
{
	RemoveInstance(Index);
	InstancesMeshComp->AddInstance(InTransform);
	InstanceIndexes.Add(Index);
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
