// Copyright BanMing

#include "Aura/Game/AuraGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "LoadScreenSaveGame.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	DeleteSlot(LoadSlot->GetLoadSlotName(), SlotIndex);

	if (ULoadScreenSaveGame* SaveGameObject = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass)))
	{
		SaveGameObject->PlayerName = LoadSlot->GetPlayerName();
		SaveGameObject->SlotIndex = SlotIndex;
		SaveGameObject->SlotName = LoadSlot->GetLoadSlotName();
		SaveGameObject->MapName = LoadSlot->GetMapName();
		SaveGameObject->SlotStatus = Taken;
		UGameplayStatics::SaveGameToSlot(SaveGameObject, LoadSlot->GetLoadSlotName(), SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}

	return Cast<ULoadScreenSaveGame>(SaveGameObject);
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
}
