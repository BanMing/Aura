// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"

#include "AsyncTask_WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

/**
 *
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "Async Task"))
class AURA_API UAsyncTask_WaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncTask_WaitCooldownChange* AsyncTask_WaitCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEhandle);

public:
	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature OnCooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownChangeSignature OnCooldownEnd;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;
};
