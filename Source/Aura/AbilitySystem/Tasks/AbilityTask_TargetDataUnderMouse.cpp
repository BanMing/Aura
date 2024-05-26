// Copyright BanMing

#include "AbilitySystem/Tasks/AbilityTask_TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameplayAbilitySpec.h"

UAbilityTask_TargetDataUnderMouse* UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UAbilityTask_TargetDataUnderMouse* Res = NewAbilityTask<UAbilityTask_TargetDataUnderMouse>(OwningAbility);
	return Res;
}

void UAbilityTask_TargetDataUnderMouse::Activate()
{
	Super::Activate();

	bool IsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (IsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		// Server side

		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_TargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(CursorHit);
	FGameplayAbilityTargetDataHandle DataHandle(TargetData);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(), DataHandle, FGameplayTag(), AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidDataEvent.Broadcast(DataHandle);
	}
}

void UAbilityTask_TargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivateTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidDataEvent.Broadcast(DataHandle);
	}
}
