// Copyright BanMing

#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"

#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Item : AuraAttributeSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(Item.Key, Item.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	for (auto& Item : AuraAttributeSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Item.Value()).AddLambda([this, Item](const FOnAttributeChangeData& Data) { BroadcastAttributeInfo(Item.Key, Item.Value()); });
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& GameplayAttribute)
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
