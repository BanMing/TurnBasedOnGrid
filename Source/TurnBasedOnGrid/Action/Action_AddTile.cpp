// Copyright BanMing

#include "Action/Action_AddTile.h"

#include "Grid/GridBase.h"
#include "Player/PlayerAction.h"

void AAction_AddTile::ExecuteAction_Implementation(FIntPoint Index)
{
	if (PlayerAction->Grid->IsIndexValid(Index))
	{
		return;
	}
	FTileData Data;
	Data.Index = Index;
	FVector Location = PlayerAction->Grid->GetTileLocationFromXY(Index.X, Index.Y);
	Data.Type = PlayerAction->Grid->TraceForGround(Location);
	Data.Transform.SetLocation(Location);
	Data.Transform.SetScale3D(PlayerAction->Grid->GetTitleScale());
	// Data.Transform.SetRotation(PlayerAction->Grid->GetTitleRotationfromGridIndex(Index).Quaternion());
	PlayerAction->Grid->AddGridTile(Data);
}
