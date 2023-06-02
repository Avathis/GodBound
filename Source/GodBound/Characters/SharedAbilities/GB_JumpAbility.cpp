// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_JumpAbility.h"

#include "AbilitySystemLog.h"
#include "GodBound/BaseClasses/Characters/GB_PlayableCharacter.h"
#include "Kismet/KismetMathLibrary.h"


FDistanceTraceResult::FDistanceTraceResult()
{
	
	ObstacleLedgePoint = FVector::ZeroVector;
	ObstacleMidPoint = FVector::ZeroVector;
	ObstacleEndPoint = FVector::ZeroVector;
	LandPoint = FVector::ZeroVector;
}

bool UGB_JumpAbility::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                         const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGB_JumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	bIsRunning = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag("State.Movement.Run")) ? true:false;
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGB_JumpAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGB_JumpAbility::OnDistanceTraceDataReadyCallback(const FDistanceTraceResult& TraceData)
{
}

bool UGB_JumpAbility::StartTraceForDistance()
{
	if(!GetAvatarActorFromActorInfo())
	{
		UE_LOG(LogTemp,Error,TEXT("AvatarActorInvalid"))
		return false;
	}
	JumpInitialPosition = CurrentActorInfo->AvatarActor->GetActorLocation();
	JumpDirection = CurrentActorInfo->AvatarActor->GetActorForwardVector();

	FVector HighestHit = FVector::ZeroVector;
	HighestHit = TraceForHeight();
	if(HighestHit == FVector::ZeroVector)
	{
		return false;
	}
	
	if(TraceForDepth(HighestHit)==FVector::ZeroVector)
	{
		return false;
	}
	return true;
}

bool UGB_JumpAbility::StartClimbTraceForDistance(float FV, float RV)
{
	if(FV==0&&RV==0)
	{
		FVector TraceStart = GetAvatarActorFromActorInfo()->GetActorLocation();
		TraceStart+= FVector(0.f,0.f,100);
		FVector TraceEnd = TraceStart+GetAvatarActorFromActorInfo()->GetActorForwardVector()*50;
		TArray<AActor*> ActorsToIgnoreLocal;
		FHitResult OutHit;
		if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),TraceStart,TraceEnd, 5.f, { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) ,
				UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) },false, ActorsToIgnore, EDrawDebugTrace::ForDuration,OutHit,
				true))
		{
			FHitResult DepthOutHit;
			FVector CurrentStartTrace = OutHit.Location+FVector(0.f,0.f,100.f);
			FVector CurrentEndTrace = CurrentStartTrace-FVector(0.f,0.f,100);
			WallRotation = ReverseRotationZ(OutHit.ImpactNormal);
			if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),CurrentStartTrace,CurrentEndTrace, DepthTraceRadius,TraceObjectTypesToHit,false,ActorsToIgnore,TraceDebugType,DepthOutHit,true,DebugTraceColor,DebugTraceHitColor,DebugTraceDuration))
			{
				ObstacleLedgePoint = DepthOutHit.Location;
			}
			return true;
		}
	}
	return false;
;}

