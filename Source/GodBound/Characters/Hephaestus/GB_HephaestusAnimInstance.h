// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GB_HephaestusAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_HephaestusAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	public:

	//UGB_HephaestusAnimInstance();

	virtual void NativeInitializeAnimation() override;
		
	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();

	UFUNCTION(BlueprintCallable, Category = animationProperties)
	bool CheckForState(struct FGameplayTag TagToCheck);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float MovementSpeed;
    	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	bool bIsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bDied;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AGB_Character* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	bool bIsInCombat;

	UPROPERTY(EditAnywhere, BlueprintReadWRite, Category = Movement)
	bool bIsInSpecialSprint;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float ForwardAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RightAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Direction;
};
