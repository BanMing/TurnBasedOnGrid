// Copyright BanMing

#include "Player/TBOGPawn.h"

// Sets default values
ATBOGPawn::ATBOGPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATBOGPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATBOGPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATBOGPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
