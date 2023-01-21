// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Abilities/HeroAttributeSet.h"
#include "BaseEnemyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UBaseEnemyAttributeSet : public UHeroAttributeSet
{
	GENERATED_BODY()

public:

protected:
	/* Distance so enemy can attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData AttackDistance;

	/* Minimum Attack rate (time before re-attacking) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData AttackMinRate;

	/* Maximum Attack rate (time before re-attacking) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData AttackMaxRate;

	/* Walk speed when chasing/attacking player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData ChasingWalkSpeed;

	/* Time dead until corpse disappear */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData DeathLifeSpan;

	/* Distance to enemy greater than this it will loose interest. If less it will chase the player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxAggroDistance;

	/* Radius acceptance to reach a patrol target*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData PatrolRadius;

	/* Minimum Patrol Wait Time until finding new patrol target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData PatrolWaitTimeMin;

	/* Maximum Patrol Wait Time until finding new patrol target */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData PatrolWaitTimeMax;

	/* Walk speed when patrolling */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData PatrollingWalkSpeed;

public:
	virtual void ShowDebug() const override;

	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, AttackDistance);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, AttackMaxRate);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, AttackMinRate);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, ChasingWalkSpeed);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, DeathLifeSpan);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, MaxAggroDistance);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, PatrolRadius);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, PatrolWaitTimeMax);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, PatrolWaitTimeMin);
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, PatrollingWalkSpeed);
};
