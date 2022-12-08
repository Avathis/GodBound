// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GB_AbilityTaskAttributeChanged.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);
UCLASS()
class GODBOUND_API UGB_AbilityTaskAttributeChanged : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	public:
	UPROPERTY(BlueprintAssignable)
    	FOnAttributeChanged OnAttributeChanged;
    	
    	// Listens for an attribute changing.
    	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    	static UGB_AbilityTaskAttributeChanged* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAttribute Attribute);
    

    	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
    	static UGB_AbilityTaskAttributeChanged	* ListenForAttributesChange(UAbilitySystemComponent* AbilitySystemComponent, TArray<FGameplayAttribute> Attributes);

    	UFUNCTION(BlueprintCallable)
    	void EndTask();
    
    protected:
    	UPROPERTY()
    	UAbilitySystemComponent* ASC;
    
    	FGameplayAttribute AttributeToListenFor;
    	TArray<FGameplayAttribute> AttributesToListenFor;
    
    	void AttributeChanged(const FOnAttributeChangeData& Data);
};
