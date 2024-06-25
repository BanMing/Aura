// Copyright BanMing

#include "Aura/UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAttributeSet.h"
#include "Aura/Player/AuraPlayerController.h"
#include "Aura/Player/AuraPlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
	if (GetAuraASC())
	{
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(GetAuraASC());
		}
		else
		{
			GetAuraASC()->OnAbilitiesGiven.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
		}
	}
}

void UAuraWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* ASC)
{
	if (!ASC->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda(
		[this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(UAuraAbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = UAuraAbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
			Info.CooldownTag = UAuraAbilitySystemComponent::GetCooldownTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
	ASC->ForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPC == nullptr)
	{
		AuraPC = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPC;
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPS == nullptr)
	{
		AuraPS = Cast<AAuraPlayerState>(PlayerState);
	}

	return AuraPS;
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraASC == nullptr)
	{
		AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AuraASC;
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAttributeSet()
{
	if (AuraAttributeSet == nullptr)
	{
		AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	}
	return AuraAttributeSet;
}
