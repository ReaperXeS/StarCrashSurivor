// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RXDamageExecCalculation.generated.h"

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API URXDamageExecCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	URXDamageExecCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
