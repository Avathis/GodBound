// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GB_AbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

		virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
