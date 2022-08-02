// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Character.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GodBound/BaseClasses/GB_GameplayAbility.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"
#include "GB_PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GodBound/BaseClasses/UI/GB_HealthWidget.h"
#include "Kismet/GameplayStatics.h"
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

	UIHealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UIHealthBar"));
	UIHealthBarComponent->SetupAttachment(RootComponent);
	UIHealthBarComponent->SetRelativeLocation(FVector(0, 0, 120));
	UIHealthBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIHealthBarComponent->SetDrawSize(FVector2D(500, 500));
	
}

// Called when the game starts or when spawned
void AGB_Character::BeginPlay()
{
	Super::BeginPlay();
	CharacterMovementComponent = Cast<UGB_CharacterMovementComponent>(GetMovementComponent());
	PlayerController = Cast<AGB_PlayerController>(GetController());
	if(IsValid(AbilitySystemComponent))
	{
		Attributes = AbilitySystemComponent->GetSet<UGB_AttributeSet>();
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetHealthAttribute()).AddUObject(this, &AGB_Character::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxHealthAttribute()).AddUObject(this, &AGB_Character::MaxHealthChanged);
		SpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetSpeedAttribute()).AddUObject(this, &AGB_Character::SpeedChanged);
		MaxSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxSpeedAttribute()).AddUObject(this, &AGB_Character::MaxSpeedChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetStaminaAttribute()).AddUObject(this, &AGB_Character::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxStaminaAttribute()).AddUObject(this, &AGB_Character::MaxStaminaChanged);
		EnergyChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetEnergyAttribute()).AddUObject(this, &AGB_Character::EnergyChanged);
		MaxEnergyChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attributes->GetMaxEnergyAttribute()).AddUObject(this, &AGB_Character::MaxEnergyChanged);
		AbilitySystemComponent->InitAbilityActorInfo(this, this); // Possibly change to InitializeComponent();
	}
	
	InitializeHealthBar();
	if(this == Cast<AGB_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))&& UIHealthBar)
	{
		UIHealthBar->SetVisibility(ESlateVisibility::Hidden);
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
		//UE_LOG(LogTemp, Warning, TEXT("%i"),Hit.bBlockingHit);
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

void AGB_Character::InitializeHealthBar()
{
	if(UIHealthBar || !IsValid(AbilitySystemComponent))
	{
		return;
	}
	AGB_PlayerController* MainController = Cast<AGB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	if(MainController)
	{
		if(UIHealthBarClass)
		{
			UIHealthBar = CreateWidget<UGB_HealthWidget>(MainController, UIHealthBarClass);
			if(UIHealthBar && UIHealthBarComponent)
			{
				UIHealthBarComponent->SetWidget(UIHealthBar);

				UIHealthBar->SetHealthPercentage(GetHealth() / GetMaxHealth());
			}
		}
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

void AGB_Character::EnterCombat()
{

}

void AGB_Character::ExitCombat()
{

}

void AGB_Character::UnequipAbilitySet(FGBAbilitySet_GrantedHandles AbilitySetHandle)
{
	if (AbilitySetHandle.IsValid())
	{
		UGB_AbilitySet::TakeAbilitySet(AbilitySetHandle);
	}
}

FGBAbilitySet_GrantedHandles AGB_Character::ChangeAbilitySet(UGB_AbilitySystemComponent* GBASC, const UGB_AbilitySet* AbilitySet, UObject* SourceObject, FGBAbilitySet_GrantedHandles FormerAbilitySet)
{
	UnequipAbilitySet(FormerAbilitySet);
	return EquipAbilitySet(GBASC,AbilitySet, SourceObject);
}

FGBAbilitySet_GrantedHandles AGB_Character::EquipAbilitySet(UGB_AbilitySystemComponent* GBASC, const UGB_AbilitySet* AbilitySet, UObject* SourceObject)
{
	return AbilitySet->GiveToAbilitySystem(GBASC, SourceObject);
}

UGB_CharacterMovementComponent* AGB_Character::GetAdvMovementComponent()
{
	return CharacterMovementComponent;
}

float AGB_Character::GetHealth() const
{
	return Attributes->GetHealth();
}

float AGB_Character::GetMaxHealth() const
{
	return Attributes->GetMaxHealth();
}

float AGB_Character::GetSpeed() const
{
	return Attributes->GetSpeed();
}

float AGB_Character::GetMaxSpeed() const
{
	return Attributes->GetMaxSpeed();
}

float AGB_Character::GetHealthRegenRate() const
{
	return Attributes->GetHealthRegenRate();
}

float AGB_Character::GetEnergy() const
{
	return Attributes->GetEnergy();
}

float AGB_Character::GetMaxEnergy() const
{
	return Attributes->GetMaxEnergy();
}

float AGB_Character::GetEnergyRegenRate() const
{
	return Attributes->GetEnergyRecoverRate();
}

float AGB_Character::GetStamina() const
{
	return Attributes->GetStamina();
}

float AGB_Character::GetMaxStamina() const
{
	return Attributes->GetMaxStamina();
}

float AGB_Character::GetStaminaRegenRate() const
{
	return Attributes->GetStaminaRecoverRate();
}

float AGB_Character::GetHealthPercentage()
{
	if(Attributes)
	{
		return Attributes->GetHealth()/Attributes->GetMaxHealth();
	}
	return 1.f;
}

float AGB_Character::GetStaminaPercentage()
{
	if (Attributes)
	{
		return Attributes->GetStamina() / Attributes->GetMaxHealth();
	}
	return 0.f;
}

float AGB_Character::GetOverHeatPercentage()
{
	if (Attributes)
	{
		return Attributes->GetOverHeat() / Attributes->GetOverHeatMax();
	}
	return 0.f;
}

float AGB_Character::GetEnergyPercentage()
{
	if (Attributes)
	{
		return Attributes->GetEnergy() / Attributes->GetMaxEnergy();
	}
	return 0.f;
}

float AGB_Character::GetOverHeat()
{
	if (Attributes)
	{
		return Attributes->GetOverHeat();
	}
	return 0.f;
}

float AGB_Character::GetMaxOverHeat()
{
	if (Attributes)
	{
		return Attributes->GetOverHeatMax();
	}
	return 0.f;
}

void AGB_Character::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;
	if(UIHealthBar)
	{
		UIHealthBar->SetHealthPercentage(Health/GetMaxHealth());
	}
}

void AGB_Character::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_Character::SpeedChanged(const FOnAttributeChangeData& Data)
{

}

void AGB_Character::MaxSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetAdvMovementComponent()->MaxWalkSpeed = Data.NewValue;
}

void AGB_Character::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_Character::EnergyChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_Character::MaxEnergyChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_Character::EnergyRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_Character::StaminaChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_Character::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_Character::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

