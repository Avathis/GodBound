// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_CharacterMovementComponent.h"

UGB_CharacterMovementComponent::UGB_CharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UGB_CharacterMovementComponent::ChangeMovementState(EMovementState NewMovementState)
{
	MovementType = NewMovementState;
	switch (MovementType)
	{
	case EMovementState::EMS_Idle: break;
	case EMovementState::EMS_Walking:
		{
			MaxWalkSpeed = 650;
			break;
		}
	case EMovementState::EMS_Sprinting:
		{
			MaxWalkSpeed = 900;
			UE_LOG(LogTemp,Warning,TEXT("Sprinting"))
			break;
		}
	case EMovementState::EMS_AbilitySprint:
		{
			MaxWalkSpeed = 1500; 
			break;
		}
	default: ;
	}
}
