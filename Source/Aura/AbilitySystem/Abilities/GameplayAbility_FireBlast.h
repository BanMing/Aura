// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/GameplayAbility_DamageBase.h"
#include "CoreMinimal.h"

#include "GameplayAbility_FireBlast.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_FireBlast : public UGameplayAbility_DamageBase
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) const override;
	virtual FString GetNextLevelDescription(int32 Level) const override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumBlasts = 12;
};
