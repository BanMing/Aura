// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/GameplayAbility_ProjectileSpell.h"
#include "CoreMinimal.h"

#include "GameplayAbility_FireBolt.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_FireBolt : public UGameplayAbility_ProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) const override;
	virtual FString GetNextLevelDescription(int32 Level) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles = 5;
};
