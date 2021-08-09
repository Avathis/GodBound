// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayEffectExecutionCalculation.h"
#include "GodBound/BaseClasses/Attribtes/BaseAttributeSet.h"
#include "LightningEffectExecCalculation.generated.h"


/**
 * 
 */
UCLASS()
class GODBOUND_API ULightningEffectExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	ULightningEffectExecCalculation();

	//static const FLightningDamageStatics& LightningDamageStatics();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
