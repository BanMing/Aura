// Copyright BanMing

#pragma once

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"

#include "DamageTextComponent.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage, bool bIsBlockedHit, bool bIsCriticalHit);
};
