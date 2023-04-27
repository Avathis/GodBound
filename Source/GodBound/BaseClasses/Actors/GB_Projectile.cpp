// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AGB_Projectile::AGB_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereHitBox = CreateDefaultSubobject<USphereComponent>("SphereHitBox");
	SphereHitBox->SetupAttachment(GetRootComponent());
	
	BoxComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	BoxComponent->SetupAttachment(SphereHitBox);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
}

void AGB_Projectile::PostNetReceiveLocationAndRotation()
{
	if (ProjectileMovementComponent && ProjectileMovementComponent->UpdatedComponent && ProjectileMovementComponent->bInterpMovement)
	{
		const FRepMovement& ConstRepMovement = GetReplicatedMovement();
		const FVector NewLocation = FRepMovement::RebaseOntoLocalOrigin(ConstRepMovement.Location, this);
		ProjectileMovementComponent->MoveInterpolationTarget(NewLocation, ConstRepMovement.Rotation);
	}
	else
	{
		Super::PostNetReceiveLocationAndRotation();
	}
}

// Called when the game starts or when spawned
void AGB_Projectile::BeginPlay()
{
	Super::BeginPlay();
    
	if (GetLocalRole() != ROLE_Authority)
	{
		if (ProjectileMovementComponent && ProjectileMovementComponent->UpdatedComponent)
		{
			ProjectileMovementComponent->SetInterpolatedComponent(BoxComponent);
			ProjectileMovementComponent->bSimulationEnabled = false;
			ProjectileMovementComponent->bInterpMovement = true;
			ProjectileMovementComponent->bInterpRotation = true;
		}
	}
	
}

// Called every frame
void AGB_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

