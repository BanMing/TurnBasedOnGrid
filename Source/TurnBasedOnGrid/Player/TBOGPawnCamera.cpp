// Copyright BanMing

#include "Player/TBOGPawnCamera.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"

ATBOGPawnCamera::ATBOGPawnCamera()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATBOGPawnCamera::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CameraContext, 0);
		}
	}

	DesiredZoomLength = SpringArmComponent->TargetArmLength;
	DesiredLocation = GetActorLocation();
	DesiredRotation = GetActorRotation();
}

void ATBOGPawnCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SpringArmComponent->TargetArmLength = FMath::FInterpTo(SpringArmComponent->TargetArmLength, DesiredZoomLength, DeltaTime, ZoomInterpSpeed);
	SetActorLocation(FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaTime, LocationInterpSpeed));
	SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaTime, RotationInterpSpeed));
}

void ATBOGPawnCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ThisClass::Zoom);
		EnhancedInputComponent->BindAction(RotationAction, ETriggerEvent::Started, this, &ThisClass::Rotation);
	}
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATBOGPawnCamera::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	DesiredLocation = GetActorLocation() + GetActorForwardVector() * InputAxisVector.Y * MoveScaler + GetActorRightVector() * InputAxisVector.X * MoveScaler;
}

void ATBOGPawnCamera::Zoom(const FInputActionValue& InputActionValue)
{
	const float InputValue = InputActionValue.Get<float>();
	DesiredZoomLength = FMath::Clamp(DesiredZoomLength + InputValue * ZoomScaler, MinZoomLength, MaxZoomLength);
}

void ATBOGPawnCamera::Rotation(const FInputActionValue& InputActionValue)
{
	const float InputValue = InputActionValue.Get<float>();
	DesiredRotation.Yaw += InputValue > 0 ? RotationDegree : -RotationDegree;
	DesiredRotation.Normalize();
}
