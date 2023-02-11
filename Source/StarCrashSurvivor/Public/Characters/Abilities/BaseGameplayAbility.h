// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Characters/Abilities/AbilityTypes.h"
#include "BaseGameplayAbility.generated.h"

class ABaseCharacter;
class UInputAction;

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UBaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	EGameplayAbilityInput InputId;

	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> InputAction;

public:
	FORCEINLINE int32 GetInputId() const { return static_cast<int32>(InputId); }
	FORCEINLINE TObjectPtr<UInputAction> GetInputAction() const { return InputAction; }

	TObjectPtr<ABaseCharacter> GetOwningCharacter() const;
};
