// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HeroOverlay.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UHeroOverlay::SetGold(const int32 Amount) const
{
	if (GoldText)
	{
		GoldText->SetText(FText::FromString(FString::FromInt(Amount)));
	}
}

void UHeroOverlay::SetSoul(const int32 Amount) const
{
	if (SoulText)
	{
		SoulText->SetText(FText::FromString(FString::FromInt(Amount)));
	}
}

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
