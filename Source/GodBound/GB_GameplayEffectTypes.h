// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "GB_GameplayEffectTypes.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct GODBOUND_API FGB_GameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()
		
public:
	float GetEffectStrength() const { return EffectStrength; };

	void SetEffectStrength(float EffectStrengthToSet) { EffectStrength = EffectStrengthToSet; };
protected:
	float EffectStrength;

public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
    
    virtual FGB_GameplayEffectContext* Duplicate() const override
    {
        FGB_GameplayEffectContext* NewContext = new FGB_GameplayEffectContext();
        *NewContext = *this;
        NewContext->AddActors(Actors);
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        return NewContext;
    }

    virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;


};

template <>
struct TStructOpsTypeTraits<FGB_GameplayEffectContext> : public TStructOpsTypeTraitsBase2<FGB_GameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true // Necessary so that TSharedPtr<FHitResult> Data is copied around
    };
};