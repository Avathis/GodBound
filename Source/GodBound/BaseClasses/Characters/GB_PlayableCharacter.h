// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_Character.h"
#include "InputActionValue.h"
#include "GodBound/BaseClasses/BaseTypes.h"
#include "GB_PlayableCharacter.generated.h"

/**
 * 
 */
struct FInputActionInstance;
UCLASS()
class GODBOUND_API AGB_PlayableCharacter : public AGB_Character
{
	GENERATED_BODY()

	

	public:
	AGB_PlayableCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UGB_InputConfig* InputConfig;

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

	UFUNCTION(BlueprintImplementableEvent)
	void ParkourTransition(FHitResult Target, float RightValue, FGameplayTag EventTag);
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	UWidgetComponent* OverheadWidget;

protected:
	void BindASCInput();

	bool ASCInputBound = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	void Input_AbilityInputTagTriggered(const FInputActionInstance& InputActionInstance);

	void Input_Confirm(const FInputActionInstance& InputActionInstance);
	void Input_Cancel(const FInputActionInstance& InputActionInstance);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	void Move(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float MoveRightValue = 0.f;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	float MoveForwardValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	class UInputAction*LookAction;

	void Look(const FInputActionValue& Value);
	
	virtual void OnRep_PlayerState() override;
	
	
};
