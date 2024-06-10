// Copyright BanMing

#include "AbilitySystem/AsyncTask/AsyncTask_WaitCooldownChange.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"

UAsyncTask_WaitCooldownChange* UAsyncTask_WaitCooldownChange::AsyncTask_WaitCooldownChange(UAbilitySystemComponent* InASC, const FGameplayTag& InCooldownTag)
{
	if (!IsValid(InASC) || !InCooldownTag.IsValid())
	{
		return nullptr;
	}

	UAsyncTask_WaitCooldownChange* WaitCooldownChange = NewObject<UAsyncTask_WaitCooldownChange>();
	WaitCooldownChange->ASC = InASC;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	InASC->RegisterGameplayTagEvent(InCooldownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange, &UAsyncTask_WaitCooldownChange::CooldownTagChanged);

	InASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UAsyncTask_WaitCooldownChange::OnActiveEffectAdded);
	return WaitCooldownChange;
}

void UAsyncTask_WaitCooldownChange::EndTask()
{
	if (IsValid(ASC) && CooldownTag.IsValid())
	{
		ASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAsyncTask_WaitCooldownChange::CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	if (NewCount == 0)
	{
		OnCooldownEnd.Broadcast(0.f);
	}
}

void UAsyncTask_WaitCooldownChange::OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpec, FActiveGameplayEffectHandle GEhandle)
{
	FGameplayTagContainer AssetTagsContainer;
	GESpec.GetAllAssetTags(AssetTagsContainer);

	FGameplayTagContainer GrantTagsContainer;
	GESpec.GetAllGrantedTags(GrantTagsContainer);

	if (AssetTagsContainer.HasTagExact(CooldownTag) || GrantTagsContainer.HasTagExact(CooldownTag))
	{
		FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAllOwningTags(CooldownTag.GetSingleTagContainer());
		TArray<float> TimeRemainings = TargetASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (TimeRemainings.Num() > 0)
		{
			TimeRemainings.Sort();
			OnCooldownStart.Broadcast(TimeRemainings.Last());
		}
		else
		{
			OnCooldownStart.Broadcast(0);
		}
	}
}
