// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/Characters/GB_PlayableCharacter.h"
#include "GodBound/BaseClasses/BaseTypes.h"
#include "GB_Hephaestus.generated.h"

/**
 * 
 */

 //OOC - Out of Combat
 //M   - Mage
 //B   - Blacksmith
 //W   - Weapons
 //F   - Fighter/Brawl



UCLASS()
class GODBOUND_API AGB_Hephaestus : public AGB_PlayableCharacter
{
	GENERATED_BODY()
	public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifeTimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Mode", Replicated)
	EHephaestusSet CombatSet;

	UPROPERTY(EditAnywhere, BlueprintReadWRite, Category = "Combat Mode")
	TMap<EHephaestusSet, UGB_AbilitySet*> AbilitySetsInMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ChargeTurnRate = 10.f;


};
