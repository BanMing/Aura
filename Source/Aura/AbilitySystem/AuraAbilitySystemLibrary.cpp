// Copyright BanMing

#include "Aura/AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Abilities/GameplayAbility_DamageBase.h"
#include "AbilitySystem/AuraAbilityTypes.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/Player/AuraPlayerState.h"
#include "Aura/UI/HUD/AuraHUD.h"
#include "Aura/UI/WidgetController/AttributeMenuWidgetController.h"
#include "Aura/UI/WidgetController/AuraWidgetController.h"
#include "Aura/UI/WidgetController/OverlayAuraWidgetController.h"
#include "AuraGameplayTags.h"
#include "Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			OutParams.AbilitySystemComponent = PS->GetAbilitySystemComponent();
			OutParams.AttributeSet = PS->GetAttributeSet();
			OutParams.PlayerController = PC;
			OutParams.PlayerState = PS;
			OutAuraHUD = AuraHUD;

			return true;
		}
	}

	return false;
}

UOverlayAuraWidgetController* UAuraAbilitySystemLibrary::GetOverlayAuraWidgetController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams WidgetControllerParams;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams WidgetControllerParams;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
	{
		return AuraHUD->GetAttributeMenuController(WidgetControllerParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuController(const UObject* WorldContextObject)
{
	AAuraHUD* AuraHUD = nullptr;
	FWidgetControllerParams WidgetControllerParams;
	if (MakeWidgetControllerParams(WorldContextObject, WidgetControllerParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WidgetControllerParams);
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return;
	}
	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(ASC->GetAvatarActor());

	const FGameplayEffectSpecHandle PrimarySpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttribute, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimarySpecHandle.Data.Get());

	const FGameplayEffectSpecHandle SecondarySpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttribute, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondarySpecHandle.Data.Get());

	const FGameplayEffectSpecHandle VitalSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttribute, Level, EffectContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	if (UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject))
	{
		const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
		for (TSubclassOf<UGameplayAbility> Ability : Info.StartupAbilities)
		{
			if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
				ASC->GiveAbility(AbilitySpec);
			}
		}

		for (TSubclassOf<UGameplayAbility> Ability : CharacterClassInfo->CommonAbilities)
		{
			if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
				ASC->GiveAbility(AbilitySpec);
			}
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr)
	{
		return nullptr;
	}

	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr)
	{
		return nullptr;
	}

	return AuraGameMode->AbilityInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetbIsBlockedHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetbIsCriticalHit();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetbIsSuccessfulDebuff();
	}
	return false;
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetbIsRadialDamage();
	}
	return false;
}

float UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.0f;
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDamage();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffDuration();
	}
	return 0.0f;
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDebuffFrequency();
	}
	return 0.0f;
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return *AuraEffectContext->GetDamageType();
	}
	return FGameplayTag();
}

FVector UAuraAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UAuraAbilitySystemLibrary::GetKnockbackImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->GetKnockbackImpulse();
	}
	return FVector::ZeroVector;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsBlockedHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetbIsBlockedHit(bIsBlockedHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetbIsCriticalHit(bIsCriticalHit);
	}
}

void UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bIsSuccessfulDebuff)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetbIsSuccessfulDebuff(bIsSuccessfulDebuff);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float DebuffDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDamage(DebuffDamage);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float DebuffDuration)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffDuration(DebuffDuration);
	}
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, float DebuffFrequency)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDebuffFrequency(DebuffFrequency);
	}
}

void UAuraAbilitySystemLibrary::SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& DamageType)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDamageType(DamageType, true);
	}
}

void UAuraAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& DeathImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetDeathImpulse(DeathImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetKnockbackImpulse(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& KnockbackImpulse)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetKnockbackImpulse(KnockbackImpulse);
	}
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool IsRadialDamage)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetbIsRadialDamage(IsRadialDamage);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float RadialDamageInnerRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageInnerRadius(RadialDamageInnerRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float RadialDamageOuterRadius)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOuterRadius(RadialDamageOuterRadius);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FVector& RadialDamageOrigin)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetRadialDamageOrigin(RadialDamageOrigin);
	}
}

