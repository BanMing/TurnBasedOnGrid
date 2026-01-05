// Copyright BanMing

#include "Action/Pathfinding/Action_FindPathToTarget.h"

#include "Grid/GridBase.h"
#include "Grid/Pathfinding/GridPathfinding.h"
#include "Player/PlayerAction.h"

void AAction_FindPathToTarget::ExecuteAction_Implementation(FIntPoint Index)
{
	check(PlayerAction);
	check(PlayerAction->Grid);

	PlayerAction->Grid->ClearStateFromTiles(ETileState::IsInPath);

	TArray<FIntPoint> Path = PlayerAction->Grid->GridPathfinding->FindPath(PlayerAction->CurSelectTile, Index, bIncludeDiagonals);
	for (const FIntPoint& Index : Path)
	{
		PlayerAction->Grid->AddStateToTile(Index, ETileState::IsInPath);
	}
}

void AAction_FindPathToTarget::Destroyed()
{
	Super::Destroyed();
	ExecuteAction(FIntPoint(-999, -999));
}
