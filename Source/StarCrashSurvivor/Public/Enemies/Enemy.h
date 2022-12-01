// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Enemy.generated.h"

class AAIController;
class UParticleSystem;
class UAttributesComponent;
class UHealthBarComponent;

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
	UAnimMontage* DeathMontage;

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
	UHealthBarComponent* HealthBarWidget;

	/**
	 * States
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EDeathState DeathState = EDeathState::EDS_Alive;

	UPROPERTY(VisibleAnywhere, Category = "State")
	float DeathLifeSpan = 5.f;

	void Die();

	/**
	 * AI
	 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxAggroDistance = 1000.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 15.f;

	UPROPERTY()
	AAIController* AIController;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolWaitTimeMin = 3.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolWaitTimeMax = 8.f;

	void MoveToTarget(const AActor* Target) const;
	bool InTargetRange(const AActor* Target, double Radius) const;

	FTimerHandle PatrolTimerHandle;
	void PatrolTimerFinished() const;
	AActor* ComputeNewPatrolTarget();
	void CheckCombatTarget();
	void CheckCurrentPatrolTarget();
public:
	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
