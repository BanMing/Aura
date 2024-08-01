// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "AuraGameModeBase.generated.h"

class UCharacterClassInfo;
class UAbilityInfo;
class UMVVM_LoadSlot;
class USaveGame;
	/**
 *
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Characrer Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;
};
