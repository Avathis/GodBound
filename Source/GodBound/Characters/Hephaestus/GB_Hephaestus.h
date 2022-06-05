// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/Characters/GB_PlayableCharacter.h"
#include "GB_Hephaestus.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_Hephaestus : public AGB_PlayableCharacter
{
	GENERATED_BODY()
	public:

	void OnMoveForward(float Value);
	void MoveForward(float Value) override;

	void OnRight(float Value) override;
	void MoveRight(float Value) override;

	virtual void OnLookUp(float Value);
	void LookUp(float Value)override;

	virtual void OnTurnRight(float Value);
	void TurnRight(float Value)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float ChargeTurnRate = 10.f;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

};
