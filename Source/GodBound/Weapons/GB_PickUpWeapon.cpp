// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_PickUpWeapon.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GodBound/BaseClasses/Characters/GB_PlayableCharacter.h"

AGB_PickUpWeapon::AGB_PickUpWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickUpArea"));
	AreaSphere->SetupAttachment(GetRootComponent());
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());
}

void AGB_PickUpWeapon::BeginPlay()
{
	Super::BeginPlay();
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,ECollisionResponse::ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AGB_PickUpWeapon::OnSphereOverLap);
	}
}

void AGB_PickUpWeapon::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGB_PickUpWeapon::OnSphereOverLap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGB_PlayableCharacter* PlayerCharacter = Cast<AGB_PlayableCharacter>(OtherActor);
	if(PlayerCharacter&&PickupWidget)
	{
		PickupWidget->SetVisibility(true);
	}
}
