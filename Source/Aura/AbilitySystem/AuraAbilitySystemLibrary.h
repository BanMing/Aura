// Copyright BanMing

#pragma once

#include "Aura/UI/WidgetController/OverlayAuraWidgetController.h"
#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemLibrary.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|WigetController")
	static UOverlayAuraWidgetController* GetOverlayAuraWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|WigetController")
	static UAttributeMenuWidgetController* GetAttributeMenuController(const UObject* WorldContextObject);
};
