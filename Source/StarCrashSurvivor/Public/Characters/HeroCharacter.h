// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputAction.h"
#include "Interfaces/PickupInterface.h"
#include "AbilitySystemInterface.h"
#include "Abilities/HeroAttributeSet.h"
#include "HeroCharacter.generated.h"

class UBaseGameplayAbility;
class UGameplayAbility;
/***
 * Forward Declarations
 */
class AItem;
class AWeapon;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UHeroOverlay;
class UAnimMontage;
class ATreasure;
class ASoul;
class UInputAction;
class UInputMappingContext;
class UAbilitySystemComponent;
class UHeroAttributeSet;

UCLASS()
class STARCRASHSURVIVOR_API AHeroCharacter : public ABaseCharacter, public IPickupInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeroCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddSouls(ASoul* Soul) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/********************************************/
	/*				Abilities					*/
	/********************************************/
	void StaminaChanged(const FOnAttributeChangeData& Data);

	/** Ability System Component. Required to use Gameplay Attributes and Gameplay Abilities. */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	const UHeroAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<UBaseGameplayAbility> AttackLightAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UBaseGameplayAbility>> StartupAbilities;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* EyeBrows;

	/*** 
	 * States
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "States")
	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;
	EActionState ActionState = EActionState::EAS_Idle;

	virtual bool CanAttack() const override;
	virtual void Attack() override;

	/**
	 * Input
	 **/
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* HeroMappingContext;

	void ActionInputWithAbility(const FInputActionInstance& InputActionInstance);

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionMove;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionLookAround;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionZoomInOutCamera;

	/*********************************************/
	/* Combo Attack
	 *********************************************/
	bool bComboAttackWindowOpened = false;
	bool bComboAttackTriggered = false;

private:
	/**
	 * Movement functions
	 */
	void MoveCharacter(const FInputActionValue& ActionValue);
	void LookAround(const FInputActionValue& ActionValue);
	void ZoomCamera(const FInputActionValue& ActionValue);

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	/*****
	 * HUD
	 ****/
	UPROPERTY()
	UHeroOverlay* HeroOverlay;

	void InitializeHeroOverlay();

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
	FORCEINLINE AItem* GetOverlappingItem() const { return OverlappingItem; }
	AWeapon* GetOverlappingWeapon() const;

	virtual
	FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }


	virtual void OnAttackComboBegin();
	virtual void OnAttackComboEnd(FName NextAttackSectionName);
	virtual void OnAttackEnd() override;
	virtual void OnDodgeEnd() override;
	void OnEquipEnd();
	void OnHideWeaponAttachToSocket();
	void OnHitReactEnd();
	void OnShowWeaponAttachToSocket();

	void EquipWeapon(AWeapon* Weapon);
};
