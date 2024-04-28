// Copyright BanMing

#pragma once

#include "AbilitySystemGlobals.h"
#include "CoreMinimal.h"

#include "AuraAbilitySystemGlobals.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
public:
	/** Should allocate a project specific GameplayEffectContext struct. Caller is responsible for deallocation */
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
