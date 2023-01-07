// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UBehaviorTree;
class AEnemy;
/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTree;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY()
	AEnemy* EnemyPawn;

	virtual void Tick(float DeltaTime) override;
};
