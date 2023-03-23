// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GB_PlayerController.generated.h"

/**
 * 
 */
class UGB_AbilitySystemComponent;
UCLASS()
class GODBOUND_API AGB_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	void CreateHUD();

	class UGB_HUDWidget* GetHUD();

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	AGB_PlayerState* GetGBPlayerState() const;
	
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	UGB_AbilitySystemComponent* GetGBAbilitySystemComponent() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		TSubclassOf<class UUserWidget> HUDOverlayAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
		UGB_HUDWidget* HUDOverlay;

protected:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_PlayerState() override;

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;
};
