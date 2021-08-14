// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "FireCooldown.generated.h"


/**
 * 
 */
UCLASS()
class GODBOUND_API UFireCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	UFireCooldown();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition OverHeatDef;

	FGameplayEffectAttributeCaptureDefinition HealthDef;
};
