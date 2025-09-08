// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "TBOGPawnCamera.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class TURNBASEDONGRID_API ATBOGPawnCamera : public APawn
{
	GENERATED_BODY()

public:
	ATBOGPawnCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionValue& InputActionValue);
	void Zoom(const FInputActionValue& InputActionValue);
	void Rotation(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> CameraContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ZoomAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> RotationAction;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Scaler")
	float MinZoomLength = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Scaler")
	float MaxZoomLength = 1200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Scaler")
	float ZoomScaler = 40.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Scaler")
	float MoveScaler = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Scaler")
	float RotationDegree = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interp Speed")
	float ZoomInterpSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interp Speed")
	float LocationInterpSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interp Speed")
	float RotationInterpSpeed = 5.f;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

protected:
	UPROPERTY(BlueprintReadOnly)
	float DesiredZoomLength;

	UPROPERTY(BlueprintReadOnly)
	FVector DesiredLocation;

	UPROPERTY(BlueprintReadOnly)
	FRotator DesiredRotation;
};
