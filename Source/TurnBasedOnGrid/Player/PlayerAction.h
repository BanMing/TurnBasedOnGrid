// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PlayerAction.generated.h"

class UInputMappingContext;
class AGridBase;
class UInputAction;
class AActionBase;
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

	UFUNCTION(BlueprintCallable)
	void SetClickActions(TSubclassOf<AActionBase> LeftAction, TSubclassOf<AActionBase> RightAction);
	
	UFUNCTION(BlueprintCallable)
	void SetTriggerActions(TSubclassOf<AActionBase> LeftAction, TSubclassOf<AActionBase> RightAction);

protected:
	void SetupInput();
	void ShotdownInput();
	void OnLeftClicked(const FInputActionValue& InputActionValue);
	void OnRightClicked(const FInputActionValue& InputActionValue);

	void OnLeftTriggered(const FInputActionValue& InputActionValue);
	void OnRightTriggered(const FInputActionValue& InputActionValue);

public:
	UPROPERTY(BlueprintReadOnly)
	FIntPoint CurHoveredTile;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint CurSelectTile;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AGridBase> Grid;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> SelectAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> UnSelectAction;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActionBase> SelectActionLeftClick;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActionBase> SelectActionRightClick;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActionBase> SelectActionLeftTrigger;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActionBase> SelectActionRightTrigger;
};
