// Fill out your copyright notice in the Description page of Project Settings.


#include "LightningEffectExecCalculation.h"

ULightningEffectExecCalculation::ULightningEffectExecCalculation()
{
	RelevantAttributesToCapture.Add(LightningDamageStatics().AbilityPowerDef);
	RelevantAttributesToCapture.Add(LightningDamageStatics().EnergyDef);
}

const FLightningDamageStatics& ULightningEffectExecCalculation::LightningDamageStatics()
{
	static FLightningDamageStatics Statics;
	return Statics;
}


void ULightningEffectExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
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
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FLightningDamageStatics().AbilityPowerDef, EvaluateParameters,BaseDamage);

	float Electricity = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FLightningDamageStatics().EnergyDef, EvaluateParameters, Electricity);

	float DamageDone = BaseDamage+Electricity;

	DamageDone*=-1.f;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(LightningDamageStatics().HealthProperty,EGameplayModOp::Additive,DamageDone));
}
