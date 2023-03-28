// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_InputConfig.h"

UGB_InputConfig::UGB_InputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UGB_InputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FGB_InputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

const UInputAction* UGB_InputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FGB_InputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]."), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

FGameplayTag UGB_InputConfig::FindInputTagForNativeInputAction(const UInputAction* InputAction, bool bLogNotFound) const
{
	for (const FGB_InputAction& Action : NativeInputActions)
	{
		if(Action.InputAction)
		{
			if(Action.InputAction==InputAction)
			{
				return Action.InputTag;
			}
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputTag for Action on InputConfig [%s]."),*GetNameSafe(this));
	}
	return FGameplayTag::RequestGameplayTag("");
}

FGameplayTag UGB_InputConfig::FindInputTagForAbilityInputAction(const UInputAction* InputAction,
	bool bLogNotFound) const
{
	for (const FGB_InputAction& Action : AbilityInputActions)
	{
		if(Action.InputAction)
		{
			if(Action.InputAction==InputAction)
			{
				return Action.InputTag;
			}
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputTag for Action on InputConfig [%s]."),*GetNameSafe(this));
	}
	return FGameplayTag::RequestGameplayTag("");
}
