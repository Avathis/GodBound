// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AnimInstance.h"
#include "GodBound/BaseClasses/Characters/GB_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UGB_AnimInstance::NativeInitializeAnimation()
{
	if(!Pawn)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn)
		{
			MainCharacter = Cast<AGB_Character>(Pawn);
			if(MainCharacter)
			{
				ForwardAxis = MainCharacter->ForwardAxis;
				RightAxis = MainCharacter->RightAxis;
			}
		}
	}
}

void UGB_AnimInstance::UpdateAnimationProperties()
{
	if(!Pawn)
	{
		Pawn = TryGetPawnOwner();
	}
	if(Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
		MainCharacter = Cast<AGB_Character>(Pawn);
		if(MainCharacter)
		{
			ForwardAxis = MainCharacter->ForwardAxis;
			RightAxis = MainCharacter->RightAxis;
		}
	}
	
}