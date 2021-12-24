// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GB_AttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class GODBOUND_API UGB_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Health);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, MaxHealth);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, HealthRegenRate);
	

	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Energy")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Energy);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Energy")
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet,  MaxEnergy);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Energy")
	FGameplayAttributeData EnergyRecoverRate;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet,  EnergyRecoverRate);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Endurance")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Stamina);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Endurance")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, MaxStamina);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Endurance")
	FGameplayAttributeData StaminaRecoverRate;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, StaminaRecoverRate);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Strength")
	FGameplayAttributeData AbilityPower;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, AbilityPower);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverHeat;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverHeat);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverHeatMax;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverHeatMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverCharge;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverCharge);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverChargeMax;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverChargeMax);
};
