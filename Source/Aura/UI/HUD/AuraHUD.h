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
	UOverlayAuraWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet);

	UAttributeMenuWidgetController* GetAttributeMenuController(const FWidgetControllerParams& WCParams);

private:
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayAuraWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayAuraWidgetController> OverlayAuraWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
