// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/BaseGameplayAbility.h"
#include "Characters/BaseCharacter.h"

TObjectPtr<ABaseCharacter> UBaseGameplayAbility::GetOwningCharacter() const
{
	return Cast<ABaseCharacter>(GetOwningActorFromActorInfo());
}
