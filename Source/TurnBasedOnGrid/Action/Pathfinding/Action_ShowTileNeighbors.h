// Copyright BanMing

#pragma once

#include "Action/ActionBase.h"
#include "CoreMinimal.h"

#include "Action_ShowTileNeighbors.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDONGRID_API AAction_ShowTileNeighbors : public AActionBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAction_Implementation(FIntPoint Index) override;

	virtual void Destroyed() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	bool bIncludeDiagonals;
};
