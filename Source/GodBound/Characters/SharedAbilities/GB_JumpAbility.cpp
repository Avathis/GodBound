// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_JumpAbility.h"

#include "AbilitySystemLog.h"
#include "GodBound/BaseClasses/Characters/GB_PlayableCharacter.h"


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

FVector UGB_JumpAbility::TraceForHeight()
{
	FHitResult HighestHit;
	FHitResult CurrentHighestHit;
	FVector CurrentHeightTraceStart = JumpInitialPosition;
	CurrentHeightTraceStart.Z-=HeightTraceOffset*2;
	FVector CurrentHeightTraceEnd = JumpInitialPosition+JumpDirection* (bIsRunning ? HeightTraceLengthRunning : HeightTraceLengthWalking);
	CurrentHeightTraceEnd.Z-=HeightTraceOffset*2;
	ActorsToIgnore.AddUnique(GetAvatarActorFromActorInfo());

	for(int32 i = 0; i <= HeightTracesNum; i++)
	{
		FHitResult CurrentHit;
		//GetWorld()->SweepSingleByObjectType(CurrentHit,CurrentHeightTraceStart, CurrentHeightTraceEnd, JumpDirection.ToOrientationQuat(),TraceObjectTypesToHit, FCollisionShape::MakeSphere(HeightTraceRadius));
		if(UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),CurrentHeightTraceStart,CurrentHeightTraceEnd, HeightTraceRadius,TraceObjectTypesToHit,false,ActorsToIgnore,TraceDebugType,CurrentHit,true,DebugTraceColor,DebugTraceHitColor,DebugTraceDuration))
		{
			CurrentHighestHit = CurrentHit;
			CurrentHeightTraceStart.Z+=HeightTraceOffset;
			CurrentHeightTraceEnd.Z+=HeightTraceOffset;
		}
		else if(i==1)
		{
			ActionType = EDistanceTraceType::EDTT_MantleLow;
			HeightIndex = i;
			return CurrentHighestHit.Location;
		}
		else if(i>1&&i<5)
		{
			ActionType = EDistanceTraceType::EDTT_Vault;
			HeightIndex = i;
			return CurrentHighestHit.Location;
		}
		else if(i>=5)
		{
			HeightIndex = i;
			ActionType = EDistanceTraceType::EDTT_MantleHigh;
			return CurrentHighestHit.Location;
		}
		else
		{
			return CurrentHighestHit.Location;
		}
		
	}
	return FVector::ZeroVector;
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

FVector UGB_JumpAbility::ApplyWarpOffset(FVector Point, float X, float Z)
{
	const FVector HelperVector = Point+JumpDirection*X;;
	return HelperVector+FVector(0.f,0.f,Z);
	//return Point+Direction
}

