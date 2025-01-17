// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Abilities/HeroAttributeSet.h"
#include "GameplayEffectExtension.h"

void UHeroAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		const float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone > 0.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("Damage done: %f"), LocalDamageDone);
			// Apply the health change and then clamp it
			const float NewHealth = GetHealth() - LocalDamageDone;
			SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		}
	}
}

void UHeroAttributeSet::ShowDebugOfAttribute(const FGameplayAttribute Attribute) const
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::White, FString::Printf(TEXT("%s: %f"), *Attribute.AttributeName, Attribute.GetNumericValue(this)));
	}
}

void UHeroAttributeSet::ShowDebug() const
{
	if (GEngine)
	{
		ShowDebugOfAttribute(GetGoldAttribute());
		ShowDebugOfAttribute(GetHealthAttribute());
		ShowDebugOfAttribute(GetMaxHealthAttribute());
		ShowDebugOfAttribute(GetMaxStaminaAttribute());
		ShowDebugOfAttribute(GetSoulsAttribute());
		ShowDebugOfAttribute(GetStaminaAttribute());
	}
}
