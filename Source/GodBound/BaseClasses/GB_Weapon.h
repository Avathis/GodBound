// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GB_Weapon.generated.h"

UENUM()
enum class EWeaponState : uint8
{
	EWS_PickUpable UMETA(DisplayName = "PickUpable"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_Sheathed UMETA(DisplayName = "Sheathed"),
	EWS_Thrown UMETA(DisplayName = "Thrown"),
	EWS_Dropped UMETA(DisplayName = "Dropped"),

	EWS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class GODBOUND_API AGB_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGB_Weapon();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AddedComponents")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AddedComponents")
	class UProjectileMovementComponent* ProjectileMovementComponent;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables", meta = (AllowPrivateAccess = true))
	EWeaponState WeaponState;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
