// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_EnemyCharacter.h"

#include "Blueprint/UserWidget.h"

#include "Components/SphereComponent.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"



AGB_EnemyCharacter::AGB_EnemyCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGB_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	
}

void AGB_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AGB_EnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


