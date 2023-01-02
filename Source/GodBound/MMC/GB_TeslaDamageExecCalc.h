// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GB_TeslaDamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_TeslaDamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UGB_TeslaDamageExecCalc();
	void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

};
