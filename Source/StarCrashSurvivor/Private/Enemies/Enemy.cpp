// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"

#include "Items/Soul.h"
#include "AIController.h"
#include "Characters/HeroCharacter.h"
#include "Components/AttributesComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Kismet/GameplayStatics.h"
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

void AEnemy::MoveToTarget(const AActor* Target) const
{
	if (AIController && Target)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(60.f);
		AIController->MoveTo(MoveRequest);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(AttributesComponent->GetHealthPercent());
		UpdateHealthBarWidgetVisibility(false);
	}

	GetCharacterMovement()->MaxWalkSpeed = PatrollingWalkSpeed;

	AIController = Cast<AAIController>(GetController());
	MoveToTarget(CurrentPatrolTarget);
	if (PawnSensingComponent)
	{
		PawnSensingComponent->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	}

	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;
		EquippedWeapon = World->SpawnActor<AWeapon>(WeaponClass, SpawnParams);
		EquippedWeapon->Equip(GetMesh(), "WeaponSocket", true, this);
	}
}

void AEnemy::StartAttackTimer()
{
	GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AEnemy::Attack, FMath::RandRange(AttackMinRate, AttackMaxRate));
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimerHandle);
}

bool AEnemy::CanAttack() const
{
	return EnemyState != EEnemyState::EES_Attacking && !IsDead() && EnemyState != EEnemyState::EES_Engaged;
}

void AEnemy::Attack()
{
	Super::Attack();
	EnemyState = EEnemyState::EES_Engaged;

	PlayAnimMontageRandomSection(AttackMontage);
}

void AEnemy::OnAttackEnd()
{
	Super::OnAttackEnd();

	EnemyState = EEnemyState::EES_None;
}

void AEnemy::Die_Implementation()
{
	Super::Die_Implementation();

	SetLifeSpan(DeathLifeSpan);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ClearPatrolTimer();
	ClearAttackTimer();
	SpawnSoul();
}

void AEnemy::UpdateEnemyState(const EEnemyState NewState, AActor* Target)
{
	switch (NewState)
	{
	case EEnemyState::EES_Chasing:
		ClearAttackTimer();
		EnemyState = EEnemyState::EES_Chasing;
		ClearPatrolTimer();
		GetCharacterMovement()->MaxWalkSpeed = ChasingWalkSpeed;
		CombatTarget = Target;
		MoveToTarget(CombatTarget);
		break;
	case EEnemyState::EES_Patrolling:
		ClearAttackTimer();
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = PatrollingWalkSpeed;
		MoveToTarget(CurrentPatrolTarget);
		break;
	case EEnemyState::EES_Attacking:
		EnemyState = EEnemyState::EES_Attacking;
		StartAttackTimer();
		break;
	default:
		break;
	}
}

bool AEnemy::IsCombatTargetDead() const
{
	return CombatTarget && CombatTarget->ActorHasTag(C_TAG_DEAD);
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
	if (EnemyState == EEnemyState::EES_Patrolling && Pawn->ActorHasTag(C_TAG_HERO))
	{
		UpdateEnemyState(EEnemyState::EES_Chasing, Pawn);
	}
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
}

void AEnemy::PatrolTimerFinished() const
{
	MoveToTarget(CurrentPatrolTarget);
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AttributesComponent && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(AttributesComponent->GetHealthPercent());
	}

	if (EventInstigator)
	{
		CombatTarget = EventInstigator->GetPawn();
		CheckCombatTarget();
	}
	return Damage;
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	Super::Destroyed();
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
		return ValidTargets[FMath::RandRange(0, ValidTargets.Num() - 1)];
	}

	return nullptr;
}

void AEnemy::SpawnSoul()
{
	if (SoulClass && AttributesComponent)
	{
		// Spawn soul above enemy
		if (ASoul* Soul = GetWorld()->SpawnActor<ASoul>(SoulClass, GetActorLocation() + FVector(0.f, 0.f, 125.f), GetActorRotation()))
		{
			Soul->SetOwner(this);
			Soul->SetSouls(AttributesComponent->GetSouls());
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

bool AEnemy::IsOutsideCombatRadius() const
{
	return EnemyState != EEnemyState::EES_Patrolling && !InTargetRange(CombatTarget, MaxAggroDistance);
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius() || IsCombatTargetDead())
	{
		// Outside aggro distance, lose interest
		CombatTarget = nullptr;
		UpdateHealthBarWidgetVisibility(false);

		if (EnemyState != EEnemyState::EES_Engaged)
		{
			UpdateEnemyState(EEnemyState::EES_Patrolling, CurrentPatrolTarget);
		}
	}
	else
	{
		const bool InCombatRange = InTargetRange(CombatTarget, AttackDistance);
		if (EnemyState != EEnemyState::EES_Chasing && !InCombatRange)
		{
			// Outside attack distance, chase character
			UpdateEnemyState(EEnemyState::EES_Chasing, CombatTarget);
		}
		else if (InCombatRange && CanAttack())
		{
			// Attack character
			UpdateEnemyState(EEnemyState::EES_Attacking, CombatTarget);
		}
	}
}

void AEnemy::CheckCurrentPatrolTarget()
{
	if (InTargetRange(CurrentPatrolTarget, PatrolRadius))
	{
		CurrentPatrolTarget = ComputeNewPatrolTarget();
		if (CurrentPatrolTarget)
		{
			GetWorldTimerManager().SetTimer(PatrolTimerHandle, this, &AEnemy::PatrolTimerFinished, FMath::RandRange(PatrolWaitTimeMin, PatrolWaitTimeMax));
		}
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDead()) { return; }

	if (EnemyState == EEnemyState::EES_Patrolling)
	{
		CheckCurrentPatrolTarget();
	}
	else
	{
		CheckCombatTarget();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	if (!IsDead())
	{
		UpdateHealthBarWidgetVisibility(true);
	}
	ClearAttackTimer();
	ClearPatrolTimer();

	StopAnimMontage(AttackMontage);
	if (InTargetRange(CombatTarget, AttackDistance) && !IsDead())
	{
		StartAttackTimer();
	}
}
