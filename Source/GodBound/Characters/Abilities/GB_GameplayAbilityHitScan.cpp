// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_GameplayAbilityHitScan.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

namespace GBConsoleVariables
{
	static float DrawBulletTracesDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletTraceDuraton(
		TEXT("GodBound.Weapon.DrawBulletTraceDuration"),
		DrawBulletTracesDuration,
		TEXT("Should we do debug drawing for bullet traces (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletHitDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHits(
		TEXT("GodBound.Weapon.DrawBulletHitDuration"),
		DrawBulletHitDuration,
		TEXT("Should we do debug drawing for bullet impacts (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletHitRadius = 3.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHitRadius(
		TEXT("GodBound.Weapon.DrawBulletHitRadius"),
		DrawBulletHitRadius,
		TEXT("When bullet hit debug drawing is enabled (see DrawBulletHitDuration), how big should the hit radius be? (in uu)"),
		ECVF_Default);
}


FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	if (ConeHalfAngleRad > 0.f)
	{
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Dir.Rotation();
		FQuat DirQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f, 0.0, AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;
		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
}


bool UGB_GameplayAbilityHitScan::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGB_GameplayAbilityHitScan::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// Bind target data callback
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	OnTargetDataReadyCallbackDelegateHandle = MyAbilityComponent->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).AddUObject(this, &ThisClass::OnTargetDataReadyCallback);
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGB_GameplayAbilityHitScan::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (IsEndAbilityValid(Handle, ActorInfo))
	{
		if (ScopeLockCount > 0)
		{
			WaitingToExecute.Add(FPostLockDelegate::CreateUObject(this, &ThisClass::EndAbility, Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled));
			return;
		}

		UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
		check(MyAbilityComponent);
		
		MyAbilityComponent->AbilityTargetDataSetDelegate(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey()).Remove(OnTargetDataReadyCallbackDelegateHandle);
		MyAbilityComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());

		Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	}
}

int32 UGB_GameplayAbilityHitScan::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults)
{
	for (int32 Idx = 0; Idx < HitResults.Num(); ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Idx;
		}
		else
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();
			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				return Idx;
			}
		}
	}
	return INDEX_NONE;
}

FHitResult UGB_GameplayAbilityHitScan::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace,
	float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;
	
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;
	AddAdditionalTraceIgnoreActors(TraceParams);

	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);

	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity, TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (HitResults.Num() > 0)
	{
		for (FHitResult& CurHitResult : HitResults)
		{
			auto Pred = [&CurHitResult](const FHitResult& Other)
			{
				return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
			};

			if (!OutHitResults.ContainsByPredicate(Pred))
			{
				OutHitResults.Add(CurHitResult);
			}
		}

		Hit = OutHitResults.Last();
	}
	else
	{
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}

	return Hit;
}

FHitResult UGB_GameplayAbilityHitScan::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace,
	float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const
{
#if ENABLE_DRAW_DEBUG
	if (GBConsoleVariables::DrawBulletTracesDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor::Red, false, GBConsoleVariables::DrawBulletTracesDuration, 0, DebugThickness);
	}
#endif 
	FHitResult Impact;
	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		Impact = WeaponTrace(StartTrace, EndTrace, /*SweepRadius=*/ 0.0f, bIsSimulated, /*out*/ OutHits);
	}

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, /*out*/ SweepHits);
			
			const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);
			if (SweepHits.IsValidIndex(FirstPawnIdx))
			{
				bool bUseSweepHits = true;
				for (int32 Idx = 0; Idx < FirstPawnIdx; ++Idx)
				{
					const FHitResult& CurHitResult = SweepHits[Idx];

					auto Pred = [&CurHitResult](const FHitResult& Other)
					{
						return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
					};
					if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
					{
						bUseSweepHits = false;
						break;
					}
				}

				if (bUseSweepHits)
				{
					OutHits = SweepHits;
				}
			}
		}
	}

	return Impact;
}

