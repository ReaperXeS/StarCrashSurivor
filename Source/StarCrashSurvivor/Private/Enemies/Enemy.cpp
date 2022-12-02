// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"

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

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>("HealthBarWidget");
	HealthBarWidget->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComponent");
	PawnSensingComponent->SetPeripheralVisionAngle(45.f);
	PawnSensingComponent->SightRadius = 4000.f;
}

void AEnemy::MoveToTarget(const AActor* Target) const
{
	if (AIController && Target)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(15.f);
		AIController->MoveTo(MoveRequest);
	}
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(AttributesComponent->GetHealthPercent());
		HealthBarWidget->SetVisibility(false);
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
		EquippedWeapon->Equip(GetMesh(), "RightHandSocket", true, this);
	}
}

void AEnemy::Die()
{
	// Already in death state 
	if (DeathState != EDeathState::EDS_Alive) { return; }

	// Compute Death State
	FName Section;
	switch (FMath::RandRange(1, 6))
	{
	case 1:
		DeathState = EDeathState::EDS_Death1;
		Section = "Death1";
		break;
	case 2:
		DeathState = EDeathState::EDS_Death2;
		Section = "Death2";
		break;
	case 3:
		DeathState = EDeathState::EDS_Death3;
		Section = "Death3";
		break;
	case 4:
		DeathState = EDeathState::EDS_Death4;
		Section = "Death4";
		break;
	case 5:
		DeathState = EDeathState::EDS_Death5;
		Section = "Death5";
		break;
	case 6:
		DeathState = EDeathState::EDS_Death6;
		Section = "Death6";
		break;
	default:
		DeathState = EDeathState::EDS_Death1;
		Section = "Death1";
		break;
	}
	PlayAnimMontage(DeathMontage, 1.0f, Section);
	SetLifeSpan(DeathLifeSpan);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::UpdateEnemyState(const EEnemyState NewState, AActor* Target)
{
	switch (NewState)
	{
	case EEnemyState::EES_Chasing:
		EnemyState = EEnemyState::EES_Chasing;
		GetWorldTimerManager().ClearTimer(PatrolTimerHandle);
		GetCharacterMovement()->MaxWalkSpeed = ChasingWalkSpeed;
		CombatTarget = Target;
		MoveToTarget(CombatTarget);
		break;
	case EEnemyState::EES_Patrolling:
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = PatrollingWalkSpeed;
		MoveToTarget(CurrentPatrolTarget);
		break;
	case EEnemyState::EES_Attacking:
		EnemyState = EEnemyState::EES_Attacking;
	// PlayAnimMontage(AttackMontage);
		break;
	default:
		break;
	}
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
	if (EnemyState == EEnemyState::EES_Patrolling && Pawn->ActorHasTag(FName("HeroCharacter")))
	{
		UpdateEnemyState(EEnemyState::EES_Chasing, Pawn);
		UE_LOG(LogTemp, Warning, TEXT("Chasing on Seen"));
	}
}

void AEnemy::PatrolTimerFinished() const
{
	MoveToTarget(CurrentPatrolTarget);
}

float AEnemy::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AttributesComponent)
	{
		AttributesComponent->ReceiveDamage(Damage);
		HealthBarWidget->SetVisibility(true);
		if (EventInstigator)
		{
			UpdateEnemyState(EEnemyState::EES_Chasing, EventInstigator->GetPawn());
		}
	}

	if (HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(AttributesComponent->GetHealthPercent());
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

void AEnemy::CheckCombatTarget()
{
	if (EnemyState != EEnemyState::EES_Patrolling && !InTargetRange(CombatTarget, MaxAggroDistance))
	{
		// Outside aggro distance, lose interest
		CombatTarget = nullptr;
		if (HealthBarWidget)
		{
			HealthBarWidget->SetVisibility(false);
		}

		UpdateEnemyState(EEnemyState::EES_Patrolling, CurrentPatrolTarget);
		UE_LOG(LogTemp, Warning, TEXT("Patrolling"));
	}
	else
	{
		const bool InCombatRange = InTargetRange(CombatTarget, AttackDistance);
		if (EnemyState != EEnemyState::EES_Chasing && !InCombatRange)
		{
			// Outside attack distance, chase character
			UpdateEnemyState(EEnemyState::EES_Chasing, CombatTarget);
			UE_LOG(LogTemp, Warning, TEXT("Chasing"));
		}
		else if (InCombatRange && EnemyState != EEnemyState::EES_Attacking)
		{
			// Attack character
			UpdateEnemyState(EEnemyState::EES_Attacking, CombatTarget);
			// PlayAnimMontage(AttackMontage);
			UE_LOG(LogTemp, Warning, TEXT("Attack"));
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

	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckCurrentPatrolTarget();
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (AttributesComponent && AttributesComponent->IsAlive())
	{
		DirectionalHitReact(ImpactPoint);
		// Aggro on player
	}
	else
	{
		Die();
	}


	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}

	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, ImpactPoint);
	}
}
