// Copyright BanMing

#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	OnInitializeStartupAbilities(GetAuraASC());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	// Super::BindCallbacksToDependencies();
	GetAuraASC()->OnAbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) {
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});
}