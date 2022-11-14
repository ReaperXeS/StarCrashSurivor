// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"

#include "Characters/HeroCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	SetRootComponent(MeshComp);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Overlap with pawn only
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlapBegin);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereOverlapEnd);
}

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(GetWorld()->GetTimeSeconds() * TimeConstant);
}

void AItem::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AHeroCharacter* Hero = Cast<AHeroCharacter>(OtherActor))
	{
		Hero->SetOverlappingItem(this);
	}
}

void AItem::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AHeroCharacter* Hero = Cast<AHeroCharacter>(OtherActor))
	{
		Hero->SetOverlappingItem(nullptr);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HoveringEnabled)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

