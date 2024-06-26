// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AuraAbilitySystemLibrary.generated.h"

class UOverlayAuraWidgetController;
class UAttributeMenuWidgetController;
class USpellMenuWidgetController;
class UCharacterClassInfo;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
/**
 *
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayAuraWidgetController* GetOverlayAuraWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|CharacterClassesDefaults")
	static void InitDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|CharacterClassesDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|CharacterClassesDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library|Enemy")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, float CharacterLevel);
};
