// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"

#include "Items/Soul.h"
#include "Characters/HeroCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemies/BaseEnemyAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Perception/PawnSensingComponent.h"
#include "Math/UnrealMathUtility.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>("HealthBarWidget");
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
	PawnSensingComponent->SightRadius = 4000.f;

	Tags.Add(C_TAG_ENEMY);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		// TODO: Crash here AttributeSet is null maybe??
		HealthBarWidget->SetHealthPercent(AttributeSet->GetHealth() / AttributeSet->GetMaxHealth());
		UpdateHealthBarWidgetVisibility(false);
	}

	UE_LOG(LogTemp, Warning, TEXT("MaxAggroDistance: %f"), EnemyAttributeSet->GetMaxAggroDistance());

	check(AbilitySystemComponent);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AEnemy::AttributeChanged);

	// Give Attack Ability
	// TODO: Review Abilities on Enemy
	if (AttackAbility)
	{
		AttackAbilityHandle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AttackAbility));
	}

	// Set walk speed in patrolling walk speed
	GetCharacterMovement()->MaxWalkSpeed = PatrollingWalkSpeed;

	// Set up the pawn sensing component
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	// Spawn weapon and attach to socket
	if (UWorld* World = GetWorld(); World && WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		EquippedWeapon = World->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
		EquippedWeapon->Equip(GetMesh(), "WeaponSocket", true, this);
	}
}

void AEnemy::InitializeAttributeSet()
{
	AttributeSet = AbilitySystemComponent->GetSet<UHeroAttributeSet>();
	EnemyAttributeSet = Cast<UBaseEnemyAttributeSet>(AttributeSet);
}

void AEnemy::AttributeChanged(const FOnAttributeChangeData& Data) const
{
	if (HealthBarWidget && AttributeSet->GetHealthAttribute() == Data.Attribute)
	{
		HealthBarWidget->SetHealthPercent(Data.NewValue / AttributeSet->GetMaxHealth());
	}
}

void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();

	SetLifeSpan(DeathLifeSpan);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnSoul();
}

bool AEnemy::InTargetRange(const AActor* Target, const double Radius) const
{
	if (Target)
	{
		const double TargetDistance = (Target->GetActorLocation() - GetActorLocation()).Size();
		return TargetDistance <= Radius;
	}
	return false;
}

void AEnemy::PawnSeen(APawn* Pawn)
{
	if (Pawn->ActorHasTag(C_TAG_HERO))
	{
		UpdateCombatTarget(Pawn);
	}
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (EventInstigator)
	{
		UpdateCombatTarget(EventInstigator->GetPawn());
	}
	return Damage;
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsDead())
	{
		CheckCombatTarget();
	}
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	Super::Destroyed();
}

void AEnemy::CheckCombatTarget()
{
	if (CombatTarget && (IsOutsideCombatRadius() || CombatTarget->ActorHasTag(C_TAG_DEAD)))
	{
		// Outside aggro distance or dead, lose interest
		UpdateCombatTarget(nullptr);
	}
}

AActor* AEnemy::ComputeNewPatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != CurrentPatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	if (ValidTargets.Num() > 0)
	{
		CurrentPatrolTarget = ValidTargets[FMath::RandRange(0, ValidTargets.Num() - 1)];
	}
	else
	{
		CurrentPatrolTarget = nullptr;
	}
	return CurrentPatrolTarget;
}

void AEnemy::SpawnSoul()
{
	if (SoulClass && AttributeSet)
	{
		// Spawn soul above enemy
		if (ASoul* Soul = GetWorld()->SpawnActor<ASoul>(SoulClass, GetActorLocation() + FVector(0.f, 0.f, 125.f), GetActorRotation()))
		{
			Soul->SetOwner(this);
			Soul->SetSouls(AttributeSet->GetSouls());
		}
	}
}

void AEnemy::UpdateHealthBarWidgetVisibility(const bool bVisible) const
{
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(bVisible);
	}
}

void AEnemy::UpdateCombatTarget(AActor* NewTarget)
{
	CombatTarget = NewTarget;
	UpdateHealthBarWidgetVisibility(CombatTarget != nullptr);
	GetCharacterMovement()->MaxWalkSpeed = CombatTarget != nullptr ? ChasingWalkSpeed : PatrollingWalkSpeed;
}

bool AEnemy::IsOutsideCombatRadius() const
{
	return !InTargetRange(CombatTarget, MaxAggroDistance);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead())
	{
		UpdateHealthBarWidgetVisibility(true);
	}
}
