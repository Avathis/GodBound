 // Fill out your copyright notice in the Description page of Project Settings.


#include "GB_MeleeTraceComponent.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values for this component's properties
UGB_MeleeTraceComponent::UGB_MeleeTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UGB_MeleeTraceComponent::SetupMeleeTrace(USkeletalMeshComponent* MeshComponent)
{
	if(MeshComponent)
	{
		OwnerMesh = MeshComponent;
	}
	
}

void UGB_MeleeTraceComponent::DoTheTrace()
{
	if(bIsTraceActiveRight)
	{
		if(!LastKnownSocketLocationRight.IsEmpty())
		{
			for (const auto Socket : SocketNamesRight)
			{
				const FVector* Start = LastKnownSocketLocationRight.Find(Socket);
				const FVector End = OwnerMesh->GetSocketLocation(Socket);
				TArray<FHitResult> OutHits;
				switch (MeleeTraceType) {
				case EKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), *Start, End, MyObjectTypesToHit, ShouldTraceComplex, ActorsHit, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
				case EKismetTraceType::BoxTrace: break;
				case EKismetTraceType::CapsuleTrace: break;
				case EKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMulti(GetWorld(), *Start, End, SphereRadius, MyTraceChannel, ShouldTraceComplex, ActorsHit, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
				default: ;
				}
				ActorsToBeAdded(OutHits);
			}
		}
		
	}
	if(bIsTraceActiveLeft)
	{
		if(!LastKnownSocketLocationLeft.IsEmpty())
		{
			for (const auto Socket : SocketNamesLeft)
			{
				const FVector* Start = LastKnownSocketLocationRight.Find(Socket);
				const FVector End = OwnerMesh->GetSocketLocation(Socket);
				TArray<FHitResult> OutHits;
				switch (MeleeTraceType) {
				case EKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), *Start, End, MyObjectTypesToHit, ShouldTraceComplex, ActorsHit, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
				case EKismetTraceType::BoxTrace: break;
				case EKismetTraceType::CapsuleTrace: break;
				case EKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMulti(GetWorld(), *Start, End, SphereRadius, MyTraceChannel, ShouldTraceComplex, ActorsHit, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
				default: ;
				}
				ActorsToBeAdded(OutHits);
			}
		}
	}
	if(bIsAbilityTraceActive)
	{
		if(!LastKnownSocketLocationAbility.IsEmpty())
		{
			for(const auto Socket : AbilitySocketNames)
			{
				const FVector* Start = LastKnownSocketLocationAbility.Find(Socket);
				const FVector End = OwnerMesh->GetSocketLocation(Socket);
				TArray<FHitResult> OutHits;
				switch (AbilityTraceType) {
				case EKismetTraceType::LineTrace:
					{
						UKismetSystemLibrary::LineTraceMultiForObjects(GetWorld(), *Start, End, MyObjectTypesToHit, ShouldTraceComplex, ActorsHit, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
				case EKismetTraceType::BoxTrace: break;
				case EKismetTraceType::CapsuleTrace: break;
				case EKismetTraceType::SphereTrace:
					{
						UKismetSystemLibrary::SphereTraceMulti(GetWorld(), *Start, End, SphereRadius, MyTraceChannel, ShouldTraceComplex, ActorsHit, MyDrawDebugType, OutHits, ShouldIgnoreSelf, MyTraceColor, MyTraceHitColor, MyDrawTime);
						break;
					}
				default: ;
				}
				ActorsToBeAdded(OutHits);
			}
		}
	}
	UpdateSocketLocations();
}

void UGB_MeleeTraceComponent::UpdateSocketLocations()
{
	if(bIsTraceActiveRight)
	{
		for (FName SocketName : SocketNamesRight)
		{
			LastKnownSocketLocationRight.Remove(SocketName);
			LastKnownSocketLocationRight.Add(SocketName, OwnerMesh->GetSocketLocation(SocketName));
		}
	}
	if(bIsTraceActiveLeft)
	{
		for(FName SocketName :SocketNamesLeft)
		{
			LastKnownSocketLocationLeft.Remove(SocketName);
			LastKnownSocketLocationLeft.Add(SocketName,OwnerMesh->GetSocketLocation(SocketName));
		}
	}
	if(bIsAbilityTraceActive)
	{
		for(FName SocketName : AbilitySocketNames)
		{
			LastKnownSocketLocationAbility.Remove(SocketName);
			LastKnownSocketLocationAbility.Add(SocketName, OwnerMesh->GetSocketLocation(SocketName));
		}
	}
}


// Called when the game starts
void UGB_MeleeTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UGB_MeleeTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(bIsTraceActiveRight || bIsTraceActiveLeft || bIsAbilityTraceActive)
	{
		DoTheTrace();
	}
}

void UGB_MeleeTraceComponent::StartTrace(bool bIsRight)
{
	if(bIsRight)
	{
		bIsTraceActiveRight = true;
	}
	else
	{
		bIsTraceActiveLeft = true;
	}
	bIsTraceActive = true;
	
}

void UGB_MeleeTraceComponent::StartAbilityTrace(TArray<FName> AbilitySocketNamesIN)
{
	for (FName AbilitySocketName : AbilitySocketNamesIN)
	{
		AbilitySocketNames.Add(AbilitySocketName);
	}
	bIsAbilityTraceActive = true;
}


void UGB_MeleeTraceComponent::EndTrace(bool bIsRight)
{
	if(bIsRight)
	{
		bIsTraceActiveRight = false;
		LastKnownSocketLocationRight.Empty();
	}
	else
	{
		bIsTraceActiveLeft = false;
		LastKnownSocketLocationLeft.Empty();
	}
	bIsTraceActive = false;
	//LastKnownSocketLocationRight.Empty();
	
	//TODO This can cause, in very certain scenarios, an unwanted refresh.. Will come back to it if it becomes too troublesome
	ActorsHit.Empty();
}

void UGB_MeleeTraceComponent::EndAbilityTrace()
{
	AbilitySocketNames.Empty();
	LastKnownSocketLocationAbility.Empty();
	bIsAbilityTraceActive = false;
}


void UGB_MeleeTraceComponent::ActorsToBeAdded(TArray<FHitResult> HitArrayToAdd)
{
	for (auto Hit : HitArrayToAdd)
	{
		if(Hit.GetActor() && Hit.GetActor()!=GetOwner())
		{
			ActorsHit.AddUnique(Hit.GetActor());
			OnItemAdded.Broadcast(Hit);
		}
	}
}

void UGB_MeleeTraceComponent::ActorsToBeAddedAbility(TArray<FHitResult> HitArrayToAddAbility)
{
	for (auto Hit : HitArrayToAddAbility)
	{
		if(Hit.GetActor() && Hit.GetActor()!=GetOwner())
		{
			ActorsHit.AddUnique(Hit.GetActor());
			OnAbilityItemAdded.Broadcast(Hit);
		}
	}
}
