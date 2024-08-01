// Copyright BanMing

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"

#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitcherIndex);

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

public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex OnSetWidgetSwitcherIndex;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString LoadSlotName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	int32 SlotIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, FieldNotify, Setter, Getter, meta = (AllowPrivateAccess = "true"))
	FString PlayerName = FString("Default Name");
};
