// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GameplayEffectTypes.h"
#include "GB_PlayerState.generated.h"

class UGB_AbilitySystemComponent;
/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AGB_PlayerState();

	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystemComponent")
	UGB_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystemComponent")
	class UGB_AttributeSet* AttributeSet;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent")
	UGB_AbilitySystemComponent* GetGBAbilitySystemComponent() const{return AbilitySystemComponent;};
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	

	class UGB_AttributeSet* GetAttributeSetBase() const;

	void SetAttributeSet();

	UFUNCTION(BlueprintCallable, Category = "GASDocumentation|GDPlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetEnergy() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxEnergy() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetEnergyRecoverRate() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetStaminaRecoverRate() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetOverHeat();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetMaxOverHeat();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetOverHeatPercentage();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetStaminaPercentage();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetEnergyPercentage();


protected:

	FGameplayTag FallenTag;
	FGameplayTag DeadTag;
	

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle SpeedChangedDelegateHandle;
	FDelegateHandle MaxSpeedChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle EnergyChangedDelegateHandle;
	FDelegateHandle MaxEnergyChangedDelegateHandle;
	FDelegateHandle EnergyRegenRateChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;
	FDelegateHandle OverHeatChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void SpeedChanged(const FOnAttributeChangeData& Data);
	virtual void MaxSpeedChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void EnergyChanged(const FOnAttributeChangeData& Data);
	virtual void MaxEnergyChanged(const FOnAttributeChangeData& Data);
	virtual void EnergyRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void OverHeatChanged(const FOnAttributeChangeData& Data);
};
