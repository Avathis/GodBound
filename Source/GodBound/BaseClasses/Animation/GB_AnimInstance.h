// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GB_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
		
	UFUNCTION(BlueprintCallable, Category = AnimationProperties, meta=(BlueprintThreadSafe))
	void UpdateAnimationProperties();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MovementSpeed;
    	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bDied;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	class AGB_Character* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsInCombat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsInSpecialSprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float ForwardAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RightAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Direction;
	
};
