// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GB_DamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_DamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	UGB_DamageExecCalc();
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