void UGB_GameplayAbilityHitScan::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits)
{
	const int32 BulletsPerCartridge = GetNumberOfBullets();

	for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
	{
		const float BaseSpreadAngle = GetCalculatedSpreadAngle();
		const float ActualSpreadAngle = BaseSpreadAngle * 1.f;

		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);
		const FVector BulletDir = VRandConeNormalDistribution(InputData.AimDir, HalfSpreadAngleInRadians, 1.f);

		const FVector EndTrace = InputData.StartTrace + (BulletDir * MaxRange);
		FVector HitLocation = EndTrace;
		

		
		if(BaseTargetingSource == EGBAbilityTargetingSource::WeaponTowardsFocus)
		{
			FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ GetAvatarActorFromActorInfo());
			TraceParams.bReturnPhysicalMaterial = true;
			AddAdditionalTraceIgnoreActors(TraceParams);
			//TraceParams.bDebugQuery = true;
			const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, false);
			FHitResult HitResult;
			GetWorld()->LineTraceSingleByChannel(HitResult,InputData.StartAssistTrace,InputData.EndAssistTrace,TraceChannel);
			if(HitResult.IsValidBlockingHit())
			{
				FVector DirectionOfTrace = UKismetMathLibrary::GetDirectionUnitVector(HitResult.TraceStart, HitResult.Location);
				FVector SafeTraceEnd = HitResult.TraceStart+DirectionOfTrace*(HitResult.Distance+10);
				HitLocation = SafeTraceEnd;
			}
			else
			{
				HitLocation = HitResult.TraceEnd;
			}
			if (GBConsoleVariables::DrawBulletHitDuration > 0.0f)
			{
				DrawDebugLine(GetWorld(), InputData.StartAssistTrace, InputData.EndAssistTrace, FColor::Green, false, GBConsoleVariables::DrawBulletTracesDuration, 0, 2.f);
			}
		}

		TArray<FHitResult> AllImpacts;

		FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, HitLocation, 1.f, false, AllImpacts);

		const AActor* HitActor = Impact.GetActor();

		if (HitActor)
		{
#if ENABLE_DRAW_DEBUG
			if (GBConsoleVariables::DrawBulletHitDuration > 0.0f)
			{
				DrawDebugPoint(GetWorld(), Impact.ImpactPoint, GBConsoleVariables::DrawBulletHitRadius, FColor::Red, false, GBConsoleVariables::DrawBulletHitRadius);
			}
#endif

			if (AllImpacts.Num() > 0)
			{
				OutHits.Append(AllImpacts);
			}

			HitLocation = Impact.ImpactPoint;
		}
		if (OutHits.Num() == 0)
		{
			if (!Impact.bBlockingHit)
			{
				Impact.Location = EndTrace;
				Impact.ImpactPoint = EndTrace;
			}

			OutHits.Add(Impact);
		}
	}
}

void UGB_GameplayAbilityHitScan::WhatIsWrongWithThisFunction()
{
	
}

int32 UGB_GameplayAbilityHitScan::GetNumberOfBullets()
{
	return Bullets;
}

float UGB_GameplayAbilityHitScan::GetCalculatedSpreadAngle()
{
	return 0.f;
}

void UGB_GameplayAbilityHitScan::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;
		Avatar->GetAttachedActors(/*out*/ AttachedActors);
		TraceParams.AddIgnoredActors(AttachedActors);
	}
}

ECollisionChannel UGB_GameplayAbilityHitScan::DetermineTraceChannel(FCollisionQueryParams& TraceParams,
	bool bIsSimulated) const
{
	return ECollisionChannel::ECC_GameTraceChannel1;
}

