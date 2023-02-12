// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameFramework/Actor.h"
#include "Shield.generated.h"

class UBoxComponent;
class USoundBase;

UCLASS()
class STARCRASHSURVIVOR_API AShield : public AItem
{
	GENERATED_BODY()

	// TODO: A lot in common with AWeapon, refactor to base class
public:
	// Sets default values for this actor's properties
	AShield();

protected:
	void AttachToSocket(USceneComponent* InParent, FName SocketName);

	UPROPERTY(VisibleAnywhere, Category="Shield")
	TObjectPtr<UBoxComponent> ShieldBox;

	UPROPERTY(EditAnywhere, Category="Weapon")
	TObjectPtr<USoundBase> EquipSound;

	UPROPERTY(EditAnywhere, Category="Weapon")
	FName SocketName = "ShieldSocket";

	// Armor value of the shield, applied to character when blocking
	UPROPERTY(EditAnywhere, Category="Weapon")
	float Armor = 25.f;

public:
	FORCEINLINE float GetArmor() const { return Armor; }
	FORCEINLINE FName GetSocketName() const { return SocketName; }

	void Equip(USceneComponent* InParent, FName SocketName, bool bPlaySound, AActor* NewOwner);
};
