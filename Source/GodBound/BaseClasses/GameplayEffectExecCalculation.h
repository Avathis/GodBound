// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "Attribtes/BaseAttributeSet.h"

#include "GameplayEffectExecCalculation.generated.h"

/**
 * 
 */
struct FMeleeDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FMeleeDamageStatics& MeleeDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Stamina, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Energy, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Target, false);
	}
};

UCLASS()
class GODBOUND_API UGameplayEffectExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	public:
	UGameplayEffectExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
