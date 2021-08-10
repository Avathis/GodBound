// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/Characters/PlayableCharacterBase.h"
#include "Hephaestus.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AHephaestus : public APlayableCharacterBase
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintNativeEvent)
	void ReleaseFireBall();
	void ReleaseFireBall_Implementation();
};
