// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_Character.h"
#include "GB_EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_EnemyCharacter : public AGB_Character
{
	GENERATED_BODY()

	protected:
	virtual void BeginPlay() override;

	public:
	AGB_EnemyCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaSeconds) override;

	//void ShowHealthBar();
	
};
