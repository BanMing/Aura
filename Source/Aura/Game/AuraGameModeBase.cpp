// Copyright BanMing

#include "Aura/Game/AuraGameModeBase.h"

#include "Kismet/GameplayStatics.h"
#include "LoadScreenSaveGame.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->GetLoadSlotName(), SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->GetLoadSlotName(), SlotIndex);
	}
	if (ULoadScreenSaveGame* SaveGameObject = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass)))
	{
		SaveGameObject->PlayerName = LoadSlot->GetPlayerName();
		SaveGameObject->SlotIndex = SlotIndex;
		SaveGameObject->SlotName = LoadSlot->GetLoadSlotName();
		UGameplayStatics::SaveGameToSlot(SaveGameObject, LoadSlot->GetLoadSlotName(), SlotIndex);
	}
}
