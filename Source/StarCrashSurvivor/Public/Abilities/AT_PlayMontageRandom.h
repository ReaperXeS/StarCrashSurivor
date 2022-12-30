// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "AT_PlayMontageRandom.generated.h"

/**
 * Play Montage and wait for event to finish, play a random section from the montage. Start Section is ignored.
 */
UCLASS()
class STARCRASHSURVIVOR_API UAT_PlayMontageRandom : public UAbilityTask_PlayMontageAndWait
{
	GENERATED_BODY()

	/** 
	 * Start playing an animation montage on the avatar actor and wait for it to finish. Will play a random section from the montage
	 *
	 * @param OwningAbility
	 * @param TaskInstanceName Set to override the name of this task, for later querying
	 * @param MontageToPlay The montage to play on the character
	 * @param Rate Change to play the montage faster or slower
	 * @param bStopWhenAbilityEnds If true, this montage will be aborted if the ability ends normally. It is always stopped when the ability is explicitly cancelled
	 * @param AnimRootMotionTranslationScale Change to modify size of root motion or set to 0 to block it entirely
	 * @param StartTimeSeconds Starting time offset in montage, this will be overridden by StartSection if that is also set
	 */
	UFUNCTION(BlueprintCallable, Category="Ability|Tasks", meta = (DisplayName="PlayMontageAndWaitRandomSection",
		HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_PlayMontageRandom* CreatePlayMontageRandom(UGameplayAbility* OwningAbility,
	                                                      FName TaskInstanceName, UAnimMontage* MontageToPlay, float Rate = 1.f, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f);
};
