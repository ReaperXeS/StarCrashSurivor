// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "HeroAttributeSet.generated.h"

/**
 * This macro defines a set of helper functions for accessing and initializing attributes.
 *
 * The following example of the macro:
 *		ATTRIBUTE_ACCESSORS(ULyraHealthSet, Health)
 * will create the following functions:
 *		static FGameplayAttribute GetHealthAttribute();
 *		float GetHealth() const;
 *		void SetHealth(float NewVal);
 *		void InitHealth(float NewVal);
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class STARCRASHSURVIVOR_API UHeroAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	// AttributeSet Overrides
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	/** Health Attribute, publicly accessible */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;

	/** Health Attribute, publicly accessible */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Souls;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Gold;

	void ShowDebugOfAttribute(FGameplayAttribute Attribute) const;

public:
	virtual void ShowDebug() const;

	// Damage is a meta attribute used by the DamageExecution to calculate final damage, which then turns into -Health
	// Temporary value that only exists on the Server. Not replicated.
	UPROPERTY(BlueprintReadOnly, Category = "Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Damage)

	// Armor reduces the amount of damage done by attackers
	UPROPERTY(BlueprintReadOnly, Category = "Armor")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Armor)

	//~ Helper functions for "Health" attributes
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Health);

	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, MaxHealth);

	// Stamina
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Stamina);

	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, MaxStamina);

	// Souls
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Souls);

	// Gold
	ATTRIBUTE_ACCESSORS(UHeroAttributeSet, Gold);
};
