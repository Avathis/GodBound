// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_LightningDamageExec.h"

#include "GodBound/BaseClasses/GB_GameplayAbility.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"

struct FLightningDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(OverCharge);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
    //DECLARE_ATTRIBUTE_CAPTUREDEF

    FLightningDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Energy, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverCharge, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Health, Target, true);
    }
};


const FLightningDamageStatics& LightningDamageStatics()
{
    static FLightningDamageStatics Statics;
    return Statics;
}

UGB_LightningDamageExec::UGB_LightningDamageExec()
{
    RelevantAttributesToCapture.Add(LightningDamageStatics().EnergyDef);
    RelevantAttributesToCapture.Add(LightningDamageStatics().OverChargeDef);
}

void UGB_LightningDamageExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UGB_GameplayAbility* Ability = Cast<UGB_GameplayAbility>(ExecutionParams.GetOwningSpec().GetContext().GetAbilityInstance_NotReplicated());

    if(Ability)
    {
        const float AP = Ability->AbilityPower.GetValueAtLevel(Ability->GetAbilityLevel());

        UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();

        AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;
	
        const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

        const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();

        FAggregatorEvaluateParameters EvaluateParameters;
        EvaluateParameters.SourceTags = SourceTags;

        float OverCharge = 0.f;
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(LightningDamageStatics().OverChargeDef, EvaluateParameters, OverCharge);

        float Energy = 0.f;
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(LightningDamageStatics().EnergyDef, EvaluateParameters, Energy);

        float FinalDamage = AP;
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(LightningDamageStatics().HealthProperty, EGameplayModOp::Additive, FinalDamage));
        // OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(OverHeatStatics().OverHeatProperty, EGameplayModOp::Additive, OverHeatToAdd));
    }
}