FVector UGB_JumpAbility::TraceForHeight()
{
	FHitResult HighestHit;
	FHitResult CurrentHighestHit;
	
	FVector CurrentHeightTraceStart = JumpInitialPosition;
	CurrentHeightTraceStart.Z-=HeightTraceOffset*2;
	FVector CurrentHeightTraceEnd = JumpInitialPosition+JumpDirection* (bIsRunning ? HeightTraceLengthRunning : HeightTraceLengthWalking);
	CurrentHeightTraceEnd.Z-=HeightTraceOffset*2;
	float VectorLengthDelta = 0.f;
	ActorsToIgnore.AddUnique(GetAvatarActorFromActorInfo());

	for(int32 i = 0; i <= HeightTracesNum; i++)
	{
		FHitResult CurrentHit;
		//GetWorld()->SweepSingleByObjectType(CurrentHit,CurrentHeightTraceStart, CurrentHeightTraceEnd, JumpDirection.ToOrientationQuat(),TraceObjectTypesToHit, FCollisionShape::MakeSphere(HeightTraceRadius));
		if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),CurrentHeightTraceStart,CurrentHeightTraceEnd, HeightTraceRadius,TraceObjectTypesToHit,false,ActorsToIgnore,TraceDebugType,CurrentHit,true,DebugTraceColor,DebugTraceHitColor,DebugTraceDuration))
		{
			VectorLengthDelta = CurrentHighestHit.Distance - CurrentHit.Distance;
			CurrentHighestHit = CurrentHit;
			CurrentHeightTraceStart.Z+=HeightTraceOffset;
			CurrentHeightTraceEnd.Z+=HeightTraceOffset;
			if(HeightTracesNum == i)
			{
				HeightIndex = i;
				if(VectorLengthDelta>20||CurrentHighestHit.Distance==0)
				{
					ActionType = EDistanceTraceType::EDTT_HandFree;
				}
				else
				{
					ActionType = EDistanceTraceType::EDTT_HangBraced;
				}
				
			}
			WallRotation = ReverseRotationZ(CurrentHighestHit.ImpactNormal);
		}
		else if(i==1&&CurrentHighestHit.IsValidBlockingHit())
		{
			ActionType = EDistanceTraceType::EDTT_MantleLow;
			HeightIndex = i;
			return CurrentHighestHit.Location;
		}
		else if(i>1&&i<5&&CurrentHighestHit.IsValidBlockingHit())
		{
			ActionType = EDistanceTraceType::EDTT_Vault;
			HeightIndex = i;
			return CurrentHighestHit.Location;
		}
		else if(i>=5&&i<7&&CurrentHighestHit.IsValidBlockingHit())
		{
			HeightIndex = i;
			ActionType = EDistanceTraceType::EDTT_MantleHigh;
			return CurrentHighestHit.Location;
		}
		else if (i>=7&&CurrentHighestHit.IsValidBlockingHit())
		{
			HeightIndex = i;
			if(VectorLengthDelta>20)
			{
				ActionType = EDistanceTraceType::EDTT_HandFree;
			}
			else
			{
				ActionType = EDistanceTraceType::EDTT_HangBraced;
			}
			ActionType = EDistanceTraceType::EDTT_HangBraced;
			return CurrentHighestHit.Location;
		}
		else if(!CurrentHighestHit.IsValidBlockingHit()&&!i != HeightTracesNum)
		{
			CurrentHeightTraceStart.Z+=HeightTraceOffset;
			CurrentHeightTraceEnd.Z+=HeightTraceOffset;
		}
		else
		{
			return CurrentHighestHit.Location;
		}
		
	}
	return CurrentHighestHit.Location;
}

FVector UGB_JumpAbility::TraceForDepth(FVector HighestHit)
{
	FHitResult FurthestHit;
	for(int32 i = 0; i<DepthTracesNum;i++)
	{
		FHitResult CurrentHit;
		FVector CurrentStartTrace = (HighestHit+FVector(0.f,0.f,100.f))+(JumpDirection*DepthTracesOffset*i);
		FVector CurrentEndTrace = CurrentStartTrace-FVector(0.f,0.f,100);
		
		if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),CurrentStartTrace,CurrentEndTrace, DepthTraceRadius,TraceObjectTypesToHit,false,ActorsToIgnore,TraceDebugType,CurrentHit,true,DebugTraceColor,DebugTraceHitColor,DebugTraceDuration))
		{
			FurthestHit = CurrentHit;
			if(i==0)
			{
				ObstacleLedgePoint = CurrentHit.Location;
			}
			ObstacleMidPoint = CurrentHit.Location;
			if(ActionType==EDistanceTraceType::EDTT_Vault&&i==DepthTracesNum-1)
			{
				if(HeightIndex<3)
				{
					ActionType = EDistanceTraceType::EDTT_MantleLow;
				}
				else
				{
					ActionType = EDistanceTraceType::EDTT_MantleHigh;
				}
			}
		}
		else
		{
			FHitResult FinalHit;
			FVector StartFinalTrace = CurrentHit.TraceStart+JumpDirection*(bIsRunning?EndOffsetRunning:EndOffsetWalking);
			FVector EndFinalTrace = StartFinalTrace - FVector(0.f,0.f,FinalEndTraceSubtract);
			if(UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),StartFinalTrace,EndFinalTrace,TraceObjectTypesToHit,false,ActorsToIgnore,TraceDebugType,FinalHit,true,DebugTraceColor,DebugTraceHitColor,DebugTraceDuration))
			{
				LandPoint = FinalHit.Location;
				return LandPoint;
			}
			break;
		}
	}
	return FVector::ZeroVector;
}

FVector UGB_JumpAbility::ApplyWarpOffset(FVector Point, float X, float Y, float Z)
{
	const FVector HelperVector = Point+JumpDirection*X+JumpDirectionRight*Y;
	
	return HelperVector+FVector(0.f,0.f,Z);
	//return Point+Direction
}

FRotator UGB_JumpAbility::ReverseRotationZ(FVector NormalImpactZ)
{
	FRotator ReversRotation = UKismetMathLibrary::MakeRotFromX(NormalImpactZ);
	FRotator ReversRotator = UKismetMathLibrary::NormalizedDeltaRotator(ReversRotation, FRotator(0.f,180.f,0.f));
	return FRotator(0.f,ReversRotator.Yaw, 0.f);
}

