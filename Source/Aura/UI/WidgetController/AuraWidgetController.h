// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "AuraWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);

class UAttributeSet;
class UAbilitySystemComponent;
class APlayerController;
class APlayerState;
class UAuraAttributeSet;
class AAuraPlayerState;
class AAuraPlayerController;
class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
	{
	}

	FWidgetControllerParams(APlayerController* PlayerController, APlayerState* PlayerState, UAbilitySystemComponent* AbilitySystemComponent, UAttributeSet* AttributeSet)
		: PlayerController(PlayerController), PlayerState(PlayerState), AbilitySystemComponent(AbilitySystemComponent), AttributeSet(AttributeSet)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

/**
 *
 */
UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);

	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();
	UFUNCTION(BlueprintCallable)
	virtual void BindCallbacksToDependencies();

protected:
	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintPure)
	AAuraPlayerController* GetAuraPC();
	UFUNCTION(BlueprintPure)
	AAuraPlayerState* GetAuraPS();
	UFUNCTION(BlueprintPure)
	UAuraAbilitySystemComponent* GetAuraASC();
	UFUNCTION(BlueprintPure)
	UAuraAttributeSet* GetAuraAttributeSet();

protected:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Ability")
	FAbilityInfoSignature AbilityInfoDelegate;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<class UAbilityInfo> AbilityInfo;

private:
	TObjectPtr<AAuraPlayerController> AuraPC;
	TObjectPtr<AAuraPlayerState> AuraPS;
	TObjectPtr<UAuraAbilitySystemComponent> AuraASC;
	TObjectPtr<UAuraAttributeSet> AuraAttributeSet;
};
