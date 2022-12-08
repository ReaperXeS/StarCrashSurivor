// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/StarCrashSurvivorHUD.h"

#include "HUD/HeroOverlay.h"

void AStarCrashSurvivorHUD::BeginPlay()
{
	Super::BeginPlay();

	if (const UWorld* World = GetWorld(); HeroOverlayClass && World)
	{
		if (APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			HeroOverlay = CreateWidget<UHeroOverlay>(PlayerController, HeroOverlayClass);
			HeroOverlay->AddToViewport();
		}
	}
}
