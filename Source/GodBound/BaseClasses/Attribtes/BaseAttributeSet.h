// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


UCLASS()
class GODBOUND_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	UBaseAttributeSet();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);

	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Energy")
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Energy);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Endurance")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Strength")
	FGameplayAttributeData AbilityPower;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, AbilityPower);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverHeat;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, OverHeat);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverHeatMax;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, OverHeatMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverCharge;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, OverCharge);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverChargeMax;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, OverChargeMax);
};
