// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayableCharacter.h"

#include "Components/BoxComponent.h"
#include "GB_PlayerController.h"
#include "Components/WidgetComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GodBound/BaseClasses/GB_Weapon.h"
#include "GodBound/BaseClasses/Components/GB_CameraComponent.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"
#include "GodBound/BaseClasses/Components/GB_SpringArmComponent.h"
#include "GodBound/Player/GB_PlayerState.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GodBound/GameMode/GB_GameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GodBound/BaseClasses/GB_InputComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AGB_PlayableCharacter::AGB_PlayableCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UGB_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	CameraBoom = CreateDefaultSubobject<UGB_SpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UGB_CameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, UGB_SpringArmComponent::SocketName);

	/*
	CameraCollisionBox = CreateDefaultSubobject<UBoxComponent>("CameraCollisionBox");
	CameraCollisionBox->SetupAttachment(GetRootComponent());
	CameraCollisionBox->SetRelativeLocation(FVector(-100.000000,140.000000,30.000000));
	CameraCollisionBox->SetBoxExtent(FVector(32.000000,32.000000,32.000000));
	*/
	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetRootComponent());
	OverheadWidget->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AGB_PlayableCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	//DOREPLIFETIME(AGB_PlayableCharacter, ActiveWeapon);
}

void AGB_PlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	if(PlayerController)
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AGB_PlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if( UGB_InputComponent* EnhancedInputComponent = CastChecked<UGB_InputComponent>(PlayerInputComponent))
	{
		if(InputConfig)
		{
			EnhancedInputComponent->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.Move")),ETriggerEvent::Triggered,this,&ThisClass::Move, false);
			EnhancedInputComponent->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.Look")), ETriggerEvent::Triggered, this, &ThisClass::Look, false);
			EnhancedInputComponent->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.Ability.Confirm")), ETriggerEvent::Triggered, this, &ThisClass::Input_Confirm, false);
			EnhancedInputComponent->BindNativeAction(InputConfig, FGameplayTag::RequestGameplayTag(FName("Input.Ability.Cancel")), ETriggerEvent::Triggered, this, &ThisClass::Input_Cancel, false);
			TArray<uint32> BindHandle;
			//EnhancedInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::Input_AbilityInputTagPressed,&ThisClass::Input_AbilityInputTagReleased, BindHandle);
			EnhancedInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::Input_AbilityInputTagTriggered,BindHandle);
		}
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGB_PlayableCharacter::Move);
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGB_PlayableCharacter::Look);
	}
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
		
		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSetBase for convenience attribute functions
		/*
		if(!AttributeSet && !PS->GetAttributeSetBase())
		{
			PS->SetAttributeSet();
		}
		*/
		AttributeSet =  PS->GetAttributeSetBase();

		InitializeAbilities();
		
		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		InitializeHealthBar();
		
		InitializeHealthBarAttributes();
		
		// Respawn specific things that won't affect first possession.

		// Forcibly set the DeadTag count to 0
		/*
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		


		AddStartupEffects();

		AddCharacterAbilities();
*/
		AGB_PlayerController* PC = Cast<AGB_PlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		/*
		SetHealth(GetMaxHealth());
		SetMana(GetMaxMana());
		SetStamina(GetMaxStamina());
*/
		// End respawn specific things
		
	}
}


void AGB_PlayableCharacter::MoveForward(float Value)
{
	if(!bCanManuallyMove) return;
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
	ForwardAxis = Value;
	/*
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
	if(!bCanManuallyMove) return;
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
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController() && bCanManuallyLookAround)
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Value);
	}
}

void AGB_PlayableCharacter::TurnRightAtRate(float Value)
{
	if(bCanManuallyLookAround)
	AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AGB_PlayableCharacter::LookUp(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController() && bCanManuallyLookAround)
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Value);
	}
}

void AGB_PlayableCharacter::LookUpAtRate(float Value)
{
	if(bCanManuallyLookAround)
	AddControllerPitchInput(Value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGB_PlayableCharacter::Interact()
{
	
}
/*
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
*/
void AGB_PlayableCharacter::EquipWeapon(AGB_Weapon* WeaponToEquip)
{
	ServerEquipWeapon(WeaponToEquip);
	return;
	if(HasAuthority())
	{
		if(WeaponToEquip)
		{
			WeaponToEquip->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepRelative, true));
			const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(WeaponToEquip->SocketName);
			if(Socket)
			{
				Socket->AttachActor(WeaponToEquip, GetMesh());
			}
			//WeaponToEquip->AttachToActor(Character,FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponRef->SocketName);
			//WeaponToEquip->WeaponState = EWeaponState::EWS_Equipped;
			WeaponToEquip->SetOwner(this);
		}
	}
	else
	{
		ServerEquipWeapon(WeaponToEquip);
	}
	//ServerEquipWeapon(WeaponToEquip);
	
}

