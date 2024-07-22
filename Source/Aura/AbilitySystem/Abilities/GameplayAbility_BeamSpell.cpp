// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_BeamSpell.h"

#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"
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

				if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
				{
					if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::PrimaryTargetDied))
					{
						CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::PrimaryTargetDied);
					}
				}
			}
		}
	}
}

void UGameplayAbility_BeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets)
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(MouseHitActor);
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	TArray<AActor*> OutOverlappingActors;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OutOverlappingActors, ActorsToIgnore, EffectRadius, MouseHitLocation);

	int32 NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);

	UAuraAbilitySystemLibrary::GetClosestTargets(NumAdditionalTargets, OutOverlappingActors, OutAdditionalTargets, MouseHitLocation);

	for (AActor* Target : OutAdditionalTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::AdditionalTargetDied);
			}
		}
	}
}
