// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributesComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARCRASHSURVIVOR_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributesComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Gold;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	int32 Souls;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float DodgeCost = 15.f;

	// Stamina Regen Rate per second
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaRegenRate = 8.f;
public:
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	FORCEINLINE float GetHealthPercent() const { return Health / MaxHealth; }
	FORCEINLINE float GetStaminaPercent() const { return Stamina / MaxStamina; }
	FORCEINLINE bool HasEnoughStamina(const float StaminaCost) const { return StaminaCost <= Stamina; }
	FORCEINLINE bool IsAlive() const { return Health > 0.f; }

	void AddGold(int32 Amount);
	void AddSouls(int32 Amount);
	void ReceiveDamage(const float DamageAmount);
	void RegenStamina(const float DeltaTime);
	void UseStamina(const float StaminaCost);
};
