// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayerState.h"
#include "GodBound/BaseClasses/Attributes/GB_AttributeSet.h"
#include "GodBound/BaseClasses/Characters/GB_Character.h"
#include "GodBound/BaseClasses/UI/GB_HealthWidget.h"

AGB_PlayerState::AGB_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UGB_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	NetUpdateFrequency = 100.0f;
	
	AttributeSet = CreateDefaultSubobject<UGB_AttributeSet>(TEXT("BaseAttributeSet"));
	AbilitySystemComponent->AddAttributeSetSubobject(AttributeSet);
	//AttributeSet = AbilitySystemComponent->GetSet<UGB_AttributeSet>();
	
}

void AGB_PlayerState::PostInitializeComponents()
{

	Super::PostInitializeComponents();
}

UAbilitySystemComponent* AGB_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UGB_AttributeSet* AGB_PlayerState::GetAttributeSetBase() const
{
	return AttributeSet;
}

void AGB_PlayerState::SetAttributeSet()
{
	//AttributeSet = AbilitySystemComponent->GetSet<UGB_AttributeSet>();
}

bool AGB_PlayerState::IsAlive() const
{
	return true;
}

float AGB_PlayerState::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AGB_PlayerState::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AGB_PlayerState::GetSpeed() const
{
	return AttributeSet->GetSpeed();
}

float AGB_PlayerState::GetMaxSpeed() const
{
	return AttributeSet->GetMaxSpeed();
}

float AGB_PlayerState::GetHealthRegenRate() const
{
	return AttributeSet->GetHealthRegenRate();
}

float AGB_PlayerState::GetEnergy() const
{
	return AttributeSet->GetEnergy();
}

float AGB_PlayerState::GetMaxEnergy() const
{
	return AttributeSet->GetMaxEnergy();
}

float AGB_PlayerState::GetEnergyRecoverRate() const
{
	return AttributeSet->GetEnergyRecoverRate();
}

float AGB_PlayerState::GetStamina() const
{
	return AttributeSet->GetStamina();
}

float AGB_PlayerState::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

float AGB_PlayerState::GetStaminaRecoverRate() const
{
	return AttributeSet->GetStaminaRecoverRate();
}

float AGB_PlayerState::GetHealthPercentage()
{
	return AttributeSet->GetMaxHealth()/AttributeSet->GetHealth();
}

float AGB_PlayerState::GetOverHeat()
{
	return AttributeSet->GetOverHeat();
}

float AGB_PlayerState::GetMaxOverHeat()
{
	return AttributeSet->GetOverHeatMax();
}

float AGB_PlayerState::GetOverHeatPercentage()
{
	return AttributeSet->GetOverHeatMax()/AttributeSet->GetOverHeat();
}

float AGB_PlayerState::GetStaminaPercentage()
{
	return AttributeSet->GetStamina()/AttributeSet->GetMaxStamina();
}

float AGB_PlayerState::GetEnergyPercentage()
{
	return AttributeSet->GetMaxEnergy()/AttributeSet->GetEnergy();
}

void AGB_PlayerState::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(AbilitySystemComponent))
	{
		
		
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGB_PlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AGB_PlayerState::MaxHealthChanged);
		SpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetSpeedAttribute()).AddUObject(this, &AGB_PlayerState::SpeedChanged);
		MaxSpeedChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxSpeedAttribute()).AddUObject(this, &AGB_PlayerState::MaxSpeedChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AGB_PlayerState::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &AGB_PlayerState::MaxStaminaChanged);
		EnergyChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetEnergyAttribute()).AddUObject(this, &AGB_PlayerState::EnergyChanged);
		MaxEnergyChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxEnergyAttribute()).AddUObject(this, &AGB_PlayerState::MaxEnergyChanged);
		//AbilitySystemComponent->InitAbilityActorInfo(this, this); // Possibly change to InitializeComponent();
	}
}

void AGB_PlayerState::HealthChanged(const FOnAttributeChangeData& Data)
{
	float Health = Data.NewValue;

	// Update floating status bar
	AGB_Character* Hero = Cast<AGB_Character>(GetPawn());
	if (Hero)
	{
		UGB_HealthWidget* HealthWidget = Hero->GetHealthWidget();
		if (HealthWidget)
		{
			HealthWidget->SetHealthPercentage(GetHealthPercentage());
		}
	}
}

void AGB_PlayerState::MaxHealthChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::SpeedChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::MaxSpeedChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::HealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::EnergyChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::MaxEnergyChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::EnergyRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::StaminaChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::MaxStaminaChanged(const FOnAttributeChangeData& Data)
{
}

void AGB_PlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
}
