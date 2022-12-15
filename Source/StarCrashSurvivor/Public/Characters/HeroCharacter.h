// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "InputAction.h"
#include "Interfaces/PickupInterface.h"
#include "HeroCharacter.generated.h"

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

UCLASS()
class STARCRASHSURVIVOR_API AHeroCharacter : public ABaseCharacter, public IPickupInterface
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

	virtual void Jump() override;

	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddGold(ATreasure* Treasure) override;
	virtual void AddSouls(ASoul* Soul) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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
	ECharacterState CharacterState = ECharacterState::ECS_UnEquipped;
	EActionState ActionState = EActionState::EAS_Idle;

	virtual bool CanAttack() const override;
	virtual void Attack() override;

	bool CanDodge() const;
	void Dodge();

	/**
	 * Input
	 **/
	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* HeroMappingContext;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionMove;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionLookAround;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionJump;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionZoomInOutCamera;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionInteract;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionAttack;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* ActionDodge;

private:
	/**
	 * Movement functions
	 */
	void MoveCharacter(const FInputActionValue& ActionValue);
	void LookAround(const FInputActionValue& ActionValue);
	void ZoomCamera(const FInputActionValue& ActionValue);

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	/*
	 * Action Mapping
	 * */
	void Interact();
	void EKeyPressed();

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	bool CanShowWeapon() const;
	bool CanHideWeapon() const;
	bool CanPickupWeapon() const;

	/*****
	 * HUD
	 ****/
	UPROPERTY()
	UHeroOverlay* HeroOverlay;

	void InitializeHeroOverlay();

public:
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


	virtual void OnAttackEnd() override;
	virtual void OnDodgeEnd() override;
	void OnEquipEnd();
	void OnHideWeaponAttachToSocket();
	void OnHitReactEnd();
	void OnShowWeaponAttachToSocket();
};
