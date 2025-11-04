// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PlayerAction.generated.h"
class AGridBase;

UCLASS()
class TURNBASEDONGRID_API APlayerAction : public AActor
{
	GENERATED_BODY()

public:
	APlayerAction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateTileUnderCursor();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridBase> Grid;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint CurHoveredTile;
};
