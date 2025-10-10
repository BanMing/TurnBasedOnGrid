// Copyright BanMing

#include "Actor/LevelLoading.h"

#include "Kismet/GameplayStatics.h"
#include "TurnBasedOnGrid.h"

ALevelLoading::ALevelLoading()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALevelLoading::LoadLevel(FName Level, bool bForce)
{
	if (LevelLoaded.Equals(Level.ToString()) && !bForce)
	{
		return;
	}
	// Unload
	if (ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this, FName(LevelLoaded)))
	{
		LevelStreaming->SetShouldBeLoaded(false);
		LevelStreaming->SetShouldBeVisible(false);
	}

	// Load
	if (ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this, Level))
	{
		LevelLoaded = Level.ToString();
		LevelStreaming->SetShouldBeLoaded(true);
		LevelStreaming->SetShouldBeVisible(true);
	}
	else
	{
		UE_LOG(LogGrid, Error, TEXT("Can't load level %s"), *Level.ToString());
	}
}

void ALevelLoading::LoadLevelByDefault()
{
	LoadLevel(FName(LevelLoaded), true);
}
