// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AbilitySystemLibrary.h"
#include "GodBound/GB_GameplayEffectTypes.h"

float UGB_AbilitySystemLibrary::GetEffectStrength(const FGameplayEffectContextHandle& EffectContext)
{
	const FGB_GameplayEffectContext* GBEffectContext = static_cast<const FGB_GameplayEffectContext*>(EffectContext.Get());
	if (GBEffectContext)
	{
		return GBEffectContext->GetEffectStrength();
	}
	return 0.f;
}
