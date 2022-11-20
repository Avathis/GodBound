// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_GameplayAbility.h"
#include "GB_PassiveAbility.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_PassiveAbility : public UGB_GameplayAbility
{
	GENERATED_BODY()
	public:
    virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec);
};
