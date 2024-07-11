// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_FireBolt.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectTypes.h"

FString UGameplayAbility_FireBolt::GetDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n"
									"<Small>Level:</> <Level>%d</>\n"
									"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
									"<Small>Cooldown:</> <Cooldown>%.1f</>\n\n"
									"<Default>Launches a bolt of fire, "
									"exploding on impact and dealing:</>"
									"<Damage>%d</>"
									"<Default> fire damage with a chance to burn</>\n\n"),
			Level, ManaCost, Cooldown, ScaleDamage);
	}
	else
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n"
									"<Small>Level:</> <Level>%d</>\n"
									"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
									"<Small>Cooldown:</> <Cooldown>%.1f</>\n\n"
									"<Default>Launches %d bolts of fire, "
									"exploding on impact and dealing:</>"
									"<Damage>%d</>"
									"<Default> fire damage with a chance to burn</>\n\n"),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumProjectiles), ScaleDamage);
	}
}

FString UGameplayAbility_FireBolt::GetNextLevelDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n "
								"<Small>Level:</> <Level>%d</>\n"
								"<Default>Launches %d bolts of fire, "
								"exploding on impact and dealing:</><Damage>%d</>"
								"<Default> fire damage with a chance to burn</>\n\n "),
		Level, FMath::Min(Level, MaxNumProjectiles), ScaleDamage);
}

void UGameplayAbility_FireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
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
	const int32 NumProjectiles = FMath::Min(GetAbilityLevel(), MaxNumProjectiles);
	const FVector Forward = Rotation.Vector();
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	for (const FRotator& RotationDir : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(CombatSocketLocation);
		SpawnTransform.SetRotation(RotationDir.Quaternion());

		AAuraProjectile* Projectile =
			GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		Projectile->FinishSpawning(SpawnTransform);
	}
}
