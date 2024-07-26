// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_ArcaneShards.h"

FString UGameplayAbility_ArcaneShards::GetDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>ARCANE SHARDS</>\n\n"
									"<Small>Level:</> <Level>%d</>\n"
									"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
									"<Small>Cooldown:</> <Cooldown>%.1f</>\n\n"
									"<Default>Launches an arcane shard, "
									"exploding on impact and dealing:</>"
									"<Damage>%d</>"
									"<Default> arcane damage with a chance to burn</>\n\n"),
			Level, ManaCost, Cooldown, ScaleDamage);
	}
	else
	{
		return FString::Printf(TEXT("<Title>ARCANE SHARDS</>\n\n"
									"<Small>Level:</> <Level>%d</>\n"
									"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
									"<Small>Cooldown:</> <Cooldown>%.1f</>\n\n"
									"<Default>Launches %d arcane shards, "
									"exploding on impact and dealing:</>"
									"<Damage>%d</>"
									"<Default> arcane damage with a chance to burn</>\n\n"),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumArcaneShards), ScaleDamage);
	}
}

FString UGameplayAbility_ArcaneShards::GetNextLevelDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n "
								"<Small>Level:</> <Level>%d</>\n"
								"<Default>Launches %d arcane shards, "
								"exploding on impact and dealing:</><Damage>%d</>"
								"<Default> arcane damage with a chance to burn</>\n\n "),
		Level, FMath::Min(Level, MaxNumArcaneShards), ScaleDamage);
}
