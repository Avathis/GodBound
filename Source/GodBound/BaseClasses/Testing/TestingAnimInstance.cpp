// Fill out your copyright notice in the Description page of Project Settings.


#include "TestingAnimInstance.h"
#include "GodBound/BaseClasses/Characters/PlayableCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTestingAnimInstance::NativeInitializeAnimation()
{
	if(!Pawn)
	{
		Pawn = TryGetPawnOwner();
		if(Pawn)
		{
			MainCharacter = Cast<APlayableCharacterBase>(Pawn);
		}
	}
}

void UTestingAnimInstance::UpdateAnimationProperties()
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

		if(!MainCharacter)
		{
			MainCharacter = Cast<APlayableCharacterBase>(Pawn);
		}

	}
}