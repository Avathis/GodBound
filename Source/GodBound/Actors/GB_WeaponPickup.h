// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/Actors/GB_Interactable.h"
#include "GB_WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_WeaponPickup : public AGB_Interactable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AGB_Weapon> WeaponClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", Replicated)
	AGB_Weapon* WeaponRef;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void Interact(AActor* Character) override;

	virtual void OnEndInteract() override;

	void PickUp();

	void EquipWeapon();
	

protected:
	virtual void BeginPlay() override;
	
};
