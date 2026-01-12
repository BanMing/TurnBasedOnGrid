// Copyright BanMing
#include "Action/Pathfinding/Action_ShowTileNeighbors.h"

#include "Grid/GridBase.h"
#include "Grid/Pathfinding/GridPathfinding.h"
#include "Player/PlayerAction.h"

void AAction_ShowTileNeighbors::ExecuteAction_Implementation(FIntPoint Index)
{
	check(PlayerAction);
	check(PlayerAction->Grid);

	PlayerAction->Grid->ClearStateFromTiles(ETileState::IsNeighbor);
	TArray<FPathfindingData> Neihbors = PlayerAction->Grid->GridPathfinding->GetValidTileNeighbors(Index, bIncludeDiagonals);
	for (const FPathfindingData& Neihbor : Neihbors)
	{
		PlayerAction->Grid->AddStateToTile(Neihbor.Index, ETileState::IsNeighbor);
	}
}

void AAction_ShowTileNeighbors::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	ExecuteAction(FIntPoint(-999, -999));
}
