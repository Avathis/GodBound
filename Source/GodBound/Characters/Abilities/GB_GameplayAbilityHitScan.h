// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/GB_GameplayAbility.h"

#include "GB_GameplayAbilityHitScan.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EGBAbilityTargetingSource : uint8
{
	// From the player's camera towards camera focus
	CameraTowardsFocus,
	// From the pawn's center, in the pawn's orientation
	PawnForward,
	// From the pawn's center, oriented towards camera focus
	PawnTowardsFocus,
	// From the weapon's muzzle or location, in the pawn's orientation
	WeaponForward,
	// From the weapon's muzzle or location, towards camera focus
	WeaponTowardsFocus,
	// Custom blueprint-specified source location
	Custom
};

UCLASS()
class GODBOUND_API UGB_GameplayAbilityHitScan : public UGB_GameplayAbility
{
	GENERATED_BODY()
public:

	
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	struct FRangedWeaponFiringInput
	{
		// Start of the trace
		FVector StartTrace;

		// End of the trace if aim were perfect
		FVector EndAim;

		// The direction of the trace if aim were perfect
		FVector AimDir;

		FVector StartAssistTrace;

		FVector EndAssistTrace;

		FVector AssistAimDir;

		// Can we play bullet FX for hits during this trace
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero)
			, EndAim(ForceInitToZero)
			, AimDir(ForceInitToZero)
		{
		}
	};
protected:
	static int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults);
	
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, OUT TArray<FHitResult>& OutHitResults) const;
	
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, OUT TArray<FHitResult>& OutHits) const;
	
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, OUT TArray<FHitResult>& OutHits);

	void WhatIsWrongWithThisFunction();

	int32 GetNumberOfBullets();

	float GetCalculatedSpreadAngle();

	virtual void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	
	virtual ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;

	void PerformLocalTargeting(OUT TArray<FHitResult>& OutHits);

	FVector GetWeaponTargetingSourceLocation() const;
	FTransform GetTargetingTransform(APawn* SourcePawn, EGBAbilityTargetingSource Source) const;

	FTransform GetAssistTargetingTransform(APawn* SourcePawn, EGBAbilityTargetingSource Source) const;

	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);

	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnRangedWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);

private:
	FDelegateHandle OnTargetDataReadyCallbackDelegateHandle;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	float MaxRange = 3000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	int Bullets = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	EGBAbilityTargetingSource BaseTargetingSource = EGBAbilityTargetingSource::WeaponTowardsFocus;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilityProperties", meta = (AllowPrivateAccess = "true"))
	FName MuzzleSocket = FName("");
};
