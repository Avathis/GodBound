// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_TargetDummy.h"
#include "GodBound/BaseClasses/Characters/Attributes/GB_AttributeSet.h"

// Sets default values
AGB_TargetDummy::AGB_TargetDummy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	AbilitySystemComponent = CreateDefaultSubobject<UGB_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AGB_TargetDummy::BeginPlay()
{
	Super::BeginPlay();
	if(IsValid(AbilitySystemComponent))
	{
		Attributes = AbilitySystemComponent->GetSet<UGB_AttributeSet>();
	}
}

// Called every frame
void AGB_TargetDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGB_TargetDummy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

