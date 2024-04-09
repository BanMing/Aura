// Copyright BanMing

#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Aura/Player/AuraPlayerState.h"
#include "Aura/UI/HUD/AuraHUD.h"
#include "Aura/UI/WidgetController/AuraWidgetController.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"

UOverlayAuraWidgetController* UAuraAbilitySystemLibrary::GetOverlayAuraWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

			return AuraHUD->GetAttributeMenuController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr)
	{
		return;
	}

	FCharacterClassDefaultInfo ClassDefaultInfo = AuraGameMode->CharacterClassInfo->GetCharacterCalssInfo(CharacterClass);

	FGameplayEffectContextHandle EffectComtextHandle = ASC->MakeEffectContext();
	EffectComtextHandle.AddSourceObject(ASC->GetAvatarActor());

	const FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttribute, Level, EffectComtextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->SecondaryAttribute, Level, EffectComtextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->VitalAttribute, Level, EffectComtextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}
