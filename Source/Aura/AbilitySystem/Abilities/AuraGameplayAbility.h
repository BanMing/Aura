// Copyright BanMing

#pragma once

#include "Abilities/GameplayAbility.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "AuraGameplayAbility.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) const;
	virtual FString GetNextLevelDescription(int32 Level) const;
	static FString GetLockedDescription(int32 Level);

protected:
	float GetManaCost(int32 Level) const;
	float GetCooldown(int32 Level) const;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
