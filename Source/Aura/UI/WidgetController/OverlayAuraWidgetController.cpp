// Copyright BanMing
#include "Aura/UI/WidgetController/OverlayAuraWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Player/AuraPlayerState.h"

void UOverlayAuraWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());

	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

void UOverlayAuraWidgetController::BindCallbacksToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxHealthChanged.Broadcast(Data.NewValue); });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnManaChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) { OnMaxManaChanged.Broadcast(Data.NewValue); });

	if (GetAuraASC())
	{
		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag Tag : AssetTags)
				{
					// "A.1".MatchesTag("A") will return True, "A".MatchesTag("A.1") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						// const FString Msg = FString::Printf(TEXT("GE Tag %s"), *Tag.ToString());
						// GEngine->AddOnScreenDebugMessage(-1, 8, FColor::Blue, Msg);
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			});
	}

	if (AAuraPlayerState* PS = GetAuraPS())
	{
		PS->OnPlayerXPChanged.AddUObject(this, &ThisClass::OnXPChanged);
		PS->OnPlayerLevelChanged.AddLambda([this](int32 NewLevel) { OnPlayerLevelChanged.Broadcast(NewLevel); });
		PS->OnPlayerAttributePointsChanged.AddLambda([this](int32 NewPoints) { OnPlayerAttributePointsChanged.Broadcast(NewPoints); });
		PS->OnPlayerSpellPointsChanged.AddLambda([this](int32 NewPoints) { OnPlayerSpellPointsChanged.Broadcast(NewPoints); });
	}
	Super::BindCallbacksToDependencies();
}

void UOverlayAuraWidgetController::OnXPChanged(int32 NewXP) const
{
	AAuraPlayerState* PS = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = PS->GetLevelUpInfo();
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo!"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInfos.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInfos[Level].LevelUpRequirement;
		const int32 PrevLevelUpRequirement = LevelUpInfo->LevelUpInfos[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PrevLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PrevLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnPlayerXPPercentChanged.Broadcast(XPBarPercent);
	}
}
