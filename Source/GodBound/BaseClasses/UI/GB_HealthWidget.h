// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GB_HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_HealthWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHealthPercentage(float HeealthPercentage);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetManaPercentage(float ManaPercentage);
	
};
