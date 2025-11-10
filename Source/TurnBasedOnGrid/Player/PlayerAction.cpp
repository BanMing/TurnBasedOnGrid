// Copyright BanMing

#include "Player/PlayerAction.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Grid/GridBase.h"
#include "InputActionValue.h"
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

	SetupInput();
}

void APlayerAction::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ShotdownInput();
	Super::EndPlay(EndPlayReason);
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

void APlayerAction::SetupInput()
{
	check(InputContext);
	check(SelectAction);
	check(UnSelectAction);
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (!PC)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		if (InputContext)
		{
			Subsystem->AddMappingContext(InputContext, 0);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
	{
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, this, &ThisClass::OnSelectTile);
		EnhancedInputComponent->BindAction(UnSelectAction, ETriggerEvent::Started, this, &ThisClass::OnUnSelectTile);
	}
}

void APlayerAction::ShotdownInput()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);

	if (!PC)
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		if (InputContext)
		{
			Subsystem->RemoveMappingContext(InputContext);
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PC->InputComponent))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
	}
}

void APlayerAction::OnSelectTile(const FInputActionValue& InputActionValue)
{
	if (!Grid)
	{
		return;
	}
	FIntPoint HoveredTile = Grid->GetTileIndexUnderCursor();
	if (!Grid->HasStateInTile(HoveredTile, ETileState::Selected))
	{
		Grid->AddStateToTile(HoveredTile, ETileState::Selected);
	}
}

void APlayerAction::OnUnSelectTile(const FInputActionValue& InputActionValue)
{
	if (!Grid)
	{
		return;
	}
	FIntPoint HoveredTile = Grid->GetTileIndexUnderCursor();
	Grid->RemoveStateFromTile(HoveredTile, ETileState::Selected);
}
