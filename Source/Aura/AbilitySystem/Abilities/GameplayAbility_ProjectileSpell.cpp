// Copyright BanMing

#include "GameplayAbility_ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectTypes.h"
#include "Interaction/CombatInterface.h"

void UGameplayAbility_ProjectileSpell::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGameplayAbility_ProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, bool bOverridePitch, float PitchOverride)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	SpawnProjectileByTag(ProjectileTargetLocation, GameplayTags.CombatSocket_Weapon, bOverridePitch, PitchOverride);
}

void UGameplayAbility_ProjectileSpell::SpawnProjectileByTag(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}
	const FVector CombatSocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - CombatSocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CombatSocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AAuraProjectile* Projectile =
		GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	// EffectContextHandle.SetAbility(this);
	// EffectContextHandle.AddSourceObject(Projectile);
	// TArray<TWeakObjectPtr<AActor>> Actors;
	// Actors.Add(Projectile);
	// EffectContextHandle.AddActors(Actors);
	// FHitResult HitResult;
	// HitResult.Location = ProjectileTargetLocation;
	// EffectContextHandle.AddHitResult(HitResult);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	for (auto& Pair : DamageTypes)
	{
		const float DamageVal = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, DamageVal);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);
}
