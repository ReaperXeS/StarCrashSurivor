#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	ECS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHanded UMETA(DisplayName = "Equipped One Handed"),
	ECS_EquippedTwoHanded UMETA(DisplayName = "Equipped Two Handed"),
};

UENUM(BlueprintType)
enum class EActionState : uint8 {
	EAS_Idle UMETA(DisplayName = "Idle"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
};
