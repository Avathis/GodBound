// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/Animation/GB_AnimInstance.h"
#include "GodBound/BaseClasses/BaseTypes.h"
#include "GB_HephaestusAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_HephaestusAnimInstance : public UGB_AnimInstance
{
	GENERATED_BODY()

	public:
/*
	virtual void NativeInitializeAnimation() override;
		
	UFUNCTION(BlueprintCallable, Category = AnimationProperties)
	void UpdateAnimationProperties();
*/
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	EHephaestusSet CombatSet;
	
	
};
