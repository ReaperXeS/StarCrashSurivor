// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CharacterTypes.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Abilities/GameplayAbility.h"
#include "BaseCharacter.generated.h"

// Forward Declarations
class AShield;
class AWeapon;
class UAbilitySystemComponent;
class UBaseGameplayAbility;
class UGameplayEffect;
class UHeroAttributeSet;

UCLASS()
class STARCRASHSURVIVOR_API ABaseCharacter : public ACharacter, public IHitInterface, public IGameplayTagAssetInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IHitInterface
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// Called when ApplyDamage is called on the character
	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Initialize character attributes
	virtual void BeginPlay() override;

	// TODO: Review this
	virtual void DirectionalHitReact(const FVector& ImpactPoint);

	// Function to compute direction of impact
	UFUNCTION(BlueprintCallable)
	FName ComputeDirectionalHitSection(const FVector& ImpactPoint) const;

	/********************************************/
	/*				Abilities					*/
	/********************************************/

	// Used to apply ShieldArmorEffect on the character
	UFUNCTION(BlueprintCallable)
	void ApplyShieldArmor(bool bAdd) const;

	// Effect to applied when blocking with a shield
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reaper|Abilities")
	TSubclassOf<UGameplayEffect> ShieldArmorEffect;

	// Flag to show some debug info of the GAS system
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|Abilities")
	bool DebugGAS = false;

	// Used to show debug info on the screen
	virtual void ShowDebugGAS() const;

	// Initialize the ability system component with attributes
	virtual void InitializeAttributeSet();

	/** Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reaper|Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	// Attribute set for the character via DataTable
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Reaper|Abilities")
	TObjectPtr<const UHeroAttributeSet> AttributeSet;

	// Abilities granted to the character on spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reaper|Abilities")
	TArray<TSubclassOf<UBaseGameplayAbility>> StartupAbilities;

	// Effect applied when receiving damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|Abilities")
	TSubclassOf<UGameplayEffect> DamageEffect;

	// Reference to the Weapon currently equipped 
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AWeapon> EquippedWeapon;

	// Reference to the Shield currently equipped 
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<AShield> EquippedShield;

	// Current combat target of the character, set only for the enemy right now
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reaper|AI")
	TObjectPtr<AActor> CombatTarget;

	// Called from blueprint when the character dies
	UFUNCTION(BlueprintNativeEvent)
	void Die();

	/*****************************
	 * States
	 ****************************/
	// Used to determine which animation to play and stay when dead
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reaper|State")
	TEnumAsByte<EDeathState> DeathState;

	/***
	 * Animation Montages
	 */

	// Helper to play random section of an animation montage
	UFUNCTION(BlueprintCallable, Category = "Reaper|Animation")
	uint8 PlayAnimMontageRandomSection(UAnimMontage* AnimMontage);

	// Montage to play when the character dies
	UPROPERTY(EditDefaultsOnly, Category = "Reaper|Animation")
	TObjectPtr<UAnimMontage> DeathMontage;

	// Montage to play when the character gets hit
	UPROPERTY(EditDefaultsOnly, Category = "Reaper|Animation")
	TObjectPtr<UAnimMontage> HitReactMontage;

	// Debug flag to show trace and some more info
	UPROPERTY(EditAnywhere, Category = "Reaper|Development")
	bool bDebug = false;

	// Buffer used for not warping too close to the target
	UPROPERTY(EditAnywhere, Category = "Reaper|Combat")
	float WarpTargetDistance = 75.f;

	// Method used to warp to target
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	// Method used to warp to target
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

public:
	// IAbilitySystemInterface
	virtual FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }

	// IGameplayTagAssetInterface
	UFUNCTION(BlueprintCallable, Category = GameplayTags)
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	// Used to set weapon collision, mostly used in AnimNotify
	UFUNCTION(BlueprintCallable, Category = "Reaper|Combat")
	void UpdateWeaponCollision(ECollisionEnabled::Type NewCollisionEnabled) const;

	// Used in AnimInstance to know if character is dead
	UFUNCTION(BlueprintCallable, Category = "Reaper|State")
	bool IsDead() const;

	// Getter for DeathState used in AnimInstance
	FORCEINLINE EDeathState GetDeathState() const { return DeathState; }

	// Getter for CombatTarget used for AI
	FORCEINLINE TObjectPtr<AActor> GetCombatTarget() const { return CombatTarget; }
};
