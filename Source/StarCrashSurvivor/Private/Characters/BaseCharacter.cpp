// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"

#include "Characters/CharacterTypes.h"
#include "Components/AttributesComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttributesComponent = CreateDefaultSubobject<UAttributesComponent>("Attributes");

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateWeaponCollision(ECollisionEnabled::NoCollision);
}

bool ABaseCharacter::CanAttack() const
{
	return true;
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
	if (IsDead()) { return; }

	Tags.Add(C_TAG_DEAD);

	// Compute Death State
	const uint8 SectionIndex = PlayAnimMontageRandomSection(DeathMontage);
	if (const TEnumAsByte<EDeathState> DeathPose(SectionIndex); DeathPose <= EDS_Death6)
	{
		DeathState = DeathPose;
	}
}

bool ABaseCharacter::IsDead() const
{
	return ActorHasTag(C_TAG_DEAD);
}

uint8 ABaseCharacter::PlayAnimMontageRandomSection(UAnimMontage* AnimMontage)
{
	if (AnimMontage)
	{
		const uint8 SectionIndex = FMath::RandRange(0, AnimMontage->CompositeSections.Num() - 1);
		const FName SectionName = AnimMontage->CompositeSections[SectionIndex].SectionName;
		if (bDebug)
		{
			UE_LOG(LogTemp, Warning, TEXT("Playing section %s"), *SectionName.ToString());
		}
		PlayAnimMontage(AnimMontage, 1.0f, SectionName);
		return SectionIndex;
	}
	return -1;
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if (CombatTarget == nullptr) { return FVector(); }

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::OnAttackEnd()
{
}

void ABaseCharacter::UpdateWeaponCollision(const ECollisionEnabled::Type NewCollisionEnabled) const
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(NewCollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (AttributesComponent && AttributesComponent->IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else
	{
		Die();
	}

	UpdateWeaponCollision(ECollisionEnabled::NoCollision);

	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, ImpactPoint);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1 |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(GetActorForwardVector(), ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// If cross product points down, Theta is negative
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta = -Theta;
	}

	if (GEngine && bDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Theta: %f"), Theta));
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 60.f, 5.f, FColor::Red, 5.f);
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);
		UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100.f, 5.f, FColor::Blue, 5.f);

		// Draw debug sphere
		DrawDebugSphere(GetWorld(), ImpactPoint, 8.0f, 12, FColor::Red, false, 5.0f);
	}

	FName Section = "FromBack";
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = "FromFront";
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = "FromLeft";
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = "FromRight";
	}

	PlayAnimMontage(HitReactMontage, 1, Section);
}

float ABaseCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AttributesComponent)
	{
		AttributesComponent->ReceiveDamage(Damage);
	}
	return Damage;
}