void UGB_GameplayAbilityHitScan::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	
	if (AvatarPawn && AvatarPawn->IsLocallyControlled())
	{
		FRangedWeaponFiringInput InputData;
		InputData.bCanPlayBulletFX = (AvatarPawn->GetNetMode() != NM_DedicatedServer);
		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn, BaseTargetingSource);
		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();
		InputData.EndAim = InputData.StartTrace + InputData.AimDir * MaxRange;
		const FTransform TargetAssistTransform = GetAssistTargetingTransform(AvatarPawn, BaseTargetingSource);
		InputData.AssistAimDir = TargetAssistTransform.GetUnitAxis(EAxis::X);
		InputData.StartAssistTrace = TargetAssistTransform.GetTranslation();
		InputData.EndAssistTrace = InputData.StartAssistTrace+InputData.AssistAimDir * MaxRange;
		
		

#if ENABLE_DRAW_DEBUG
		if (GBConsoleVariables::DrawBulletTracesDuration > 0.0f)
		{
			static float DebugThickness = 2.0f;
			DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir * 100.0f), FColor::Yellow, false, GBConsoleVariables::DrawBulletTracesDuration, 0, DebugThickness);
		}
#endif

		TraceBulletsInCartridge(InputData, /*out*/ OutHits);

		
	}
}

FVector UGB_GameplayAbilityHitScan::GetWeaponTargetingSourceLocation() const
{
	ACharacter* const AvatarCharacter = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	check(AvatarCharacter);

	
	const FVector SourceLoc = AvatarCharacter->GetMesh()->GetSocketLocation(MuzzleSocket);
	const FQuat SourceRot = AvatarCharacter->GetActorQuat();

	FVector TargetingSourceLocation = SourceLoc;

	//@TODO: Add an offset from the weapon instance and adjust based on pawn crouch/aiming/etc...

	return TargetingSourceLocation;
}

FTransform UGB_GameplayAbilityHitScan::GetTargetingTransform(APawn* SourcePawn, EGBAbilityTargetingSource Source) const
{
	check(SourcePawn);
	AController* SourcePawnController = SourcePawn->GetController();
	
	check(Source != EGBAbilityTargetingSource::Custom);

	const FVector ActorLoc = SourcePawn->GetActorLocation();
	FQuat AimQuat = SourcePawn->GetActorQuat();
	AController* Controller = SourcePawn->Controller;
	FVector SourceLoc;

	double FocalDistance = 1024.0f;
	FVector FocalLoc;

	FVector CamLoc;
	FRotator CamRot;
	bool bFoundFocus = false;


	if ((Controller != nullptr) && ((Source == EGBAbilityTargetingSource::CameraTowardsFocus) || (Source == EGBAbilityTargetingSource::PawnTowardsFocus) || (Source == EGBAbilityTargetingSource::WeaponTowardsFocus)))
	{
		bFoundFocus = true;

		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC != nullptr)
		{
			PC->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);
		}
		else
		{
			SourceLoc = GetWeaponTargetingSourceLocation();
			CamLoc = SourceLoc;
			CamRot = Controller->GetControlRotation();
		}
		
		FVector AimDir = CamRot.Vector().GetSafeNormal();
		FocalLoc = CamLoc + (AimDir * FocalDistance);
		
		if (PC)
		{
			const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
			CamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);
			FocalLoc = CamLoc + (AimDir * FocalDistance);
		}
		
		if (Source == EGBAbilityTargetingSource::CameraTowardsFocus)
		{
			return FTransform(CamRot, CamLoc);
		}
	}

	if ((Source == EGBAbilityTargetingSource::WeaponForward) || (Source == EGBAbilityTargetingSource::WeaponTowardsFocus))
	{
		SourceLoc = GetWeaponTargetingSourceLocation();
	}
	else
	{
		SourceLoc = ActorLoc;
	}

	if (bFoundFocus && ((Source == EGBAbilityTargetingSource::PawnTowardsFocus) || (Source == EGBAbilityTargetingSource::WeaponTowardsFocus)))
	{
		return FTransform((FocalLoc - SourceLoc).Rotation(), SourceLoc);
	}
	
	return FTransform(AimQuat, SourceLoc);
}

