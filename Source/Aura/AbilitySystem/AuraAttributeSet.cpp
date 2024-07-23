// Copyright BanMing

#include "Aura/AbilitySystem/AuraAttributeSet.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/AuraGameplayTags.h"
#include "Aura/Interaction/EnemyInterface.h"
#include "AuraAbilityTypes.h"
#include "GameFramework/Character.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "GameplayEffectExtension.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Fire, GetResistanceFireAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Lightning, GetResistanceLightningAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Arcane, GetResistanceArcaneAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resistance_Physical, GetResistancePhysicalAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, InComingDamge, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, InComingXP, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ResistanceFire, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ResistanceLightning, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ResistanceArcane, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ResistancePhysical, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	// if (Attribute == GetHealthAttribute())
	//{
	//	float CurrentValue = Health.GetCurrentValue();
	//	float BaseValue = Health.GetBaseValue();
	//	int a = 0;
	// }
}

void UAuraAttributeSet::PostAttributeBaseChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) const
{
	Super::PostAttributeBaseChange(Attribute, OldValue, NewValue);
	// if (Attribute == GetHealthAttribute())
	//{
	//	float CurrentValue = Health.GetCurrentValue();
	//	float BaseValue = Health.GetBaseValue();
	//	int a = 0;
	// }
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Do some check for attributes to make the value valid
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetMaxHealthAttribute() && bTopOffHealth)
	{
		// float CurrentValue = Health.GetCurrentValue();
		// float BaseValue = Health.GetBaseValue();
		// int a = 0;
		bTopOffHealth = false;
		SetHealth(GetMaxHealth());
	}
	else if (Attribute == GetMaxManaAttribute() && bTopOffMana)
	{
		bTopOffMana = false;
		SetMana(GetMaxMana());
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)

	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	// We can get many useful info from Data
	// Source = causer of the effect, Target = target of the effect (owner of this AS)
	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Props.TargetCharacter->Implements<UCombatInterface>() && ICombatInterface::Execute_IsDead(Props.TargetCharacter))
	{
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// float CurrentValue = Health.GetCurrentValue();
		// float BaseValue = Health.GetBaseValue();
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetInComingDamgeAttribute())
	{
		HandleInComingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetInComingXPAttribute())
	{
		HandleInComingXP(Props);
	}
}

void UAuraAttributeSet::HandleInComingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamge = GetInComingDamge();
	SetInComingDamge(0.f);
	if (LocalIncomingDamge > 0.f)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamge;
		SetHealth(FMath::Clamp(NewHealth, 0.f, GetMaxHealth()));

		const bool bFatal = NewHealth < 0.f;
		if (bFatal)
		{
			const FVector DeathImpulse = UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			CombatInterface->Die(DeathImpulse);
			SendXPEvent(Props);
		}
		else
		{
			if (Props.TargetAvatarActor->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetAvatarActor))
			{
				FGameplayTagContainer GameplayTagContainer;
				GameplayTagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(GameplayTagContainer);
			}

			const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackImpulse(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce, true, true);
			}
		}

		ShowFloatingText(Props, LocalIncomingDamge, UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle), UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle));

		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			HandleDebuff(Props);
		}
	}
}

void UAuraAttributeSet::HandleDebuff(const FEffectProperties& Props)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	const FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuffTypes[DamageType];
	UTargetTagsGameplayEffectComponent& TargetTagsGEComp = Effect->AddComponent<UTargetTagsGameplayEffectComponent>();
	FInheritedTagContainer TagContainerMods;
	TagContainerMods.Added.AddTag(DebuffTag);
	if (DebuffTag.MatchesTagExact(GameplayTags.Debuff_Stun))
	{
		TagContainerMods.Added.AddTag(GameplayTags.Player_Block_InputCursorTrace);
		TagContainerMods.Added.AddTag(GameplayTags.Player_Block_InputHeld);
		TagContainerMods.Added.AddTag(GameplayTags.Player_Block_InputPressed);
		TagContainerMods.Added.AddTag(GameplayTags.Player_Block_InputReleased);
	}

	TargetTagsGEComp.SetAndApplyTargetTagChanges(TagContainerMods);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAuraAttributeSet::GetInComingDamgeAttribute();

	if (FGameplayEffectSpec* MutalbeSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutalbeSpec->GetContext().Get());
		AuraContext->SetDamageType(DamageType, true);
		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutalbeSpec);
	}
}

