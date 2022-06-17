// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GB_AssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_AssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UGB_AssetManager& Get();

		void StartInitialLoading() override;
};
