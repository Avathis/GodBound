// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Hephaestus.h"

#include "Net/UnrealNetwork.h"

void AGB_Hephaestus::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGB_Hephaestus, CombatSet);
}
