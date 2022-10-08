// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayableCharacter.h"

#include "Components/BoxComponent.h"
#include "GB_PlayerController.h"
#include "GodBound/BaseClasses/Components/GB_CameraComponent.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"
#include "GodBound/BaseClasses/Components/GB_SpringArmComponent.h"

AGB_PlayableCharacter::AGB_PlayableCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGB_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CameraBoom = CreateDefaultSubobject<UGB_SpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UGB_CameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, UGB_SpringArmComponent::SocketName);

	CameraCollisionBox = CreateDefaultSubobject<UBoxComponent>("CameraCollisionBox");
	CameraCollisionBox->SetupAttachment(GetRootComponent());
	CameraCollisionBox->SetRelativeLocation(FVector(-100.000000,140.000000,30.000000));
	CameraCollisionBox->SetBoxExtent(FVector(32.000000,32.000000,32.000000));
}

void AGB_PlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("GBAbilityInputID"), static_cast<int32>(GBAbilityInputID::Confirm), static_cast<int32>(GBAbilityInputID::Cancel)));
	PlayerInputComponent->BindAxis(FName("MoveForward"),this, &AGB_PlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"),this, &AGB_PlayableCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AGB_PlayableCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookUpAR"),this, &AGB_PlayableCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis(FName("Turn"),this, &AGB_PlayableCharacter::TurnRight);
	PlayerInputComponent->BindAxis(FName("TurnAR"),this, &AGB_PlayableCharacter::TurnRightAtRate);
}

void AGB_PlayableCharacter::MoveForward(float Value)
{
	ForwardAxis = Value;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			15.f,
			FColor::Red,
			FString(TEXT("MoveForward Value %d!"),Value)
		);
	}
	if ((Controller) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	
}



void AGB_PlayableCharacter::MoveRight(float Value)
{
	RightAxis = Value;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			2,
			15.f,
			FColor::Red,
			FString(TEXT("MoveRight Value %d!"),Value)
		);
	}
	if ( (Controller) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
	
}

void AGB_PlayableCharacter::TurnRight(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Value);
	}
}

void AGB_PlayableCharacter::TurnRightAtRate(float Value)
{
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGB_PlayableCharacter::LookUp(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Value);
	}
}

void AGB_PlayableCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGB_PlayableCharacter::Interact()
{
	
}

FHitResult AGB_PlayableCharacter::HitTraceFromCamera(float MaxRange)
{
	FHitResult Hit;
	FVector PlayerLocation;
	FRotator PlayerRotation;

	PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	FVector End = PlayerLocation + PlayerRotation.Vector() * MaxRange;
	FCollisionQueryParams TraceParams;
	TArray<AActor*> ActorsToIgnore;
	CameraCollisionBox->GetOverlappingActors(ActorsToIgnore);
	TraceParams.AddIgnoredActors(ActorsToIgnore);
	TraceParams.AddIgnoredActor(this);

	if(GetWorld()->LineTraceSingleByChannel(Hit,PlayerLocation, End, ECC_Visibility, TraceParams))
	{
		return Hit;
		
		//UE_LOG(LogTemp, Warning, TEXT("%i"),Hit.bBlockingHit);
	}
	return Hit;
}
//LEGACY

void AGB_PlayableCharacter::EnterCombat()
{
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void AGB_PlayableCharacter::ExitCombat()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}