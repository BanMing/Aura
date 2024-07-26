// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/GameplayAbility_BeamSpell.h"
#include "CoreMinimal.h"

#include "GameplayAbility_Electrocutes.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_Electrocutes : public UGameplayAbility_BeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) const override;
	virtual FString GetNextLevelDescription(int32 Level) const override;
};
