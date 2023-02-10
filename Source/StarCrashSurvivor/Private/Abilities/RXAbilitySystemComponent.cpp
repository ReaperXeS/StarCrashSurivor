// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RXAbilitySystemComponent.h"

void URXAbilitySystemComponent::ReceiveDamage(URXAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
