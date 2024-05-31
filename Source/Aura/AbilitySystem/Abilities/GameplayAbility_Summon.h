// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "CoreMinimal.h"

#include "GameplayAbility_Summon.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_Summon : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Summon")
	TArray<FVector> GetSpawnLocations();

	UFUNCTION(BlueprintPure, Category = "Summon")
	TSubclassOf<APawn> GetRandomMinionClass();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MaxSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float SpawnSpread = 90.f;
};
