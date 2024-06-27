// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "AbilityInfo.generated.h"

class UGameplayAbility;
USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	FGameplayTag StatusTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Ability Info")
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, Category = "Ability Info")
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	int32 LevelRequirement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	TSubclassOf<UGameplayAbility> Ability;
};

/**
 *
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	FAuraAbilityInfo FindAbilityInfoByTag(const FGameplayTag AbilityTag, bool bLogNotFound = false) const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
	TArray<FAuraAbilityInfo> AbilityInfos;
};
