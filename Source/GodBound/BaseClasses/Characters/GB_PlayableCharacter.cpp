// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayableCharacter.h"

#include "Components/BoxComponent.h"
#include "GB_PlayerController.h"
#include "Components/WidgetComponent.h"
#include "GodBound/BaseClasses/Components/GB_CameraComponent.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"
#include "GodBound/BaseClasses/Components/GB_SpringArmComponent.h"
#include "GodBound/Player/GB_PlayerState.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

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

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetRootComponent());
}

void AGB_PlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("GBAbilityInputID"), static_cast<int32>(GBAbilityInputID::Confirm), static_cast<int32>(GBAbilityInputID::Cancel)));
	PlayerInputComponent->BindAxis(FName("MoveForward"),this, &AGB_PlayableCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"),this, &AGB_PlayableCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AGB_PlayableCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("LookUpAR"),this, &AGB_PlayableCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis(FName("Turn"),this, &AGB_PlayableCharacter::TurnRight);
	PlayerInputComponent->BindAxis(FName("TurnAR"),this, &AGB_PlayableCharacter::TurnRightAtRate);

	BindASCInput();
}

void AGB_PlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AGB_PlayerState* PS = GetPlayerState<AGB_PlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UGB_AbilitySystemComponent>(PS->GetAbilitySystemComponent());

		if(AbilitySystemComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("ABILITY COMPONENT IS VALID"))
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ABILITY COMPONENT IS NOT VALID"))
		}
		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSetBase for convenience attribute functions
		if(!AttributeSet && !PS->GetAttributeSetBase())
		{
			PS->SetAttributeSet();
		}
		AttributeSet =  PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		//InitializeAttributes();

		
		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		/*
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());

		// End respawn specific things


		AddStartupEffects();

		AddCharacterAbilities();

		AGDPlayerController* PC = Cast<AGDPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}

		InitializeFloatingStatusBar();*/
	}
}

void AGB_PlayableCharacter::MoveForward(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	ForwardAxis = Value;/*
	if ((Controller) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	*/
}



void AGB_PlayableCharacter::MoveRight(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	RightAxis = Value;
	/*
	if ( (Controller) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}*/
	
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

void AGB_PlayableCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("GBAbilityInputID"), static_cast<int32>(GBAbilityInputID::Confirm), static_cast<int32>(GBAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

void AGB_PlayableCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	AGB_PlayerState* PS = GetPlayerState<AGB_PlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UGB_AbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		if(!AttributeSet && !PS->GetAttributeSetBase())
		{
			PS->SetAttributeSet();
		}
		AttributeSet = PS->GetAttributeSetBase();
		if(GEngine)
		{
			if(AttributeSet)
			{
				GEngine->AddOnScreenDebugMessage(3,15.f,FColor::White,FString("PlayerStateSuccessfully replicated"));
			}
		}

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		//InitializeAttributes();
		/*
		AGB_PlayerController* PC = Cast<AGB_PlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
		*/
		// Simulated on proxies don't have their PlayerStates yet when BeginPlay is called so we call it again here
		//InitializeFloatingStatusBar();


		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		//AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		//SetHealth(GetMaxHealth());
		//SetMana(GetMaxMana());
		//SetStamina(GetMaxStamina());
	}
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