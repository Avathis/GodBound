// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayEffectExecCalculation.h"
#include "Attribtes/BaseAttributeSet.h"
#include "AbilitySystemComponent.h"

UGameplayEffectExecCalculation::UGameplayEffectExecCalculation()
{
}

static const FMeleeDamageStatics& MeleeDamageStatics()
{
	static FMeleeDamageStatics DmgStatics;
	return DmgStatics;
}

void UGameplayEffectExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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

	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(MeleeDamageStatics().)
	//DEFINE_ATTRIBUTE_CAPTUREDEF()
}
