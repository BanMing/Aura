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

	UFUNCTION(BlueprintCallable)
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HomingAccelerationMin = 1600.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float HomingAccelerationMax = 3200.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsHomingProjectiles = true;
};
