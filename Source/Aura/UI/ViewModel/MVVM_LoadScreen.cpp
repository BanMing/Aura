// Copyright BanMing

#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Aura/Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitilalizeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_0->SetLoadSlotName("LoadSLot_0");
	LoadSlot_0->SetSlotIndex(0);
	LoadSlots.Add(0, LoadSlot_0);

	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_1->SetLoadSlotName("LoadSLot_1");
	LoadSlot_1->SetSlotIndex(1);
	LoadSlots.Add(1, LoadSlot_1);

	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotViewModelClass);
	LoadSlot_2->SetLoadSlotName("LoadSLot_2");
	LoadSlot_2->SetSlotIndex(2);
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotViewModelByIndex(int32 index)
{
	return LoadSlots.FindChecked(index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnterName)
{
	const FString& PlayerName = EnterName.IsEmpty() ? "Default" : EnterName;
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[Slot]->SetPlayerName(PlayerName);
	LoadSlots[Slot]->SlotStatus = Taken;
	LoadSlots[Slot]->SetSlotIndex(Slot);
	LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
	AuraGameMode->SaveSlotData(LoadSlots[Slot], Slot);

	LoadSlots[Slot]->InitializeSlot();
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->OnSetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(int32 Slot)
{
	OnSlotSelected.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		LoadSlot.Value->OnEnableSelectSlotButton.Broadcast(Slot != LoadSlot.Key);
	}
	SelectedSlot = LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	if (IsValid(SelectedSlot))
	{
		AAuraGameModeBase::DeleteSlot(SelectedSlot->GetLoadSlotName(), SelectedSlot->GetSlotIndex());
		SelectedSlot->SlotStatus = Vacant;
		SelectedSlot->InitializeSlot();
		SelectedSlot->OnEnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->GetLoadSlotName(), LoadSlot.Key);
		LoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
		LoadSlot.Value->SlotStatus = SaveObject->SlotStatus;
		LoadSlot.Value->SetSlotIndex(LoadSlot.Key);
		LoadSlot.Value->SetMapName(SaveObject->MapName);
		LoadSlot.Value->InitializeSlot();
	}
}
