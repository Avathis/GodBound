// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_HephaestusAnimInstance.h"
#include "GodBound/BaseClasses/Characters/GB_Character.h"

void UGB_HephaestusAnimInstance::NativeInitializeAnimation()
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

void UGB_HephaestusAnimInstance::UpdateAnimationProperties()
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
