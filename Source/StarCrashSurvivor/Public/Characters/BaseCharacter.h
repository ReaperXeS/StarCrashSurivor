// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaseCharacter.generated.h"

// Forward Declarations
class AWeapon;
class UAttributesComponent;

UCLASS()
class STARCRASHSURVIVOR_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void DirectionalHitReact(const FVector& ImpactPoint);
	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UAttributesComponent* AttributesComponent;

	UPROPERTY(VisibleInstanceOnly)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	AActor* CombatTarget;

	virtual bool CanAttack() const;
	virtual void Attack();

	virtual void Die();

	/*****************************
	 * States
	 ****************************/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	TEnumAsByte<EDeathState> DeathState;

	/***
	 * Animation Montages
	 */
	uint8 PlayAnimMontageRandomSection(UAnimMontage* AnimMontage);
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* EquipMontage;

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

	UPROPERTY(EditAnywhere, Category = "Combat")
	float WarpTargetDistance = 75.f;

	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();
public:
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void OnAttackEnd();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UpdateWeaponCollision(ECollisionEnabled::Type NewCollisionEnabled) const;

	UFUNCTION(BlueprintCallable, Category = "State")
	bool IsDead() const;

	FORCEINLINE EDeathState GetDeathState() const { return DeathState; }
};
