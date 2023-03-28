// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "InputTriggers.h"
#include "GB_InputConfig.h"
#include "Misc/AssertionMacros.h"
#include "GB_InputComponent.generated.h"

/**
 * 
 */

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UObject;

UCLASS(Config = Input)
class GODBOUND_API UGB_InputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:

	UGB_InputComponent(const FObjectInitializer& ObjectInitializer);

	void AddInputMappings(const UGB_InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;
	void RemoveInputMappings(const UGB_InputConfig* InputConfig, UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const;

	template<class UserClass, typename FuncType>
	void BindNativeAction(const UGB_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename TriggeredFuncType>
	void BindAbilityActions(const UGB_InputConfig* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc,TArray<uint32>& BindHandles);
	/*
	 *template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	 *void BindAbilityActions(const UGB_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
	*/
	void RemoveBinds(TArray<uint32>& BindHandles);
	
};

template<class UserClass, typename FuncType>
void UGB_InputComponent::BindNativeAction(const UGB_InputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
}

template<class UserClass, typename TriggeredFuncType>
void UGB_InputComponent::BindAbilityActions(const UGB_InputConfig* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FGB_InputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (TriggeredFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, TriggeredFunc).GetHandle());
			}
		}
	}
}
/*template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UGB_InputComponent::BindAbilityActions(const UGB_InputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	for (const FGB_InputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}

			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}
}*/
