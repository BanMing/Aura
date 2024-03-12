// Copyright BanMing

#include "Aura/Character/AuraCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* AAuraCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	check(Weapon);
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> EffectAttribute, float Level) const
{
	UAbilitySystemComponent* TargetASC = GetAbilitySystemComponent();
	check(IsValid(TargetASC));
	check(EffectAttribute);

	FGameplayEffectContextHandle EffectHandle = TargetASC->MakeEffectContext();
	EffectHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectAttribute, Level, EffectHandle);
	TargetASC->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(), TargetASC);
}

void AAuraCharacterBase::AddCharacterAbilities()
{
	if (!HasAuthority())
	{
		return;
	}

	UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	ASC->AddCharacterAbilities(StartupAbilities);
}
