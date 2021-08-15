// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Character.h"
#include "GodBound/BaseClasses/Attribtes/BaseAttributeSet.h"
#include "GodBound/BaseClasses/GameplayAbilityBase.h"
#include "Components/GB_SpringArmComponent.h"
#include "Components/GB_CameraComponent.h"
#include "Components/GB_CharacterMovementComponent.h"
#include "GB_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGB_Character::AGB_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<UGB_SpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UGB_CameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, UGB_SpringArmComponent::SocketName);

	CameraCollisionBox = CreateDefaultSubobject<UBoxComponent>("CameraCollisionBox");
	CameraCollisionBox->SetupAttachment(GetRootComponent());
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AGB_Character::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<AGB_PlayerController>(GetController());
	if(IsValid(AbilitySystemComponent))
	{
		Attributes = AbilitySystemComponent->GetSet<UBaseAttributeSet>();
	}
}

void AGB_Character::MoveForward(float Value)
{
	if(PlayerController && Value != 0)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AGB_Character::MoveRight(float Value)
{
	if(PlayerController && Value != 0)
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

void AGB_Character::TurnRight(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Value);
	}
}

void AGB_Character::TurnRightAtRate(float Value)
{
	
}

void AGB_Character::LookUp(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Value);
	}
}

void AGB_Character::LookUpAtRate(float Value)
{
	
}

void AGB_Character::Interact()
{
	
}

void AGB_Character::PressSpace()
{
	bSpacePressed = true;
}

void AGB_Character::ReleaseSpace()
{
	bSpacePressed = false;
}

void AGB_Character::PressLMB()
{
	bLMBPressed = true;
}

void AGB_Character::ReleaseLMB()
{
	bLMBPressed = false;
}

void AGB_Character::PressRMB()
{
	bRMBPressed = true;
}

void AGB_Character::ReleaseRMB()
{
	bRMBPressed = false;
}

void AGB_Character::PressShift()
{
	bShiftPressed = true;
}

void AGB_Character::ReleaseShift()
{
	bShiftPressed = false;
}

void AGB_Character::PressCtrl()
{
	bCtrlPressed = true;
	GetCharacterMovement()->bOrientRotationToMovement = !GetCharacterMovement()->bOrientRotationToMovement;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
	//CameraBoom->bUsePawnControlRotation = !CameraBoom->bUsePawnControlRotation;
}

void AGB_Character::ReleaseCtrl()
{
	bCtrlPressed = false;
	//GetCharacterMovement()->bOrientRotationToMovement = false;
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


void AGB_Character::GrantAbility(TSubclassOf<UGameplayAbilityBase> AbilityClass, int32 Level, int32 InputCode)
{
	if(GetLocalRole() == ROLE_Authority && IsValid(AbilitySystemComponent) && IsValid(AbilityClass))
	{
		UGameplayAbilityBase* Ability = AbilityClass->GetDefaultObject<UGameplayAbilityBase>();
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
	AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent,FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("GBAbilityInputID"), static_cast<int32>(GBAbilityInputID::Confirm), static_cast<int32>(GBAbilityInputID::Cancel)));
	PlayerInputComponent->BindAxis(FName("MoveForward"),this, &AGB_Character::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"),this, &AGB_Character::MoveRight);
	
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &AGB_Character::LookUp);
	PlayerInputComponent->BindAxis(FName("LookUpAR"),this, &AGB_Character::LookUpAtRate);
	PlayerInputComponent->BindAxis(FName("Turn"),this, &AGB_Character::TurnRight);
	PlayerInputComponent->BindAxis(FName("TurnAR"),this, &AGB_Character::TurnRightAtRate);

	
	PlayerInputComponent->BindAction(FName("Shift"),IE_Pressed,this,&AGB_Character::PressShift);
	PlayerInputComponent->BindAction(FName("ReShift"),IE_Pressed,this,&AGB_Character::ReleaseShift);
	
	PlayerInputComponent->BindAction(FName("Space"),IE_Pressed,this,&AGB_Character::PressSpace);
	PlayerInputComponent->BindAction(FName("ReSpace"),IE_Pressed,this,&AGB_Character::ReleaseSpace);
	
	PlayerInputComponent->BindAction(FName("LMB"),IE_Pressed,this,&AGB_Character::PressLMB);
	PlayerInputComponent->BindAction(FName("ReLMB"),IE_Pressed,this,&AGB_Character::ReleaseLMB);
	
	PlayerInputComponent->BindAction(FName("RMB"),IE_Pressed,this,&AGB_Character::PressRMB);
	PlayerInputComponent->BindAction(FName("ReRMB"),IE_Pressed,this,&AGB_Character::ReleaseRMB);

	PlayerInputComponent->BindAction(FName("Control"), IE_Pressed, this, &AGB_Character::PressCtrl);
}

