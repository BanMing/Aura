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
		CurrentSpellPoints = PS->GetPlayerSpellPoints();
	}
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->OnAbilityStatusChanged.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				UpdateButtonStatus();
			}
			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
				Info.StatusTag = StatusTag;
				Info.AbilityLevel = AbilityLevel;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraASC()->OnAbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);

	if (AAuraPlayerState* PS = GetAuraPS())
	{
		PS->OnPlayerSpellPointsChanged.AddLambda(
			[this](int32 NewPoints)
			{
				CurrentSpellPoints = NewPoints;
				OnPlayerSpellPointsChanged.Broadcast(NewPoints);
				UpdateButtonStatus();
			});
	}
}

void USpellMenuWidgetController::SpellGlobleSlected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityType;
		OnStopWaitingForEquipSelection.Broadcast(AbilityType);
		bWaitingForEquipSelection = false;
	}

	CurrentSpellPoints = GetAuraPS()->GetPlayerSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_None);
	const FGameplayAbilitySpec* Spec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValid = Spec != nullptr;

	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = FAuraGameplayTags::Get().Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*Spec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	UpdateButtonStatus();
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
		OnStopWaitingForEquipSelection.Broadcast(AbilityType);
		bWaitingForEquipSelection = false;
	}
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;
	OnSpellGlobeSelected.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	OnWaitForEquipSelection.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;

	const FGameplayTag SelectedStatus = GetAuraASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		SelectedSlot = GetAuraASC()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection)
	{
		return;
	}

	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType))
	{
		return;
	}

	GetAuraASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void USpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 Spellpoints, bool& bSpendPointButtonEnable, bool& bEquipButtonEnabled)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bSpendPointButtonEnable = false;

	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bEquipButtonEnabled = true;
		bSpendPointButtonEnable = Spellpoints > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		bEquipButtonEnabled = false;
		bSpendPointButtonEnable = Spellpoints > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bEquipButtonEnabled = true;
		bSpendPointButtonEnable = Spellpoints > 0;
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Locked))
	{
		bEquipButtonEnabled = false;
	}
}

void USpellMenuWidgetController::UpdateButtonStatus()
{
	bool bSpendPointButtonEnable = false;
	bool bEquipButtonEnabled = false;
	FString Description;
	FString NextLevelDescription;

	ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bSpendPointButtonEnable, bEquipButtonEnabled);
	GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
	OnSpellGlobeSelected.Broadcast(bSpendPointButtonEnable, bEquipButtonEnabled, Description, NextLevelDescription);
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& SlotTag, const FGameplayTag& PreSlotTag)
{
	bWaitingForEquipSelection = false;
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	// Clear prevous ability widget info
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreSlotTag;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = SlotTag;
	AbilityInfoDelegate.Broadcast(Info);

	OnStopWaitingForEquipSelection.Broadcast(Info.AbilityType);
	OnSpellGlobeReassigned.Broadcast(AbilityTag);
	
	GlobeDeselect();
}
