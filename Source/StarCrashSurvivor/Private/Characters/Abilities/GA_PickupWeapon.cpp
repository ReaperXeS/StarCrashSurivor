// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/GA_PickupWeapon.h"
#include "Characters/HeroCharacter.h"
#include "Items/Weapons/Weapon.h"
#include "AbilitySystemComponent.h"

TObjectPtr<AHeroCharacter> UGA_PickupWeapon::GetHeroCharacter() const
{
	return Cast<AHeroCharacter>(GetOwningActorFromActorInfo());
}

bool UGA_PickupWeapon::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		if (const AActor* AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr)
		{
			if (const AHeroCharacter* HeroCharacter = Cast<AHeroCharacter>(AvatarActor); HeroCharacter && HeroCharacter->GetOverlappingItem())
			{
				return true;
			}
		}
	}
	return false;
}

void UGA_PickupWeapon::EquipWeapon()
{
	// Attach new weapon
	if (AHeroCharacter* Character = GetHeroCharacter(); Character && Character->GetOverlappingItem())
	{
		Character->EquipItem(Character->GetOverlappingItem());
	}
}
