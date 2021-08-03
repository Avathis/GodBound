// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableCharacterBase.h"
#include "GodBound/BaseClasses/Attribtes/BaseAttributeSet.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayableCharacterController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APlayableCharacterBase::APlayableCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void APlayableCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayableCharacterController>(GetController());
	if(IsValid(AbilitySystemComponent))
	{
		Attributes = AbilitySystemComponent->GetSet<UBaseAttributeSet>();
	}
}

void APlayableCharacterBase::MoveForward(float Value)
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

void APlayableCharacterBase::MoveRight(float Value)
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

void APlayableCharacterBase::TurnRight(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddYawInput(Value);
	}
}

void APlayableCharacterBase::TurnRightAtRate(float Value)
{
	
}

void APlayableCharacterBase::LookUp(float Value)
{
	if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
	{
		APlayerController* const PC = CastChecked<APlayerController>(Controller);
		PC->AddPitchInput(Value);
	}
}

void APlayableCharacterBase::LookUpAtRate(float Value)
{
	
}

void APlayableCharacterBase::Interact()
{
	
}

void APlayableCharacterBase::PressSpace()
{
	bSpacePressed = true;
}

void APlayableCharacterBase::ReleaseSpace()
{
	bSpacePressed = false;
}

void APlayableCharacterBase::PressLMB()
{
	bLMBPressed = true;
}

void APlayableCharacterBase::ReleaseLMB()
{
	bLMBPressed = false;
}

void APlayableCharacterBase::PressRMB()
{
	bRMBPressed = true;
}

void APlayableCharacterBase::ReleaseRMB()
{
	bRMBPressed = false;
}

void APlayableCharacterBase::PressShift()
{
	bShiftPressed = true;
}

void APlayableCharacterBase::ReleaseShift()
{
	bShiftPressed = false;
}

void APlayableCharacterBase::PressCtrl()
{
	bCtrlPressed = true;
	GetCharacterMovement()->bOrientRotationToMovement = !GetCharacterMovement()->bOrientRotationToMovement;
	bUseControllerRotationYaw = !bUseControllerRotationYaw;
	//CameraBoom->bUsePawnControlRotation = !CameraBoom->bUsePawnControlRotation;
}

void APlayableCharacterBase::ReleaseCtrl()
{
	bCtrlPressed = false;
	//GetCharacterMovement()->bOrientRotationToMovement = false;
}


// Called every frame
void APlayableCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayableCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"),this, &APlayableCharacterBase::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"),this, &APlayableCharacterBase::MoveRight);
	
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APlayableCharacterBase::LookUp);
	PlayerInputComponent->BindAxis(FName("LookUpAR"),this, &APlayableCharacterBase::LookUpAtRate);
	PlayerInputComponent->BindAxis(FName("Turn"),this, &APlayableCharacterBase::TurnRight);
	PlayerInputComponent->BindAxis(FName("TurnAR"),this, &APlayableCharacterBase::TurnRightAtRate);

	
	PlayerInputComponent->BindAction(FName("Shift"),IE_Pressed,this,&APlayableCharacterBase::PressShift);
	PlayerInputComponent->BindAction(FName("ReShift"),IE_Pressed,this,&APlayableCharacterBase::ReleaseShift);
	
	PlayerInputComponent->BindAction(FName("Space"),IE_Pressed,this,&APlayableCharacterBase::PressSpace);
	PlayerInputComponent->BindAction(FName("ReSpace"),IE_Pressed,this,&APlayableCharacterBase::ReleaseSpace);
	
	PlayerInputComponent->BindAction(FName("LMB"),IE_Pressed,this,&APlayableCharacterBase::PressLMB);
	PlayerInputComponent->BindAction(FName("ReLMB"),IE_Pressed,this,&APlayableCharacterBase::ReleaseLMB);
	
	PlayerInputComponent->BindAction(FName("RMB"),IE_Pressed,this,&APlayableCharacterBase::PressRMB);
	PlayerInputComponent->BindAction(FName("ReRMB"),IE_Pressed,this,&APlayableCharacterBase::ReleaseRMB);

	PlayerInputComponent->BindAction(FName("Control"), IE_Pressed, this, &APlayableCharacterBase::PressCtrl);
}

