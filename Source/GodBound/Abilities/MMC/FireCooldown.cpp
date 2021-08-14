// Fill out your copyright notice in the Description page of Project Settings.


#include "FireCooldown.h"
#include "GodBound/BaseClasses/Attribtes/BaseAttributeSet.h"
#include "GodBound/BaseClasses/GameplayAbilityBase.h"

UFireCooldown::UFireCooldown()
{
	OverHeatDef.AttributeToCapture = UBaseAttributeSet::GetOverHeatAttribute();
	OverHeatDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	OverHeatDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(OverHeatDef);
}

float UFireCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UGameplayAbilityBase* Ability = Cast<UGameplayAbilityBase>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	float CD = Ability->CooldownDuration;
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters Params;
	Params.SourceTags = SourceTags;

	float OverHeat = 0.f;
	GetCapturedAttributeMagnitude(OverHeatDef, Spec, Params, OverHeat);

	return OverHeat/Ability->CooldownDuration;
}

