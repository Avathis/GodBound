// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AbilitySystemGlobals.h"
#include "GB_GameplayEffectTypes.h"

FGameplayEffectContext* UGB_AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGB_GameplayEffectContext();
}
