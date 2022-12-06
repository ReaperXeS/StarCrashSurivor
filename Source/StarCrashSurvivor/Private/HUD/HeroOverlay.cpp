// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HeroOverlay.h"

#include "Components/ProgressBar.h"


void UHeroOverlay::SetHealthBarPercent(const float HealthPercent) const
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(HealthPercent);
	}
}

void UHeroOverlay::SetStaminaBarPercent(const float StaminaPercent) const
{
	if (StaminaProgressBar)
	{
		StaminaProgressBar->SetPercent(StaminaPercent);
	}
}
