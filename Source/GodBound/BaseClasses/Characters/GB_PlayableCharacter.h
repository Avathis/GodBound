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

	virtual void MoveForward(float Value);
	virtual void OnRight(float Value);
	virtual void MoveRight(float Value);
	virtual void TurnRight(float Value);
	virtual void TurnRightAtRate(float Value);
	virtual void LookUp(float Value);
	virtual void LookUpAtRate(float Value);

	virtual void PressCtrl();
	virtual void ReleaseCtrl();

	bool bCtrlPressed;

	void Interact();

	UFUNCTION(Blueprintcallable)
	FHitResult HitTraceFromCamera(float MaxRange = 4000.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AGB_Weapon* ActiveWeapon;
	
};
