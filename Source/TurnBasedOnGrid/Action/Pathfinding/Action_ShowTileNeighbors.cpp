// Copyright BanMing
#include "Action/Pathfinding/Action_ShowTileNeighbors.h"

#include "Grid/GridBase.h"
#include "Grid/GridPathfinding.h"
#include "Player/PlayerAction.h"

void AAction_ShowTileNeighbors::ExecuteAction_Implementation(FIntPoint Index)
{
	check(PlayerAction);
	check(PlayerAction->Grid);

	PlayerAction->Grid->ClearStateFromTiles(ETileState::IsNeighbor);
	TArray<FIntPoint> Neihbors = PlayerAction->Grid->GridPathfinding->GetValidTileNeihbors(Index, bIncludeDiagonals);
	for (const FIntPoint& Neihbor : Neihbors)
	{
		PlayerAction->Grid->AddStateToTile(Neihbor, ETileState::IsNeighbor);
	}
}

void AAction_ShowTileNeighbors::Destroyed()
{
	ExecuteAction(FIntPoint(-999, -999));
	Super::Destroyed();
}
