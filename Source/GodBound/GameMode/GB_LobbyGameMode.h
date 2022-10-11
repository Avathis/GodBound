// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_GameMode.h"
#include "GB_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_LobbyGameMode : public AGB_GameMode
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
};
