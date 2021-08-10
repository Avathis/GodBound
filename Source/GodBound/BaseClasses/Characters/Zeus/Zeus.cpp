// Fill out your copyright notice in the Description page of Project Settings.


#include "Zeus.h"
#include "GodBound/BaseClasses/Attribtes/BaseAttributeSet.h"
#include "GodBound/BaseClasses/GameplayAbilityBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GodBound/BaseClasses/Characters/PlayableCharacterController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

FVector AZeus::LightningStrike()
{
	if(IsValid(PlayerController))
	{
		FVector PlayerLocation;
		FRotator PlayerRotation;

		PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
		FVector SocketLocation = GetMesh()->GetSocketLocation(FName("HandSocket"));
		FVector ShotDirection = -PlayerRotation.Vector();

		//DrawDebugCamera(GetWorld(), PlayerLocation, PlayerRotation, 90, 2, FColor::Red, true);

		FVector End = PlayerLocation + PlayerRotation.Vector() * 3000.f;
		
		FHitResult Hit;
		
		FCollisionQueryParams TraceParams;
		TArray<AActor*> ActorsToIgnore;
		CameraCollisionBox->GetOverlappingActors(ActorsToIgnore);
		/*
		for(AActor* Debug : ActorsToIgnore)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Debug->GetName());
		}
		
		if(ActorsToIgnore.IsEmpty())
		{
			UE_LOG(LogTemp,Warning,TEXT("Its Empty")); 
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("It's not empty"));
		}
		*/
		TraceParams.AddIgnoredActors(ActorsToIgnore);
		TraceParams.AddIgnoredActor(this);
		
		if(GetWorld()->LineTraceSingleByChannel(Hit,PlayerLocation, End, ECC_Visibility, TraceParams))
		{
			return Hit.Location;
		}
		End.Z = 0;
		return End;
		
	}
	FHitResult Hit;
	return Hit.Location;
}

void AZeus::BeginPlay()
{
	Super::BeginPlay();
}

void AZeus::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AZeus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


