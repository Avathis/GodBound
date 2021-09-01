// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Components/GB_AbilitySystemComponent.h"
#include "GB_Character.generated.h"

UENUM(BlueprintType)
enum class GBAbilityInputID : uint8
{
	//0
	None			UMETA(DisplayName = "None"),
	//1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	//2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	//3 Shift
	Sprint			UMETA(DisplayName = "Sprint"),
	//4 Space
	Jump			UMETA(DisplayName = "Jump"),
	//5 
	Dash			UMETA(DisplayName = "Dash"),
	//6 LMB
	Ability1		UMETA(DisplayName = "Ability1"),
	//7 RMB
	Ability2		UMETA(DisplayName = "Ability2"),
	//8 Q
	Ability3		UMETA(DisplayName = "Ability3"),
	//9 E
	Ability4		UMETA(DisplayName = "Ability4"),
	//10 R
	Ability5		UMETA(DisplayName = "Ability5"),
	//
};

UCLASS()
class GODBOUND_API AGB_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGB_Character(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	FVector TraceFromCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraBoom", meta = (AllowPrivateAccess = "true"))
	class UGB_SpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UGB_CameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	class AGB_PlayerController* PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisionBlocker", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CameraCollisionBox;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UGB_CharacterMovementComponent* CharacterMovementComponent;
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ForwardAxis = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RightAxis = 0;
	
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
	void StartSprinting();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void StopSprinting();
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FHitResult FireDebugBeam();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GrantAbility(TSubclassOf<class UGB_GameplayAbility> AbilityClass, int32 Level, int32 InputCode);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ActivateAbility(int32 InputCode);
	/*UPROPERTIES*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	const class UGB_AttributeSet* Attributes ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class UGB_AbilitySystemComponent* AbilitySystemComponent;

	virtual UGB_AbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
