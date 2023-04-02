// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AbilitySet.h"
#include "GB_GameplayAbility.h"
#include "Components/GB_AbilitySystemComponent.h"


UGB_AbilitySet::UGB_AbilitySet(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

FGBAbilitySet_GrantedHandles UGB_AbilitySet::GiveToAbilitySystem(UGB_AbilitySystemComponent* GBASC, UObject* SourceObject) const
{
	check(GBASC);

	if (!GBASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return FGBAbilitySet_GrantedHandles();
	}

	FGBAbilitySet_GrantedHandles OutGrantedHandles;
	OutGrantedHandles.AbilitySystemComponent = GBASC;

	// Grant the gameplay abilities.
	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++AbilityIndex)
	{
		const FGBAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];

		if (!IsValid(AbilityToGrant.Ability))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayAbilities[%d] on ability set [%s] is not valid."), AbilityIndex, *GetNameSafe(this));
			continue;
		}

		UGB_GameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UGB_GameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);
		AbilitySpec.InputID = AbilityToGrant.InputID;

		const FGameplayAbilitySpecHandle AbilitySpecHandle = GBASC->GiveAbility(AbilitySpec);
		OutGrantedHandles.AddAbilitySpecHandle(AbilitySpecHandle);
	}

	// Grant the gameplay effects.
	for (int32 EffectIndex = 0; EffectIndex < GrantedGameplayEffects.Num(); ++EffectIndex)
	{
		const FGBAbilitySet_GameplayEffect& EffectToGrant = GrantedGameplayEffects[EffectIndex];

		if (!IsValid(EffectToGrant.GameplayEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedGameplayEffects[%d] on ability set [%s] is not valid"), EffectIndex, *GetNameSafe(this));
			continue;
		}

		const UGameplayEffect* GameplayEffect = EffectToGrant.GameplayEffect->GetDefaultObject<UGameplayEffect>();
		const FActiveGameplayEffectHandle GameplayEffectHandle = GBASC->ApplyGameplayEffectToSelf(GameplayEffect, EffectToGrant.EffectLevel, GBASC->MakeEffectContext());
		OutGrantedHandles.AddGameplayEffectHandle(GameplayEffectHandle);
	}

	// Grant the attribute sets.
	for (int32 SetIndex = 0; SetIndex < GrantedAttributes.Num(); ++SetIndex)
	{
		const FGBAbilitySet_AttributeSet& SetToGrant = GrantedAttributes[SetIndex];

		if (!IsValid(SetToGrant.AttributeSet))
		{
			UE_LOG(LogTemp, Error, TEXT("GrantedAttributes[%d] on ability set [%s] is not valid"), SetIndex, *GetNameSafe(this));
			continue;
		}

		UAttributeSet* NewSet = NewObject<UAttributeSet>(GBASC->GetOwner(), SetToGrant.AttributeSet);
		GBASC->AddAttributeSetSubobject(NewSet);
		OutGrantedHandles.AddAttributeSet(NewSet);
	}
	return OutGrantedHandles;
}

FGBAbilitySet_GrantedHandles UGB_AbilitySet::GiveAbilitySetToInterface(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface, UObject* OverrideSourceObject) const
{
	UGB_AbilitySystemComponent* GBASC = Cast<UGB_AbilitySystemComponent>(AbilitySystemInterface.GetObject());
	return GiveToAbilitySystem(GBASC, OverrideSourceObject);
}

void UGB_AbilitySet::TakeAbilitySet(FGBAbilitySet_GrantedHandles& AbilitySetHandle)
{
	if (!AbilitySetHandle.IsValid())
	{
		return;
	}

	if (!AbilitySetHandle.AbilitySystemComponent->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySetHandle.AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySetHandle.AbilitySystemComponent->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : AbilitySetHandle.GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			AbilitySetHandle.AbilitySystemComponent->RemoveActiveGameplayEffect(Handle);
		}
	}

	AbilitySetHandle.Reset();
}


void FGBAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FGBAbilitySet_GrantedHandles::AddGameplayEffectHandle(const FActiveGameplayEffectHandle& Handle)
{
	if (Handle.IsValid())
	{
		GameplayEffectHandles.Add(Handle);
	}
}

void FGBAbilitySet_GrantedHandles::AddAttributeSet(UAttributeSet* Set)
{
	GrantedAttributeSets.Add(Set);
}

void FGBAbilitySet_GrantedHandles::TakeFromAbilitySystem(UGB_AbilitySystemComponent* GB_ASC)
{
	check(GB_ASC);

	if (!GB_ASC->IsOwnerActorAuthoritative())
	{
		// Must be authoritative to give or take ability sets.
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			GB_ASC->ClearAbility(Handle);
		}
	}

	for (const FActiveGameplayEffectHandle& Handle : GameplayEffectHandles)
	{
		if (Handle.IsValid())
		{
			GB_ASC->RemoveActiveGameplayEffect(Handle);
		}
	}

	for (UAttributeSet* Set : GrantedAttributeSets)
	{
		GB_ASC->RemoveSpawnedAttribute(Set);
	}

	AbilitySpecHandles.Reset();
	GameplayEffectHandles.Reset();
	GrantedAttributeSets.Reset();
}
/*
void FAbilitySetStatic::UnequipAbilitySet(FGBAbilitySet_GrantedHandles& AbilitySetHandle)
{
	if (AbilitySetHandle.IsValid())
	{
		UGB_AbilitySet::TakeAbilitySet(AbilitySetHandle);
	}
}

FGBAbilitySet_GrantedHandles FAbilitySetStatic::EquipAbilitySet(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface, const UGB_AbilitySet* AbilitySet, UObject* SourceObject)
{
	return AbilitySet->GiveAbilitySetToInterface(AbilitySystemInterface, SourceObject);
}
*/