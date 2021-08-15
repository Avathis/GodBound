// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */


UCLASS()
class GODBOUND_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	UBaseAttributeSet();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UBaseAttributeSet,Health);
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Health);
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Health);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData MaxHealth;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UBaseAttributeSet,MaxHealth);
    GAMEPLAYATTRIBUTE_VALUE_GETTER(MaxHealth);
    GAMEPLAYATTRIBUTE_VALUE_SETTER(MaxHealth);
    GAMEPLAYATTRIBUTE_VALUE_INITTER(MaxHealth);

	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData Energy;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData Stamina;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health")
	FGameplayAttributeData AbilityPower;

<<<<<<< HEAD
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverHeatMax;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, OverHeatMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverCharge;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, OverCharge);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy")
	FGameplayAttributeData OverChargeMax;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, OverChargeMax);
=======
>>>>>>> parent of 8853fd0 (yes)
};