void AGB_PlayableCharacter::ServerEquipWeapon_Implementation(AGB_Weapon* WeaponToEquip)
{
	if(WeaponToEquip)
	{
		WeaponToEquip->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		const USkeletalMeshSocket* Socket = GetMesh()->GetSocketByName(WeaponToEquip->SocketName);
		if(Socket)
		{
			if(Socket->AttachActor(WeaponToEquip, GetMesh()))
			{
				WeaponToEquip->WeaponState = EWeaponState::EWS_Equipped;
				WeaponToEquip->SetOwner(this);
			}
		}
		//WeaponToEquip->AttachToActor(this,FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponToEquip->SocketName);
	}
	
}


void AGB_PlayableCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		//AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent,FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("GBAbilityInputID"), static_cast<int32>(GBAbilityInputID::Confirm), static_cast<int32>(GBAbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

void AGB_PlayableCharacter::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UGB_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityInputTagPressed(InputTag);
		
		UE_LOG(LogTemp, Error, TEXT("Input_AbilityInputTagPressed"));
	}
	/*
	if (UGB_AbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponent())
	{
		const UInputAction* InputAction = Cast<UInputAction>(InputActionInstance.GetSourceAction());

		const FGameplayTag InputTag = InputConfig->FindInputTagForAbilityInputAction(InputAction);

		if (InputTag.IsValid())
		{
			const FInputActionValue InputActionValue = InputActionInstance.GetValue();
			if (InputActionValue.Get<bool>())
			{
				AbilitySystemComponent->AbilityInputTagPressed(InputTag);
			}
			else
			{
				AbilitySystemComponent->AbilityInputTagReleased(InputTag);
			}
		}
	}
	*/
}

void AGB_PlayableCharacter::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UGB_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AGB_PlayableCharacter::Input_AbilityInputTagTriggered(const FInputActionInstance& InputActionInstance)
{
	if (UGB_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		const UInputAction* InputAction = Cast<UInputAction>(InputActionInstance.GetSourceAction());

		const FGameplayTag InputTag = InputConfig->FindInputTagForAbilityInputAction(InputAction);

		if (InputTag.IsValid())
		{
			const FInputActionValue InputActionValue = InputActionInstance.GetValue();
			if (InputActionValue.Get<bool>())
			{
				ASC->AbilityInputTagPressed(InputTag);
			}
			else
			{
				ASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}

void AGB_PlayableCharacter::Input_Confirm(const FInputActionInstance& InputActionInstance)
{
	if(UGB_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->LocalInputConfirm();
	}
}

void AGB_PlayableCharacter::Input_Cancel(const FInputActionInstance& InputActionInstance)
{
	if(UGB_AbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		ASC->LocalInputCancel();
	}
}

void AGB_PlayableCharacter::Move(const FInputActionValue& Value)
{
	const FVector2d CurrentValue = Value.Get<FVector2d>();
	const FRotator Rotation = PlayerController->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	MoveRightValue = CurrentValue.X;
	MoveForwardValue = CurrentValue.Y;
	
	if(!CharacterMovementComponent->bIsClimbing)
	{
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, CurrentValue.Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, CurrentValue.X);
	}
	else
	{
		if(CurrentValue.Y>0||CurrentValue.Y<0)
		{
			
		}
		if(CurrentValue.X>0||CurrentValue.X<0)
		{
			FVector TraceStart = GetActorLocation()+GetActorRightVector()*50*CurrentValue.X;
			TraceStart+= FVector(0.f,0.f,100);
			FVector TraceEnd = TraceStart+GetActorForwardVector()*50;
			TArray<AActor*> ActorsToIgnore;
			TArray<FHitResult> OutHits;
			ActorsToIgnore.Add(this);
			if(UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(),TraceStart,TraceEnd, 10.f, { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) ,
				UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) },false, ActorsToIgnore, EDrawDebugTrace::None,OutHits,
				true))
			{
				const FVector RightDirection = GetActorRightVector();
				AddMovementInput(RightDirection, CurrentValue.X);
			}
			else
			{
				FVector SideTraceStart = GetActorLocation()+GetActorForwardVector()*75+GetActorRightVector()*100*CurrentValue.X;
				SideTraceStart+= FVector(0.f,0.f,100);
				FVector SideTraceEnd = SideTraceStart-GetActorRightVector()*100*CurrentValue.X;
				TArray<AActor*> SideActorsToIgnore;
				FHitResult SideOutHit;
				ActorsToIgnore.Add(this);
				if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),SideTraceStart,SideTraceEnd, 5.f, { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) ,
					UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) },false, SideActorsToIgnore, EDrawDebugTrace::None,SideOutHit,
					true))
				{
					ParkourTransition(SideOutHit, CurrentValue.X,FGameplayTag::RequestGameplayTag("State.Movement.Climb.Transition.Hop"));
				}
			}
		}
	}
	
}

void AGB_PlayableCharacter::Look(const FInputActionValue& Value)
{
	const FVector2d LookAxisValue = Value.Get<FVector2d>();
	if(PlayerController)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
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
		/*
		if(!AttributeSet && !PS->GetAttributeSetBase())
		{
			PS->SetAttributeSet();
		}
		*/
		AttributeSet = PS->GetAttributeSetBase();
		
		
		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();
		InitializeHealthBar();
		InitializeHealthBarAttributes();
		
		AGB_PlayerController* PC = Cast<AGB_PlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}
		
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