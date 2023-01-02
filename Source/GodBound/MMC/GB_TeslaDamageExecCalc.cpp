// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_TeslaDamageExecCalc.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"

struct FTeslaDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(OverCharge);
	DECLARE_ATTRIBUTE_CAPTUREDEF(OverChargeMax);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AbilityPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	//DECLARE_ATTRIBUTE_CAPTUREDEF

	FTeslaDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, AbilityPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverCharge, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverChargeMax, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Health, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Armor, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, LightningResistance, Target, true);

	}
};

UGB_TeslaDamageExecCalc::UGB_TeslaDamageExecCalc()
{
	RelevantAttributesToCapture.Add(FTeslaDamageStatics().AbilityPowerDef);
	RelevantAttributesToCapture.Add(FTeslaDamageStatics().OverChargeDef);
	RelevantAttributesToCapture.Add(FTeslaDamageStatics().OverChargeMaxDef);
	RelevantAttributesToCapture.Add(FTeslaDamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(FTeslaDamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(FTeslaDamageStatics().LightningResistanceDef);
}

void UGB_TeslaDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FTeslaDamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float LightningResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FTeslaDamageStatics().LightningResistanceDef, EvaluationParameters, LightningResistance);
	LightningResistance = FMath::Max<float>(LightningResistance, 0.0f);

	float OverCharge = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FTeslaDamageStatics().OverChargeDef, EvaluationParameters, OverCharge);
	OverCharge = FMath::Max<float>(OverCharge, 0.0f);

	float OverChargeMax = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FTeslaDamageStatics().OverChargeMaxDef, EvaluationParameters, OverChargeMax);
	OverChargeMax = FMath::Max<float>(OverChargeMax, 0.0f);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FTeslaDamageStatics().AbilityPowerDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage")), false, -1.0f), 0.0f);

	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here

	
	UnmitigatedDamage *= (1+(OverCharge/OverChargeMax)*0.2f);

	float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor + LightningResistance));

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(FTeslaDamageStatics().HealthProperty, EGameplayModOp::Additive, -MitigatedDamage));
	}
}
