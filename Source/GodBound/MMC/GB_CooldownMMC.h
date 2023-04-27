// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GB_CooldownMMC.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_CooldownMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	
	UGB_CooldownMMC();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
