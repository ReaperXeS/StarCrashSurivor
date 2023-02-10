// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;
class USceneComponent;
/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UFUNCTION()
	void OnWeaponBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AttachToSocket(USceneComponent* InParent, FName SocketName);

	UPROPERTY(EditAnywhere, Category="Weapon")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	UBoxComponent* WeaponBox;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere, Category="Weapon")
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category="Weapon")
	FVector BoxTraceExtent = FVector(5.f, 5.f, 5.f);

	UPROPERTY(EditAnywhere, Category = "Development")
	bool bDebug = false;

	UFUNCTION(BlueprintImplementableEvent, Category="Weapon")
	void CreateFields(const FVector& FieldLocation);

	UPROPERTY(EditAnywhere, Category="Weapon")
	float BaseDamage = 20.f;

	bool ActorIsSameType(const AActor* OtherActor) const;

	UPROPERTY(EditAnywhere, Category="Weapon")
	FName SocketName = "Socket_RightHand";

public:
	void Equip(USceneComponent* InParent, FName SocketName, bool bPlaySound, AActor* NewOwner);

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }
	FORCEINLINE FName GetSocketName() const { return SocketName; }

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
};
