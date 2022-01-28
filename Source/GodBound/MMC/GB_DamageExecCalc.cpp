// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_DamageExecCalc.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"

struct FDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Stamina);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(OverHeat);

	FDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Stamina, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Energy, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Health, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverHeat, Source, false);
	}
};

const FDamageStatics& DamageStatics()
{
	static FDamageStatics Statics;
	return Statics;
}

UGB_DamageExecCalc::UGB_DamageExecCalc()
{
/*
	RelevantAttributesToCapture();
	RelevantAttributesToCapture();
	RelevantAttributesToCapture();
*/
}


void UGB_DamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	
}
