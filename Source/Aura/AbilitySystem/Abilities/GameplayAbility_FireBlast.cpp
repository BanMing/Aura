// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_FireBlast.h"

FString UGameplayAbility_FireBlast::GetDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT("<Title>FIRE BLAST</>\n\n"
								"<Small>Level:</> <Level>%d</>\n"
								"<Small>ManaCost:</> <ManaCost>%.1f</>\n"
								"<Small>Cooldown:</> <Cooldown>%.1f</>\n\n"
								"<Default>Launches %d bolts of fire, "
								"exploding on impact and dealing:</>"
								"<Damage>%d</>"
								"<Default>radial fire damage with a chance to burn</>\n\n"),
		Level, ManaCost, Cooldown, NumBlasts, ScaleDamage);
}

FString UGameplayAbility_FireBlast::GetNextLevelDescription(int32 Level) const
{
	const int32 ScaleDamage = GetDamage(Level);
	return FString::Printf(TEXT("<Title>NEXT LEVEL</>\n\n "
								"<Small>Level:</> <Level>%d</>\n"
								"<Default>Launches %d bolts of fire, "
								"exploding on impact and dealing:</><Damage>%d</>"
								"<Default>radial fire damage with a chance to burn</>\n\n "),
		Level, NumBlasts, ScaleDamage);
}
