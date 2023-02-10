// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HeroAnimInstance.h"

#include "Characters/HeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHeroAnimInstance::OnHideWeaponAttachToSocket()
{
	if (HeroCharacter)
	{
		HeroCharacter->OnHideWeaponAttachToSocket();
	}
}

void UHeroAnimInstance::OnShowWeaponAttachToSocket()
{
	if (HeroCharacter)
	{
		HeroCharacter->OnShowWeaponAttachToSocket();
	}
}

void UHeroAnimInstance::EnableWeaponCollision()
{
	if (HeroCharacter)
	{
		HeroCharacter->UpdateWeaponCollision(ECollisionEnabled::QueryOnly);
	}
}

void UHeroAnimInstance::DisableWeaponCollision()
{
	if (HeroCharacter)
	{
		HeroCharacter->UpdateWeaponCollision(ECollisionEnabled::NoCollision);
	}
}

void UHeroAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	HeroCharacter = Cast<AHeroCharacter>(TryGetPawnOwner());
	if (HeroCharacter)
	{
		HeroMovementComponent = HeroCharacter->GetCharacterMovement();
	}
}

void UHeroAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (HeroMovementComponent)
	{
		GroundSpeed = HeroMovementComponent->Velocity.Size2D();
		IsFalling = HeroMovementComponent->IsFalling();
	}

	if (HeroCharacter)
	{
		DeathState = HeroCharacter->GetDeathState();
		IsDead = HeroCharacter->IsDead();
	}
}

void UHeroAnimInstance::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (HeroCharacter)
	{
		HeroCharacter->GetOwnedGameplayTags(TagContainer);
	}
	else
	{
		TagContainer = FGameplayTagContainer();
	}
}
