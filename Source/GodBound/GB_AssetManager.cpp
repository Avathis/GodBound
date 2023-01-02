// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_AssetManager.h"
#include "GB_AbilitySystemGlobals.h"

UGB_AssetManager& UGB_AssetManager::Get()
{
	UGB_AssetManager* Singleton = Cast<UGB_AssetManager>(GEngine->AssetManager);

	if (Singleton)
	{
		return *Singleton;
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("Invalid AssetManager in DefaultEngine.ini, must be UGB_AssetManager!"));
		return *NewObject<UGB_AssetManager>();	 // never calls this
	}
}

void UGB_AssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}