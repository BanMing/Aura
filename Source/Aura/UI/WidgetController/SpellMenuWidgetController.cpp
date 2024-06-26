// Copyright BanMing

#include "UI/WidgetController/SpellMenuWidgetController.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	OnInitializeStartupAbilities(GetAuraASC());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	Super::BindCallbacksToDependencies();
}