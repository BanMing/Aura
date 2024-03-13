// Copyright BanMing

#include "AbilitySystem/Tasks/AbilityTask_TargetDataUnderMouse.h"

#include "GameFramework/PlayerController.h"

UAbilityTask_TargetDataUnderMouse* UAbilityTask_TargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UAbilityTask_TargetDataUnderMouse* Res = NewAbilityTask<UAbilityTask_TargetDataUnderMouse>(OwningAbility);
	return Res;
}

void UAbilityTask_TargetDataUnderMouse::Activate()
{
	Super::Activate();

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	ValidDateEvent.Broadcast(CursorHit.Location);
	// if (AAuraCharacter* Character = Cast<AAuraCharacter>(GetAvatarActor()))
	//{
	//	AAuraPlayerController* PC = Cast<AAuraPlayerController>(Character->GetController());
	//	const FHitResult& CursorHit = PC->GetCursorHit();
	//	if (CursorHit.bBlockingHit)
	//	{
	//		ValidDateEvent.Broadcast(CursorHit.ImpactPoint);
	//		DrawDebugSphere(GetWorld(), CursorHit.ImpactPoint, 8.f, 8, FColor::Green, false, 5.f);
	//	}
	// }
}
