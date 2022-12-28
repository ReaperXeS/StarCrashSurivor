// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/BaseGameplayAbility.h"
#include "GA_PickupWeapon.generated.h"

class AHeroCharacter;
/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UGA_PickupWeapon : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	AHeroCharacter* GetHeroCharacter() const;

public:
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon();
};
