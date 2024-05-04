// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "CoreMinimal.h"

#include "AuraDamageGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damge")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
