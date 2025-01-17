// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GroomComponent.h"
#include "HUD/HeroOverlay.h"
#include "HUD/StarCrashSurvivorHUD.h"
#include "Items/Soul.h"
#include "Items/Treasure.h"
#include "Items/Weapons/Weapon.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/CustomGameplayTags.h"
#include "GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Characters/Abilities/HeroAttributeSet.h"
#include "Characters/Abilities/BaseGameplayAbility.h"
#include "Items/Shield.h"

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

	// AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

// Called when the game starts or when spawned
void AHeroCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (const APlayerController* PC = Cast<APlayerController>(GetController()); PC && HeroMappingContext)
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			InputSystem->AddMappingContext(HeroMappingContext, 0);
		}
	}

	check(AbilitySystemComponent);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AHeroCharacter::AttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AHeroCharacter::AttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetSoulsAttribute()).AddUObject(this, &AHeroCharacter::AttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetGoldAttribute()).AddUObject(this, &AHeroCharacter::AttributeChanged);
	InitializeHeroOverlay();
}

void AHeroCharacter::AttributeChanged(const FOnAttributeChangeData& Data)
{
	if (HeroOverlay)
	{
		if (AttributeSet->GetStaminaAttribute() == Data.Attribute)
		{
			HeroOverlay->SetStaminaBarPercent(Data.NewValue / AttributeSet->GetMaxStamina());
		}
		else if (AttributeSet->GetHealthAttribute() == Data.Attribute)
		{
			HeroOverlay->SetHealthBarPercent(Data.NewValue / AttributeSet->GetMaxHealth());
		}
		else if (AttributeSet->GetSoulsAttribute() == Data.Attribute)
		{
			HeroOverlay->SetSoul(Data.NewValue);
		}
		else if (AttributeSet->GetGoldAttribute() == Data.Attribute)
		{
			HeroOverlay->SetGold(Data.NewValue);
		}
	}
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

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	Input->BindAction(ActionMove, ETriggerEvent::Triggered, this, &AHeroCharacter::MoveCharacter);
	Input->BindAction(ActionLookAround, ETriggerEvent::Triggered, this, &AHeroCharacter::LookAround);
	Input->BindAction(ActionZoomInOutCamera, ETriggerEvent::Triggered, this, &AHeroCharacter::ZoomCamera);

	if (StartupAbilities.Num() > 0)
	{
		for (TSubclassOf<UBaseGameplayAbility> StartupAbility : StartupAbilities)
		{
			if (StartupAbility.GetDefaultObject())
			{
				AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, StartupAbility.GetDefaultObject()->GetInputId()));
				Input->BindAction(StartupAbility.GetDefaultObject()->GetInputAction(), ETriggerEvent::Started, this, &AHeroCharacter::ActionInputWithAbilityPressed);
				Input->BindAction(StartupAbility.GetDefaultObject()->GetInputAction(), ETriggerEvent::Completed, this, &AHeroCharacter::ActionInputWithAbilityReleased);
			}
		}
	}

	Tags.Add(C_TAG_HERO);
}

void AHeroCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	if (!IsDead())
	{
		FGameplayTagContainer TagContainer;
		GetOwnedGameplayTags(TagContainer);
		TagContainer.AddTag(TAG_ActionState_HitReaction);
	}
}

float AHeroCharacter::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ReturnValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	return ReturnValue;
}

void AHeroCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AHeroCharacter::AddGold(ATreasure* Treasure)
{
	if (Treasure && Treasure->PickupGameplayEffect)
	{
		UGameplayEffect* Effect = Treasure->PickupGameplayEffect->GetDefaultObject<UGameplayEffect>();
		check(Effect->Modifiers.Num() > 0);
		Effect->Modifiers[0].ModifierMagnitude = FScalableFloat(Treasure->GetGold());
		AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect, 1, AbilitySystemComponent->MakeEffectContext());
	}
}

