// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GodBound/BaseClasses/Components/GB_AbilitySystemComponent.h"
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
	// 11
	Control			UMETA(DisplayName = "Control")
};

UCLASS(config=Game)
class GODBOUND_API AGB_Character : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGB_Character(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	FVector TraceFromCamera();

	UFUNCTION(BlueprintCallable)
	virtual void EnterCombat();

	UFUNCTION(BlueprintCallable)
	virtual void ExitCombat();

	UFUNCTION(BlueprintCallable)
	virtual UGB_CharacterMovementComponent* GetAdvMovementComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraBoom", meta = (AllowPrivateAccess = "true"))
	class UGB_SpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UGB_CameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	class AGB_PlayerController* PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VisionBlocker", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* CameraCollisionBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UGB_CharacterMovementComponent* CharacterMovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ForwardAxis = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RightAxis = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate = 45.f;
	
private:
	

	float Speed = 0.f;

	
	
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	FHitResult FireDebugBeam();

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void GrantAbility(TSubclassOf<class UGB_GameplayAbility> AbilityClass, int32 Level, int32 InputCode);

	UFUNCTION(BlueprintCallable, Category = "Abilities")
	void ActivateAbility(int32 InputCode);
	/*UPROPERTIES*/

	
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

	//HealthBar

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EnemyHealthBar")
	TSubclassOf<class UGB_HealthWidget> UIHealthBarClass;

	UPROPERTY()
	UGB_HealthWidget* UIHealthBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "EnemyHealthBar")
	class UWidgetComponent* UIHealthBarComponent;

	UFUNCTION()
	void InitializeHealthBar();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes", meta = (AllowPrivateAccess = "true"))
	const class UGB_AttributeSet* Attributes ;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class UGB_AbilitySystemComponent* AbilitySystemComponent;

	virtual UGB_AbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return AbilitySystemComponent;
	}

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
    float GetEnergyRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
    float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
	float GetHealthPercentage();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetOverHeatPercentage();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetStaminaPercentage();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		float GetEnergyPercentage();


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

};
