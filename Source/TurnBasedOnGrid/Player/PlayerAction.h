// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PlayerAction.generated.h"

class UInputMappingContext;
class AGridBase;
class UInputAction;
struct FInputActionValue;

UCLASS()
class TURNBASEDONGRID_API APlayerAction : public AActor
{
	GENERATED_BODY()

public:
	APlayerAction();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateTileUnderCursor();

protected:
	void SetupInput();
	void OnSelectTile(const FInputActionValue& InputActionValue);
	void OnUnSelectTile(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> UnSelectAction;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridBase> Grid;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint CurHoveredTile;
};
