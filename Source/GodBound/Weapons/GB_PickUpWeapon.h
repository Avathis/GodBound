// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/GB_Weapon.h"
#include "GB_PickUpWeapon.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_PickUpWeapon : public AGB_Weapon
{
	GENERATED_BODY()
public:
	AGB_PickUpWeapon();
	virtual void BeginPlay() override;
protected:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	virtual void OnSphereOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AddedComponents", meta = (AllowPrivateAccess = true))
	class USphereComponent* AreaSphere;

	UPROPERTY(EditAnywhere, Category = "AddedComponents")
	class UWidgetComponent* PickupWidget;
	
};
