// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeroOverlay.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UHeroOverlay : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulText;
public:
	void SetGold(const int32 Amount) const;
	void SetSoul(const int32 Amount) const;
	void SetHealthBarPercent(const float HealthPercent) const;
	void SetStaminaBarPercent(const float StaminaPercent) const;
};
