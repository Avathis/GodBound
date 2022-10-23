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

UENUM(BlueprintType)
enum class ETurningInPlace : uint8
{
	ETIP_Left UMETA(DisplayName = "TurningLeft"),
	EITP_Right UMETA(DisplayName = "TurningRight"),
	ETIP_NotTurning UMETA(DisplayName = "NotTurning"),

	ETIP_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class GODBOUND_API UBaseTypes : public UObject
{
	GENERATED_BODY()
	
};
