// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayEffectExecutionCalculation.h"
#include "GodBound/BaseClasses/Attribtes/BaseAttributeSet.h"
#include "LightningEffectExecCalculation.generated.h"

struct FLightningDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AbilityPower);

	FLightningDamageStatics& MeleeDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Stamina, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Energy, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, AbilityPower, Target, false);
	}
};
/**
 * 
 */
UCLASS()
class GODBOUND_API ULightningEffectExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	ULightningEffectExecCalculation();

	static const FLightningDamageStatics& LightningDamageStatics();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
