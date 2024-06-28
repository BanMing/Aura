// Copyright BanMing

#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	OnInitializeStartupAbilities(GetAuraASC());
	if (AAuraPlayerState* PS = GetAuraPS())
	{
		OnPlayerSpellPointsChanged.Broadcast(PS->GetPlayerSpellPoints());
	}
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->OnAbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
		{
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	if (AAuraPlayerState* PS = GetAuraPS())
	{
		PS->OnPlayerSpellPointsChanged.AddLambda([this](int32 NewPoints) { OnPlayerSpellPointsChanged.Broadcast(NewPoints); });
	}
}