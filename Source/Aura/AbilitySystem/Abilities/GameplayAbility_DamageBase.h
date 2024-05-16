// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "CoreMinimal.h"
#include "Interaction/CombatInterface.h"

#include "GameplayAbility_DamageBase.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_DamageBase : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:
	UFUNCTION(BlueprintPure)
	const FTaggedMontage GetRandomTaggedMontage() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
