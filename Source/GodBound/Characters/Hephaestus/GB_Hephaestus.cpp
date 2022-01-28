// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Hephaestus.h"

void AGB_Hephaestus::OnRight(float Value)
{
    if(ActorHasTag(FName("Abilities.Fire.Charge.Active")))
    {
        AddControllerYawInput(Value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
    }
    else
    {
        MoveRight(Value);
    }
}

void AGB_Hephaestus::MoveRight(float Value)
{
    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);
	
    // get right vector 
    const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
    // add movement in that direction
    AddMovementInput(Direction, Value);
}
