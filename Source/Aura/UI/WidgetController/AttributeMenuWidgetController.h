// Copyright BanMing

#pragma once

#include "Aura/UI/WidgetController/AuraWidgetController.h"
#include "CoreMinimal.h"

#include "AttributeMenuWidgetController.generated.h"

struct FAuraAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

private:
	void BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& GameplayAttribute);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<class UAttributeInfo> AttributeInfo;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FOnPlayerStatChangeSignature OnPlayerAttributePointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FOnPlayerStatChangeSignature OnPlayerSpellPointsChanged;
};
