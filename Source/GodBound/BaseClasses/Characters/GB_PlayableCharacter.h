// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_Character.h"
#include "GB_PlayableCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_PlayableCharacter : public AGB_Character
{
	GENERATED_BODY()

	

	public:
	AGB_PlayableCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void OnRight(float Value);
	void MoveRight(float Value);
	void TurnRight(float Value);
	void TurnRightAtRate(float Value);
	void LookUp(float Value);
	void LookUpAtRate(float Value);

	void PressCtrl();
	void ReleaseCtrl();

	bool bCtrlPressed;

	void Interact();

	UFUNCTION(Blueprintcallable)
	FHitResult HitTraceFromCamera(float MaxRange = 4000.f);
	
};
