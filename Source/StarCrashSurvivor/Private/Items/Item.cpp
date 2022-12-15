// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/PickupInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Overlap with pawn only

	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>("Embers");
	ItemEffect->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlapBegin);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);
}

float AItem::TransformedSin() const
{
	return Amplitude * FMath::Sin(GetWorld()->GetTimeSeconds() * TimeConstant);
}

void AItem::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IPickupInterface* Hero = Cast<IPickupInterface>(OtherActor))
	{
		Hero->SetOverlappingItem(this);
	}
}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IPickupInterface* Hero = Cast<IPickupInterface>(OtherActor))
	{
		Hero->SetOverlappingItem(nullptr);
	}
}

void AItem::SpawnPickupSound() const
{
	if (PickupSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(this, PickupSound, GetActorLocation());
	}
}

void AItem::SpawnPickupEffect() const
{
	if (PickupEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PickupEffect, GetActorLocation());
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}
