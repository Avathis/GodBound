// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	UBaseAttributeSet();
public:
	UPROPERTY()
	FGameplayAttributeData Health;

	UPROPERTY()
	FGameplayAttributeData Energy;

	UPROPERTY()
	FGameplayAttributeData Stamina;
};
