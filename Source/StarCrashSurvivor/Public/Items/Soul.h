// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API ASoul : public AItem
{
	GENERATED_BODY()
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	float AboveGroundZLocation = 100.f;

	UPROPERTY(EditAnywhere, Category = "Soul Properties")
	float DriftRate = -15.f;

	double DesiredZLocation = 0.0f;

public:
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE void SetSouls(const int32 NewSouls) { Souls = NewSouls; }
};
