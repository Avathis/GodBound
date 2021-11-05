// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Sprint.h"
#include "GodBound/BaseClasses/Characters/GB_Character.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"

void UGB_Sprint::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}
	}
	AGB_Character * Character = CastChecked<AGB_Character>(ActorInfo->AvatarActor.Get());
	if(Character)
	{
		UGB_CharacterMovementComponent* MovementComponent = Cast<UGB_CharacterMovementComponent>(Character->GetMovementComponent());
		if(MovementComponent)
		{
			MovementComponent->ChangeMovementState(EMovementState::EMS_Sprinting);
		}
	}
}

void UGB_Sprint::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	
}

void UGB_Sprint::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	if (ScopeLockCount > 0)
	{
		WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &UGB_Sprint::CancelAbility, Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility));
		return;
	}
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
	AGB_Character* Character = Cast<AGB_Character>(ActorInfo->OwnerActor);
	if(Character)
	{
		UGB_CharacterMovementComponent* MovementComponent = Cast<UGB_CharacterMovementComponent>(Character->GetMovementComponent());
		if(MovementComponent)
		{
			MovementComponent->ChangeMovementState(EMovementState::EMS_Walking);
		}
	}
}


