// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_DamageBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UGameplayAbility_DamageBase::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());

	// for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	//{
	//	const float DamageVal = Pair.Value.GetValueAtLevel(GetAbilityLevel());
	//	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, DamageVal);
	// }

	const float DamageVal = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, DamageVal);

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UGameplayAbility_DamageBase::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor, const FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockbackDirectionOveride,
	bool bOverrideDeathInpulse, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams Params;
	Params.WorldCOntextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceASC = GetAbilitySystemComponentFromActorInfo();

	Params.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackMagnitude = KnockbackMagnitude;
	Params.KnockbackChance = KnockbackChance;

	if (IsValid(TargetActor))
	{
		MakeImpulseParams(TargetActor, bOverridePitch, PitchOverride, bOverrideKnockbackDirection, KnockbackDirectionOveride, Params, bOverrideDeathInpulse, DeathImpulseDirectionOverride);
	}

	if (bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
	}

	return Params;
}

void UGameplayAbility_DamageBase::MakeImpulseParams(AActor* TargetActor, bool bOverridePitch, float PitchOverride, bool bOverrideKnockbackDirection, FVector& KnockbackDirectionOveride, FDamageEffectParams& Params,
	bool bOverrideDeathInpulse, FVector& DeathImpulseDirectionOverride) const
{
	FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	const FVector ToTarget = Rotation.Vector();

	if (FMath::RandRange(1, 100) < KnockbackChance)
	{
		if (bOverrideKnockbackDirection)
		{
			KnockbackDirectionOveride.Normalize();
			Params.KnockbackImpulse = KnockbackDirectionOveride * KnockbackMagnitude;
			if (bOverridePitch)
			{
				FRotator KnockbackRotation = KnockbackDirectionOveride.Rotation();
				KnockbackRotation.Pitch = PitchOverride;
				Params.KnockbackImpulse = KnockbackRotation.Vector() * KnockbackMagnitude;
			}
		}
		else
		{
			Params.KnockbackImpulse = ToTarget * KnockbackMagnitude;
		}
	}

	if (bOverrideDeathInpulse)
	{
		DeathImpulseDirectionOverride.Normalize();
		Params.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}
	else
	{
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
	}
}

const FTaggedMontage UGameplayAbility_DamageBase::GetRandomTaggedMontage() const
{
	// if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const TArray<FTaggedMontage>& TaggedMontages = ICombatInterface::Execute_GetAttackMontages(GetAvatarActorFromActorInfo());
		if (TaggedMontages.Num() > 0)
		{
			const int RandomIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
			return TaggedMontages[RandomIndex];
		}
	}
	return FTaggedMontage();
}

float UGameplayAbility_DamageBase::GetDamageByDamageType(int32 Level, const FGameplayTag& InDamageType) const
{
	checkf(DamageTypes.Contains(InDamageType), TEXT("GameplayAbility %s does not contain DamageType %s"), *GetNameSafe(this), *InDamageType.ToString());
	return DamageTypes[InDamageType].GetValueAtLevel(Level);
}

float UGameplayAbility_DamageBase::GetDamage(int32 Level) const
{
	return Damage.GetValueAtLevel(Level);
}

float UGameplayAbility_DamageBase::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}
