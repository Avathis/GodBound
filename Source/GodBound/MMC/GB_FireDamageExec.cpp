// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_FireDamageExec.h"
#include "GodBound/BaseClasses/GB_GameplayAbility.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetMathLibrary.h"

struct FFireDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(OverHeat);
    DECLARE_ATTRIBUTE_CAPTUREDEF(AbilityPower);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
    DECLARE_ATTRIBUTE_CAPTUREDEF(HeatResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
    //DECLARE_ATTRIBUTE_CAPTUREDEF

    FFireDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, AbilityPower, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverHeat, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Health, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Armor, Target, true);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, HeatResistance, Target, true);

    }
};

const FFireDamageStatics& FireDamageStatics()
{
    static FFireDamageStatics Statics;
    return Statics;
}

UGB_FireDamageExec::UGB_FireDamageExec()
{
    RelevantAttributesToCapture.Add(FireDamageStatics().AbilityPowerDef);
    RelevantAttributesToCapture.Add(FireDamageStatics().OverHeatDef);
    RelevantAttributesToCapture.Add(FireDamageStatics().HealthDef);
    RelevantAttributesToCapture.Add(FireDamageStatics().ArmorDef);
    RelevantAttributesToCapture.Add(FireDamageStatics().HeatResistanceDef);
}

void UGB_FireDamageExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
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
	

    float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FireDamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float HeatResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FireDamageStatics().HeatResistanceDef, EvaluationParameters, HeatResistance);
	HeatResistance = FMath::Max<float>(HeatResistance, 0.0f);

	float OverHeat = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FireDamageStatics().OverHeatDef, EvaluationParameters, OverHeat);
	OverHeat = FMath::Max<float>(OverHeat, 0.0f);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FireDamageStatics().AbilityPowerDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Damage")), false, -1.0f), 0.0f);

	
	float UnmitigatedDamage = Damage; // Can multiply any damage boosters here

	if(SourceTags->HasTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Hephaestus.F.ArmsOfAshes"))))
	{
		UnmitigatedDamage*=1.15;
	}
	if (OverHeat > 100.f)
	{
		UnmitigatedDamage *= 1.5f;
	}

	float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor + HeatResistance));

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(FireDamageStatics().HealthProperty, EGameplayModOp::Additive, -MitigatedDamage));
	}
	//UE_LOG(LogTemp, Error, TEXT("%s"), MitigatedDamage)
	/*
	// Broadcast damages to Target ASC
	UGDAbilitySystemComponent* TargetASC = Cast<UGDAbilitySystemComponent>(TargetAbilitySystemComponent);
	if (TargetASC)
	{
		UGDAbilitySystemComponent* SourceASC = Cast<UGDAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetASC->ReceiveDamage(SourceASC, UnmitigatedDamage, MitigatedDamage);
	}*/
}