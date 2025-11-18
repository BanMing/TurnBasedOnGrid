// Copyright BanMing

#pragma once

#include "Action/ActionBase.h"
#include "CoreMinimal.h"

#include "Action_AddTile.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDONGRID_API AAction_AddTile : public AActionBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAction_Implementation(FIntPoint Index) override;
};
