// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
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
class UAbilitySystemComponent;
class UHeroAttributeSet;
class UInputMappingContext;

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

	/********************************************/
	/*				IPickupInterface			*/
	/********************************************/
	virtual void SetOverlappingItem(AItem* Item) override;

	virtual void AddGold(class ATreasure* Treasure) override;
	virtual void AddSouls(class ASoul* Soul) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/********************************************/
	/*				Abilities					*/
	/********************************************/
	void AttributeChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayEffect> SoulEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	TSubclassOf<UGameplayEffect> TreasureEffect;


	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* EyeBrows;

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
	FORCEINLINE AItem* GetOverlappingItem() const { return OverlappingItem; }
	AWeapon* GetOverlappingWeapon() const;

	virtual FORCEINLINE UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }


	virtual void OnAttackComboBegin();
	virtual void OnAttackComboEnd(FName NextAttackSectionName);
	void OnHideWeaponAttachToSocket();
	void OnShowWeaponAttachToSocket();

	void EquipWeapon(AWeapon* Weapon);
};
