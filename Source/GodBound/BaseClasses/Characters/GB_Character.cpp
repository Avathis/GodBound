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
	
	
	//InitializeHealthBar();
}

/*
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
*/

void AGB_Character::GrantAbility(TSubclassOf<UGB_GameplayAbility> AbilityClass, int32 Level, int32 InputCode, FGameplayTag InputTag)
{
	if(GetLocalRole() == ROLE_Authority && AbilitySystemComponent.IsValid() && IsValid(AbilityClass))
	{
		UGB_GameplayAbility* Ability = AbilityClass->GetDefaultObject<UGB_GameplayAbility>();
		if(IsValid(Ability))
		{
			FGameplayAbilitySpec AbilitySpec(Ability,Level,InputCode);
			if(InputTag.IsValid())
			{
				AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
			}
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
		
	}
}

void AGB_Character::ActivateAbility(int32 InputCode)
{
	if(AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->AbilityLocalInputPressed(InputCode);
	}
}

void AGB_Character::InitializeHealthBar()
{
	if(UIHealthBar || !AbilitySystemComponent.IsValid())
	{
		return;
	}
	AGB_PlayerController* MainController = Cast<AGB_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	if(MainController && MainController->IsLocalPlayerController())
	{
		if(UIHealthBarClass)
		{
			UIHealthBar = CreateWidget<UGB_HealthWidget>(MainController, UIHealthBarClass);
			if(UIHealthBar && UIHealthBarComponent)
			{
				UIHealthBarComponent->SetWidget(UIHealthBar);
				
				UIHealthBar->SetHealthPercentage(GetHealth() / GetMaxHealth());
				UIHealthBar->SetVisibility(ESlateVisibility::Hidden);
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

void AGB_Character::Fall()
{
	if(FallenEffect)
	{
		AbilitySystemComponent->ApplyGameplayEffectToSelf(FallenEffect.GetDefaultObject(),1, FGameplayEffectContextHandle());
	}
	else
	{
		Die();
	}
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

void AGB_Character::InitializeAttributes()
{
	if (!AbilitySystemComponent.IsValid())
	{
		return;
	}

	if (!DefaultAttributes)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing DefaultAttributes for %s. Please fill in the character's Blueprint."), *FString(__FUNCTION__), *GetName());
		return;
	}

	// Can run on Server and Client
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributes, 0.f, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent.Get());
	}
}

float AGB_Character::GetHealth() const
{
	if(AttributeSet.IsValid())
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
	if(AttributeSet.IsValid())
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
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetSpeed();
	}
	return 0.f;
	
}

float AGB_Character::GetMaxSpeed() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxSpeed();
	}
	return 0.f;
	
}

float AGB_Character::GetHealthRegenRate() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealthRegenRate();
	}
	return 0.f;
	
}

float AGB_Character::GetEnergy() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetEnergy();
	}
	return 0.f;
	
}

float AGB_Character::GetMaxEnergy() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxEnergy();
	}
	return 0.f;
	
}

float AGB_Character::GetEnergyRegenRate() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetEnergyRecoverRate();
	}
	return 0.f;
	
}

float AGB_Character::GetStamina() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetStamina();
	}
	return 0.f;
	
}

float AGB_Character::GetMaxStamina() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMaxStamina();
	}
	return 0.f;
}

float AGB_Character::GetStaminaRegenRate() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetStaminaRecoverRate();
	}
	return 0.f;
	
}

float AGB_Character::GetHealthPercentage()
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth()/AttributeSet->GetMaxHealth();
	}
	return 1.f;
}

float AGB_Character::GetStaminaPercentage()
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetStamina() / AttributeSet->GetMaxHealth();
	}
	return 0.f;
}

float AGB_Character::GetOverHeatPercentage()
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetOverHeat() / AttributeSet->GetOverHeatMax();
	}
	return 0.f;
}

float AGB_Character::GetEnergyPercentage()
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetEnergy() / AttributeSet->GetMaxEnergy();
	}
	return 0.f;
}

float AGB_Character::GetOverHeat()
{
	if (AttributeSet.IsValid())
	{
		return AttributeSet->GetOverHeat();
	}
	return 0.f;
}

float AGB_Character::GetMaxOverHeat()
{
	if (AttributeSet.IsValid())
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
	if(!IsLocallyControlled()&&UGameplayStatics::GetPlayerController(GetWorld(),0)->IsLocalController())
	{
		ShowHealthBar();
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

