// Copyright BanMing

#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"

#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/AuraGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Item : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Item.Key, Item.Value());
	}

	if (AAuraPlayerState* PS = Cast<AAuraPlayerState>(PlayerState))
	{
		OnPlayerAttributePointsChanged.Broadcast(PS->GetPlayerAttributePoints());
		OnPlayerSpellPointsChanged.Broadcast(PS->GetPlayerSpellPoints());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Item : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Item.Value()).AddLambda([this, Item](const FOnAttributeChangeData& Data) { BroadcastAttributeInfo(Item.Key, Item.Value()); });
	}

	if (AAuraPlayerState* PS = Cast<AAuraPlayerState>(PlayerState))
	{
		PS->OnPlayerAttributePointsChanged.AddLambda([this](int32 NewPoints) { OnPlayerAttributePointsChanged.Broadcast(NewPoints); });
		PS->OnPlayerSpellPointsChanged.AddLambda([this](int32 NewPoints) { OnPlayerSpellPointsChanged.Broadcast(NewPoints); });
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* ASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	ASC->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& GameplayAttribute)
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
