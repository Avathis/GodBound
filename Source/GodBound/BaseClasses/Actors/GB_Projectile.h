// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GB_Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
UCLASS()
class GODBOUND_API AGB_Projectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AGB_Projectile();

	virtual void PostNetReceiveLocationAndRotation() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereHitBox;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BoxComponent;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