void UAuraAbilitySystemLibrary::SetRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, const bool IsRadialDamage, float InnerRadius, float OuterRadius, FVector DamageOrigin)
{
	DamageEffectParams.bIsRadialDamage = IsRadialDamage;
	DamageEffectParams.RadialDamageInnerRadius = InnerRadius;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadius;
	DamageEffectParams.RadialDamageOrigin = DamageOrigin;
}

void UAuraAbilitySystemLibrary::SetKnockbackDirectionEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	Magnitude = Magnitude == 0.f ? DamageEffectParams.KnockbackMagnitude : Magnitude;
	DamageEffectParams.KnockbackImpulse = KnockbackDirection.GetSafeNormal() * Magnitude;
}

void UAuraAbilitySystemLibrary::SetDeathImpulseDirectionEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector DeathImpulseDirection, float Magnitude)
{
	Magnitude = Magnitude == 0.f ? DamageEffectParams.KnockbackMagnitude : Magnitude;
	DamageEffectParams.DeathImpulse = DeathImpulseDirection.GetSafeNormal() * Magnitude;
}

void UAuraAbilitySystemLibrary::SetTargetASCEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, AActor* Target)
{
	DamageEffectParams.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;	   //(SCENE_QUERY_STAT(GetLivePlayersWithinRadius), false, nullptr);

	SphereParams.AddIgnoredActors(ActorsToIgnore);

	// query scene to see what we hit
	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UAuraAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosetTarget, const FVector& Origin)
{
	if (MaxTargets >= Actors.Num())
	{
		OutClosetTarget = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0)
		{
			break;
		}

		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* Actor : ActorsToCheck)
		{
			const double DistSquared = FVector::DistSquared(Origin, Actor->GetActorLocation());
			if (DistSquared < ClosestDistance)
			{
				ClosestDistance = DistSquared;
				ClosestActor = Actor;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosetTarget.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bFirstIsPlayer = FirstActor->ActorHasTag("Player");
	const bool bSecondIsPlayer = SecondActor->ActorHasTag("Player");
	const bool bFirstIsEnemy = FirstActor->ActorHasTag("Enemy");
	const bool bSecondIsEnemy = SecondActor->ActorHasTag("Enemy");

	return !(bFirstIsPlayer && bSecondIsPlayer || bFirstIsEnemy && bSecondIsEnemy);
}

int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, float CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return 0;
	}
	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = ClassDefaultInfo.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

FGameplayEffectSpecHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& Params)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	const AActor* SourceAvatarActor = Params.SourceASC->GetAvatarActor();

	FGameplayEffectContextHandle EffectContextHandle = Params.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	SetDeathImpulse(EffectContextHandle, Params.DeathImpulse);
	SetKnockbackImpulse(EffectContextHandle, Params.KnockbackImpulse);

	SetIsRadialDamage(EffectContextHandle, Params.bIsRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, Params.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, Params.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, Params.RadialDamageOrigin);

	const FGameplayEffectSpecHandle EffectSpecHandle = Params.SourceASC->MakeOutgoingSpec(Params.DamageGameplayEffectClass, Params.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, Params.DamageType, Params.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Chance, Params.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Frequency, Params.DebuffFrequency);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Damage, Params.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, GameplayTags.Debuff_Duration, Params.DebuffDuration);

	Params.TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data);
	return EffectSpecHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Res;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Res.Add(Direction.Rotation());
		}
	}
	else
	{
		Res.Add(Forward.Rotation());
	}

	return Res;
}

TArray<FVector> UAuraAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Res;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, Axis);
			Res.Add(Direction);
		}
	}
	else
	{
		Res.Add(Forward);
	}

	return Res;
}
