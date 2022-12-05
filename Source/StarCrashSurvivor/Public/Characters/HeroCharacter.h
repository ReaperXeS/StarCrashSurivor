// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "HeroCharacter.generated.h"

/***
 * Forward Declarations
 */
class AItem;
class AWeapon;
class USpringArmComponent;
class UCameraComponent;
class UGroomComponent;
class UAnimMontage;

UCLASS()
class STARCRASHSURVIVOR_API AHeroCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHeroCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint) override;
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
private:
	/**
	 * Movement functions
	 */
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUp(float AxisValue);
	void Turn(float AxisValue);

	/*
	 * Action Mapping
	 * */
	void EKeyPressed();
	void ZoomInCamera();
	void ZoomOutCamera();

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	bool CanShowWeapon() const;
	bool CanHideWeapon() const;
	bool CanPickupWeapon() const;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }


	virtual void OnAttackEnd() override;
	void OnEquipEnd();
	void OnHideWeaponAttachToSocket();
	void OnHitReactEnd();
	void OnShowWeaponAttachToSocket();
};
