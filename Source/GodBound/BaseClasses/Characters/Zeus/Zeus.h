// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/Characters/PlayableCharacterBase.h"
#include "Zeus.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AZeus : public APlayableCharacterBase
{
	GENERATED_BODY()
	FVector LightningStrike();
protected:
	void BeginPlay() override;
public:
	void Tick(float DeltaSeconds) override;
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
};
