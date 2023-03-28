// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GB_MeleeTraceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, FHitResult, LastItem);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GODBOUND_API UGB_MeleeTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintAssignable, Category = "Trace")
	FOnItemAdded OnItemAdded;
	
	UGB_MeleeTraceComponent();

	UFUNCTION(BlueprintCallable, Category = "Trace")
	void SetupMeleeTrace(USkeletalMeshComponent* MeshComponent);

private:
	void DoTheTrace();

	void UpdateSocketLocations();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ActorTraced(AActor* ActorHit);

	UFUNCTION(BlueprintCallable)
	void StartTrace(bool bIsRight);

	UFUNCTION(BlueprintCallable)
	void EndTrace(bool bIsRight);

	UFUNCTION()
	void ActorsToBeAdded(TArray<FHitResult> HitArrayToAdd);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
	TArray<FName> SocketNames;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
	TArray<FName> SocketNamesRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
	TArray<FName> SocketNamesLeft;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
	TArray<FVector> LastPositionOfSockets;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trace")
	TArray<AActor*> ActorsHit;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
	TMap<FName, FVector> LastKnownSocketLocationRight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Trace")
	TMap<FName, FVector> LastKnownSocketLocationLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trace")
	USkeletalMeshComponent* OwnerMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TraceSettings")
	bool bIsTraceActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TraceSettings")
	bool bIsTraceActiveRight = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TraceSettings")
	bool bIsTraceActiveLeft = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	bool ShouldIgnoreSelf = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	TEnumAsByte<ETraceTypeQuery> MyTraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	TArray<TEnumAsByte<EObjectTypeQuery>> MyObjectTypesToHit = { UEngineTypes::ConvertToObjectType(ECC_WorldStatic) , UEngineTypes::ConvertToObjectType(ECC_WorldDynamic) };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	TEnumAsByte<EDrawDebugTrace::Type> MyDrawDebugType = EDrawDebugTrace::Type::ForDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	float MyDrawTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	FLinearColor MyTraceColor = FLinearColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	FLinearColor MyTraceHitColor = FLinearColor::Green;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TraceSettings")
	bool ShouldTraceComplex = true;

	
		
};
