// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseTypes.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EHephaestusSet : uint8
{
	EHS_OOC,
	EHS_M,
	EHS_B,
	EHS_W,
	EHS_F
};

UCLASS()
class GODBOUND_API UBaseTypes : public UObject
{
	GENERATED_BODY()
	
};
