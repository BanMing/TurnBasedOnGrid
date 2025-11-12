// Copyright BanMing

#include "Action/ActionBase.h"

AActionBase::AActionBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AActionBase::InitAction(APlayerAction* InPlayerAction)
{
	check(InPlayerAction);
	PlayerAction = InPlayerAction;
}
