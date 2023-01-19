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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxAggroDistance;

public:
	ATTRIBUTE_ACCESSORS(UBaseEnemyAttributeSet, MaxAggroDistance);
};
