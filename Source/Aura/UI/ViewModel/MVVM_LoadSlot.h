// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "Game/LoadScreenSaveGame.h"
#include "MVVMViewModelBase.h"

#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnable);

/**
 *
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void InitializeSlot();

public:
	FString GetLoadSlotName() const;
	void SetLoadSlotName(FString InLoadSlotName);

	int32 GetSlotIndex() const;
	void SetSlotIndex(int32 InSlotIndex);

	FString GetPlayerName() const;
	void SetPlayerName(FString InPlayerName);

	FString GetMapName() const;
	void SetMapName(FString InMapName);

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex OnSetWidgetSwitcherIndex;

	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton OnEnableSelectSlotButton;

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true")) FString LoadSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName = FString("Default Name");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString MapName = FString();
};
