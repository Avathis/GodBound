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
	
	
	InitializeHealthBar();
	if(this == Cast<AGB_Character>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))&& UIHealthBar)
	{
		UIHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
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
	if(GetLocalRole() == ROLE_Authority && AbilitySystemComponent && IsValid(AbilityClass))
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
	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputCode);
	}
}

void AGB_Character::InitializeHealthBar()
{
	if(UIHealthBar || !AbilitySystemComponent)
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

UGB_HealthWidget* AGB_Character::GetHealthWidget()
{
	return UIHealthBar;
}

float AGB_Character::GetHealth() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	else
	{
		return 0.f;
	}
}

float AGB_Character::GetMaxHealth() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}
	else
	{
		return 0.f;
	}
}

float AGB_Character::GetSpeed() const
{
	//TODO: Changed this in order to test, POssibly troublesome
	//return GetCharacterMovement()->MaxWalkSpeed;
	if(AttributeSet)
	{
		return AttributeSet->GetSpeed();
	}
	return 0.f;
	
}

float AGB_Character::GetMaxSpeed() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetMaxSpeed();
	}
	return 0.f;
	
}

float AGB_Character::GetHealthRegenRate() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetHealthRegenRate();
	}
	return 0.f;
	
}

float AGB_Character::GetEnergy() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetEnergy();
	}
	return 0.f;
	
}

float AGB_Character::GetMaxEnergy() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetMaxEnergy();
	}
	return 0.f;
	
}

float AGB_Character::GetEnergyRegenRate() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetEnergyRecoverRate();
	}
	return 0.f;
	
}

float AGB_Character::GetStamina() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetStamina();
	}
	return 0.f;
	
}

float AGB_Character::GetMaxStamina() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetMaxStamina();
	}
	return 0.f;
}

float AGB_Character::GetStaminaRegenRate() const
{
	if(AttributeSet)
	{
		return AttributeSet->GetStaminaRecoverRate();
	}
	return 0.f;
	
}

float AGB_Character::GetHealthPercentage()
{
	if(AttributeSet)
	{
		return AttributeSet->GetHealth()/AttributeSet->GetMaxHealth();
	}
	return 1.f;
}

float AGB_Character::GetStaminaPercentage()
{
	if (AttributeSet)
	{
		return AttributeSet->GetStamina() / AttributeSet->GetMaxHealth();
	}
	return 0.f;
}

float AGB_Character::GetOverHeatPercentage()
{
	if (AttributeSet)
	{
		return AttributeSet->GetOverHeat() / AttributeSet->GetOverHeatMax();
	}
	return 0.f;
}

float AGB_Character::GetEnergyPercentage()
{
	if (AttributeSet)
	{
		return AttributeSet->GetEnergy() / AttributeSet->GetMaxEnergy();
	}
	return 0.f;
}

float AGB_Character::GetOverHeat()
{
	if (AttributeSet)
	{
		return AttributeSet->GetOverHeat();
	}
	return 0.f;
}

float AGB_Character::GetMaxOverHeat()
{
	if (AttributeSet)
	{
		return AttributeSet->GetOverHeatMax();
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
	//GetAdvMovementComponent()->MaxWalkSpeed = Data.NewValue;
}

void AGB_Character::MaxSpeedChanged(const FOnAttributeChangeData& Data)
{
	//GetAdvMovementComponent()->MaxWalkSpeed = Data.NewValue;
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

