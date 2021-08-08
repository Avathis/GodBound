// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TestingAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UTestingAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	public:
	virtual void NativeInitializeAnimation() override;
		
	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();
    	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;
    	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bDied;
    
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;
    	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APlayableCharacterBase* MainCharacter;
	
};
