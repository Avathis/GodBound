// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_CharacterMovementComponent.h"

UGB_CharacterMovementComponent::UGB_CharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WalkMovementSpeed = 650;
	RunMovementSpeed = 900;
	AbilitySprintMovementSpeed = 1500;

}

EMovementState UGB_CharacterMovementComponent::GetMovementType()
{
	return MovementType;
}

void UGB_CharacterMovementComponent::ChangeMovementState(EMovementState NewMovementState)
{
	MovementType = NewMovementState;
	/*
	switch (MovementType)
	{
	case EMovementState::EMS_Idle: break;
	case EMovementState::EMS_Walking:
		{
		MaxWalkSpeed = WalkMovementSpeed;
			break;
		}
	case EMovementState::EMS_Sprinting:
		{
			MaxWalkSpeed = RunMovementSpeed;
			//UE_LOG(LogTemp,Warning,TEXT("Sprinting"))
			break;
		}
	case EMovementState::EMS_AbilitySprint:
		{
			MaxWalkSpeed = AbilitySprintMovementSpeed; 
			break;
		}
	case EMovementState::EMS_HephaestusChargeSprint:
	{
		break;
	}
	default: ;
	}
	*/
}
