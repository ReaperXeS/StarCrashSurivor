// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Characters/Abilities/HeroAttributeSet.h"
#include "Enemy.generated.h"


class UBaseEnemyAttributeSet;
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

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UHealthBarComponent* HealthBarWidget;

	/********************************************/
	/*				Abilities					*/
	/********************************************/

	virtual void InitializeAttributeSet() override;

	UPROPERTY(BlueprintReadOnly)
	const UBaseEnemyAttributeSet* EnemyAttributeSet;

	void AttributeChanged(const FOnAttributeChangeData& Data) const;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayAbility> AttackAbility;

	FGameplayAbilitySpecHandle AttackAbilityHandle;

	/**
	 * States
	 */
	virtual void Die_Implementation() override;

	/**
	 * AI
	 */
	UPROPERTY()
	AAIController* AIController;

	UPROPERTY(EditInstanceOnly, Category = "AI")
	AActor* CurrentPatrolTarget;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "AI")
	TArray<AActor*> PatrolTargets;

	bool InTargetRange(const AActor* Target, double Radius) const;

	UFUNCTION(BlueprintCallable)
	AActor* ComputeNewPatrolTarget();

	UPROPERTY(VisibleAnywhere, Category = "AI")
	UPawnSensingComponent* PawnSensingComponent;

	UFUNCTION()
	void PawnSeen(APawn* Pawn);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	/*****
	 * Combat
	 *****/
	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ASoul> SoulClass;

	void SpawnSoul();

	UFUNCTION(BlueprintCallable)
	void UpdateHealthBarWidgetVisibility(const bool bVisible) const;

	void UpdateCombatTarget(AActor* NewTarget);

public:
	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
	bool IsOutsideCombatRadius() const;

	void CheckCombatTarget();
};
