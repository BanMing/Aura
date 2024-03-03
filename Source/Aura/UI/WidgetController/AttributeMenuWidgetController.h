// Copyright BanMing

#pragma once

#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/UI/WidgetController/AuraWidgetController.h"
#include "CoreMinimal.h"

#include "AttributeMenuWidgetController.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAttributeInfo> AttributeInfo;
};
