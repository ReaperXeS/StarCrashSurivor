#pragma once

UENUM(BlueprintType)
enum class EGameplayAbilityInput : uint8
{
	EGAI_None UMETA(DisplayName = "None"),
	EGAI_Attack UMETA(DisplayName = "Attack"),
	EGAI_Dodge UMETA(DisplayName = "Dodge"),
	EGAI_Interact UMETA(DisplayName = "Interact"),
	EGAI_Jump UMETA(DisplayName = "Jump"),
	EGAI_ShowHideWeapon UMETA(DisplayName = "ShowHideWeapon"),
	EGAI_AttackStrong UMETA(DisplayName = "AttackStrong"),
	EGAI_Block UMETA(DisplayName = "Block"),
	EGAI_BlockEnd UMETA(DisplayName = "BlockEnd"),
};
