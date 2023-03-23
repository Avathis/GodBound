// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GodBound/GB_GameplayEffectTypes.h"
#include "GB_GameplayAbility.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGB_AbilityActivationPolicy : uint8
{
	OnInputTriggered,
	
	WhileInputActive,
	
	OnSpawn
};

UENUM(BlueprintType)
enum class EGB_AbilityActivationGroup : uint8
{
	Independent,
	
	Exclusive_Replaceable,
	
	Exclusive_Blocking,

	MAX	UMETA(Hidden)
};


UENUM(BlueprintType)
enum class AbilityUpgrades : uint8
{
	Normal
};

UCLASS()
class GODBOUND_API UGB_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	public:

	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle SetContextEffectStrength(float EffectStrength, FGameplayEffectSpecHandle GameplayEffectSpec);

	EGB_AbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }
	EGB_AbilityActivationGroup GetActivationGroup() const { return ActivationGroup; }
	
	UGB_GameplayAbility();
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	FScalableFloat CooldownDuration;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer CooldownTags;

	UPROPERTY()
	FGameplayTagContainer TempCooldownTags;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Cost", meta = (AllowPrivateAccess = "true"))
	FScalableFloat AbilityCost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	FScalableFloat AbilityPower;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	FScalableFloat AbilityDuration;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	//class AGB_Character* PlayerCharacter;

	UFUNCTION(BlueprintCallable)
		float GetGameplayEffectSpecHandle();

	void OnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
	{
		NativeOnAbilityFailedToActivate(FailedReason);
		ScriptOnAbilityFailedToActivate(FailedReason);
	}

protected:
	virtual void NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ScriptOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGB_AbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EGB_AbilityActivationGroup ActivationGroup;
};
