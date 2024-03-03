// Copyright BanMing

#pragma once

#include "Aura/UI/Widget/AuraUserWidget.h"
#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"
#include "Aura/UI/WidgetController/OverlayAuraWidgetController.h"
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "AuraHUD.generated.h"

/**
 *
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UOverlayAuraWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

public:
	UAttributeMenuWidgetController* GetAttributeMenuController(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	UUserWidget* InitAttributeMenu(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWigetClass;

	UPROPERTY()
	TObjectPtr<UOverlayAuraWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayAuraWidgetController> OverlayAuraWidgetControllerClass;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> AttributeWigetClass;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
