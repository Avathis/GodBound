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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Health);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, MaxHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", ReplicatedUsing = OnRep_Speed)
	FGameplayAttributeData Speed;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Speed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	FGameplayAttributeData MaxSpeed;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, MaxSpeed);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Health", ReplicatedUsing = OnRep_HealthRegenRate)
	FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, HealthRegenRate);	

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Energy", ReplicatedUsing = OnRep_Energy)
	FGameplayAttributeData Energy;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Energy);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Energy", ReplicatedUsing = OnRep_MaxEnergy)
	FGameplayAttributeData MaxEnergy;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet,  MaxEnergy);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Energy", ReplicatedUsing = OnRep_EnergyRecoverRate)
	FGameplayAttributeData EnergyRecoverRate;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet,  EnergyRecoverRate);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Endurance", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Stamina);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Endurance", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, MaxStamina);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Endurance", ReplicatedUsing = OnRep_StaminaRecoverRate)
	FGameplayAttributeData StaminaRecoverRate;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, StaminaRecoverRate);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Strength", ReplicatedUsing = OnRep_AbilityPower)
	FGameplayAttributeData AbilityPower;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, AbilityPower);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy", ReplicatedUsing = OnRep_OverHeat)
	FGameplayAttributeData OverHeat;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverHeat);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy", ReplicatedUsing = OnRep_OverHeatMax)
	FGameplayAttributeData OverHeatMax;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverHeatMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy", ReplicatedUsing = OnRep_OverCharge)
	FGameplayAttributeData OverCharge;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverCharge);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Energy", ReplicatedUsing = OnRep_OverChargeMax)
	FGameplayAttributeData OverChargeMax;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, OverChargeMax);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance", ReplicatedUsing = OnRep_HeatResistance)
	FGameplayAttributeData HeatResistance;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, HeatResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance", ReplicatedUsing = OnRep_ColdResistance)
	FGameplayAttributeData ColdResistance;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, ColdResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance", ReplicatedUsing = OnRep_PhysicalResistance)
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, PhysicalResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance", ReplicatedUsing = OnRep_LightningResistance)
	FGameplayAttributeData LightningResistance;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, LightningResistance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resistance", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UGB_AttributeSet, Armor);

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate);

	UFUNCTION()
	virtual void OnRep_Energy(const FGameplayAttributeData& OldEnergy);

	UFUNCTION()
	virtual void OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy);

	UFUNCTION()
	virtual void OnRep_EnergyRecoverRate(const FGameplayAttributeData& OldEnergyRecoverRate);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_StaminaRecoverRate(const FGameplayAttributeData& OldStaminaRecoverRate);
	
	UFUNCTION()
	virtual void OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower);

	UFUNCTION()
	virtual void OnRep_OverHeat(const FGameplayAttributeData& OldOverHeat);

	UFUNCTION()
	virtual void OnRep_OverHeatMax(const FGameplayAttributeData& OldOverHeatMax);

	UFUNCTION()
	virtual void OnRep_OverCharge(const FGameplayAttributeData& OldOverCharge);

	UFUNCTION()
	virtual void OnRep_OverChargeMax(const FGameplayAttributeData& OldOverChargeMax);

	UFUNCTION()
	virtual void OnRep_HeatResistance(const FGameplayAttributeData& OldHeatResistance);

	UFUNCTION()
	virtual void OnRep_ColdResistance(const FGameplayAttributeData& OldColdResistance);

	UFUNCTION()
	virtual void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance);

	UFUNCTION()
virtual void OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance);

	/*UFUNCTION()
virtual void OnRep_(const FGameplayAttribute& Old);
*/


	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_Speed(const FGameplayAttributeData& OldMoveSpeed);

	
};
