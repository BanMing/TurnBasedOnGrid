// Copyright BanMing

#include "Player/PlayerAction.h"

#include "Action/ActionBase.h"
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

void APlayerAction::SetSelectActions(TSubclassOf<AActionBase> LeftAction, TSubclassOf<AActionBase> RightAction)
{
	if (SelectActionLeftClick)
	{
		SelectActionLeftClick->Destroy();
		SelectActionLeftClick = nullptr;
	}

	if (LeftAction.Get() && GetWorld())
	{
		SelectActionLeftClick = GetWorld()->SpawnActorDeferred<AActionBase>(LeftAction, FTransform::Identity, this);
		SelectActionLeftClick->InitAction(this);
		SelectActionLeftClick->FinishSpawning(FTransform::Identity);
	}

	if (SelectActionRightClick)
	{
		SelectActionRightClick->Destroy();
		SelectActionRightClick = nullptr;
	}

	if (RightAction.Get() && GetWorld())
	{
		SelectActionRightClick = GetWorld()->SpawnActorDeferred<AActionBase>(LeftAction, FTransform::Identity, this);
		SelectActionRightClick->InitAction(this);
		SelectActionRightClick->FinishSpawning(FTransform::Identity);
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
	if (SelectActionLeftClick)
	{
		FIntPoint HoveredTile = Grid->GetTileIndexUnderCursor();
		SelectActionLeftClick->ExecuteAction(HoveredTile);
	}
}

void APlayerAction::OnUnSelectTile(const FInputActionValue& InputActionValue)
{
	if (SelectActionRightClick)
	{
		FIntPoint HoveredTile = Grid->GetTileIndexUnderCursor();
		SelectActionRightClick->ExecuteAction(HoveredTile);
	}
}