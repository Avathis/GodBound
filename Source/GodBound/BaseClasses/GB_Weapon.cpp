// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Weapon.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGB_Weapon::AGB_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
	SetRootComponent(MeshComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AGB_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGB_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

