// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor_GroundTrace.h"
#include "GB_TA_PowerJump.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_TA_PowerJump : public AGameplayAbilityTargetActor_GroundTrace
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IncreaseRange")
		float MaxRangeAdder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IncreaseRange")
		float MaxRangeLimit;
	
};
