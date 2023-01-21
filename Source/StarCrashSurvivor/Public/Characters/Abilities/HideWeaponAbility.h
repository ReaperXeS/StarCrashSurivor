// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/BaseGameplayAbility.h"
#include "HideWeaponAbility.generated.h"

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UHideWeaponAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
};
