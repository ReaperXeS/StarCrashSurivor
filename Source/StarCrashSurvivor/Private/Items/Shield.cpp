// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Shield.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

// Sets default values
AShield::AShield()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShieldBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ShieldBox"));
	ShieldBox->SetupAttachment(RootComponent);
	ShieldBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShieldBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	ShieldBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AShield::AttachToSocket(USceneComponent* InParent, FName SocketNameParam)
{
	if (InParent && MeshComp)
	{
		ItemState = EItemState::EIS_Equipped;
		MeshComp->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketNameParam);
	}
}

void AShield::Equip(USceneComponent* InParent, FName SocketNameParam, bool bPlaySound, AActor* NewOwner)
{
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	AttachToSocket(InParent, SocketNameParam);
	if (bPlaySound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}

	if (SphereComp)
	{
		SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}
