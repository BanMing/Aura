// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_Electrocutes.h"

FString UGameplayAbility_Electrocutes::GetDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>ELECTROCUTE</>\n\n"
									"<Small>Level:</> <Level>%d</>\n"
									"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
									"<Small>Cooldown:</> <Cooldown>%.1f</>\n\n"
									"<Default>Launches a electrocute of lighting, "
									"exploding on impact and dealing:</>"
									"<Damage>%d</>"
									"<Default> lighting damage with a chance to burn</>\n\n"),
			Level, ManaCost, Cooldown, ScaleDamage);
	}
	else
	{
		return FString::Printf(TEXT("<Title>ELECTROCUTE</>\n\n"
									"<Small>Level:</> <Level>%d</>\n"
									"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
									"<Small>Cooldown:</> <Cooldown>%.1f</>\n\n"
									"<Default>Launches %d electrocutes of lighting, "
									"exploding on impact and dealing:</>"
									"<Damage>%d</>"
									"<Default> lighting damage with a chance to burn</>\n\n"),
			Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets), ScaleDamage);
	}
}

FString UGameplayAbility_Electrocutes::GetNextLevelDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n "
								"<Small>Level:</> <Level>%d</>\n"
								"<Default>Launches %d electrocutes of lighting, "
								"exploding on impact and dealing:</><Damage>%d</>"
								"<Default> lighting damage with a chance to burn</>\n\n "),
		Level, FMath::Min(Level, MaxNumShockTargets), ScaleDamage);
}
