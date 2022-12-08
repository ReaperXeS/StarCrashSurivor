// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StarCrashSurvivorHUD.generated.h"

class UHeroOverlay;
/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API AStarCrashSurvivorHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHeroOverlay> HeroOverlayClass;
private:
	UPROPERTY()
	UHeroOverlay* HeroOverlay;

public:
	FORCEINLINE UHeroOverlay* GetHeroOverlay() const { return HeroOverlay; }
};
