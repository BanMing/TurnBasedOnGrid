// Copyright BanMing

#pragma once

#include "Action/ActionBase.h"
#include "CoreMinimal.h"

#include "Action_SelectTile.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDONGRID_API AAction_SelectTile : public AActionBase
{
	GENERATED_BODY()
public:
	virtual void ExecuteAction_Implementation(FIntPoint Index) override;

	 virtual void Destroyed() override;
};
