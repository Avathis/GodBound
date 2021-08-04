// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "PlayableCharacterBase.generated.h"

UCLASS()
class GODBOUND_API APlayableCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayableCharacterBase();

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnRight(float Value);
	void TurnRightAtRate(float Value);
	void LookUp(float Value);
	void LookUpAtRate(float Value);

	void PressSpace();
	void ReleaseSpace();
	void PressShift();
	void ReleaseShift();
	void PressLMB();
	void ReleaseLMB();
	void PressRMB();
	void ReleaseRMB();
	void Interact();
	void PressCtrl();
	void ReleaseCtrl();

	bool bShiftPressed = false;
	bool bSpacePressed = false;
	bool bLMBPressed = false;
	bool bRMBPressed = false;
	bool bCtrlPressed = false;

	float Speed = 0.f;

	/*UFUNCTIONS*/

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	const void FireDebugBeam();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GrantAbility(TSubclassOf<class UGameplayAbilityBase> AbilityClass, int32 Level, int32 InputCode);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ActivateAbility(int32 InputCode);
	/*UPROPERTIES*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	const class UBaseAttributeSet* Attributes ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraBoom", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	class APlayableCharacterController* PlayerController;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
