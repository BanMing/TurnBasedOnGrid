// Copyright BanMing

#include "Action/Action_RemoveTile.h"

#include "Grid/GridBase.h"
#include "Player/PlayerAction.h"

void AAction_RemoveTile::ExecuteAction_Implementation(FIntPoint Index)
{
	if (PlayerAction->Grid->IsIndexValid(Index))
	{
		PlayerAction->Grid->RemoveGridTile(Index);
	}
}
