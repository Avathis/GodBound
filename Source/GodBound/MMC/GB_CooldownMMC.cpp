// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_CooldownMMC.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GodBound/BaseClasses/GB_GameplayAbility.h"

UGB_CooldownMMC::UGB_CooldownMMC()
{
}

float UGB_CooldownMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UGB_GameplayAbility* Ability = Cast<UGB_GameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());
    
	const float CD = Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel());
    	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    
	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTags;

	return CD;
}
