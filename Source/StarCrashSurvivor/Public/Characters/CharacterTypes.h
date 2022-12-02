#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "Unequipped"),
	ECS_EquippedOneHanded UMETA(DisplayName = "Equipped One Handed"),
	ECS_EquippedTwoHanded UMETA(DisplayName = "Equipped Two Handed"),
};

UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Idle UMETA(DisplayName = "Idle"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_Equipping UMETA(DisplayName = "Equipping"),
};

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	EDS_Death1 UMETA(DisplayName = "Death 1"),
	EDS_Death2 UMETA(DisplayName = "Death 2"),
	EDS_Death3 UMETA(DisplayName = "Death 3"),
	EDS_Death4 UMETA(DisplayName = "Death 4"),
	EDS_Death5 UMETA(DisplayName = "Death 5"),
	EDS_Death6 UMETA(DisplayName = "Death 6"),
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
};
