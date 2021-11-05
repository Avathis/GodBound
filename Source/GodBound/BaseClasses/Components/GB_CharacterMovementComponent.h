// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GB_CharacterMovementComponent.generated.h"

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	EMS_Idle,
	EMS_Walking,
	EMS_Sprinting,
	EMS_AbilitySprint
};

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
public:
	UGB_CharacterMovementComponent(const FObjectInitializer& ObjectInitializer);
	EMovementState MovementType = EMovementState::EMS_Walking;

	UFUNCTION(BlueprintCallable)
	void ChangeMovementState(EMovementState NewMovementState);
};
