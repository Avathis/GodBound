// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_WeaponPickup.h"

#include "GodBound/BaseClasses/GB_Weapon.h"
#include "GodBound/BaseClasses/Characters/GB_PlayableCharacter.h"

#include "Net/UnrealNetwork.h"

void AGB_WeaponPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AGB_WeaponPickup::Interact(AActor* Character)
{
	Super::Interact(Character);
	if(WeaponRef)
	{
		AGB_PlayableCharacter* PlayerCharacter = Cast<AGB_PlayableCharacter>(Character);
		if(PlayerCharacter)
		{
			PlayerCharacter->EquipWeapon(WeaponRef);
			WeaponRef = nullptr;
		}
		OnEndInteract();
		/*
		WeaponRef->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
		WeaponRef->AttachToActor(Character,FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponRef->SocketName);
		WeaponRef->WeaponState = EWeaponState::EWS_Equipped;
		WeaponRef->SetOwner(Character);
		WeaponRef = nullptr;
		*/
	}
	
	
}

void AGB_WeaponPickup::OnEndInteract()
{
	//WeaponRef->Destroy();
	Destroy();
	
}

void AGB_WeaponPickup::BeginPlay()
{
	Super::BeginPlay();
	if(GetWorld())
	{
		if(HasAuthority())
		{
			WeaponRef = GetWorld()->SpawnActor<AGB_Weapon>(WeaponClassToSpawn, GetActorLocation(), GetActorRotation());
			if(WeaponRef)
			{
				WeaponRef->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false));
			}
		}
	}
}
