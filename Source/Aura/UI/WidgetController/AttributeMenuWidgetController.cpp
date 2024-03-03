// Copyright BanMing

#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	//AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)
		->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag Tag : AssetTags)
				{
					// "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
					FGameplayTag AttributesTag = FGameplayTag::RequestGameplayTag(FName("Attributes"));
					if (Tag.MatchesTag(AttributesTag))
					{
						// const FString Msg = FString::Printf(TEXT("GE Tag %s"), *Tag.ToString());
						// GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Blue, Msg);
						FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
						//Info.AttributeValue=
					}
				}
			});
}
