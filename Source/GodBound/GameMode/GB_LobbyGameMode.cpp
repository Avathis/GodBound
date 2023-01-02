// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_LobbyGameMode.h"
#include "GB_GameState.h"

void AGB_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumOfPlayers = GameState.Get()->PlayerArray.Num();
	if(NumOfPlayers==2)
	{
		UWorld* World = GetWorld();
		if(World)
		{
			bUseSeamlessTravel = true;
			World->ServerTravel(FString("/Game/Maps/CombatMap?listen"));
		}
	}
}
