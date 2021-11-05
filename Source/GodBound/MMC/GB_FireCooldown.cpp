// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_FireCooldown.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GodBound/BaseClasses/GB_GameplayAbility.h"

UGB_FireCooldown::UGB_FireCooldown()
{
	OverHeatDef.AttributeToCapture = UGB_AttributeSet::GetOverHeatAttribute();
	OverHeatDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	OverHeatDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(OverHeatDef);
}

float UGB_FireCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UGB_GameplayAbility* Ability = Cast<UGB_GameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	const float CD = Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel());
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTags;

	float OverHeat = 0.f;
	GetCapturedAttributeMagnitude(OverHeatDef, Spec, Params, OverHeat);

	if(OverHeat>100)
	{
		return 0.f;
	}
	return CD - CD*OverHeat/100;
}
