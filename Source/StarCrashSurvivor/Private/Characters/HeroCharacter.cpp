// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "Components/AttributesComponent.h"
#include "HUD/HeroOverlay.h"
#include "HUD/StarCrashSurvivorHUD.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Weapons/Weapon.h"

// Sets default values
AHeroCharacter::AHeroCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom);
	GetCharacterMovement()->bOrientRotationToMovement = true;

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	EyeBrows = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrows"));
	EyeBrows->SetupAttachment(GetMesh());
	EyeBrows->AttachmentName = FString("head");
}

void AHeroCharacter::AddSouls(ASoul* Soul)
{
	if (AttributesComponent && Soul)
	{
		AttributesComponent->AddSouls(Soul->GetSouls());
		HeroOverlay->SetSoul(AttributesComponent->GetSouls());
	}
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializeHeroOverlay();
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttributesComponent && HeroOverlay)
	{
		AttributesComponent->RegenStamina(DeltaTime);
		HeroOverlay->SetStaminaBarPercent(AttributesComponent->GetStaminaPercent());
	}
}

// Called to bind functionality to input
void AHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHeroCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AHeroCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &AHeroCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("ZoomInCamera", IE_Pressed, this, &AHeroCharacter::ZoomInCamera);
	PlayerInputComponent->BindAction("ZoomOutCamera", IE_Pressed, this, &AHeroCharacter::ZoomOutCamera);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AHeroCharacter::EKeyPressed);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AHeroCharacter::Attack);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AHeroCharacter::Dodge);

	Tags.Add(C_TAG_HERO);
}

void AHeroCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	if (!IsDead())
	{
		ActionState = EActionState::EAS_HitReaction;
	}
}

void AHeroCharacter::Jump()
{
	if (ActionState == EActionState::EAS_Idle)
	{
		Super::Jump();
	}
}

float AHeroCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const auto ReturnValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AttributesComponent && HeroOverlay)
	{
		HeroOverlay->SetHealthBarPercent(AttributesComponent->GetHealthPercent());
	}

	return ReturnValue;
}

void AHeroCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AHeroCharacter::AddGold(ATreasure* Treasure)
{
	if (AttributesComponent && Treasure)
	{
		AttributesComponent->AddGold(Treasure->GetGold());
		HeroOverlay->SetGold(AttributesComponent->GetGold());
	}
}

bool AHeroCharacter::CanAttack() const
{
	return ActionState == EActionState::EAS_Idle && CharacterState != ECharacterState::ECS_UnEquipped && EquippedWeapon;
}

void AHeroCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAnimMontageRandomSection(AttackMontage);
		ActionState = EActionState::EAS_Attacking;
	}
}

bool AHeroCharacter::CanDodge() const
{
	return ActionState == EActionState::EAS_Idle && AttributesComponent->HasEnoughStamina(AttributesComponent->GetDodgeCost());
}

void AHeroCharacter::Dodge()
{
	if (CanDodge())
	{
		PlayAnimMontage(DodgeMontage, 1, FName("Default"));
		ActionState = EActionState::EAS_Dodge;
		if (AttributesComponent && HeroOverlay)
		{
			AttributesComponent->UseStamina(AttributesComponent->GetDodgeCost());
			HeroOverlay->SetStaminaBarPercent(AttributesComponent->GetStaminaPercent());
		}
	}
}

void AHeroCharacter::OnAttackEnd()
{
	ActionState = EActionState::EAS_Idle;
}

void AHeroCharacter::OnDodgeEnd()
{
	Super::OnDodgeEnd();

	ActionState = EActionState::EAS_Idle;
}

void AHeroCharacter::MoveForward(const float AxisValue)
{
	if (ActionState == EActionState::EAS_Idle && Controller && AxisValue != 0)
	{
		// Move Forward where we look
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, AxisValue);
	}
}

void AHeroCharacter::MoveRight(const float AxisValue)
{
	if (ActionState == EActionState::EAS_Idle && Controller && AxisValue != 0)
	{
		// Move Right from direction we are looking
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, AxisValue);
	}
}

void AHeroCharacter::LookUp(const float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void AHeroCharacter::Turn(const float AxisValue) { AddControllerYawInput(AxisValue); }

void AHeroCharacter::EKeyPressed()
{
	if (AWeapon* Weapon = Cast<AWeapon>(OverlappingItem); Weapon && CanPickupWeapon())
	{
		// TODO: Drop current weapon

		// Attach new weapon
		Weapon->Equip(GetMesh(), "Socket_RightHand", true, this);
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
		EquippedWeapon = Weapon;
		OverlappingItem = nullptr;
	}
	else if (CanHideWeapon())
	{
		PlayAnimMontage(EquipMontage, 1, "UnEquip");
		ActionState = EActionState::EAS_Equipping;
		CharacterState = ECharacterState::ECS_UnEquipped;
	}
	else if (CanShowWeapon())
	{
		// Attach new weapon
		PlayAnimMontage(EquipMontage, 1, "Equip");
		ActionState = EActionState::EAS_Equipping;
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHeroCharacter::ZoomInCamera()
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - 20.f, 100.f, 500.f);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AHeroCharacter::ZoomOutCamera()
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + 20.f, 100.f, 500.f);
}

bool AHeroCharacter::CanShowWeapon() const
{
	return ActionState == EActionState::EAS_Idle && CharacterState == ECharacterState::ECS_UnEquipped && EquippedWeapon;
}

bool AHeroCharacter::CanHideWeapon() const
{
	return ActionState == EActionState::EAS_Idle && CharacterState != ECharacterState::ECS_UnEquipped && EquippedWeapon;
}

bool AHeroCharacter::CanPickupWeapon() const
{
	return ActionState == EActionState::EAS_Idle && CharacterState == ECharacterState::ECS_UnEquipped;
}

void AHeroCharacter::InitializeHeroOverlay()
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (const AStarCrashSurvivorHUD* ScsHUD = Cast<AStarCrashSurvivorHUD>(PlayerController->GetHUD()))
		{
			HeroOverlay = ScsHUD->GetHeroOverlay();
			if (HeroOverlay)
			{
				HeroOverlay->SetHealthBarPercent(1.f);
				HeroOverlay->SetStaminaBarPercent(1.f);
				HeroOverlay->SetGold(0);
				HeroOverlay->SetSoul(0);
			}
		}
	}
}

void AHeroCharacter::OnEquipEnd()
{
	ActionState = EActionState::EAS_Idle;
}

void AHeroCharacter::OnHideWeaponAttachToSocket()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), "BackWeaponHolder", false, this);
	}
}

void AHeroCharacter::OnHitReactEnd()
{
	ActionState = EActionState::EAS_Idle;
}

void AHeroCharacter::OnShowWeaponAttachToSocket()
{
	// Attach new weapon
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), "Socket_RightHand", false, this);
	}
}
