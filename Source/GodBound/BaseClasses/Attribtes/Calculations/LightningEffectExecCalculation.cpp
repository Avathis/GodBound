// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningEffectExecCalculation.h"

struct FLightningDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AbilityPower);

	FLightningDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Stamina, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Energy, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Health, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, AbilityPower, Source, false);
	}
};

const FLightningDamageStatics& LightningDamageStatics()
{
	static FLightningDamageStatics Statics;
	return Statics;
}

ULightningEffectExecCalculation::ULightningEffectExecCalculation()
{
	RelevantAttributesToCapture.Add(LightningDamageStatics().AbilityPowerDef);
	RelevantAttributesToCapture.Add(LightningDamageStatics().EnergyDef);
	RelevantAttributesToCapture.Add(LightningDamageStatics().HealthDef);
}

void ULightningEffectExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	UAbilitySystemComponent* TargetABSC = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetABSC ? TargetABSC->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float BaseDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(LightningDamageStatics().AbilityPowerDef, EvaluateParameters,BaseDamage);

	float Electricity = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(LightningDamageStatics().EnergyDef, EvaluateParameters, Electricity);

	float DamageDone = BaseDamage-Electricity;

	DamageDone*=-1.f;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(LightningDamageStatics().HealthProperty,EGameplayModOp::Additive,DamageDone));
	UE_LOG(LogTemp, Warning, TEXT("ClassFinished %f"), DamageDone);
}
