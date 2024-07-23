// Copyright BanMing

#pragma once

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "GameplayAbility_PassiveBase.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_PassiveBase : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void ReceiveDeactivate(const FGameplayTag& AbilityTag);
};
