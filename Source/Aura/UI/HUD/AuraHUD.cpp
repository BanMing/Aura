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
	checkf(OverlayWigetClass, TEXT("Overlay Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(OverlayAuraWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWigetClass);
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

UUserWidget* AAuraHUD::InitAttributeMenu(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
{
	checkf(AttributeWigetClass, TEXT("Attribute Menu Widget Class uninitialized, please fill out BP_AuraHUD"));
	checkf(AttributeMenuWidgetControllerClass, TEXT("Attribute Menu Widget Controller Class uninitialized, please fill out BP_AuraHUD"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), AttributeWigetClass);
	UAuraUserWidget* AttributeMenuWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WCParams(PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
	UAttributeMenuWidgetController* Controller = GetAttributeMenuController(WCParams);

	AttributeMenuWidget->SetWidgetController(Controller);

	Controller->BroadcastInitialValues();
	
	AttributeMenuWidget->AddToViewport();

	return Widget;
}
