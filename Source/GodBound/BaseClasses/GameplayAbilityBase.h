// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

	const FGameplayTagContainer* GetCooldownTags() const override;

	void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer CooldownTags;

	UPROPERTY()
	FGameplayTagContainer TempCooldownTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	float AbilityPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	float AbilityDuration;
};
