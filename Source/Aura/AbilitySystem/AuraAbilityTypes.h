#pragma once

#include "CoreMinimal.h"
#include "Engine/NetSerialization.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Templates/SubclassOf.h"

#include "AuraAbilityTypes.generated.h"
class UGameplayEffect;
class UAbilitySystemComponent;

#define EFFECT_CONTEXT_ACCESSORS(PropertyType, PropertyName)          \
	FORCEINLINE PropertyType Get##PropertyName() const                \
	{                                                                 \
		return this->PropertyName;                                    \
	}                                                                 \
                                                                      \
	FORCEINLINE void Set##PropertyName(PropertyType In##PropertyName) \
	{                                                                 \
		this->PropertyName = In##PropertyName;                        \
	}

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams()
	{
	}

	UPROPERTY()
	TObjectPtr<UObject> WorldCOntextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASC = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASC = nullptr;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	int32 AbilityLevel = 1;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FAuraGameplayEffectContext::StaticStruct();
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FGameplayEffectContext* Duplicate() const override
	{
		FGameplayEffectContext* NewContext = new FGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		if (GetDamageType())
		{
			// NewContext->SetDamageType()
		}
		return NewContext;
	}

public:
	EFFECT_CONTEXT_ACCESSORS(bool, bIsBlockedHit)
	EFFECT_CONTEXT_ACCESSORS(bool, bIsCriticalHit)
	EFFECT_CONTEXT_ACCESSORS(bool, bIsSuccessfulDebuff)
	EFFECT_CONTEXT_ACCESSORS(float, DebuffDamage)
	EFFECT_CONTEXT_ACCESSORS(float, DebuffDuration)
	EFFECT_CONTEXT_ACCESSORS(float, DebuffFrequency)

	FORCEINLINE TSharedPtr<FGameplayTag> GetDamageType() const
	{
		return this->DamageType;
	}

	void SetDamageType(const FGameplayTag& InDamageType, bool bReset = false);

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = -1.f;

	UPROPERTY()
	float DebuffDuration = -1.f;

	UPROPERTY()
	float DebuffFrequency = -1.f;

	TSharedPtr<FGameplayTag> DamageType;
};

template <>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true	   // Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};