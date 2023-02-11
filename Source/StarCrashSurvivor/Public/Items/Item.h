// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UGameplayEffect;
class UStaticMeshComponent;
class UNiagaraComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EItemState : uint8
{
	EIS_Hovering UMETA(DisplayName = "Hovering"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
};

UCLASS()
class STARCRASHSURVIVOR_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|Hovering Parameters")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reaper|Hovering Parameters")
	float TimeConstant = 5.f;

	float TransformedSin() const;

	UFUNCTION()
	virtual void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(VisibleAnywhere, Category = "Reaper|Effects")
	UNiagaraComponent* ItemEffect;

	UPROPERTY(EditAnywhere, Category = "Reaper|Effects")
	class UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere, Category = "Reaper|Sounds")
	USoundBase* PickupSound;

	void SpawnPickupSound() const;
	void SpawnPickupEffect() const;

public:
	// Effect to applied when picked up
	UPROPERTY(EditAnywhere, Category="Reaper|Effects")
	TSubclassOf<UGameplayEffect> PickupGameplayEffect;
};
