// Copyright BanMing

#pragma once

#include "Aura/AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "CoreMinimal.h"

#include "GameplayAbility_ProjectileSpell.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UGameplayAbility_ProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	/** Actually activate ability, do not call this directly */
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnProjetile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AAuraProjectile> ProjectileClass;
};