void UAuraAttributeSet::HandleInComingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetInComingXP();
	SetInComingXP(0.f);

	// Source Character is the ower, since GA_ListenForEvents applies GE_EventBaseEffect, adding to IncomingXP
	if (Props.SourceCharacter->Implements<UCombatInterface>() || Props.SourceCharacter->Implements<UPlayerInterface>())
	{
		const int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurrentXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurrentXP + LocalIncomingXP);
		const int NumLevelUps = NewLevel - CurrentLevel;

		if (NumLevelUps > 0)
		{
			IPlayerInterface::Execute_AddPlayerLevel(Props.SourceCharacter, NumLevelUps);
			int32 AttributePointsReward = 0;
			int32 SpellPointsReward = 0;

			for (int32 i = 0; i < NumLevelUps; ++i)
			{
				SpellPointsReward += IPlayerInterface::Execute_GetSpellPointReward(Props.SourceCharacter, CurrentLevel + i);
				AttributePointsReward += IPlayerInterface::Execute_GetAttributePointReward(Props.SourceCharacter, CurrentLevel + i);
			}

			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);

			bTopOffHealth = true;
			bTopOffMana = true;

			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}

		IPlayerInterface::Execute_AddXP(Props.SourceCharacter, LocalIncomingXP);
	}
}

void UAuraAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bIsBlockedHit, bool bIsCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		// Player attacks enemy
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bIsBlockedHit, bIsCriticalHit);
			return;
		}

		// Enemy attacks player
		if (AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bIsBlockedHit, bIsCriticalHit);
		}
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	int32 Level = 1;
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		Level = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
	}

	if (IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(Props.TargetCharacter))
	{
		const ECharacterClass CharacterClass = IEnemyInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		const int32 RewardXP = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter, CharacterClass, Level);

		FGameplayEventData Payload;
		Payload.EventTag = FAuraGameplayTags::Get().Attributes_Meta_InComingXP;
		Payload.EventMagnitude = RewardXP;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter, Payload.EventTag, Payload);
	}
}

#pragma region Primary Attributes Replication Methods
void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Resilience, OldResilience);
}
void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Vigor, OldVigor);
}

#pragma endregion

#pragma region Secondary Attributes

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Armor, OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ArmorPenetration, OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BlockChance, OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitChance, OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitDamage, OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, CriticalHitResistance, OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, HealthRegeneration, OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ManaRegeneration, OldManaRegeneration);
}

#pragma endregion

#pragma region Vital Attributes Replication Methods
void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldMaxMana);
}
#pragma endregion

#pragma region Meta Attributes
void UAuraAttributeSet::OnRep_InComingDamge(const FGameplayAttributeData& OldInComingDamge)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, InComingDamge, OldInComingDamge);
}

void UAuraAttributeSet::OnRep_InComingXP(const FGameplayAttributeData& OldInComingXP)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, InComingXP, OldInComingXP);
}
#pragma endregion

#pragma region Resistance Attributes

void UAuraAttributeSet::OnRep_ResistanceFire(const FGameplayAttributeData& OldResistanceFire)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ResistanceFire, OldResistanceFire);
}

void UAuraAttributeSet::OnRep_ResistanceLightning(const FGameplayAttributeData& OldResistanceLightning)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ResistanceLightning, OldResistanceLightning);
}

void UAuraAttributeSet::OnRep_ResistanceArcane(const FGameplayAttributeData& OldResistanceArcane)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ResistanceArcane, OldResistanceArcane);
}

void UAuraAttributeSet::OnRep_ResistancePhysical(const FGameplayAttributeData& OldResistancePhysical)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ResistancePhysical, OldResistancePhysical);
}

#pragma endregion
