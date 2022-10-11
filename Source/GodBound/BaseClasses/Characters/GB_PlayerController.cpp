// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PlayerController.h"

#include "AbilitySystemComponent.h"
#include "Blueprint/UserWidget.h"
#include "GodBound/Player/GB_PlayerState.h"

void AGB_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	if (HUDOverlay)
	{
		HUDOverlay->AddToViewport();
		HUDOverlay->SetVisibility(ESlateVisibility::Visible);
	}
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
