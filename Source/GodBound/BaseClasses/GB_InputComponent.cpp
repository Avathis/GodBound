// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_InputComponent.h"

UGB_InputComponent::UGB_InputComponent(const FObjectInitializer& ObjectInitializer)
{
}

void UGB_InputComponent::AddInputMappings(const UGB_InputConfig* InputConfig,
                                          UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	
}

void UGB_InputComponent::RemoveInputMappings(const UGB_InputConfig* InputConfig,
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem) const
{
	
}

void UGB_InputComponent::RemoveBinds(TArray<uint32>& BindHandles)
{
	for (uint32 Handle : BindHandles)
	{
		RemoveBindingByHandle(Handle);
	}
	BindHandles.Reset();
}
