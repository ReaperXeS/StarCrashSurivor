// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/RXDamageExecCalculation.h"

// Copyright 2020 Dan Kestranek.

#include "Abilities/CustomGameplayTags.h"
#include "Abilities/RXAbilitySystemComponent.h"
#include "Characters/Abilities/HeroAttributeSet.h"

// Declare the attributes to capture and define how we want to capture them from the Source and Target.
struct RXDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	RXDamageStatics()
	{
		// Snapshot happens at time of GESpec creation

		// We're not capturing anything from the Source in this example, but there could be like AttackPower attributes that you might want.

		// Capture optional Damage set on the damage GE as a CalculationModifier under the ExecutionCalculation
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHeroAttributeSet, Damage, Source, true);

		// Capture the Target's Armor. Don't snapshot.
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHeroAttributeSet, Armor, Target, false);
	}
};

static const RXDamageStatics& DamageStatics()
{
	static RXDamageStatics DStatics;
	return DStatics;
}

URXDamageExecCalculation::URXDamageExecCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void URXDamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	// AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	// AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Armor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, Armor);
	Armor = FMath::Max<float>(Armor, 0.0f);

	float Damage = 0.0f;
	// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	// Add SetByCaller damage if it exists
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(TAG_Data_Damage, false, -1.0f), 0.0f);

	const float UnmitigatedDamage = Damage; // Can multiply any damage boosters here
	const float MitigatedDamage = (UnmitigatedDamage) * (100 / (100 + Armor));

	if (MitigatedDamage > 0.f)
	{
		// Set the Target's damage meta attribute
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, MitigatedDamage));
	}

	// Broadcast damages to Target ASC
	if (URXAbilitySystemComponent* TargetAsc = Cast<URXAbilitySystemComponent>(TargetAbilitySystemComponent))
	{
		URXAbilitySystemComponent* SourceAsc = Cast<URXAbilitySystemComponent>(SourceAbilitySystemComponent);
		TargetAsc->ReceiveDamage(SourceAsc, UnmitigatedDamage, MitigatedDamage);
	}
}
