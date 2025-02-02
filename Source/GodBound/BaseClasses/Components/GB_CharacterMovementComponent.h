// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GB_CharacterMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct FGB_CharacterGroundInfo
{
	GENERATED_BODY()

	FGB_CharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};


UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Idle,
	EMS_Walking,
	EMS_Sprinting,
	EMS_Jumping,
	EMS_AbilitySprint,
	EMS_HephaestusChargeSprint
};

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FGB_SavedMove : public FSavedMove_Character
	{
		public:

		typedef FSavedMove_Character Super;

		///@brief Resets all saved variables.
		virtual void Clear() override;

		///@brief Store input commands in the compressed flags.
		virtual uint8 GetCompressedFlags() const override;

		///@brief This is used to check whether or not two moves can be combined into one.
		///Basically you just check to make sure that the saved variables are the same.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		///@brief Sets up the move before sending it to the server. 
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		///@brief Sets variables on character movement component before making a predictive correction.
		virtual void PrepMoveFor(class ACharacter* Character) override;

		// Sprint
		uint8 SavedRequestToStartSprinting : 1;

		// Aim Down Sights
		uint8 SavedRequestToStartADS : 1;
	};

	class FGB_NetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
		public:
		FGB_NetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);

		typedef FNetworkPredictionData_Client_Character Super;

		///@brief Allocates a new copy of our custom saved move
		virtual FSavedMovePtr AllocateNewMove() override;
	};

public:
	UGB_CharacterMovementComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementState MovementType = EMovementState::EMS_Walking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsClimbing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bIsBracedClimbing = false;

	UFUNCTION(BlueprintCallable)
	EMovementState GetMovementType();

	UFUNCTION(BlueprintCallable)
	void ChangeMovementState(EMovementState NewMovementState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
		float WalkMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
		float RunMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MovementSpeed")
		float AbilitySprintMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
		float SprintSpeedMultiplier = 1.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aim Down Sights")
		float ADSSpeedMultiplier = 0.5f;;

	uint8 RequestToStartSprinting : 1;
	uint8 RequestToStartADS : 1;

	virtual float GetMaxSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Lyra|CharacterMovement")
	const FGB_CharacterGroundInfo& GetGroundInfo();
	
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	// Sprint
	UFUNCTION(BlueprintCallable, Category = "Sprint")
		void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Sprint")
		void StopSprinting();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	bool bIsRunning = false;
	
	// Aim Down Sights
	UFUNCTION(BlueprintCallable, Category = "Aim Down Sights")
		void StartAimDownSights();
	UFUNCTION(BlueprintCallable, Category = "Aim Down Sights")
		void StopAimDownSights();
protected:
	FGB_CharacterGroundInfo CachedGroundInfo;
		
};
