// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameplayTagAssetInterface.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UHeroAnimInstance : public UAnimInstance, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void OnAttackComboBegin();
	UFUNCTION(BlueprintCallable)
	void OnAttackComboEnd(FName NextAttackSectionName);
	UFUNCTION(BlueprintCallable)
	void OnHideWeaponAttachToSocket();
	UFUNCTION(BlueprintCallable)
	void OnShowWeaponAttachToSocket();

	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void DisableWeaponCollision();

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/**
	 * Get any owned gameplay tags on the asset
	 * 
	 * @param TagContainer	[OUT] Set of tags on the asset
	 */
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const;

	UPROPERTY(BlueprintReadOnly)
	class AHeroCharacter* HeroCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* HeroMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	bool IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	TEnumAsByte<EDeathState> DeathState;
};
