// Copyright BanMing

#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 WidgetSwitcherIndex = SlotStatus.GetValue();
	OnSetWidgetSwitcherIndex.Broadcast(WidgetSwitcherIndex);
}

FString UMVVM_LoadSlot::GetLoadSlotName() const
{
	return LoadSlotName;
}

void UMVVM_LoadSlot::SetLoadSlotName(FString InLoadSlotName)
{
	UE_MVVM_SET_PROPERTY_VALUE(LoadSlotName, InLoadSlotName);
}

int32 UMVVM_LoadSlot::GetSlotIndex() const
{
	return SlotIndex;
}

void UMVVM_LoadSlot::SetSlotIndex(int32 InSlotIndex)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotIndex, InSlotIndex);
}

FString UMVVM_LoadSlot::GetPlayerName() const
{
	return PlayerName;
}

void UMVVM_LoadSlot::SetPlayerName(FString InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}

FString UMVVM_LoadSlot::GetMapName() const
{
	return MapName;
}

void UMVVM_LoadSlot::SetMapName(FString InMapName)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName, InMapName);
}
