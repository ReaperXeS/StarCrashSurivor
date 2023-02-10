// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputAction.h"
#include "Interfaces/PickupInterface.h"
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
	virtual float TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/********************************************/
	/*				IPickupInterface			*/
	/********************************************/
	virtual void SetOverlappingItem(AItem* Item) override;

	// Apply Gold effect to the player from the Treasure
	virtual void AddGold(class ATreasure* Treasure) override;

	// Apply Soul effect to the player from the Treasure
	virtual void AddSouls(class ASoul* Soul) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/********************************************/
	/*				Abilities					*/
	/********************************************/
	// Called when Attributes changes and update UI accordingly
	void AttributeChanged(const FOnAttributeChangeData& Data);

	// Camera for the Hero Character
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	// Camera Boom for the Hero Character
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	// Hair Component for the Hero Character
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Hair;

	// Eye Brows Component for the Hero Character
	UPROPERTY(VisibleAnywhere)
	UGroomComponent* EyeBrows;

	/**
	 * Input Mapping Context of the Hero Character
	 **/
	UPROPERTY(EditAnywhere, Category="Reaper|Input")
	UInputMappingContext* HeroMappingContext;

	// Input Action call back when player press button of an action
	void ActionInputWithAbilityPressed(const FInputActionInstance& InputActionInstance);
	// Input Action call back when player release button of an action
	void ActionInputWithAbilityReleased(const FInputActionInstance& InputActionInstance);

private:
	// Input Action for Movement
	UPROPERTY(EditAnywhere, Category="Reaper|Input")
	UInputAction* ActionMove;
	// ActionMove Input Action Callback
	void MoveCharacter(const FInputActionValue& ActionValue);

	// Input Action for Looking Around
	UPROPERTY(EditAnywhere, Category="Reaper|Input")
	UInputAction* ActionLookAround;
	// ActionLookAround Input Action Callback
	void LookAround(const FInputActionValue& ActionValue);

	// Input Action for Zooming In or Out
	UPROPERTY(EditAnywhere, Category="Reaper|Input")
	UInputAction* ActionZoomInOutCamera;
	// ActionZoomInOutCamera Action Callback
	void ZoomCamera(const FInputActionValue& ActionValue);

	// Function to handle moving forward/backward
	void MoveForward(float AxisValue);
	// Function to handle strafing
	void MoveRight(float AxisValue);

	// Reference to the Item the player is overlapping
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	// Initialize the Hud Overlay
	void InitializeHeroOverlay();

	// Hud overlay for the Hero Character
	UPROPERTY()
	UHeroOverlay* HeroOverlay;

public:
	// Getter for OverlappingItem
	FORCEINLINE AItem* GetOverlappingItem() const { return OverlappingItem; }
	// Getter for OverlappingWeapon
	AWeapon* GetOverlappingWeapon() const;

	// Called from Anim Instance when the player is hiding his weapon
	void OnHideWeaponAttachToSocket();
	// Called from Anim Instance when the player is show his weapon
	void OnShowWeaponAttachToSocket();

	// Called an item is picked up
	void EquipItem(AItem* Item);
};
