// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_LightningDamageExec.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"

struct FLightningDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(OverCharge);
    DECLARE_ATTRIBUTE_CAPTUREDEF(Energy);
    //DECLARE_ATTRIBUTE_CAPTUREDEF

    FLightningDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, Energy, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UGB_AttributeSet, OverCharge, Source, false);
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
    
}
