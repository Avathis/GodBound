// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GB_AbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_AbilitySystemLibrary : public UAbilitySystemBlueprintLibrary
{
	GENERATED_BODY()
	
		UFUNCTION(BlueprintPure, Category = "KaosAbilityLibrary|Effects")
		static float GetEffectStrength(const FGameplayEffectContextHandle& EffectContext);
};
