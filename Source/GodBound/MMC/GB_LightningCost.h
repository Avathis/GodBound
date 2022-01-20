// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "GB_LightningCost.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_LightningCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	public:
	UGB_LightningCost();
	float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec)const override;

	FGameplayEffectAttributeCaptureDefinition OverChargeDef;
};
