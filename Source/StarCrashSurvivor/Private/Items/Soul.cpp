// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"

#include "Interfaces/PickupInterface.h"

void ASoul::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().Z > DesiredZLocation)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, DriftRate * DeltaTime));
	}
}

void ASoul::BeginPlay()
{
	Super::BeginPlay();

	FCollisionQueryParams CollisionParams;
	CollisionParams.bDebugQuery = true;
	FHitResult OutHit;
	if (GetWorld()->LineTraceSingleByObjectType(OutHit, GetActorLocation(), GetActorLocation() - FVector(0, 0, 2000), FCollisionObjectQueryParams(ECC_WorldStatic), CollisionParams))
	{
		DesiredZLocation = OutHit.ImpactPoint.Z + AboveGroundZLocation;
	}
}

void ASoul::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor))
	{
		PickupInterface->AddSouls(this);
		SpawnPickupEffect();
		SpawnPickupSound();
		Destroy();
	}
}
