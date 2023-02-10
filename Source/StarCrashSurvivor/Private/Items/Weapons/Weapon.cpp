// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Interfaces/HitInterface.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponBox"));
	WeaponBox->SetupAttachment(RootComponent);
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
}

void AWeapon::AttachToSocket(USceneComponent* InParent, const FName SocketNameParam)
{
	if (InParent && MeshComp)
	{
		ItemState = EItemState::EIS_Equipped;
		MeshComp->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketNameParam);
	}
}

bool AWeapon::ActorIsSameType(const AActor* OtherActor) const
{
	return GetOwner() != nullptr && OtherActor != nullptr && OtherActor->Tags.Num() > 0 && GetOwner()->ActorHasTag(OtherActor->Tags[0]);
}

void AWeapon::Equip(USceneComponent* InParent, FName SocketNameParam, bool bPlaySound, AActor* NewOwner)
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


void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnWeaponBoxOverlapBegin);
}

void AWeapon::OnWeaponBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ActorIsSameType(OtherActor)) { return; }

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	TArray<FHitResult> OutHits;
	UKismetSystemLibrary::BoxTraceMulti(
		this,
		BoxTraceStart->GetComponentLocation(),
		BoxTraceEnd->GetComponentLocation(),
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		IgnoreActors,
		bDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		OutHits,
		true);

	for (auto Hit : OutHits)
	{
		if (Hit.GetActor())
		{
			if (const IHitInterface* HitInterface = Cast<IHitInterface>(Hit.GetActor()); !ActorIsSameType(Hit.GetActor()) && HitInterface && IgnoreActors.Find(Hit.GetActor()) == INDEX_NONE)
			{
				UGameplayStatics::ApplyDamage(Hit.GetActor(), BaseDamage, GetOwner() ? GetOwner()->GetInstigatorController() : nullptr, this, UDamageType::StaticClass());
				HitInterface->Execute_GetHit(Hit.GetActor(), Hit.ImpactPoint, GetOwner());
			}
			IgnoreActors.AddUnique(Hit.GetActor());
			CreateFields(Hit.ImpactPoint);
		}
	}
}

void AWeapon::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
