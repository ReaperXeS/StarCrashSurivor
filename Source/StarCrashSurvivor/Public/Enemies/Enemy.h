// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class UParticleSystem;
class UAttributesComponent;
class UWidgetComponent;

UCLASS()
class STARCRASHSURVIVOR_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/***
	 * Animation Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "FX")
	UParticleSystem* HitParticle;

	UPROPERTY(EditAnywhere, Category = "Development")
	bool bDebug = false;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAttributesComponent* AttributesComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UWidgetComponent* HealthBarWidget;

public:
};
