// Copyright BanMing

#include "AbilitySystem/Abilities/GameplayAbility_DamageBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UGameplayAbility_DamageBase::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());

	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	{
		const float DamageVal = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, DamageVal);
	}

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
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
