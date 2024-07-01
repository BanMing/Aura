// Copyright BanMing

#include "Aura/AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "Aura/AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level) const
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), *GetName(), Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level) const
{
	return FString::Printf(TEXT("<Default>Next Level:%s, </><Level>%d</>\n <Default>Causes much more damage. </>"), *GetName(), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level:</><Level>%d</>"), Level);
}

float UAuraGameplayAbility::GetManaCost(int32 Level) const
{
	float ManaCost = 0.f;
	if (UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (const FGameplayModifierInfo& Modifier : CostEffect->Modifiers)
		{
			if (Modifier.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(Level, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(int32 Level) const
{
	float Cooldown = 0.f;
	if (UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(Level, Cooldown);
	}
	return Cooldown;
}
