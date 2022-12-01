// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"

#include "HUD/HealthBar.h"

void UHealthBarComponent::SetHealthPercent(const float HealthPercent)
{
	if (!HealthBarWidget)
	{
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthBarPercent(HealthPercent);
	}
}
