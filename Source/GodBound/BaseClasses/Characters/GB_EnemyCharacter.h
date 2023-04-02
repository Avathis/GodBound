// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GB_Character.h"
#include "GB_EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GODBOUND_API AGB_EnemyCharacter : public AGB_Character
{
	GENERATED_BODY()

	protected:
	virtual void BeginPlay() override;

	public:
	AGB_EnemyCharacter(const FObjectInitializer& ObjectInitializer);
	
	virtual void Tick(float DeltaSeconds) override;

	virtual UGB_AbilitySystemComponent* GetAbilitySystemComponent() const override
	{
		return ASC;
	}

	FDelegateHandle HealthChangedDelegateHandle;
	//FDelegateHandle ;
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	UGB_AbilitySystemComponent* ASC;

	virtual void GrantAbility(TSubclassOf<class UGB_GameplayAbility> AbilityClass, int32 Level, int32 InputCode, FGameplayTag InputTag) override;
	
};
