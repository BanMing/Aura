// Copyright BanMing

#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Engine/CurveTable.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	const ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceActor);
	const ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetActor);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	FGameplayEffectContextHandle GameplayEffectContextHandle = Spec.GetContext();

	// Get Damage Set by Caller Magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	// Capture BlockChance on Target, and determine if there was a successful Block
	float BlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, BlockChance);
	BlockChance = FMath::Max<float>(BlockChance, 0.f);
	const bool bBlocked = FMath::RandRange(1, 100) < BlockChance;
	// If Block, halve the damage.
	Damage = bBlocked ? Damage / 2 : Damage;

	UAuraAbilitySystemLibrary::SetIsCriticalHit(GameplayEffectContextHandle, bBlocked);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceActor);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalcCoefficients->FindCurve("ArmorPenetration", FString());
	const float ArmorPenetrationCoefficients = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);
	// ArmorPenetration ignores a percentage of the Target's Armor
	const float EffectiveArmor = TargetArmor *= (kMaxAttributeAmount - SourceArmorPenetration * ArmorPenetrationCoefficients) / kMaxAttributeAmount;

	// Armor ignores a percentage of incoming Damage
	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalcCoefficients->FindCurve("EffectiveArmor", FString());
	const float EffectiveArmorCoefficients = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	Damage *= (kMaxAttributeAmount - EffectiveArmor * EffectiveArmorCoefficients) / 100.f;

	// Critical hit damage
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0.f);

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalcCoefficients->FindCurve("CriticalHitResistance", FString());
	const float CriticalHitResistanceCoefficients = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficients;
	bool bIsCriticalHit = FMath::RandRange(0, 100) < EffectiveCriticalHitChance;
	if (bIsCriticalHit)
	{
		float SourceCriticalHitDamage = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParameters, SourceCriticalHitDamage);
		SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);
		// Double damage plus a bonus if critical hit
		Damage = Damage * 2.f + SourceCriticalHitDamage;
	}
	UAuraAbilitySystemLibrary::SetIsCriticalHit(GameplayEffectContextHandle, bIsCriticalHit);

	FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetInComingDamgeAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
