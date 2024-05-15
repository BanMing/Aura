// Copyright BanMing

#include "Aura/Character/AuraCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura.h"
#include "Aura/AbilitySystem/AuraAbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

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

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& Montage)
{
	check(Weapon);

	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if (Montage.MatchesTagExact(GameplayTags.Montage_Attack_Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	else if (Montage.MatchesTagExact(GameplayTags.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandTipSocketName);
	}
	else if (Montage.MatchesTagExact(GameplayTags.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandTipSocketName);
	}

	return FVector::ZeroVector;
}

UAnimMontage* AAuraCharacterBase::GetHitReactAnimMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

const TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	return AttackMontages;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	// Set character rogdoll
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
	bDead = true;
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

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}
