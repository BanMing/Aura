// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/GameplayAbility_DamageBase.h"
#include "CoreMinimal.h"

#include "GameplayAbility_FireBlast.generated.h"
class AAuraFireBall;
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

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumBlasts = 12;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
