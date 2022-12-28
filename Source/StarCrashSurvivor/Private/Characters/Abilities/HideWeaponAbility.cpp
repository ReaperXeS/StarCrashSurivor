// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/HideWeaponAbility.h"
#include "Characters/HeroCharacter.h"

void UHideWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
