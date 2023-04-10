// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_DamageExecCalc.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(OverHeat);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AbilityPower);
	
	DECLARE_ATTRIBUTE_CAPTUREDEF(HeatResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ColdResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Energy, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Health, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverHeat, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, AbilityPower, Source, false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, HeatResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, ColdResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, PhysicalResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Armor, Target, false);
		
	}
};

const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UGB_DamageExecCalc::UGB_DamageExecCalc()
{

	RelevantAttributesToCapture.Add(FDamageStatics().HealthDef);
	RelevantAttributesToCapture.Add(FDamageStatics().AbilityPowerDef);
	RelevantAttributesToCapture.Add(FDamageStatics().EnergyDef);
	RelevantAttributesToCapture.Add(FDamageStatics().OverHeatDef);
	
	RelevantAttributesToCapture.Add(FDamageStatics().HeatResistanceDef);
	RelevantAttributesToCapture.Add(FDamageStatics().ColdResistanceDef);
	RelevantAttributesToCapture.Add(FDamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(FDamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(FDamageStatics().ArmorDef);

}


void UGB_DamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	

	FVector TargetForwardVector = TargetActor->GetActorForwardVector();
	FVector SourceForwardVector;
	
	
	

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	
	
	float FireDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Fire")), false, -1.0f), 0.0f);
	if(FireDamage<0)
	{
		float HeatResistance = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().HeatResistanceDef, EvaluationParameters, HeatResistance);
		HeatResistance = FMath::Max<float>(HeatResistance, 0.0f);
		if(HeatResistance<0)
		{
			FireDamage-=HeatResistance;
		}
	}

	float FrostDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Frost")), false, -1.0f), 0.0f);
	if(FrostDamage<0)
	{
		float ColdResistance = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().ColdResistanceDef, EvaluationParameters, ColdResistance);
		ColdResistance = FMath::Max<float>(ColdResistance, 0.0f);
		if(ColdResistance<0)
		{
			FrostDamage-=ColdResistance;
		}
	}

	float LightningDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Lightning")), false, -1.0f), 0.0f);
	if(LightningDamage<0)
	{
		float LightningResistance = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().LightningResistanceDef, EvaluationParameters, LightningResistance);
		LightningResistance = FMath::Max<float>(LightningResistance, 0.0f);
		if(LightningResistance<0)
		{
			LightningDamage-=LightningResistance;
		}
	}

	float PhysicalDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage.Physical")), false, -1.0f), 0.0f);
	if(PhysicalDamage<0)
	{
		float PhysicalResistance = 0.0f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().PhysicalResistanceDef, EvaluationParameters, PhysicalResistance);
		PhysicalResistance = FMath::Max<float>(PhysicalResistance, 0.0f);
		if(PhysicalResistance<0)
		{
			PhysicalDamage-=PhysicalResistance;
		}
	}
	
	float Armor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float MitigatedDamage = (FireDamage+FrostDamage+LightningDamage+PhysicalDamage)*(100 / (100 + Armor));
	
	
	
	if(TargetTags->HasTagExact(FGameplayTag::RequestGameplayTag(FName("State.Blocking"))))
	{
		return;
	}
	if(TargetTags->HasTagExact(FGameplayTag::RequestGameplayTag(FName("State.Blocking.Frontal"))))
	{
		if(SourceTags->HasTagExact(FGameplayTag::RequestGameplayTag(FName("GameplayEffect.Melee"))))
		{
			SourceForwardVector = UKismetMathLibrary::FindLookAtRotation(SourceActor->GetActorLocation(),TargetActor->GetActorLocation()).Quaternion().GetForwardVector();
		}
		else if(Spec.GetContext().HasOrigin())
		{
		
			SourceForwardVector = UKismetMathLibrary::FindLookAtRotation(Spec.GetContext().GetOrigin(),TargetActor->GetActorLocation()).Quaternion().GetForwardVector();
		}
		else
		{
			SourceForwardVector = UKismetMathLibrary::FindLookAtRotation(SourceActor->GetActorLocation(),TargetActor->GetActorLocation()).Quaternion().GetForwardVector();
		}
		if(FVector::DotProduct(TargetForwardVector, SourceForwardVector)<=0)
		{
			return;
		}
	}
	

	float OverHeat = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FDamageStatics().OverHeatDef, EvaluationParameters, OverHeat);
	OverHeat = FMath::Max<float>(OverHeat, 0.0f);

	if(SourceTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Hephaestus.F.ArmsOfAshes"))))
	{
		MitigatedDamage*=1.15;
	}
	if (OverHeat > 100.f)
	{
		MitigatedDamage *= 1.5f;
	}
	
	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(FDamageStatics().HealthProperty, EGameplayModOp::Additive, -MitigatedDamage));
	}
	
}
