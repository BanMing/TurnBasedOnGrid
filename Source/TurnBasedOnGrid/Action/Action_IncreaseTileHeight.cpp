// Copyright BanMing

#include "Action/Action_IncreaseTileHeight.h"

#include "Grid/GridBase.h"
#include "Player/PlayerAction.h"

void AAction_IncreaseTileHeight::ExecuteAction_Implementation(FIntPoint Index)
{
	if (PlayerAction->Grid->IsIndexValid(Index))
	{
		FTileData TileData = PlayerAction->Grid->GetTileDatafromGridIndex(Index);
		TileData.Transform.SetLocation(TileData.Transform.GetLocation() + FVector(0.f, 0.f, 1.f));
		PlayerAction->Grid->AddGridTile(TileData);
	}
}