FTransform UGB_GameplayAbilityHitScan::GetAssistTargetingTransform(APawn* SourcePawn,
	EGBAbilityTargetingSource Source) const
{
	check(SourcePawn);
	check(Source != EGBAbilityTargetingSource::Custom);

	const FVector ActorLoc = SourcePawn->GetActorLocation();
	FQuat AimQuat = SourcePawn->GetActorQuat();
	AController* Controller = SourcePawn->Controller;
	FVector SourceLoc;

	double FocalDistance = 350.0f;
	FVector FocalLoc;

	FVector CamLoc;
	FRotator CamRot;
	bool bFoundFocus = false;

	if (Controller != nullptr)
	{
		APlayerController* PC = Cast<APlayerController>(Controller);
		if (PC != nullptr)
		{
			PC->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);
			FVector AimDir = CamRot.Vector().GetSafeNormal();
			FocalLoc = CamLoc+(AimDir*FocalDistance);
			CamLoc = FocalLoc+(FocalLoc|AimDir)*AimDir;
			return FTransform(CamRot, FocalLoc);
		}
	}
	return FTransform(AimQuat, SourceLoc);
}

void UGB_GameplayAbilityHitScan::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData,
                                                           FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	if (const FGameplayAbilitySpec* AbilitySpec = MyAbilityComponent->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FScopedPredictionWindow	ScopedPrediction(MyAbilityComponent);
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		const bool bShouldNotifyServer = CurrentActorInfo->IsLocallyControlled() && !CurrentActorInfo->IsNetAuthority();
		if (bShouldNotifyServer)
		{
			MyAbilityComponent->CallServerSetReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey(), LocalTargetDataHandle, ApplicationTag, MyAbilityComponent->ScopedPredictionKey);
		}

		const bool bIsTargetDataValid = true;

		bool bProjectileWeapon = false;

#if WITH_SERVER_CODE
		if (!bProjectileWeapon)
		{
			if (AController* Controller = GetControllerFromActorInfo())
			{
				if (Controller->GetLocalRole() == ROLE_Authority)
				{
					TArray<uint8> HitReplaces;
					for (uint8 i = 0; (i < LocalTargetDataHandle.Num()) && (i < 255); ++i)
					{
						if (FGameplayAbilityTargetData_SingleTargetHit* SingleTargetHit = static_cast<FGameplayAbilityTargetData_SingleTargetHit*>(LocalTargetDataHandle.Get(i)))
						{
							if (SingleTargetHit->bHitReplaced)
							{
								HitReplaces.Add(i);
							}
								
						}
					}
				}
			}
		}
#endif //WITH_SERVER_CODE
		
		if (bIsTargetDataValid && CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			OnRangedWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon ability %s failed to commit (bIsTargetDataValid=%d)"), *GetPathName(), bIsTargetDataValid ? 1 : 0);
			K2_EndAbility();
		}
	}
	
	MyAbilityComponent->ConsumeClientReplicatedTargetData(CurrentSpecHandle, CurrentActivationInfo.GetActivationPredictionKey());
}

void UGB_GameplayAbilityHitScan::StartRangedWeaponTargeting()
{
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	UAbilitySystemComponent* MyAbilityComponent = CurrentActorInfo->AbilitySystemComponent.Get();
	check(MyAbilityComponent);

	AController* Controller = GetControllerFromActorInfo();
	check(Controller);
	
	FScopedPredictionWindow ScopedPrediction(MyAbilityComponent, CurrentActivationInfo.GetActivationPredictionKey());

	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);
	
	FGameplayAbilityTargetDataHandle TargetData;
	
	if (FoundHits.Num() > 0)
	{
		for (const FHitResult& FoundHit : FoundHits)
		{
			FGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			TargetData.Add(NewTargetData);
		}
	}
	OnTargetDataReadyCallback(TargetData, FGameplayTag());
}
