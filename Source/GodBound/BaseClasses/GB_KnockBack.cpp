// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_KnockBack.h"
#include "Components/GB_AbilitySystemComponent.h"

void UGB_KnockBack::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo,Spec);
	ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
}
