// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/BirdPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ABirdPawn::ABirdPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArmComponent);
	SpringArmComponent->TargetArmLength = 500.f;	
}

// Called when the game starts or when spawned
void ABirdPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABirdPawn::MoveForward(float AxisValue)
{
	
	// AddMovementInput(GetActorForwardVector(), AxisValue);
	if (Controller && (AxisValue != 0.f))
	{
		UE_LOG(LogTemp, Warning, TEXT("MoveForward: %f"), AxisValue);
		AddMovementInput(GetActorForwardVector(), AxisValue);
	}
}

void ABirdPawn::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void ABirdPawn::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void ABirdPawn::Turn(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

// Called every frame
void ABirdPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABirdPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABirdPawn::MoveForward);
	PlayerInputComponent->BindAxis("LookUp", this, &ABirdPawn::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &ABirdPawn::Turn);
}

