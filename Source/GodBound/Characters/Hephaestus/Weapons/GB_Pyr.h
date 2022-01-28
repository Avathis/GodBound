// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/GB_Weapon.h"
#include "GB_Pyr.generated.h"

UCLASS()
class GODBOUND_API AGB_Pyr : public AGB_Weapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGB_Pyr();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
