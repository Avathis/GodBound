// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GB_FireCooldown.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_FireCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	UGB_FireCooldown();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	FGameplayEffectAttributeCaptureDefinition OverHeatDef;

	FGameplayEffectAttributeCaptureDefinition HealthDef;
};
