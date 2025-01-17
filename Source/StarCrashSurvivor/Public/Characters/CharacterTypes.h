﻿#pragma once

inline FName C_TAG_DEAD = FName("DEAD");
inline FName C_TAG_ENEMY = FName("Enemy");
inline FName C_TAG_HERO = FName("HERO");

UENUM(BlueprintType)
enum EDeathState
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
	EES_None UMETA(DisplayName = "None"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
};
