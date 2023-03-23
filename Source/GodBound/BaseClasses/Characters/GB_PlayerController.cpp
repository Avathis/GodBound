// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayerController.h"

#include "AbilitySystemComponent.h"
#include "GodBound/BaseClasses/Components/GB_AbilitySystemComponent.h"
#include "GodBound/BaseClasses/UI/GB_HUDWidget.h"
#include "GodBound/Player/GB_PlayerState.h"

void AGB_PlayerController::CreateHUD()
{
	if(HUDOverlay)
	{
		return;
	}
	if(!HUDOverlayAsset)
	{
		return;
	}
	if (!IsLocalPlayerController())
	{
		return;
	}
	AGB_PlayerState* PState = GetPlayerState<AGB_PlayerState>();
	if(!PState)
	{
		return;
	}
	HUDOverlay = CreateWidget<UGB_HUDWidget>(this, HUDOverlayAsset);
	HUDOverlay->AddToViewport();
	
	HUDOverlay->SetCurrentHealth(PState->GetHealth());
	HUDOverlay->SetMaxHealth(PState->GetMaxHealth());
	HUDOverlay->SetHealthPercentage(PState->GetHealthPercentage());

	HUDOverlay->SetOverHeat(PState->GetOverHeat());
	HUDOverlay->SetMaxOverHeat(PState->GetMaxOverHeat());
	HUDOverlay->SetOverHeatPercentage(PState->GetOverHeatPercentage());
}

UGB_HUDWidget* AGB_PlayerController::GetHUD()
{
	return HUDOverlay;
}

AGB_PlayerState* AGB_PlayerController::GetGBPlayerState() const
{
	return CastChecked<AGB_PlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UGB_AbilitySystemComponent* AGB_PlayerController::GetGBAbilitySystemComponent() const
{
	const AGB_PlayerState* GBPS = GetGBPlayerState();
	return (GBPS ? GBPS->GetGBAbilitySystemComponent() : nullptr);
}

void AGB_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	/*
	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UGB_HUDWidget>(this, HUDOverlayAsset);
	}

	if (HUDOverlay)
	{
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	*/
}

void AGB_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	AGB_PlayerState* PS = GetPlayerState<AGB_PlayerState>();
	if(PS)
	{
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AGB_PlayerController::OnRep_PlayerState()
{

	Super::OnRep_PlayerState();
	CreateHUD();
}

void AGB_PlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UGB_AbilitySystemComponent* GBASC = GetGBAbilitySystemComponent())
	{
		GBASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
