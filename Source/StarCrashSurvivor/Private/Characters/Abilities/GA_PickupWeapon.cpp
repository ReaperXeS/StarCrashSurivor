// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GA_PickupWeapon.h"
#include "Characters/HeroCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "AbilitySystemComponent.h"

AHeroCharacter* UGA_PickupWeapon::GetHeroCharacter() const
{
	return Cast<AHeroCharacter>(GetOwningActorFromActorInfo());
}

bool UGA_PickupWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		if (const AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr)
		{
			if (const AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(AvatarActor); HeroCharacter && HeroCharacter->GetOverlappingWeapon())
			{
				return true;
			}
		}
	}
	return false;
}

void UGA_PickupWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Attach new weapon
	if (AHeroCharacter* Character = GetHeroCharacter(); Character && Character->GetOverlappingWeapon() && GetAbilitySystemComponentFromActorInfo())
	{
		Character->EquipWeapon(Character->GetOverlappingWeapon());
		GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("NoWeapon")));
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
