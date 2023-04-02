// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_EnemyCharacter.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"



AGB_EnemyCharacter::AGB_EnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGB_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	ASC = CreateDefaultSubobject<UGB_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent = ASC;
	AttributeSet = CreateDefaultSubobject<UGB_AttributeSet>(TEXT("BaseAttributeSet"));
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet.Get());
	
}

void AGB_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeAttributes();
	ASC->InitAbilityActorInfo(this, this);
}

void AGB_EnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGB_EnemyCharacter::GrantAbility(TSubclassOf<UGB_GameplayAbility> AbilityClass, int32 Level, int32 InputCode,
	FGameplayTag InputTag)
{
	if(GetLocalRole() == ROLE_Authority && ASC && IsValid(AbilityClass))
	{
		UGB_GameplayAbility* Ability = AbilityClass->GetDefaultObject<UGB_GameplayAbility>();
		if(IsValid(Ability))
		{
			FGameplayAbilitySpec AbilitySpec(Ability,Level,InputCode);
			if(InputTag.IsValid())
			{
				AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
			}
			ASC->GiveAbility(AbilitySpec);
		}
		
	}
}


