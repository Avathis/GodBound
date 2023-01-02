// Fill out your copyright notice in the Description page of Project Settings.


#include "GB_Interactable.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GodBound/BaseClasses/Characters/GB_PlayableCharacter.h"
#include "GodBound/BaseClasses/GB_GameplayEffect.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AGB_Interactable::AGB_Interactable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractableArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractiveArea"));
	SetRootComponent(InteractableArea);
	InteractableArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	InteractableArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbilitySystemComponent = CreateDefaultSubobject<UGB_AbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickUpWidget"));
	PickupWidget->SetupAttachment(GetRootComponent());
	
}

UAbilitySystemComponent* AGB_Interactable::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGB_Interactable::WidgetSetVisible_Implementation(bool bMakeVisible, AGB_Character* Character)
{
	if(Character)
	{
		if(Character->IsLocallyControlled())
		{
			PickupWidget->SetVisibility(bMakeVisible);
		}
	}
}

// Called when the game starts or when spawned
void AGB_Interactable::BeginPlay()
{
	Super::BeginPlay();
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	if(PickupWidget)
	{
		
		PickupWidget->SetVisibility(false);
	}
	if(HasAuthority())
	{
		InteractableArea->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		InteractableArea->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,ECollisionResponse::ECR_Overlap);
		
	}
	InteractableArea->OnComponentBeginOverlap.AddDynamic(this, &AGB_Interactable::OnEnterInteract);
	InteractableArea->OnComponentEndOverlap.AddDynamic(this, &AGB_Interactable::OnExitInteract);
}

void AGB_Interactable::OnEnterInteract(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGB_PlayableCharacter* PlayerCharacter = Cast<AGB_PlayableCharacter>(OtherActor);
	if(PlayerCharacter&&PickupWidget&&GE.GetDefaultObject())
	{
		InteractEffectHandle = AbilitySystemComponent->ApplyGameplayEffectToTarget(GE.GetDefaultObject(),PlayerCharacter->GetAbilitySystemComponent(),0, FGameplayEffectContextHandle(new FGameplayEffectContext(this, this)));
		WidgetSetVisible(true, PlayerCharacter);
	}
}

void AGB_Interactable::OnExitInteract(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor&&OtherComp&&OverlappedComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Valid->Proceeding to delete"))
		AGB_PlayableCharacter* PlayerCharacter = Cast<AGB_PlayableCharacter>(OtherActor);
		if(PlayerCharacter)
		{
			PlayerCharacter->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(InteractEffectHandle);
			WidgetSetVisible(false, PlayerCharacter);
		}
	}
	
}


// Called every frame
void AGB_Interactable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AGB_Interactable::Interact(AActor* Character)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(3,15.f,FColor::White,FString("SUCCESSFULLY INTERACTED"));
	}
}

void AGB_Interactable::OnEndInteract()
{
	
}


void AGB_Interactable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}


