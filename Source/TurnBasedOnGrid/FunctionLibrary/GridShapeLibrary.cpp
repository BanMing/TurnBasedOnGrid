// Copyright BanMing

#include "FunctionLibrary/GridShapeLibrary.h"

#include "Grid/TileType.h"
#include "Kismet/KismetMathLibrary.h"
#include "TurnBasedOnGrid.h"

FGridShapeData UGridShapeLibrary::GetShapeData(EGridShape GridShape)
{
	FString DataTablePath = TEXT("/Game/Data/GridShape/DT_GridShapeData.DT_GridShapeData");
	if (UDataTable* GridShapeDataTable = LoadObject<UDataTable>(nullptr, *DataTablePath))
	{
		FString ShapeName = StaticEnum<EGridShape>()->GetNameStringByValue((int64) GridShape);
		FGridShapeData* Data = GridShapeDataTable->FindRow<FGridShapeData>(FName(ShapeName), TEXT(""));
		if (!Data)
		{
			UE_LOG(LogGrid, Error, TEXT("Cant find shape %s in %s data table"), *ShapeName, *GridShapeDataTable->GetName());
			return FGridShapeData();
		}
		return *Data;
	}
	return FGridShapeData();
}

bool UGridShapeLibrary::IsTileTypeWalkable(ETileType TileType)
{
	switch (TileType)
	{
		case ETileType::None:
			return false;
		case ETileType::Normal:
			return true;
		case ETileType::Obstacle:
			return false;
		default:
			return false;
	}
}

FVector UGridShapeLibrary::SnapVectorToVector(FVector V1, FVector V2)
{
	FVector Res;
	Res.X = FMath::GridSnap(V1.X, V2.X);
	Res.Y = FMath::GridSnap(V1.Y, V2.Y);
	Res.Z = FMath::GridSnap(V1.Z, V2.Z);
	return Res;
}
