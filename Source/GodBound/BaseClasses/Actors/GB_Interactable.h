// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"

#include "GameFramework/Actor.h"
#include "GB_Interactable.generated.h"

UCLASS()
class GODBOUND_API AGB_Interactable : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGB_Interactable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* InteractableArea;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UGB_AbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<class UGB_GameplayEffect> GE;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(NetMulticast, Reliable)
	void WidgetSetVisible(bool bMakeVisible, class AGB_Character* Character);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnEnterInteract(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnExitInteract(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AddedComponents")
	class UWidgetComponent* PickupWidget;

	UFUNCTION(Blueprintcallable)
	virtual void Interact(AActor* Character);
	
	virtual void OnEndInteract();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	FActiveGameplayEffectHandle InteractEffectHandle;

};
