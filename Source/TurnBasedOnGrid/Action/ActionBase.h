// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ActionBase.generated.h"

class APlayerAction;

UCLASS()
class TURNBASEDONGRID_API AActionBase : public AActor
{
	GENERATED_BODY()

public:
	AActionBase();
	virtual void InitAction(APlayerAction* InPlayerAction);

public:
	UFUNCTION(BlueprintNativeEvent)
	void ExecuteAction(FIntPoint Index);
	virtual void ExecuteAction_Implementation(FIntPoint Index)
	{
	}

public:
	UPROPERTY(BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	TObjectPtr<APlayerAction> PlayerAction;
};
