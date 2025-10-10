// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LevelLoading.generated.h"

UCLASS()
class TURNBASEDONGRID_API ALevelLoading : public AActor
{
	GENERATED_BODY()

public:
	ALevelLoading();

public:
	UFUNCTION(BlueprintCallable)
	void LoadLevel(FName Level, bool bForce = false);
	
	UFUNCTION(BlueprintCallable)
	void LoadLevelByDefault();

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FString LevelLoaded;
};
