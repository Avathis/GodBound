// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_Character.h"
#include "GodBound/BaseClasses/BaseTypes.h"
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
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanManuallyMove = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanManuallyLookAround = true;
	
	virtual void MoveForward(float Value);
	virtual void MoveRight(float Value);
	virtual void TurnRight(float Value);
	virtual void TurnRightAtRate(float Value);
	virtual void LookUp(float Value);
	virtual void LookUpAtRate(float Value);

	void Interact();

	virtual void EnterCombat() override;

	virtual void ExitCombat() override;

	/*UFUNCTION(Blueprintcallable)
	FHitResult HitTraceFromCamera(float MaxRange = 4000.f);*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AGB_Weapon* ActiveWeapon;
	
	UFUNCTION()
	void EquipWeapon(AGB_Weapon* WeaponToEquip);

	UFUNCTION(NetMulticast, Reliable)
	void ServerEquipWeapon(AGB_Weapon* WeaponToEquip);

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeAbilities();

	UFUNCTION(BlueprintImplementableEvent)
	void InitializeHealthBarAttributes();
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UWidgetComponent* OverheadWidget;

	

protected:
	void BindASCInput();

	bool ASCInputBound = false;
	
	virtual void OnRep_PlayerState() override;
	
	
};
