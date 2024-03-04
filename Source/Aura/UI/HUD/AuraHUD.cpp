// Copyright BanMing

#include "Aura/UI/HUD/AuraHUD.h"

UOverlayAuraWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayAuraWidgetController>(this, OverlayAuraWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}

	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayAuraWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	UOverlayAuraWidgetController* Controller = GetOverlayWidgetController(WCParams);

	OverlayWidget->SetWidgetController(Controller);
	Controller->BroadcastInitialValues();

	OverlayWidget->AddToViewport();
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuController == nullptr)
	{
		AttributeMenuController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuController->SetWidgetControllerParams(WCParams);
		AttributeMenuController->BindCallbacksToDependencies();
	}

	return AttributeMenuController;
}