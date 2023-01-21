// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemyAttributeSet.h"

void UBaseEnemyAttributeSet::ShowDebug() const
{
	Super::ShowDebug();

	if (GEngine)
	{
		ShowDebugOfAttribute(GetAttackDistanceAttribute());
		ShowDebugOfAttribute(GetAttackMaxRateAttribute());
		ShowDebugOfAttribute(GetAttackMinRateAttribute());
		ShowDebugOfAttribute(GetChasingWalkSpeedAttribute());
		ShowDebugOfAttribute(GetDeathLifeSpanAttribute());
		ShowDebugOfAttribute(GetMaxAggroDistanceAttribute());
		ShowDebugOfAttribute(GetPatrolRadiusAttribute());
		ShowDebugOfAttribute(GetPatrolWaitTimeMaxAttribute());
		ShowDebugOfAttribute(GetPatrolWaitTimeMinAttribute());
		ShowDebugOfAttribute(GetPatrollingWalkSpeedAttribute());
	}
}
