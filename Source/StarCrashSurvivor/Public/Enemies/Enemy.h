// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Characters/Abilities/HeroAttributeSet.h"
#include "Enemy.generated.h"


class AAIController;
class ASoul;
class UAttributesComponent;
class UHealthBarComponent;
class UParticleSystem;
class UPawnSensingComponent;


UCLASS()
class STARCRASHSURVIVOR_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void StartAttackTimer();
	void ClearAttackTimer();
	virtual bool CanAttack() const override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UHealthBarComponent* HealthBarWidget;

	virtual void Attack() override;
	virtual void OnAttackEnd() override;

	/********************************************/
	/*				Abilities					*/
	/********************************************/
	void AttributeChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayAbility> AttackAbility;

	FGameplayAbilitySpecHandle AttackAbilityHandle;

	/**
	 * States
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

	UPROPERTY(VisibleAnywhere, Category = "State")
	float DeathLifeSpan = 5.f;

	virtual void Die_Implementation() override;

	void UpdateEnemyState(const EEnemyState NewState, AActor* Target);

	bool IsCombatTargetDead() const;

	/**
	 * AI
	 */

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackDistance = 150.f;

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
	void ClearPatrolTimer();
	void PatrolTimerFinished() const;
	AActor* ComputeNewPatrolTarget();

	void CheckCombatTarget();
	void CheckCurrentPatrolTarget();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrollingWalkSpeed = 125.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float ChasingWalkSpeed = 300.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	/*****
	 * Combat
	 *****/
	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMinRate = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMaxRate = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ASoul> SoulClass;

	void SpawnSoul();

private:
	void UpdateHealthBarWidgetVisibility(const bool bVisible) const;
	bool IsOutsideCombatRadius() const;

public:
	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
};
