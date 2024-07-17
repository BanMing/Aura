// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_BeamSpell.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UGameplayAbility_BeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UGameplayAbility_BeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UGameplayAbility_BeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwerCharacter);
	if (OwerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwerCharacter))
		{
			const FVector SocketLocation = Weapon->GetSocketLocation("TipSocket");
			TArray<AActor*> ActorsToIgnore;
			FHitResult OutHit;
			UKismetSystemLibrary::SphereTraceSingle(OwerCharacter, SocketLocation, BeamTargetLocation, 10, TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, OutHit, true);

			if (OutHit.bBlockingHit)
			{
				MouseHitActor = OutHit.GetActor();
				MouseHitLocation = OutHit.ImpactPoint;
			}
		}
	}
}
