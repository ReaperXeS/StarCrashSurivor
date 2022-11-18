// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API AWeapon : public AItem
{
	GENERATED_BODY()
protected:
	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	void AttachToSocket(USceneComponent* InParent, FName SocketName);

	UPROPERTY(EditAnywhere, Category="Weapon")
	USoundBase* EquipSound;
public:
	void Equip(USceneComponent* InParent, FName SocketName, bool bPlaySound);
};
