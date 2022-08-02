// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_CharacterMovementComponent.h"
#include "GodBound/BaseClasses/Characters/GB_Character.h"

UGB_CharacterMovementComponent::UGB_CharacterMovementComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WalkMovementSpeed = 650;
	RunMovementSpeed = 900;
	AbilitySprintMovementSpeed = 1500;

}

EMovementState UGB_CharacterMovementComponent::GetMovementType()
{
	return MovementType;
}

void UGB_CharacterMovementComponent::ChangeMovementState(EMovementState NewMovementState)
{
	MovementType = NewMovementState;
	/*
	switch (MovementType)
	{
	case EMovementState::EMS_Idle: break;
	case EMovementState::EMS_Walking:
		{
		MaxWalkSpeed = WalkMovementSpeed;
			break;
		}
	case EMovementState::EMS_Sprinting:
		{
			MaxWalkSpeed = RunMovementSpeed;
			//UE_LOG(LogTemp,Warning,TEXT("Sprinting"))
			break;
		}
	case EMovementState::EMS_AbilitySprint:
		{
			MaxWalkSpeed = AbilitySprintMovementSpeed; 
			break;
		}
	case EMovementState::EMS_HephaestusChargeSprint:
	{
		break;
	}
	default: ;
	}
	*/
}

float UGB_CharacterMovementComponent::GetMaxSpeed() const
{
	AGB_Character* Owner = Cast<AGB_Character>(GetOwner());
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), *FString(__FUNCTION__));
		return Super::GetMaxSpeed();
	}

	if (Owner->GetAbilitySystemComponent()->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun"))))
	{
		return 0.0f;
	}

	if (RequestToStartSprinting)
	{
		return Owner->GetSpeed() * SprintSpeedMultiplier;
	}

	if (RequestToStartADS)
	{
		return Owner->GetSpeed() * ADSSpeedMultiplier;
	}

	return Owner->GetSpeed();
}

void UGB_CharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	//The Flags parameter contains the compressed input flags that are stored in the saved move.
	//UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
	//It basically just resets the movement component to the state when the move was made so it can simulate from there.
	RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;

	RequestToStartADS = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

FNetworkPredictionData_Client* UGB_CharacterMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner != NULL);

	if (!ClientPredictionData)
	{
		UGB_CharacterMovementComponent* MutableThis = const_cast<UGB_CharacterMovementComponent*>(this);

		MutableThis->ClientPredictionData = new FGB_NetworkPredictionData_Client(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void UGB_CharacterMovementComponent::StartSprinting()
{
	RequestToStartSprinting = true;
}

void UGB_CharacterMovementComponent::StopSprinting()
{
	RequestToStartSprinting = false;
}

void UGB_CharacterMovementComponent::StartAimDownSights()
{
	RequestToStartADS = true;
}

void UGB_CharacterMovementComponent::StopAimDownSights()
{
	RequestToStartADS = false;
}

UGB_CharacterMovementComponent::FGB_NetworkPredictionData_Client::FGB_NetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr UGB_CharacterMovementComponent::FGB_NetworkPredictionData_Client::AllocateNewMove()
{
	return FSavedMovePtr(new FGB_SavedMove());
}

void UGB_CharacterMovementComponent::FGB_SavedMove::Clear()
{
	Super::Clear();

	SavedRequestToStartSprinting = false;
	SavedRequestToStartADS = false;
}

uint8 UGB_CharacterMovementComponent::FGB_SavedMove::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (SavedRequestToStartSprinting)
	{
		Result |= FLAG_Custom_0;
	}

	if (SavedRequestToStartADS)
	{
		Result |= FLAG_Custom_1;
	}

	return Result;
}

bool UGB_CharacterMovementComponent::FGB_SavedMove::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{
	//Set which moves can be combined together. This will depend on the bit flags that are used.
	if (SavedRequestToStartSprinting != ((FGB_SavedMove*)&NewMove)->SavedRequestToStartSprinting)
	{
		return false;
	}

	if (SavedRequestToStartADS != ((FGB_SavedMove*)&NewMove)->SavedRequestToStartADS)
	{
		return false;
	}

	return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void UGB_CharacterMovementComponent::FGB_SavedMove::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

	UGB_CharacterMovementComponent* CharacterMovement = Cast<UGB_CharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
		SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
		SavedRequestToStartADS = CharacterMovement->RequestToStartADS;
	}
}

void UGB_CharacterMovementComponent::FGB_SavedMove::PrepMoveFor(ACharacter* Character)
{
	Super::PrepMoveFor(Character);

	UGB_CharacterMovementComponent* CharacterMovement = Cast<UGB_CharacterMovementComponent>(Character->GetCharacterMovement());
	if (CharacterMovement)
	{
	}
}
