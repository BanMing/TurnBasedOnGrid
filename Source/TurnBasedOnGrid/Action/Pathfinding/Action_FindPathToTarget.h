// Copyright BanMing

#pragma once

#include "Action/ActionBase.h"
#include "CoreMinimal.h"

#include "Action_FindPathToTarget.generated.h"

/**
 *
 */
UCLASS()
class TURNBASEDONGRID_API AAction_FindPathToTarget : public AActionBase
{
	GENERATED_BODY()

public:
	virtual void ExecuteAction_Implementation(FIntPoint Index) override;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

public:
	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	bool bIncludeDiagonals;
};
