// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "TBOGPlayerController.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDONGRID_API ATBOGPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
};
