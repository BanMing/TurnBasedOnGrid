// Copyright BanMing

#include "Action/Action_SelectTile.h"

#include "Grid/GridBase.h"
#include "Player/PlayerAction.h"

void AAction_SelectTile::ExecuteAction_Implementation(FIntPoint Index)
{
	check(PlayerAction);
	check(PlayerAction->Grid);

	PlayerAction->Grid->RemoveStateFromTile(PlayerAction->CurSelectTile, ETileState::Selected);

	if (PlayerAction->CurSelectTile != Index)
	{
		PlayerAction->CurSelectTile = Index;
		PlayerAction->Grid->AddStateToTile(Index, ETileState::Selected);
	}
	else
	{
		PlayerAction->CurSelectTile = FIntPoint(-999, -999);
	}
}

void AAction_SelectTile::Destroyed()
{
	ExecuteAction(FIntPoint(-999, -999));
	Super::Destroyed();
}
