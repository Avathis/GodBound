// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_LightningCost.h"

#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GodBound/Characters/Zeus/GB_LightningAbility.h"

UGB_LightningCost::UGB_LightningCost()
{
    OverChargeDef.AttributeToCapture = UGB_AttributeSet::GetOverChargeAttribute();
    OverChargeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
    OverChargeDef.bSnapshot = false;

    OverChargeMaxDef.AttributeToCapture = UGB_AttributeSet::GetOverChargeMaxAttribute();
    OverChargeMaxDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
    OverChargeMaxDef.bSnapshot = false;
    
    RelevantAttributesToCapture.Add(OverChargeDef);
    RelevantAttributesToCapture.Add(OverChargeMaxDef);
    
}


float UGB_LightningCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
    const UGB_LightningAbility* Ability = Cast<UGB_LightningAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

    const float Cost = Ability->AbilityCost.GetValueAtLevel(Ability->GetAbilityLevel());

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

    FAggregatorEvaluateParameters Params;
    
    Params.SourceTags = SourceTags;
    
    float OverCharge = 0.f;
    GetCapturedAttributeMagnitude(OverChargeDef,Spec,Params, OverCharge);

    float OverChargeMax = 0.f;
    GetCapturedAttributeMagnitude(OverChargeMaxDef,Spec,Params, OverChargeMax);
    
    return Cost+OverCharge/OverChargeMax*Cost;
}
