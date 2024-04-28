#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"

#include "AuraAbilityTypes.generated.h"

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
		return NewContext;
	}

public:
	bool IsBlockedHit() const
	{
		return bIsBlockedHit;
	}

	bool IsCriticalHit() const
	{
		return bIsCriticalHit;
	}

	void SetIsBlockedHit(bool IsBlockedHit)
	{
		bIsBlockedHit = IsBlockedHit;
	}

	void SetIsCriticalHit(bool IsCriticalHit)
	{
		bIsCriticalHit = IsCriticalHit;
	}

protected:
	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;
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