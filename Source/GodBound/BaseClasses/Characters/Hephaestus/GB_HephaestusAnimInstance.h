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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class AGB_Character* MainCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float ForwardAxis;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	float RightAxis;
};
