// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_FireBolt.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"

FString UGameplayAbility_FireBolt::GetDescription(int32 Level) const
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
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
			Level, ManaCost, Cooldown, Damage);
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
			Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), Damage);
	}
}

FString UGameplayAbility_FireBolt::GetNextLevelDescription(int32 Level) const
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n "
								"<Small>Level:</> <Level>%d</>\n"
								"<Default>Launches %d bolts of fire, "
								"exploding on impact and dealing:</><Damage>%d</>"
								"<Default> fire damage with a chance to burn</>\n\n "),
		Level, FMath::Min(Level, NumProjectiles), Damage);
}