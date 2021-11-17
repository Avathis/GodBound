// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Character.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GodBound/BaseClasses/GB_GameplayAbility.h"


#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"
#include "GB_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGB_Character::AGB_Character(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGB_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	
	//CameraCollisionBox->SetScale
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	//Movement
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	AbilitySystemComponent = CreateDefaultSubobject<UGB_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AGB_Character::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AGB_PlayerController>(GetController());
	if(IsValid(AbilitySystemComponent))
	{
		Attributes = AbilitySystemComponent->GetSet<UGB_AttributeSet>();
	}
}



FHitResult AGB_Character::FireDebugBeam()
{
	if(IsValid(PlayerController))
	{
		FVector PlayerLocation;
		FRotator PlayerRotation;

		PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
		FVector SocketLocation = GetMesh()->GetSocketLocation(FName("HandSocket"));
		FVector ShotDirection = -PlayerRotation.Vector();

		//DrawDebugCamera(GetWorld(), PlayerLocation, PlayerRotation, 90, 2, FColor::Red, true);

		FVector End = PlayerLocation + PlayerRotation.Vector() * 2000.f;
		
		FHitResult Hit;
		
		FCollisionQueryParams TraceParams;
		FCollisionQueryParams TraceParamsSocket;
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
		
		TraceParamsSocket.AddIgnoredActor(this);
		
		if(GetWorld()->LineTraceSingleByChannel(Hit,PlayerLocation, End, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(),PlayerLocation, Hit.Location,FColor::Green,false,5,0,1);
			FHitResult Hit2;
			
			if(GetWorld()->LineTraceSingleByChannel(Hit2, SocketLocation, Hit.Location + UKismetMathLibrary::FindLookAtRotation(SocketLocation,Hit.Location).Vector()*10.f, ECC_Visibility, TraceParamsSocket))
			{
				//DrawDebugLine(GetWorld(),SocketLocation, End, FColor::Purple,false,5,0,1);
				DrawDebugLine(GetWorld(), SocketLocation, Hit2.Location + UKismetMathLibrary::FindLookAtRotation(SocketLocation,Hit.Location).Vector()*10.f, FColor::Silver, false, 5, 0, 1);
				UE_LOG(LogTemp, Warning,TEXT("%s"), *Hit2.GetActor()->GetName());
				
				return Hit2;
			}
			else
			{
				DrawDebugLine(GetWorld(), SocketLocation, Hit.Location, FColor::Red, false, 5, 0, 1);
			}
		}
		else
		{
			
			//DrawDebugLine(GetWorld(),SocketLocation, Hit.Location,FColor::Red,false,5,0,1);
			//DrawDebugLine(GetWorld(), PlayerLocation, Hit.Location, FColor::Green, false, 5,0,1);
		}
	}
	FHitResult Hit;
	return Hit;
}

FVector AGB_Character::TraceFromCamera()
{
	FHitResult Hit;
	FVector PlayerLocation;
	FRotator PlayerRotation;

	PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	FVector End = PlayerLocation + PlayerRotation.Vector() * 4000.f;
	FCollisionQueryParams TraceParams;
	TArray<AActor*> ActorsToIgnore;
	CameraCollisionBox->GetOverlappingActors(ActorsToIgnore);
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	TraceParams.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(Hit,PlayerLocation, End, ECC_Visibility, TraceParams))
	{
		return Hit.Location;
	}
	return End;
	
}


void AGB_Character::GrantAbility(TSubclassOf<UGB_GameplayAbility> AbilityClass, int32 Level, int32 InputCode)
{
	if(GetLocalRole() == ROLE_Authority && IsValid(AbilitySystemComponent) && IsValid(AbilityClass))
	{
		UGB_GameplayAbility* Ability = AbilityClass->GetDefaultObject<UGB_GameplayAbility>();
		if(IsValid(Ability))
		{
			FGameplayAbilitySpec AbilitySpec(Ability,Level,InputCode);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
		
	}
}

void AGB_Character::ActivateAbility(int32 InputCode)
{
	if(IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputCode);
	}
}

// Called every frame
void AGB_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGB_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

