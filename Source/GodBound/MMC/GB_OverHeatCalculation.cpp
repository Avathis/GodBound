// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_OverHeatCalculation.h"
#include "GodBound/BaseClasses/Characters/Attributes/GB_AttributeSet.h"
/*
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
*/
UGB_OverHeatCalculation::UGB_OverHeatCalculation()
{
	//RelevantAttributesToCapture.Add(OverHeatStatics().HealthDef);
	//RelevantAttributesToCapture.Add(OverHeatStatics().OverHeatDef);
}

void UGB_OverHeatCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	Super::Execute_Implementation(ExecutionParams, OutExecutionOutput);
}
