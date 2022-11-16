// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "Animation/AnimInstance.h"
#include "HeroAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UHeroAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class AHeroCharacter* HeroCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* HeroMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool IsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
};
