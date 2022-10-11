// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GB_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		UUserWidget* HUDOverlay;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	
};
