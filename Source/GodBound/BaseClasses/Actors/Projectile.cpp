// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(GetRootComponent());
	//SetRootComponent(StaticMesh);
	SphereComponent = CreateDefaultSubobject<USphereComponent>("BoxComponent");
	SphereComponent->SetupAttachment(GetRootComponent());
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
}

float AProjectile::GetAngle(float Distance)
{
	return UKismetMathLibrary::Asin(Distance*10/UKismetMathLibrary::Sqrt(1000))/2;
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

