// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/HideWeaponAbility.h"
#include "Characters/HeroCharacter.h"

void UHideWeaponAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	// if (AHeroCharacter* Character = Cast<AHeroCharacter>(GetOwningActorFromActorInfo()); Character && Character->GetOverlappingWeapon() && GetAbilitySystemComponentFromActorInfo())
	// {
	// 	Character->EquipWeapon(Character->GetOverlappingWeapon());
	// 	const UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	// 	AnimInstance->Play
	// 	
	// 	GetAbilitySystemComponentFromActorInfo()->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(FName("NoWeapon")));
	// }

	// EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
	// PlayAnimMontage(EquipMontage, 1, "UnEquip");
	// ActionState = EActionState::EAS_Equipping;
	// CharacterState = ECharacterState::ECS_UnEquipped;

	// else if (CanShowWeapon())
	// {
	// 	// Attach new weapon
	// 	PlayAnimMontage(EquipMontage, 1, "Equip");
	// 	ActionState = EActionState::EAS_Equipping;
	// 	CharacterState = ECharacterState::ECS_EquippedOneHanded;
	// }
}
