// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_OverHeatCalculation.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"

struct FOverHeatStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(OverHeat);

	FOverHeatStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Stamina, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Energy, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Health, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverHeat, Source, false);
	}
};

const FOverHeatStatics& OverHeatStatics()
{
	static FOverHeatStatics Statics;
	return Statics;
}

UGB_OverHeatCalculation::UGB_OverHeatCalculation()
{
	RelevantAttributesToCapture.Add(OverHeatStatics().HealthDef);
	RelevantAttributesToCapture.Add(OverHeatStatics().OverHeatDef);
}

void UGB_OverHeatCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceABSC = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceABSC ? SourceABSC->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	
	float BaseHeal = 10.0f;

	float OverHeat = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(OverHeatStatics().OverHeatDef, EvaluateParameters, OverHeat);

	float OverHeatToAdd = OverHeat/10;
	
	BaseHeal -= OverHeatToAdd;

	OverHeat += OverHeatToAdd;
	

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(OverHeatStatics().HealthProperty, EGameplayModOp::Additive, BaseHeal));
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(OverHeatStatics().OverHeatProperty, EGameplayModOp::Additive, OverHeatToAdd));
	UE_LOG(LogTemp, Warning, TEXT("Healed %f"), BaseHeal);
	UE_LOG(LogTemp, Warning, TEXT("OverHeat %f"), OverHeatToAdd);
}
