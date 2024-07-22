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

FDamageEffectParams UGameplayAbility_DamageBase::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
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

	if (IsValid(TargetActor) && FMath::RandRange(1, 100) < KnockbackChance)
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockbackImpulse = ToTarget * KnockbackMagnitude;
	}
	return Params;
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
