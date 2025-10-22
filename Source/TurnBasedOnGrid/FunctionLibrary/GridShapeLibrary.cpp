// Copyright BanMing

#include "FunctionLibrary/GridShapeLibrary.h"

#include "TurnBasedOnGrid.h"

FGridShapeData UGridShapeLibrary::GetShapeData(EGridShape GridShape)
{
	FString DataTablePath = TEXT("/Game/Blueprints/Grid/GridShape/DT_GridShapeData.DT_GridShapeData");
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
