// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GodBound/BaseClasses/GB_GameplayAbility.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GB_JumpAbility.generated.h"

UENUM(BlueprintType)
enum class EDistanceTraceType : uint8
{
	EDTT_NormalJump,
	EDTT_MantleLow,
	EDTT_MantleHigh,
	EDTT_Vault,
	EDTT_Climb,
	EDTT_Hang,
	EDTT_MAX
	
};

USTRUCT(BlueprintType)
struct FDistanceTraceResult
{
	GENERATED_BODY()
	FDistanceTraceResult();

	EDistanceTraceType Action = EDistanceTraceType::EDTT_MAX;
	
	FVector ObstacleLedgePoint;
	
	FVector ObstacleMidPoint;
	
	FVector ObstacleEndPoint;

	FVector LandPoint;
};

/**
 * 
 */
UCLASS()
class GODBOUND_API UGB_JumpAbility : public UGB_GameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilityProperties")
	bool bParkourEnabled = false;

	void OnDistanceTraceDataReadyCallback(const FDistanceTraceResult& TraceData);

	UFUNCTION(BlueprintCallable)
	bool StartTraceForDistance();

	FVector TraceForHeight();

	FVector TraceForDepth(FVector HighestHit);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDistanceTraceDataReady(const FDistanceTraceResult& TraceData);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	bool bIsRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 HeightTracesNum = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 HeightTraceRadius = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 HeightTraceOffset = 75;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 HeightTraceLength = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 HeightTraceLengthRunning = 250;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 HeightTraceLengthWalking = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 DepthTracesNum = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 DepthTracesOffset = 75;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 DepthTraceRadius = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 DepthTraceLength = 75;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 DepthEndTraceOffset = DepthTracesOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 EndOffsetRunning = 2*DepthTracesOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	int32 EndOffsetWalking = DepthTracesOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "TraceSettings")
	float FinalEndTraceSubtract = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypesToHit = { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) , UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	TEnumAsByte<EDrawDebugTrace::Type> TraceDebugType = EDrawDebugTrace::Type::ForDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	float DebugTraceDuration = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	FLinearColor DebugTraceColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	FLinearColor DebugTraceHitColor = FLinearColor::Green;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	TArray<AActor*> ActorsToIgnore;

	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FVector JumpInitialPosition;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FVector JumpDirection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	int32 HeightIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FVector ObstacleLedgePoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FHitResult ObstacleLedgeHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FVector ObstacleMidPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FHitResult ObstacleMidHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FVector ObstacleEndPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FVector LandPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	FHitResult LandHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "JumpProperties")
	EDistanceTraceType ActionType = EDistanceTraceType::EDTT_NormalJump;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector ApplyWarpOffset(FVector Point, float X, float Z);
	
private:
	FDelegateHandle OnTraceReadyDelegateHandle;

	
	
};
