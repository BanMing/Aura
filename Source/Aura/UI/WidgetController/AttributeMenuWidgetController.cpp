// Copyright BanMing

#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"

#include "AttributeSet.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for (auto& Item : GetAuraAttributeSet()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Item.Key, Item.Value());
	}

	if (AAuraPlayerState* PS = GetAuraPS())
	{
		OnPlayerAttributePointsChanged.Broadcast(PS->GetPlayerAttributePoints());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	for (auto& Item : GetAuraAttributeSet()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Item.Value()).AddLambda([this, Item](const FOnAttributeChangeData& Data) { BroadcastAttributeInfo(Item.Key, Item.Value()); });
	}

	if (AAuraPlayerState* PS = GetAuraPS())
	{
		PS->OnPlayerAttributePointsChanged.AddLambda([this](int32 NewPoints) { OnPlayerAttributePointsChanged.Broadcast(NewPoints); });
	}
	Super::BindCallbacksToDependencies();
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& GameplayAttribute)
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
