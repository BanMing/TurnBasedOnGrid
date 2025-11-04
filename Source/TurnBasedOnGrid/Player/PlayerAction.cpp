// Copyright BanMing

#include "Player/PlayerAction.h"

#include "Grid/GridBase.h"
#include "Kismet/GameplayStatics.h"

APlayerAction::APlayerAction()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerAction::BeginPlay()
{
	Super::BeginPlay();
	AActor* GridActor = UGameplayStatics::GetActorOfClass(this, AGridBase::StaticClass());
	check(GridActor);

	Grid = Cast<AGridBase>(GridActor);
	check(Grid);
}

void APlayerAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateTileUnderCursor();
}

void APlayerAction::UpdateTileUnderCursor()
{
	if (!Grid)
	{
		return;
	}
	FIntPoint HoveredTile = Grid->GetTileIndexUnderCursor();
	if (CurHoveredTile != HoveredTile)
	{
		Grid->RemoveStateFromTile(CurHoveredTile, ETileState::Hovered);
		CurHoveredTile = HoveredTile;
		Grid->AddStateToTile(HoveredTile, ETileState::Hovered);
	}
}
