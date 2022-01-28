// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GB_Weapon.generated.h"

UCLASS()
class GODBOUND_API AGB_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGB_Weapon();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AddedComponents")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AaddedComponents")
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
