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
	UBoxComponent* ShieldBox;

	UPROPERTY(EditAnywhere, Category="Weapon")
	USoundBase* EquipSound;

	UPROPERTY(EditAnywhere, Category="Weapon")
	FName SocketName = "ShieldSocket";

public:
	FORCEINLINE FName GetSocketName() const { return SocketName; }

	void Equip(USceneComponent* InParent, FName SocketName, bool bPlaySound, AActor* NewOwner);
};