void AHeroCharacter::AddSouls(ASoul* Soul)
{
	if (Soul && Soul->PickupGameplayEffect)
	{
		UGameplayEffect* Effect = Soul->PickupGameplayEffect->GetDefaultObject<UGameplayEffect>();
		check(Effect->Modifiers.Num() > 0);
		Effect->Modifiers[0].ModifierMagnitude = FScalableFloat(Soul->GetSouls());
		AbilitySystemComponent->ApplyGameplayEffectToSelf(Effect, 1, AbilitySystemComponent->MakeEffectContext());
	}
}

void AHeroCharacter::ActionInputWithAbilityPressed(const FInputActionInstance& InputActionInstance)
{
	if (StartupAbilities.Num() > 0)
	{
		for (TSubclassOf<UBaseGameplayAbility> Ability : StartupAbilities)
		{
			if (InputActionInstance.GetSourceAction() == Ability.GetDefaultObject()->GetInputAction())
			{
				AbilitySystemComponent->AbilityLocalInputPressed(Ability.GetDefaultObject()->GetInputId());
				break;
			}
		}
	}
}

void AHeroCharacter::ActionInputWithAbilityReleased(const FInputActionInstance& InputActionInstance)
{
	if (StartupAbilities.Num() > 0)
	{
		for (TSubclassOf<UBaseGameplayAbility> Ability : StartupAbilities)
		{
			if (InputActionInstance.GetSourceAction() == Ability.GetDefaultObject()->GetInputAction())
			{
				AbilitySystemComponent->AbilityLocalInputReleased(Ability.GetDefaultObject()->GetInputId());
				break;
			}
		}
	}
}

TObjectPtr<AWeapon> AHeroCharacter::GetOverlappingWeapon() const
{
	return Cast<AWeapon>(OverlappingItem);
}

void AHeroCharacter::LookAround(const FInputActionValue& ActionValue)
{
	// Lookup
	AddControllerPitchInput(ActionValue.Get<FVector2d>().Y);
	AddControllerYawInput(ActionValue.Get<FVector2d>().X);
}

void AHeroCharacter::MoveCharacter(const FInputActionValue& ActionValue)
{
	if (Controller)
	{
		MoveForward(ActionValue.Get<FVector2d>().Y);
		MoveRight(ActionValue.Get<FVector2d>().X);
	}
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

void AHeroCharacter::ZoomCamera(const FInputActionValue& ActionValue)
{
	CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength + ActionValue.Get<float>() * 20.f, 100.f, 500.f);
	// CameraBoom->TargetArmLength = FMath::Clamp(CameraBoom->TargetArmLength - 20.f, 100.f, 500.f);
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

void AHeroCharacter::OnHideWeaponAttachToSocket()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), "BackWeaponHolder", false, this);
	}
}

void AHeroCharacter::OnShowWeaponAttachToSocket()
{
	// Attach new weapon
	if (EquippedWeapon)
	{
		EquippedWeapon->Equip(GetMesh(), "Socket_RightHand", false, this);
	}
}

void AHeroCharacter::EquipItem(AItem* Item)
{
	if (AWeapon* Weapon = Cast<AWeapon>(Item))
	{
		// Attach new weapon
		Weapon->Equip(GetMesh(), Weapon->GetSocketName(), true, this);
		EquippedWeapon = Weapon;
		OverlappingItem = nullptr;

		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AddLooseGameplayTag(TAG_Weapon_OneHand);
		}
	}
	else if (AShield* Shield = Cast<AShield>(Item))
	{
		// Attach new weapon
		Shield->Equip(GetMesh(), Shield->GetSocketName(), true, this);
		EquippedShield = Shield;
		OverlappingItem = nullptr;
		if (GetAbilitySystemComponent())
		{
			GetAbilitySystemComponent()->AddLooseGameplayTag(TAG_Weapon_Shield);
		}
	}
}
