// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_GASActor.h"

#include "Components/SphereComponent.h"
#include "GodBound/BaseClasses/Components/GB_AbilitySystemComponent.h"

// Sets default values
AGB_GASActor::AGB_GASActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractiveArea"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	AbilitySystemComponent = CreateDefaultSubobject<UGB_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));

}

UAbilitySystemComponent* AGB_GASActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGB_GASActor::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
}

// Called every frame
void AGB_GASActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

