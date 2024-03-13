// Copyright BanMing

#pragma once

#include "Abilities/Tasks/AbilityTask.h"
#include "CoreMinimal.h"

#include "AbilityTask_TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FVector&, Data);

/**
 *
 */
UCLASS()
class AURA_API UAbilityTask_TargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_TargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidDateEvent;

protected:
	virtual void Activate() override;
};
