// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include <GameplayAbilities/Public/AbilitySystemInterface.h>
#include "GB_AbilitySet.generated.h"


class UGB_AbilitySystemComponent;
class UGB_GameplayAbility;
class UGameplayEffect;
 /**
  * FGB_AbilitySet_GameplayAbility
  *
  *	Data used by the ability set to grant gameplay abilities.
  */
USTRUCT(BlueprintType)
struct FGBAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// Gameplay ability to grant.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGB_GameplayAbility> Ability = nullptr;

	// Level of ability to grant.
	UPROPERTY(EditDefaultsOnly)
		int32 AbilityLevel = 1;

	// Tag used to process input for the ability.
	UPROPERTY(EditDefaultsOnly, Meta = (Categories = "InputTag"))
		FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly)
		int32 InputID;
};


/**
 * FGB_AbilitySet_GameplayEffect
 *
 *	Data used by the ability set to grant gameplay effects.
 */
USTRUCT(BlueprintType)
struct FGBAbilitySet_GameplayEffect
{
	GENERATED_BODY()

public:

	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UGameplayEffect> GameplayEffect = nullptr;

	// Level of gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
		float EffectLevel = 1.0f;
};

/**
 * FGB_AbilitySet_AttributeSet
 *
 *	Data used by the ability set to grant attribute sets.
 */
USTRUCT(BlueprintType)
struct FGBAbilitySet_AttributeSet
{
	GENERATED_BODY()

public:
	// Gameplay effect to grant.
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UAttributeSet> AttributeSet;

};

/**
 * FGB_AbilitySet_GrantedHandles
 *
 *	Data used to store handles to what has been granted by the ability set.
 */
USTRUCT(BlueprintType)
struct FGBAbilitySet_GrantedHandles
{
	GENERATED_BODY()

public:

	bool IsValid() const
	{
		return AbilitySystemComponent.IsValid();
	}

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle);
	void AddAttributeSet(UAttributeSet* Set);

	void TakeFromAbilitySystem(UGB_AbilitySystemComponent* GB_ASC);
	TWeakObjectPtr<UGB_AbilitySystemComponent> AbilitySystemComponent = nullptr;

	
	void Reset()
	{
		AbilitySpecHandles.Reset();
		GameplayEffectHandles.Reset();
		GrantedAttributeSets.Reset();
		AbilitySystemComponent.Reset();
	}

	// Handles to the granted abilities.
	UPROPERTY()
		TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

	// Handles to the granted gameplay effects.
	UPROPERTY()
		TArray<FActiveGameplayEffectHandle> GameplayEffectHandles;

	// Pointers to the granted attribute sets
	UPROPERTY()
		TArray<TObjectPtr<UAttributeSet>> GrantedAttributeSets;
};

UCLASS(BlueprintType, Const)
class GODBOUND_API UGB_AbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	UGB_AbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Grants the ability set to the specified ability system component.
	// The returned handles can be used later to take away anything that was granted.
	FGBAbilitySet_GrantedHandles GiveToAbilitySystem(UGB_AbilitySystemComponent* GBASC, UObject* SourceObject = nullptr) const;
	FGBAbilitySet_GrantedHandles GiveAbilitySetToInterface(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface, UObject* OverrideSourceObject = nullptr) const;

	static void TakeAbilitySet(FGBAbilitySet_GrantedHandles& AbilitySetHandle);
	//UFUNCTION(BlueprintCallable, DisplayName = "GiveSetToAbilitySystem", meta = (ScriptName = "GiveSetToAbilitySystem"))
	//FGBAbilitySet_GrantedHandles GiveSetToAbilitySystem(UGB_AbilitySystemComponent* GBASC, UObject* SourceObject = nullptr);
protected:

	// Gameplay abilities to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Abilities", meta = (TitleProperty = Ability))
		TArray<FGBAbilitySet_GameplayAbility> GrantedGameplayAbilities;

	// Gameplay effects to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay Effects", meta = (TitleProperty = GameplayEffect))
		TArray<FGBAbilitySet_GameplayEffect> GrantedGameplayEffects;

	// Attribute sets to grant when this ability set is granted.
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Sets", meta = (TitleProperty = AttributeSet))
		TArray<FGBAbilitySet_AttributeSet> GrantedAttributes;
};
