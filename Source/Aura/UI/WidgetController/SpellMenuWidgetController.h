// Copyright BanMing

#pragma once

#include "AuraGameplayTags.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"

#include "SpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, bool, bSpendPointButtonEnable, bool, bEquipButtonEnabled);

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

private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 Spellpoints, bool& bSpendPointButtonEnable, bool& bEquipButtonEnabled);
	void UpdateButtonStatus();

public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FOnPlayerStatChangeSignature OnPlayerSpellPointsChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Player Stat")
	FSpellGlobeSelectedSignature OnSpellGlobleSelected;

private:
	FSelectedAbility SelectedAbility;
	int32 CurrentSpellPoints = 0;
};
