// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_GameplayAbility.h"

#include "Characters/GB_PlayableCharacter.h"
#include "GodBound/BaseClasses/Characters/GB_Character.h"



FGameplayEffectSpecHandle UGB_GameplayAbility::SetContextEffectStrength(float EffectStrength, FGameplayEffectSpecHandle GameplayEffectSpec)
{
	if (GameplayEffectSpec.Data) 
	{
		FGB_GameplayEffectContext* GBEffectContext = static_cast<FGB_GameplayEffectContext*>(GameplayEffectSpec.Data->GetContext().Get());
		if (GBEffectContext)
		{
			GBEffectContext->SetEffectStrength(EffectStrength);
		}
	}
	return GameplayEffectSpec;
}

UGB_GameplayAbility::UGB_GameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));

	//PlayerCharacter = Cast<AGB_Character>(GetAvatarActorFromActorInfo());

}

AController* UGB_GameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		// Look for a player controller or pawn in the owner chain.
		AActor* TestActor = CurrentActorInfo->OwnerActor.Get();
		while (TestActor)
		{
			if (AController* C = Cast<AController>(TestActor))
			{
				return C;
			}

			if (APawn* Pawn = Cast<APawn>(TestActor))
			{
				return Pawn->GetController();
			}

			TestActor = TestActor->GetOwner();
		}
	}

	return nullptr;
}

AGB_PlayableCharacter* UGB_GameplayAbility::GetPlayerCharFromActorInfo()
{
	if(AGB_PlayableCharacter* PlayerCharacter = Cast<AGB_PlayableCharacter>(GetActorInfo().AvatarActor))
	{
		return PlayerCharacter;
	}
	return nullptr;
}

float UGB_GameplayAbility::GetGameplayEffectSpecHandle()
{
	
	return 1.0f;
}

void UGB_GameplayAbility::NativeOnAbilityFailedToActivate(const FGameplayTagContainer& FailedReason) const
{
	
}

const FGameplayTagContainer* UGB_GameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;

}

void UGB_GameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}

