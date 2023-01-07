// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Enemy.h"

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree)
	{
		RunBehaviorTree(BehaviorTree);
	}

	EnemyPawn = Cast<AEnemy>(InPawn);
}

void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetBlackboardComponent() && EnemyPawn)
	{
		EnemyPawn->CheckCombatTarget();
		GetBlackboardComponent()->SetValueAsObject(FName("CombatTarget"), EnemyPawn->GetCombatTarget());
	}
}
