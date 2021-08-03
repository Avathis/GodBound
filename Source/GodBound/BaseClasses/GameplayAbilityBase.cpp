// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"
#include "DrawDebugHelpers.h"
#include "Characters/PlayableCharacterBase.h"
#include "Kismet/GameplayStatics.h"

void UGameplayAbilityBase::FireDebugBeam() const
{
	/*
	APlayerController PlayerController = UGameplayStatics::GetPlayerController(this,0);
	if(IsValid(PlayerController))
	{
		FVector PlayerLocation;
		FQuat PlayerRotation;
		FTransform PlayerTransform = GetMesh()->GetSocketTransform(FName("AimSocket"));
		PlayerLocation = PlayerTransform.GetLocation();
		PlayerRotation = PlayerTransform.GetRotation();

		FVector ShotDirection = -PlayerRotation.Vector();

		//DrawDebugCamera(GetWorld(), PlayerLocation, PlayerRotation, 90, 2, FColor::Red, true);

		FVector End = PlayerLocation + PlayerRotation.Vector() * 20.f;
		
		FHitResult Hit;
		DrawDebugLine(GetWorld(),PlayerLocation, End,FColor::Red,false,-1,0,1);
	}
	*/
}
