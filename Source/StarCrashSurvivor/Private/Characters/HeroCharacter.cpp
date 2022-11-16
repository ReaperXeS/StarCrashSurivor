// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HeroCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
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

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
}

void AHeroCharacter::MoveForward(const float AxisValue)
{
	if (Controller && AxisValue != 0)
	{
		// Move Forward where we look
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(Direction, AxisValue);
	}
}

void AHeroCharacter::MoveRight(const float AxisValue)
{
	if (Controller && AxisValue != 0)
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

void AHeroCharacter::Turn(const float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void AHeroCharacter::Attack()
{
	if (ActionState == EActionState::EAS_Idle && CharacterState == ECharacterState::ECS_EquippedOneHanded && GetMesh() && GetMesh()->GetAnimInstance() && AttackMontage)
	{
		ActionState = EActionState::EAS_Attacking;
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
		const FName SectionName = FName("Attack" + FString::FromInt(FMath::RandRange(1, 2)));
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AHeroCharacter::EKeyPressed()
{
	if (AWeapon* Weapon = Cast<AWeapon>(OverlappingItem))
	{
		Weapon->Equip(GetMesh(), "Socket_RightHand");
		CharacterState = ECharacterState::ECS_EquippedOneHanded;
	}
}

void AHeroCharacter::ZoomInCamera()
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - 20.f, 100.f, 500.f);
}

void AHeroCharacter::ZoomOutCamera()
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + 20.f, 100.f, 500.f);
}

void AHeroCharacter::OnAttackEnd()
{
	ActionState = EActionState::EAS_Idle;
}

