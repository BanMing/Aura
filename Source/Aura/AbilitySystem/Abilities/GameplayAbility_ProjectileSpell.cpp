// Copyright BanMing

#include "GameplayAbility_ProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UGameplayAbility_ProjectileSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer)
	{
		return;
	}

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface != nullptr)
	{
		const FVector CombatSocketLocation = CombatInterface->GetCombatSocketLocation();

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatSocketLocation);
		// TODO: Set the projectile rotation

		AAuraProjectile* Projectile =
			GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// TODO: Get projectile a Gameplay Effect Spec for causing damage.
		Projectile->FinishSpawning(SpawnTransform);
	}
}
