// Copyright BanMing

#pragma once

#include "AuraGameplayTags.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"

#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpendPointButtonEnable, bool, bEquipButtonEnabled, FString, Description, FString, NextLevelDescription);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbialityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbialityTag);

struct FSelectedAbility
{
	FGameplayTag Ability = FAuraGameplayTags::Get().Abilities_None;

	FGameplayTag Status = FAuraGameplayTags::Get().Abilities_Status_Unlocked;
};
/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	UFUNCTION(BlueprintCallable)
	void SpellGlobleSlected(const FGameplayTag& AbialityTag);

	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();

	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 Spellpoints, bool& bSpendPointButtonEnable, bool& bEquipButtonEnabled);
	void UpdateButtonStatus();
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& SlotTag, const FGameplayTag& PreSlotTag);

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FOnPlayerStatChangeSignature OnPlayerSpellPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FSpellGlobeSelectedSignature OnSpellGlobeSelected;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FWaitForEquipSelectionSignature OnWaitForEquipSelection;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FWaitForEquipSelectionSignature OnStopWaitingForEquipSelection;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FSpellGlobeReassignedSignature OnSpellGlobeReassigned;

private:
	FSelectedAbility SelectedAbility;
	int32 CurrentSpellPoints = 0;
	bool bWaitingForEquipSelection = true;
	FGameplayTag SelectedSlot;
};
