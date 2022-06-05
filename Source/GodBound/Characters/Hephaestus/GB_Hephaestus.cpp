// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Hephaestus.h"
#include "GodBound/BaseClasses/Components/GB_CharacterMovementComponent.h"

void AGB_Hephaestus::OnMoveForward(float Value)
{
    if (GetAdvMovementComponent()->GetMovementType() == EMovementState::EMS_HephaestusChargeSprint)
    {
        MoveForward(1.f);
    }
    else
    {
        MoveForward(Value);
    }
}

void AGB_Hephaestus::MoveForward(float Value)
{
    ForwardAxis = Value;
    //UE_LOG(LogTemp,Warning,TEXT("ForwardAxis: %f"),Value);

    if (PlayerController && Value != 0)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }

}

void AGB_Hephaestus::OnRight(float Value)
{
    if(GetAdvMovementComponent()->GetMovementType()==EMovementState::EMS_HephaestusChargeSprint)
    {
        AddControllerYawInput(Value * ChargeTurnRate * GetWorld()->GetDeltaSeconds());
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

void AGB_Hephaestus::OnLookUp(float Value)
{
        LookUp(Value);
}

void AGB_Hephaestus::LookUp(float Value)
{
    if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
    {
        APlayerController* const PC = CastChecked<APlayerController>(Controller);
        PC->AddPitchInput(Value);
    }
}

void AGB_Hephaestus::OnTurnRight(float Value)
{
    if (GetAdvMovementComponent()->GetMovementType() == EMovementState::EMS_HephaestusChargeSprint)
    {
        AddControllerYawInput(Value * ChargeTurnRate * GetWorld()->GetDeltaSeconds());
    }
    else
    {
        TurnRight(Value);
    }
}

void AGB_Hephaestus::TurnRight(float Value)
{
    if (Value != 0.f && Controller && Controller->IsLocalPlayerController())
    {
        APlayerController* const PC = CastChecked<APlayerController>(Controller);
        PC->AddYawInput(Value);
    }
}

void AGB_Hephaestus::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{   
    AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"), FString("CancelTarget"), FString("GBAbilityInputID"), static_cast<int32>(GBAbilityInputID::Confirm), static_cast<int32>(GBAbilityInputID::Cancel)));
    PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AGB_Hephaestus::OnMoveForward);
    PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AGB_Hephaestus::OnRight);

    PlayerInputComponent->BindAxis(FName("LookUp"), this, &AGB_Hephaestus::OnLookUp);
    PlayerInputComponent->BindAxis(FName("LookUpAR"), this, &AGB_PlayableCharacter::LookUpAtRate);
    PlayerInputComponent->BindAxis(FName("Turn"), this, &AGB_Hephaestus::OnTurnRight);
    PlayerInputComponent->BindAxis(FName("TurnAR"), this, &AGB_PlayableCharacter::TurnRightAtRate);
      
}
