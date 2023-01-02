// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AttributeSet.h"

#include "Net/UnrealNetwork.h"

void UGB_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, Energy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, MaxEnergy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, EnergyRecoverRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, StaminaRecoverRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, AbilityPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, OverHeat, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, OverHeatMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, OverCharge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, OverChargeMax, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, HeatResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, ColdResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UGB_AttributeSet, Speed, COND_None, REPNOTIFY_Always);
}

void UGB_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, Health, OldHealth);
}

void UGB_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, MaxHealth ,OldMaxHealth)
}

void UGB_AttributeSet::OnRep_HealthRegenRate(const FGameplayAttributeData& OldHealthRegenRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, HealthRegenRate ,OldHealthRegenRate)
}

void UGB_AttributeSet::OnRep_Energy(const FGameplayAttributeData& OldEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet,  Energy,OldEnergy)
}

void UGB_AttributeSet::OnRep_MaxEnergy(const FGameplayAttributeData& OldMaxEnergy)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet,MaxEnergy  ,OldMaxEnergy)
}

void UGB_AttributeSet::OnRep_EnergyRecoverRate(const FGameplayAttributeData& OldEnergyRecoverRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet,EnergyRecoverRate  ,OldEnergyRecoverRate);
}

void UGB_AttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, Stamina ,OldStamina);
}

void UGB_AttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet,MaxStamina  ,OldMaxStamina);
}

void UGB_AttributeSet::OnRep_StaminaRecoverRate(const FGameplayAttributeData& OldStaminaRecoverRate)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, StaminaRecoverRate ,OldStaminaRecoverRate);
}

void UGB_AttributeSet::OnRep_AbilityPower(const FGameplayAttributeData& OldAbilityPower)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, AbilityPower,OldAbilityPower);
}

void UGB_AttributeSet::OnRep_OverHeat(const FGameplayAttributeData& OldOverHeat)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, OverHeat, OldOverHeat);
}

void UGB_AttributeSet::OnRep_OverHeatMax(const FGameplayAttributeData& OldOverHeatMax)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, OverHeatMax,OldOverHeatMax);
}

void UGB_AttributeSet::OnRep_OverCharge(const FGameplayAttributeData& OldOverCharge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, OverCharge,OldOverCharge);
}

void UGB_AttributeSet::OnRep_OverChargeMax(const FGameplayAttributeData& OldOverChargeMax)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, OverChargeMax,OldOverChargeMax);
}

void UGB_AttributeSet::OnRep_HeatResistance(const FGameplayAttributeData& OldHeatResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, HeatResistance,OldHeatResistance);
}

void UGB_AttributeSet::OnRep_ColdResistance(const FGameplayAttributeData& OldColdResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, ColdResistance,OldColdResistance);
}

void UGB_AttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, PhysicalResistance,OldPhysicalResistance);
}

void UGB_AttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, LightningResistance,OldLightningResistance);
}

void UGB_AttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, Armor ,OldArmor);
}

void UGB_AttributeSet::OnRep_Speed(const FGameplayAttributeData& OldMoveSpeed)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGB_AttributeSet, Speed , OldMoveSpeed);
}
